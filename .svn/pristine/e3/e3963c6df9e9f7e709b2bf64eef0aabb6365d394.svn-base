#pragma once

#include "message.hpp"
#include "network/tcp_socket.hpp"

class trade_msg_handler_t
{
public:
	static bool init_msg_handler();

public:
	static bool handle_cg_trade_buy_item_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_trade_sell_item_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_get_city_trade_shop_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_trade_supplement_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_trade_event_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_troop_trade_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};