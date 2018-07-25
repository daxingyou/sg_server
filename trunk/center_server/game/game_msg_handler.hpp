#ifndef __CENTER_GAME_MSG_HANDLER_HPP__
#define __CENTER_GAME_MSG_HANDLER_HPP__

#include "main/center_server.hpp"
#include "main/center_server_fwd.hpp"
#include "msg_handler.hpp"
#include "protos_fwd.hpp"
#include "tcp_socket.hpp"
#include "game_manager.hpp"
#include "cmd_desc.hpp"
#include "log/log_unify_save.hpp"
class game_msg_handler_t
{
public:
    typedef bool (*msg_handler_func)(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

    static bool init_msg_handler();

    static bool handle_server_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr msg_buf)
    {
        log_trace("recv role[%lu] cmd[%s:%d] message from game[%d]", uid, CMD_DESC(cmd), cmd, game_manager_t::get_game_id_by_socket(s));

        msg_handler_func func = m_game_msg_handle.find_func(cmd);
        if ( NULL == func )
        {
            log_error("no handler func for msg cmd:%d", cmd);
            return false;
        }

        func(s, msg_buf);
        //log_unify_save::save();

        return true;
    }

public:
    static network::server_msg_handler_t<op_cmd::GE_BEGIN, op_cmd::GE_END, msg_handler_func> m_game_msg_handle;

};

#endif
