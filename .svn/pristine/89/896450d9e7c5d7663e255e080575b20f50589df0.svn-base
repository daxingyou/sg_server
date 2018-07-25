#ifndef __GATE_GAME_MSG_HANDLER_HPP__
#define __GATE_GAME_MSG_HANDLER_HPP__

#include "msg_handler.hpp"
#include "protos_fwd.hpp"
#include "cmd_desc.hpp"
#include "game_manager.hpp"

class game_msg_handler_t
{
public:
    typedef bool (*msg_handler_func)(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

    static bool init_msg_handler();
	static bool init_cross_msg_handler();
	static void set_is_batch_move(uint32_t val);

    static bool handle_server_msg(const network::tcp_socket_ptr& s,uint64_t uid, uint16_t cmd, const network::msg_buf_ptr msg_buf)
    {
        msg_handler_func func = NULL;
        if (cmd > op_cmd::GA_BEGIN && cmd < op_cmd::GA_END)
        {
            switch (cmd)
            {
            case op_cmd::ga_role_move_notify:
            case op_cmd::ga_role_into_vision_notify:
            case op_cmd::ga_role_out_vision_notify:
            case op_cmd::ga_broadcast_notify:
                break;
            default:
                log_trace("recv role[%lu] cmd[%s:%d] message from game[%d]", uid, CMD_DESC(cmd), cmd, game_manager_t::get_game_id_by_socket(s));
                break;
            }
            func = m_game_msg_handle.find_func(cmd);
        }
        else
        {
            log_error("no register func for msg cmd[%s:%d]", CMD_DESC(cmd), cmd);
            return false;
        }

        if (NULL == func)
        {
            log_error("no register func for msg cmd[%s:%d]", CMD_DESC(cmd), cmd);
            return false;
        }

        func(s, msg_buf);
        return true;
    };

	static bool check_cross_msg(uint16_t cmd);

public:
    static bool handle_ga_role_move_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
    static bool handle_ga_role_into_vision_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
    static bool handle_ga_role_out_vision_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
    static bool handle_ga_syn_game_info_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ga_broadcast_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ga_reload_config_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ga_syn_cross_id_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ga_unreg_user_scene_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ga_reg_user_success_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ga_user_cross_server_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	
private:
    static network::server_msg_handler_t<op_cmd::GA_BEGIN, op_cmd::GA_END, msg_handler_func> m_game_msg_handle;
	static std::set<uint16_t> m_cross_msg_register;
	static uint32_t m_is_batch_move;
};

#endif
