#ifndef __CENTER_GATE_MSG_HANDLER_HPP__
#define __CENTER_GATE_MSG_HANDLER_HPP__

#include "main/center_server.hpp"
#include "main/center_server_fwd.hpp"
#include "msg_handler.hpp"
#include "protos_fwd.hpp"
#include "tcp_socket.hpp"
#include "gate_manager.hpp"
#include "cmd_desc.hpp"
#include "log/log_unify_save.hpp"

class gate_msg_handler_t
{
public:
    typedef bool (*msg_handler_func)(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

    static bool init_msg_handler();

    static bool handle_server_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr msg_buf)
    {
        msg_handler_func func;
        if (cmd > op_cmd::CE_BEGIN && cmd < op_cmd::CE_END)
        {
            log_trace("recv role[%lu] cmd[%s:%d] message from client", uid, CMD_DESC(cmd), cmd);
            func = m_gate_msg_handle_from_client.find_func(cmd);
            if ( NULL == func )
            {
                log_error("no handler func for msg cmd:%d", cmd);
                return false;
            }
        }
        else if (cmd > op_cmd::AE_BEGIN && cmd < op_cmd::AE_END)
        {
            log_trace("recv role[%lu] cmd[%s:%d] message from gate[%d]", uid, CMD_DESC(cmd), cmd, gate_manager_t::get_gate_id_by_socket(s));
            
			func = m_gate_msg_handle.find_func(cmd);
            if ( NULL == func )
            {
                log_error("no handler func for msg cmd:[%s:%d]", CMD_DESC(cmd), cmd);
                return false;
            }
        }
        else
        {
            log_error("cmd[%s:%d] error!", CMD_DESC(cmd), cmd);
            return false;
        }

        func(s, msg_buf);
        log_unify_save::save();
        return true;
    }

public:
    static bool handle_ae_syn_gate_info_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ae_reg_user_center_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ae_create_role_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ae_unreg_user_scene_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ae_user_cross_server_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

    static network::server_msg_handler_t<op_cmd::AE_BEGIN, op_cmd::AE_END, msg_handler_func> m_gate_msg_handle;
    static network::server_msg_handler_t<op_cmd::CE_BEGIN, op_cmd::CE_END, msg_handler_func> m_gate_msg_handle_from_client;

};

#endif
