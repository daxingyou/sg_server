#ifndef __GAME_FAMILY_SKILL_MSG_HANDLE_H__
#define __GAME_FAMILY_SKILL_MSG_HANDLE_H__

#include "gate/gate_msg_handler.hpp"

class family_skill_msg_handle_t
{
public:
    static bool init_msg_handler();

public:
	static bool handle_cg_family_skill_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_family_skill_levelup_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};

#endif
