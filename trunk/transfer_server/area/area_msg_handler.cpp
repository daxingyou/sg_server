#include "area_msg_handler.hpp"
#include "main/transfer_server.hpp"
#include "main/transfer_server_fwd.hpp"
#include "protos_fwd.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<op_cmd::RF_BEGIN, op_cmd::RF_END, area_msg_handler_t::msg_handler_func> area_msg_handler_t::m_area_msg_handler;
server_msg_handler_t<op_cmd::RG_BEGIN, op_cmd::RG_END, area_msg_handler_t::msg_handler_func> area_msg_handler_t::m_area_2_game_msg_handler;
server_msg_handler_t<op_cmd::RE_BEGIN, op_cmd::RE_END, area_msg_handler_t::msg_handler_func> area_msg_handler_t::m_area_2_center_msg_handler;
server_msg_handler_t<op_cmd::RH_BEGIN, op_cmd::RH_END, area_msg_handler_t::msg_handler_func> area_msg_handler_t::m_area_2_chat_msg_handler;
bool area_msg_handler_t::init_msg_handler()
{
	bool ret = true;
	ret &= m_area_msg_handler.register_func(op_cmd::rf_test_reply, handle_rf_test_reply);

	ret &= m_area_2_game_msg_handler.register_func(op_cmd::rg_arena_match_reply, handle_rg_arena_match_reply);
	ret &= m_area_2_game_msg_handler.register_func(op_cmd::rg_arena_battle_result_notify, handle_rg_arena_battle_result_notify);
	ret &= m_area_2_game_msg_handler.register_func(op_cmd::rg_get_arena_rank_data_reply, handle_rg_get_arena_rank_data_reply);
	ret &= m_area_2_game_msg_handler.register_func(op_cmd::rg_world_cup_play_reply, handle_rg_world_cup_play_reply);
	ret &= m_area_2_game_msg_handler.register_func(op_cmd::rg_world_cup_bet_reply, handle_rg_world_cup_bet_reply);
	ret &= m_area_2_game_msg_handler.register_func(op_cmd::rg_world_cup_bet_reward_reply, handle_rg_world_cup_bet_reward_reply);
	ret &= m_area_2_game_msg_handler.register_func(op_cmd::rg_arena_cancel_match_reply, handle_rg_arena_cancel_match_reply);
	ret &= m_area_2_game_msg_handler.register_func(op_cmd::rg_arena_self_rank_reply, handle_rg_arena_self_rank_reply);
	
	ret &= m_area_2_center_msg_handler.register_func(op_cmd::re_world_cup_data_reply, handle_re_world_cup_data_reply);
	ret &= m_area_2_center_msg_handler.register_func(op_cmd::re_world_cup_mail_notify, handle_re_world_cup_mail_notify);
	ret &= m_area_2_center_msg_handler.register_func(op_cmd::re_world_cup_coming_quarter_notify, handle_re_world_cup_coming_quarter_notify);
	ret &= m_area_2_center_msg_handler.register_func(op_cmd::re_world_cup_coming_soon_notify, handle_re_world_cup_coming_soon_notify);
	ret &= m_area_2_center_msg_handler.register_func(op_cmd::re_world_cup_ready_notify, handle_re_world_cup_ready_notify);
	ret &= m_area_2_center_msg_handler.register_func(op_cmd::re_world_cup_update_notify, handle_re_world_cup_update_notify);
	ret &= m_area_2_center_msg_handler.register_func(op_cmd::re_world_cup_battle_notify, handle_re_world_cup_battle_notify);
	ret &= m_area_2_center_msg_handler.register_func(op_cmd::re_world_cup_bet_data_reply, handle_re_world_cup_bet_data_reply);
	ret &= m_area_2_center_msg_handler.register_func(op_cmd::re_world_cup_odds_reply, handle_re_world_cup_odds_reply);

	ret &= m_area_2_chat_msg_handler.register_func(op_cmd::rh_get_hero_strategy_info_reply, handle_rh_get_hero_strategy_info_reply);
	return ret;
}

bool area_msg_handler_t::handle_rf_test_reply(const network::tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG(proto::server::rf_test_reply);
	
	log_info("area_msg test [%d]", msg.test1());
	return true;
}

bool area_msg_handler_t::handle_rg_arena_match_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::rg_arena_match_reply);

	env::server->send_msg_to_center(msg.server_id(), op_cmd::rg_arena_match_reply, uid, msg);
	return true;
}

bool area_msg_handler_t::handle_rg_arena_battle_result_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::rg_arena_battle_result_notify);

	env::server->send_msg_to_center(msg.server_id(), op_cmd::rg_arena_battle_result_notify, uid, msg);
	return true;
}

bool area_msg_handler_t::handle_rg_get_arena_rank_data_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::rg_get_arena_rank_data_reply);

	env::server->send_msg_to_center(msg.server_id(), op_cmd::rg_get_arena_rank_data_reply, uid, msg);
	return true;
}


bool area_msg_handler_t::handle_rg_world_cup_play_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::rg_world_cup_play_reply);

	env::server->send_msg_to_center(msg.server_id(), op_cmd::rg_world_cup_play_reply, uid, msg);
	return true;
}


bool area_msg_handler_t::handle_rg_world_cup_bet_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::rg_world_cup_bet_reply);

	env::server->send_msg_to_center(msg.server_id(), op_cmd::rg_world_cup_bet_reply, uid, msg);
	return true;
}


bool area_msg_handler_t::handle_rg_world_cup_bet_reward_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::rg_world_cup_bet_reward_reply);

	env::server->send_msg_to_center(msg.server_id(), op_cmd::rg_world_cup_bet_reward_reply, uid, msg);
	return true;
}

bool area_msg_handler_t::handle_rg_arena_cancel_match_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::rg_arena_cancel_match_reply);

	env::server->send_msg_to_center(msg.server_id(), op_cmd::rg_arena_cancel_match_reply, uid, msg);
	return true;
}


bool area_msg_handler_t::handle_rg_arena_self_rank_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::rg_arena_self_rank_reply);

	env::server->send_msg_to_center(msg.server_id(), op_cmd::rg_arena_self_rank_reply, uid, msg);
	return true;
}

bool area_msg_handler_t::handle_re_world_cup_data_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::re_world_cup_data_reply);
	if (msg.server_id() == 0)
	{
		env::server->send_msg_to_all_center(op_cmd::re_world_cup_data_reply, uid, msg);
	}
	else
	{
		env::server->send_msg_to_center(msg.server_id(), op_cmd::re_world_cup_data_reply, uid, msg);
	}
	return true;
}

bool area_msg_handler_t::handle_re_world_cup_mail_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::re_world_cup_mail_notify);
	
	env::server->send_msg_to_center(msg.server_id(), op_cmd::re_world_cup_mail_notify, uid, msg);
	return true;
}

bool area_msg_handler_t::handle_re_world_cup_coming_quarter_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::re_world_cup_coming_quarter_notify);

	env::server->send_msg_to_center(msg.server_id(), op_cmd::re_world_cup_coming_quarter_notify, uid, msg);
	return true;
}

bool area_msg_handler_t::handle_re_world_cup_coming_soon_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::re_world_cup_coming_soon_notify);

	env::server->send_msg_to_center(msg.server_id(), op_cmd::re_world_cup_coming_soon_notify, uid, msg);
	return true;
}

bool area_msg_handler_t::handle_re_world_cup_ready_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::re_world_cup_ready_notify);

	env::server->send_msg_to_center(msg.server_id(), op_cmd::re_world_cup_ready_notify, uid, msg);
	return true;
}

bool area_msg_handler_t::handle_re_world_cup_update_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::re_world_cup_update_notify);

	env::server->send_msg_to_all_center(op_cmd::re_world_cup_ready_notify, uid, msg);
	return true;
}

bool area_msg_handler_t::handle_re_world_cup_battle_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::re_world_cup_battle_notify);

	env::server->send_msg_to_all_center(op_cmd::re_world_cup_battle_notify, uid, msg);
	return true;
}

bool area_msg_handler_t::handle_re_world_cup_bet_data_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::re_world_cup_bet_data_reply);

	env::server->send_msg_to_center(msg.server_id(), op_cmd::re_world_cup_bet_data_reply, uid, msg);
	return true;
}

bool area_msg_handler_t::handle_re_world_cup_odds_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::re_world_cup_odds_reply);

	env::server->send_msg_to_center(msg.server_id(), op_cmd::re_world_cup_odds_reply, uid, msg);
	return true;
}

bool area_msg_handler_t::handle_rh_get_hero_strategy_info_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::rh_get_hero_strategy_info_reply);

	env::server->send_msg_to_chat(msg.server_id(), op_cmd::rh_get_hero_strategy_info_reply, uid, msg);
	return true;
}
