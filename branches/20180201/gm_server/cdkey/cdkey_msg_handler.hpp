#ifndef __CDKEY_MSG_HANDLER_HPP__
#define __CDKEY_MSG_HANDLER_HPP__

#include "center/center_msg_handler.hpp"

class cdkey_msg_handler_t 
{
public:
	static bool init_msg_handler();

	//¶Ò»»Àñ°üÂë
	static bool handler_em_do_change_cdkey_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};


#endif
