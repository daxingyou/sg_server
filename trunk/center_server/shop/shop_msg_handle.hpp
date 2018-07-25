#ifndef __CENTER_SHOP_MSG_H__
#define __CENTER_SHOP_MSG_H__

#include "network/tcp_socket.hpp"
#include "message.hpp"

class shop_msg_handle_t
{
public:
	static bool init_gate_msg_handler();
	static bool init_game_msg_handler();

	static bool handle_ce_shop_goods_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_buy_goods_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	static bool handle_ge_check_user_buy_goods_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};

#endif
