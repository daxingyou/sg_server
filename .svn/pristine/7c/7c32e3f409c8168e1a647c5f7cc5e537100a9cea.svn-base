#include "interior_manager.hpp"
#include "interior.hpp"
#include "log/log_wrapper.hpp"
#include "role/role_manager.hpp"
#include "item/item_manager.hpp"
#include "hero/hero_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "scene/scene_manager.hpp"
#include "GarrisonCitylist.pb.h"
#include "task/task_manager.hpp"
#include "Garrison.pb.h"
#include "config/config_manager.hpp"
#include "role/money_manager.hpp"
#include "tblh/Comprehensive.tbls.h"

USING_NS_COMMON;

void interior_manager_t::get_build_copper(uint64_t uid)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL != p_role &&  NULL != p_role->get_interior_info())
	{
		proto::client::gc_get_build_copper_reply reply;

		uint32_t copper = p_role->get_interior_info()->get_copper();
		
		if (copper == 0)
		{
			reply.set_reply_code(errcode_enum::notice_interior_copper_null);
		}
		else
		{
			reply.set_reply_code(0);
			reply.set_copper_count(copper);
			money_manager_t::add_money(p_role, proto::common::MONEY_TYPE_COPPER, copper, log_enum::source_type_interior_building_produce, 0);
		}
		p_role->get_interior_info()->notify_info_to_client();
		p_role->notify_save_data();
		proto::common::role_change_data* p_data = reply.mutable_rcd();
		proto::common::personal_info* p_info = p_data->mutable_per_info();
		p_role->get_personal_info(p_info);
		p_role->send_msg_to_client(op_cmd::gc_get_build_copper_reply, reply);
	}
}


void interior_manager_t::upgrade_build(uint64_t uid)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL != p_role &&  NULL != p_role->get_interior_info())
	{
		proto::client::gc_upgrade_interior_build_reply reply;
		uint32_t cur_level = p_role->get_interior_info()->get_level();
		if (cur_level >= INTERIOR_BUILD_LEVEL_MAX)
		{
			reply.set_reply_code(errcode_enum::notice_upgrade_interior_error);
		}
		else
		{
			BuildConf* p_config = GET_CONFIG_SPECIAL(Build, cur_level, false, BuildConf);
			if (NULL == p_config)
			{
				log_error("NULL == p_config,cur_level[%d]", cur_level);
				return;
			}
			if (errcode_enum::error_ok != item_manager_t::remove_items(p_role, p_config->upgrade_material, log_enum::source_type_interior_upgrade_build_cost_item, 0, reply.mutable_rcd()))
			{
				reply.set_reply_code(0);
				p_role->get_interior_info()->set_level(++cur_level);
				p_role->notify_save_data();
				reply.set_level(cur_level);
			}
			else
			{
				reply.set_reply_code(errcode_enum::notice_item_not_enough);
			}
		}
		p_role->send_msg_to_client(op_cmd::gc_upgrade_interior_build_reply, reply);
	}
}

void interior_manager_t::dispatch_task(uint64_t uid, const proto::client::cg_dispatch_task_request& dtr)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role || NULL == p_role->get_interior_info())
	{
		return;
	}
	proto::client::gc_dispatch_task_reply reply;

	if (0 == dtr.hero_id_list_size())
	{
		return;
	}
	for (uint16_t i = 0; i < dtr.hero_id_list_size(); ++i)
	{
		uint32_t hero_id = dtr.hero_id_list(i);
		if (NULL == hero_manager_t::get_hero(p_role, hero_id))
		{
			return;
		}
		reply.add_hero_id_list(hero_id);
	}
	if (0 == p_role->get_interior_info()->get_left_times())
	{
		reply.set_reply_code(errcode_enum::notice_interior_task_times_is_zero);
		p_role->send_msg_to_client(op_cmd::gc_dispatch_task_reply, reply);
		return;
	}
	if (p_role->get_interior_info()->is_start())
	{
		reply.set_reply_code(errcode_enum::notice_task_not_finish);
		p_role->send_msg_to_client(op_cmd::gc_dispatch_task_reply, reply);
		return;
	}
	for (uint16_t i = 0; i < dtr.hero_id_list_size(); ++i)
	{
		uint32_t hero_id = dtr.hero_id_list(i);
		p_role->get_interior_info()->add_hero(hero_id);
	}
	p_role->get_interior_info()->start_task();
	p_role->notify_save_data();
	p_role->get_interior_info()->notify_info_to_client();

	reply.set_reply_code(0);
	reply.set_task_id(p_role->get_interior_info()->get_task_id());
	reply.set_task_star_level(p_role->get_interior_info()->get_task_star_level());
	p_role->send_msg_to_client(op_cmd::gc_dispatch_task_reply, reply);
}

void interior_manager_t::refresh_task(uint64_t uid)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role || NULL == p_role->get_interior_info())
	{
		return;
	}
	proto::client::gc_refresh_task_reply reply;
	if (p_role->get_interior_info()->is_start())
	{
		reply.set_reply_code(errcode_enum::notice_task_not_finish);
		p_role->send_msg_to_client(op_cmd::gc_refresh_task_reply, reply);
		return;
	}
	if (!money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_SILVER, 2000, log_enum::source_type_interior_refresh_task, 0))
	{
		reply.set_reply_code(errcode_enum::notice_silver_money_not_enough);
		p_role->send_msg_to_client(op_cmd::gc_refresh_task_reply, reply);
		return;
	}
	reply.set_reply_code(0);
	proto::common::role_change_data* p_data = reply.mutable_rcd();
	proto::common::personal_info* p_info = p_data->mutable_per_info();
	p_role->get_personal_info(p_info);
	reply.set_task_id(p_role->get_interior_info()->random_task());
	reply.set_task_star_level(p_role->get_interior_info()->get_task_star_level());
	p_role->send_msg_to_client(op_cmd::gc_refresh_task_reply, reply);
	p_role->notify_save_data();
}

void interior_manager_t::upgrade_task_star_level(uint64_t uid)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role || NULL == p_role->get_interior_info())
	{
		return;
	}
	proto::client::gc_upgrade_task_star_level_reply reply;
	if (!money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_YUANBAO, INTERIOR_TASK_UPGRADE_STAR_COST, log_enum::source_type_interior_upgrade_task_star_level, 0))
	{
		reply.set_reply_code(errcode_enum::notice_gold_money_not_enough);
		p_role->send_msg_to_client(op_cmd::gc_refresh_task_reply, reply);
		return;
	}
	if (p_role->get_interior_info()->upgrade_task_star_level())
	{
		reply.set_reply_code(0);
		reply.set_task_star_level(p_role->get_interior_info()->get_task_star_level());
		p_role->notify_save_data();
		proto::common::role_change_data* p_data = reply.mutable_rcd();
		proto::common::personal_info* p_info = p_data->mutable_per_info();
		p_role->get_personal_info(p_info);
	}
	else
	{
		reply.set_reply_code(errcode_enum::notice_interior_upgrade_task_star);
	}
	p_role->send_msg_to_client(op_cmd::gc_upgrade_task_star_level_reply, reply);
}

void interior_manager_t::get_interior_reward(uint64_t uid)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role || NULL == p_role->get_interior_info())
	{
		return;
	}
	
	proto::client::gc_get_interior_reward_reply reply;
	proto::common::role_change_data* p_data = reply.mutable_rcd();

	if (p_role->get_interior_info()->get_task_reward(p_data))
	{
		reply.set_reply_code(0);
		p_role->notify_save_data();
		p_role->get_interior_info()->notify_info_to_client();
		proto::common::personal_info* p_info = p_data->mutable_per_info();
		p_role->get_personal_info(p_info);
	}
	else
	{
		reply.set_reply_code(1);
	}
	p_role->send_msg_to_client(op_cmd::gc_get_interior_reward_reply, reply);
}

void interior_manager_t::complete_interior_task(uint64_t uid)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role || NULL == p_role->get_interior_info())
	{
		return;
	}

	proto::client::gc_complete_interior_task_reply reply;
	if (!money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_YUANBAO, INTERIOR_TASK_COMPLETE_COST, log_enum::source_type_interior_complete_task, 0))
	{
		reply.set_reply_code(errcode_enum::notice_gold_money_not_enough);
		p_role->send_msg_to_client(op_cmd::gc_complete_interior_task_reply, reply);
		return;
	}
	if (p_role->get_interior_info()->complete_interior_task())
	{
		reply.set_reply_code(0);
		p_role->notify_save_data();
		p_role->get_interior_info()->notify_info_to_client();
		proto::common::role_change_data* p_data = reply.mutable_rcd();
		proto::common::personal_info* p_info = p_data->mutable_per_info();
		p_role->get_personal_info(p_info);
	}
	else
	{
		reply.set_reply_code(errcode_enum::notice_task_not_finish);
	}
	p_role->send_msg_to_client(op_cmd::gc_complete_interior_task_reply, reply);
}


//////////////////////////////////////////////////////////////////////////
uint32_t interior_manager_t::calc_reward_time(uint64_t uid)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		return 0;
	}
	interior_ptr p_interior = p_role->get_interior_info();
	if (NULL == p_interior)
	{
		return 0;
	}
	uint32_t reward_time = 0;
	uint32_t orct = p_interior->get_offline_reward_calc_time();
	if (orct != 0)
	{
		uint32_t now = common::time_util_t::now_time();
		if (now > orct)
		{
			reward_time = now - orct;
			orct = 0;
		}
		if (!scene_manager_t::is_auto_play_map(p_role->get_map_tid()))
		{
			orct = now;
		}
		p_interior->set_offline_reward_calc_time(orct);
	}
	return reward_time;
}

void interior_manager_t::notify_offline_reward_info(uint64_t uid, bool is_instant_battle/* = false*/)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		return;
	}
	interior_ptr p_interior = p_role->get_interior_info();
	if (NULL == p_interior)
	{
		return;
	}
	if (!p_interior->is_first_enter_game())
	{
		if (!scene_manager_t::is_auto_play_map(p_role->get_map_tid()))
		{
			return;
		}
	}
	p_interior->set_is_first_enter_game(false);
	uint32_t reward_calc_time = calc_reward_time(uid);
	if (is_instant_battle)
	{
		auto p_instant_battle_second = GET_CONF(Comprehensive, comprehensive_common::instant_battle_second);
		uint32_t instant_battle_second = GET_COMPREHENSIVE_VALUE_1(p_instant_battle_second);

		// 快速完成 两小时奖励
		reward_calc_time = instant_battle_second;
	}
	do
	{
		auto p_min_offline_reward_second = GET_CONF(Comprehensive, comprehensive_common::min_offline_reward_second);
		uint32_t min_offline_reward_second = GET_COMPREHENSIVE_VALUE_1(p_min_offline_reward_second);
		auto p_max_offline_reward_second = GET_CONF(Comprehensive, comprehensive_common::max_offline_reward_second);
		uint32_t max_offline_reward_second = GET_COMPREHENSIVE_VALUE_1(p_max_offline_reward_second);

		if (reward_calc_time < min_offline_reward_second)
		{
			break;
		}
		if (reward_calc_time > max_offline_reward_second)
		{
			// 最多只有24小时奖励
			reward_calc_time = max_offline_reward_second;
		}
		uint32_t minute = reward_calc_time / 60;
		Hades::Data::GarrisonCitylist* p_citylist_config = GET_CONFIG(GarrisonCitylist, p_interior->get_garrison_city());
		if (nullptr == p_citylist_config)
		{
			log_error("[zqx role] role[%lu] garrison_city[%u] nullptr == p_citylist_config", uid, p_interior->get_garrison_city());
			break;
		}

		proto::client::gc_offline_info_notify ntf;
		proto::common::role_change_data* p_data = ntf.mutable_rcd();
		ntf.set_copper(p_citylist_config->coin() * minute);
		ntf.set_kill_monster_count(minute);
		money_manager_t::add_money(p_role, proto::common::MONEY_TYPE_COPPER, p_citylist_config->coin() * minute, log_enum::source_type_role_offline_reward, 0);

		ntf.set_exp(p_citylist_config->exp() * minute);
		ntf.set_offline_time(reward_calc_time);
		ntf.set_map_id(p_role->get_map_tid());
		p_role->add_exp(p_citylist_config->exp() * minute, log_enum::source_type_role_offline_reward, true, p_data);
		for (auto item_tid : p_citylist_config->item())
		{
			item_ptr p_item = item_manager_t::add_item(p_role, item_tid, minute, log_enum::source_type_role_offline_reward, 0, p_data);
			if (p_item != NULL)
			{
				proto::common::item_single* p_item_single = ntf.add_item_list();
				p_item->peek_data(p_item_single);
				p_item_single->set_num(minute);
			}
		}
		reward_calc_time = reward_calc_time % 60;
		proto::common::personal_info* p_info = p_data->mutable_per_info();
		p_role->get_personal_info(p_info);
		p_role->send_msg_to_client(op_cmd::gc_offline_info_notify, ntf);
	} while (false);

	p_role->save_self(true);
}

void interior_manager_t::update_garrison_city(uint64_t uid)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		return;
	}
	interior_ptr p_interior = p_role->get_interior_info();
	if (NULL == p_interior)
	{
		return;
	}
	uint32_t cur_garrsion_id = p_role->get_task_mgr()->get_current_garrison();
	Hades::Data::Garrison* p_garrison = GET_CONFIG(Garrison, cur_garrsion_id);
	if (nullptr == p_garrison)
	{
		log_error("[zqx role] role[%lu] garrison_city[%u] nullptr == p_garrison", uid, cur_garrsion_id);
		return;
	}

	const scene_ptr& p_scene = scene_manager_t::find_scene(p_role->get_scene_id());
	if (nullptr == p_scene)
	{
		log_error("[zqx role] role[%lu] scene[%lu] nullptr == p_scene", uid, p_role->get_scene_id());
		return;
	}

	uint32_t area_id = p_scene->get_area_id(p_role->get_current_x(), p_role->get_current_y());
	for (int32_t city_tid : p_garrison->clear_city())
	{
		Hades::Data::GarrisonCitylist* p_citylist_config = GET_CONFIG(GarrisonCitylist, city_tid);
		if (nullptr == p_citylist_config)
		{
			continue;
		}
		if (p_citylist_config->area() == area_id)
		{
			p_interior->set_garrison_city(city_tid);
			break;
		}
	}
}

void interior_manager_t::update_offline_reward_info(uint64_t uid)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		return;
	}
	interior_ptr p_interior = p_role->get_interior_info();
	if (NULL == p_interior)
	{
		return;
	}
	p_interior->set_offline_reward_calc_time(common::time_util_t::now_time());
	update_garrison_city(uid);
}