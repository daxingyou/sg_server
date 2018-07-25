#ifndef __GAME_OFFICE_MSG_HANDLE_H__
#define __GAME_OFFICE_MSG_HANDLE_H__

#include "gate/gate_msg_handler.hpp"

class office_msg_handle_t
{
public:
    static bool init_msg_handler();

public:
	// 获取信息
	static bool handle_cg_office_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	// 每日奖励
	static bool handle_cg_office_award_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};

#endif
