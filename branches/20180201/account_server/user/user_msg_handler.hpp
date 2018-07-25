#ifndef __ACCOUNT_USER_MSG_HANDLER_HPP__
#define __ACCOUNT_USER_MSG_HANDLER_HPP__

#include "msg_handler.hpp"
#include "protos_fwd.hpp"

class user_msg_handler_t
{
public:
    static bool init_msg_handler();
    static bool handle_server_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

public:
    static network::client_msg_handler_t<op_cmd::CN_BEGIN, op_cmd::CN_END> m_user_msg_handle;
};

#endif
