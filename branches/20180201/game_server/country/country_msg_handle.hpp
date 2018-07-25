#ifndef __COUNTRY_MSG_HANDLE_H__
#define __COUNTRY_MSG_HANDLE_H__

#include "network/tcp_socket.hpp"
#include "message.hpp"

class country_msg_handle_t
{
public:
    static bool init_gate_msg_handler();
	static bool init_center_msg_handler();

public:
	// 同步国家信息
	static bool handle_eg_update_country_notify(const network::msg_buf_ptr& msg_buf);

	static bool handle_eg_country_name_change_request(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_country_year_name_request(const network::msg_buf_ptr& msg_buf);
};

#endif
