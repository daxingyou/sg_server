#ifndef __GATE_CENTER_MSG_HANDLER_HPP__
#define __GATE_CENTER_MSG_HANDLER_HPP__

#include "main/gate_server.hpp"
#include "main/gate_server_fwd.hpp"
#include "msg_handler.hpp"
#include "protos_fwd.hpp"
#include "cmd_desc.hpp"

class center_msg_handler_t
{
public:
    typedef bool (*msg_handler_func)(uint16_t cmd, const network::msg_buf_ptr& msg_buf);

    static bool init_msg_handler();
    
    static bool handle_server_msg(uint16_t cmd, const network::msg_buf_ptr msg_buf,  uint64_t uid)
    {
        msg_handler_func func = NULL;
        if (cmd > op_cmd::EA_BEGIN && cmd < op_cmd::EA_END)
        {
            switch (cmd)
            {
            case op_cmd::ea_broadcast_notify:
                break;
            default:
                log_trace("recv user[%lu] cmd[%s:%d] message from center", uid, CMD_DESC(cmd), cmd);
                break;
            }
            func = m_center_msg_handle.find_func(cmd);

        }
        else if (cmd > op_cmd::MA_BEGIN && cmd < op_cmd::MA_END)
        {
            log_trace("recv user[%lu] cmd[%s:%d] message from gm", uid, CMD_DESC(cmd), cmd);
            func = m_gm_msg_handle.find_func(cmd);
        }
        else
        {
            log_error("no register func for msg cmd[%s:%d]", CMD_DESC(cmd), cmd);
            return false;
        }

        if (NULL == func)
        {
            log_error("no handler func for msg cmd[%s:%d]", CMD_DESC(cmd), cmd);
            return false;
        }

        func(cmd, msg_buf);

        return true;
    }

public:
	// center => gate
	static bool handle_ea_broadcast_notify(uint16_t cmd, const network::msg_buf_ptr& msg_buf);
	static bool handle_ea_unreg_user_center_notify(uint16_t cmd, const network::msg_buf_ptr& msg_buf);
	static bool handle_ea_create_role_reply(uint16_t cmd, const network::msg_buf_ptr& msg_buf);

	// gm => gate
    static bool handle_ma_masked_msg_notify(uint16_t cmd, const network::msg_buf_ptr& msg_buf);
    static bool handle_ma_reload_config_notify(uint16_t cmd, const network::msg_buf_ptr& msg_buf);

public:
    static network::server_msg_handler_t<op_cmd::EA_BEGIN, op_cmd::EA_END, msg_handler_func> m_center_msg_handle;
	static network::server_msg_handler_t<op_cmd::MA_BEGIN, op_cmd::MA_END, msg_handler_func> m_gm_msg_handle;
};

#endif
