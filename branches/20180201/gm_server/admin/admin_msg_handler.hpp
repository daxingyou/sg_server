#ifndef __GM_ADMIN_MSG_HANDLER_HPP__
#define __GM_ADMIN_MSG_HANDLER_HPP__

#include "msg_handler.hpp"
#include "gm_common.pb.h"
#include "gm.pb.h"
#include "network.hpp"
#include <sys/time.h>
#include "cmd_desc.hpp"

class admin_msg_handler_t
{
public:
    typedef bool (*msg_handler_func)(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

    static bool init_msg_handler();

	static bool init_center_msg_handler();

    static bool handle_server_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr msg_buf)
    {
        log_trace("recv role[%lu] cmd[%s:%d] message from gm tool", uid,CMD_DESC(cmd), cmd);

        msg_handler_func func = m_admin_msg_handler.find_func(cmd);
        if ( NULL == func )
        {
            log_error("no handler func for msg cmd:%d", cmd);
            return false;
        }
        func(s, msg_buf);

        return true;
    }

public:
	static bool handle_tm_test_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	static bool handle_tm_send_mail_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	static bool handle_tm_get_user_ol_num_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

private:
    static network::server_msg_handler_t<op_cmd::TM_BEGIN, op_cmd::TM_END, msg_handler_func> m_admin_msg_handler;
};

#endif
