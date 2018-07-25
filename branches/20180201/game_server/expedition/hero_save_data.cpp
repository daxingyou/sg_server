#include "hero_save_data.hpp"
#include "log.hpp"
#include "fight/fight_manager.hpp"
#include "tblh/Monster.tbls.h"
#include "config/config_manager.hpp"

hero_save_data_t::hero_save_data_t()
{

}

hero_save_data_t::~hero_save_data_t()
{

}

void hero_save_data_t::load_data(const proto::common::hero_save_data& hsd)
{
	m_hero_uid = hsd.hero_uid();
	m_hp = hsd.hp();
	for (int32_t i = 0; i < hsd.skill_list_size(); ++i)
	{
		const proto::common::skill_save_data& ssd = hsd.skill_list(i);
		m_skill_cd.insert(std::make_pair(ssd.skill_id(), ssd.cd()));
	}
}

void hero_save_data_t::peek_data(proto::common::hero_save_data* p_data)
{
	p_data->set_hero_uid(m_hero_uid);
	p_data->set_hp(m_hp);
	for (auto it : m_skill_cd)
	{
		proto::common::skill_save_data* p_skill = p_data->add_skill_list();
		p_skill->set_skill_id(it.first);
		p_skill->set_cd(it.second);
	}
}


void hero_save_data_t::save_data(const proto::common::hero_save_data& hsd)
{
	m_hp = hsd.hp();
	m_skill_cd.clear();
	for (int32_t i = 0; i < hsd.skill_list_size(); ++i)
	{
		const proto::common::skill_save_data& ssd = hsd.skill_list(i);
		m_skill_cd.insert(std::make_pair(ssd.skill_id(), ssd.cd()));
	}
}

int32_t hero_save_data_t::get_skill_cd(uint32_t skill_id)
{
	auto it = m_skill_cd.find(skill_id);
	if (it == m_skill_cd.end())
	{
		return -1;
	}
	return it->second;
}

bool hero_save_data_t::has_cd()
{
	return !m_skill_cd.empty();
}

void hero_save_data_t::clear_cd()
{
	m_skill_cd.clear();
}

