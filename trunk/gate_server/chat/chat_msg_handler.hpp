#ifndef __GATE_CHAT_MSG_HANDLER_HPP__
#define __GATE_CHAT_MSG_HANDLER_HPP__

#include "msg_handler.hpp"
#include "protos_fwd.hpp"
#include "cmd_desc.hpp"

class chat_msg_handler_t
{
public:
    typedef bool (*msg_handler_func)(const network::msg_buf_ptr& msg_buf);

    static bool init_msg_handler();

    static bool handle_server_msg(uint64_t uid, uint16_t cmd, const network::msg_buf_ptr msg_buf)
    {
        log_trace("recv role[%lu] cmd[%s:%d] message from chat",uid, CMD_DESC(cmd), cmd);

        msg_handler_func func = m_chat_msg_handle.find_func(cmd);
        if ( NULL == func )
        {
            log_error("no handler func for msg cmd:%d", cmd);
            return false;
        }
        func(msg_buf);

        return true;
    };

public:
	static bool handle_ha_test_reply(const network::msg_buf_ptr& msg_buf);
	static bool handle_ha_change_server_channel_notify(const network::msg_buf_ptr& msg_buf);
	static bool handle_ha_server_channel_chat_notify(const network::msg_buf_ptr& msg_buf);
	static bool handle_ha_personal_chat_notify(const network::msg_buf_ptr& msg_buf);

private:
    static network::server_msg_handler_t<op_cmd::HA_BEGIN, op_cmd::HA_END, msg_handler_func> m_chat_msg_handle;
};

#endif
