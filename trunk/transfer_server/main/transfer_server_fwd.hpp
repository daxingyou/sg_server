#ifndef __TRANSFER_SERVER_FWD_HPP__
#define __TRANSFER_SERVER_FWD_HPP__

#include <boost/asio.hpp>

#include "log.hpp"
#include "common_struct.hpp"

class transfer_server_t;

struct env
{
    static transfer_server_t* server;
    static common::transfer_xml_t* xml;
};

#endif
