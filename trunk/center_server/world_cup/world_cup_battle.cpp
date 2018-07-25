#include "world_cup_battle.hpp"
#include "main/center_server.hpp"
#include "main/center_server_fwd.hpp"
#include "protos_fwd.hpp"
#include "utility.hpp"
USING_NS_COMMON;

void world_cup_battle_t::load_data(const proto::common::world_cup_battle& battle_single)
{
	m_uid = string_util_t::string_to_uint64(battle_single.uid());
	m_round = battle_single.round();
	m_battle_id = battle_single.battle_id();

	const proto::common::world_cup_player& wcp_a = battle_single.player_a();
	m_player_a_uid = wcp_a.uid();
	m_player_a_name = wcp_a.name();
	m_player_a_family_uid = wcp_a.family_id();
	m_player_a_family_name = wcp_a.family_name();

	const proto::common::world_cup_player& wcp_b = battle_single.player_b();
	m_player_b_uid = wcp_b.uid();
	m_player_b_name = wcp_b.name();
	m_player_b_family_uid = wcp_b.family_id();
	m_player_b_family_name = wcp_b.family_name();

	m_player_a_score = battle_single.score_a();
	m_player_b_score = battle_single.score_b();
	m_winner_uid = battle_single.winner();
	m_state = battle_single.state();
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
	p_player_a->set_uid(m_player_a_uid);
	p_player_a->set_name(m_player_a_name);
	p_player_a->set_family_id(m_player_a_family_uid);
	p_player_a->set_family_name(m_player_a_family_name);
	p_battle_single->set_score_a(m_player_a_score);

	proto::common::world_cup_player* p_player_b = p_battle_single->mutable_player_b();
	p_player_b->set_uid(m_player_b_uid);
	p_player_b->set_name(m_player_b_name);
	p_player_b->set_family_id(m_player_b_family_uid);
	p_player_b->set_family_name(m_player_b_family_name);
	p_battle_single->set_score_b(m_player_b_score);

	p_battle_single->set_winner(m_winner_uid);
	p_battle_single->set_state(m_state);
}
