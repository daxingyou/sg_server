#pragma once
#include "message.hpp"
#include "network/tcp_socket.hpp"
#include "msg_handler.hpp"

class sys_notice_msg_handle_t
{
public:
	static bool init_msg_handler();
public:
	static bool handle_eg_check_sys_notice_reply(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_syn_sys_notice(const network::msg_buf_ptr& msg_buf); 
	static bool handle_eg_gm_clear_notice_notify(const network::msg_buf_ptr& msg_buf);
};