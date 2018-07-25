#ifndef __GM_SERVER_FWD_HPP__
#define __GM_SERVER_FWD_HPP__

#include "log.hpp"
#include "common/common_fwd.hpp"
#include <boost/asio.hpp>

class gm_server_t;

struct env
{
    static gm_server_t* server;
    static boost::asio::signal_set* signals;
    static common::gm_xml_t* xml;
    static struct ConnectionPool_S *conn_zdb_pool;
};

#endif