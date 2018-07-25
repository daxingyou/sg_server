#ifndef __RED_POINT_MSG_HANDLER_H__
#define __RED_POINT_MSG_HANDLER_H__

#include "message.hpp"
#include "network/tcp_socket.hpp"
class red_point_msg_handle_t
{
public:
	static bool init_gate_msg_handler();
	static bool init_center_msg_handler();

	//from client
	static bool handle_cg_red_point_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	//from center
	static bool handle_eg_red_point_notify(const network::msg_buf_ptr& msg_buf);
};

#endif //__RED_POINT_MSG_HANDLER_H__