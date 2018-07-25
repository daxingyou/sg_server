#include "world_cup_bet.hpp"
#include "main/area_server.hpp"
#include "main/area_server_fwd.hpp"
#include "utility.hpp"
USING_NS_COMMON;

void world_cup_bet_t::load_data(const proto::common::world_cup_bet& bet_single)
{

}

void world_cup_bet_t::peek_data(proto::common::world_cup_bet* p_bet_single)
{
	if (NULL == p_bet_single)
	{
		log_error("NULL == p_bet_single");
		return;
	}
	p_bet_single->set_uid(string_util_t::uint64_to_string(m_uid));
	p_bet_single->set_battle_uid(string_util_t::uint64_to_string(m_battle_uid));
	p_bet_single->set_player_uid(string_util_t::uint64_to_string(m_player_uid));
	p_bet_single->set_winner(string_util_t::uint64_to_string(m_winner));
	p_bet_single->set_odds(m_odds);
	p_bet_single->set_bet(m_bet);
	p_bet_single->set_result(m_result);
	p_bet_single->set_state(m_state);
}