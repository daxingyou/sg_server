#pragma once

#include "common/common_fwd.hpp"
#include "common_struct.hpp"
#include <boost/asio.hpp>

class db_server_t;

struct env
{
    static db_server_t* server;
    static boost::asio::signal_set* signals;
    static common::db_xml_t* xml;
    static struct ConnectionPool_S *conn_game_zdb_pool;
	static struct ConnectionPool_S *conn_account_db_pool;
};

#define ROLE_LOG(format, ...) if(env::xml->log_debug.role){log_debug(format, ##__VA_ARGS__);}
