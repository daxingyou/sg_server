#ifndef __GAME_CHAT_MSG_HANDLER_HPP__
#define __GAME_CHAT_MSG_HANDLER_HPP__

#include "main/game_server.hpp"
#include "main/game_server_fwd.hpp"
#include "msg_handler.hpp"
#include "protos_fwd.hpp"
#include "role/role_manager.hpp"
#include "cmd_desc.hpp"

class chat_msg_handler_t
{
public:
    typedef bool(*msg_handler_func_1)(role_ptr p_role, const network::msg_buf_ptr& msg_buf);
    typedef bool(*msg_handler_func_2)(const network::msg_buf_ptr& msg_buf);

    static bool init_msg_handler();
    
    static bool handle_server_msg(uint64_t uid, uint16_t cmd, const network::msg_buf_ptr msg_buf)
    {
        log_trace("recv role[%lu] cmd[%s:%d] message from chat", uid, CMD_DESC(cmd), cmd);

        msg_handler_func_1 func = m_chat_msg_handle_1.find_func(cmd);
        if (NULL == func)
        {
            log_error("no handler func for msg cmd:%d", cmd);
            return false;
        }
		role_ptr p_role = role_manager_t::find_role(uid);
		if (NULL == p_role && cmd != op_cmd::hg_syn_online_role_list_request)
		{
			log_error("role[%lu] not found by cmd[%s:%d]", uid, CMD_DESC(cmd), cmd);
			return true;
		}
        func(p_role, msg_buf);

        //role_unify_save::save();
        //log_unify_save::save();

        return true;
    }

    static bool handle_server_msg(uint16_t cmd, const network::msg_buf_ptr msg_buf)
    {
        log_trace("recv cmd[%s:%d] message from chat", CMD_DESC(cmd), cmd);

        msg_handler_func_2 func = m_chat_msg_handle_2.find_func(cmd);
        if (NULL == func)
        {
            log_error("no handler func for msg cmd:%d", cmd);
            return false;
        }
        func(msg_buf);

        return true;
    }

public:
	static bool handle_hg_test_notify(role_ptr p_role, const network::msg_buf_ptr& msg_buf);
	static bool handle_hg_get_nearby_roles_request(role_ptr p_role, const network::msg_buf_ptr& msg_buf);
	static bool handle_hg_task_seek_help_request(role_ptr p_role, const network::msg_buf_ptr& msg_buf);
	static bool handle_hg_syn_online_role_list_request(role_ptr p_role, const network::msg_buf_ptr& msg_buf);
	static bool handle_hg_user_channel_notify(role_ptr p_role, const network::msg_buf_ptr& msg_buf); 
	static bool handle_hg_chat_get_role_info_request(role_ptr p_role, const network::msg_buf_ptr& msg_buf);
	static bool handle_hg_notify_progress_state_notify(role_ptr p_role, const network::msg_buf_ptr& msg_buf);
public:
    static network::server_msg_handler_t<op_cmd::HG_BEGIN, op_cmd::HG_END, msg_handler_func_1> m_chat_msg_handle_1;
    static network::server_msg_handler_t<op_cmd::HG_BEGIN, op_cmd::HG_END, msg_handler_func_2> m_chat_msg_handle_2;
};

#endif
