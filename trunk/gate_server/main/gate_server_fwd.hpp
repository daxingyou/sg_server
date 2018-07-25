#ifndef __GATE_SERVER_FWD_HPP__
#define __GATE_SERVER_FWD_HPP__

#include "common/common_fwd.hpp"
#include <boost/asio.hpp>

class gate_server_t;

struct env
{
    static gate_server_t* server;
    static common::gate_xml_t* xml;
};

#define ROLE_LOG(format, ...) if(env::xml->log_debug.role){log_debug(format, ##__VA_ARGS__);}
#endif
