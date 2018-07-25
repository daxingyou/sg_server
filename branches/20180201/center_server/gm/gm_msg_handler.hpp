#ifndef __CENTER_GM_MSG_HANDLER_HPP__
#define __CENTER_GM_MSG_HANDLER_HPP__

#include "main/center_server.hpp"
#include "main/center_server_fwd.hpp"
#include "msg_handler.hpp"
#include "log/log_wrapper.hpp"
#include "op_cmd.hpp"
#include "common/log_enum.hpp"
#include "gm_listener.hpp"
#include "gm_common.pb.h"
#include "gm.pb.h"
#include "common.pb.h"
#include "user/global_user_manager.hpp"
#include "cmd_desc.hpp"

class gm_msg_handler_t
{
public:
    typedef bool (*msg_handler_func)(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

    static bool init_msg_handler();
	
    static bool handle_server_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr msg_buf)
    {
        log_trace("recv role[%lu] cmd[%s:%d] message from gm", uid, CMD_DESC(cmd), cmd);

        msg_handler_func func = m_gm_msg_handler.find_func(cmd);
        if ( NULL == func )
        {
            log_error("no handler func for msg cmd:%d", cmd);
            return false;
        }
        func(s, msg_buf);

        return true;
    }
public:
	static bool handle_me_ban_account_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_me_access_ip_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_me_reload_config_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	static bool handle_me_send_mail_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_me_get_user_ol_num_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
public:
    static network::server_msg_handler_t<op_cmd::ME_BEGIN, op_cmd::ME_END, msg_handler_func> m_gm_msg_handler;
};

#endif
