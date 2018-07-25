#ifndef __DB_ACCOUNT_MSG_HANDLER_HPP__
#define __DB_ACCOUNT_MSG_HANDLER_HPP__

#include "msg_handler.hpp"
#include "protos_fwd.hpp"
#include "cmd_desc.hpp"

class db_msg_handler_t
{
public:
    typedef bool (*msg_handler_func)(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

    static bool init_msg_handler();

    static bool handle_server_msg(const network::tcp_socket_ptr& s,uint64_t uid, uint16_t cmd, const network::msg_buf_ptr msg_buf)
    {
        log_trace("recv role[%lu] cmd[%s:%d] message from db", uid, CMD_DESC(cmd), cmd);

        msg_handler_func func = m_db_msg_handle.find_func(cmd);
        if ( NULL == func )
        {
            log_error("no handler func for msg cmd:%d", cmd);
            return false;
        }
        func(s, msg_buf);

        return true;
    };

    static bool handle_dn_create_role_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
    static bool handle_dn_role_data_update_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

private:
    static network::server_msg_handler_t<op_cmd::DN_BEGIN, op_cmd::DN_END, msg_handler_func> m_db_msg_handle;
};

#endif
