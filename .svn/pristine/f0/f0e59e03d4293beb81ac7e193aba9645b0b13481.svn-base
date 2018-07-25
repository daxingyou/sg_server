#include "lifeforce_attr_manager.hpp"
#include "tblh/LifeStar.tbls.h"
#include "role/role.hpp"
#include "role/role_manager.hpp"
#include "role/money_manager.hpp"
#include "item/drop_manager.hpp"
#include "common/MyTuple.h"
#include "hero/hero_manager.hpp"
#include "hero/hero.hpp"
#include "lifeforce/lifeforce_manager.hpp"
#include "formation/formation.hpp"
#include "formation/formation_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "tblh/LabelTable.tbls.h"

USING_NS_CONFIG;
bool lifeforce_attr_manager_t::grant_lifeforce_reward(uint64_t role_uid,uint32_t source_type, 
														proto::common::role_change_data* p_data)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid [%lu]", role_uid);
		return false;
	}

	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return false;
	}

	LIFEFORCE_LOG("role[%lu] begin grant reward!", role_uid);

	const uint32_t star_level = p_role->get_lifestar_level();
	const uint32_t state_level = p_role->get_lifestate_level();

	//一级境界零星 返回
	if (1 >= state_level && 0 == star_level)
	{
		log_error("role[%lu] grant lifeforce reward fail state level[%d] star level[%d]",
			role_uid, state_level, star_level);
		return false;
	}

	LifeStar* p_config = GET_CONF(LifeStar, hash_util_t::hash_to_uint32(state_level, star_level));
	if (NULL == p_config)
	{
		log_error("NULL == p_config role[%lu] state level[%d] star level[%d]", 
				role_uid, state_level, star_level);
		return false;
	}

	//奖励类型 <奖励/ID>
	const std::map<uint32_t, uint32_t>& other_reward = p_config->other_reward();

	for (auto& it : other_reward)
	{
		switch (it.first)
		{
		case LIFEFORCE_REWARD_TYPE::LIFEFORCE_REWARD_SKILL:
		{
			add_skill_reward(role_uid, it.second, source_type, p_data);
		}
		break;
		case LIFEFORCE_REWARD_TYPE::LIFEFORCE_REWARD_FORMATION:
		{
			//add_tactic_reward(role_uid, it.second, source_type, p_data);
		}
		break;
		case LIFEFORCE_REWARD_TYPE::LIFEFORCE_REWARD_ITEM:
		{
			return add_item_reward(role_uid, it.second, source_type, p_data);
		}
		break;
		default:
			break;
		}
	}
	add_single_attr_reward(role_uid, p_data);
	return true;
}


bool lifeforce_attr_manager_t::add_tactic_reward(
	uint64_t role_uid,
	uint32_t tactic_id,
	uint32_t source_type,
	proto::common::role_change_data* p_data)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", role_uid);
		return false;
	}
	tactic_mgr_ptr p_tactic = p_role->get_tactic_mgr();
	if (NULL == p_tactic)
	{
		log_error("NULL == p_tactic role[%lu]",role_uid);
		return false;
	}
	if (p_tactic->has_tactic(tactic_id))
	{
		LIFEFORCE_LOG("role[%lu] already have tactic[%d]",role_uid, tactic_id);
		return true;
	}
	if (errcode_enum::error_ok != p_tactic->add_tactic(tactic_id,true))
	{
		log_error("role[%lu] add tactic[%d]", role_uid, tactic_id);
		return false;
	}
	return true;
}

bool lifeforce_attr_manager_t::add_item_reward(uint64_t role_uid,uint32_t drop_id, uint32_t source_type, 
												proto::common::role_change_data* p_data)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", role_uid);
		return false;
	}

	const uint32_t state_level = p_role->get_lifestate_level();
	const uint32_t star_level = p_role->get_lifestar_level();

	LIFEFORCE_LOG("role[%lu] state_level[%d] star_level[%d] begin grant item reward drop id[%d]", 
		role_uid, state_level, star_level,drop_id);


	LIFEFORCE_LOG("role[%lu] drop_id[%d]", role_uid, drop_id);
	bool b_result = drop_manager_t::drop(p_role, drop_id, source_type,
		proto::common::drop_sys_type_lifeforce, state_level, p_data);


	if (!b_result)
	{
		log_error("role[%lu] state_level[%d] star_level[%d] grant item reward fail drop_id[%d]",
			role_uid, state_level, star_level, drop_id);
		return false;
	}
	return true;
}

bool lifeforce_attr_manager_t::add_skill_reward(uint64_t role_uid,uint32_t skill_id, uint32_t source_type, 
	proto::common::role_change_data* p_data)
{
	//技能模块暂时没有
	return true;
}

void lifeforce_attr_manager_t::add_single_attr_reward(uint64_t role_uid, proto::common::role_change_data* p_data)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", role_uid);
		return;
	}

	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return;
	}

	std::vector<hero_ptr> hero_list;
	hero_manager_t::get_all_heroes_include_hide(p_role, hero_list);
	for (uint32_t i = 0; i < hero_list.size(); ++i)
	{
		hero_ptr p_hero = hero_list[i];
		if(NULL == p_hero)
			continue;
		p_hero->calc_fighting();
		proto::common::hero_data* p_hero_data = p_data->mutable_hero();
		proto::common::hero_single* p_hero_single = p_hero_data->add_hero_list();
		p_hero->peek_data(p_hero_single);
	}
}

uint32_t lifeforce_attr_manager_t::add_fighting(uint64_t role_uid)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", role_uid);
		return 0;
	}
	const uint32_t cur_label_level = p_role->get_lifeforce_label();
	LabelTable* p_config = GET_CONF(LabelTable, cur_label_level);
	if (NULL == p_config)
	{
		LIFEFORCE_LOG("NULL == p_config id[%d]",cur_label_level);
		return 0;
	}

	return p_config->fight_add();
}
