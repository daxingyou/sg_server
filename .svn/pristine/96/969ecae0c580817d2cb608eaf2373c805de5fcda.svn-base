#ifndef __USER_TROOP_MSG_HANDLE_H__
#define __USER_TROOP_MSG_HANDLE_H__

#include "message.hpp"
#include "network/tcp_socket.hpp"
#include <string>
class user_troop_msg_handle_t
{
public:
	static bool init_gate_msg_handler();
	static bool init_center_msg_handler();

public:
	static bool handle_cg_troop_create_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_troop_join_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_troop_auto_match_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_troop_process_invite_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_troop_back_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	static bool handle_eg_troop_event_notify(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_troop_agree_apply_request(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_troop_invite_shadow_notify(const network::msg_buf_ptr& msg_buf);
};

#endif
