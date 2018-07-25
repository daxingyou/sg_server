#include "recharge_server.hpp"
#include "log.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

recharge_server_t::recharge_server_t()
{
}

recharge_server_t::~recharge_server_t()
{
}

bool recharge_server_t::on_init_server()
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

bool recharge_server_t::on_close_server()
{
	stop_network();

    close_network();

    close_zdb();

    close_xml();

    g_log.close();

    CMD_CLOSE;

    return true;
}

void recharge_server_t::on_run_server()
{
    log_info("account server running...");
}

bool recharge_server_t::load_server_xml()
{
	try
	{
		boost::property_tree::ptree pt;
		boost::property_tree::xml_parser::read_xml(SERVER_XML_PATH, pt);

		env::xml = new recharge_xml_t;

		env::xml->thread_num.network = pt.get<uint32_t>("server.recharge_server.thread_num.network");

		//listen center
		env::xml->listen_at_center.ip = pt.get<std::string>("server.recharge_server.center.listen.client.ip");
		env::xml->listen_at_center.port = pt.get<uint32_t>("server.recharge_server.center.listen.client.port");
		CHECK_PORT(env::xml->listen_at_center.port); 

		//connect log
		env::xml->connect_to_log.ip = pt.get<std::string>("server.log_server.listen.account.ip");
		env::xml->connect_to_log.port = pt.get<uint32_t>("server.log_server.listen.account.port");
		CHECK_PORT(env::xml->connect_to_log.port);

		env::xml->ip_access = pt.get<std::string>("server.recharge_server.ip_access");

		//white name 
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
		log_info("load version:%s", env::xml->version.c_str());
	}
	catch (boost::property_tree::ptree_error& e)
	{
		printf("\033[31mthrow error[%s]\033[0m\n", e.what());
		return false;
	}

	return true;
}

bool recharge_server_t::load_xml()
{
    if (!load_server_xml())
    {
        printf("\033[31mload [%s] failed!\033[0m\n", SERVER_XML_PATH);
        return false;
    }

    printf("load [%s] success!\n", SERVER_XML_PATH);

    return true;
}
void recharge_server_t::close_xml()
{
    SAFE_DELETE_P(env::xml);
    log_info("close xml success!");
}

//----------------------------------------------------------------------------------
bool recharge_server_t::init_zdb()
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
void recharge_server_t::close_zdb()
{
    zdb_util_t::close_zdb(env::conn_zdb_pool, m_db_url);
}

//-----------------------------------------------------------------
bool recharge_server_t::init_network()
{
    if (!m_network.init(env::xml->thread_num.network, env::xml->ip_white_list))
    {
        log_error("init network failed!");
        return false;
    }

    // listen at login
    if (!m_network.listen_at(env::xml->listen_at_center.ip, env::xml->listen_at_center.port, &m_center_listener))
    {
        return false;
    }
    else
    {
        if (!center_listener_t::init_msg_handler())
        {
            log_error("init login msg handler failed!");
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
void recharge_server_t::close_network()
{
	msg_count_t::write_msg_recv_count_data_file(env::xml->is_log_msg_count);
	msg_count_t::write_msg_send_count_data_file(env::xml->is_log_msg_count);

    login_manager_t::clear_login();

    m_network.close();
}

//--------------------------------------------------------------------
void recharge_server_t::on_add_center(const network::tcp_socket_ptr& s)
{
	m_center_listener.set_socket(s);
}
void recharge_server_t::on_del_center(const network::tcp_socket_ptr& s)
{
    env::server->post_network_close_socket(s);
    m_center_listener.close_socket();
}

//--------------------------------------------------------------------------------------------------
void recharge_server_t::on_add_log(const tcp_socket_ptr& s)
{
    m_log_connector.set_socket(s);
}
void recharge_server_t::on_del_log(const tcp_socket_ptr& s)
{
    env::server->post_network_close_socket(s);
    m_log_connector.close_socket();
}

void recharge_server_t::stop_network()
{
	m_network.stop();
}
