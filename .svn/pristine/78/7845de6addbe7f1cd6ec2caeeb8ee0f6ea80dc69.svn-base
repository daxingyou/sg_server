#ifndef __GAME_PK_MSG_HANDLE_H__
#define __GAME_PK_MSG_HANDLE_H__

#include "gate/gate_msg_handler.hpp"

class pk_msg_handle_t
{
public:
    static bool init_msg_handler();

	static bool handle_cg_pk_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_challenge_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_challenge_process_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	static bool handle_eg_challenge_reply(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_challenge_process_reply(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_challenge_fight_request(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_challenge_transfer_notify(const network::msg_buf_ptr& msg_buf);
};

#endif