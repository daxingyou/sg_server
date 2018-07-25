#pragma once

#include "msg_handler.hpp"
#include "protos_fwd.hpp"
#include "tcp_socket.hpp"
#include "cmd_desc.hpp"
#include "transfer/transfer_manager.hpp"

class transfer_msg_handler_t
{
public:
    typedef bool (*msg_handler_func)(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

    static bool init_msg_handler();

    static bool handle_server_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr msg_buf)
    {
		uint32_t transfer_id = transfer_manager_t::get_transfer_id_by_socket(s);

        log_trace("recv role[%lu] cmd[%s:%d] message from transfer[%d]", uid,CMD_DESC(cmd), cmd, transfer_id);

		msg_handler_func func = NULL;
		if (cmd >= op_cmd::FE_BEGIN && cmd < op_cmd::FE_END)
		{
			func = m_transfer_msg_handler.find_func(cmd);
		}
		else if (cmd >= op_cmd::RG_BEGIN && cmd < op_cmd::RG_END)
		{
			func = m_area_2_game_msg_handler.find_func(cmd);
		}
		else if (cmd >= op_cmd::RE_BEGIN && cmd < op_cmd::RE_END)
		{
			func = m_area_2_center_msg_handler.find_func(cmd);
		}
        if ( NULL == func )
        {
            log_error("no handler func for msg cmd:%d", cmd);
            return false;
        }
        func(s, msg_buf);

        return true;
    };

public:
	
	static bool handle_fe_syn_transfer_info_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_fe_test_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	
	static bool handle_rg_arena_match_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_rg_arena_battle_result_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_rg_get_arena_rank_data_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_rg_world_cup_play_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_rg_world_cup_bet_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_rg_world_cup_bet_reward_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_rg_arena_cancel_match_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_rg_arena_self_rank_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	
	static bool handle_re_world_cup_data_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_re_world_cup_mail_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_re_world_cup_coming_quarter_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_re_world_cup_coming_soon_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_re_world_cup_ready_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_re_world_cup_update_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_re_world_cup_bet_data_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_re_world_cup_odds_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
private:
	static network::server_msg_handler_t<op_cmd::FE_BEGIN, op_cmd::FE_END, msg_handler_func> m_transfer_msg_handler;
	static network::server_msg_handler_t<op_cmd::RG_BEGIN, op_cmd::RG_END, msg_handler_func> m_area_2_game_msg_handler;
	static network::server_msg_handler_t<op_cmd::RE_BEGIN, op_cmd::RE_END, msg_handler_func> m_area_2_center_msg_handler;
}; 
