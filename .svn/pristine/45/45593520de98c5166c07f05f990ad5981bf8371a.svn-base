#ifndef __CENTER_MAIL_MSG_H__
#define __CENTER_MAIL_MSG_H__

#include "message.hpp"
#include "network/tcp_socket.hpp"

class mail_msg_handle_t
{
public:
	static bool init_gate_msg_handler();
	static bool init_game_msg_handler();

	static bool handle_ce_mail_list_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_mail_content_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_mail_get_addenda_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_mail_remove_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_mail_batch_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	static bool handle_ge_mail_get_addenda_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ge_mail_new_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};

#endif