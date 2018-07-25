#ifndef __FIGHT_STUDY_MSG_HANDLE_HPP__
#define __FIGHT_STUDY_MSG_HANDLE_HPP__

#include "gate/gate_msg_handler.hpp"

class fight_study_msg_handle_t
{

public:
	static bool init_msg_handler();

	static bool handle_fight_study_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_fight_study_cancel_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_fight_study_answer_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

};

#endif//__FIGHT_STUDY_MSG_HANDLE_HPP__