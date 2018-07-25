#pragma once

#include "msg_handler.hpp"
#include "protos_fwd.hpp"
#include "cmd_desc.hpp"
#include "center_manager.hpp"

class center_msg_handler_t
{
public:
    typedef bool (*msg_handler_func)(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

    static bool init_msg_handler();

    static bool handle_server_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr msg_buf)
	{
        center_ptr p_center = center_manager_t::get_center_by_socket(s);
        if (NULL == p_center)
        {
            log_error("recv role[%lu] cmd[%s:%d] message from center failed: NULL == p_center", uid, CMD_DESC(cmd), cmd);
            return false;
        }
		log_trace("recv role[%lu] cmd[%s:%d] message from center[server_id=%d]", uid, CMD_DESC(cmd), cmd, p_center->get_id());

		msg_handler_func func = m_center_msg_handle.find_func(cmd);
		if (NULL == func)
		{
			log_error("no handler func for msg cmd:%d", cmd);
			return false;
		}
		func(s, msg_buf);

		return true;
    };


public:
	static bool handle_ef_syn_center_info_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ef_test_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	
private:
    static network::server_msg_handler_t<op_cmd::EF_BEGIN, op_cmd::EF_END, msg_handler_func> m_center_msg_handle;
};
