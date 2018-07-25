#include "login_server.hpp"
#include "log.hpp"
#include "user/user_manager.hpp"
#include "user/user_msg_handler.hpp"
#include "gate/gate_manager.hpp"
#include "gate/gate_msg_handler.hpp"
#include "common/redis_client.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

login_server_t::login_server_t()
{
}

login_server_t::~login_server_t()
{
}

bool login_server_t::on_init_server()
{
    CMD_INIT;

    m_state = LOGIN_SERVER_STATE_START;

    // xml
    if (!load_xml())
    {
        close_xml();
        COMMON_ASSERT(false);
        return false;
    }

    // log
    if (!g_log.open(env::xml->log_path, server_name(), env::xml->is_log_trace, env::xml->is_log_debug, env::xml->is_log_info, env::xml->is_new_logfile_per_hour))
    {
        COMMON_ASSERT(false);
        return false;
    }

    // zdb
    if (!init_zdb())
    {
        COMMON_ASSERT(false);
        return false;
    }

    // redis
    if (!init_redis())
    {
        COMMON_ASSERT(false);
        return false;
    }

    // network
    if (!init_network())
    {
        COMMON_ASSERT(false);
        return false;
    }

    start_server_run_timer();

    return true;
}

bool login_server_t::on_close_server()
{
    {
        cancel_server_run_timer();
        
    }

    close_network();

    close_redis();

    close_zdb();

    close_xml();

    g_log.close();

    CMD_CLOSE;

    return true;
}

void login_server_t::on_run_server()
{
}

//----------------------------------------------------------------------------------
bool login_server_t::load_xml()
{
    if (!load_server_xml())
    {
        printf("\033[31mload [%s] failed!\033[0m\n", SERVER_XML_PATH);
        return false;
    }
    printf("load [%s] success!\n", SERVER_XML_PATH);

    return true;
}
void login_server_t::close_xml()
{
    SAFE_DELETE_P(env::xml);
    log_info("close xml success!");
}
bool login_server_t::load_server_xml()
{
    try
    {
        boost::property_tree::ptree pt;
        boost::property_tree::xml_parser::read_xml(SERVER_XML_PATH, pt);

        uint32_t platform = pt.get<uint32_t>("server.platform");
        if (platform > PLATFORM_MAX)
        {
            log_error("server.platform[%d] > platform_max[%d]", platform, PLATFORM_MAX);
            return false;
        }
        uint32_t srv_group_id = pt.get<uint32_t>("server.server_id");
        if (srv_group_id > SERVER_GROUP_ID_MAX)
        {
            log_error("server.server_id[%d] > server_id_max[%d]", srv_group_id, SERVER_GROUP_ID_MAX);
            return false;
        }

        env::xml = new login_xml_t;

        env::xml->srv_group.platform = platform;
        env::xml->srv_group.group_id = srv_group_id;
        env::xml->srv_group.group_name = pt.get<std::string>("server.server_name");

        env::xml->user_num_max = pt.get<uint32_t>("server.login_server.user_num_max");
        env::xml->login_queue_timer = pt.get<uint32_t>("server.login_server.login_queue_timer");
        env::xml->login_queue_process = pt.get<uint32_t>("server.login_server.login_queue_process");
        env::xml->ip_access = pt.get<uint32_t>("server.login_server.ip_access");
        env::xml->timeout.session = pt.get<uint32_t>("server.login_server.timeout.session");

        env::xml->thread_num.network = pt.get<uint32_t>("server.login_server.thread_num.network");

        env::xml->listen_at_client.ip = pt.get<std::string>("server.login_server.listen.client.ip");
        env::xml->listen_at_client.port = pt.get<uint32_t>("server.login_server.listen.client.port");
        CHECK_PORT(env::xml->listen_at_client.port);

        env::xml->listen_at_client.out_ip = pt.get<std::string>("server.login_server.listen.client.out_ip");
        env::xml->listen_at_client.out_port = pt.get<uint32_t>("server.login_server.listen.client.out_port");
        CHECK_PORT(env::xml->listen_at_client.out_port);

        env::xml->listen_at_gate.ip = pt.get<std::string>("server.login_server.listen.gate.ip");
        env::xml->listen_at_gate.port = pt.get<uint32_t>("server.login_server.listen.gate.port");
        CHECK_PORT(env::xml->listen_at_gate.port);

        env::xml->connect_to_account.ip = pt.get<std::string>("server.login_server.connect.account.ip");
        env::xml->connect_to_account.port = pt.get<uint32_t>("server.login_server.connect.account.port");
        CHECK_PORT(env::xml->connect_to_account.port);

        env::xml->mysql.host = pt.get<std::string>("server.login_server.mysql.host");
        env::xml->mysql.db = pt.get<std::string>("server.login_server.mysql.db");
        env::xml->mysql.user = pt.get<std::string>("server.login_server.mysql.user");
        env::xml->mysql.pwd = pt.get<std::string>("server.login_server.mysql.pwd");
        env::xml->mysql.init_conn = pt.get<uint32_t>("server.login_server.mysql.init_connections");
        env::xml->mysql.max_conn = pt.get<uint32_t>("server.login_server.mysql.max_connections");

        env::xml->redis.host = pt.get<std::string>("server.login_server.redis.host");
        env::xml->redis.port = pt.get<uint32_t>("server.login_server.redis.port");
        env::xml->redis.pwd = pt.get<std::string>("server.login_server.redis.pwd");
        env::xml->redis.db = pt.get<uint32_t>("server.login_server.redis.db");
        env::xml->redis.save_interval = pt.get<uint32_t>("server.login_server.redis.save_interval");
        env::xml->redis.save_count_limit = pt.get<uint32_t>("server.login_server.redis.save_count_limit");

        std::string str_ip_white_list = pt.get<std::string>("server.common.ip_white_list");
        string_util_t::split(str_ip_white_list, env::xml->ip_white_list, ";");
        if (env::xml->ip_white_list.empty())
        {
            printf("\033[31mip_white_list is empty!\033[0m\n");
            return false;
        }

        env::xml->log_path = pt.get<std::string>("server.common.log_path");
        env::xml->is_log_trace = pt.get<bool>("server.common.log_trace");
        env::xml->is_log_debug = pt.get<bool>("server.common.log_debug");
		env::xml->is_log_info = pt.get<bool>("server.common.log_info");
		env::xml->is_log_msg_count = pt.get<bool>("server.common.log_msg_count");
        env::xml->is_new_logfile_per_hour = pt.get<bool>("server.common.new_logfile_per_hour");
        env::xml->reconnect_interval_time = pt.get<uint32_t>("server.common.reconnect_interval_time");
		env::xml->can_create_new_role = pt.get<uint32_t>("server.can_create_new_role");
    }
    catch (boost::property_tree::ptree_error& e)
    {
        printf("\033[31mthrow error[%s]\033[0m\n", e.what());
        return false;
    }

    if (NULL == env::xml)
    {
        printf("\033[31menv::xml is NULL!\033[0m\n");
        return false;
    }

    return true;
}

//----------------------------------------------------------------------------------
bool login_server_t::init_network()
{
    // network
    if (!m_network.init(env::xml->thread_num.network, env::xml->ip_white_list))
    {
        log_error("init network failed!");
        return false;
    }

    // listen for gate
    if (!m_network.listen_at(env::xml->listen_at_gate.ip, env::xml->listen_at_gate.port, &m_gate_listener))
    {
        return false;
    }
    else
    {
        if (!gate_msg_handler_t::init_msg_handler())
        {
            log_error("init gate msg handler failed!");
            return false;
        }
    }

    // listen for client
    if (!m_network.listen_at(env::xml->listen_at_client.ip, env::xml->listen_at_client.port, &m_user_listener))
    {
        return false;
    }
    else
    {
        if (!user_msg_handler_t::init_msg_handler())
        {
            log_error("init client msg handler failed!");
            return false;
        }
    }

    // connect to account
    if (!m_network.connect_to(env::xml->connect_to_account.ip, env::xml->connect_to_account.port, &m_account_connector, env::xml->reconnect_interval_time))
    {
        return false;
    }

    return true;
}
void login_server_t::close_network()
{
	msg_count_t::write_msg_recv_count_data_file(env::xml->is_log_msg_count);
	msg_count_t::write_msg_send_count_data_file(env::xml->is_log_msg_count);

    user_manager_t::clear_client();
    gate_manager_t::clear_gate();
    m_network.close();
}

//----------------------------------------------------------------------------------
bool login_server_t::init_zdb()
{
    boost::tie(m_db_url, env::conn_zdb_pool) = zdb_util_t::open_zdb(
        env::xml->mysql.host, 
        env::xml->mysql.db, 
        env::xml->mysql.user, 
        env::xml->mysql.pwd, 
        env::xml->mysql.max_conn,
        env::xml->mysql.init_conn);
    if (NULL == env::conn_zdb_pool)
    {
        log_error("init zdb connection pool failed!");
        return false;
    }

    return true;
}
void login_server_t::close_zdb()
{
    zdb_util_t::close_zdb(env::conn_zdb_pool, m_db_url);
}

//----------------------------------------------------------------------------------
bool login_server_t::init_redis()
{
    redis_client_t::simple_table_field_list tables;
    tables.insert(std::make_pair("account_ban", ""));
    tables.insert(std::make_pair("ip_access", ""));

	if (g_redis_save.init(env::xml->redis.host,
		env::xml->redis.port,
		env::xml->redis.pwd.c_str(),
		env::xml->redis.db,
		env::xml->redis.save_interval,
		env::xml->redis.save_count_limit,
		env::conn_zdb_pool))
	{
		log_error("init redis_save client failed!");
		return false;

	}
    if (!redis_client_t::init(env::xml->redis.host,
        env::xml->redis.port,
        env::xml->redis.pwd.c_str(),
        env::xml->redis.db,
        env::xml->redis.save_interval,
        env::xml->redis.save_count_limit,
        env::server->get_ios(),
        env::conn_zdb_pool,
        tables))
    {
        log_error("init redis failed!");
        return false;
    }

    return true;
}
void login_server_t::close_redis()
{
	g_redis_save.close(env::conn_zdb_pool);
    redis_client_t::close(env::conn_zdb_pool);
}

//----------------------------------------------------------------------------------
void login_server_t::start_server_run_timer()
{
    m_server_run_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
    m_server_run_timer->expires_from_now(boost::posix_time::seconds(10));
    m_server_run_timer->async_wait(boost::bind(&login_server_t::server_run_timeout_callback, this, boost::asio::placeholders::error));
}

void login_server_t::server_run_timeout_callback(const boost::system::error_code& ec)
{
    if (ec == boost::asio::error::operation_aborted)
    {
        
        return;
    }
    else if (ec)
    {
        log_error("Timer error: %s", ec.message().c_str());
        return;
    }

    m_state = LOGIN_SERVER_STATE_RUNNING;
    log_info("login server running...");
}

void login_server_t::cancel_server_run_timer()
{
    if (NULL != m_server_run_timer)
    {
        m_server_run_timer->cancel();
    }
}

//-------------------------------------------------------------------------------------------
void login_server_t::on_add_client(const tcp_socket_ptr& s)
{
    user_manager_t::add_socket(s);
}
void login_server_t::on_del_client(const tcp_socket_ptr& s)
{
    user_manager_t::del_socket(s);
}
void login_server_t::on_client_msg(const tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const msg_buf_ptr& msg_buf)
{
    if (m_state != LOGIN_SERVER_STATE_RUNNING)
    {
        log_error("login server is not open!");
        return;
	}
	if (NULL == s)
	{
		log_error("NULL == s");
		return;
	}

	socket_data_t* p_socket_data = (socket_data_t*)s->get_bind_data();
	if (NULL == p_socket_data)
	{
		log_error("NULL == p_socket_data");
		return;
	}
	// 检查是否为消息轰炸
	if (!p_socket_data->check_msg_bomb(cmd))
	{
		log_error("msg cmd[%s:%d] bomb, and kick the user[%lu] !!!!!!!!!!", CMD_DESC(cmd), cmd, uid);

		user_manager_t::del_socket(s);

		return;
	}
	msg_count_t::push_msg_recv_info(msg_buf, cmd, env::xml->is_log_msg_count);
    user_msg_handler_t::m_user_msg_handle.handle_client_msg(s,uid, cmd, msg_buf);
}

//-------------------------------------------------------------------------------------------
void login_server_t::on_add_gate(const tcp_socket_ptr& s)
{
    gate_manager_t::add_gate(s);
}
void login_server_t::on_del_gate(const tcp_socket_ptr& s)
{
    gate_manager_t::del_gate(s);
}
void login_server_t::on_gate_msg(const tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const msg_buf_ptr& msg_buf)
{
	msg_count_t::push_msg_recv_info(msg_buf, cmd, env::xml->is_log_msg_count);
    gate_msg_handler_t::handle_server_msg(s, uid,cmd, msg_buf);
}

//-------------------------------------------------------------------------------------------
void login_server_t::on_add_account(const tcp_socket_ptr& s)
{
	m_account_connector.set_socket(s);

	proto::server::ln_run_server_notify ntf;
	ntf.set_server_id(env::xml->srv_group.group_id);
	ntf.set_login_ip(env::xml->listen_at_client.out_ip);
	ntf.set_login_port(env::xml->listen_at_client.out_port);
	ntf.set_server_name(env::xml->srv_group.group_name);
	ntf.set_can_create_new_role(env::xml->can_create_new_role);

	send_msg_to_account(op_cmd::ln_run_server_notify, ntf);
}
void login_server_t::on_del_account(const tcp_socket_ptr& s)
{
	env::server->post_network_close_socket(s);
	m_account_connector.close_socket();
}
void login_server_t::on_account_msg(uint64_t uid,uint16_t cmd, const msg_buf_ptr& msg_buf)
{
	
}