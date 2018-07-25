#ifndef __VIDEO_SERVER_FWD_HPP__
#define __VIDEO_SERVER_FWD_HPP__

#include <boost/asio.hpp>

#include "common/common_fwd.hpp"
#include "common/xml_define.hpp"

struct video_xml_t
{
	uint32_t fight_version = 0;
	std::string log_path = "";
	bool is_log_trace = false;
	bool is_log_debug = false;
	bool is_log_info = false;
	bool is_new_logfile_per_hour = false;
	std::vector<std::string> ip_white_list;

	uint32_t fight_ttl = 0;

	uint32_t net_thread_num = 0;

	common::listen_t listen_at_client;

	common::mysql_t mysql;

	struct {
		std::string host = "";
		uint32_t port = 0;
		std::string pwd = "";
	} redis;

	struct {
		uint32_t redis_db = 0;
		uint32_t process_num = 0;
		struct {
			uint32_t recent_count = 0;
			uint32_t life_time = 0;
		} personal;
		struct {
			uint32_t recent_count = 0;
			uint32_t rank_lv = 0;
		} strongest_king;
	} cross_arena;
};

class video_server_t;

struct env
{
	static video_server_t* server;
	static video_xml_t* xml;
	static struct ConnectionPool_S *conn_zdb_pool;
};

#endif