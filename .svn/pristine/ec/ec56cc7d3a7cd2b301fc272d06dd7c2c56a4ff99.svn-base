#ifndef __CENTER_DB_MSG_HANDLER_HPP__
#define __CENTER_DB_MSG_HANDLER_HPP__

#include "main/center_server.hpp"
#include "main/center_server_fwd.hpp"
#include "msg_handler.hpp"
#include "protos_fwd.hpp"
#include "cmd_desc.hpp"

class db_msg_handler_t
{
public:
    typedef bool (*msg_handler_func)(const network::msg_buf_ptr& msg_buf);

    static bool init_msg_handler();

    static bool handle_server_msg(uint64_t uid, uint16_t cmd, const network::msg_buf_ptr msg_buf)
    {
        log_trace("recv role[%lu] cmd[%s:%d] message from db", uid, CMD_DESC(cmd), cmd);

        msg_handler_func func = m_db_msg_handle.find_func(cmd);
        if ( NULL == func )
        {
            log_error("no handler func for msg cmd:%d", cmd);
            return false;
        }

        func(msg_buf);

        return true;
    }

public:
    static bool handle_de_create_role_reply(const network::msg_buf_ptr& msg_buf);

	static bool handle_de_change_name_notify(const network::msg_buf_ptr& msg_buf);
private:
    static network::server_msg_handler_t<op_cmd::DE_BEGIN, op_cmd::DE_END, msg_handler_func> m_db_msg_handle;
};

#endif
