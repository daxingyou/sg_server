#include "video_server.hpp"
#include "video_server_fwd.hpp"
#include "log.hpp"
#include "cross/cross_arena_video.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

bool video_server_t::on_init_server()
{
	CMD_INIT;

	// xml
	if (!load_xml()) {
		close_xml();
		COMMON_ASSERT(false);
		return false;
	}

	// log
	if (!g_log.open(env::xml->log_path, server_name(), env::xml->is_log_trace, env::xml->is_log_debug, env::xml->is_log_info, env::xml->is_new_logfile_per_hour)) {
		COMMON_ASSERT(false);
		return false;
	}

	// zdb
	if (!init_zdb()) {
		COMMON_ASSERT(false);
		return false;
	}

	// logic
	if (!init_logic()) {
		COMMON_ASSERT(false);
		return false;
	}

	// network
	if (!init_network()) {
		COMMON_ASSERT(false);
		return false;
	}

	return true;
}

bool video_server_t::on_close_server()
{
	stop_network();
	close_network();

	clear_logic();

	close_zdb();
	close_xml();
	g_log.close();

	CMD_CLOSE;

	return true;
}

void video_server_t::on_run_server()
{
	log_info("video server running...");
}

//---------------------------------------------------------------------------------------
bool video_server_t::load_xml()
{
	if (!load_server_xml()) {
		printf("\033[31mload [%s] failed!\033[0m\n", GLOBAL_SERVER_XML_PATH);
		return false;
	}
	printf("load [%s] success!\n", GLOBAL_SERVER_XML_PATH);

	return true;
}

bool video_server_t::load_server_xml()
{
	try {
		boost::property_tree::ptree pt;
		boost::property_tree::xml_parser::read_xml(GLOBAL_SERVER_XML_PATH, pt);

		env::xml = new video_xml_t;

		env::xml->fight_version = pt.get<uint32_t>("server.video_server.fight_version");
		env::xml->log_path = pt.get<std::string>("server.common.log_path");
		env::xml->is_log_trace = pt.get<bool>("server.common.log_trace");
		env::xml->is_log_debug = pt.get<bool>("server.common.log_debug");
		env::xml->is_log_info = pt.get<bool>("server.common.log_info");
		env::xml->is_new_logfile_per_hour = pt.get<bool>("server.common.new_logfile_per_hour");

		std::string str_ip_white_list = pt.get<std::string>("server.common.ip_white_list");
		string_util_t::split(str_ip_white_list, env::xml->ip_white_list, ";");
		if (env::xml->ip_white_list.empty()) {
			printf("\033[31mip_white_list is empty!\033[0m\n");
			return false;
		}

		env::xml->fight_ttl = pt.get<uint32_t>("server.video_server.fight_ttl");

		env::xml->net_thread_num = pt.get<uint32_t>("server.video_server.thread_num.network");

		env::xml->listen_at_client.ip = pt.get<std::string>("server.video_server.listen.ip");
		env::xml->listen_at_client.port = pt.get<uint32_t>("server.video_server.listen.port");
		CHECK_PORT(env::xml->listen_at_client.port);

		env::xml->mysql.host = pt.get<std::string>("server.video_server.mysql.host");
		env::xml->mysql.db = pt.get<std::string>("server.video_server.mysql.db");
		env::xml->mysql.user = pt.get<std::string>("server.video_server.mysql.user");
		env::xml->mysql.pwd = pt.get<std::string>("server.video_server.mysql.pwd");
		
		env::xml->redis.host = pt.get<std::string>("server.video_server.redis.host");
		env::xml->redis.port = pt.get<uint32_t>("server.video_server.redis.port");
		env::xml->redis.pwd = pt.get<std::string>("server.video_server.redis.pwd");

		// cross_arena
		{
			env::xml->cross_arena.redis_db = pt.get<uint32_t>("server.video_server.cross_arena.db");
			env::xml->cross_arena.process_num = pt.get<uint32_t>("server.video_server.cross_arena.process_thread_num");

			env::xml->cross_arena.personal.recent_count = pt.get<uint32_t>("server.video_server.cross_arena.personal.recent_count");
			env::xml->cross_arena.personal.life_time = pt.get<uint32_t>("server.video_server.cross_arena.personal.life_time");

			env::xml->cross_arena.strongest_king.recent_count = pt.get<uint32_t>("server.video_server.cross_arena.strongest_king.recent_count");
			env::xml->cross_arena.strongest_king.rank_lv = pt.get<uint32_t>("server.video_server.cross_arena.strongest_king.rank_lv");
		}

		env::xml->mysql.init_conn += env::xml->cross_arena.process_num;

	} catch (boost::property_tree::ptree_error& e) {
		printf("\033[31mthrow error[%s]\033[0m\n", e.what());
		return false;
	}

	if (NULL == env::xml) {
		printf("\033[31menv::xml is NULL!\033[0m\n");
		return false;
	}

	return true;
}

void video_server_t::close_xml()
{
	SAFE_DELETE_P(env::xml);
	log_info("close xml success!");
}

//---------------------------------------------------------------------------------------

bool video_server_t::init_zdb()
{
	boost::tie(m_db_url, env::conn_zdb_pool) = zdb_util_t::open_zdb(
		env::xml->mysql.host,
		env::xml->mysql.db,
		env::xml->mysql.user,
		env::xml->mysql.pwd,
		env::xml->mysql.init_conn,
		env::xml->mysql.init_conn);

	if (NULL == env::conn_zdb_pool)
	{
		log_error("init zdb connection pool failed!");
		return false;
	}

	return true;
}

void video_server_t::close_zdb()
{
	zdb_util_t::close_zdb(env::conn_zdb_pool, m_db_url);
}

//---------------------------------------------------------------------------------------

bool video_server_t::init_network()
{
	if (!m_network.init(env::xml->net_thread_num, env::xml->ip_white_list)) {
		log_error("init network failed!");
		return false;
	}

	// listen for client
	if (!m_network.listen_at(env::xml->listen_at_client.ip, env::xml->listen_at_client.port, &m_client_listener))	{
		return false;
	} 

	return true;
}

void video_server_t::close_network()
{
	m_network.close();
}

void video_server_t::stop_network()
{
	m_network.stop();
}

//---------------------------------------------------------------------------------------

bool video_server_t::init_logic()
{
	if (!init_video()) {
		COMMON_ASSERT(false);
		return false;
	}

	return true;
}

void video_server_t::clear_logic()
{
	close_video();
}

//---------------------------------------------------------------------------------------

bool video_server_t::init_video()
{
	if (!cross_arena_service::getInstance().init(
		env::xml->redis.host.c_str(),
		env::xml->redis.port,
		env::xml->redis.pwd.c_str(),
		env::xml->cross_arena.redis_db,
		env::xml->cross_arena.process_num,
		env::conn_zdb_pool)) {
		return false;
	}

	return true;
}

void video_server_t::close_video()
{
	cross_arena_service::getInstance().close();
	cross_arena_service::delInstance();
}

//---------------------------------------------------------------------------------------