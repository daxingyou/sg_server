#include "transfer_msg_handler.hpp"
#include "main/area_server.hpp"
#include "main/area_server_fwd.hpp"
#include "protos_fwd.hpp"
#include "transfer_manager.hpp"
#include "test/cross_fight_test.hpp"
#include "arena/arena_manager.hpp"
#include "arena/arena_score_rank_mgr.hpp"
#include "world_cup/world_cup_manager.hpp"
#include "hero_strategy/hero_strategy_manager.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<op_cmd::FR_BEGIN, op_cmd::FR_END, transfer_msg_handler_t::msg_handler_func> transfer_msg_handler_t::m_transfer_msg_handle;
server_msg_handler_t<op_cmd::GR_BEGIN, op_cmd::GR_END, transfer_msg_handler_t::msg_handler_func> transfer_msg_handler_t::m_game_msg_handle;
server_msg_handler_t<op_cmd::ER_BEGIN, op_cmd::ER_END, transfer_msg_handler_t::msg_handler_func> transfer_msg_handler_t::m_center_msg_handle;
server_msg_handler_t<op_cmd::HR_BEGIN, op_cmd::HR_END, transfer_msg_handler_t::msg_handler_func> transfer_msg_handler_t::m_chat_msg_handle;
bool transfer_msg_handler_t::init_msg_handler()
{
	bool ret = true;
	ret &= m_transfer_msg_handle.register_func(op_cmd::fr_syn_transfer_info_request, handle_fr_syn_tansfer_info_request);
	ret &= m_transfer_msg_handle.register_func(op_cmd::fr_test_request, handle_fr_test_request);

	// message from game
	ret &= m_game_msg_handle.register_func(op_cmd::gr_cross_fight_test_notify, handle_gr_cross_fight_test_notify);
	ret &= m_game_msg_handle.register_func(op_cmd::gr_arena_match_request, handle_gr_arena_match_request);
	ret &= m_game_msg_handle.register_func(op_cmd::gr_get_arena_rank_data_request, handle_gr_get_arena_rank_data_request);
	ret &= m_game_msg_handle.register_func(op_cmd::gr_world_cup_play_request, handle_gr_world_cup_play_request);
	ret &= m_game_msg_handle.register_func(op_cmd::gr_world_cup_bet_request, handle_gr_world_cup_bet_request);
	ret &= m_game_msg_handle.register_func(op_cmd::gr_world_cup_bet_reward_request, handle_gr_world_cup_bet_reward_request);
	ret &= m_game_msg_handle.register_func(op_cmd::gr_role_leave_game_notify, handle_gr_role_leave_game_notify);
	ret &= m_game_msg_handle.register_func(op_cmd::gr_arena_cancel_match_request, handle_gr_arena_cancel_match_request);
	ret &= m_game_msg_handle.register_func(op_cmd::gr_arena_self_rank_request, handle_gr_arena_self_rank_request);
	// message from center
	ret &= m_center_msg_handle.register_func(op_cmd::er_world_cup_data_request, handle_er_world_cup_data_request);
	ret &= m_center_msg_handle.register_func(op_cmd::er_world_cup_bet_data_request, handle_er_world_cup_bet_data_request);
	ret &= m_center_msg_handle.register_func(op_cmd::er_world_cup_odds_request, handle_er_world_cup_odds_request);
	// message from chat
	ret &= m_chat_msg_handle.register_func(op_cmd::hr_get_hero_strategy_info_request, handle_hr_get_hero_strategy_info_request);
	
    return ret;
}

bool transfer_msg_handler_t::handle_fr_syn_tansfer_info_request(const network::tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG(proto::server::fr_syn_transfer_info_request);

	transfer_manager_t::set_transfer(s, msg.server_id());
	return true;
}

bool transfer_msg_handler_t::handle_fr_test_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::fr_test_request);

	proto::server::rf_test_reply reply;
	reply.set_test1(222);
	env::server->send_msg_to_all_transfer(op_cmd::rf_test_reply, 0, reply);
	env::server->send_msg_to_transfer(s, op_cmd::rf_test_reply, 0, reply);
//	env::server->send_msg_to_transfer(2, op_cmd::rf_test_reply, 0, reply);
	return true;
}

bool transfer_msg_handler_t::handle_gr_cross_fight_test_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::gr_cross_fight_test_notify);
	cross_fight_test_t::on_cross_fight_test_notify(msg);
	return true;
}

bool transfer_msg_handler_t::handle_gr_arena_match_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::gr_arena_match_request);

	arena_manager_t::match(uid, msg.score(), msg.arena_level(), msg.user());
	return true;
}

bool transfer_msg_handler_t::handle_gr_get_arena_rank_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::gr_get_arena_rank_data_request);

	const arena_rank_ptr& p_arena_rank = arena_score_rank_mgr_t::get_arena_rank_ptr();
	if (p_arena_rank) {
		proto::server::rg_get_arena_rank_data_reply reply;
		reply.set_server_id(msg.server_id());
		p_arena_rank->get_rank_list(msg.rank_start_pos(), uid, reply);
		env::server->send_msg_to_transfer(s, op_cmd::rg_get_arena_rank_data_reply, uid, reply);
	}

	return true;
}

bool transfer_msg_handler_t::handle_gr_world_cup_play_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::gr_world_cup_play_request);
	proto::server::rg_world_cup_play_reply reply;
	reply.set_server_id(msg.server_id());
	uint32_t reply_code = world_cup_manager_t::play(uid, msg.rcd());
	reply.set_reply_code(reply_code);
	env::server->send_msg_to_transfer(s, op_cmd::rg_world_cup_play_reply, uid, reply);
	return true;
}

bool transfer_msg_handler_t::handle_gr_world_cup_bet_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::gr_world_cup_bet_request);
	proto::server::rg_world_cup_bet_reply reply;
	reply.set_server_id(msg.server_id());
	reply.set_bet(msg.bet());
	world_cup_manager_t::bet(uid, msg.battle_uid(), msg.winner(), msg.odds(), msg.bet(), reply);
	env::server->send_msg_to_transfer(s, op_cmd::rg_world_cup_bet_reply, uid, reply);
	return true;
}

bool transfer_msg_handler_t::handle_gr_world_cup_bet_reward_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::gr_world_cup_bet_reward_request);
	proto::server::rg_world_cup_bet_reward_reply reply;
	reply.set_server_id(msg.server_id());
	uint32_t reward_bet = 0;
	uint32_t reply_code = world_cup_manager_t::bet_reward(uid, msg.battle_uid(), reward_bet);
	reply.set_reply_code(reply_code);
	reply.set_reward(reward_bet);
	env::server->send_msg_to_transfer(s, op_cmd::rg_world_cup_bet_reward_reply, uid, reply);
	return true;
}

bool transfer_msg_handler_t::handle_gr_role_leave_game_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::gr_role_leave_game_notify);
	arena_manager_t::deal_role_logout(uid);
	return true;
}

bool transfer_msg_handler_t::handle_gr_arena_cancel_match_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::gr_arena_cancel_match_request);
	arena_manager_t::cancel_match(uid);
	return true;
}


bool transfer_msg_handler_t::handle_gr_arena_self_rank_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::gr_arena_self_rank_request);
	proto::server::rg_arena_self_rank_reply reply;
	reply.set_server_id(msg.server_id());
	reply.set_rank(arena_score_rank_mgr_t::get_arena_rank(uid));
	env::server->send_msg_to_transfer(s, op_cmd::rg_arena_self_rank_reply, uid, reply);
	return true;
}

bool transfer_msg_handler_t::handle_er_world_cup_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::er_world_cup_data_request);
	proto::server::re_world_cup_data_reply reply;
	reply.set_server_id(msg.server_id());
	world_cup_manager_t::peek_data(reply.mutable_world_cup_info());
	env::server->send_msg_to_transfer(s, op_cmd::re_world_cup_data_reply, 0, reply);
	return true;
}

bool transfer_msg_handler_t::handle_er_world_cup_bet_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::er_world_cup_bet_data_request);
	proto::server::re_world_cup_bet_data_reply reply;
	reply.set_server_id(msg.server_id());
	world_cup_manager_t::peek_bet_data(reply, uid);
	env::server->send_msg_to_transfer(s, op_cmd::re_world_cup_bet_data_reply, uid, reply);
	return true;
}

bool transfer_msg_handler_t::handle_er_world_cup_odds_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::er_world_cup_odds_request);
	proto::server::re_world_cup_odds_reply reply;
	reply.set_server_id(msg.server_id());
	world_cup_manager_t::peek_odds_data(reply.mutable_odds_data(), msg.battle_uid());
	env::server->send_msg_to_transfer(s, op_cmd::re_world_cup_odds_reply, uid, reply);
	return true;
}

bool transfer_msg_handler_t::handle_hr_get_hero_strategy_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::hr_get_hero_strategy_info_request);
	hero_strategy_manager_t::get_hero_strategy_data_to_chat(msg.server_id());
	return true;
}
