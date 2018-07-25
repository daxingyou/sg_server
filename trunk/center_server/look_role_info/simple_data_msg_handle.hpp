#pragma once

#include "network/tcp_socket.hpp"
#include "message.hpp"

class simple_data_msg_handle_t
{
public:
	static bool init_client_msg_handler();
	static bool init_game_msg_handler();

	static bool handle_ce_other_role_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	static bool handle_ge_role_simple_data_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};
