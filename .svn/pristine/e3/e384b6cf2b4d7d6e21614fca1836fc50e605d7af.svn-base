#ifndef __GAME_SHOP_MSG_HANDLE_H__
#define __GAME_SHOP_MSG_HANDLE_H__

#include "message.hpp"
#include "network/tcp_socket.hpp"

class shop_msg_handle_t
{
public:
	static bool init_center_msg_handler();
	static bool init_gate_msg_handler();

	static bool handle_eg_check_user_buy_goods_request(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_give_user_goods_notify(const network::msg_buf_ptr& msg_buf);

	static bool handle_cg_buy_goods_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	//寄售行消息
	static bool handle_cg_goods_buy_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_goods_buy_reply(const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_goods_sell_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_goods_sell_reply(const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_goods_return_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_goods_return_reply(const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_goods_resell_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_goods_resell_reply(const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_goods_profit_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_goods_profit_reply(const network::msg_buf_ptr& msg_buf);

	// 随机商店
	static bool handle_cg_random_shop_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_random_shop_buy_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_random_shop_refresh_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};

#endif
