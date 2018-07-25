#ifndef __LUCKYDRAW_MSG_HANDLE_HPP__
#define __LUCKYDRAW_MSG_HANDLE_HPP__

#include "network/tcp_socket.hpp"
#include "message.hpp"
class luckydraw_msg_handle_t
{
public:
	static bool init_gate_msg_handler();
	static bool init_game_msg_handler();

public:
	static bool handle_ce_buy_wish_summon_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_open_summon_wish_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_get_wish_summon_info_request(const network::tcp_socket_ptr&s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_get_king_money_request(const network::tcp_socket_ptr&s, const network::msg_buf_ptr& msg_buf);
public:
	static bool handle_ge_buy_wish_summon_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_king_open_wish_summon_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};

#endif