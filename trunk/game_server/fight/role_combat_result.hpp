#pragma once
#include "macros.hpp"
// author:wys 
// date:2017.5.18
// desc:玩家战斗结果缓存

class role_combat_result
{
public:
	role_combat_result();
	~role_combat_result();

	void set_combat_result(uint32_t combat_result, uint64_t fight_uid, const std::string& target_name);
//	void save_auto_fight(uint32_t auto_fight) { m_auto_fight = auto_fight; }
	void reset_combat_result();
	uint32_t get_combat_result() { return m_combat_result; }
	const std::string& get_target_name() { return m_target_name; }
//	uint32_t auto_fight() { return m_auto_fight; }

private:
	std::string m_target_name = "";
	uint64_t m_fight_uid = 0;			// 战斗UID
	uint32_t m_combat_result = 0;		// 战斗结果
//	uint32_t m_auto_fight = 0;			// 是否自动战斗(不清除)
};

