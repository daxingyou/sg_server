#pragma once

#include "message.hpp"
#include "network/tcp_socket.hpp"

class collect_msg_handler_t
{
public:
	static bool init_msg_handler();

public:
	static bool handle_cg_start_collect_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_end_collect_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_cancel_collect_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	
};