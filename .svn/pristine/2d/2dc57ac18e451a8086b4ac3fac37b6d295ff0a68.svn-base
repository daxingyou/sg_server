#pragma once

#include "msg_handler.hpp"
#include "protos_fwd.hpp"
#include "tcp_socket.hpp"
#include "cmd_desc.hpp"
#include "transfer_manager.hpp"

class transfer_msg_handler_t
{
public:
    typedef bool (*msg_handler_func)(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

    static bool init_msg_handler();

    static bool handle_server_msg(const network::tcp_socket_ptr& s, uint64_t uid, uint16_t cmd, const network::msg_buf_ptr msg_buf)
    {

        msg_handler_func func = NULL;
		if (cmd >= op_cmd::FR_BEGIN && cmd < op_cmd::FR_END)
		{
            log_trace("recv role[%lu] cmd[%s:%d] message from transfer[%d]", uid,CMD_DESC(cmd), cmd, transfer_manager_t::get_transfer_id_by_socket(s));
			func = m_transfer_msg_handle.find_func(cmd);
		}
		else if (cmd >= op_cmd::GR_BEGIN && cmd < op_cmd::GR_END)
		{
            log_trace("recv role[%lu] cmd[%s:%d] message from game", uid, CMD_DESC(cmd), cmd);
			func = m_game_msg_handle.find_func(cmd);
		}
		else if (cmd >= op_cmd::ER_BEGIN && cmd < op_cmd::ER_END)
		{
			log_trace("recv role[%lu] cmd[%s:%d] message from center", uid, CMD_DESC(cmd), cmd);
			func = m_center_msg_handle.find_func(cmd);
		}
		else if (cmd >= op_cmd::HR_BEGIN && cmd < op_cmd::HR_END)
		{
			log_trace("recv role[%lu] cmd[%s:%d] message from chat", uid, CMD_DESC(cmd), cmd);
			func = m_chat_msg_handle.find_func(cmd);
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
	static bool handle_fr_syn_tansfer_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_fr_test_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

public:

	static bool handle_gr_cross_fight_test_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_gr_arena_match_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_gr_get_arena_rank_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_gr_world_cup_play_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_gr_world_cup_bet_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_gr_world_cup_bet_reward_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_gr_role_leave_game_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_gr_arena_cancel_match_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_gr_arena_self_rank_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
public:
	static bool handle_er_world_cup_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_er_world_cup_bet_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_er_world_cup_odds_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
public:
	static bool handle_hr_get_hero_strategy_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
private:
    static network::server_msg_handler_t<op_cmd::FR_BEGIN, op_cmd::FR_END, msg_handler_func> m_transfer_msg_handle;
	static network::server_msg_handler_t<op_cmd::GR_BEGIN, op_cmd::GR_END, msg_handler_func> m_game_msg_handle;
	static network::server_msg_handler_t<op_cmd::ER_BEGIN, op_cmd::ER_END, msg_handler_func> m_center_msg_handle;
	static network::server_msg_handler_t<op_cmd::HR_BEGIN, op_cmd::HR_END, msg_handler_func> m_chat_msg_handle;
};
