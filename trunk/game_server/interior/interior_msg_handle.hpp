#ifndef __GAME_INTERIOR_MSG_HANDLE_H__
#define __GAME_INTERIOR_MSG_HANDLE_H__

#include "gate/gate_msg_handler.hpp"

class interior_msg_handle_t
{
public:
    static bool init_msg_handler();
    
    static bool handle_cg_get_build_copper_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_upgrade_interior_build_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_dispatch_task_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_refresh_task_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_upgrade_task_star_level_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_get_interior_reward_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_complete_interior_task_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_get_offline_reward_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_instant_battle_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};

#endif