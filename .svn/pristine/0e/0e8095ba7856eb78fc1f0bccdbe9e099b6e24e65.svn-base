#ifndef __AREA_SERVER_FWD_HPP__
#define __AREA_SERVER_FWD_HPP__

#include "log.hpp"
#include "common/common_struct.hpp"
#include <boost/asio.hpp>

class area_server_t;

struct env
{
    static area_server_t* server;
    static boost::asio::signal_set* signals;
    static common::area_xml_t* xml;
    static struct ConnectionPool_S *conn_zdb_pool;
};
#define WORLDCUP_LOG(format, ...) if(env::xml->log_debug.worldcup){log_debug(format, ##__VA_ARGS__);}
#define ARENA_LOG(format, ...) if(env::xml->log_debug.arena){log_debug(format, ##__VA_ARGS__);}
#endif
