#ifndef __CENTER_FIELD_BOSS_MSG_H__
#define __CENTER_FIELD_BOSS_MSG_H__

#include "message.hpp"
#include "network/tcp_socket.hpp"

class field_boss_msg_handle_t
{
public:
	static bool init_gate_msg_handler();
	static bool init_game_msg_handler();

public:
	static bool handle_ce_field_boss_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	static bool handle_ge_summon_field_boss_replay(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_field_boss_killed_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_field_boss_rank_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};

#endif