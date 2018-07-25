#include "object_attr.hpp"
#include "config_mgr.h"
#include "hero/cultivate_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "cache_key.hpp"
#include "role/role_unify_save.hpp"
#include "tblh/Hero.tbls.h"

object_attr_t::object_attr_t()
{
}

object_attr_t::object_attr_t(uint32_t hero_id)
{
	init_hero(hero_id);
}

object_attr_t::~object_attr_t()
{
}

// ³õÊ¼»¯Îä½«
void object_attr_t::init_hero(uint32_t hero_id)
{
	m_tid = hero_id;
	set_current_exp(0);
	set_exp_today(0);
	set_wakeup(0);
	set_grade(0);
	set_current_level(1);
	set_talent_level(1);
	set_talent_count(cultivate_manager_t::get_init_talent_count());
	init_from_conf(hero_id);
}

void object_attr_t::init_from_conf(uint32_t hero_id)
{
	Hero* p_hero_conf = GET_CONF(Hero, hero_id);
	if (NULL == p_hero_conf)
	{
		log_error("NULL == p_hero_conf[%d]", hero_id);
		return;
	}
	set_star(p_hero_conf->star());
	add_plugin(p_hero_conf->default_model());
	set_current_plugin(p_hero_conf->default_model());

	hero_skill skill_info;
	for (uint32_t i = 0; i < p_hero_conf->default_skills().size(); ++i)
	{
		skill_info.skill_index = i + 1;
		skill_info.skill_id = p_hero_conf->default_skills()[i];
		skill_info.skill_level = 1;
		skill_info.skill_pos = i+1;
		m_hero_skills.insert(std::make_pair(skill_info.skill_index, skill_info));
	}
	skill_info.skill_level = 0;
	skill_info.skill_pos = 0;
	for (uint32_t i = 0; i < p_hero_conf->more_skills().size(); ++i)
	{
		skill_info.skill_index = i + 11;
		skill_info.skill_id = p_hero_conf->more_skills()[i];
		m_hero_skills.insert(std::make_pair(skill_info.skill_index, skill_info));
	}
}

void object_attr_t::add_plugin(uint32_t plugin)
{
	if (0 == plugin)
	{
		log_error("add plugin error [%d]",plugin);
		return;
	}

	auto max_plugin_num = GET_CONF(Comprehensive, comprehensive_common::max_plugin_num);
	uint32_t max_plugin = GET_COMPREHENSIVE_VALUE_1(max_plugin_num);

	if (0 != max_plugin && max_plugin <= m_plugins.size())
	{
		log_warn("plugin already upper limit[%d]", max_plugin);
		return;
	}

	m_plugins.push_back(plugin); 
}

bool object_attr_t::set_current_plugin(uint32_t plugin)
{
	for (uint32_t i = 0; i < m_plugins.size(); ++i)
	{
		if (m_plugins.at(i) == plugin)
		{
			m_current_plugin = plugin;
			return true;
		}
	}
	return false;
}

void object_attr_t::add_famous(uint32_t famous)
{
	if (std::find(m_famous.begin(), m_famous.end(), famous) == m_famous.end())
	{
		m_famous.push_back(famous);
	}
}

void object_attr_t::set_add_attr(uint32_t attr_type, int32_t attr_value)
{
	m_add_attrs[attr_type] += attr_value;
}

void object_attr_t::reset_add_attr()
{
	m_add_attrs.clear();
}

void object_attr_t::set_base_attr(const proto::common::base_attr& attr)
{
	m_tid = attr.id();
	m_uid = string_util_t::string_to_uint64(attr.unique_id());
	m_type = attr.type();
	m_exp = attr.exp();
	m_exp_today = attr.exp_today();
	m_level = attr.level();
	m_star = attr.star();
	m_degree = attr.degree();
	m_grade = attr.grade();
	m_talent_count = attr.talent_count();
	m_talent_level = attr.talent_level();
	m_talent_degree = attr.talent_degree();
	m_talent_use = attr.talent_use();
	m_active_talent_use = attr.active_talent_use();
// 	m_atk_skill_level = attr.atk_skill_level();
// 	m_ult_skill_level = attr.ult_skill_level();
// 	m_ult_skill2_level = attr.ult_skill2_level();
	m_wakeup = attr.wakeup();
	m_delete_flag = attr.delete_flag();
	m_hero_state = attr.hero_state();
	m_plugins.clear();
	for (int i = 0; i < attr.plugins_size(); ++i)
	{
		m_plugins.push_back(attr.plugins(i));
	}
	m_current_plugin = attr.current_plugin();

	m_hero_skills.clear();
	hero_skill skill_info;
	for (int32_t i = 0; i < attr.skill_data().skill_list_size(); i++)
	{
		skill_info.skill_id = attr.skill_data().skill_list(i).skill_id();
		skill_info.skill_level = attr.skill_data().skill_list(i).skill_level();
		skill_info.skill_pos = attr.skill_data().skill_list(i).skill_pos();
		skill_info.skill_index = attr.skill_data().skill_list(i).skill_index();
		m_hero_skills.insert(std::make_pair(skill_info.skill_index, skill_info));
	}
	fill_skill();
	m_last_equip_plan = string_util_t::string_to_uint64(attr.last_equip_plan());
	m_nick_name = attr.nick_name();
	m_famous.clear();
	for (int i = 0; i < attr.famous_size(); ++i)
	{
		m_famous.push_back(attr.famous(i));
	}
}

void object_attr_t::peek_base_attr(proto::common::base_attr* p_attr)
{
	p_attr->set_id(m_tid);
	p_attr->set_unique_id(string_util_t::uint64_to_string(m_uid));
	p_attr->set_type(m_type);
	p_attr->set_exp(m_exp);
	p_attr->set_exp_today(m_exp_today);
	p_attr->set_level(m_level);
	p_attr->set_star(m_star);
	p_attr->set_degree(m_degree);
	p_attr->set_grade(m_grade);
	p_attr->set_talent_count(m_talent_count);
	p_attr->set_talent_level(m_talent_level);
	p_attr->set_talent_degree(m_talent_degree);
	p_attr->set_talent_use(m_talent_use);
	p_attr->set_active_talent_use(m_active_talent_use);
// 	p_attr->set_atk_skill_level(m_atk_skill_level);
// 	p_attr->set_ult_skill_level(m_ult_skill_level);
// 	p_attr->set_ult_skill2_level(m_ult_skill2_level);
	p_attr->set_wakeup(m_wakeup);
	p_attr->set_current_plugin(m_current_plugin);
	p_attr->set_delete_flag(m_delete_flag);
	p_attr->set_hero_state(m_hero_state);
	for (auto& plugin : m_plugins)
	{
		p_attr->add_plugins(plugin);
	}

	proto::common::hero_skill_single *p_skill_single;
	HeroSkillMap::iterator iter = m_hero_skills.begin();
	for (; iter != m_hero_skills.end(); ++iter)
	{
		p_skill_single = p_attr->mutable_skill_data()->add_skill_list();
		p_skill_single->set_skill_id(iter->second.skill_id);
		p_skill_single->set_skill_level(iter->second.skill_level);
		p_skill_single->set_skill_pos(iter->second.skill_pos);
		p_skill_single->set_skill_index(iter->second.skill_index);
	}
	p_attr->set_last_equip_plan(string_util_t::uint64_to_string(m_last_equip_plan));
	p_attr->set_nick_name(m_nick_name);
	for (auto& famous : m_famous)
	{
		p_attr->add_famous(famous);
	}
}

void object_attr_t::wakeup()
{
	set_wakeup(1);
	fill_skill();
}

void object_attr_t::fill_skill()
{
	Hero* p_hero_conf = GET_CONF(Hero, m_tid);
	if (NULL == p_hero_conf)
	{
		log_error("NULL == p_hero_conf[%d]", m_tid);
		return;
	}
	HeroSkillMap tmp_map = m_hero_skills;
	m_hero_skills.clear();
	if (m_wakeup > 0)
	{
		for (uint32_t i = 0; i < p_hero_conf->wakeup_skills().size(); ++i)
		{
			hero_skill skill_info;
			uint32_t skill_index = i + 1;
			skill_info.skill_index = skill_index;
			skill_info.skill_id = p_hero_conf->wakeup_skills()[i];
			HeroSkillMap::iterator it = tmp_map.find(skill_index);
			if (it == tmp_map.end())
			{
				skill_info.skill_level = 1;
				skill_info.skill_pos = i + 1;
			}
			else
			{
				const hero_skill& hs = it->second;
				skill_info.skill_level = hs.skill_level;
				skill_info.skill_pos = hs.skill_pos;
			}
			m_hero_skills.insert(std::make_pair(skill_index, skill_info));
		}
	}
	else
	{
		for (uint32_t i = 0; i < p_hero_conf->default_skills().size(); ++i)
		{
			hero_skill skill_info;
			uint32_t skill_index = i + 1;
			skill_info.skill_index = skill_index;
			skill_info.skill_id = p_hero_conf->default_skills()[i];
			HeroSkillMap::iterator it = tmp_map.find(skill_index);
			if (it == tmp_map.end())
			{
				skill_info.skill_level = 1;
				skill_info.skill_pos = i + 1;
			}
			else
			{
				const hero_skill& hs = it->second;
				skill_info.skill_level = hs.skill_level;
				skill_info.skill_pos = hs.skill_pos;
			}
			m_hero_skills.insert(std::make_pair(skill_index, skill_info));
		}
	}

	
	for (uint32_t i = 0; i < p_hero_conf->more_skills().size(); ++i)
	{
		hero_skill skill_info;
		uint32_t skill_index = i + 11;
		skill_info.skill_index = skill_index;
		skill_info.skill_id = p_hero_conf->more_skills()[i];
		HeroSkillMap::iterator it = tmp_map.find(skill_index);
		if (it == tmp_map.end())
		{
			skill_info.skill_level = 0;
			skill_info.skill_pos = 0;
		}
		else
		{
			const hero_skill& hs = it->second;
			skill_info.skill_level = hs.skill_level;
			skill_info.skill_pos = hs.skill_pos;
		}
		m_hero_skills.insert(std::make_pair(skill_index, skill_info));
	}

}
