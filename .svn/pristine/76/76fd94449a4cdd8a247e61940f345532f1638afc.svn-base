#ifndef _GENERAL_EVENT_MSG_HANDLER_HPP_
#define _GENERAL_EVENT_MSG_HANDLER_HPP_

#include "gate/gate_msg_handler.hpp"
#include "center/center_msg_handler.hpp"

class general_event_msg_handler_t
{
public:
	static bool init_gate_msg_handler();
	
	static bool init_center_msg_handler();
public:
	static bool handle_cg_get_general_event_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_get_general_event_detail_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_get_general_event_prize_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_get_server_level_limit_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
public:
	static bool handler_eg_sync_general_event_count_info(const network::msg_buf_ptr& msg_buf);

	static bool handler_eg_sync_general_event_data(const network::msg_buf_ptr& msg_buf);

	static bool handler_eg_sync_general_event_open(const network::msg_buf_ptr& msg_buf);

	static bool handler_eg_sync_general_event_done(const network::msg_buf_ptr& msg_buf);

	static bool handle_eg_sync_server_level(const network::msg_buf_ptr& msg_buf);
};

#endif
