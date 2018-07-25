#include "tower_manager.hpp"
#include "tower.hpp"
#include "log/log_wrapper.hpp"
#include "item/item_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "config/config_manager.hpp"
#include "role/money_manager.hpp"
#include "tblh/Tower.tbls.h"
#include "tblh/TowerAchieve.tbls.h"
#include "tblh/TowerTrigger.tbls.h"
#include "tblh/errcode_enum.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "fight/fight_manager.hpp"
#include "item/drop_manager.hpp"
#include "rank/rank_manager.hpp"
#include "tblh/rank_common.hpp"
#include "achieve/achieve_common.hpp"

USING_NS_COMMON;

uint32_t tower_manager_t::fight(role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	tower_ptr p_tower = p_role->get_tower();
	if (NULL == p_tower)
	{
		log_error("NULL == p_tower");
		return errcode_enum::notice_role_null;
	}
	uint32_t current_tower = p_tower->get_current_tower();
	auto p_tower_conf = GET_CONF(Tower, current_tower);
	if (NULL == p_tower_conf)
	{
		log_error("NULL == p_tower_conf[%d]", current_tower);
		return errcode_enum::notice_unknown;
	}
	uint32_t against_id = p_tower_conf->against_id();
	proto::common::fight_param fp;
	fp.set_type(proto::common::fight_type_tower);
	fp.set_trigger_id(0);
	fight_manager_t::fight_pve(p_role, against_id, &fp);
	TOWER_LOG("role[%lu] fight tower[%d]", p_role->get_uid(), current_tower);
	return errcode_enum::error_ok;
}

void tower_manager_t::fight_result(role_ptr p_role, bool iswin, uint64_t trigger_uid, proto::common::role_change_data* rcd)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}

	if (rcd == NULL)
	{
		log_error("role[%lu] NULL == rcd", p_role->get_uid());
		return;
	}
	tower_ptr p_tower = p_role->get_tower();
	if (NULL == p_tower)
	{
		log_error("NULL == p_tower");
		return;
	}
	if (p_tower->get_auto_fight_time() > 0)
	{
		log_error("auto_fighting!!!");
		return;
	}
	TOWER_LOG("role[%lu] fight_result trigger_uid[%lu]", p_role->get_uid(), trigger_uid);
	if (0 == trigger_uid)
	{
		//普通关卡
		if (!iswin)
		{
			return;
		}
		uint32_t current_tower = p_tower->get_current_tower();
		TOWER_LOG("role[%lu] fight_win current_tower[%d]", p_role->get_uid(), current_tower);
		auto p_tower_conf = GET_CONF(Tower, current_tower);
		if (NULL == p_tower_conf)
		{
			log_error("NULL == p_tower_conf[%d]", current_tower);
			return;
		}
		proto::client::gc_tower_fight_reply reply;
		p_role->get_personal_info(rcd->mutable_per_info());
		//过关奖励

		if (p_tower_conf->awards() != 0)
		{
			if (!drop_manager_t::drop(p_role, p_tower_conf->awards(), log_enum::source_type_tower_fight,
				proto::common::drop_sys_type_pve, current_tower, rcd))
			{
				log_error("role[%lu] tower[%u] drop error", p_role->get_uid(), current_tower);
				return;
			}
		}
		uint32_t record_tower = p_tower->get_record_tower();
		if (record_tower < current_tower)
		{
			//首通
			if (0 == record_tower)
			{
				//第一关首通
				uint32_t first_day = time_util_t::get_today();
				p_tower->set_first_day(first_day);
				TOWER_LOG("role[%lu] first_day[%d]", p_role->get_uid(), first_day);
			}
			p_tower->set_record_tower(current_tower);
			TOWER_LOG("role[%lu] record_tower[%d]", p_role->get_uid(), current_tower);
			//排行榜
			rank_manager_t::update_tower(p_role->get_uid(), current_tower, time_util_t::get_today() - p_tower->get_first_day(), p_role->get_level());

			if (p_tower_conf->first_awards() != 0)
			{
				if (!drop_manager_t::drop(p_role, p_tower_conf->first_awards(), log_enum::source_type_tower_fight,
					proto::common::drop_sys_type_pve, current_tower, rcd))
				{
					log_error("role[%lu] tower[%u] first drop error", p_role->get_uid(), current_tower);
					return;
				}
			}
		}
		p_tower->trigger(current_tower);
		p_tower->set_current_tower(current_tower + 1);
		p_tower->save_self();
		p_tower->peek_data(reply.mutable_tower_info());
		reply.set_reply_code(errcode_enum::error_ok);
		p_role->send_msg_to_client(op_cmd::gc_tower_fight_reply, reply);
		achieve_common_t::notify_progress_state(p_role->get_uid(), proto::common::ACHIEVE_TOWER_LEVEL, 0, current_tower);
		achieve_common_t::notify_progress_state(p_role->get_uid(), proto::common::ACHIEVE_TOWER_NUMBER);
	}
	else
	{
		//隐藏关卡
		if (iswin)
		{
			TOWER_LOG("role[%lu] fight_win trigger[%lu]", p_role->get_uid(), trigger_uid);
			tower_trigger_ptr p_trigger = p_tower->get_trigger(trigger_uid);
			if (NULL == p_trigger)
			{
				log_error("role[%lu] NULL == p_trigger[%lu]", p_role->get_uid(), trigger_uid);
				return;
			}
			uint32_t trigger_tid = p_trigger->get_tid();
			auto p_trigger_conf = GET_CONF(TowerTrigger, trigger_tid);
			if (NULL == p_trigger_conf)
			{
				log_error("NULL == p_trigger_conf[%d]", trigger_tid);
				return;
			}
			if (p_trigger_conf->awards() != 0)
			{
				if (!drop_manager_t::drop(p_role, p_trigger_conf->awards(), log_enum::source_type_tower_fight,
					proto::common::drop_sys_type_pve, trigger_tid, rcd))
				{
					log_error("role[%lu] trigger[%d] drop error", p_role->get_uid(), trigger_tid);
					return;
				}
			}
			p_trigger->set_expired_time(0);
			TOWER_LOG("role[%lu] set_expired_time(0) trigger[%lu]", p_role->get_uid(), trigger_uid);
		}
		proto::client::gc_tower_trigger_fight_reply reply;
		p_role->get_personal_info(rcd->mutable_per_info());
		p_tower->save_self();
		p_tower->peek_data(reply.mutable_tower_info());
		reply.set_reply_code(errcode_enum::error_ok);
		p_role->send_msg_to_client(op_cmd::gc_tower_trigger_fight_reply, reply);
	}
}

uint32_t tower_manager_t::auto_fight(role_ptr p_role, uint32_t& auto_fight_time)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	tower_ptr p_tower = p_role->get_tower();
	if (NULL == p_tower)
	{
		log_error("NULL == p_tower");
		return errcode_enum::notice_role_null;
	}
	if (p_tower->get_auto_fight_time() > 0)
	{
		log_error("auto_fighting!!!");
		return errcode_enum::notice_unknown;
	}
	uint32_t current_tower = p_tower->get_current_tower();
	uint32_t record_tower = p_tower->get_record_tower();
	if (0 == record_tower)
	{
		log_error("0 == record_tower");
		return errcode_enum::notice_unknown;
	}
	if (record_tower < current_tower)
	{
		log_error("record_tower < current_tower");
		return errcode_enum::notice_unknown;
	}
	uint32_t cost_time = 0;
	for (uint32_t i = current_tower; i <= record_tower; ++i)
	{
		auto p_tower_conf = GET_CONF(Tower, i);
		if (NULL == p_tower_conf)
		{
			log_error("NULL == p_tower_conf[%d]", i);
			return errcode_enum::notice_unknown;
		}
		cost_time += p_tower_conf->sweep_time();
	}
	auto_fight_time = time_util_t::now_time() + cost_time;
	p_tower->set_auto_fight_time(auto_fight_time);
	p_tower->set_auto_fight_timer();
	p_tower->save_self();

	achieve_common_t::notify_progress_state(p_role->get_uid(), proto::common::ACHIEVE_TOWER_QUICK_FIGHT_NUMBER, 0);

	return errcode_enum::error_ok;
}

uint32_t tower_manager_t::auto_fight_immediately(role_ptr p_role, uint32_t& auto_fight_time, proto::common::role_change_data* p_data)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	tower_ptr p_tower = p_role->get_tower();
	if (NULL == p_tower)
	{
		log_error("NULL == p_tower");
		return errcode_enum::notice_role_null;
	}
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return errcode_enum::notice_unknown;
	}
	uint32_t current_tower = p_tower->get_current_tower();
	uint32_t record_tower = p_tower->get_record_tower();
	if (0 == record_tower)
	{
		log_error("0 == record_tower");
		return errcode_enum::notice_unknown;
	}
	if (record_tower < current_tower)
	{
		log_error("record_tower < current_tower");
		return errcode_enum::notice_unknown;
	}
	uint32_t now_time = time_util_t::now_time();
	if (p_tower->get_auto_fight_time() > 0 && now_time >= p_tower->get_auto_fight_time())
	{
		log_error("auto_fighting over!!!");
		return errcode_enum::notice_unknown;
	}
	if (!money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_YUANBAO, get_auto_fight_immediately_gold(), 
		log_enum::source_type_tower_auto_fight_immediately, 0, true, p_data))
	{
		log_error("auto_fight_immediately_gold not enough!!!");
		return errcode_enum::notice_gold_money_not_enough;
	}
	auto_fight_time = now_time;
	p_tower->set_auto_fight_time(auto_fight_time);
	p_tower->del_auto_fight_timer();
	p_tower->save_self();
	return errcode_enum::error_ok;
}

uint32_t tower_manager_t::auto_fight_reward(role_ptr p_role, proto::common::tower_data* p_info, proto::common::role_change_data* p_data)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	tower_ptr p_tower = p_role->get_tower();
	if (NULL == p_tower)
	{
		log_error("NULL == p_tower");
		return errcode_enum::notice_role_null;
	}
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return errcode_enum::notice_unknown;
	}
	if (NULL == p_info)
	{
		log_error("NULL == p_info");
		return errcode_enum::notice_unknown;
	}
	if (0 == p_tower->get_auto_fight_time())
	{
		log_error("not auto_fighting!!!");
		return errcode_enum::notice_unknown;
	}
	uint32_t now_time = time_util_t::now_time();
	if (now_time < p_tower->get_auto_fight_time())
	{
		log_error("auto_fighting not over!!!");
		return errcode_enum::notice_unknown;
	}
	uint32_t current_tower = p_tower->get_current_tower();
	uint32_t record_tower = p_tower->get_record_tower();
	
	for (uint32_t i = current_tower; i <= record_tower; ++i)
	{
		auto p_tower_conf = GET_CONF(Tower, i);
		if (NULL == p_tower_conf)
		{
			log_error("NULL == p_tower_conf[%d]", i);
			return errcode_enum::notice_unknown;
		}

		if (p_tower_conf->awards() != 0)
		{
			if (!drop_manager_t::drop(p_role, p_tower_conf->awards(), log_enum::source_type_tower_auto_fight,
				proto::common::drop_sys_type_pve, i, p_data))
			{
				log_error("role[%lu] tower[%u] drop error", p_role->get_uid(), i);
				return errcode_enum::notice_unknown;
			}
		}
		p_tower->trigger(i);
	}
	//item_manager_t::add_items(p_role, award_item, log_enum::source_type_tower_auto_fight, 0, p_data);
	p_tower->set_current_tower(record_tower + 1);
	p_tower->set_auto_fight_time(0);
	p_tower->save_self();
	p_tower->peek_data(p_info);
	proto::common::personal_info* per_info = p_data->mutable_per_info();
	p_role->get_personal_info(per_info);
	return errcode_enum::error_ok;
}


uint32_t tower_manager_t::reset(role_ptr p_role, uint32_t& current_tower, uint32_t& reset_count)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	tower_ptr p_tower = p_role->get_tower();
	if (NULL == p_tower)
	{
		log_error("NULL == p_tower");
		return errcode_enum::notice_role_null;
	}
	if (p_tower->get_auto_fight_time() > 0)
	{
		log_error("auto_fighting!!!");
		return errcode_enum::notice_unknown;
	}
	current_tower = p_tower->get_current_tower();
	if (1 == current_tower)
	{
		log_error("not need reset!!!");
		return errcode_enum::notice_unknown;
	}
	reset_count = p_tower->get_reset_count();
	if (0 == reset_count)
	{
		log_error("reset_count none!!!");
		return errcode_enum::notice_unknown;
	}
	current_tower = 1;
	reset_count -= 1;
	p_tower->set_current_tower(current_tower);
	p_tower->set_reset_count(reset_count);
	p_tower->save_self();
	return errcode_enum::error_ok;
}


uint32_t tower_manager_t::achieve_reward(role_ptr p_role, uint32_t& current_achieve, proto::common::role_change_data* p_data)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	tower_ptr p_tower = p_role->get_tower();
	if (NULL == p_tower)
	{
		log_error("NULL == p_tower");
		return errcode_enum::notice_role_null;
	}
	current_achieve = p_tower->get_current_achieve();
	auto p_conf = GET_CONF(TowerAchieve, current_achieve);
	if (NULL == p_conf)
	{
		log_error("NULL == p_conf[%d]", current_achieve);
		return errcode_enum::notice_unknown;
	}
	if (p_tower->get_record_tower() < p_conf->floor())
	{
		log_error("record_tower not enough");
		return errcode_enum::notice_unknown;
	}
	drop_manager_t::drop(p_role, p_conf->award(), log_enum::source_type_tower_achieve_reward, proto::common::drop_sys_type_tower, current_achieve, p_data);
	current_achieve += 1;
	p_tower->set_current_achieve(current_achieve);
	p_tower->save_self();
	return errcode_enum::error_ok;
}


uint32_t tower_manager_t::fight_trigger(role_ptr p_role, uint64_t trigger_uid)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	tower_ptr p_tower = p_role->get_tower();
	if (NULL == p_tower)
	{
		log_error("NULL == p_tower");
		return errcode_enum::notice_role_null;
	}
	if (0 == p_tower->get_trigger_fight_count())
	{
		log_error("role[%lu] 0 == p_tower->get_trigger_fight_count", p_role->get_uid());
		return errcode_enum::pagoda_hide_no_times;
	}
	tower_trigger_ptr p_trigger = p_tower->get_trigger(trigger_uid);
	if (NULL == p_trigger)
	{
		log_error("role[%lu] NULL == p_trigger[%lu]", p_role->get_uid(), trigger_uid);
		return errcode_enum::notice_unknown;
	}
	if (0 == p_trigger->get_expired_time())
	{
		log_error("role[%lu] p_trigger[%lu] already fight", p_role->get_uid(), trigger_uid);
		return errcode_enum::notice_unknown;
	}
	if (time_util_t::now_time() >= p_trigger->get_expired_time())
	{
		log_error("role[%lu] p_trigger[%lu] expired", p_role->get_uid(), trigger_uid);
		return errcode_enum::notice_unknown;
	}
	uint32_t trigger_tid = p_trigger->get_tid();
	auto p_trigger_conf = GET_CONF(TowerTrigger, trigger_tid);
	if (NULL == p_trigger_conf)
	{
		log_error("NULL == p_trigger_conf[%d]", trigger_tid);
		return errcode_enum::notice_unknown;
	}
	p_tower->set_trigger_fight_count(p_tower->get_trigger_fight_count() - 1);
	uint32_t against_id = p_trigger_conf->against_id();
	proto::common::fight_param fp;
	fp.set_type(proto::common::fight_type_tower);
	fp.set_trigger_id(trigger_uid);
	fight_manager_t::fight_pve(p_role, against_id, &fp);
	return errcode_enum::error_ok;
}

uint32_t tower_manager_t::get_auto_fight_immediately_gold()
{
	auto gold = GET_CONF(Comprehensive, comprehensive_common::dreamland_sweep_cost_gold);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(gold);
	return val;
}
