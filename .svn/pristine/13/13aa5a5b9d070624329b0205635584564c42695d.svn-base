#ifndef __GAME_LIANDAN_MSG_HANDLE_H__
#define __GAME_LIANDAN_MSG_HANDLE_H__

#include "gate/gate_msg_handler.hpp"

class liandan_msg_handle_t
{
public:
    static bool init_msg_handler();

public:
	static bool handle_cg_liandan_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_liandan_unlock_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_liandan_doing_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_liandan_done_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_liandan_return_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

};

#endif
