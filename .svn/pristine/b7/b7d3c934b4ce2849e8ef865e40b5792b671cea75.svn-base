#include "equip_plan_manager.hpp"
#include "main/game_server_fwd.hpp"
#include "main/game_server.hpp"
#include "cache_key.hpp"
#include "role/role_unify_save.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "config/config_manager.hpp"
#include "common/utility.hpp"
#include "log/log_wrapper.hpp"
#include "tblh/errcode_enum.hpp"
#include "role/role_manager.hpp"
#include "global_id.hpp"
#include "equip_manager.hpp"
#include "hero/hero_manager.hpp"

equip_plan_manager_t::equip_plan_manager_t(uint64_t uid)
	: m_owner(uid)
{
	m_key = cache_key_t::create(m_owner, cache_name::equip_plan);
}

equip_plan_manager_t::~equip_plan_manager_t()
{

}

void equip_plan_manager_t::load_data(const proto::common::equip_plan_data& ld)
{
	for (int32_t i = 0; i < ld.equip_plan_info_size(); ++i)
	{
		equip_plan_ptr p_equip_plan = equip_plan_ptr(new equip_plan_t);
		p_equip_plan->load_data(ld.equip_plan_info(i));
		m_equip_plan_map.insert(std::make_pair(p_equip_plan->get_uid(), p_equip_plan));
	}
}

void equip_plan_manager_t::peek_data(proto::common::equip_plan_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return;
	}
	for (auto it : m_equip_plan_map)
	{
		equip_plan_ptr p_equip_plan = it.second;
		if (NULL == p_equip_plan)
		{
			log_error("NULL == p_equip_plan role[%lu]", m_owner);
			continue;
		}
		proto::common::equip_plan_single* p_single = p_data->add_equip_plan_info();
		p_equip_plan->peek_data(p_single);
	}
}

void equip_plan_manager_t::save_self()
{
	proto::common::equip_plan_data equip_plan_data;
	peek_data(&equip_plan_data);
	role_unify_save::add_task(m_owner, m_key, equip_plan_data);
}

uint64_t equip_plan_manager_t::gen_new_equip_plan_id()
{
	uint64_t uid = common::g_id_allocator.gen_new_id(common::ID_ALLOCATOR_TYPE_EQUIP_PLAN);
	return uid;
}

uint32_t equip_plan_manager_t::create_plan(const std::string& name, uint64_t weapon, uint64_t armor, uint64_t talisman)
{
	if (m_equip_plan_map.size() >= get_max_num())
	{
		log_error("role[%lu] create equip plan failed, full!!", m_owner);
		return errcode_enum::equip_manage_error_2;
	}
	if (has_same_name(name))
	{
		log_error("role[%lu] create equip plan failed, name[%s] exist", m_owner, name.c_str());
		return errcode_enum::equip_manage_error_1;
	}
	if (has_same_plan(weapon, armor, talisman))
	{
		log_error("role[%lu] create equip plan failed, weapon[%lu] armor[%lu] talisman[%lu] exist", 
			m_owner, weapon, armor, talisman);
		return errcode_enum::notice_unknown;
	}
	uint64_t uid = gen_new_equip_plan_id();
	equip_plan_ptr p_equip_plan = equip_plan_ptr(new equip_plan_t(uid));
	p_equip_plan->set_name(name);
	p_equip_plan->set_weapon(weapon);
	p_equip_plan->set_armor(armor);
	p_equip_plan->set_talisman(talisman);
	m_equip_plan_map.insert(std::make_pair(uid, p_equip_plan));
	EQUIP_LOG("role[%lu] create equip plan[%lu] name[%s] success, weapon[%lu] armor[%lu] talisman[%lu] exist",
		m_owner, uid, name.c_str(), weapon, armor, talisman);
	if (weapon > 0)
	{
		check_equip_hero(weapon);
	}
	if (armor > 0)
	{
		check_equip_hero(armor);
	}
	if (talisman > 0)
	{
		check_equip_hero(talisman);
	}
	save_self();
	return errcode_enum::error_ok;
}

uint32_t equip_plan_manager_t::modify_plan(uint64_t uid, const std::string& name, uint64_t weapon, uint64_t armor, uint64_t talisman)
{
	equip_plan_ptr p_equip_plan = get_equip_plan(uid);
	if (NULL == p_equip_plan)
	{
		log_error("role[%lu] equip_plan[%lu] not exist", m_owner, uid);
		return errcode_enum::notice_unknown;
	}
	if (p_equip_plan->get_name() == name &&
		p_equip_plan->get_weapon() == weapon &&
		p_equip_plan->get_armor() == armor &&
		p_equip_plan->get_talisman() == talisman)
	{
		log_error("role[%lu] equip_plan[%lu] modify invalid, no change", m_owner, uid);
		return errcode_enum::notice_unknown;
	}
	if (p_equip_plan->get_name() != name)
	{
		if (has_same_name(name))
		{
			log_error("role[%lu] modify equip plan[%lu] failed, name[%s] exist", m_owner, uid, name.c_str());
			return errcode_enum::equip_manage_error_1;
		}
	}
	//if (has_same_plan(weapon, armor, talisman))
	//{
	//	log_error("role[%lu] modify equip plan[%lu] failed, weapon[%lu] armor[%lu] talisman[%lu] exist, delete",
	//		m_owner, uid, weapon, armor, talisman);
	//	del_plan(uid);
	//	return errcode_enum::notice_unknown;
	//}
	p_equip_plan->set_name(name);
	p_equip_plan->set_weapon(weapon);
	p_equip_plan->set_armor(armor);
	p_equip_plan->set_talisman(talisman);
	p_equip_plan->set_hero_uid(0);
	save_self();
	EQUIP_LOG("role[%lu] modify equip plan[%lu] success, name[%s] weapon[%lu] armor[%lu] talisman[%lu] exist",
		m_owner, uid, name.c_str(), weapon, armor, talisman);
	return errcode_enum::error_ok;
}

uint32_t equip_plan_manager_t::delete_plan(uint64_t uid)
{
	equip_plan_ptr p_equip_plan = get_equip_plan(uid);
	if (NULL == p_equip_plan)
	{
		log_error("role[%lu] equip_plan[%lu] not exist", m_owner, uid);
		return errcode_enum::notice_unknown;
	}
	uint64_t hero_uid = p_equip_plan->get_hero_uid();
	if (hero_uid > 0)
	{
		role_ptr p_role = role_manager_t::find_role(m_owner);
		if (NULL == p_role)
		{
			log_error("NULL == p_role[%lu]", m_owner);
			return errcode_enum::notice_role_null;
		}
		hero_ptr p_hero = hero_manager_t::get_hero_by_unique_id(p_role, hero_uid);
		if (NULL == p_hero)
		{
			log_error("role[%lu] NULL == p_hero[%lu]", m_owner, hero_uid);
			return errcode_enum::notice_hero_null;
		}
		p_hero->set_last_equip_plan(0);
	}
	del_plan(uid);
	save_self();
	EQUIP_LOG("role[%lu] delete equip plan[%lu] success", m_owner, uid);
	return errcode_enum::error_ok;
}

uint32_t equip_plan_manager_t::use_plan(uint64_t equip_plan_uid, uint64_t hero_uid, proto::common::role_change_data* p_data)
{
	equip_plan_ptr p_equip_plan = get_equip_plan(equip_plan_uid);
	if (NULL == p_equip_plan)
	{
		log_error("role[%lu] equip_plan[%lu] not exist", m_owner, equip_plan_uid);
		return errcode_enum::notice_unknown;
	}
	role_ptr p_role = role_manager_t::find_role(m_owner);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_owner);
		return errcode_enum::notice_role_null;
	}
	hero_ptr p_hero = hero_manager_t::get_hero_by_unique_id(p_role, hero_uid);
	if (NULL == p_hero)
	{
		log_error("role[%lu] NULL == p_hero[%lu]", m_owner, hero_uid);
		return errcode_enum::notice_hero_null;
	}
	use_plan_equip(hero_uid, equip_pos_weapon, p_equip_plan->get_weapon(), p_data);
	use_plan_equip(hero_uid, equip_pos_armor, p_equip_plan->get_armor(), p_data);
	use_plan_equip(hero_uid, equip_pos_talisman, p_equip_plan->get_talisman(), p_data);
	save_self();
	EQUIP_LOG("role[%lu] hero[%lu:%d] use equip plan[%lu] success", m_owner, hero_uid, p_hero->get_tid(), equip_plan_uid);
	return errcode_enum::error_ok;
}

void equip_plan_manager_t::check_hero_plan(uint64_t hero_uid)
{
	role_ptr p_role = role_manager_t::find_role(m_owner);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_owner);
		return;
	}
	hero_ptr p_hero = p_role->get_hero_by_unique_id(hero_uid);
	if (NULL == p_hero)
	{
		log_error("role[%lu] NULL == p_hero[%lu]", p_role->get_uid(), hero_uid);
		return;
	}
	EQUIP_LOG("role[%lu] check_hero_plan[%lu]", m_owner, hero_uid);
	for (auto it : m_equip_plan_map)
	{
		equip_plan_ptr p_equip_plan = it.second;
		if (NULL == p_equip_plan)
		{
			continue;
		}
		//EQUIP_LOG("role[%lu] p_equip_plan->get_hero_uid[%lu]", m_owner, p_equip_plan->get_hero_uid());
		if (p_equip_plan->get_hero_uid() == hero_uid)
		{
			p_equip_plan->set_hero_uid(0);
			p_hero->set_last_equip_plan(p_equip_plan->get_uid());
			EQUIP_LOG("role[%lu] hero[%lu] cancel equip_plan[%lu]", m_owner, hero_uid, p_equip_plan->get_uid());
		}
		if (p_equip_plan->get_weapon() == p_hero->get_equip(equip_pos_weapon)
			&& p_equip_plan->get_armor() == p_hero->get_equip(equip_pos_armor)
			&& p_equip_plan->get_talisman() == p_hero->get_equip(equip_pos_talisman))
		{
			p_equip_plan->set_hero_uid(hero_uid);
			EQUIP_LOG("role[%lu] hero[%lu] use equip_plan[%lu]", m_owner, hero_uid, p_equip_plan->get_uid());
		}
	}
}

equip_plan_ptr equip_plan_manager_t::get_equip_plan(uint64_t uid)
{
	auto it = m_equip_plan_map.find(uid);
	if (it == m_equip_plan_map.end())
	{
		return NULL;
	}
	return it->second;
}


void equip_plan_manager_t::del_plan(uint64_t uid)
{
	auto it = m_equip_plan_map.find(uid);
	if (it != m_equip_plan_map.end())
	{
		m_equip_plan_map.erase(it);
	}
}

bool equip_plan_manager_t::has_same_name(const std::string& name)
{
	for (auto it : m_equip_plan_map)
	{
		equip_plan_ptr p_equip_plan = it.second;
		if (NULL == p_equip_plan)
		{
			continue;
		}
		if (p_equip_plan->get_name() == name)
		{
			return true;
		}
	}
	return false;
}


bool equip_plan_manager_t::has_same_plan(uint64_t weapon, uint64_t armor, uint64_t talisman)
{
	for (auto it : m_equip_plan_map)
	{
		equip_plan_ptr p_equip_plan = it.second;
		if (NULL == p_equip_plan)
		{
			continue;
		}
		if (p_equip_plan->get_weapon() == weapon && 
			p_equip_plan->get_armor() == armor && 
			p_equip_plan->get_talisman() == talisman)
		{
			return true;
		}
	}
	return false;
}

uint32_t equip_plan_manager_t::get_max_num()
{
	return 20;
}

void equip_plan_manager_t::use_plan_equip(uint64_t hero_uid, uint32_t equip_pos, uint64_t equip_uid, proto::common::role_change_data* p_data)
{
	role_ptr p_role = role_manager_t::find_role(m_owner);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_owner);
		return;
	}
	EQUIP_LOG("role[%lu] hero[%lu] use_plan_equip pos[%d] uid[%lu]", m_owner, hero_uid, equip_pos, equip_uid);
	if (equip_uid > 0)
	{
		item_ptr p_equip = equip_manager_t::get_equip_by_uid(p_role, equip_uid);
		if (NULL == p_equip)
		{
			log_error("role[%lu] p_equip[%lu] not found", m_owner, equip_uid);
			return;
		}
		uint64_t last_hero_uid = p_equip->get_hero_uid();
		if (last_hero_uid > 0)
		{
			if (last_hero_uid != hero_uid)
			{
				equip_manager_t::equip_off(p_role, equip_uid, last_hero_uid, p_data);
				equip_manager_t::equip_on(p_role, equip_uid, hero_uid, p_data);
			}
			else
			{
				//什么都不用做
			}
		}
		else
		{
			equip_manager_t::equip_on(p_role, equip_uid, hero_uid, p_data);
		}
	}
	else
	{
		hero_ptr p_hero = p_role->get_hero_by_unique_id(hero_uid);
		if (NULL == p_hero)
		{
			log_error("role[%lu] NULL == p_hero[%lu]", p_role->get_uid(), hero_uid);
			return;
		}
		uint64_t exist_equip_id = p_hero->get_equip(equip_pos);
		EQUIP_LOG("role[%lu] hero[%lu] use_plan_equip pos[%d] exist_equip_id[%lu]", m_owner, hero_uid, equip_pos, exist_equip_id);
		if (exist_equip_id > 0)
		{
			equip_manager_t::equip_off(p_role, exist_equip_id, hero_uid, p_data);
		}
	}
}

void equip_plan_manager_t::check_equip_hero(uint64_t equip_uid)
{
	role_ptr p_role = role_manager_t::find_role(m_owner);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_owner);
		return;
	}
	item_ptr p_equip = equip_manager_t::get_equip_by_uid(p_role, equip_uid);
	if (NULL == p_equip)
	{
		log_error("role[%lu] p_equip[%lu] not found", m_owner, equip_uid);
		return;
	}
	uint64_t last_hero_uid = p_equip->get_hero_uid();
	if (last_hero_uid > 0)
	{
		check_hero_plan(last_hero_uid);
	}
}

void equip_plan_manager_t::del_equip(uint64_t equip_uid)
{
	for (auto it : m_equip_plan_map)
	{
		equip_plan_ptr p_equip_plan = it.second;
		if (NULL == p_equip_plan)
		{
			continue;
		}
		if (p_equip_plan->get_weapon() == equip_uid)
		{
			p_equip_plan->set_weapon(0);
		}
		if (p_equip_plan->get_armor() == equip_uid)
		{
			p_equip_plan->set_armor(0);
		}
		if (p_equip_plan->get_talisman() == equip_uid)
		{
			p_equip_plan->set_talisman(0);
		}
	}
}

