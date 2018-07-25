#ifndef __GAME_RECHARGE_MSG_HANDLE_H__
#define __GAME_RECHARGE_MSG_HANDLE_H__

#include "gate/gate_msg_handler.hpp"

class recharge_msg_handle_t
{
public:
    static bool init_msg_handler();

public:
	// ��ֵ
	static bool handle_cg_recharge_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	// ��ֵ����
	static bool handle_cg_recharge_reward_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	// ÿ���ػ���Ϣ
	static bool handle_cg_recharge_cheap_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	// ����ÿ���ػ�
	static bool handle_cg_recharge_cheap_buy_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

};

#endif
