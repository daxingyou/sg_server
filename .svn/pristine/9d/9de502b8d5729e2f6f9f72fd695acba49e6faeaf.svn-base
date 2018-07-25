#include "active_reward_manager.hpp"
#include <boost/format.hpp>
#include <set>
#include <map>
#include "achieve_common_enum.hpp"
#include "achieve_common.hpp"
#include "role/role.hpp"
#include "role/role_manager.hpp"
#include "main/game_server_fwd.hpp"
#include "common/log_enum.hpp"
#include "tblh/ActiveReward.tbls.h"
#include "item/drop_manager.hpp"
#include "role/money_manager.hpp"
#include "log/log_wrapper.hpp"
#include "tblh/errcode_enum.hpp"
#include "role/role_unify_save.hpp"
#include "cache_key.hpp"

active_reward_manager_t::active_reward_manager_t()
{
}

active_reward_manager_t::~active_reward_manager_t()
{
}

void active_reward_manager_t::achieve_main_role_level(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t param1 /*= 0*/, uint32_t param2 /*= 0*/)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", uid);
		return;
	}
	uint32_t id = 0;
	if (!achieve_common_t::get_config_id(uid,achieve_common_active_reward,type, id))
	{
		ACTIVE_REWARD_LOG("Achieve_Event_Type[%d] id[%d]",type,id);
		return;
	}

	if (!achieve_common_t::add_element(achieve_common_active_reward, uid, id))
	{
		return;
	}
	active_reward_ptr p_active_reward = get_active_reward(uid,id);
	if (NULL == p_active_reward)
	{
		log_error("NULL == p_active_reward role_uid[%lu] active_reward_id[%d] Achieve_Event_Type[%d] event_id[%d]", uid, id, type, 0);
		return;
	}

	if (p_active_reward->get_is_finish())
	{
		ACTIVE_REWARD_LOG("role_uid[%lu] active_reward_id[%d] Achieve_Event_Type[%d] event_id[%d] already finish already finish ", uid, id, type, 0);
		return;
	}
	const uint32_t target = achieve_common_t::get_target_count(achieve_common_active_reward, id);
	const uint32_t old_count = p_active_reward->get_count();
	if (old_count < p_role->get_level())
	{
		if (target < p_role->get_level())
		{
			p_active_reward->set_count(target);
		}
		else
		{
			p_active_reward->set_count(p_role->get_level());
		}
	}
	
	const uint32_t new_count = p_active_reward->get_count();
	const uint32_t add_count = new_count - old_count;
	///p_active_reward->save_self(uid, true);
	save_all_self(uid);
	ACTIVE_REWARD_LOG("role[%lu] add active_reward[%d] addcount[%d] old count/new count[%d/%d]", uid, id, add_count, old_count, new_count);
	
	if (new_count < target)
	{
		///没有完成活跃奖励任务
		return;
	}

	p_active_reward->set_is_finish();
	do
	{
		activity_mgr_ptr p_activity_mgr = p_role->get_activity_mgr();
		if (NULL == p_activity_mgr)
		{
			log_error("role[%lu] NULL == p_activity_mgr", uid);
			break;
		}

		ActiveReward* p_config = GET_CONF(ActiveReward, id);
		if (NULL == p_config)
		{
			log_error("NULL == p_config[%d]", id);
			break;
		}

		p_activity_mgr->add_liveness_count(p_config->liveness());
		p_activity_mgr->save_self();

	} while (false);

	///p_active_reward->save_self(uid, true);
	save_all_self(uid);
	p_role->save_self();
	ACTIVE_REWARD_LOG("finish active_reward role_uid[%lu] Achieve_Event_Type[%d] active_reward_id[%d]", uid, type, id);
}

void active_reward_manager_t::achieve_hero_cultivate(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t event_id, uint32_t num /*= 1*/)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", uid);
		return;
	}
	uint32_t id = 0;
	if (!achieve_common_t::get_config_id(uid,achieve_common_active_reward,type, id, event_id))
	{
		ACTIVE_REWARD_LOG("Achieve_Event_Type[%d] id[%d]", type, id);
		return;
	}

	if (!achieve_common_t::add_element( achieve_common_active_reward,uid, id))
	{
		return;
	}
	active_reward_ptr p_active_reward = get_active_reward(uid, id);
	if (NULL == p_active_reward)
	{
		log_error("NULL == p_active_reward role_uid[%lu] active_reward_id[%d] Achieve_Event_Type[%d] event_id[%d]", uid, id, type, event_id);
		return;
	}

	if (p_active_reward->get_is_finish())
	{
		ACTIVE_REWARD_LOG("role_uid[%lu] active_reward_id[%d] Achieve_Event_Type[%d] event_id[%d] already finish already finish ", uid, id, type, event_id);
		return;
	}

	uint32_t cur_count_total = 0;
	if (!achieve_common_t::get_hero_achieve_total(uid, type, event_id, cur_count_total))
	{
		ACTIVE_REWARD_LOG("role[%lu] hero active_reward[%d] count is zero!", uid, id);
		return;
	}
	const uint32_t target = achieve_common_t::get_target_count(achieve_common_active_reward, id);
	const uint32_t old_count = p_active_reward->get_count();
	if (old_count < cur_count_total)
	{
		if (cur_count_total > target)
		{
			p_active_reward->set_count(target);
		} 
		else
		{
			p_active_reward->set_count(cur_count_total);
		}
		
	}
	const uint32_t new_count = p_active_reward->get_count();
	const uint32_t add_count = new_count - old_count;
	///p_active_reward->save_self(uid, true);
	save_all_self(uid);
	ACTIVE_REWARD_LOG("role[%lu] add active_reward[%d] addcount[%d] old count/new count[%d/%d]", uid, id, add_count, old_count, new_count);
	
	if (new_count < target)
	{
		///没有完成活跃奖励任务
		return;
	}
	p_active_reward->set_is_finish();
	do
	{
		activity_mgr_ptr p_activity_mgr = p_role->get_activity_mgr();
		if (NULL == p_activity_mgr)
		{
			log_error("role[%lu] NULL == p_activity_mgr", uid);
			break;
		}

		ActiveReward* p_config = GET_CONF(ActiveReward, id);
		if (NULL == p_config)
		{
			log_error("NULL == p_config[%d]", id);
			break;
		}

		p_activity_mgr->add_liveness_count(p_config->liveness());
		p_activity_mgr->save_self();

	} while (false);

	///p_active_reward->save_self(uid, true);
	save_all_self(uid);
	p_role->save_self();
	ACTIVE_REWARD_LOG("finish active_reward role_uid[%lu] Achieve_Event_Type[%d] active_reward_id[%d]", uid, type, id);
}

void active_reward_manager_t::achieve_unify_handle(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t event_id, uint32_t num /*= 1*/)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", uid);
		return;
	}
	uint32_t id = 0;
	if (!achieve_common_t::get_config_id(uid,achieve_common_active_reward,type, id, event_id))
	{
		ACTIVE_REWARD_LOG("Achieve_Event_Type[%d] id[%d]", type, id);
		return;
	}

	if (!achieve_common_t::add_element(achieve_common_active_reward, uid, id))
	{
		return;
	}
	active_reward_ptr p_active_reward = get_active_reward(uid, id);
	if (NULL == p_active_reward)
	{
		log_error("NULL == p_active_reward role_uid[%lu] active_reward_id[%d] Achieve_Event_Type[%d] event_id[%d]", uid, id, type, event_id);
		return;
	}

	if (p_active_reward->get_is_finish())
	{
		ACTIVE_REWARD_LOG("role_uid[%lu] active_reward_id[%d] Achieve_Event_Type[%d] event_id[%d] already finish", uid, id, type, event_id);
		return;
	}
	const uint32_t target = achieve_common_t::get_target_count(achieve_common_active_reward, id);
	const uint32_t old_count = p_active_reward->get_count();
	if (old_count + num > target)
	{
		p_active_reward->set_count(target);
	} 
	else
	{
		p_active_reward->set_count(old_count + num);
	}
	
	const uint32_t new_count = p_active_reward->get_count();
	const uint32_t add_count = new_count - old_count;
	///p_active_reward->save_self(uid, true);
	save_all_self(uid);
	ACTIVE_REWARD_LOG("role[%lu] add active_reward[%d] addcount[%d] old count/new count[%d/%d]", uid, id, add_count, old_count, new_count);
	
	if (new_count < target)
	{
		///没有完成活跃奖励任务
		return;
	}
	p_active_reward->set_is_finish();
	do
	{
		activity_mgr_ptr p_activity_mgr = p_role->get_activity_mgr();
		if (NULL == p_activity_mgr)
		{
			log_error("role[%lu] NULL == p_activity_mgr", uid);
			break;
		}

		ActiveReward* p_config = GET_CONF(ActiveReward, id);
		if (NULL == p_config)
		{
			log_error("NULL == p_config[%d]", id);
			break;
		}

		p_activity_mgr->add_liveness_count(p_config->liveness());
		p_activity_mgr->save_self();

	} while (false);

	///p_active_reward->save_self(uid, true);
	save_all_self(uid);
	p_role->save_self();
	ACTIVE_REWARD_LOG("finish active_reward role_uid[%lu] Achieve_Event_Type[%d] active_reward_id[%d]", uid, type, id);
}

void active_reward_manager_t::achieve_total_unify_handle(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t param /*= 0*/, uint32_t num /*= 1*/)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", uid);
		return;
	}
	uint32_t id = 0;
	if (!achieve_common_t::get_config_id(uid,achieve_common_active_reward,type, id))
	{
		ACTIVE_REWARD_LOG("Achieve_Event_Type[%d] id[%d]", type, id);
		return;
	}
	
	if (!achieve_common_t::add_element(achieve_common_active_reward, uid, id))
	{
		return;
	}
	active_reward_ptr p_active_reward = get_active_reward(uid, id);
	if (NULL == p_active_reward)
	{
		log_error("NULL == p_active_reward role_uid[%lu] active_reward_id[%d] Achieve_Event_Type[%d] event_id[%d]", uid, id, type, 0);
		return;
	}

	if (p_active_reward->get_is_finish())
	{
		ACTIVE_REWARD_LOG("role_uid[%lu] active_reward_id[%d] Achieve_Event_Type[%d] event_id[%d] already finish already finish ", uid,id,type,0);
		return;
	}
	const uint32_t target = achieve_common_t::get_target_count(achieve_common_active_reward, id);
	const uint32_t old_count = p_active_reward->get_count();
	if (old_count + num > target)
	{
		p_active_reward->set_count(target);
	}
	else
	{
		p_active_reward->set_count(old_count + num);
	}
	const uint32_t new_count = p_active_reward->get_count();
	const uint32_t add_count = new_count - old_count;
	///p_active_reward->save_self(uid, true);
	save_all_self(uid);
	ACTIVE_REWARD_LOG("role[%lu] add active_reward[%d] addcount[%d] old count/new count[%d/%d]", uid, id, add_count, old_count, new_count);
	
	if (new_count < target)
	{
		///没有完成活跃奖励任务
		return;
	}
	p_active_reward->set_is_finish();
	do
	{
		activity_mgr_ptr p_activity_mgr = p_role->get_activity_mgr();
		if (NULL == p_activity_mgr)
		{
			log_error("role[%lu] NULL == p_activity_mgr", uid);
			break;
		}

		ActiveReward* p_config = GET_CONF(ActiveReward, id);
		if (NULL == p_config)
		{
			log_error("NULL == p_config[%d]", id);
			break;
		}

		p_activity_mgr->add_liveness_count(p_config->liveness());
		p_activity_mgr->save_self();

	} while (false);

	///p_active_reward->save_self(uid, true);
	save_all_self(uid);
	p_role->save_self();
	ACTIVE_REWARD_LOG("finish active_reward role_uid[%lu] Achieve_Event_Type[%d] active_reward_id[%d]", uid, type, id);
}

void active_reward_manager_t::achieve_replace_model_handle(const uint64_t uid, const proto::common::Achieve_Event_Type type, uint32_t event_id /*= 0*/, uint32_t new_value /*= 0*/)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", uid);
		return;
	}
	uint32_t id = 0;
	if (!achieve_common_t::get_config_id(uid, achieve_common_active_reward, type, id, event_id))
	{
		ACTIVE_REWARD_LOG("Achieve_Event_Type[%d] id[%d]", type, id);
		return;
	}

	if (!achieve_common_t::add_element(achieve_common_active_reward, uid, id))
	{
		return;
	}
	active_reward_ptr p_active_reward = get_active_reward(uid, id);
	if (NULL == p_active_reward)
	{
		log_error("NULL == p_active_reward role_uid[%lu] active_reward_id[%d] Achieve_Event_Type[%d] event_id[%d]", uid, id, type, event_id);
		return;
	}

	if (p_active_reward->get_is_finish())
	{
		ACTIVE_REWARD_LOG("role_uid[%lu] active_reward_id[%d] Achieve_Event_Type[%d] event_id[%d] already finish already finish ", uid, id, type, event_id);
		return;
	}
	const uint32_t target = achieve_common_t::get_target_count(achieve_common_active_reward, id);
	const uint32_t old_count = p_active_reward->get_count();
	if (new_value >= target)
	{
		p_active_reward->set_count(target);
	}
	else if(old_count < new_value)
	{
		p_active_reward->set_count(new_value);
	}
	const uint32_t new_count = p_active_reward->get_count();
	const uint32_t add_count = new_count - old_count;
	///p_active_reward->save_self(uid, true);
	save_all_self(uid);
	ACTIVE_REWARD_LOG("role[%lu] add active_reward[%d] addcount[%d] old count/new count[%d/%d]", uid, id, add_count, old_count, new_count);

	if (new_count < target)
	{
		///没有完成活跃奖励任务
		return;
	}
	p_active_reward->set_is_finish();
	do 
	{
		activity_mgr_ptr p_activity_mgr = p_role->get_activity_mgr();
		if (NULL == p_activity_mgr)
		{
			log_error("role[%lu] NULL == p_activity_mgr", uid);
			break;
		}

		ActiveReward* p_config = GET_CONF(ActiveReward, id);
		if (NULL == p_config)
		{
			log_error("NULL == p_config[%d]", id);
			break;
		}

		p_activity_mgr->add_liveness_count(p_config->liveness());
		p_activity_mgr->save_self();

	} while (false);
	
	///p_active_reward->save_self(uid, true);
	save_all_self(uid);
	p_role->save_self();
	ACTIVE_REWARD_LOG("finish active_reward role_uid[%lu] Achieve_Event_Type[%d] active_reward_id[%d]", uid, type, id);
}

void active_reward_manager_t::load_all_data(uint64_t uid, const proto::common::active_reward_data& data)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", uid);
		return;
	}

	for (int32_t i = 0; i < data.active_reward_size(); ++i)
	{
		active_reward_ptr p_active_reward = active_reward_ptr(new active_reward_t());
		p_active_reward->load_data(uid, data.active_reward(i));
		add_active_reward(uid, p_active_reward,true);
	}
}

void active_reward_manager_t::peek_all_data(uint64_t uid, proto::client::gc_active_reward_list_reply& reply, bool is_client /* = true */)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", uid);
		return;
	}

	proto::common::active_reward_data* p_data = reply.mutable_active_reward_list();
	std::set<uint32_t>& task_list = p_role->get_active_reward_task_list();
	if (task_list.empty())
	{
		log_error("task_list.empty() role_uid[%lu] exist active reward task task_list.size() [%d]", uid, (int32_t)task_list.size());
	}
	std::map<uint32_t, active_reward_ptr>& active_reward_map = p_role->get_active_reward_map();
	proto::common::active_reward_data data_list;
	proto::common::active_reward_data data_already_list;
	for (auto& it : task_list)
	{
		if (0 == active_reward_map.count(it)){
			ActiveReward* p_config = GET_CONF(ActiveReward, it);
			if (NULL == p_config){
				log_error("NULL ==  p_config id[%d]", it);
				continue;
			}
			proto::common::active_reward_data_single* p_single = data_list.add_active_reward();
			p_single->set_tid(it);
			p_single->set_is_finish(false);
			p_single->set_count(0);
			p_single->set_is_grant(false);
		}
		else{
			active_reward_ptr p_active_reward = active_reward_map[it];
			if (NULL == p_active_reward){
				log_error("NULL == p_active_reward role_uid[%lu] active_reward_id[%d]", uid, it);
				continue;
			}

			if (!p_active_reward->get_is_finish()){
				proto::common::active_reward_data_single* p_single = data_list.add_active_reward();
				p_active_reward->peek_data(p_single, is_client);
				continue;
			}
			if (p_active_reward->get_is_grant()){
				p_active_reward->peek_data(data_already_list.add_active_reward(), is_client);
			}
			else{
				p_active_reward->peek_data(p_data->add_active_reward(), is_client);
			}
		}		
	}

	for (int32_t i = 0; i < data_list.active_reward_size(); ++i )
	{
		proto::common::active_reward_data_single* p_single = p_data->add_active_reward();
		p_single->CopyFrom(data_list.active_reward(i));
	}

	for (int32_t i = 0; i < data_already_list.active_reward_size(); ++i)
	{
		proto::common::active_reward_data_single* p_single = p_data->add_active_reward();
		p_single->CopyFrom(data_already_list.active_reward(i));
	}

	//for (int i = 0; i < p_data->active_reward_size(); ++i)
	//{
	//	const proto::common::active_reward_data_single& single = p_data->active_reward(i);
	//	log_warn("is_finish[%d] is_grant[%d]", single.is_finish(),single.is_grant());
	//}
}

void active_reward_manager_t::peek_all_data(uint64_t uid, proto::common::active_reward_data* p_data)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", uid);
		return;
	}

	if (NULL == p_data)
	{
		log_error("NULL == p_data role_uid[%lu]", uid);
		return;
	}
	
	std::map<uint32_t, active_reward_ptr>& active_reward_map = p_role->get_active_reward_map();
	for (auto& it : active_reward_map)
	{
		active_reward_ptr p_active_reward = it.second;
		if (NULL == p_active_reward)
		{
			log_error("NULL == p_active_reward role_uid[%lu] active_reward_id[%d]", uid, it.first);
			continue;
		}
		p_active_reward->peek_data(p_data->add_active_reward(), false);
	}
	
}

//void active_reward_manager_t::save_all_self(uint64_t uid)
//{
//	role_ptr p_role = role_manager_t::find_role(uid);
//	if (NULL == p_role)
//	{
//		log_error("NULL == p_role role_uid[%lu]", uid);
//		return;
//	}
//	std::map<uint32_t, active_reward_ptr>& active_reward_map = p_role->get_active_reward_map();
//	if (active_reward_map.empty())
//	{
//		///玩家可能会没有完成任何活跃奖励任务
//		ACTIVE_REWARD_LOG("active_reward_map.empty() role_uid[%lu]", uid);
//		return;
//	}
//	for (auto& it : active_reward_map)
//	{
//		active_reward_ptr p_active_reward = it.second;
//		if (NULL == p_active_reward)
//		{
//			log_error("NULL == p_active_reward role_uid[%lu] active_reward_id[%d]", uid, it.first);
//			continue;
//		}
//		p_active_reward->save_self(uid);
//	}
//}

void active_reward_manager_t::save_all_self(uint64_t uid)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", uid);
		return;
	}
	proto::common::active_reward_data data;
	peek_all_data(uid, &data);
	if (0 != data.active_reward_size())
	{
		std::string key = cache_key_t::create(uid, cache_name::active_reward);
		role_unify_save::add_task(uid, key, data);
	}
}

bool active_reward_manager_t::add_active_reward(const uint64_t role_uid, const uint32_t id,const proto::common::Achieve_Event_Type type)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", role_uid);
		return false;
	}

	ActiveReward* p_config = GET_CONF(ActiveReward, id);
	if (NULL == p_config)
	{
		log_error("NULL ==  p_config id[%d]", id);
		return false;
	}

	const Dawn::triple<uint32_t, uint32_t, uint32_t>& tuple_list = p_config->condition();
	std::map<uint32_t,active_reward_ptr>& active_reward_map = p_role->get_active_reward_map();
	auto iter = active_reward_map.find(id);
	if (iter == active_reward_map.end())
	{
		uint64_t uid = achieve_common_t::gen_uid(achieve_common_active_reward);
		active_reward_ptr p_active_reward = active_reward_ptr(new active_reward_t(uid,id,role_uid,type));
		if (NULL != p_active_reward)
		{
			p_active_reward->set_event_id(tuple_list.second);
			active_reward_map.insert(std::make_pair(p_active_reward->get_tid(), p_active_reward));
		}
		
		///保存
		///p_active_reward->save_self(role_uid);
		save_all_self(role_uid);
	}
	else
	{
		active_reward_ptr p_active_reward = iter->second;
		if (NULL == p_active_reward)
		{
			log_error("NULL == p_active_reward role[%lu] id[%d]",role_uid,id);
			return false;
		}

		if (p_active_reward->compare_is_same(id, tuple_list.first, tuple_list.second))
		{
			p_active_reward->set_type(tuple_list.first);
			p_active_reward->reset_count();
			p_active_reward->set_event_id(tuple_list.second);
			///p_active_reward->save_self(role_uid);
			save_all_self(role_uid);
		}
	}
	return true;
}

bool active_reward_manager_t::add_active_reward(const uint64_t role_uid, active_reward_ptr p_active_reward,bool is_load /* = false */)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", role_uid);
		return false;
	}

	if (NULL == p_active_reward)
	{
		log_error("NULL == p_active_reward role_uid[%lu]", role_uid);
		return false;
	}

	std::map<uint32_t, active_reward_ptr>& active_reward_map = p_role->get_active_reward_map();

	active_reward_map.insert(std::make_pair(p_active_reward->get_tid(), p_active_reward));
	///p_active_reward->save_self(role_uid, true);

	if (!is_load)
	{
		save_all_self(role_uid);
	}
	return true;
}

active_reward_ptr active_reward_manager_t::get_active_reward(const uint64_t role_uid, const uint32_t id)
{
	if (0 == id)
	{
		log_error("0 == id role_uid[%lu] id[%d]", role_uid, id);
		return active_reward_ptr();
	}

	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu] ", role_uid);
		return active_reward_ptr();
	}
	std::map<uint32_t, active_reward_ptr>& active_reward_map = p_role->get_active_reward_map();
	auto iter = active_reward_map.find(id);
	if (iter != active_reward_map.end())
		return iter->second;

	return active_reward_ptr();
}

bool active_reward_manager_t::get_level_range(const uint32_t level, uint32_t& range)
{
	std::string length = boost::lexical_cast<std::string>(level);
	if (1 == length.length())
	{
		///[0,9]
		range = 1;
		return true;
	}
	else if (2 == length.length())
	{
		///[10,99]
		range = uint32_t((level - level % 10) / 10) + 1;
		return true;
	}
	else if (3 == length.length())
	{
		///[100,999] 为了以后扩展 目前等级没有三位数
		range = uint32_t((level - level % 100) / 10) + 1;
		return true;
	}
	else
	{
		return false;
	}
}

void active_reward_manager_t::init_task_list(const uint64_t role_uid)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", role_uid);
		return;
	}
	uint32_t level_range = 0;
	if (!get_level_range(p_role->get_level(), level_range))
	{
		log_error("role[%lu] level range[%d] error", role_uid,level_range);
		return;
	}

	std::map<uint32_t, ActiveReward*> confs;
	GET_ALL_CONF(ActiveReward, confs);
	if (confs.empty())
	{
		log_error("role[%lu] new day init active reward task list fail!", role_uid);
		return;
	}
	check_not_grant_reward_send_mail(role_uid);
	std::set<uint32_t>& task_list = p_role->get_active_reward_task_list();
	std::map<uint32_t, active_reward_ptr>& active_reward_map = p_role->get_active_reward_map();
	task_list.clear();
	active_reward_map.clear();
	
	for (auto& it : confs)
	{
		ActiveReward* p_config = it.second;
		if (NULL == p_config)
		{
			log_error("NULL == p_config role[%lu] new day init active reward task fail",role_uid);
			return;
		}
		const std::set<uint32_t>& set_config  = p_config->level_range_set();
		if (set_config.empty())
		{
			log_error("set_config.empty() [%d]", it.first);
			continue;
		}
			
	
		if (0 == set_config.count(level_range))
		{
			continue;
		}
			
		task_list.insert(it.first);
	}
	ACTIVE_REWARD_LOG("role[%lu] new day init active reward task list success!",role_uid);
}

void active_reward_manager_t::get_active_reward_list(const uint64_t role_uid)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", role_uid);
		return;
	}

	proto::client::gc_active_reward_list_reply reply;
	peek_all_data(role_uid, reply);
	
	p_role->send_msg_to_client(op_cmd::gc_active_reward_list_reply, reply);
}

void active_reward_manager_t::check_not_grant_reward_send_mail(const uint64_t role_uid)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", role_uid);
		return;
	}

	std::map<uint32_t, active_reward_ptr>& active_reward_map = p_role->get_active_reward_map();

	for (auto& it : active_reward_map)
	{
		active_reward_ptr p_reward = it.second;
		if (NULL == p_reward)
		{
			continue;
		}
		if (!p_reward->get_is_finish())
		{
			continue;
		}
		if (p_reward->get_is_grant())
		{
			continue;
		}
		const uint32_t drop_id = achieve_common_t::get_drop_id(achieve_common_active_reward, p_reward->get_tid());
		bool b_result = drop_manager_t::drop(p_role, drop_id, log_enum::source_type_active_reward_finish,
			proto::common::drop_sys_type_active_reward, p_reward->get_tid());
		if (!b_result)
		{
			log_error("b_result false role[%lu] active_reward_id[%d] drop_id[%d]", p_role->get_uid(), p_reward->get_tid(), drop_id);
			continue;
		}
		ACTIVE_REWARD_LOG("role[%lu] not get active reward [%d],new day init  auto grant drop", role_uid, p_reward->get_tid());
	}
}

uint32_t active_reward_manager_t::get_active_reward_gift(const uint64_t role_uid, const uint32_t tid,proto::common::role_change_data* p_data)
{
	uint32_t reply_code = errcode_enum::error_ok;
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role_uid[%lu]", role_uid);
		return errcode_enum::notice_unknown;
	}

	if (NULL == p_data)
	{
		log_error("NULL == p_data role_uid[%lu]", role_uid);
		return errcode_enum::notice_unknown;
	}

	active_reward_ptr p_active = get_active_reward(role_uid, tid);
	if (NULL == p_active)
	{
		log_error("NULL == p_active role_uid[%lu]", role_uid);
		return  errcode_enum::notice_unknown;
	}

	do 
	{
		if (!p_active->get_is_finish())
		{
			reply_code = errcode_enum::notice_active_reward_task_not_finish;	///没有完成该活跃任务
			break;
		}

		if (p_active->get_is_grant())
		{
			reply_code = errcode_enum::notice_active_reward_already_get;	///已经领取该活跃任务奖励
			break;
		}

		const uint32_t drop_id = achieve_common_t::get_drop_id(achieve_common_active_reward, tid);
		bool b_result = drop_manager_t::drop(p_role, drop_id, log_enum::source_type_active_reward_finish,
			proto::common::drop_sys_type_active_reward, tid, p_data);
		if (!b_result)
		{
			log_error("b_result false role[%lu] get active_reward id[%d] fail drop_id[%d] ", role_uid, tid, drop_id);
			reply_code = errcode_enum::notice_unknown;
		}
		
	} while (false);

	if (reply_code == errcode_enum::error_ok)
	{
		p_active->set_is_grant();
		///p_active->save_self(role_uid, true);
		save_all_self(role_uid);
		p_role->save_self();
		ACTIVE_REWARD_LOG("role_uid[%lu] get active_reward id[%d]", role_uid, tid);
	}
	
	return reply_code;
}

