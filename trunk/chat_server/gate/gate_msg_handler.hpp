#ifndef __CHAT_GATE_MSG_HANDLER_HPP__
#define __CHAT_GATE_MSG_HANDLER_HPP__

#include "main/chat_server.hpp"
#include "main/chat_server_fwd.hpp"
#include "msg_handler.hpp"
#include "protos_fwd.hpp"
#include "tcp_socket.hpp"
#include "cmd_desc.hpp"
#include "gate_manager.hpp"

class gate_msg_handler_t
{
public:
    typedef bool (*msg_handler_func)(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

    static bool init_msg_handler();

    static bool handle_server_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr msg_buf)
    {
        msg_handler_func func;
        if (cmd > op_cmd::CH_BEGIN && cmd < op_cmd::CH_END)
        {
            log_trace("recv role[%lu] cmd[%s:%d] message from client to gate[%d]", uid, CMD_DESC(cmd), cmd, gate_manager_t::get_gate_id_by_socket(s));
            func = m_gate_msg_handle_from_client.find_func(cmd);
            if ( NULL == func )
            {
                log_error("no handler func for msg cmd:%d", cmd);
                return false;
            }
        }
        else if (cmd > op_cmd::AH_BEGIN && cmd < op_cmd::AH_END)
        {
            log_trace("recv role[%lu] cmd[%s:%d] message from gate[%d]", uid, CMD_DESC(cmd), cmd, gate_manager_t::get_gate_id_by_socket(s));
            func = m_gate_msg_handle.find_func(cmd);
            if ( NULL == func )
            {
                log_error("no handler func for msg cmd:%d", cmd);
                return false;
            }
        }
        else
        {
            log_error("cmd[%s:%d] error!", CMD_DESC(cmd), cmd);
            return false;
        }

        func(s, msg_buf);

        return true;
    }


    // client --> gate -- > chat
	static bool handle_ch_test_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ch_channel_chat_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ch_get_server_channel_list_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ch_change_server_channel_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ch_server_channel_chat_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	//static bool handle_ch_personal_chat_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ch_hero_strategy_hero_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ch_hero_strategy_get_comments_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ch_hero_strategy_like_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_ch_hero_strategy_create_comment_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	
    // gate --> chat
    static bool handle_ah_syn_gate_info_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

public:
    static network::server_msg_handler_t<op_cmd::CH_BEGIN, op_cmd::CH_END, msg_handler_func> m_gate_msg_handle_from_client;
    static network::server_msg_handler_t<op_cmd::AH_BEGIN, op_cmd::AH_END, msg_handler_func> m_gate_msg_handle;

	static int32_t check_chat_info_valid(std::string& chat_info);
};

#endif 
