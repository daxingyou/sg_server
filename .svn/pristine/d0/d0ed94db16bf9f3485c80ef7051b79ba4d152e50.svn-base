#ifndef __WORLD_CUP_BATTLE_HPP__
#define __WORLD_CUP_BATTLE_HPP__

#include "common.pb.h"
#include "macros.hpp"
#include <boost/shared_ptr.hpp>

class world_cup_battle_t
{

public:
	world_cup_battle_t(){}
public:
	void load_data(const proto::common::world_cup_battle& battle_single);
	void peek_data(proto::common::world_cup_battle* p_battle_single);
public:
	void set_uid(uint64_t val) { m_uid = val; }
	uint64_t get_uid() { return m_uid; }

	void set_round(uint32_t val) { m_round = val; }
	uint32_t get_round() { return m_round; }

	void set_battle_id(uint32_t val) { m_battle_id = val; }
	uint32_t get_battle_id() { return m_battle_id; }

	void set_player_a_uid(const std::string& val) { m_player_a_uid = val; }
	const std::string& get_player_a_uid() { return m_player_a_uid; }
	void set_player_a_name(const std::string& val) { m_player_a_name = val; }
	const std::string& get_player_a_name() { return m_player_a_name; }
	void set_player_a_family_uid(const std::string& val) { m_player_a_family_uid = val; }
	const std::string& get_player_a_family_uid() { return m_player_a_family_uid; }
	void set_player_a_family_name(const std::string& val) { m_player_a_family_name = val; }
	const std::string& get_player_a_family_name() { return m_player_a_family_name; }

	void set_player_b_uid(const std::string& val) { m_player_b_uid = val; }
	const std::string& get_player_b_uid() { return m_player_b_uid; }
	void set_player_b_name(const std::string& val) { m_player_b_name = val; }
	const std::string& get_player_b_name() { return m_player_b_name; }
	void set_player_b_family_uid(const std::string& val) { m_player_b_family_uid = val; }
	const std::string& get_player_b_family_uid() { return m_player_b_family_uid; }
	void set_player_b_family_name(const std::string& val) { m_player_b_family_name = val; }
	const std::string& get_player_b_family_name() { return m_player_b_family_name; }

	void set_player_a_score(uint32_t val) { m_player_a_score = val; }
	uint32_t get_player_a_score() { return m_player_a_score; }

	void set_player_b_score(uint32_t val) { m_player_b_score = val; }
	uint32_t get_player_b_score() { return m_player_b_score; }

	void set_winner_uid(const std::string& val) { m_winner_uid = val; }
	const std::string& get_winner_uid() { return m_winner_uid; }

	void set_state(uint32_t val) { m_state = val; }
	uint32_t get_state() { return m_state; }
private:
	uint64_t m_uid = 0;
	uint32_t m_round = 0;
	uint32_t m_battle_id = 0;
	std::string m_player_a_uid = "";
	std::string m_player_a_name = "";
	std::string m_player_a_family_uid = "";
	std::string m_player_a_family_name = "";
	std::string m_player_b_uid = "";
	std::string m_player_b_name = "";
	std::string m_player_b_family_uid = "";
	std::string m_player_b_family_name = "";
	uint32_t m_player_a_score = 0;
	uint32_t m_player_b_score = 0;
	std::string m_winner_uid = "";
	uint32_t m_state = 0;
};
typedef boost::shared_ptr<world_cup_battle_t> world_cup_battle_ptr;
#endif