#ifndef __LOG_CENTER_MSG_HANDLER_HPP__
#define __LOG_CENTER_MSG_HANDLER_HPP__

#include "msg_handler.hpp"
#include "protos_fwd.hpp"
#include "cmd_desc.hpp"

class center_msg_handler_t
{
public:
    typedef bool (*msg_handler_func)(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

    static bool init_msg_handler();

    static bool handle_server_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr msg_buf)
    {
        switch (cmd)
        {
        case op_cmd::eo_batch_save_logs_notify:
            break;
        default:
			log_trace("recv role[%lu] cmd[%s:%d] message from center", uid,CMD_DESC(cmd), cmd);            
            break;
        }

        msg_handler_func func = m_center_msg_handle.find_func(cmd);
        if ( NULL == func )
        {
            log_error("no handler func for msg cmd:%d", cmd);
            return false;
        }
        func(s, msg_buf);

        return true;
    };

public:
    static bool handle_batch_save_logs_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

public:
    static void add_logs(uint16_t cmd, const std::string& msg_buf);

public:
    static network::server_msg_handler_t<op_cmd::EO_BEGIN, op_cmd::EO_END, msg_handler_func> m_center_msg_handle;
};

#endif
