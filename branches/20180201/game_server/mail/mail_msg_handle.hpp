#ifndef __GAME_MAIL_MSG_H__
#define __GAME_MAIL_MSG_H__

#include "message.hpp"
#include "network/tcp_socket.hpp"

class mail_msg_handle_t
{
public:
	static bool init_center_msg_handler();

	static bool handle_eg_mail_get_addenda_request(const network::msg_buf_ptr& msg_buf);
};

#endif
