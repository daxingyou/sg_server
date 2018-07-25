#ifndef __GATE_USER_MSG_HANDLER_HPP__
#define __GATE_USER_MSG_HANDLER_HPP__

#include "msg_handler.hpp"
#include "protos_fwd.hpp"

class user_msg_handler_t
{
public:
    static bool init_msg_handler();
    static bool handle_ca_pingpong_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
    static bool handle_ca_connect_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
    static bool handle_ca_lock_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
    static bool handle_ca_unlock_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ca_login_game_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ca_create_role_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ca_frames_check_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

public:
    static network::client_msg_handler_t<op_cmd::CA_BEGIN, op_cmd::CA_END> m_user_msg_handle;
};

#endif
