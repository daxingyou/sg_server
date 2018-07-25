#include "world_cup_battle.hpp"
#include "arena/arena_score_rank_mgr.hpp"
#include "main/area_server.hpp"
#include "main/area_server_fwd.hpp"
#include "utility.hpp"
#include "redis_client.hpp"
#include "global_id.hpp"
#include "tblh/errcode_enum.hpp"
USING_NS_COMMON;

void world_cup_battle_t::load_data(const proto::common::world_cup_battle& battle_single)
{

}

void world_cup_battle_t::peek_data(proto::common::world_cup_battle* p_battle_single)
{
	if (NULL == p_battle_single)
	{
		log_error("NULL == p_battle_single");
		return;
	}
	p_battle_single->set_uid(string_util_t::uint64_to_string(m_uid));
	p_battle_single->set_round(m_round);
	p_battle_single->set_battle_id(m_battle_id);

	proto::common::world_cup_player* p_player_a = p_battle_single->mutable_player_a();
	uint32_t rank_index_a = arena_score_rank_mgr_t::get_arena_rank(m_player_a_uid);
	if (p_player_a && 0 != rank_index_a)
	{
		p_player_a->set_uid(string_util_t::uint64_to_string(m_player_a_uid));
		p_player_a->set_name(redis_client_t::get_string("arena_score_rank", "name", m_player_a_uid));
		p_player_a->set_family_id("");
		p_player_a->set_family_name("");
	}
	p_battle_single->set_score_a(m_player_a_score);

	proto::common::world_cup_player* p_player_b = p_battle_single->mutable_player_b();
	uint32_t rank_index_b = arena_score_rank_mgr_t::get_arena_rank(m_player_b_uid);
	if (p_player_b && 0 != rank_index_b)
	{
		p_player_b->set_uid(string_util_t::uint64_to_string(m_player_b_uid));
		p_player_b->set_name(redis_client_t::get_string("arena_score_rank", "name", m_player_b_uid));
		p_player_b->set_family_id("");
		p_player_b->set_family_name("");
	}
	p_battle_single->set_score_b(m_player_b_score);

	p_battle_single->set_winner(string_util_t::uint64_to_string(m_winner_uid));
	p_battle_single->set_state(m_state);
}

void world_cup_battle_t::load_bets()
{
	std::vector<std::uint64_t> bet_uids;
	redis_client_t::get_keys_by_field_condition<uint64_t, uint32_t, std::vector<std::uint64_t>>("world_cup_bet", "battle_uid", m_uid, bet_uids);
	for (auto& uid : bet_uids)
	{
		world_cup_bet_ptr p_bet = world_cup_bet_ptr(new world_cup_bet_t);
		p_bet->set_uid(uid);
		p_bet->set_battle_uid(redis_client_t::get_uint64("world_cup_bet", "battle_uid", uid));
		p_bet->set_player_uid(redis_client_t::get_uint64("world_cup_bet", "player_uid", uid));
		p_bet->set_winner(redis_client_t::get_uint64("world_cup_bet", "winner", uid));
		p_bet->set_odds(redis_client_t::get_uint32("world_cup_bet", "odds", uid));
		p_bet->set_bet(redis_client_t::get_uint32("world_cup_bet", "bet", uid));
		p_bet->set_result(redis_client_t::get_uint32("world_cup_bet", "result", uid));
		p_bet->set_state(redis_client_t::get_uint32("world_cup_bet", "state", uid));
		m_bet_map.insert(std::make_pair(p_bet->get_player_uid(), p_bet));
	}
}

void world_cup_battle_t::peek_bet(uint64_t player_uid, proto::common::world_cup_bet* p_bet_single)
{
	p_bet_single->set_battle_uid(string_util_t::uint64_to_string(m_uid));
	p_bet_single->set_uid(string_util_t::uint64_to_string(0));
	world_cup_bet_map::iterator it = m_bet_map.find(player_uid);
	if (it == m_bet_map.end())
	{
		return;
	}
	auto p_bet = it->second;
	if (NULL == p_bet)
	{
		return;
	}
	p_bet->peek_data(p_bet_single);
}

void world_cup_battle_t::peek_odds_data(proto::common::world_cup_odds* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return;
	}
	p_data->set_odds_a(m_odds_a);
	p_data->set_bet_a(m_bet_a);
	p_data->set_vote_a(m_vote_a);
	p_data->set_odds_b(m_odds_b);
	p_data->set_bet_b(m_bet_b);
	p_data->set_vote_b(m_vote_b);
}

uint64_t world_cup_battle_t::get_new_bet_uid()
{
	return g_id_allocator.gen_new_id(ID_ALLOCATOR_TYPE_BET);
}

void world_cup_battle_t::bet(uint64_t player_id, uint64_t winner, uint32_t odds, uint32_t bet, proto::server::rg_world_cup_bet_reply& reply)
{
	if (m_state != proto::common::world_cup_battle_state_bet)
	{
		log_error("battle[%lu] state invalid", m_uid);
		reply.set_reply_code(errcode_enum::bountytask_cathectic_error2);
		return;
	}
	if (m_bet_map.find(player_id) != m_bet_map.end())
	{
		log_error("battle[%lu] player[%lu] already bet", m_uid, player_id);
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}
	if (bet > get_max_bet() || bet < get_min_bet())
	{
		log_error("battle[%lu] player[%lu] bet invalid", m_uid, player_id);
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}
	if (winner == m_player_a_uid)
	{
		if (odds != m_odds_a)
		{
			reply.set_reply_code(errcode_enum::bountytask_cathectic_error1);
			reply.set_odds(m_odds_a);
			return;
		}
		m_bet_a += bet;
		m_vote_a += 1;
	}
	else if (winner == m_player_b_uid)
	{
		if (odds != m_odds_b)
		{
			reply.set_reply_code(errcode_enum::bountytask_cathectic_error1);
			reply.set_odds(m_odds_b);
			return;
		}
		m_bet_b += bet;
		m_vote_a += 1;
	}
	else
	{
		log_error("battle[%lu] player[%lu] invalid winner", m_uid, player_id);
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}
	calc_odds();
	world_cup_bet_ptr p_bet = world_cup_bet_ptr(new world_cup_bet_t);
	uint64_t bet_uid = get_new_bet_uid();
	p_bet->set_uid(bet_uid);
	p_bet->set_battle_uid(m_uid);
	p_bet->set_player_uid(player_id);
	p_bet->set_winner(winner);
	p_bet->set_odds(odds);
	p_bet->set_bet(bet);
	p_bet->set_result(0);
	p_bet->set_state(0);
	m_bet_map.insert(std::make_pair(bet_uid, p_bet));
	p_bet->peek_data(reply.mutable_self_bet());
	reply.set_reply_code(errcode_enum::error_ok);
}

uint32_t world_cup_battle_t::bet_reward(uint64_t player_id, uint32_t& reward_bet)
{
	auto it = m_bet_map.find(player_id);
	if (it == m_bet_map.end())
	{
		log_error("battle[%lu] player[%lu] no bet", m_uid, player_id);
		return errcode_enum::notice_unknown;
	}
	auto p_bet = it->second;
	if (NULL == p_bet)
	{
		log_error("battle[%lu] player[%lu] no bet", m_uid, player_id);
		return errcode_enum::notice_unknown;
	}
	if (p_bet->get_state() == 1)
	{
		log_error("battle[%lu] player[%lu] bet already reward", m_uid, player_id);
		return errcode_enum::notice_unknown;
	}
	if (p_bet->get_result() == proto::common::world_cup_bet_result_win)
	{
		reward_bet = p_bet->get_bet() * (10 + p_bet->get_odds()) / 10;
	}
	else if (p_bet->get_result() == proto::common::world_cup_bet_result_draw)
	{
		reward_bet = p_bet->get_bet();
	}
	else
	{
		log_error("battle[%lu] player[%lu] bet no reward", m_uid, player_id);
		return errcode_enum::notice_unknown;
	}
	return errcode_enum::error_ok;
}

void world_cup_battle_t::bet_result()
{
	if (m_state != proto::common::world_cup_battle_state_end)
	{
		log_error("world_cup_battle_t::bet_result state error");
		return;
	}
	for (auto it : m_bet_map)
	{
		auto p_bet = it.second;
		if (NULL == p_bet)
		{
			continue;
		}
		if (p_bet->get_result() != proto::common::world_cup_bet_result_none)
		{
			continue;
		}
		if (0 == m_winner_uid)
		{
			p_bet->set_result(proto::common::world_cup_bet_result_draw);
		}
		else
		{
			if (p_bet->get_winner() == m_winner_uid)
			{
				p_bet->set_result(proto::common::world_cup_bet_result_win);
			}
			else
			{
				p_bet->set_result(proto::common::world_cup_bet_result_lose);
			}
		}
	}
}

void world_cup_battle_t::calc_odds()
{
	uint32_t old_odds_a = m_odds_a;
	uint32_t old_odds_b = m_odds_b;
	if (m_bet_a > m_bet_b)
	{
		uint32_t delta_bet = m_bet_a - m_bet_b;
		uint32_t max_delta_bet = get_delta_bet();
		if (delta_bet <= max_delta_bet)
		{
			return;
		}
		m_odds_a = m_bet_a * 10 / (m_bet_b + max_delta_bet);
		m_odds_b = (m_bet_b + max_delta_bet) * 10 / m_bet_a;
	}
	else
	{
		uint32_t delta_bet = m_bet_b - m_bet_a;
		uint32_t max_delta_bet = get_delta_bet();
		if (delta_bet <= max_delta_bet)
		{
			return;
		}
		m_odds_a = (m_bet_a + max_delta_bet) * 10 / m_bet_b;
		m_odds_b = m_bet_b * 10 / (m_bet_a + max_delta_bet);
	}
	if (old_odds_a != m_odds_a || old_odds_b != m_odds_b)
	{
		proto::server::re_world_cup_update_notify ntf;
		peek_data(ntf.mutable_battle_info());
		env::server->send_msg_to_transfer(op_cmd::re_world_cup_update_notify, 0, ntf);
	}
}

uint32_t world_cup_battle_t::get_min_bet()
{
	return 10000;
}

uint32_t world_cup_battle_t::get_max_bet()
{
	return 200000;
}

uint32_t world_cup_battle_t::get_delta_bet()
{
	return 1000000;
}
