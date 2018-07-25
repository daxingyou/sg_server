#ifndef __LOGIN_ACCOUNT_MSG_HANDLER_HPP__
#define __LOGIN_ACCOUNT_MSG_HANDLER_HPP__

#include "msg_handler.hpp"
#include "protos_fwd.hpp"
#include "account_manager.hpp"

class account_msg_handler_t
{
public:
    typedef bool (*msg_handler_func)(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

    static bool init_msg_handler();

    static bool handle_server_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr msg_buf)
    {
        log_trace("recv role[%lu] cmd[%s:%d] message from account[%d]", uid, CMD_DESC(cmd), cmd, account_manager_t::get_account_id_by_socket(s));

        msg_handler_func func = m_account_msg_handle.find_func(cmd);
        if ( NULL == func )
        {
            log_error("no handler func for msg cmd:%d", cmd);
            return false;
        }
        func(s, msg_buf);

        return true;
    };

    static bool handle_nl_syn_account_info_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
private:
    static network::server_msg_handler_t<op_cmd::NL_BEGIN, op_cmd::NL_END, msg_handler_func> m_account_msg_handle;
};

#endif
