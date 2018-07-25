#ifndef __DB_GAME_MSG_HANDLER_HPP__
#define __DB_GAME_MSG_HANDLER_HPP__

#include "msg_handler.hpp"
#include "op_cmd.hpp"
#include "gm.pb.h"
#include "gm_common.pb.h"
#include "game_manager.hpp"
#include "cmd_desc.hpp"

class game_msg_handler_t
{
public:
    typedef bool (*msg_handler_func)(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

    static bool init_msg_handler();

    static bool handle_server_msg(const network::tcp_socket_ptr& s, uint64_t uid,uint16_t cmd, const network::msg_buf_ptr msg_buf)
    {
         switch (cmd)
         {
         case op_cmd::gd_batch_save_to_db_notify:
             log_trace("recv role[%lu] cmd[%s:%d] message from game[%d]", uid, CMD_DESC(cmd), cmd, game_manager_t::get_game_id_by_socket(s));
             break;
         default:
             //log_trace("recv role[%lu] cmd[%s:%d] message from game[%d]", uid, CMD_DESC(cmd), cmd, game_manager_t::get_game_id_by_socket(s));
             break;
         }

        msg_handler_func func = m_game_msg_handler.find_func(cmd);
        if ( NULL == func )
        {
            log_error("no handler func for msg cmd: %d", cmd);
        }
        else
        {
            func(s, msg_buf);
        }

        return true;
    };

    static network::server_msg_handler_t<op_cmd::GD_BEGIN, op_cmd::GD_END, msg_handler_func> m_game_msg_handler;

public:
    static bool handle_gd_syn_game_info_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
    static bool handle_gd_role_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
    static bool handle_gd_batch_save_to_db_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_gd_reload_config_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_gd_change_name_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};

#endif
