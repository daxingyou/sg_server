#ifndef __ACTIVITY_MSG_HANDLE_H__
#define __ACTIVITY_MSG_HANDLE_H__

#include "message.hpp"
#include "network/tcp_socket.hpp"
class activity_msg_handle_t
{
public:
    static bool init_msg_handler();

	static bool handle_cg_activity_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	
	static bool handle_cg_liveness_box_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};

#endif
