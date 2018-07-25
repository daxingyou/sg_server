#ifndef __WORLD_CUP_BET_HPP__
#define __WORLD_CUP_BET_HPP__

#include "common.pb.h"
#include "macros.hpp"
#include <boost/shared_ptr.hpp>

class world_cup_bet_t
{

public:
	world_cup_bet_t(){}
public:
	void load_data(const proto::common::world_cup_bet& bet_single);
	void peek_data(proto::common::world_cup_bet* p_bet_single);
public:
	void set_uid(uint64_t val) { m_uid = val; }
	uint64_t get_uid() { return m_uid; }

	void set_battle_uid(uint64_t val) { m_battle_uid = val; }
	uint64_t get_battle_uid() { return m_battle_uid; }

	void set_player_uid(uint64_t val) { m_player_uid = val; }
	uint64_t get_player_uid() { return m_player_uid; }

	void set_winner(uint64_t val) { m_winner = val; }
	uint64_t get_winner() { return m_winner; }

	void set_odds(uint32_t val) { m_odds = val; }
	uint32_t get_odds() { return m_odds; }

	void set_bet(uint32_t val) { m_bet = val; }
	uint32_t get_bet() { return m_bet; }

	void set_result(uint32_t val) { m_result = val; }
	uint32_t get_result() { return m_result; }

	void set_state(uint32_t val) { m_state = val; }
	uint32_t get_state() { return m_state; }
private:
	uint64_t m_uid = 0;					//唯一ID			
	uint64_t m_battle_uid = 0;			//投注场次唯一ID
	uint64_t m_player_uid = 0;			//投注玩家唯一ID
	uint64_t m_winner = 0;				//投注内容（谁赢）
	uint32_t m_odds = 0;				//投注赔率
	uint32_t m_bet = 0;					//赌注
	uint32_t m_result = 0;				//结果world_cup_bet_result
	uint32_t m_state = 0;				//状态(0为待领取1为已领取)
};
typedef boost::shared_ptr<world_cup_bet_t> world_cup_bet_ptr;
#endif