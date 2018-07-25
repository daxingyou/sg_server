#include "common/config_mgr.h"
#include "fight/fight_manager.hpp"
#include "tblh/FightAgainst.tbls.h"

#include "partner_tutor.hpp"
#include "log.hpp"


partner_tutor_t::partner_tutor_t(uint64_t object_id)
	: partner_t(object_id)
{

}

partner_tutor_t::~partner_tutor_t()
{

}

void partner_tutor_t::get_object_info(proto::common::object_common_info* object_info)
{
	if (object_info) {
		partner_t::get_object_info(object_info);
		object_info->set_object_name(m_name);
		object_info->set_level(m_level);
		object_info->set_npc_tid(m_tutor_tid);
		object_info->set_plugin(m_plugin);
		object_info->set_fighting_value(m_power);
	}
}

bool partner_tutor_t::init_tutor(TutorTable* p_conf, const std::string& name, uint32_t level)
{
	if (!p_conf) {
		log_error("p_conf null error");
		return false;
	}

	m_tutor_tid = p_conf->id();
	m_name = name;
	m_plugin = p_conf->plugin();
	m_level = level;
	m_power = p_conf->power();
	m_fight_id = p_conf->fight_id();

	return true;
}

hero_map* partner_tutor_t::get_hero_list()
{
	return &m_hero_list;
}

bool partner_tutor_t::init_hero_list()
{
	if (m_hero_list.size() != 0)
		return true;
	FightAgainst* p_against_table = GET_CONF(FightAgainst, m_fight_id);
	if (NULL == p_against_table)
	{
		log_error("partner_tutor_t[%lu] p_against_table[%u] null error", get_object_id(), m_fight_id);
		return false;
	}

	
	std::map<uint32_t, uint32_t>::const_iterator it = p_against_table->get_against_ids().begin();
	for (; it != p_against_table->get_against_ids().end(); ++it)
	{
		MonsterHero* conf = GET_CONF(MonsterHero, it->second);
		if (conf == NULL)
		{
			log_error("fight_manager conf NULL id[%d]", it->second);
			continue;
		}

		uint32_t pos = it->first;

		proto::common::hero_single single;

		bool ret = fight_manager_t::fill_hero_single(
			0, 
			0, 
			conf, 
			m_level, 
			&single, 
			get_object_id(), 
			pos,
			NULL);
		if (ret == false)
			continue;
		
		hero_ptr p_hero(new hero_t(get_object_id_type()));
		p_hero->load_data(single);
		m_hero_list[p_hero->get_tid()] = p_hero;
		p_hero->calc_fighting();
	}
	return true;
}

bool partner_tutor_t::get_hero_list(uint32_t fight_type, uint32_t team_member_size, std::vector<uint64_t>& list)
{
	if (init_hero_list() == false)
		return false;
	uint32_t team_heroes_max_num = fight_manager_t::get_hero_count_by_member_size(team_member_size);
	hero_map::const_iterator it;
	uint32_t index = 0;
	for (it = m_hero_list.begin(); it != m_hero_list.end(); ++it)
	{
		list.push_back(it->first);
		++index;
		if (index >= team_heroes_max_num)
			break;
	}
	return true;
}

hero_ptr partner_tutor_t::get_hero_by_unique_id(uint64_t unique_id)
{
	// 判断是否要初始化管理器
	if (init_hero_list() == false)
		return NULL;
	auto it = m_hero_list.find(unique_id);
	if (it != m_hero_list.end())
	{
		return it->second;
	}

	return NULL;
}

bool partner_tutor_t::fill_fight_camp(
	uint32_t camp_index,
	uint64_t unique_offset,
	uint32_t team_member_size,
	proto::common::fight_camp* camp,
	proto::common::fight_type fight_type,
	proto::common::hero_state_data* state,
	int32_t team_member_pos)
{
	FightAgainst* p_against_table = GET_CONF(FightAgainst, m_fight_id);
	if (NULL == p_against_table)
	{
		log_error("partner_tutor_t[%lu] p_against_table[%u] null error", get_object_id(), m_fight_id);
		return false;
	}

	return fight_manager_t::fill_fight_camp(
		camp_index,
		p_against_table,
		m_level,
		team_member_size,
		camp,
		state,
		get_object_id(),
		team_member_pos,
		get_object_type(),
		&m_name);
}

// 填充武将列表
bool partner_tutor_t::fill_hero_data(
	uint32_t camp_index,
	uint64_t unique_offset,
	uint32_t team_member_size,
	proto::common::fight_type fight_type,
	proto::common::hero_data* fhd,
	proto::common::hero_state_data* state,
	int32_t team_member_pos)
{
	FightAgainst* p_against_table = GET_CONF(FightAgainst, m_fight_id);
	if (NULL == p_against_table)
	{
		log_error("partner_tutor_t[%lu] p_against_table[%u] null error", get_object_id(), m_fight_id);
		return false;
	}
	return fight_manager_t::fill_hero_data(
		camp_index,
		get_object_id(),
		unique_offset,
		m_level,
		0,
		team_member_size,
		p_against_table->get_against_ids(),
		fhd,
		state,
		team_member_pos);
}
