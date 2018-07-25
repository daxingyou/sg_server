#ifndef __GAME_FORMATION_MSG_HANDLE_H__
#define __GAME_FORMATION_MSG_HANDLE_H__

#include "gate/gate_msg_handler.hpp"

class formation_msg_handle_t
{
public:
    static bool init_msg_handler();
    
	static bool handle_update_formation_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_update_cur_form_id_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_fight_update_formation_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_fight_update_cur_form_id_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_get_fight_formation_req(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_get_fight_special_heroes_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};

#endif // !__GAME_FORMATION_MSG_HANDLE_H__