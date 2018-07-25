#ifndef __CROSS_MSG_HANDLER_HPP__
#define __CROSS_MSG_HANDLER_HPP__

#include "main/area_server.hpp"
#include "main/area_server_fwd.hpp"
#include "msg_handler.hpp"
#include "protos_fwd.hpp"
#include "tcp_socket.hpp"
#include "cross_manager.hpp"
#include "cmd_desc.hpp"

class cross_msg_handler_t
{
public:
    typedef bool (*msg_handler_func)(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

    static bool init_msg_handler();

    static bool handle_server_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr msg_buf)
    {
        log_trace("recv role[%lu] cmd[%s:%d] message from cross[%d]", uid, CMD_DESC(cmd), cmd, cross_manager_t::get_cross_id_by_socket(s));

        msg_handler_func func = m_cross_msg_handle.find_func(cmd);
        if ( NULL == func )
        {
            log_error("no handler func for msg cmd:%d", cmd);
            return false;
        }

        func(s, msg_buf);

        return true;
    }

public:
	static bool handle_sr_syn_cross_info_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_sr_arena_battle_result_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

public:

    static network::server_msg_handler_t<op_cmd::SR_BEGIN, op_cmd::SR_END, msg_handler_func> m_cross_msg_handle;
};

#endif
