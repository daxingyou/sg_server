#include "achieve_manager.hpp"
#include "global_id.hpp"
#include "role/money_manager.hpp"
#include "common/config_mgr.h"
#include "achieve_method.hpp"
#include "role/role.hpp"
#include "role/role_manager.hpp"
#include "item/drop_manager.hpp"
#include "hero/hero_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "errcode_enum.hpp"
#include "common/MyTuple.h"
#include "achieve/achieve_msg_handle.hpp"
#include "hero/hero.hpp"
#include "tblh/HeroGrade.tbls.h"
#include "log/log_wrapper.hpp"
#include "common/utility.hpp"
#include "achieve_common.hpp"
#include "red_point/red_point_manager.hpp"
#include "role/role_unify_save.hpp"
#include "cache_key.hpp"

USING_NS_CONFIG;
USING_NS_COMMON;

void achieve_manager_t::init()
{
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_MAIN_ROLE_LEVEL] = log_enum::source_type_achieve_main_role_level;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_MAIN_ROLE_FIGHT] = log_enum::source_type_achieve_main_role_fight;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_EQUIP_TYPE_NUMBER] = log_enum::source_type_achieve_equip_type_number;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_EQUIP_TYPE_SAME] = log_enum::source_type_achieve_equip_type_same;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_EQUIP_RECAST] = log_enum::source_type_achieve_equip_recast;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_EQUIP_EXATTR_NUMBER_STAR] = log_enum::source_type_achieve_equip_exattr_number_star;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_HERO_NUMBER] = log_enum::source_type_achieve_hero_number;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_HERO_NUMBER_LEVEL] = log_enum::source_type_achieve_hero_number_level;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_HERO_NUMBER_TYPE] = log_enum::source_type_achieve_hero_number_type;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_HERO_NUMBER_STAR] = log_enum::source_type_achieve_hero_number_star;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_HERO_NUMBER_TALENT_LEVEL] = log_enum::source_type_achieve_hero_number_talent_level;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_HERO_NUMBER_SKILL_LEVEL] = log_enum::source_type_achieve_hero_number_skill_level;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_TOTAL_COPPER] = log_enum::source_type_achieve_total_copper;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_SHOP_BUY_NUMBER] = log_enum::source_type_achieve_shop_buy_number;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_MOUNT_NUMBER_TYPE] = log_enum::source_type_achieve_mount_number_type;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_TASK_FINISH_NUMBER] = log_enum::source_type_achieve_task_finish_number;	
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_TASK_FINISH_COUNTRY_NUMBER] = log_enum::source_type_achieve_task_finish_country_number;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_TASK_FINISH_FAMILY_NUMBER] = log_enum::source_type_achieve_task_finish_family_number;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_TRADE_NUMBER] = log_enum::source_type_achieve_trade_number;	
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_TRADE_TOTAL_GAIN_NUMBER] = log_enum::source_type_achieve_trade_total_gain_number;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_COUNTRY_BOUNTY_STAR_NUMBER] = log_enum::source_type_achieve_country_bounty_star_number;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_TREASURE_ACTIVITY_NUMBER] = log_enum::source_type_achieve_treasure_activity_number;	
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_TREASURE_ACTIVITY_SPECIAL_EVENT_NUMBER] = log_enum::source_type_achieve_treasure_activity_special_event_number;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_CHANNEL_CHAT] = log_enum::source_type_achieve_channel_chat;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_FAMILY] = log_enum::source_type_achieve_family;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_COUNTRY] = log_enum::source_type_achieve_country;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_EMPIRE_CITY_CHALLENGE_NUMBER] = log_enum::source_type_achieve_empire_city_challenge_number;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_PK_VALUE] = log_enum::source_type_achieve_pk_value;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_TASK_FINISH_TRIAL_NUMBER] = log_enum::source_type_achieve_task_finish_trial_number;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_PACKAGE_COPPER_NUMBER] = log_enum::source_type_achieve_package_copper_number;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_HAVE_FRIEND_NUMBER] = log_enum::source_type_achieve_have_friend_number;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_TRADE_SINGLE_GAIN] = log_enum::source_type_achieve_trade_single_gain;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_TASK_FINISH_STAR_TRIAL_NUMBER] = log_enum::source_type_achieve_task_finish_star_trial_number;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_TOTAL_LOGIN] = log_enum::source_type_achieve_total_login;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_SUCCESSION_LOGIN] = log_enum::source_type_achieve_succession_login;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_TASK_FINISH_CHASE_GHOST_NUMBER] = log_enum::source_type_achieve_task_finish_chase_ghost_number;	
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_CHASE_GHOST_CIRCLE_NUMBER] = log_enum::source_type_achieve_chase_ghost_circle_number;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_FIELD_BOSS_KILL_NUMBER] = log_enum::source_type_achieve_field_boss_kill_number;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_ARENA_BATTLE_SINGLE_WIN_NUMBER] = log_enum::source_type_achieve_arena_battle_single_win_number;	
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_ARENA_BATTLE_TOTAL_WIN_NUMBER] = log_enum::source_type_achieve_arena_battle_total_win_number;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_TOWER_LEVEL] = log_enum::source_type_achieve_tower_level;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_EXPEDITION_SINGLE_POINT] = log_enum::source_type_achieve_expedition_single_point;	
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_EXPEDITION_STORE_BUY_NUMBER] = log_enum::source_type_achieve_expedition_store_buy_number;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_EXPEDITION_STORE_TOTAL_COST_HONOUR] = log_enum::source_type_achieve_expedition_store_total_cost_honour;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_EXPEDITION_HELP_OTHERS] = log_enum::source_type_achieve_expedition_help_others;	
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_EXPEDITION_CHALLENGE_PASS_NUMBER] = log_enum::source_type_achieve_expedition_challenge_pass_number;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_FAMILY_WAR_NUMBER] = log_enum::source_type_achieve_family_war_number;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_FAMILY_WAR_KILL_PLAYER_NUMBER] = log_enum::source_type_achieve_family_war_kill_player_number;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_FAMILY_WAR_DRAGON_HARM] = log_enum::source_type_achieve_family_war_dragon_harm;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_FAMILY_WAR_OCCUPIED_SUN_OR_MOON_NUMBER] = log_enum::source_type_achieve_family_war_occupied_sun_or_moon_number;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_KING_WAR_NUMBER] = log_enum::source_type_achieve_king_war_number;	
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_KING_WAR_KILL_PLAYER_NUMBER] = log_enum::source_type_achieve_king_war_kill_player_number;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_KING_WAR_DRAGON_HARM] = log_enum::source_type_achieve_king_war_dragon_harm;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_KING_WAR_QUICK_REVIVE] = log_enum::source_type_achieve_king_war_quick_revive;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_KING_WAR_START_WEAPON] = log_enum::source_type_achieve_king_war_start_weapon;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_GRADE_PASS_DUNGEON_NUMBER] = log_enum::source_type_achieve_grade_pass_dungeon_number;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_DUNGEON_TOTAL_PASS] = log_enum::source_type_achieve_dungeon_total_pass;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_RANK_TYPE_LEVEL] = log_enum::source_type_achieve_rank_type_level;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_TASK_FINISH_EXP_CIRCLE_NUMBER] = log_enum::source_type_achieve_task_finish_exp_circle_number;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_TASK_EXP_CIRCLE_ITEM_QUALITY_NUMBER] = log_enum::source_type_achieve_task_exp_circle_item_quality_number;

	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_HERO_UP_LEVEL_NUMBER] = log_enum::source_type_achieve_hero_up_level_number;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_STRENGTHEN_EQUIP_NUMBER] = log_enum::source_type_achieve_strengthen_equip_number;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_TOWER_NUMBER] = log_enum::source_type_achieve_tower_number;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_PATROL_KILL_MONSTER_GROUP_NUMBER] = log_enum::source_type_achieve_patrol_kill_monster_group_number;
	achieve_log_type_map[proto::common::Achieve_Event_Type::ACHIEVE_OPEN_GENERAL] = log_enum::source_type_achieve_open_general;
}

void achieve_manager_t::load_all_data(uint64_t uid,const proto::common::achieve_data& achieve_data)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", uid);
		return;
	}

	for (int32_t i = 0; i < achieve_data.achieve_data_list_size(); ++i)
	{
		achieve_ptr p_achieve = achieve_ptr(new achieve_t());
		p_achieve->load_data(uid,achieve_data.achieve_data_list(i));
		add_achieve(uid, p_achieve,true);
	}
}

void achieve_manager_t::peek_all_data(uint64_t uid, proto::common::achieve_data* p_achieve_data,bool is_client)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", uid);
		return;
	}
	const std::map<uint32_t, achieve_ptr>& achieve_map = p_role->get_achieve_map();
	if (achieve_map.empty())
	{
		//玩家可能会没有成就
		log_warn("achieve_map.empty() role_uid[%lu]", uid);
		return;
	}

	if (NULL == p_achieve_data)
	{
		log_error("NULL == p_achieve_data role_uid[%lu]",uid);
		return;
	}

	for (auto& it : achieve_map)
	{
		achieve_ptr p_achieve = it.second;
		if (NULL == p_achieve)
		{
			log_error("NULL == p_achieve role_uid[%lu] achieve_id[%d]", uid, it.first);
			continue;
		}
		p_achieve->peek_data(p_achieve_data->add_achieve_data_list(), is_client);
	}
}

//void achieve_manager_t::save_all_self(uint64_t uid)
//{
//	role_ptr p_role = role_manager_t::find_role(uid);
//	if (NULL == p_role)
//	{
//		log_error("NULL == p_role role_uid[%lu]", uid);
//		return;
//	}
//	const std::map<uint32_t, achieve_ptr>& achieve_map = p_role->get_achieve_map();
//	if (achieve_map.empty())
//	{
//		return;
//	}
//	for (auto& it : achieve_map)
//	{
//		achieve_ptr p_achieve = it.second;
//		if (NULL == p_achieve)
//		{
//			log_error("NULL == p_achieve role_uid[%lu] achieve_id[%d]",uid,it.first);
//			continue;
//		}
//		p_achieve->save_self(uid);
//	}
//}

void achieve_manager_t::save_all_self(uint64_t uid)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", uid);
		return;
	}
	proto::common::achieve_data data;
	peek_all_data(uid, &data, false);

	if (0 != data.achieve_data_list_size())
	{
		std::string key = cache_key_t::create(uid, cache_name::achieve);
		role_unify_save::add_task(uid, key, data);
	}

}

uint64_t achieve_manager_t::gen_achieve_uid()
{
	return common::g_id_allocator.gen_new_id(common::ID_ALLOCATOR_TYPE_ACHIEVE);
}

achieve_ptr achieve_manager_t::get_achieve(uint64_t uid, uint32_t achieve_id)
{
	if (0 == achieve_id)
	{
		log_error("0 == achieve_id role_uid[%lu] achieve_id[%d]", uid, achieve_id);
		return achieve_ptr();
	}

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu] ", uid);
		return achieve_ptr();
	}
	std::map<uint32_t, achieve_ptr>& achieve_map = p_role->get_achieve_map();
	auto iter_achieve = achieve_map.find(achieve_id);
	if (iter_achieve != achieve_map.end())
		return iter_achieve->second;

	return achieve_ptr();
}


bool achieve_manager_t::add_achieve(uint64_t uid,achieve_ptr p_achieve,bool is_load /* = false */)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", uid);
		return false;
	}

	if (NULL == p_achieve)
	{
		log_error("NULL == p_achieve role_uid[%lu]", uid);
		return false;
	}

	std::map<uint32_t, achieve_ptr>& achieve_map = p_role->get_achieve_map();

	achieve_map.insert(std::make_pair(p_achieve->get_tid(), p_achieve));
	//p_achieve->save_self(uid, true);
	
	if (!is_load)
	{
		///mysql采用了blob存储结构 整个成就一起存储
		achieve_manager_t::save_all_self(uid);
	}
	return true;
}

bool achieve_manager_t::add_achieve(const uint64_t uid,const uint32_t achieve_id, bool is_load /* = false */)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", uid);
		return false;
	}

	if (0 == achieve_id)
	{
		log_error("0 == achieve_id role_uid[%lu] achieve_id[%d]!", uid, achieve_id);
		return false;
	}
	
	const std::map<uint32_t, achieve_ptr>& achieve_map = p_role->get_achieve_map();
	auto iter_achieve = achieve_map.find(achieve_id);
	if (iter_achieve == achieve_map.end())
	{
		uint64_t achieve_uid = achieve_manager_t::gen_achieve_uid();
		achieve_ptr p_achieve = achieve_ptr(new achieve_t(achieve_uid, achieve_id, uid));
		if (!achieve_manager_t::add_achieve(uid, p_achieve,is_load))
		{
			log_error("role[%lu] add achieve[%d] to map list fail!", uid, achieve_id);
			return false;
		}
	}
	return true;
}

void achieve_manager_t::notify_client_achieve_finish(uint64_t uid,uint32_t achieve_id,proto::client::gc_achieve_finish_notify& notify)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", uid);
		return;
	}

	if (0 == achieve_id)
	{
		log_error("0 == achieve_id role_uid[%lu] achieve_id[%d]", uid,achieve_id);
		return;
	}

	achieve_ptr p_achieve = get_achieve(uid, achieve_id);
	if (NULL == p_achieve)
	{
		log_error("NULL == p_achieve role_uid[%lu] achieve_id[%d]",uid, achieve_id);
		return;
	}
	
	ACHIEVE_LOG("role[%lu] begin notify client achieve[%d] finishi", uid, achieve_id);
	notify.set_tid(p_achieve->get_tid());
	notify.set_level(p_achieve->get_level());
	const proto::common::role_change_data& rcd = notify.rcd();
	if (0 == rcd.per_info().achieve_level())
	{
		proto::common::role_change_data* p_data = notify.mutable_rcd();
		proto::common::personal_info* p_personal = p_data->mutable_per_info();
		uint32_t achieve_point = money_manager_t::get_money(p_role, proto::common::MONEY_TYPE_ACHIEVE_POINT);
		p_personal->set_achieve_point(achieve_point);
		p_personal->set_achieve_level(p_role->get_achieve_level());
		p_personal->set_achieve_recv_reward_level(p_role->get_achieve_recv_reward_level());
	}

	p_role->send_msg_to_client(op_cmd::gc_achieve_finish_notify, notify);
}

bool achieve_manager_t::get_achieve_info_list(uint64_t uid)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", uid);
		return false;
	}

	proto::client::gc_get_achieve_infolist_reply reply;
	peek_all_data(uid,reply.mutable_achieve_list());
	p_role->send_msg_to_client(op_cmd::gc_get_achieve_infolist_reply, reply);
	return true;
}

/*
 *人物成就相关
 */
////////////////////////////////////////////////////////////////////////////////

bool achieve_manager_t::add_achieve_level(uint64_t uid)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", uid);
		return false;
	}
	ACHIEVE_LOG("role[%lu] begin up achieve level", uid);
	uint32_t old_level = p_role->get_achieve_level();
	//纠正措施 防止成就等级初始化为1失败
	if (0 == old_level)
	{
		++old_level;
		p_role->set_achieve_level(old_level);
	}
	uint32_t level = p_role->get_achieve_level();
	++level;
	ACHIEVE_LOG("role_uid[%lu] add achieve level old_level[%d] new_level[%d]", uid, old_level, level);
	p_role->set_achieve_level(level);
	red_point_manager_t::red_point_notify(uid, proto::common::RED_POINT_TYPE::RED_POINT_ACHIEVE_LEVEL_REWARD, level);
	return true;
}



bool achieve_manager_t::is_up_achieve_level(uint64_t uid)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", uid);
		return false;
	}

	uint32_t achieve_level = p_role->get_achieve_level();
	if (0 == achieve_level)
		++achieve_level;
	Achievelevel* p_config_level = GET_CONF(Achievelevel, ++achieve_level);
	if (NULL == p_config_level)
	{
		log_error("NULL == p_config_level role_uid[%lu] achieve_level[%d] ", uid,achieve_level);
		return false;
	}
	const uint32_t achieve_point = money_manager_t::get_money(p_role, proto::common::MONEY_TYPE_ACHIEVE_POINT);
	const uint32_t level_up_point = p_config_level->level_up_point();
	if (achieve_point >= level_up_point)
	{
		ACHIEVE_LOG("role[%lu] can up achieve level have achieve_point[%d] need level_up_point[%d]", uid,achieve_point, level_up_point);
		return true;
	}
	ACHIEVE_LOG("role[%lu] not can up achieve level have achieve_point[%d] need level_up_point[%d]", uid, achieve_point, level_up_point);
	return false;
}

void achieve_manager_t::add_level(uint64_t uid)
{
	while (true)
	{
		if (!is_up_achieve_level(uid))
		{
			return;
		}

		add_achieve_level(uid);
	}
}

void achieve_manager_t::notify_achieve_state(uint64_t uid, proto::common::Achieve_Event_Type type,uint32_t param1 ,uint32_t param2)
{
	achieve_msg_handle_t::handle_event_msg(uid, type, param1, param2);
}

uint32_t achieve_manager_t::get_achieve_reward(uint64_t uid,uint32_t type, uint32_t tid,uint32_t level, proto::common::role_change_data* rcd)
{
	
	uint32_t reply_code;
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", uid);
		reply_code = errcode_enum::notice_role_null;
		return reply_code;
	}
	const uint32_t achieve_level = p_role->get_achieve_level();
	uint32_t achieve_reward_level = p_role->get_achieve_recv_reward_level();
	uint32_t old_achieve_reward_level = achieve_reward_level;
	ACHIEVE_LOG("role[%lu] begin get number[%d] level achieve reward", uid,achieve_reward_level + 2);
	do 
	{
		if (achieve_level == achieve_reward_level)
		{
			log_error("role[%lu] already get achieve_level[%d] reward cur achieve_reward_level[%d]", uid, achieve_level, achieve_reward_level);
			break;
		}

		auto p_config_level = GET_CONF(Achievelevel, achieve_reward_level + 2);
		if (NULL == p_config_level)
		{
			log_error("NULL == p_config_level role[%lu] achieve_level[%d]",uid , achieve_level);
			break;
		}

		if (!drop_manager_t::check_bag_num(p_role, p_config_level->drop_id()))
		{
			ACHIEVE_LOG("role_uid[%lu] get acheive_level[%d] achieve reward fail because bag fill drop_id[%d]", uid, achieve_reward_level, p_config_level->drop_id());
			reply_code = errcode_enum::task_bag_full_mail;
		}

		bool b_result = drop_manager_t::drop(p_role, p_config_level->drop_id(), log_enum::source_type_achieve_person_reward,
			proto::common::drop_sys_type_achieve, achieve_level, rcd);

		if (!b_result)
		{
			log_error("role_uid[%lu] get achieve_level[%d] achieve reward drop fail drop_id[%d]", uid, achieve_reward_level, p_config_level->drop_id());
			break;
		}
		ACHIEVE_LOG("role[%lu] get number[%d] level achieve reward success drop_id[%d]", uid,achieve_reward_level + 2, p_config_level->drop_id());

		p_role->set_achieve_recv_reward_level(++achieve_reward_level);
		const uint32_t achieve_point = money_manager_t::get_money(p_role, proto::common::MONEY_TYPE_ACHIEVE_POINT);
		log_wrapper_t::send_achieve_log(uid,0, 0, 0, 0, 0, 0, achieve_point, 0, achieve_point, achieve_level,0, achieve_level,
			old_achieve_reward_level, achieve_reward_level,log_enum::source_type_achieve_person_reward,p_config_level->drop_id(),"");

		ACHIEVE_LOG("role_uid[%lu] achieve_reward_level set[%d]", uid, achieve_reward_level);
		reply_code = errcode_enum::error_ok;
	} while (0);

	//刷新成就基础数值
	if (NULL != rcd)
	{
		proto::common::personal_info* p_personal = rcd->mutable_per_info();
		p_personal->set_achieve_level(p_role->get_achieve_level());
		p_personal->set_achieve_point(money_manager_t::get_money(p_role, proto::common::MONEY_TYPE_ACHIEVE_POINT));
		p_personal->set_achieve_recv_reward_level(p_role->get_achieve_recv_reward_level());	
	}

	return reply_code;
}

void achieve_manager_t::get_person_achieve_record_list(uint64_t uid)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", uid);
		return;
	}
	ACHIEVE_LOG("role[%lu] begin get achieve record", uid);
	proto::client::gc_get_achieve_record_reply reply;
	std::string achieve_cup_number_list, achieve_type_number_list;//成就奖杯数列表 成就完成类别数列表
	achieve_manager_t::get_cup_and_type_number_list(uid, achieve_cup_number_list, achieve_type_number_list);
	reply.set_achieve_cup_list(achieve_cup_number_list);
	reply.set_achieve_type_number_list(achieve_type_number_list);

	ACHIEVE_LOG("role[%lu] have achieve_cup_number_list[%s] achieve_type_number_list[%s]", uid,achieve_cup_number_list.c_str(), achieve_type_number_list.c_str());

	std::map<uint32_t, Achieverecord*> confs;
	GET_ALL_CONF(Achieverecord, confs);
	for (auto& it : confs)
	{
		Achieverecord* p_config = it.second;
		if (NULL == p_config)
		{
			log_error("NULL == p_config role_uid[%lu] Achieverecord table config id[%d]", uid, it.first);
			continue;
		}
		proto::common::achieve_record* p_achieve_record = reply.mutable_achieve_record();
		proto::common::achieve_record_single* p_achieve_record_single = p_achieve_record->add_achieve_record_list();

		switch (p_config->get_id())
		{
		case PERSON_RECORD_TYPE::ACHIEVE_PERSON_REOCRD_CREATETIME:
		{
			//创角时间
			std::string str_create_time = string_util_t::uint32_to_string(p_role->get_create_time());
			p_achieve_record_single->set_tid(it.first);
			p_achieve_record_single->set_achieve_person_record(str_create_time);
			ACHIEVE_LOG("role_uid[%lu] Achieverecord config id[%d] str_create_time[%s]",uid,it.first,str_create_time.c_str());
		}break;
		case PERSON_RECORD_TYPE::ACHIEVE_PERSON_REOCRD_FIGHT:
		{
			//主角战斗力
			std::string str_fight = string_util_t::uint32_to_string(p_role->get_fighting());
			p_achieve_record_single->set_tid(it.first);
			p_achieve_record_single->set_achieve_person_record(str_fight);
			ACHIEVE_LOG("role_uid[%lu] tid[%d] str_fight[%s]", uid, it.first, str_fight.c_str());
		}break;
		case PERSON_RECORD_TYPE::ACHIEVE_PERSON_REOCRD_HERO_NUMBER:
		{
			//武将数量
			std::vector<hero_ptr> hero_list;
			hero_manager_t::get_all_heroes(p_role, hero_list);
			std::string str_hero_num = string_util_t::uint32_to_string(hero_list.size());
			p_achieve_record_single->set_tid(it.first);
			p_achieve_record_single->set_achieve_person_record(str_hero_num);
			ACHIEVE_LOG("role_uid[%lu] tid[%d] str_hero_num[%s]", uid, it.first, str_hero_num.c_str());
		}break;
		case PERSON_RECORD_TYPE::ACHIEVE_PERSON_REOCRD_HERO_FINGHT_MAX:
		{
			//获取战斗力最大的武将的信息
			uint32_t max_fight = 0;
			uint32_t max_fight_hero_index = 0;
			hero_ptr p_hero = nullptr;
			std::vector<hero_ptr> hero_list;
			hero_manager_t::get_all_heroes(p_role, hero_list);
			for (uint32_t i = 0; i < hero_list.size(); ++i)
			{
				auto p_temp_hero = hero_list[i];
				if (NULL == p_temp_hero)
					continue;
			
				const uint32_t hero_fight = p_temp_hero->get_fighting();
				if (hero_fight > max_fight)
				{
					max_fight_hero_index = it.first;
					max_fight = hero_fight;
					p_hero = p_temp_hero;
				}
			}
			if (NULL == p_hero)
			{
				log_error("NULL == p_hero role_uid[%lu] hero_id[%d]", uid,max_fight_hero_index);
				continue;
			}
			std::string str_fight = string_util_t::uint32_to_string(max_fight);
			 std::string str_star = string_util_t::uint32_to_string(p_hero->get_star());
			 std::string str_grade = string_util_t::uint32_to_string(p_hero->get_grade());
			 std::string str_level = string_util_t::uint32_to_string(p_hero->get_current_level());
			 std::string str_hero_id = string_util_t::uint32_to_string(p_hero->get_tid());
			 std::string str_wakeup = string_util_t::uint32_to_string(p_hero->get_wakeup());
			 p_achieve_record_single->set_tid(it.first);
			 std::string str_hero_info_list = str_hero_id + ":" + str_fight + ":" + str_level + ":" + str_star + ":" + str_grade + ":" + str_wakeup;
			 p_achieve_record_single->set_achieve_person_record(str_hero_info_list);
			 ACHIEVE_LOG("role_uid[%lu] tid[%d] str_hero_info_list[%s]", uid, it.first, str_hero_info_list.c_str());
		}break;
		case PERSON_RECORD_TYPE::ACHIEVE_PERSON_REOCRD_PROFESSION:
		{
			//主角ID client 处理门派
			std::string str_main_hero_id = string_util_t::uint32_to_string(p_role->get_main_hero_tid());
			p_achieve_record_single->set_tid(it.first);
			p_achieve_record_single->set_achieve_person_record(str_main_hero_id);
			ACHIEVE_LOG("role_uid[%lu] Achieverecord config id[%d] str_main_hero_id[%s]", uid, it.first, str_main_hero_id.c_str());
		}break;
		default:
			break;
		}
	}

	p_role->send_msg_to_client(op_cmd::gc_get_achieve_record_reply, reply);
}


bool achieve_manager_t::achieve_up_last_level(const proto::common::Achieve_Event_Type achieve_event_type,const uint32_t cur_count ,
									std::deque<uint32_t>& drop_id_deque,uint32_t& last_level, uint32_t event_id)
{
	if (0 == cur_count)
		return false;
	std::map<uint32_t, Achievelist*> confs;
	GET_ALL_CONF(Achievelist, confs);
	if (confs.empty())
	{
		log_error("confs.empty() achieve_event_type[%d] event_id[%d]", achieve_event_type, event_id);
		return false;
	}

	for (auto& it : confs)
	{
		Achievelist* p_config = it.second;
		if (NULL == p_config)
		{
			log_error("achieve_event_type[%d] achieve id[%d] event_id[%d]", achieve_event_type, it.first, event_id );
			continue;
		}
		const Dawn::triple<uint32_t, uint32_t, uint32_t>& tuple_list = p_config->condition();
		if (tuple_list.first != achieve_event_type)
			continue;
		if (tuple_list.second != event_id)
			continue;
		
		if (cur_count < tuple_list.third)
			continue;

		if (last_level < p_config->level())
		{
			last_level = p_config->level();
			drop_id_deque.push_back(p_config->drop_id());
		}
	}

	return true;
}

uint32_t achieve_manager_t::get_finish_target_count(uint32_t achieve_id)
{
	Achievelist* p_config = GET_CONF(Achievelist, achieve_id);
	if (NULL == p_config)
	{
		ACHIEVE_LOG("NULL == p_config achieve_id[%d]",achieve_id);
		return 0;
	}
	return p_config->condition().third;
}


bool achieve_manager_t::get_achieve_id(uint32_t achieve_event_type,uint32_t& achieve_id, uint32_t event_id)
{
	std::map<uint32_t, Achievelist*> confs;
	GET_ALL_CONF(Achievelist, confs);

	auto MIN_ID_CONF = GET_CONF(Comprehensive, comprehensive_common::ACHIEVE_CONFIG_MIN_ID);
	const uint32_t min_level = GET_COMPREHENSIVE_VALUE_1(MIN_ID_CONF);

	for (auto& it : confs)
	{
		Achievelist* p_config = it.second;
		if (NULL == p_config)
		{
			log_error("NULL == p_config achieve_event_type[%d] achieve_id[%d] event_id[%d]",achieve_event_type,it.first,event_id);
			continue;
		}
		const Dawn::triple<uint32_t, uint32_t, uint32_t>& tuple_list = p_config->condition();

		if (tuple_list.first != achieve_event_type)

			continue;
		if (tuple_list.second != event_id)
			continue;

		achieve_id = std::abs(int(it.first - it.first % min_level));	//取以1000为单位的整数

		return is_legal_achieve_id(achieve_id);
		
		ACHIEVE_LOG("achieve_event_type[%d] achieve_id[%d] event_id[%d]", achieve_event_type, achieve_id, event_id);
	}
	ACHIEVE_LOG("find achieve_event_type[%d] event_id[%d] error !", achieve_event_type, event_id);
	return is_legal_achieve_id(achieve_id);
}


void achieve_manager_t::get_cup_and_type_number_list(uint64_t uid,std::string& achieve_cup_list, std::string& achieve_type_list)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", uid);
		return;
	}

	uint32_t copper = 0, sliver = 0, gold = 0;
	const std::map<uint32_t, achieve_ptr>& achieve_map = p_role->get_achieve_map();
	if (achieve_map.empty())
	{
		//因 没有成就时 客户端仍然显示"0:0:0" 而加的 
		achieve_cup_list.append(string_util_t::uint32_to_string(copper).c_str());
		achieve_cup_list.append(":");
		achieve_cup_list.append(string_util_t::uint32_to_string(sliver).c_str());
		achieve_cup_list.append(":");
		achieve_cup_list.append(string_util_t::uint32_to_string(gold).c_str());
		return;
	}
	
	for (auto& it : achieve_map)
	{
		achieve_ptr p_achieve = it.second;
		if(NULL == p_achieve)
			continue;
		switch (p_achieve->get_level())
		{
		case ACHIEVE_LEVEL_TYPE::ACHIEVE_LEVEL_0:
		{
			continue;
		}
		break;
		case ACHIEVE_LEVEL_TYPE::ACHIEVE_LEVEL_1:
		{
			++copper;
		}
		break;
		case ACHIEVE_LEVEL_TYPE::ACHIEVE_LEVEL_2:
		{
			++copper;
			++sliver;
		}
		break;
		default:
			++copper;
			++sliver;
			++gold;
		}
	}

	ACHIEVE_LOG("role[%lu] have copper_cup_num[%d] sliver_cup_num[%d] gold_cup_num[%d]",uid ,copper, sliver, gold);
	achieve_cup_list.append(string_util_t::uint32_to_string(copper).c_str());
	achieve_cup_list.append(":");
	achieve_cup_list.append(string_util_t::uint32_to_string(sliver).c_str());
	achieve_cup_list.append(":");
	achieve_cup_list.append(string_util_t::uint32_to_string(gold).c_str());

	std::map<uint32_t, uint32_t> finish_number_map;
	for (auto& it : achieve_map)
	{
		achieve_ptr p_achieve = it.second;
		if (NULL == p_achieve)
			continue;
		uint32_t cur_level = p_achieve->get_level();
		if (0 == cur_level)
			continue;

        for (uint32_t i = cur_level; i > 0; --i)
        {
            Achievelist* p_config = GET_CONF(Achievelist, it.first + i);
            if (NULL == p_config)
            {
                log_error("Achievelist not find id[%d]", it.first + i);
                break;
            }

            uint32_t num = 0;
            auto iter_map = finish_number_map.find(p_config->type());
            if (iter_map == finish_number_map.end())
            {
                finish_number_map.insert(std::make_pair(p_config->type(), ++num));
            }
            else
            {
                num = iter_map->second;
                finish_number_map[iter_map->first] = ++num;
            }
        }
	}

	for (auto& it : finish_number_map)
	{
		achieve_type_list.append(string_util_t::uint32_to_string(it.first).c_str());
		achieve_type_list.append(":");
		achieve_type_list.append(string_util_t::uint32_to_string(it.second).c_str());
		achieve_type_list.append("|");
	}

	//去掉最后的"|"
	if (!achieve_type_list.empty())
	{
		std::string::size_type _pos = achieve_type_list.find_last_of('|');
		if (_pos != std::string::npos)
		{
			achieve_type_list = achieve_type_list.substr(0, _pos);
		}
	}
	ACHIEVE_LOG("role[%lu] achieve_type_list[%s] achieve_cup_list[%s]",uid, achieve_type_list.c_str(), achieve_cup_list.c_str());
}


bool achieve_manager_t::get_hero_achieve_total(const uint64_t role_uid,const proto::common::Achieve_Event_Type type,const uint32_t event_id,uint32_t& total)
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

bool achieve_manager_t::is_legal_achieve_id(const uint32_t achieve_id)
{
	auto MIN_ID_CONF = GET_CONF(Comprehensive, comprehensive_common::ACHIEVE_CONFIG_MIN_ID);
	auto MAX_ID_CONF = GET_CONF(Comprehensive, comprehensive_common::ACHIEVE_CONFIG_MAX_ID);

	uint32_t min_id = GET_COMPREHENSIVE_VALUE_1(MIN_ID_CONF);
	uint32_t max_id = GET_COMPREHENSIVE_VALUE_1(MAX_ID_CONF);
	return  (achieve_id >= min_id && achieve_id <= max_id && 0 == (achieve_id % min_id));
}

void achieve_manager_t::get_source_type_str(std::string& str, uint32_t tid)
{
	str.clear();
	Achievelist* p_config = GET_CONF(Achievelist, tid);
	if (NULL == p_config)
	{
		log_error("NULL == p_config achieve_id[%d]", tid);
		return;
	}
	str.assign(common::string_util_t::convert_to_utf8(p_config->des()));
}

uint32_t achieve_manager_t::get_achieve_log_type(uint32_t type)
{
	auto iter = achieve_log_type_map.find(type);
	if (iter != achieve_log_type_map.end())
	{
		return iter->second;
	}
	log_error("not find achieve[%d] log type!", type);
	return 0;
}

bool achieve_manager_t::push_to_delay_list(uint64_t role_uid,proto::common::Achieve_Event_Type type, uint32_t event_id ,uint32_t num )
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", role_uid);
		return false;
	}
	ACHIEVE_LOG("role[%lu] add [%d:%d] achieve finishi notify to delay list", role_uid, type, num);
	achieve_struct achieve;
	achieve.type = type;
	achieve.event_id = event_id;
	achieve.count = num;

	auto iter_list = m_all_role_delay_notify_list.find(role_uid);
	if (iter_list != m_all_role_delay_notify_list.end())
	{
		std::deque<achieve_struct>& list = iter_list->second;
		list.push_back(achieve);
	}
	else
	{
		std::deque<achieve_struct> deq_list;
		deq_list.push_back(achieve);
		m_all_role_delay_notify_list.insert(std::make_pair(role_uid, deq_list));
	}
	return true;
}

void achieve_manager_t::check_send_delay_list(uint64_t role_uid,bool is_new_role)
{
	//不是新玩家不用检测
	if (!is_new_role)
	{
		return;
	}
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", role_uid);
		return ;
	}

	auto iter_list = m_all_role_delay_notify_list.find(role_uid);
	if (iter_list == m_all_role_delay_notify_list.end())
	{
		ACHIEVE_LOG("role[%lu] from delay list check send achieve notify fail", role_uid);
		return;
	}

	std::deque<achieve_struct>& list = iter_list->second;
	for (auto& it : list)
	{
		achieve_common_t::notify_progress_state(role_uid, it.type, it.event_id,it.count);
	}
	ACHIEVE_LOG("role[%lu] achieve delay send notify finishi",role_uid);
	m_all_role_delay_notify_list.erase(role_uid);
}
