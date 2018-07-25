#ifndef __LOGIN_GATE_MSG_HANDLER_HPP__
#define __LOGIN_GATE_MSG_HANDLER_HPP__

#include "msg_handler.hpp"
#include "protos_fwd.hpp"
#include "gate_manager.hpp"

class gate_msg_handler_t
{
public:
    typedef bool (*msg_handler_func)(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

    static bool init_msg_handler();

    static bool handle_server_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr msg_buf)
    {
        log_trace("recv role[%lu] cmd[%s:%d] message from gate[%d]", uid, CMD_DESC(cmd), cmd, gate_manager_t::get_gate_id_by_socket(s));

        msg_handler_func func = m_gate_msg_handle.find_func(cmd);
        if ( NULL == func )
        {
            log_error("no handler func for msg cmd:%d", cmd);
            return false;
        }
        func(s, msg_buf);

        return true;
    };

    static bool handle_al_update_gate_user_num_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
    static bool handle_al_login_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
    static bool handle_al_kick_role_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_al_kick_gate_all_user_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
    static bool handle_al_syn_gate_info_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
    static bool handle_al_send_gm_msg_to_account_requst(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
private:
    static network::server_msg_handler_t<op_cmd::AL_BEGIN, op_cmd::AL_END, msg_handler_func> m_gate_msg_handle;
};

#endif
