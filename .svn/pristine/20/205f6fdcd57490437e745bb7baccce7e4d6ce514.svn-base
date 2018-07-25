#ifndef __GM_CENTER_MSG_HANDLER_HPP__
#define __GM_CENTER_MSG_HANDLER_HPP__

#include "main/gm_server.hpp"
#include "msg_handler.hpp"
#include "gm_common.pb.h"
#include "gm.pb.h"
#include "cmd_desc.hpp"

class center_msg_handler_t
{
public:
    typedef bool (*msg_handler_func)(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

    static bool init_msg_handler();

    static bool init_center_msg_handler();

    static bool handle_server_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr msg_buf )
    {
        log_trace("recv role[%lu] cmd[%s:%d] message from center", uid,CMD_DESC(cmd), cmd);

		msg_handler_func func = NULL;
		if (cmd > op_cmd::EM_BEGIN && cmd < op_cmd::EM_END)
		{
			func = m_center_msg_handler.find_func(cmd);
		}
		else if (cmd > op_cmd::GM_BEGIN && cmd < op_cmd::GM_END)
		{
			func = m_game_msg_handler.find_func(cmd);
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
    }

public:
	static bool handle_gm_data_threshold_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
    
	static bool handle_em_get_user_ol_num_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	static bool handle_em_manage_role_ntf(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
public:
    static network::server_msg_handler_t<op_cmd::EM_BEGIN, op_cmd::EM_END, msg_handler_func> m_center_msg_handler;
	static network::server_msg_handler_t<op_cmd::GM_BEGIN, op_cmd::GM_END, msg_handler_func> m_game_msg_handler;
};

#endif
