#pragma once

#include "message.hpp"
#include "network/tcp_socket.hpp"

class page_msg_handler_t
{
public:
	static bool init_msg_handler();
public:
	static bool handle_cg_section_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_section_reward_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_section_pass_pve_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	static bool handle_cg_quick_fight_elite_page_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_reset_quick_fight_elite_page_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

};