#pragma once

#include "message.hpp"
#include "network/tcp_socket.hpp"

class mount_msg_handler_t
{
public:
	static bool init_msg_handler();

public:
	static bool handle_cg_use_mount_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_turn_mount_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};