#ifndef __GAME_EXPEDITION_MSG_HANDLE_H__
#define __GAME_EXPEDITION_MSG_HANDLE_H__

#include "gate/gate_msg_handler.hpp"

class expedition_msg_handle_t
{
public:
    static bool init_client_msg_handler();

	static bool init_center_msg_handler();
public:
	static bool handle_cg_expedition_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_expedition_fight_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_expedition_refresh_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_expedition_help_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_expedition_shop_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_expedition_award_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_expedition_box_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	
	static bool handle_eg_expedition_fight_data_reply(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_expedition_refresh_reply(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_expedition_refresh_stage_reply(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_expedition_help_reply(const network::msg_buf_ptr& msg_buf);
};

#endif
