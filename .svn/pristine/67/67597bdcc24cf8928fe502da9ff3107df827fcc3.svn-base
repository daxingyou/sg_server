#ifndef __GAME_TOWER_MSG_HANDLE_H__
#define __GAME_TOWER_MSG_HANDLE_H__

#include "gate/gate_msg_handler.hpp"

class tower_msg_handle_t
{
public:
    static bool init_msg_handler();
public:
	static bool handle_cg_tower_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_tower_fight_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_tower_auto_fight_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_tower_auto_fight_immediately_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_tower_auto_fight_reward_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_tower_reset_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_tower_achieve_reward_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_tower_trigger_fight_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};

#endif
