#ifndef __GAME_GATE_MSG_HANDLER_HPP__
#define __GAME_GATE_MSG_HANDLER_HPP__

#include "main/game_server.hpp"
#include "main/game_server_fwd.hpp"
#include "msg_handler.hpp"
#include "protos_fwd.hpp"
#include "tcp_socket.hpp"
#include "role/role_unify_save.hpp"
#include "log/log_unify_save.hpp"
#include "role/role_manager.hpp"
#include "gate_manager.hpp"
#include "cmd_desc.hpp"

class gate_msg_handler_t
{
public:
    typedef bool (*msg_handler_func)(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

    static bool init_msg_handler();

    static bool handle_server_msg(const network::tcp_socket_ptr& s, uint16_t cmd, const network::msg_buf_ptr msg_buf, uint64_t uid)
    {
        msg_handler_func func;
        if (cmd > op_cmd::CG_BEGIN && cmd < op_cmd::CG_END)
        {
			switch (cmd)
			{
				case op_cmd::cg_move_request:
				case op_cmd::cg_server_time_request:
					break;
				default:
					log_trace("recv role[%lu] cmd[%s:%d] message from client", uid, CMD_DESC(cmd), cmd);
					break;
			}

            func = m_gate_msg_handle_from_client.find_func(cmd);
            if ( NULL == func )
            {
                log_error("no handler func for msg cmd:%d", cmd);
                return false;
            }
        }
        else if (cmd > op_cmd::AG_BEGIN && cmd < op_cmd::AG_END)
        {
            gate_ptr p_gate = gate_manager_t::get_gate_by_socket(s);
            if (NULL == p_gate)
            {
                log_error("recv role[%lu] cmd[%s:%d] message from gate failed: NULL == p_gate", uid, CMD_DESC(cmd), cmd);
                return false;
            }
            uint32_t gate_id = p_gate->get_id();
            uint32_t server_id = p_gate->get_server_id();
            log_trace("recv role[%lu] cmd[%s:%d] message from gate[server_id=%d: gate_id=%d]", uid, CMD_DESC(cmd), cmd, server_id, gate_id);
            
			func = m_gate_msg_handle.find_func(cmd);
            if ( NULL == func )
            {
                log_error("no handler func for msg cmd:%d", cmd);
                return false;
            }
        }
        else
        {
            log_error("cmd[%s:%d] error!", CMD_DESC(cmd), cmd);
            return false;
        }

        func(s, msg_buf);

        //role_unify_save::save();
        //log_unify_save::save();

        return true;
    }

    // gate --> game
    static bool handle_ag_syn_gate_info_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ag_unreg_user_scene_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ag_role_enter_game_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	

    // client --> gate -- > game

public:
    static network::server_msg_handler_t<op_cmd::CG_BEGIN, op_cmd::CG_END, msg_handler_func> m_gate_msg_handle_from_client;
    static network::server_msg_handler_t<op_cmd::AG_BEGIN, op_cmd::AG_END, msg_handler_func> m_gate_msg_handle;
};

#endif 
