#include "family_skill_manager.hpp"
#include "main/game_server_fwd.hpp"
#include "main/game_server.hpp"
#include "cache_key.hpp"
#include "role/role_unify_save.hpp"
#include "config/config_manager.hpp"
#include "common/utility.hpp"
#include "log/log_wrapper.hpp"
#include "tblh/errcode_enum.hpp"
#include "role/money_manager.hpp"
#include "tblh/FamilySkill.tbls.h"
#include "role/role_manager.hpp"
#include "hero/hero_manager.hpp"

family_skill_manager_t::family_skill_manager_t(uint64_t uid)
	: m_owner(uid)
{
	m_key = cache_key_t::create(m_owner, cache_name::family_skill);
}

family_skill_manager_t::~family_skill_manager_t()
{

}

void family_skill_manager_t::load_data(const proto::common::family_skill_data& fsd)
{
	for (int32_t i = 0; i < fsd.skill_data_size(); ++i)
	{
		family_skill_ptr p_family_skill = family_skill_ptr(new family_skill_t);
		p_family_skill->load_data(fsd.skill_data(i));
		m_family_skill_map.insert(std::make_pair(p_family_skill->get_tid(), p_family_skill));
	}
}

void family_skill_manager_t::peek_data(proto::common::family_skill_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return;
	}
	for (auto it : m_family_skill_map)
	{
		family_skill_ptr p_family_skill = it.second;
		if (NULL == p_family_skill)
		{
			log_error("NULL == p_family_skill role[%lu]", m_owner);
			continue;
		}
		proto::common::family_skill_single* p_single = p_data->add_skill_data();
		p_family_skill->peek_data(p_single);
	}
}

void family_skill_manager_t::save_self()
{
	proto::common::family_skill_data family_skill_data;
	peek_data(&family_skill_data);
	role_unify_save::add_task(m_owner, m_key, family_skill_data);
}

void family_skill_manager_t::calc_attr(std::map<uint32_t, int>& attrs)
{
	for (auto it : m_family_skill_map)
	{
		family_skill_ptr p_skill = it.second;
		if (NULL == p_skill)
		{
			log_error("NULL == p_skill");
			continue;
		}
		if (0 == p_skill->get_level())
		{
			continue;
		}
		FamilySkill* p_conf = GET_CONF(FamilySkill, hash_util_t::hash_to_uint32(p_skill->get_tid(), p_skill->get_level()));
		if (NULL == p_conf)
		{
			log_error("FamilySkill NULL == p_conf[%d:%d]", p_skill->get_tid(), p_skill->get_level());
			continue;
		}
		for (auto attr : p_conf->attr())
		{
			attrs[attr.first] += attr.second;
		}
	}
}

uint32_t family_skill_manager_t::levelup(uint32_t skill_id, proto::common::family_skill_single* p_single, proto::common::role_change_data* p_data)
{
	role_ptr p_role = role_manager_t::find_role(m_owner);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_owner);
		return errcode_enum::notice_role_null;
	}
	if (0 == p_role->get_family_id())
	{
		log_error("p_role[%lu] not in family", m_owner);
		return errcode_enum::notice_no_family;
	}
	if (NULL == p_single)
	{
		log_error("NULL == p_single");
		return errcode_enum::notice_unknown;
	}
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return errcode_enum::notice_unknown;
	}
	family_skill_ptr p_skill;
	auto it = m_family_skill_map.find(skill_id);
	if (it == m_family_skill_map.end())
	{
		p_skill.reset(new family_skill_t);
		if (p_skill) {
			p_skill->set_tid(skill_id);
			m_family_skill_map.insert(std::make_pair(skill_id, p_skill));
		}
	}
	else
	{
		p_skill = it->second;
	}
	if (NULL == p_skill)
	{
		log_error("role[%lu] family_skill[%d] null", m_owner, skill_id);
		return errcode_enum::notice_unknown;
	}
	uint32_t skill_level = p_skill->get_level();
	FamilySkill* p_conf = GET_CONF(FamilySkill, hash_util_t::hash_to_uint32(skill_id, skill_level+1));
	if (NULL == p_conf)
	{
		log_error("FamilySkill NULL == p_conf[%d:%d]", skill_id, skill_level+1);
		return errcode_enum::notice_unknown;
	}
	if (p_role->get_level() < p_conf->need_role_level())
	{
		log_error("role[%lu] levelup family_skill[%d:%d] failed, role_level[%d] not enough, need[%d]",
			m_owner, skill_id, skill_level + 1, p_role->get_level(), p_conf->need_role_level());
		return errcode_enum::notice_role_levle_not_enough;
	}
	if (p_role->get_family_level() < p_conf->need_family_level())
	{
		log_error("role[%lu] levelup family_skill[%d:%d] failed, family_level[%d] not enough, need[%d]",
			m_owner, skill_id, skill_level + 1, p_role->get_family_level(), p_conf->need_family_level());
		return errcode_enum::family_error_family_level_limit;
	}
	if (!money_manager_t::use_money(p_role, (proto::common::MONEY_TYPE)p_conf->cost_money().first, p_conf->cost_money().second, log_enum::source_type_family_invite_join,skill_id))
	{
		log_error("role[%lu] levelup family_skill[%d:%d] failed, money not enough", m_owner, skill_id, skill_level + 1);
		return errcode_enum::notice_copper_money_not_enough;
	}
	p_skill->set_level(skill_level + 1);
	p_skill->peek_data(p_single);
	proto::common::personal_info* p_info = p_data->mutable_per_info();
	p_role->get_personal_info(p_info);
	hero_manager_t::all_heros_update_attr(p_role, p_data);
	save_self();
	FAMILY_LOG("role[%lu] levelup family_skill[%d:%d] success", m_owner, skill_id, skill_level + 1);
	return errcode_enum::error_ok;
}
