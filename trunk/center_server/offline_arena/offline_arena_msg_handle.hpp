#ifndef __CENTER_OFFLINE_ARENA_MSG_H__
#define __CENTER_OFFLINE_ARENA_MSG_H__

#include "message.hpp"
#include "network/tcp_socket.hpp"

class offline_arena_msg_handle_t
{
public:
	static bool init_client_msg_handler();
	static bool init_game_msg_handler();
	
	static bool handle_ce_offline_arena_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_offline_arena_enemy_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_offline_arena_fight_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_offline_arena_record_award_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_offline_arena_rank_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_offline_arena_record_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_offline_arena_sweep_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_offline_arena_score_award_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	static bool handle_ge_offline_arena_form_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_offline_arena_fight_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_offline_arena_buy_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_offline_arena_gmrank_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};

#endif