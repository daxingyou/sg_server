#ifndef __FIGHT_MSG_HANDLE_H__
#define __FIGHT_MSG_HANDLE_H__

#include "network/tcp_socket.hpp"

class fight_msg_handle_t
{
public:
	static bool init_msg_handler();
	static bool handle_finish_fight_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_combat_loading_end_ntf(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_combat_form_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_combat_play_end_ntf(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_combat_escape_ntf(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_combat_auto_fight_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_combat_before_form_ready_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_combat_mark_target_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_combat_speak_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);		
	static bool handle_combat_ask_escape_req(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_combat_recovery_ask_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_watch_combat_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cancel_watch_combat_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_new_role_combat_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

};

#endif//__FIGHT_MSG_HANDLE_H__