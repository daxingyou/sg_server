#ifndef __GAME_DUNGEON_MSG_HANDLE_H__
#define __GAME_DUNGEON_MSG_HANDLE_H__

#include "message.hpp"
#include "network/tcp_socket.hpp"

class dungeon_msg_handle_t
{
public:
	static bool init_gate_msg_handler();
	static bool init_center_msg_handler();

	static bool handle_cg_role_dungeon_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_open_dungeon_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_dungeon_leave_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	static bool handle_cg_open_dugeon_team_check_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_ask_team_check_reuqest(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	static bool handle_cg_dungeon_fight_again_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	static bool handle_cg_dungeon_event_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_dungeon_buy_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_dungeon_times_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	static bool handle_eg_create_dugeon_request(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_open_dungeon_reply(const network::msg_buf_ptr& msg_buf);
};

#endif // !__GAME_DUNGEON_MSG_HANDLE_H__