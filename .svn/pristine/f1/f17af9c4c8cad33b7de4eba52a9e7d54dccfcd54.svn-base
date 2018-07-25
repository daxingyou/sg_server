#ifndef __GAME_AREA_MSG_HANDLER_HPP__
#define __GAME_AREA_MSG_HANDLER_HPP__

#include "main/game_server.hpp"
#include "main/game_server_fwd.hpp"
#include "msg_handler.hpp"
#include "protos_fwd.hpp"
#include "cmd_desc.hpp"

class area_msg_handler_t
{
public:
    typedef bool (*msg_handler_func)(const network::msg_buf_ptr& msg_buf);

    static bool init_msg_handler();

    static bool handle_server_msg(uint64_t uid, uint16_t cmd, const network::msg_buf_ptr msg_buf )
    {
		log_trace("recv role[%lu] cmd[%s:%d] message from area", uid, CMD_DESC(cmd), cmd);
        msg_handler_func func = m_area_msg_handle.find_func(cmd);
		if (NULL == func)
		{
			log_error("no handler func for msg cmd:%d", cmd);
			return false;
		}
        
		func(msg_buf);
        return true;
    }

public:

	// area => cross
	static bool handle_rs_syn_area_info_notify(const network::msg_buf_ptr& msg_buf);
	static bool handle_rs_cross_fight_test_notify(const network::msg_buf_ptr& msg_buf);
	static bool handle_rs_arena_battle_notify(const network::msg_buf_ptr& msg_buf);
	static bool handle_rs_arena_battle_ai_notify(const network::msg_buf_ptr& msg_buf);

public:

    static network::server_msg_handler_t<op_cmd::RS_BEGIN, op_cmd::RS_END, msg_handler_func> m_area_msg_handle;
};

#endif
