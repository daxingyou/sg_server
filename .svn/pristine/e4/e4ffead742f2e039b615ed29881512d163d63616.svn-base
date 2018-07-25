#ifndef __DB_CENTER_MSG_HANDLER_HPP__
#define __DB_CENTER_MSG_HANDLER_HPP__

#include "msg_handler.hpp"
#include "op_cmd.hpp"
#include "log/log_wrapper.hpp"
#include "cmd_desc.hpp"
#include "protos_fwd.hpp"


class chat_msg_handler_t
{
public:
    typedef bool (*msg_handler_func)(const network::msg_buf_ptr& msg_buf);

    static bool init_msg_handler();

	static bool init_client_msg_handler();

    static bool handle_server_msg(uint64_t uid,uint16_t cmd, const network::msg_buf_ptr msg_buf)
    {
        log_trace("recv role[%lu] cmd[%s:%d] message from center", uid,CMD_DESC(cmd), cmd);

        msg_handler_func func = m_chat_msg_handler.find_func(cmd);
        if ( NULL == func )
        {
            log_error("no handler func for msg cmd: %d", cmd);
        }
        else
        {
            func(msg_buf);
        }

        return true;
    };

public:

	static bool handle_he_test_reply(const network::msg_buf_ptr& msg_buf);
	static bool handle_he_family_chat_notify(const network::msg_buf_ptr& msg_buf);
	static bool one_chat_to_another(uint64_t uid, const proto::client::ce_personal_chat_request& req, proto::client::ec_personal_chat_reply* reply = NULL);
public:
	static bool handle_ce_personal_chat_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ce_personal_chat_notice_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	
public:
    static network::server_msg_handler_t<op_cmd::HE_BEGIN, op_cmd::HE_END, msg_handler_func> m_chat_msg_handler;
private:
	static int32_t check_chat_info_valid(std::string& chat_info);
};

#endif
