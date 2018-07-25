#ifndef __USER_TROOP_MSG_HANDLE_H__
#define __USER_TROOP_MSG_HANDLE_H__

#include "message.hpp"
#include "network/tcp_socket.hpp"
#include <string>
class user_troop_msg_handle_t
{
public:
    static bool init_msg_handler();
	static bool init_game_msg_handler();

public:
    //from client
    static bool handle_ce_troop_kick_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_troop_apply_member_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_troop_process_apply_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
    static bool handle_ce_troop_invite_join_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_troop_invite_single_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
    static bool handle_ce_troop_auto_join_flag_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
    static bool handle_ce_troop_quit_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
    static bool handle_ce_troop_leave_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
    static bool handle_ce_troop_list_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
    static bool handle_ce_troop_modify_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
    static bool handle_ce_troop_transfer_leader_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_troop_invite_shadow_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

public:
	// from game
	static bool handle_ge_troop_create_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_troop_auto_match_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_troop_join_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_troop_agree_apply_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_troop_process_invite_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_troop_dissolve_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_troop_back_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_troop_transmission_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_troop_sync_task_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_troop_escape_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_troop_del_shadow_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_troop_friend_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};

#endif
