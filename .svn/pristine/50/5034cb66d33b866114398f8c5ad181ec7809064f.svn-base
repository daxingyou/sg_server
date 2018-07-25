#include "account_server.hpp"
#include "log.hpp"
#include "user/user_msg_handler.hpp"
#include "login/login_msg_handler.hpp"
#include "user/user_manager.hpp"
#include "login/login_manager.hpp"
#include "db/db_msg_handler.hpp"
#include "zdb_ex.hpp"
#include "channel/channel_manager.hpp"
#include "serverlist/server_info_manager.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

account_server_t::account_server_t()
{
}

account_server_t::~account_server_t()
{
}

bool account_server_t::on_init_server()
{
    CMD_INIT;

    // xml
    if (!load_xml())
    {
        close_xml();
        COMMON_ASSERT(false);
        return false;
    }


    // log
    if (!g_log.open(env::xml->log_path, server_name(), env::xml->is_log_trace, env::xml->is_log_debug, env::xml->is_log_info))
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

    // channel
    if (!open_channel())
    {
        close_channel();
        COMMON_ASSERT(false);
        return false;
    }

	//读取服务器列表信息
	if (!load_server_info())
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

    //uint32_t concurentThreadsSupported = boost::thread::hardware_concurrency();
    //log_warn("######### %d", concurentThreadsSupported);

    return true;
}

bool account_server_t::on_close_server()
{
	stop_network();

    close_channel();

    close_network();

    close_zdb();

    close_xml();

    g_log.close();

    CMD_CLOSE;

    return true;
}

void account_server_t::on_run_server()
{
    log_info("account server running...");
}

//------------------------------------------------------------------------
bool account_server_t::load_global_server_xml()
{
    try
    {
        boost::property_tree::ptree pt;
        boost::property_tree::xml_parser::read_xml(GLOBAL_SERVER_XML_PATH, pt);

        env::xml = new account_xml_t;

        env::xml->srv_group.platform = pt.get<uint32_t>("server.platform");

        std::string str_channel_list = pt.get<std::string>("server.channel");
        string_util_t::split(str_channel_list, env::xml->srv_group.channel_id_list, ";", false);
        if (env::xml->srv_group.channel_id_list.empty())
        {
            printf("\033[31menv::xml->srv_group.channel_id_list is empty!\033[0m\n");
            return false;
        }

        env::xml->timeout.session = pt.get<uint32_t>("server.account_server.timeout.session");
        env::xml->thread_num.network = pt.get<uint32_t>("server.account_server.thread_num.network");
        env::xml->thread_num.channel = pt.get<uint32_t>("server.account_server.thread_num.channel");

        env::xml->mysql.host = pt.get<std::string>("server.account_server.mysql.host");
        env::xml->mysql.db = pt.get<std::string>("server.account_server.mysql.db");
        env::xml->mysql.user = pt.get<std::string>("server.account_server.mysql.user");
        env::xml->mysql.pwd = pt.get<std::string>("server.account_server.mysql.pwd");
        env::xml->mysql.init_conn = pt.get<uint32_t>("server.account_server.mysql.init_connections");
        env::xml->mysql.max_conn = pt.get<uint32_t>("server.account_server.mysql.max_connections");

        char buf[256];
        //listen client
        snprintf(buf, sizeof(buf), "server.account_server.account%d.listen.client.ip", env::server->get_id());
        env::xml->listen_at_client.ip = pt.get<std::string>(buf);

        snprintf(buf, sizeof(buf), "server.account_server.account%d.listen.client.port", env::server->get_id());
        env::xml->listen_at_client.port = pt.get<uint32_t>(buf);
        CHECK_PORT(env::xml->listen_at_client.port);

        //listen login
        snprintf(buf, sizeof(buf), "server.account_server.account%d.listen.login.ip", env::server->get_id());
        env::xml->listen_at_login.ip = pt.get<std::string>(buf);

        snprintf(buf, sizeof(buf), "server.account_server.account%d.listen.login.port", env::server->get_id());
        env::xml->listen_at_login.port = pt.get<uint32_t>(buf);
        CHECK_PORT(env::xml->listen_at_login.port);

        //listen db
        snprintf(buf, sizeof(buf), "server.account_server.account%d.listen.db.ip", env::server->get_id());
        env::xml->listen_at_db.ip = pt.get<std::string>(buf);

        snprintf(buf, sizeof(buf), "server.account_server.account%d.listen.db.port", env::server->get_id());
        env::xml->listen_at_db.port = pt.get<uint32_t>(buf);
        CHECK_PORT(env::xml->listen_at_db.port);

        //connect log
        env::xml->connect_to_log.ip = pt.get<std::string>("server.glog_server.listen.account.ip");
        env::xml->connect_to_log.port = pt.get<uint32_t>("server.glog_server.listen.account.port");
        CHECK_PORT(env::xml->connect_to_log.port);

        std::string str_ip_white_list = pt.get<std::string>("server.common.ip_white_list");
        string_util_t::split(str_ip_white_list, env::xml->ip_white_list, ";");
        if (env::xml->ip_white_list.empty())
        {
            printf("\033[31mip_white_list is empty!\033[0m\n");
            return false;
        }

        env::xml->reconnect_interval_time = pt.get<uint32_t>("server.common.reconnect_interval_time"); 
        env::xml->db_log_count_limit = pt.get<uint32_t>("server.common.db_log_count_limit");

        env::xml->log_path = pt.get<std::string>("server.common.log_path");
        env::xml->is_log_trace = pt.get<bool>("server.common.log_trace");
        env::xml->is_log_debug = pt.get<bool>("server.common.log_debug");
		env::xml->is_log_info = pt.get<bool>("server.common.log_info");
		env::xml->is_log_msg_count = pt.get<bool>("server.common.log_msg_count");
        env::xml->is_new_logfile_per_hour = pt.get<bool>("server.common.new_logfile_per_hour");

		uint32_t role = pt.get<uint32_t>("server.common.log.role");
		env::xml->log_debug.role = role / 10;
		env::xml->log_sql.role = role % 10;
		
		//新增web_server_url ADD_BY_HY
        env::xml->url.login_list_url = pt.get<std::string>("server.url.login_list_url");
        env::xml->url.sdk_login_url = pt.get<std::string>("server.url.sdk_login_url");

		//服务器列表标记用来控制，是否验证登录服验证列表
		env::xml->check_login_server_flag = pt.get<uint32_t>("server.account_server.check_login_server_flag");

		//读取服务器标记列表	
        std::string str_server_list_type = pt.get<std::string>("server.server_list_type");
		string_util_t::split(str_server_list_type, server_info_manager_t::m_server_list_type_vec, ";", false);
		if (server_info_manager_t::m_server_list_type_vec.empty())
		{
			printf("\033[31m server.server_list_type is empty \033[0m\n");
			return false;
		}
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
bool account_server_t::load_channel_xml()
{
    try
    {
        if (env::xml->srv_group.channel_id_list.empty())
        {
            log_error("env::xml->srv_group.channel_id_list is empty!");
            return false;
        }

        boost::property_tree::ptree pt;
        boost::property_tree::xml_parser::read_xml(CHANNEL_XML_PATH, pt);

        for (uint32_t i = 0; i < env::xml->srv_group.channel_id_list.size(); ++i)
        {
            uint32_t channel_id_1 = env::xml->srv_group.channel_id_list[i];

            char buf[512] = { 0 };
            snprintf(buf, sizeof(buf), "channel.channel%d", channel_id_1);
            auto item = pt.get_child_optional(buf);
            if (!item)
            {
                continue;
            }

            std::string path(buf);
            
            uint32_t channel_id_2 = pt.get<uint32_t>(path + ".id");
            if (channel_id_2 > CHANNEL_ID_MAX)
            {
                log_error("channel_id[%d] > channel_id_max[%d]", channel_id_2, CHANNEL_ID_MAX);
                continue;
            }

            channel_ptr p_channel(new channel_t());
            p_channel->m_id = channel_id_2;
            p_channel->m_channel_name = pt.get<std::string>(path + ".channel_name");
            p_channel->m_login_url = env::xml->url.sdk_login_url;

            channel_manager_t::add_channel(p_channel);
        }
    }
    catch (boost::property_tree::ptree_error& e)
    {
        printf("\033[31mthrow error[%s]\033[0m\n", e.what());
        return false;
    }

    return true;
}
bool account_server_t::load_config_xml()
{
    if (env::server->get_id() > 0)
    {
        return true;
    }
    
    try
    {
        boost::property_tree::ptree pt;
        boost::property_tree::xml_parser::read_xml(ACCOUNT_XML_PATH, pt);
        uint32_t id = pt.get<uint32_t>("server.account_server.id");
        env::server->set_id(id);
    }
    catch (boost::property_tree::ptree_error& e)
    {
        printf("\033[31mthrow error[%s]\033[0m\n", e.what());
        return false;
    }

    return true;
}

bool account_server_t::load_server_xml()
{
	try
	{
		boost::property_tree::ptree pt;
		boost::property_tree::xml_parser::read_xml(SERVER_XML_PATH, pt);
		env::xml->version = pt.get<std::string>("server.version");
		log_info("load version:%s", env::xml->version.c_str());
	}
	catch (boost::property_tree::ptree_error& e)
	{
		printf("\033[31mthrow error[%s]\033[0m\n", e.what());
		return false;
	}

	return true;
}

bool account_server_t::load_xml()
{
    if (!load_config_xml())
    {
        printf("\033[31mload [%s] failed!\033[0m\n", ACCOUNT_XML_PATH);
        return false;
    }
    printf("load [%s] success!\n", ACCOUNT_XML_PATH);

    if (!load_global_server_xml())
    {
        printf("\033[31mload [%s] failed!\033[0m\n", GLOBAL_SERVER_XML_PATH);
        return false;
    }
    printf("load [%s] success!\n", GLOBAL_SERVER_XML_PATH);

    if (!load_channel_xml())
    {
        printf("\033[31mload [%s] failed!\033[0m\n", CHANNEL_XML_PATH);
        return false;
    }

    printf("load [%s] success!\n", CHANNEL_XML_PATH);

    if (!load_server_xml())
    {
        printf("\033[31mload [%s] failed!\033[0m\n", SERVER_XML_PATH);
        return false;
    }

    printf("load [%s] success!\n", SERVER_XML_PATH);

    return true;
}
void account_server_t::close_xml()
{
    SAFE_DELETE_P(env::xml);
    log_info("close xml success!");
}

//----------------------------------------------------------------------------------
bool account_server_t::open_channel()
{
	if (!channel_manager_t::open(env::xml->thread_num.channel))
	{
		return false;
	}

    log_info("init channel success!");
	return true;
}
void account_server_t::close_channel()
{
    channel_manager_t::close();
    log_info("close channel success!");
}

//----------------------------------------------------------------------------------
bool account_server_t::init_zdb()
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
void account_server_t::close_zdb()
{
    zdb_util_t::close_zdb(env::conn_zdb_pool, m_db_url);
}

//-----------------------------------------------------------------
bool account_server_t::init_network()
{
    if (!m_network.init(env::xml->thread_num.network, env::xml->ip_white_list))
    {
        log_error("init network failed!");
        return false;
    }

    // listen at client
    if (!m_network.listen_at(env::xml->listen_at_client.ip, env::xml->listen_at_client.port, &m_user_listener))
    {
        return false;
    }
    else
    {
        if (!user_msg_handler_t::init_msg_handler())
        {
            log_error("init user msg handler failed!");
            return false;
        }
    }

    // listen at login
    if (!m_network.listen_at(env::xml->listen_at_login.ip, env::xml->listen_at_login.port, &m_login_listener))
    {
        return false;
    }
    else
    {
        if (!login_msg_handler_t::init_msg_handler())
        {
            log_error("init login msg handler failed!");
            return false;
        }
    }

    // listen at db
    if (!m_network.listen_at(env::xml->listen_at_db.ip, env::xml->listen_at_db.port, &m_db_listener))
    {
        return false;
    }
    else
    {
        if (!db_msg_handler_t::init_msg_handler())
        {
            log_error("init db msg handler failed!");
            return false;
        }
    }

    // connect to log
    if (!m_network.connect_to(env::xml->connect_to_log.ip, env::xml->connect_to_log.port, &m_log_connector, env::xml->reconnect_interval_time))
    {
        return false;
    }

    return true;
}
void account_server_t::close_network()
{
	msg_count_t::write_msg_recv_count_data_file(env::xml->is_log_msg_count);
	msg_count_t::write_msg_send_count_data_file(env::xml->is_log_msg_count);

    login_manager_t::clear_login();

    m_network.close();
}

//--------------------------------------------------------------------
void account_server_t::on_add_client(const tcp_socket_ptr& s)
{
	user_manager_t::add_socket(s);
}
void account_server_t::on_del_client(const tcp_socket_ptr& s)
{
	user_manager_t::del_socket(s);
}
void account_server_t::on_client_msg(const tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const msg_buf_ptr& msg_buf)
{
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
	/* TODO 暂时注释
	if (!p_socket_data->check_msg_bomb(cmd))
	{
		log_error("msg cmd[%s:%d] bomb, and kick the user[%lu] !!!!!!!!!!", CMD_DESC(cmd), cmd, uid);
		user_manager_t::del_socket(s);
		return;
	}
	*/
	msg_count_t::push_msg_recv_info(msg_buf, cmd, env::xml->is_log_msg_count);
	user_msg_handler_t::m_user_msg_handle.handle_client_msg(s, uid, cmd, msg_buf);
}

//--------------------------------------------------------------------
void account_server_t::on_add_login(const tcp_socket_ptr& s)
{
}
void account_server_t::on_del_login(const tcp_socket_ptr& s)
{
	login_manager_t::del_login(s);
}
void account_server_t::on_login_msg(const tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const msg_buf_ptr& msg_buf)
{
	msg_count_t::push_msg_recv_info(msg_buf, cmd, env::xml->is_log_msg_count);
	login_msg_handler_t::handle_server_msg(s,uid, cmd, msg_buf);
}

//--------------------------------------------------------------------
void account_server_t::on_add_db(const tcp_socket_ptr& s)
{
    // 同步account信息给login
    proto::server::nd_syn_account_info_notify syn_msg;
    syn_msg.set_account_id(env::server->get_id());
    env::server->send_msg_to_db(s, op_cmd::nd_syn_account_info_notify, 0, syn_msg);
}
void account_server_t::on_del_db(const tcp_socket_ptr& s)
{
    env::server->post_network_close_socket(s);
}
void account_server_t::on_db_msg(const tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const msg_buf_ptr& msg_buf)
{
	msg_count_t::push_msg_recv_info(msg_buf, cmd, env::xml->is_log_msg_count);
	db_msg_handler_t::handle_server_msg(s, uid,cmd, msg_buf);
}

//--------------------------------------------------------------------------------------------------
void account_server_t::on_add_log(const tcp_socket_ptr& s)
{
    m_log_connector.set_socket(s);
}
void account_server_t::on_del_log(const tcp_socket_ptr& s)
{
    env::server->post_network_close_socket(s);
    m_log_connector.close_socket();
}

//读取服务器列表信息
bool account_server_t::load_server_info()
{
	return server_info_manager_t::load_server_info();
}

void account_server_t::stop_network()
{
	m_network.stop();
}


