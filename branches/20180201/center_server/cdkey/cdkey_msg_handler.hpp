#ifndef __CDKEY_MSG_HANDLE_HPP__
#define __CDKEY_MSG_HANDLE_HPP__

#include "network/tcp_socket.hpp"
#include "message.hpp"
class cdkey_msg_handler_t 
{
public:
	static bool init_gate_msg_handler();
	static bool init_web_msg_handler();

public:
	static bool handle_me_do_change_cdkey_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
public:
	static bool handle_ce_exchange_cdkey_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};

#endif