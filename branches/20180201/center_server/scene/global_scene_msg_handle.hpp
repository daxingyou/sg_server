#ifndef __GLOBAL_SCENE_MSG_H__
#define __GLOBAL_SCENE_MSG_H__

#include "message.hpp"
#include "network/tcp_socket.hpp"

class global_scene_msg_handle_t
{
public:
	static bool init_gate_msg_handler();
	static bool init_game_msg_handler();
	static bool handle_ge_role_scene_info_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_game_scene_unload_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_open_dungeon_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_create_dugeon_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_scene_state_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};

#endif // !__GLOBAL_SCENE_MSG_H__