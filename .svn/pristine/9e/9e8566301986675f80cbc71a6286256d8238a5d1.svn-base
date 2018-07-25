#ifndef __TRANSFER_CHAT_MSG_HANDLER_HPP__
#define __TRANSFER_CHAT_MSG_HANDLER_HPP__

#include "msg_handler.hpp"
#include "protos_fwd.hpp"
#include "tcp_socket.hpp"
#include "cmd_desc.hpp"
#include "chat_manager.hpp"

class chat_msg_handler_t
{
public:
    typedef bool (*msg_handler_func)(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

    static bool init_msg_handler();

    static bool handle_server_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr msg_buf)
    {
        chat_ptr p_chat = chat_manager_t::get_chat(s);
        if (NULL == p_chat)
        {
            log_error("recv role[%lu] cmd[%s:%d] message from chat failed: NULL == p_chat", uid,CMD_DESC(cmd), cmd);
            return false;
        }
        log_trace("recv cmd[%s:%d] message from chat[server_id=%d]", CMD_DESC(cmd), cmd, p_chat->get_server_id());

        msg_handler_func func = m_chat_msg_handle.find_func(cmd);
        if ( NULL == func )
        {
            log_error("no handler func for msg cmd:%d", cmd);
            return false;
        }
        func(s, msg_buf);

        return true;
    };

public:
	static bool handle_hf_syn_chat_info_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_hf_get_server_channel_list_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_hf_change_server_channel_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_hf_server_channel_chat_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_hf_gm_cmd_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_hf_personal_chat_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
private:
    static network::server_msg_handler_t<op_cmd::HF_BEGIN, op_cmd::HF_END, msg_handler_func> m_chat_msg_handle;
};

#endif
