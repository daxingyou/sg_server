#ifndef __DB_ACCOUNT_MSG_HANDLER_HPP__
#define __DB_ACCOUNT_MSG_HANDLER_HPP__

#include "main/db_server.hpp"
#include "main/db_server_fwd.hpp"
#include "msg_handler.hpp"
#include "protos_fwd.hpp"
#include "tcp_socket.hpp"
#include "cmd_desc.hpp"

class account_msg_handler_t
{
public:
    typedef bool (*msg_handler_func)(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

    static bool init_msg_handler();

	static bool handle_server_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr msg_buf)
	{
		log_trace("recv role[%lu] cmd[%s:%d] message from account", uid,CMD_DESC(cmd), cmd);

		msg_handler_func func = m_account_msg_handle.find_func(cmd);
		if (NULL == func)
		{
			log_error("no handler func for msg cmd: %d", cmd);
		}
		else
		{
			func(s, msg_buf);
		}

		return true;
	};

    static bool handle_nd_syn_account_info_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
    static bool handle_nd_create_role_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);


public:
	static network::server_msg_handler_t<op_cmd::ND_BEGIN, op_cmd::ND_END, msg_handler_func> m_account_msg_handle;
};

#endif 
