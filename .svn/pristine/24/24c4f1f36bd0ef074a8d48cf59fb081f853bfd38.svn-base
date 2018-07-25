#include "log_server.hpp"
#include "log.hpp"
#include "log_server_fwd.hpp"
#include "log_manager.hpp"
#include "game/game_msg_handler.hpp"
#include "center/center_msg_handler.hpp"
#include "account/account_msg_handler.hpp"
#include "msg_count.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

log_server_t::log_server_t()
{
}

log_server_t::~log_server_t()
{
}

//----------------------------------------------------------------------------------
bool log_server_t::on_init_server()
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

    // log thread
    if (!log_manager_t::init(env::xml->thread_num.log, env::xml->update_to_db_time))
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

    return true;
}

bool log_server_t::on_close_server()
{
    {
        log_manager_t::close();
    }

    close_network();

    close_zdb();

    close_xml();

    g_log.close();

    CMD_CLOSE;

    return true;
}

void log_server_t::on_run_server()
{
    log_info("log server running...");
}

//---------------------------------------------------------------------------
bool log_server_t::load_xml()
{
    if (!load_config_xml())
    {
        printf("\033[31mload [%s] failed!\033[0m\n", GATE_XML_PATH);
        return false;
    }
    printf("load [%s] success!\n", GATE_XML_PATH);

    if (!load_global_server_xml())
    {
        printf("\033[31mload [%s] failed!\033[0m\n", GLOBAL_SERVER_XML_PATH);
        return false;
    }
    printf("load [%s] success!\n", GLOBAL_SERVER_XML_PATH);

    if (!load_server_xml())
    {
        printf("\033[31mload [%s] failed!\033[0m\n", SERVER_XML_PATH);
        return false;
    }
    printf("load [%s] success!\n", SERVER_XML_PATH);

    return true;
}
bool log_server_t::load_config_xml()
{
    try
    {
        boost::property_tree::ptree pt_config;
        boost::property_tree::xml_parser::read_xml(LOG_XML_PATH, pt_config);
        uint32_t type = pt_config.get<uint32_t>("server.log_server.type");
        env::server->set_type(type);
    }
    catch (boost::property_tree::ptree_error& e)
    {
        printf("\033[31mthrow error[%s]\033[0m\n", e.what());
        return false;
    }

    return true;
}
bool log_server_t::load_server_xml()
{
    if (env::server->get_type() != log_server_type_single)
    {
        return true;
    }
    try
    {
        boost::property_tree::ptree pt;
        boost::property_tree::xml_parser::read_xml(SERVER_XML_PATH, pt);

        env::xml = new log_xml_t;

        env::xml->thread_num.network = pt.get<uint32_t>("server.log_server.thread_num.network");
        env::xml->thread_num.log = pt.get<uint32_t>("server.log_server.thread_num.log");

        env::xml->mysql.host = pt.get<std::string>("server.log_server.mysql.host");
        env::xml->mysql.db = pt.get<std::string>("server.log_server.mysql.db");
        env::xml->mysql.user = pt.get<std::string>("server.log_server.mysql.user");
        env::xml->mysql.pwd = pt.get<std::string>("server.log_server.mysql.pwd");
        env::xml->mysql.init_conn = pt.get<uint32_t>("server.log_server.mysql.init_connections");
        env::xml->mysql.max_conn = pt.get<uint32_t>("server.log_server.mysql.max_connections");

        env::xml->listen_at_game.ip = pt.get<std::string>("server.log_server.listen.game.ip");
        env::xml->listen_at_game.port = pt.get<uint32_t>("server.log_server.listen.game.port");
        CHECK_PORT(env::xml->listen_at_game.port);

        env::xml->listen_at_center.ip = pt.get<std::string>("server.log_server.listen.center.ip");
        env::xml->listen_at_center.port = pt.get<uint32_t>("server.log_server.listen.center.port");
        CHECK_PORT(env::xml->listen_at_center.port);

        env::xml->update_to_db_time = pt.get<std::uint32_t>("server.log_server.update_to_db_time");

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
bool log_server_t::load_global_server_xml()
{
    if (env::server->get_type() != log_server_type_global)
    {
        return true;
    }
    try
    {
        boost::property_tree::ptree pt;
        boost::property_tree::xml_parser::read_xml(GLOBAL_SERVER_XML_PATH, pt);

        env::xml = new log_xml_t;

        env::xml->update_to_db_time = pt.get<std::uint32_t>("server.glog_server.update_to_db_time");

        env::xml->thread_num.network = pt.get<uint32_t>("server.glog_server.thread_num.network");
        env::xml->thread_num.log = pt.get<uint32_t>("server.glog_server.thread_num.log");

        env::xml->listen_at_account.ip = pt.get<std::string>("server.glog_server.listen.account.ip");
        env::xml->listen_at_account.port = pt.get<uint32_t>("server.glog_server.listen.account.port");
        CHECK_PORT(env::xml->listen_at_account.port);

        env::xml->mysql.host = pt.get<std::string>("server.glog_server.mysql.host");
        env::xml->mysql.db = pt.get<std::string>("server.glog_server.mysql.db");
        env::xml->mysql.user = pt.get<std::string>("server.glog_server.mysql.user");
        env::xml->mysql.pwd = pt.get<std::string>("server.glog_server.mysql.pwd");
        env::xml->mysql.init_conn = pt.get<uint32_t>("server.glog_server.mysql.init_connections");
        env::xml->mysql.max_conn = pt.get<uint32_t>("server.glog_server.mysql.max_connections");

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
    }
    catch (boost::property_tree::ptree_error& e)
    {
        printf("\033[31mthrow error[%s]\033[0m\n", e.what());
        return false;
    }

    return true;
}
void log_server_t::close_xml()
{
    SAFE_DELETE_P(env::xml);
    log_info("close xml success!");
}

//----------------------------------------------------------------------------------
bool log_server_t::init_network()
{
    if (!m_network.init(env::xml->thread_num.network, env::xml->ip_white_list))
    {
        log_error("init network failed!");
        return false;
    }

    if (env::server->get_type() == log_server_type_global)
    {
        // listen for account
        if (!m_network.listen_at(env::xml->listen_at_account.ip, env::xml->listen_at_account.port, &m_account_listener))
        {
            return false;
        }
        else
        {
            if (!account_msg_handler_t::init_msg_handler())
            {
                log_error("init account msg handler failed!");
                return false;
            }
        }
    }
    else if (env::server->get_type() == log_server_type_single)
    {
        // listen for game
        if (!m_network.listen_at(env::xml->listen_at_game.ip, env::xml->listen_at_game.port, &m_game_listener))
        {
            return false;
        }
        else
        {
            if (!game_msg_handler_t::init_msg_handler())
            {
                log_error("init game msg handler failed!");
                return false;
            }
        }

        // listen for center
        if (!m_network.listen_at(env::xml->listen_at_center.ip, env::xml->listen_at_center.port, &m_center_listener))
        {
            return false;
        }
        else
        {
            if (!center_msg_handler_t::init_msg_handler())
            {
                log_error("init center msg handler failed!");
                return false;
            }
        }
    }
    else
    {
        log_error("log server type[%d] error!", env::server->get_type());
        return false;
    }

    return true;
}

void log_server_t::close_network()
{
	msg_count_t::write_msg_recv_count_data_file(env::xml->is_log_msg_count);
	msg_count_t::write_msg_send_count_data_file(env::xml->is_log_msg_count);

    m_network.close();
}

//----------------------------------------------------------------------------------
bool log_server_t::init_zdb()
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
void log_server_t::close_zdb()
{
    zdb_util_t::close_zdb(env::conn_zdb_pool, m_db_url);
}

//-----------------------------------------------------------------------------------------------------------------
void log_server_t::on_add_game(const tcp_socket_ptr& s)
{
    m_game_listener.set_socket(s);
}
void log_server_t::on_del_game(const tcp_socket_ptr& s)
{
    env::server->post_network_close_socket(s);
    m_game_listener.close_socket();
}
void log_server_t::on_game_msg(const tcp_socket_ptr& s, uint16_t cmd, const msg_buf_ptr& msg_buf, uint64_t uid)
{
	msg_count_t::push_msg_recv_info(msg_buf, cmd, env::xml->is_log_msg_count);
    log_manager_t::post_game_message(s, cmd, msg_buf, uid);
}

//-----------------------------------------------------------------------------------------------------------------
void log_server_t::on_add_center(const tcp_socket_ptr& s)
{
    m_center_listener.set_socket(s);
}
void log_server_t::on_del_center(const tcp_socket_ptr& s)
{
    env::server->post_network_close_socket(s);
    m_center_listener.close_socket();
}
void log_server_t::on_center_msg(const tcp_socket_ptr& s, uint16_t cmd, const msg_buf_ptr& msg_buf, uint64_t uid)
{
	msg_count_t::push_msg_recv_info(msg_buf, cmd, env::xml->is_log_msg_count);
    log_manager_t::post_center_message(s, cmd, msg_buf, uid);
}

//-----------------------------------------------------------------------------------------------------------------
void log_server_t::on_add_account(const tcp_socket_ptr& s)
{
    m_account_listener.set_socket(s);
}
void log_server_t::on_del_account(const tcp_socket_ptr& s)
{
    env::server->post_network_close_socket(s);
    m_account_listener.close_socket();
}
void log_server_t::on_account_msg(const tcp_socket_ptr& s, uint16_t cmd, const msg_buf_ptr& msg_buf, uint64_t uid)
{
    msg_count_t::push_msg_recv_info(msg_buf, cmd, env::xml->is_log_msg_count);
    log_manager_t::post_account_message(s, cmd, msg_buf, uid);
}