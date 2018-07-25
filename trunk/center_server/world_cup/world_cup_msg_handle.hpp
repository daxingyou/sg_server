#ifndef __CENTER_WORLD_CUP_MSG_H__
#define __CENTER_WORLD_CUP_MSG_H__

#include "message.hpp"
#include "network/tcp_socket.hpp"

class world_cup_msg_handle_t
{
public:
	static bool init_client_msg_handler();

	static bool handle_ce_world_cup_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_world_cup_odds_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};

#endif