#ifndef __CENTER_KING_WAR_MSG_H__
#define __CENTER_KING_WAR_MSG_H__

#include "network/tcp_socket.hpp"
#include "message.hpp"

class king_war_msg_handle_t
{
public:
	static bool init_gate_msg_handler();
	static bool init_game_msg_handler();

	static bool handle_ce_king_war_sign_up_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_king_war_offer_money_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_king_war_sign_up_list_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	static bool handle_ge_king_war_result_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};

#endif
