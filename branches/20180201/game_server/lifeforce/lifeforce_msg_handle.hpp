#ifndef __LIFEFORCE_MSG_HANDLE_H__
#define __LIFEFORCE_MSG_HANDLE_H__

#include "message.hpp"
#include "network/tcp_socket.hpp"
class lifeforce_msg_handle_t
{
public:
	static bool init_msg_handler();

	//from client
	static bool handle_cg_activate_life_star_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_break_life_state_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_up_level_lifelabel_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};

#endif	// __LIFEFORCE_MSG_HANDLE_H__