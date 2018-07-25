#include "lifeforce_manager.hpp"
#include "protocpp/client.pb.h"
#include "common/log_enum.hpp"
#include "tblh/LifeStar.tbls.h"
#include "tblh/LifeState.tbls.h"
#include "role/money_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "log/log_wrapper.hpp"
#include "role/money_manager.hpp"
#include "lifeforce_attr_manager.hpp"
#include "role/role.hpp"
#include "role/role_manager.hpp"
#include "common/config_mgr.h"
#include "common/utility.hpp"
#include "task/task.hpp"
#include "tblh/LabelTable.tbls.h"
#include "item/item_manager.hpp"

USING_NS_CONFIG;
USING_NS_COMMON;

void lifeforce_manager_t::level_up_lifestar_level(uint64_t role_uid)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", role_uid);
		return;
	}

	LIFEFORCE_LOG("role[%lu] begin up lifestar level!", role_uid);
	uint32_t recode = errcode_enum::error_ok;
	uint32_t need_lifeforce = 0;	//需要的命力值
	const uint32_t old_lifestar_level = p_role->get_lifestar_level();					//当前的命星等级
	const uint32_t old_lifestate_level = p_role->get_lifestate_level();					//当前的境界等级
	proto::client::gc_activate_life_star_reply reply;
	do 
	{
		//配置表是否正确 命星是获取下一级表数据
		LifeStar* p_config = GET_CONF(LifeStar, hash_util_t::hash_to_uint32(old_lifestate_level, (old_lifestar_level + 1)));
		if (NULL == p_config)
		{
			log_error("NULL == p_lifestar_config role[%lu] lifestate level[%d] lifestar level[%d]", role_uid, old_lifestate_level, old_lifestar_level + 1);
			recode = errcode_enum::notice_lifeforce_config_error;
			break;
		}

		//开发等级
		const uint32_t limit_level = p_config->limit_level();
		if (p_role->get_level() < limit_level)
		{
			LIFEFORCE_LOG("role[%lu] role level not enough level/limit level[%d/%d]", role_uid, p_role->get_level(), limit_level);
			recode = errcode_enum::notice_lifeforce_level_not_enough;
			break;
		}

		//点星所需命力是否足够
		const uint32_t old_lifeforce_value = money_manager_t::get_money(p_role, proto::common::MONEY_TYPE_LIFEFORCE);  //当前的命力值
		need_lifeforce = p_config->need_count();
		if (old_lifeforce_value < need_lifeforce)
		{
			log_error("role[%lu] up level lifestar need lifeforce not enough cur lifeforce/need lifeforce[%d/%d]", role_uid, old_lifeforce_value, need_lifeforce);
			recode = errcode_enum::notice_lifeforce_not_enough;
			break;
		}

		//扣除命力值
		if (!money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_LIFEFORCE, need_lifeforce,
			log_enum::source_type_lifeforce_up_lifestar_level, old_lifestar_level, true))
		{
			log_error("role[%lu] up lifestar level[%d] deduct lifeforce[%d] fail",role_uid,old_lifestar_level,need_lifeforce);
			break;
		}

		//升命星
		if (!p_role->level_up_lifestar())
		{
			log_error("role[%lu] up lifestar level[%d] fail",role_uid,old_lifestar_level);
			recode = errcode_enum::notice_lifeforce_star_level_already_max;
			break;
		}

		//调用奖励接口
		proto::common::role_change_data* p_data = reply.mutable_rcd();
		if (!lifeforce_attr_manager_t::grant_lifeforce_reward(role_uid, log_enum::source_type_lifeforce_up_lifestar_level, p_data))
		{
			log_error("role[%lu] up lifestar level[%d] grant reward fail", role_uid, old_lifestar_level);
			break;
		}

	} while (false);

	
	reply.set_reply_code(recode);
	if (recode == errcode_enum::error_ok)
	{
		const uint32_t new_lifestate_level = p_role->get_lifestate_level();
		const uint32_t new_lifestar_level = p_role->get_lifestar_level();

		//role change data
		proto::common::role_change_data* p_data = reply.mutable_rcd();
		proto::common::personal_info * p_personal = p_data->mutable_per_info();
		p_personal->set_lifeforce(money_manager_t::get_money(p_role, proto::common::MONEY_TYPE_LIFEFORCE));
		p_personal->set_lifestar_level(new_lifestar_level);
		p_personal->set_lifestate_level(new_lifestate_level);

		//记录日志
		log_wrapper_t::send_lifeforce_log(role_uid, old_lifestate_level, old_lifestar_level, 0, 1, new_lifestate_level,
			new_lifestar_level, log_enum::source_type_lifeforce_up_lifestar_level, need_lifeforce);

		LIFEFORCE_LOG("role[%lu] up lifestar from old level[%d] to new level[%d] success!", role_uid, old_lifestar_level, new_lifestar_level);

		if (new_lifestar_level == MAX_LIFESTAR_LEVEL)
		{
			task_mgr_ptr p_task_mgr = p_role->get_task_mgr();
			LifeState* p_cur_level_config = GET_CONF(LifeState, old_lifestate_level); 
			LifeState* p_next_level_config = GET_CONF(LifeState, old_lifestate_level + 1);

			do 
			{
				if (NULL == p_task_mgr)
				{
					log_error("NULL == p_task_mgr role[%lu] add break task fail", role_uid);
					break;
				}

				if (NULL == p_cur_level_config)
				{
					log_error("NULL == p_cur_level_config role[%lu] cur state level[%d] not exist config", role_uid);
					break;
				}

				if (NULL == p_next_level_config)
				{
					LIFEFORCE_LOG("cur state level[%d] not next level config unable up level!", old_lifestate_level);
					break;
				}
				p_task_mgr->active_task_by_id(p_cur_level_config->task_id(), accept_task_type::ACCEPT_TASK_BY_SYSTEM, *p_data);

			} while (false);
		}
	}
	else
	{
		LIFEFORCE_LOG("role[%lu] up lifestar level[%d] fail error code[%d]", role_uid, old_lifestar_level ,recode);
	}

	p_role->save_self(true);
	//回客户端消息
	p_role->send_msg_to_client(op_cmd::gc_activate_life_star_reply, reply);
}

void lifeforce_manager_t::level_up_lifestate_level(uint64_t role_uid, proto::client::gc_break_life_state_reply& reply)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", role_uid);
		return;
	}

	LIFEFORCE_LOG("role[%lu] begin up lifestate level!",role_uid);

	uint32_t recode = errcode_enum::error_ok;
	const uint32_t old_lifestar_level = p_role->get_lifestar_level();					//当前的命星等级
	const uint32_t old_lifestate_level = p_role->get_lifestate_level();					//当前的境界等级
	uint32_t task_id = 0;
	do 
	{
		LifeState* p_config = GET_CONF(LifeState, old_lifestate_level);
		if (NULL == p_config)
		{
			log_error("NULL == p_config role[%lu] lifestate level[%d]", role_uid, old_lifestate_level);
			recode = errcode_enum::notice_lifeforce_config_error;
			break;
		}
		task_id = p_config->task_id();

		//突破
		if (!p_role->level_up_lifestate(recode))
		{
			break;
		}

		//调用奖励接口
		proto::common::role_change_data* p_data = reply.mutable_rcd();
		if (!lifeforce_attr_manager_t::grant_lifeforce_reward(role_uid, log_enum::source_type_lifeforce_up_lifestate_level, p_data))
		{
			log_error("role[%lu] up lifestate level[%d] grant reward fail", role_uid, old_lifestate_level);
			break;
		}

	} while (0);
	
	reply.set_reply_code(recode);
	//是否成功
	if (recode == errcode_enum::error_ok)
	{
		const uint32_t new_lifestate_level = p_role->get_lifestate_level();
		const uint32_t new_lifestar_level = p_role->get_lifestar_level();

		//role change data
		proto::common::role_change_data* p_data = reply.mutable_rcd();
		proto::common::personal_info * p_personal = p_data->mutable_per_info();
		p_personal->set_lifestar_level(new_lifestar_level);
		p_personal->set_lifestate_level(new_lifestate_level);
		p_personal->set_lifeforce(money_manager_t::get_money(p_role, proto::common::MONEY_TYPE_LIFEFORCE));

		//记录日志
		log_wrapper_t::send_lifeforce_log(role_uid, old_lifestate_level, old_lifestar_level, 1, 9, new_lifestate_level,
			new_lifestar_level, log_enum::source_type_lifeforce_up_lifestate_level, task_id);

		LIFEFORCE_LOG("role[%lu] up lifestate from old level[%d] to new level[%d] success!", role_uid, old_lifestate_level, new_lifestate_level);
	}
	else
	{
		LIFEFORCE_LOG("role[%lu] up lifestate level[%d] fail error code[%d]", role_uid, old_lifestate_level, recode);
	}
	p_role->save_self(true);
	p_role->send_msg_to_client(op_cmd::gc_break_life_state_reply, reply);

}

void lifeforce_manager_t::level_up_lifelabel_level(uint64_t role_uid)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", role_uid);
		return;
	}

	LIFEFORCE_LOG("role[%lu] begin up lifelabel level!", role_uid);
	uint32_t recode = errcode_enum::error_ok;
	proto::client::gc_up_level_lifelabel_reply reply;
	const uint32_t cur_label_level = p_role->get_lifeforce_label();
	do 
	{
		LabelTable* p_config = GET_CONF(LabelTable, cur_label_level + 1);
		if (NULL == p_config)
		{
			log_error("NULL == p_config id[%d]",cur_label_level + 1);
			recode = errcode_enum::notice_config_error;
			break;
		}

		const uint32_t state_level = p_role->get_lifestate_level();
		const uint32_t star_level  = p_role->get_lifestar_level();

		const std::map<uint32_t, uint32_t>& level_cond = p_config->up_level_cond();
		const std::map<uint32_t, uint32_t>& item_cond = p_config->up_item_cond();

		bool is_level_cond_ok = true ? level_cond.empty() : false, is_item_cond_ok = true;
		LIFEFORCE_LOG("level_cond size[%d] item_cond size[%d]", level_cond.size(), item_cond.size());
		for (auto& it : level_cond)
		{
			LIFEFORCE_LOG("up level condition state_level[%d] star_level[%d]", it.first, it.second);
			if (it.first < state_level)
			{
				is_level_cond_ok = true;
			}
			else if(it.first == state_level && it.second <= star_level)
			{
				is_level_cond_ok = true;
			}

			break;
		}
		LIFEFORCE_LOG("is_level_cond_ok[%d]", is_level_cond_ok);
		if (!is_level_cond_ok)
		{
			recode = errcode_enum::title_xuemai;
			break;
		}
	
		for (auto& it : item_cond)
		{
			uint32_t item_num = item_manager_t::get_item_count(p_role, it.first);
			if (item_num < it.second)
			{
				is_item_cond_ok = false;
				break;
			}
		}
		LIFEFORCE_LOG("is_item_cond_ok[%d]", is_item_cond_ok);
		if (!is_item_cond_ok)
		{
			recode = errcode_enum::title_item;
			break;
		}
		else
		{
			for (auto& it : item_cond)
			{
				uint32_t result_code = item_manager_t::remove_item(role_uid, it.first, it.second, log_enum::source_type_lifeforce_up_lifelabel_level, cur_label_level + 1,reply.mutable_rcd());
				if (errcode_enum::error_ok != result_code)
				{
					log_error("role[%lu] up lifelabel level  remove item[%d] num[%d] fail result_code[%d]", role_uid, it.first, it.second, result_code);
					recode = result_code;
					break;
				}
			}
		}
	} while (false);
	
	if (recode == errcode_enum::error_ok)
	{
		p_role->level_up_lifelabel();
		///加属性 加战斗力
		lifeforce_attr_manager_t::add_single_attr_reward(role_uid, reply.mutable_rcd());
		p_role->calc_fighting();
	}

	reply.set_reply_code(recode);
	reply.set_label_level(p_role->get_lifeforce_label());
	p_role->send_msg_to_client(op_cmd::gc_up_level_lifelabel_reply, reply);
	
}

bool lifeforce_manager_t::break_task_is_finish(uint64_t role_uid, uint32_t task_id)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", role_uid);
		return false;
	}

	task_mgr_ptr p_task_mgr =  p_role->get_task_mgr();
	if (NULL == p_task_mgr)
	{
		log_error("NULL == p_task_mgr role[%lu]", role_uid);
		return false;
	}

	return p_task_mgr->is_done(task_id);
}

bool lifeforce_manager_t::check_break_task_is_finish(uint64_t role_uid, uint32_t task_id)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", role_uid);
		return false;
	}
	const uint32_t lifestate_level = p_role->get_lifestate_level();					//当前的境界等级
	task_mgr_ptr p_task_mgr = p_role->get_task_mgr();
	if (NULL == p_task_mgr)
	{
		log_error("NULL == p_task_mgr role[%lu]", role_uid);
		return false;
	}

	LifeState* p_config = GET_CONF(LifeState, lifestate_level);
	if (NULL == p_config)
	{
		log_error("NULL == p_config role[%lu] lifestate level[%d]", role_uid, lifestate_level);
		return false;
	}
	if (p_config->finish_task_id() != task_id)
	{
		LIFEFORCE_LOG("role[%lu] target_task_id[%d] finish_task_id[%d] not match",role_uid, p_config->task_id(),task_id);
		return false;
	}
	if (p_task_mgr->is_done(task_id))
	{
		proto::client::gc_break_life_state_reply reply;
		level_up_lifestate_level(role_uid, reply);
		return true;
	}
	else
	{
		LIFEFORCE_LOG("role[%lu] break task[%d] not finish!",role_uid, task_id);
		return false;
	}
		
}
