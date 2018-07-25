#pragma once

#include "message.hpp"
#include "network/tcp_socket.hpp"

class arena_msg_handler_t
{
public:
	static bool init_msg_handler();

public:
	static bool handle_cg_enter_arena_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_arena_join_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_arena_buy_ticket_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_arena_match_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_get_arena_rank_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_arena_cancel_match_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_arena_self_rank_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_leave_arena_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};