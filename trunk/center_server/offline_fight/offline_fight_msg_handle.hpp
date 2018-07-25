#ifndef __CENTER_OFFLINE_FIGHT_MSG_H__
#define __CENTER_OFFLINE_FIGHT_MSG_H__

#include "message.hpp"
#include "network/tcp_socket.hpp"

class offline_fight_msg_handle_t
{
public:
	static bool init_game_msg_handler();
	static bool handle_ge_arena_fight_data_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_expedition_fight_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_expedition_refresh_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_expedition_refresh_stage_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_expedition_help_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};

#endif