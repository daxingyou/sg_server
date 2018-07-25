#include "role_combat_result.hpp"

role_combat_result::role_combat_result()
{
}


role_combat_result::~role_combat_result()
{
}

void role_combat_result::set_combat_result(uint32_t combat_result, uint64_t fight_uid, const std::string& target_name)
{
	m_combat_result = combat_result;
	m_target_name = target_name;
	m_fight_uid = fight_uid;
}

void role_combat_result::reset_combat_result()
{
	m_combat_result = 0;
	m_fight_uid = 0;
	m_target_name.clear();
}
