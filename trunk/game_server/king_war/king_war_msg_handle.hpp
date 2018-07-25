#ifndef __GAME_KING_WAR_MSG_H__
#define __GAME_KING_WAR_MSG_H__

#include "message.hpp"
#include "network/tcp_socket.hpp"

class king_war_msg_handle_t
{
public:
	static bool init_center_msg_handler();
	static bool init_gate_msg_handler();

	static bool handle_eg_king_war_state_notify(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_king_war_time_end_notify(const network::msg_buf_ptr& msg_buf);

	static bool handle_cg_enter_king_war_scene_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_king_war_defence_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_king_war_attack_start_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_king_war_attack_end_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_king_war_attack_end_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_ask_king_war_fight_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_king_war_rank_list_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_king_war_jump_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};

#endif
