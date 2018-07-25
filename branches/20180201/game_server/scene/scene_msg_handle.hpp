#ifndef __GAME_SCENE_MSG_HANDLE_H__
#define __GAME_SCENE_MSG_HANDLE_H__

#include "message.hpp"
#include "network/tcp_socket.hpp"

class scene_msg_handle_t
{
public:
	static bool init_gate_msg_handler();
	static bool init_center_msg_handler();
    
	static bool handle_cg_enter_scene_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_jump_scene_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_jump_reading_start_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_jump_reading_break_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_troop_jump_end_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	static bool handle_eg_game_scene_reg_reply(const network::msg_buf_ptr& msg_buf);
};

#endif // !__GAME_SCENE_MSG_HANDLE_H__