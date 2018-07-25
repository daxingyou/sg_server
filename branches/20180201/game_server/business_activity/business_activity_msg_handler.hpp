#ifndef  _BUSINESS_ACTIVITY_MSG_HANDLER_HPP__
#define  _BUSINESS_ACTIVITY_MSG_HANDLER_HPP__

#include "gate/gate_msg_handler.hpp"

class business_activity_msg_handler_t
{
public:
	static bool init_msg_handler();

	/*Ò¡Ç®Ê÷*/
	static bool handle_cg_get_money_tree_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_shake_money_tree_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_levelup_money_tree_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	//ÆßÈÕµÇÂ¼
	static bool handle_cg_get_7d_login_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_get_7d_login_prize_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};

#endif
