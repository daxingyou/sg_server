#ifndef __CENTER_CITY_MSG_HANDLER_HPP__
#define __CENTER_CITY_MSG_HANDLER_HPP__

#include "message.hpp"
#include "network/tcp_socket.hpp"

class city_msg_handler_t
{
public:
	static bool init_msg_handler();

public:
	static bool handle_eg_get_city_list_reply(const network::msg_buf_ptr& msg_buf);
};

#endif