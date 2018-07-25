#include "db_server.hpp"
#include "db_server_fwd.hpp"
#include "log.hpp"
#include "center/center_msg_handler.hpp"
#include "game/game_msg_handler.hpp"
#include "account/account_msg_handler.hpp"
#include "global_id.hpp"
#include "config/config_manager.hpp"
#include "common/redis_client.hpp"
#include "msg_count.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

db_server_t::db_server_t()
    : server_base_t()
{
}

db_server_t::~db_server_t()
{
}

bool db_server_t::on_init_server()
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

    // config
    if (!load_config())
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

    return true;
}

bool db_server_t::on_close_server()
{

	log_error("close start");
    {
		release_config();
		log_error("release_config");
    }
    
	close_network();
	log_error("close_network");

	close_redis();
	log_error("close_redis");

	close_zdb();
	log_error("close_zdb");

	close_xml();
	log_error("close_xml");

    g_log.close();

    CMD_CLOSE;

    return true;
}

void db_server_t::on_run_server()
{
    log_info("db server running...");
}

//---------------------------------------------------------------------------------------
bool db_server_t::load_xml()
{
    if (!load_server_xml())
    {
        printf("\033[31mload [%s] failed!\033[0m\n", SERVER_XML_PATH);
        return false;
    }
    printf("load [%s] success!\n", SERVER_XML_PATH);

    return true;
}
bool db_server_t::load_server_xml()
{
    try
    {
        boost::property_tree::ptree pt;
        boost::property_tree::xml_parser::read_xml(SERVER_XML_PATH, pt);

        env::xml = new db_xml_t;

        env::xml->srv_group.platform = pt.get<uint32_t>("server.platform");
        env::xml->srv_group.group_id = pt.get<uint32_t>("server.server_id");
        env::xml->srv_group.start_time = pt.get<std::string>("server.start_time");

        env::xml->thread_num.network = pt.get<uint32_t>("server.db_server.thread_num.network");

        env::xml->listen_at_game.ip = pt.get<std::string>("server.db_server.listen.game.ip");
        env::xml->listen_at_game.port = pt.get<uint32_t>("server.db_server.listen.game.port");
        CHECK_PORT(env::xml->listen_at_game.port);

        env::xml->listen_at_center.ip = pt.get<std::string>("server.db_server.listen.center.ip");
        env::xml->listen_at_center.port = pt.get<uint32_t>("server.db_server.listen.center.port");
        CHECK_PORT(env::xml->listen_at_center.port);

        env::xml->connect_to_account.ip = pt.get<std::string>("server.db_server.connect.account.ip");
        env::xml->connect_to_account.port = pt.get<uint32_t>("server.db_server.connect.account.port");
        CHECK_PORT(env::xml->connect_to_account.port);

        env::xml->mysql.host = pt.get<std::string>("server.db_server.mysql.host");
        env::xml->mysql.db = pt.get<std::string>("server.db_server.mysql.db");
        env::xml->mysql.user = pt.get<std::string>("server.db_server.mysql.user");
        env::xml->mysql.pwd = pt.get<std::string>("server.db_server.mysql.pwd");
        env::xml->mysql.init_conn = pt.get<uint32_t>("server.db_server.mysql.init_connections");
        env::xml->mysql.max_conn = pt.get<uint32_t>("server.db_server.mysql.max_connections");

		env::xml->redis.host = pt.get<std::string>("server.db_server.redis.host");
		env::xml->redis.port = pt.get<uint32_t>("server.db_server.redis.port");
		env::xml->redis.pwd = pt.get<std::string>("server.db_server.redis.pwd");
		env::xml->redis.db = pt.get<uint32_t>("server.db_server.redis.db");
		env::xml->redis.save_interval = pt.get<uint32_t>("server.db_server.redis.save_interval");
		env::xml->redis.save_count_limit = pt.get<uint32_t>("server.db_server.redis.save_count_limit");

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
		env::xml->config_path = pt.get<std::string>("server.common.path.config");

		env::xml->log_debug.role = pt.get<bool>("server.common.log.role");
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

void db_server_t::close_xml()
{
    SAFE_DELETE_P(env::xml);
    log_info("close xml success!");
}

//----------------------------------------------------------------------------------------
bool db_server_t::init_network()
{
    if (!m_network.init(env::xml->thread_num.network, env::xml->ip_white_list))
    {
        log_error("init network failed!");
        return false;
    }

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

    // connect to account
    if (!m_network.connect_to(env::xml->connect_to_account.ip, env::xml->connect_to_account.port, &m_account_connector, env::xml->reconnect_interval_time))
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

    return true;
}
void db_server_t::close_network()
{
	msg_count_t::write_msg_recv_count_data_file(env::xml->is_log_msg_count);
	msg_count_t::write_msg_send_count_data_file(env::xml->is_log_msg_count);

    game_manager_t::clear_game();

    m_network.close();
}

//----------------------------------------------------------------------------------------
bool db_server_t::init_zdb()
{
    boost::tie(m_db_url, env::conn_game_zdb_pool) = zdb_util_t::open_zdb(
        env::xml->mysql.host, 
        env::xml->mysql.db, 
        env::xml->mysql.user, 
        env::xml->mysql.pwd,
        env::xml->mysql.max_conn,
        env::xml->mysql.init_conn);
    if (NULL == env::conn_game_zdb_pool)
    {
        log_error("init zdb connection pool failed!");
        return false;
    }

    return true;
}
void db_server_t::close_zdb()
{
    zdb_util_t::close_zdb(env::conn_game_zdb_pool, m_db_url);
}

//----------------------------------------------------------------------------------------
bool db_server_t::init_redis()
{
    redis_client_t::simple_table_field_list tables;
    tables.insert(std::make_pair("role", "name"));
    tables.insert(std::make_pair("item", "role_uid"));
    tables.insert(std::make_pair("heroes", "role_uid"));
    tables.insert(std::make_pair("task", "role_uid"));
    tables.insert(std::make_pair("talent", "hero_uid"));
    tables.insert(std::make_pair("role_trade_item", "role_uid"));
    tables.insert(std::make_pair("role_ex", ""));
	tables.insert(std::make_pair("role_money", ""));
	tables.insert(std::make_pair("role_fight", ""));
	tables.insert(std::make_pair("role_tower", ""));
	tables.insert(std::make_pair("country_bounty", ""));
    tables.insert(std::make_pair("role_mount", "role_uid"));
    tables.insert(std::make_pair("arena_role_data", "role_uid"));
	tables.insert(std::make_pair("achieve", "role_uid"));
	tables.insert(std::make_pair("role_dungeon", "role_uid"));
	tables.insert(std::make_pair("expedition", ""));
	tables.insert(std::make_pair("active_reward", "role_uid"));
	tables.insert(std::make_pair("random_shop", "role_uid"));

	if (g_redis_save.init(env::xml->redis.host,
		env::xml->redis.port,
		env::xml->redis.pwd.c_str(),
		env::xml->redis.db,
		env::xml->redis.save_interval,
		env::xml->redis.save_count_limit,
		env::conn_game_zdb_pool))
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
        env::conn_game_zdb_pool,
        tables))
    {
        log_error("init redis client failed!");
        return false;
    }

    return true;
}
void db_server_t::close_redis()
{
	g_redis_save.close(env::conn_game_zdb_pool);
    redis_client_t::close(env::conn_game_zdb_pool);
}

//----------------------------------------------------------------------------------------
void db_server_t::on_add_game(const network::tcp_socket_ptr& s)
{
    game_manager_t::add_game(s);
}
void db_server_t::on_del_game(const network::tcp_socket_ptr& s)
{
    game_manager_t::del_game(s);
}
void db_server_t::on_game_msg(const tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const msg_buf_ptr& msg_buf)
{
	msg_count_t::push_msg_recv_info(msg_buf, cmd, env::xml->is_log_msg_count);
	game_msg_handler_t::handle_server_msg(s, uid, cmd, msg_buf);
}
//void db_server_t::on_game_msg(const tcp_socket_ptr& s, uint16_t cmd, const msg_buf_ptr& msg_buf)
//{
//	msg_count_t::push_msg_recv_info(msg_buf, cmd, env::xml->is_log_msg_count);
//	game_msg_handler_t::handle_server_msg(s, cmd, msg_buf);
//}

//----------------------------------------------------------------------------------------
void db_server_t::on_add_center(const network::tcp_socket_ptr& s)
{
    m_center_listener.set_socket(s);
}
void db_server_t::on_del_center(const network::tcp_socket_ptr& s)
{
    env::server->post_network_close_socket(s);
    m_center_listener.close_socket();
}
void db_server_t::on_center_msg(const tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const msg_buf_ptr& msg_buf)
{
	msg_count_t::push_msg_recv_info(msg_buf, cmd, env::xml->is_log_msg_count);
	center_msg_handler_t::handle_server_msg(uid,cmd, msg_buf);
}

//----------------------------------------------------------------------------------------
void db_server_t::on_add_account(const network::tcp_socket_ptr& s)
{
	m_account_connector.set_socket(s);
}
void db_server_t::on_del_account(const network::tcp_socket_ptr& s)
{
	env::server->post_network_close_socket(s);
	m_account_connector.close_socket();
}
void db_server_t::on_account_msg(const tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const msg_buf_ptr& msg_buf)
{
	msg_count_t::push_msg_recv_info(msg_buf, cmd, env::xml->is_log_msg_count);
	account_msg_handler_t::handle_server_msg(s, uid,cmd, msg_buf);
}

void db_server_t::post_network_send_msg(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& buf)
{
	msg_count_t::push_msg_send_info(buf, env::xml->is_log_msg_count);
	m_network.post_send_msg(s, buf);
}
