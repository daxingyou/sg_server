#include "family_skill.hpp"
#include "main/game_server_fwd.hpp"
#include "utility.hpp"

USING_NS_COMMON;
family_skill_t::family_skill_t()
{

}


family_skill_t::~family_skill_t()
{

}

void family_skill_t::load_data(const proto::common::family_skill_single& ls)
{
	m_tid = ls.skill_id();
	m_level = ls.skill_level();
}

void family_skill_t::peek_data(proto::common::family_skill_single* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return;
	}
	p_data->set_skill_id(m_tid);
	p_data->set_skill_level(m_level);
}
