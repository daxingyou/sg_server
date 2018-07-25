#ifndef __CENTER_CHALLENGE_MSG_H__
#define __CENTER_CHALLENGE_MSG_H__

#include "message.hpp"
#include "network/tcp_socket.hpp"

class challenge_msg_handle_t
{
public:
	static bool init_client_msg_handler();
	static bool init_game_msg_handler();

	static bool handle_ce_challenge_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_challenge_record_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	static bool handle_ge_challenge_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_challenge_process_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_challenge_fight_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};

#endif