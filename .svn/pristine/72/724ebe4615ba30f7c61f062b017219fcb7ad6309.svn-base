#ifndef __GAME_ITEM_MSG_HANDLE_H__
#define __GAME_ITEM_MSG_HANDLE_H__

#include "gate/gate_msg_handler.hpp"
#include "center/center_msg_handler.hpp"

class item_msg_handle_t
{
public:
    static bool init_msg_handler();
	static bool init_center_msg_handler();
    
    //static bool handle_cg_create_item_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_use_item_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_sell_bag_item_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_merage_item_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_sell_hero_chip_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
public:
	static bool handle_eg_check_drop_items_reply(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_syn_item_limit_notify(const network::msg_buf_ptr& msg_buf);

	
};

#endif