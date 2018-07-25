#ifndef __CHAT_TRANSFER_MSG_HANDLER_HPP__
#define __CHAT_TRANSFER_MSG_HANDLER_HPP__

#include "main/chat_server.hpp"
#include "main/chat_server_fwd.hpp"
#include "msg_handler.hpp"
#include "protos_fwd.hpp"
#include "cmd_desc.hpp"

class transfer_msg_handler_t
{
public:
    typedef bool (*msg_handler_func)(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

    static bool init_msg_handler();
    
    static bool handle_server_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr msg_buf)
    {
        log_trace("recv role[%lu] cmd[%s:%d] message from transfer[%d]", uid,CMD_DESC(cmd), cmd, transfer_manager_t::get_transfer_id_by_socket(s));

		msg_handler_func func = NULL;
		if (cmd >= op_cmd::RH_BEGIN && cmd < op_cmd::RH_END)
		{
			func = m_area_msg_handle.find_func(cmd);
		}
		else if (cmd >= op_cmd::FH_BEGIN && cmd < op_cmd::FH_END)
		{
			func = m_transfer_msg_handle.find_func(cmd);
		}
        if (NULL == func)
        {
            log_error("no handler func for msg cmd:%d", cmd);
            return false;
        }
        func(s, msg_buf);

        return true;
    }

public:
	static bool handle_fh_test_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_fh_get_server_channel_list_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_fh_change_server_channel_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_fh_change_server_channel_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_fh_server_channel_chat_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_fh_server_channel_chat_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_fh_personal_chat_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_fh_syn_transfer_info_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_rh_get_hero_strategy_info_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	
public:
    static network::server_msg_handler_t<op_cmd::FH_BEGIN, op_cmd::FH_END, msg_handler_func> m_transfer_msg_handle;
	static network::server_msg_handler_t<op_cmd::RH_BEGIN, op_cmd::RH_END, msg_handler_func> m_area_msg_handle;
};

#endif
