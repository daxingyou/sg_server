#ifndef __GAME_DB_MSG_HANDLER_HPP__
#define __GAME_DB_MSG_HANDLER_HPP__

#include "main/game_server.hpp"
#include "main/game_server_fwd.hpp"
#include "msg_handler.hpp"
#include "protos_fwd.hpp"
#include "role/role_manager.hpp"
#include "gm.pb.h"
#include "gm_common.pb.h"
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

        //role_unify_save::save();
        //log_unify_save::save();

        return true;
    }

   static bool handle_dg_role_data_reply(const network::msg_buf_ptr& msg_buf);
   static bool handle_dg_change_name_reply(const network::msg_buf_ptr& msg_buf);

public:
    static network::server_msg_handler_t<op_cmd::DG_BEGIN, op_cmd::DG_END, msg_handler_func> m_db_msg_handle;
};

#endif
