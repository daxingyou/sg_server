#ifndef __GAME_LUCKYDRAW_MSG_HANDLE_H__
#define __GAME_LUCKYDRAW_MSG_HANDLE_H__

#include "gate/gate_msg_handler.hpp"

class luckydraw_msg_handle_t
{
public:
    static bool init_msg_handler();

	static bool init_center_msg_handler();
public:
	static bool handle_cg_luckydraw_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_luckydraw_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
public:
	static bool handle_eg_king_open_wish_summon_request(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_buy_wish_summon_request(const network::msg_buf_ptr& msg_buf);
};

#endif
