#ifndef __GAME_FIELD_BOSS_MSG_H__
#define __GAME_FIELD_BOSS_MSG_H__

#include "message.hpp"
#include "network/tcp_socket.hpp"

class field_boss_msg_handle_t
{
public:
	static bool init_center_msg_handler();
	static bool init_gate_msg_handler();

public:
	static bool handle_eg_summon_field_boss_request(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_field_boss_die_notify(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_clear_all_boss_notify(const network::msg_buf_ptr& msg_buf);

public:
	static bool handle_cg_field_boss_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_field_boss_attack_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};

#endif