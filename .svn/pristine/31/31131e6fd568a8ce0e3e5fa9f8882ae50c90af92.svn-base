#ifndef __DB_CENTER_MSG_HANDLER_HPP__
#define __DB_CENTER_MSG_HANDLER_HPP__

#include "msg_handler.hpp"
#include "op_cmd.hpp"
#include "gm.pb.h"
#include "gm_common.pb.h"
#include "cmd_desc.hpp"

#define  MIN_BAN_OFFSET_TIME   60*5    //最少封号时间为5min

class center_msg_handler_t
{
public:
    typedef bool (*msg_handler_func)(const network::msg_buf_ptr& msg_buf);

    static bool init_msg_handler();

    static bool handle_server_msg(uint64_t uid,uint16_t cmd, const network::msg_buf_ptr msg_buf)
    {
        log_trace("recv role[%lu] cmd[%s:%d] message from center", uid,CMD_DESC(cmd), cmd);

        msg_handler_func func = m_center_msg_handler.find_func(cmd);
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
	static bool handle_ed_create_role_request(const network::msg_buf_ptr& msg_buf);
	static bool handle_ed_role_leave_game_notify(const network::msg_buf_ptr& msg_buf);

public:
    static network::server_msg_handler_t<op_cmd::ED_BEGIN, op_cmd::ED_END, msg_handler_func> m_center_msg_handler;
};

#endif
