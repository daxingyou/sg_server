#ifndef __CENTER_CITY_MSG_HANDLER_HPP__
#define __CENTER_CITY_MSG_HANDLER_HPP__

#include "main/center_server.hpp"
#include "main/center_server_fwd.hpp"
#include "msg_handler.hpp"
#include "protos_fwd.hpp"
#include "tcp_socket.hpp"
#include "cmd_desc.hpp"

class city_msg_handler_t
{
public:
	static bool init_msg_handler();
	
public:
	static bool handle_ge_city_trade_add_prosperity_value_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_get_city_list_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};

#endif