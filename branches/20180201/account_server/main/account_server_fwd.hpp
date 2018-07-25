#ifndef __ACCOUNT_SERVER_FWD_HPP__
#define __ACCOUNT_SERVER_FWD_HPP__

#include "common/common_fwd.hpp"
#include <boost/asio.hpp>

class account_server_t;

struct env
{
    static account_server_t* server;
    static boost::asio::signal_set* signals;
    static common::account_xml_t* xml;
    static struct ConnectionPool_S *conn_zdb_pool;
};

#define ROLE_LOG(format, ...) if(env::xml->log_debug.role){log_debug(format, ##__VA_ARGS__);}
#endif
