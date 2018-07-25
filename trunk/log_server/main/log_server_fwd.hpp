#ifndef __LOG_SERVER_FWD_HPP__
#define __LOG_SERVER_FWD_HPP__

#include "common/common_fwd.hpp"
#include <boost/asio.hpp>

class log_server_t;

struct env
{
    static log_server_t* server;
    static common::log_xml_t* xml;
    static struct ConnectionPool_S *conn_zdb_pool;
};

#endif
