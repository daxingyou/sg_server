#ifndef __CHAT_GAME_MSG_HANDLER_HPP__
#define __CHAT_GAME_MSG_HANDLER_HPP__

#include "main/chat_server.hpp"
#include "main/chat_server_fwd.hpp"
#include "msg_handler.hpp"
#include "protos_fwd.hpp"
#include "cmd_desc.hpp"
#include "game_manager.hpp"

class game_msg_handler_t
{
public:
    typedef bool (*msg_handler_func)(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

    static bool init_msg_handler();
    
    static bool handle_server_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr msg_buf)
    {
        log_trace("recv role[%lu] cmd[%s:%d] message from game[%d]", uid, CMD_DESC(cmd), cmd, game_manager_t::get_game_id_by_socket(s));

        msg_handler_func func = m_game_msg_handle.find_func(cmd);
        if (NULL == func)
        {
            log_error("no handler func for msg cmd:%d", cmd);
            return false;
        }
        func(s, msg_buf);

        return true;
    }

public:
	static bool handle_gh_syn_game_info_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);	
    static bool handle_gh_syn_online_role_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
    static bool handle_gh_kick_role_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_gh_get_nearby_roles_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_gh_task_seek_help_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_gh_gm_cmd_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_gh_syn_online_role_list_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_gh_reload_config_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_gh_chat_get_role_info_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

public:
    static network::server_msg_handler_t<op_cmd::GH_BEGIN, op_cmd::GH_END, msg_handler_func> m_game_msg_handle;
};

#endif
