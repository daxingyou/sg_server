#ifndef __ACCOUNT_LOGIN_MSG_HANDLER_HPP__
#define __ACCOUNT_LOGIN_MSG_HANDLER_HPP__

#include "msg_handler.hpp"
#include "protos_fwd.hpp"
#include "cmd_desc.hpp"

class login_msg_handler_t
{
public:
    typedef bool (*msg_handler_func)(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

    static bool init_msg_handler();

    static bool handle_server_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr msg_buf)
    {
        log_trace("recv role[%lu] cmd[%s:%d] message from login", uid, CMD_DESC(cmd), cmd);

        msg_handler_func func = m_login_msg_handle.find_func(cmd);
        if ( NULL == func )
        {
            log_error("no handler func for msg cmd:%d", cmd);
            return false;
        }
        func(s, msg_buf);

        return true;
    };

    static bool handle_ln_syn_login_info_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ln_user_num_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ln_send_gm_msg_to_account_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
private:
    static network::server_msg_handler_t<op_cmd::LN_BEGIN, op_cmd::LN_END, msg_handler_func> m_login_msg_handle;
};

#endif
