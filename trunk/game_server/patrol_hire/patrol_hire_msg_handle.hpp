#pragma once


#include "message.hpp"
#include "network/tcp_socket.hpp"

class patrol_hire_msg_handle_t
{
public:
	static bool init_msg_handler();
public:
	static bool handle_cg_patrol_hire_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_get_patrol_hire_rwd_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_fast_patrol_hire_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_change_patrol_hire_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

};