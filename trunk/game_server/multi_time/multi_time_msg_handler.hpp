#pragma once
#include "message.hpp"
#include "network/tcp_socket.hpp"

class multi_time_msg_handler_t
{
public:
	static bool init_msg_handler();

public:
	static bool handle_cg_change_multi_state_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};