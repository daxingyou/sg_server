#ifndef __GAME_FRIEND_MSG_HANDLER_H_
#define __GAME_FRIEND_MSG_HANDLER_H_

#include "message.hpp"
#include "network/tcp_socket.hpp"

class friend_msg_handle_t
{
public:
	static bool init_center_msg_handler();
	static bool init_gate_msg_handler();

	static bool handle_eg_friend_point_operation_request(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_get_friendliness_reply(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_friendliness_change(const network::msg_buf_ptr& msg_buf);
};

#endif