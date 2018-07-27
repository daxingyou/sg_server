#include "achieve_common.hpp"
#include "global_id.hpp"
#include "tblh/Achievelist.tbls.h"
#include "tblh/ActiveReward.tbls.h"
#include "tblh/Comprehensive.tbls.h"
#include "achieve_msg_handle.hpp"
#include "achieve_manager.hpp"
#include "achieve_method.hpp"
#include "active_reward_manager.hpp"
#include <map>
#include <set>
#include "role/role_manager.hpp"
#include "role/role.hpp"
#include "general_event/general_info_manager.hpp"
#include "business_activity/business_activity_manager.hpp"

achieve_common_t::achieve_common_t()
{

}

achieve_common_t::~achieve_common_t()
{
}

bool achieve_common_t::get_config_id(const uint64_t role_uid,achieve_common_type type,uint32_t achieve_event_type,uint32_t& id, uint32_t event_id)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", role_uid);
		return false;
	}

	switch (type)
	{
	case achieve_common_achieve:
	{
		auto MIN_ID_CONF = GET_CONF(Comprehensive, comprehensive_common::ACHIEVE_CONFIG_MIN_ID);
		const uint32_t min_level = GET_COMPREHENSIVE_VALUE_1(MIN_ID_CONF);
		std::map<uint32_t, Achievelist*> achieve_confs;
		GET_ALL_CONF(Achievelist, achieve_confs);
		for (auto& it : achieve_confs)
		{
			Achievelist* p_config = it.second;
			if (NULL == p_config)
			{
				log_error("NULL == p_config");
				return false;
			}
			const Dawn::triple<uint32_t, uint32_t, uint32_t>& tuple_list = p_config->condition();

			if (tuple_list.first != achieve_event_type)
				continue;

			if (achieve_event_type == proto::common::ACHIEVE_COUNTRY_BOUNTY_STAR_NUMBER ||
				achieve_event_type == proto::common::ACHIEVE_TASK_FINISH_STAR_TRIAL_NUMBER)
			{
				if (tuple_list.second > event_id)
					continue;
			}
			else
			{
				if (tuple_list.second != event_id)
					continue;
			}

			/*if (tuple_list.second != event_id)
				continue;*/

			id = std::abs(int(it.first - it.first % min_level));					///取以1000为单位的整数
			return achieve_manager_t::is_legal_achieve_id(id);
		}
	}break;
	case achieve_common_active_reward:
	{
		std::set<uint32_t>& list = p_role->get_active_reward_task_list();
		for (auto& it : list)
		{
			ActiveReward* p_config = GET_CONF(ActiveReward,it);
			if (NULL == p_config)
			{
				log_error("NULL == p_config[%d]", it);
				return false;
			}
			const Dawn::triple<uint32_t, uint32_t, uint32_t>& tuple_list = p_config->condition();

			if (tuple_list.first != achieve_event_type)
				continue;

			if (achieve_event_type == proto::common::ACHIEVE_COUNTRY_BOUNTY_STAR_NUMBER ||
				achieve_event_type == proto::common::ACHIEVE_TASK_FINISH_STAR_TRIAL_NUMBER)
			{
				if (tuple_list.second > event_id)
					continue;
			}
			else
			{
				if (tuple_list.second != event_id)
					continue;
			}

			/*if (tuple_list.second != event_id)
				continue;*/

			id = p_config->ID();
			return true;
		}
		
	}break;
	default:
		break;
	}
	
	return false;	
}

uint64_t achieve_common_t::gen_uid(achieve_common_type type)
{
	switch (type)
	{
	case achieve_common_achieve:
	{
		return common::g_id_allocator.gen_new_id(common::ID_ALLOCATOR_TYPE_ACHIEVE);
	}break;
	case achieve_common_active_reward:
	{
		return common::g_id_allocator.gen_new_id(common::ID_ALLOCATOR_TYPE_ACTIVE_REWARD);
	}break;
	default:
		break;
	}
	return 0;
}

void achieve_common_t::notify_progress_state(uint64_t uid, proto::common::Achieve_Event_Type type, uint32_t param1 /*= 1*/, uint32_t param2 /*= 1*/)
{
	achieve_msg_handle_t::handle_event_msg(uid, type, param1, param2);
}

uint32_t achieve_common_t::get_target_count(achieve_common_type type, uint32_t id)
{
	switch (type)
	{
	case achieve_common_achieve:
	{
		Achievelist* p_config = GET_CONF(Achievelist, id);
		if (NULL == p_config)
		{
			ACHIEVE_LOG("NULL == p_config achieve_id[%d]", id);
			return 0;
		}
		return p_config->condition().third;
	}break;
	case achieve_common_active_reward:
	{
		ActiveReward* p_config = GET_CONF(ActiveReward, id);
		if (NULL == p_config)
		{
			log_error("NULL == p_config active_reward_id[%d]", id);
			return 0;
		}
		return p_config->condition().third;
	}break;
	default:
		break;
	}
	return 0;
}

uint32_t achieve_common_t::get_drop_id(achieve_common_type type, uint32_t id)
{
	switch (type)
	{
	case achieve_common_achieve:
	{
		Achievelist* p_config = GET_CONF(Achievelist, id);
		if (NULL == p_config)
			return 0;
		return p_config->drop_id();
	}break;
	case achieve_common_active_reward:
	{
		ActiveReward* p_config = GET_CONF(ActiveReward, id);
		if (NULL == p_config)
			return 0;
		return p_config->drop_id();
	}break;
	default:
		break;
	}
	return 0;
}


bool achieve_common_t::add_element(achieve_common_type type,const uint64_t role_uid ,const uint32_t id)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", role_uid);
		return false;
	}
	if (0 == id)
	{
		log_error("0 == id role_uid[%lu] id[%d]!", role_uid, id);
		return false;
	}

	switch (type)
	{
	case achieve_common_achieve:
	{
		const std::map<uint32_t, achieve_ptr>& achieve_map = p_role->get_achieve_map();
		auto iter_achieve = achieve_map.find(id);
		if (iter_achieve == achieve_map.end())
		{
			uint64_t uid = achieve_common_t::gen_uid(achieve_common_achieve);
			achieve_ptr p_achieve = achieve_ptr(new achieve_t(uid, id, role_uid));
			if (!achieve_manager_t::add_achieve(role_uid, p_achieve))
			{
				log_error("role[%lu] add achieve[%d] to map list fail!", role_uid, id);
				return false;
			}
		}
		return true;
	}break;
	case achieve_common_active_reward:
	{
		ActiveReward* p_config = GET_CONF(ActiveReward, id);
		if (NULL == p_config)
		{
			log_error("NULL ==  p_config id[%d]", id);
			return false;
		}

		const Dawn::triple<uint32_t, uint32_t, uint32_t>& tuple_list = p_config->condition();
		std::map<uint32_t, active_reward_ptr>& active_reward_map = p_role->get_active_reward_map();
		auto iter = active_reward_map.find(id);
		if (iter == active_reward_map.end())
		{
			uint64_t uid = achieve_common_t::gen_uid(achieve_common_active_reward);
			active_reward_ptr p_active_reward = active_reward_ptr(new active_reward_t(uid, id, role_uid, tuple_list.first));
			if (NULL != p_active_reward)
			{
				p_active_reward->set_event_id(tuple_list.second);
				active_reward_map.insert(std::make_pair(p_active_reward->get_tid(), p_active_reward));
			}
			
			///保存
			///p_active_reward->save_self(role_uid);
			active_reward_manager_t::save_all_self(role_uid);
		}
		else
		{
			active_reward_ptr p_active_reward = iter->second;
			if (NULL == p_active_reward)
			{
				log_error("NULL == p_active_reward role[%lu] id[%d]", role_uid, id);
				return false;
			}

			if (p_active_reward->compare_is_same(id, tuple_list.first, tuple_list.second))
			{
				p_active_reward->set_type(tuple_list.first);
				p_active_reward->reset_count();
				p_active_reward->set_event_id(tuple_list.second);
				p_active_reward->reset_is_finish();
				p_active_reward->reset_is_grant();
				///p_active_reward->save_self(role_uid);
				active_reward_manager_t::save_all_self(role_uid);
			}
		}
		return true;
	}break;
	default:
		break;
	}
	return false;
}

bool achieve_common_t::get_hero_achieve_total(const uint64_t role_uid, const proto::common::Achieve_Event_Type type, const uint32_t event_id, uint32_t& total)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", role_uid);
		return false;
	}

	std::vector<hero_ptr> hero_list;
	hero_manager_t::get_all_heroes(p_role, hero_list);
	for (uint32_t i = 0; i < hero_list.size(); ++i)
	{
		hero_ptr p_hero = hero_list[i];
		if (NULL == p_hero)
			continue;
		switch (type)
		{
		case proto::common::Achieve_Event_Type::ACHIEVE_HERO_NUMBER_LEVEL:
		{
			uint32_t cul_leve = p_hero->get_current_level();
			if (cul_leve < event_id)
				continue;
			++total;
		}break;
		case proto::common::Achieve_Event_Type::ACHIEVE_HERO_NUMBER_TYPE:
		{
			uint32_t cul_leve = p_hero->get_grade();
			if (cul_leve < event_id)
				continue;
			++total;
		}break;
		case proto::common::Achieve_Event_Type::ACHIEVE_HERO_NUMBER_STAR:
		{
			uint32_t cul_leve = p_hero->get_star();
			if (cul_leve < event_id)
				continue;
			++total;
		}break;
		default:
			continue;
		}
	}

	return true;
}

void achieve_common_t::achieve_main_role_level(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t param1 /*= 0*/, uint32_t param2 /*= 0*/)
{
	achieve_common_t::set_statistical_data(uid, type, param1, param2, proto::common::STATISTICAL_OPTION::STATISTICAL_OPTION_ADD);
	///主角升级
	achieve_method_t::achieve_main_role_level(uid, type, param1, param2);						
	active_reward_manager_t::achieve_main_role_level(uid, type, param1, param2);				
}

void achieve_common_t::achieve_hero_cultivate(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t event_id, uint32_t num /*= 1*/)
{
	achieve_common_t::set_statistical_data(uid, type, event_id, num, proto::common::STATISTICAL_OPTION::STATISTICAL_OPTION_ADD);
	///英雄相关
	achieve_method_t::achieve_hero_cultivate(uid, type, event_id, num);								
	active_reward_manager_t::achieve_hero_cultivate(uid, type, event_id, num);
}

void achieve_common_t::achieve_unify_handle(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t event_id, uint32_t num /*= 1*/)
{
	achieve_common_t::set_statistical_data(uid, type, event_id, num, proto::common::STATISTICAL_OPTION::STATISTICAL_OPTION_ADD);
	///n * n 
	achieve_method_t::achieve_unify_handle(uid, type, event_id, num);
	active_reward_manager_t::achieve_unify_handle(uid, type, event_id, num);
}

void achieve_common_t::achieve_total_unify_handle(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t param /*= 0*/, uint32_t num /*= 1*/)
{
	achieve_common_t::set_statistical_data(uid, type, param, num, proto::common::STATISTICAL_OPTION::STATISTICAL_OPTION_ADD);
	/// n 累加
	achieve_method_t::achieve_total_unify_handle(uid, type, param, num);
	active_reward_manager_t::achieve_total_unify_handle(uid, type, param, num);
}

void achieve_common_t::achieve_replace_model(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t event_id, uint32_t new_value)
{
	achieve_common_t::set_statistical_data(uid, type, event_id, new_value, proto::common::STATISTICAL_OPTION::STATISTICAL_OPTION_CMP_REPLACE);
	/// 替换
	achieve_method_t::achieve_replace_model_handle(uid, type, event_id, new_value);
	active_reward_manager_t::achieve_replace_model_handle(uid, type, event_id, new_value);
}

void achieve_common_t::achieve_direct_replace_model_handle(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t event_id /*= 0*/, uint32_t new_value /*= 0*/)
{
	achieve_common_t::set_statistical_data(uid, type, event_id, new_value, proto::common::STATISTICAL_OPTION::STATISTICAL_OPTION_REPLACE);
	/// 直接替换
	achieve_method_t::achieve_direct_replace_model_handle(uid, type, event_id, new_value);
}


void achieve_common_t::set_statistical_data(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t event_id, uint32_t count, uint32_t option)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", uid);
		return;
	}

	uint32_t id = (type * 1000 + event_id);

	p_role->add_statistical_count(id, count, option);

//七日目标统计	
	business_activity_manager_t::check_seven_day_target(p_role);

//天下大势统计////////////
	//取出最大值
	uint32_t achieve_id = 0;
	if (!achieve_common_t::get_config_id(uid, achieve_common_achieve, type, achieve_id))
	{
		ACHIEVE_LOG("role_uid[%lu] achieve_id[%d] not legal Achieve_Event_Type[%d] event_id[%d]", uid, achieve_id, type, 0);
		return;
	}
	uint32_t max_value = 0;
	achieve_ptr p_achieve = achieve_manager_t::get_achieve(uid, achieve_id);
	if (p_achieve)
		max_value = p_achieve->get_count();

	general_info_manager_t::update_progress(type, max_value, count, p_role);
}



void achieve_common_t::test_add_achieve(uint64_t role_uid)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", role_uid);
		return;
	}

	achieve_common_t::notify_progress_state(role_uid, proto::common::Achieve_Event_Type::ACHIEVE_TOTAL_COPPER, 0, 99999999);
	achieve_common_t::notify_progress_state(role_uid, proto::common::Achieve_Event_Type::ACHIEVE_PK_VALUE, 0, 99999999);
	achieve_common_t::notify_progress_state(role_uid, proto::common::Achieve_Event_Type::ACHIEVE_TRADE_SINGLE_GAIN, 0, 99999999);
	achieve_common_t::notify_progress_state(role_uid, proto::common::Achieve_Event_Type::ACHIEVE_TRADE_TOTAL_GAIN_NUMBER, 0, 99999999);
	achieve_common_t::notify_progress_state(role_uid, proto::common::Achieve_Event_Type::ACHIEVE_SUCCESSION_LOGIN, 1, 100);
	achieve_common_t::notify_progress_state(role_uid, proto::common::ACHIEVE_EXPEDITION_STORE_TOTAL_COST_HONOUR, 0,9999999);
	for (int i = 0; i < 50;i++)
	{
		///商城购买次数
		achieve_common_t::notify_progress_state(role_uid, proto::common::ACHIEVE_SHOP_BUY_NUMBER, proto::common::shop_type_exchange);
	}
	for (int i = 0; i < 8;i++)
	{
		///累计登陆
		achieve_common_t::notify_progress_state(role_uid, proto::common::ACHIEVE_TOTAL_LOGIN, 1, 1);
	}
	for (int i = 0; i < 50; i++)
	{
		///跑商次数
		achieve_common_t::notify_progress_state(role_uid, proto::common::ACHIEVE_TRADE_NUMBER, proto::common::shop_type_exchange);
	}
	for (int i = 0; i < 50;i++)
	{
		///加好友
		achieve_common_t::notify_progress_state(role_uid, proto::common::ACHIEVE_HAVE_FRIEND_NUMBER);
	}
	for (int i = 0; i < 60;i++)
	{
		//任务
		achieve_common_t::notify_progress_state(role_uid, proto::common::Achieve_Event_Type::ACHIEVE_TASK_FINISH_STAR_TRIAL_NUMBER, 5);
		achieve_common_t::notify_progress_state(role_uid, proto::common::Achieve_Event_Type::ACHIEVE_CHASE_GHOST_CIRCLE_NUMBER);
		achieve_common_t::notify_progress_state(role_uid, proto::common::Achieve_Event_Type::ACHIEVE_TASK_FINISH_NUMBER);
		achieve_common_t::notify_progress_state(role_uid, proto::common::Achieve_Event_Type::ACHIEVE_TASK_FINISH_FAMILY_NUMBER);
		achieve_common_t::notify_progress_state(role_uid, proto::common::Achieve_Event_Type::ACHIEVE_TASK_FINISH_COUNTRY_NUMBER);
		achieve_common_t::notify_progress_state(role_uid, proto::common::Achieve_Event_Type::ACHIEVE_TASK_FINISH_EXP_CIRCLE_NUMBER);
		achieve_common_t::notify_progress_state(role_uid, proto::common::Achieve_Event_Type::ACHIEVE_TASK_FINISH_TRIAL_NUMBER);
		achieve_common_t::notify_progress_state(role_uid, proto::common::Achieve_Event_Type::ACHIEVE_TASK_FINISH_CHASE_GHOST_NUMBER);
		achieve_common_t::notify_progress_state(role_uid, proto::common::Achieve_Event_Type::ACHIEVE_TREASURE_ACTIVITY_NUMBER);
		achieve_common_t::notify_progress_state(role_uid, proto::common::Achieve_Event_Type::ACHIEVE_TREASURE_ACTIVITY_SPECIAL_EVENT_NUMBER);
	}

	for (int i = 0;i < 60;i++)
	{
		//皇城站
		achieve_common_t::notify_progress_state(role_uid, proto::common::Achieve_Event_Type::ACHIEVE_EMPIRE_CITY_CHALLENGE_NUMBER);
		achieve_common_t::notify_progress_state(role_uid, proto::common::ACHIEVE_FIELD_BOSS_KILL_NUMBER);
		achieve_common_t::notify_progress_state(role_uid, proto::common::ACHIEVE_ARENA_BATTLE_TOTAL_WIN_NUMBER);
		achieve_common_t::notify_progress_state(role_uid, proto::common::ACHIEVE_TOWER_NUMBER);
		achieve_common_t::notify_progress_state(role_uid, proto::common::ACHIEVE_EXPEDITION_STORE_BUY_NUMBER);
		achieve_common_t::notify_progress_state(role_uid, proto::common::ACHIEVE_EXPEDITION_HELP_OTHERS);
		achieve_common_t::notify_progress_state(role_uid, proto::common::ACHIEVE_FAMILY_WAR_NUMBER);
		achieve_common_t::notify_progress_state(role_uid, proto::common::ACHIEVE_KING_WAR_NUMBER);
		achieve_common_t::notify_progress_state(role_uid, proto::common::ACHIEVE_FAMILY_WAR_OCCUPIED_SUN_OR_MOON_NUMBER);
		achieve_common_t::notify_progress_state(role_uid, proto::common::ACHIEVE_FAMILY_WAR_KILL_PLAYER_NUMBER);
		achieve_common_t::notify_progress_state(role_uid, proto::common::ACHIEVE_KING_WAR_KILL_PLAYER_NUMBER);
		achieve_common_t::notify_progress_state(role_uid, proto::common::ACHIEVE_KING_WAR_QUICK_REVIVE);
		achieve_common_t::notify_progress_state(role_uid, proto::common::ACHIEVE_KING_WAR_START_WEAPON);
		achieve_common_t::notify_progress_state(role_uid, proto::common::ACHIEVE_DUNGEON_TOTAL_PASS);
	}

	for (int i = 0; i < 300;i++)
	{
		achieve_common_t::notify_progress_state(role_uid, proto::common::ACHIEVE_STRENGTHEN_EQUIP_NUMBER);
	}

	for (int i = 0;i < 80;i++)
	{
		achieve_common_t::notify_progress_state(role_uid, proto::common::ACHIEVE_HERO_UP_LEVEL_NUMBER);
		
		achieve_common_t::notify_progress_state(role_uid, proto::common::ACHIEVE_PATROL_KILL_MONSTER_GROUP_NUMBER);
		achieve_common_t::notify_progress_state(role_uid, proto::common::ACHIEVE_EQUIP_RECAST);
	}
}



