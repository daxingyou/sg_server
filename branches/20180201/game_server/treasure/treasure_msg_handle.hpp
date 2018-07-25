#ifndef __GAME_TREASURE_MSG_HANDLE_H__
#define __GAME_TREASURE_MSG_HANDLE_H__

#include "gate/gate_msg_handler.hpp"

class treasure_msg_handle_t
{
public:
    static bool init_msg_handler();

	static bool handle_cg_treasure_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_treasure_drop_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_treasure_gather_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};

#endif