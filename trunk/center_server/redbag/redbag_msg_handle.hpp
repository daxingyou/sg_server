#ifndef __CENTER_REDBAG_MSG_H__
#define __CENTER_REDBAG_MSG_H__

#include "message.hpp"
#include "network/tcp_socket.hpp"

class redbag_msg_handle_t
{
public:
	static bool init_client_msg_handler();
	static bool init_game_msg_handler();
	
	static bool handle_ce_redbag_draw_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	
	static bool handle_ge_redbag_send_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};

#endif