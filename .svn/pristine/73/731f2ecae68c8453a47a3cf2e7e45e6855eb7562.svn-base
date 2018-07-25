#ifndef __GAME_EQUIP_MSG_HANDLE_H__
#define __GAME_EQUIP_MSG_HANDLE_H__

#include "gate/gate_msg_handler.hpp"

class equip_msg_handle_t
{
public:
    static bool init_msg_handler();
    
    static bool handle_cg_equip_exchange_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_equip_on_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_equip_off_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_equip_remake_attr_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_equip_remake_exattr_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_equip_replace_exattr_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_equip_smelt_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_equip_smelt_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_equip_smelt_box_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_equip_strengthen_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};

#endif