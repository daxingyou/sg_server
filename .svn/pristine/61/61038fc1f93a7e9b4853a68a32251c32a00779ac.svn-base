#include "activity_manager.hpp"
#include "cache_key.hpp"
#include "tblh/Activities.tbls.h"
#include "tblh/Liveness.tbls.h"
#include "config_mgr.h"
#include "role/role_unify_save.hpp"
#include "role/role.hpp"
#include "role/role_manager.hpp"
#include "item/item_manager.hpp"
#include "role/money_manager.hpp"
#include "time_manager.hpp"
#include "red_point/red_point_manager.hpp"
#include "sys_notice/sys_notice_manager.hpp"
USING_NS_COMMON;

activity_manager_t::activity_manager_t(uint64_t role_uid)
	: m_role_uid(role_uid)
{
	m_key = cache_key_t::create(m_role_uid, cache_name::activity);
}

void activity_manager_t::load_data(const proto::common::activities_data& activities_data)
{
	m_activity_map.clear();
	m_box_vec.clear();
	for (int i = 0; i < activities_data.activity_data_size(); ++i)
	{
		activity_ptr p_activity = activity_ptr(new activity_t());
		p_activity->load_data(activities_data.activity_data(i));
		
		m_activity_map.insert(std::make_pair(p_activity->get_tid(), p_activity));
		auto p_conf = GET_CONF(Activities, p_activity->get_tid());
		if (NULL == p_conf)
		{
			continue;
		}
		if (p_conf->reset_type() != activity_reset_open)
		{
			continue;
		}
		for (auto time_id : p_conf->time_id())
		{
			//只有活动进行中 并且上一次重置的时间不在此进行周期内才重置
			if (!time_manager_t::is_in_time(time_id))
			{
				continue;
			}
			if (time_manager_t::is_same_period(time_id, p_activity->get_last_reset_time()))
			{
				continue;
			}
			p_activity->set_count(0);
			p_activity->set_last_reset_time(common::time_util_t::now_time());
		}
	}
	
	for (int i = 0; i < activities_data.box_list_size(); ++i)
	{
		m_box_vec.push_back(activities_data.box_list(i).liveness());
	}
	m_liveness_count = activities_data.liveness();
}

void activity_manager_t::peek_data(proto::common::activities_data* p_activities_data)
{
	if (NULL == p_activities_data)
	{
		return;
	}
	std::map<uint32_t, Activities*> activities_map;
	GET_ALL_CONF(Activities, activities_map);
	for (auto it : activities_map)
	{
		uint32_t activity_id = it.first;
		auto it_activity = m_activity_map.find(activity_id);
		if (it_activity != m_activity_map.end())
		{
			activity_ptr p_activity = it_activity->second;
			if (NULL == p_activity)
			{
				log_error("NULL == p_activity");
				continue;
			}
			
			auto p_conf = GET_CONF(Activities, p_activity->get_tid());
			if (NULL == p_conf)
			{
				continue;
			}
			if (p_conf->reset_type() == activity_reset_open)
			{
				for (auto time_id : p_conf->time_id())
				{
					//只有活动进行中 并且上一次重置的时间不在此进行周期内才重置
					if (time_manager_t::is_in_time(time_id))
					{
						if (!time_manager_t::is_same_period(time_id, p_activity->get_last_reset_time()))
						{
							p_activity->set_count(0);
							p_activity->set_last_reset_time(common::time_util_t::now_time());
						}
					}
				}
			}
			
			proto::common::activity_single* p_single = p_activities_data->add_activity_data();
			p_activity->peek_data(p_single);
			//TASK_LOG("activity_id[%d] count[%d]", p_single->activity_id(), p_single->count());
		}
		else
		{
			proto::common::activity_single* p_single = p_activities_data->add_activity_data();
			p_single->set_activity_id(activity_id);
			p_single->set_count(0);
			p_single->set_last_reset_time(0);
			//TASK_LOG("activity_id[%d] count[%d]", p_single->activity_id(), p_single->count());
		}
	}
	p_activities_data->set_liveness(m_liveness_count);
	
	std::map<uint32_t, Liveness*> liveness_map;
	GET_ALL_CONF(Liveness, liveness_map);
	for (auto it : liveness_map)
	{
		uint32_t liveness = it.first;
		proto::common::liveness_box* p_liveness_box = p_activities_data->add_box_list();
		p_liveness_box->set_liveness(liveness);
		std::vector<uint32_t>::iterator it_vec = std::find(m_box_vec.begin(), m_box_vec.end(), liveness);
		if (it_vec != m_box_vec.end())
		{
			p_liveness_box->set_state(liveness_box_state_over);
		}
		else
		{
			if (m_liveness_count >= liveness)
			{
				p_liveness_box->set_state(liveness_box_state_finish);
			}
			else
			{
				p_liveness_box->set_state(liveness_box_state_none);
			}
		}
	}
}

void activity_manager_t::peek_family_activity(proto::client::gc_get_family_activities_reply& reply)
{
	std::map<uint32_t, Activities*> activities_map;
	GET_ALL_CONF(Activities, activities_map);
	for (const auto& it : activities_map) {
		if (NULL == it.second) continue;
		if (it.second->type() != activity_family) continue;

		auto it_activity = m_activity_map.find(it.first);
		if (it_activity != m_activity_map.end()) {
			activity_ptr p_activity = it_activity->second;
			if (NULL == p_activity) {
				log_error("NULL == p_activity");
				continue;
			}

			if (it.second->reset_type() == activity_reset_open) {
				for (auto time_id : it.second->time_id()) {
					//只有活动进行中 并且上一次重置的时间不在此进行周期内才重置
					if (time_manager_t::is_in_time(time_id)) {
						if (!time_manager_t::is_same_period(time_id, p_activity->get_last_reset_time())) {
							p_activity->set_count(0);
							p_activity->set_last_reset_time(common::time_util_t::now_time());
						}
					}
				}
			}

			p_activity->peek_data(reply.add_activity_data());
		} else {
			proto::common::activity_single* p_single = reply.add_activity_data();
			if (p_single) {
				p_single->set_activity_id(it.first);
				p_single->set_count(0);
				p_single->set_last_reset_time(0);
			}
		}
	}
}

void activity_manager_t::save_self()
{
	proto::common::activities_data activities_data;
	peek_data(&activities_data);
	role_unify_save::add_task(m_key, activities_data, false, true, m_role_uid);
}

bool activity_manager_t::liveness_box(uint32_t box_id, proto::common::role_change_data* p_data)
{
	if (m_liveness_count < box_id)
	{
		log_error("role[%lu] liveness not enough", m_role_uid);
		return false;
	}
	std::vector<uint32_t>::iterator it_vec = std::find(m_box_vec.begin(), m_box_vec.end(), box_id);
	if (it_vec != m_box_vec.end())
	{
		log_error("role[%lu] star_box[%d] already get", m_role_uid, box_id);
		return false;
	}
	auto p_liveness_conf = GET_CONF(Liveness, box_id);
	if (NULL == p_liveness_conf)
	{
		log_error("role[%lu] liveness_box[%d] invalid", m_role_uid, box_id);
		return false;
	}
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_role_uid);
		return false;
	}
	m_box_vec.push_back(box_id);
	uint32_t item_id = p_liveness_conf->award().first;
	uint32_t item_count = p_liveness_conf->award().second;
	item_manager_t::add_item(p_role, item_id, item_count, log_enum::source_type_bounty_star_box, box_id, p_data);

	save_self();
	return true;
}

void activity_manager_t::one_day()
{
	for (auto it : m_activity_map)
	{
		activity_ptr p_activity = it.second;
		if (NULL == p_activity)
		{
			continue;
		}
		auto p_activity_conf = GET_CONF(Activities, p_activity->get_tid());
		if (NULL == p_activity_conf)
		{
			log_error("role[%lu] p_activity_conf[%d] invalid", m_role_uid, p_activity->get_tid());
			continue;
		}
		if (p_activity_conf->reset_type() != activity_reset_day)
		{
			continue;
		}
		p_activity->set_count(0);
	}
	m_box_vec.clear();
	m_liveness_count = 0;
	save_self();
}

void activity_manager_t::one_week()
{
	for (auto it : m_activity_map)
	{
		activity_ptr p_activity = it.second;
		if (NULL == p_activity)
		{
			continue;
		}
		auto p_activity_conf = GET_CONF(Activities, p_activity->get_tid());
		if (NULL == p_activity_conf)
		{
			log_error("role[%lu] p_activity_conf[%d] invalid", m_role_uid, p_activity->get_tid());
			continue;
		}
		if (p_activity_conf->reset_type() != activity_reset_week)
		{
			continue;
		}
		p_activity->set_count(0);
	}

	save_self();
}

bool activity_manager_t::check_open(uint32_t activity_id)
{
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_role_uid);
		return false;
	}
	auto p_activity_conf = GET_CONF(Activities, activity_id);
	if (NULL == p_activity_conf)
	{
		log_error("role[%lu] p_activity_conf[%d] invalid", m_role_uid, activity_id);
		return false;
	}
	uint32_t count = 0;
	auto it_activity = m_activity_map.find(activity_id);
	if (it_activity != m_activity_map.end())
	{
		auto p_activity = it_activity->second;
		if (NULL != p_activity)
		{
			if (p_activity_conf->reset_type() == activity_reset_open)
			{
				for (auto time_id : p_activity_conf->time_id())
				{
					if (time_manager_t::is_in_time(time_id))
					{
						if (!time_manager_t::is_same_period(time_id, p_activity->get_last_reset_time()))
						{
							//只有活动进行中 并且上一次重置的时间不在此进行周期内才重置
							p_activity->set_count(0);
							p_activity->set_last_reset_time(common::time_util_t::now_time());
						}
					}
				}
			}
			count = p_activity->get_count();
		}
	}	
	
	if (p_activity_conf->times() > 0 && count >= p_activity_conf->times())
	{
		log_error("role[%lu] activity[%d] times limit count[%d] invalid", m_role_uid, activity_id, count);
		return false;
	}
	if (p_role->get_level() < p_activity_conf->level())
	{
		log_error("role[%lu] level[%d] activity_id[%d] level[%d] check failed", m_role_uid, p_role->get_level(), activity_id, p_activity_conf->level());
		return false;
	}
	if (p_activity_conf->numbers() == 1 && p_role->get_troop_id() > 0)
	{
		log_error("role[%lu] activity_id[%d] group check failed", m_role_uid, activity_id);
		return false;
	}
	if (p_activity_conf->numbers() == 2 && p_role->get_troop_id() == 0)
	{
		log_error("role[%lu] activity_id[%d] group check failed", m_role_uid, activity_id);
		return false;
	}

	//return check_time_open(activity_id);
	bool result = check_time_open(activity_id);
	if (result)
	{
		BROADCAST_SYS_NOTICE_SYS_CHANNEL(m_role_uid, proto::common::sys_notify_type_activity, activity_id);
	}
	return result;
	
}

void activity_manager_t::activity_done(uint32_t activity_id)
{
	auto p_activity_conf = GET_CONF(Activities, activity_id);
	if (NULL == p_activity_conf)
	{
		log_error("role[%lu] p_activity_conf[%d] invalid", m_role_uid, activity_id);
		return;
	}
	activity_ptr p_activity;
	auto it_activity = m_activity_map.find(activity_id);
	if (it_activity != m_activity_map.end())
	{
		p_activity = it_activity->second;
		if (NULL == p_activity)
		{
			log_error("NULL == p_activity");
			return;
		}
	}
	else
	{
		p_activity = activity_ptr(new activity_t(activity_id));
		m_activity_map.insert(std::make_pair(p_activity->get_tid(), p_activity));
	}

	//容错代码（当前值大于最大值）
	if (p_activity->get_count() >= p_activity_conf->times())
	{
		return;
	}

	p_activity->set_count(p_activity->get_count() + 1);
	m_liveness_count += p_activity_conf->liveness();
	TASK_LOG("role[%lu] activity_done[%d] count[%d]", m_role_uid, activity_id, p_activity->get_count());
	save_self();

	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL != p_role)
	{
		proto::client::gc_activity_done_notify ntf;
		p_activity->peek_data(ntf.mutable_activity_data());
		p_role->send_msg_to_client(op_cmd::gc_activity_done_notify, ntf);
	}
	//red_point_manager_t::check_activity_open_box(m_role_uid);
	sys_notice_manager_t::broadcast_system_msg_for_money(m_role_uid, proto::common::MONEY_TYPE_EXP, p_activity_conf->liveness());
}

bool activity_manager_t::check_time_open(uint32_t activity_id)
{
	auto p_activity_conf = GET_CONF(Activities, activity_id);
	if (NULL == p_activity_conf)
	{
		log_error("p_activity_conf[%d] invalid", activity_id);
		return false;
	}
	for (auto time_id : p_activity_conf->time_id())
	{
		if (time_manager_t::is_in_time(time_id))
		{
			return true;
		}
	}
	log_error("activity_id[%d] check_time_open failed", activity_id);
	return false;
}

bool activity_manager_t::is_already_open_box(uint32_t liveness)
{
	const std::vector<uint32_t>::iterator it_vec = std::find(m_box_vec.begin(), m_box_vec.end(), liveness);
	if (it_vec != m_box_vec.end())
	{
		RED_POINT_LOG("role[%lu] star_box[%d] already get", m_role_uid, liveness);
		return true;
	}
	return false;
}

bool activity_manager_t::add_liveness_count(uint32_t val)
{
	uint32_t old_count = m_liveness_count;
	m_liveness_count += val; 

	RED_POINT_LOG("role[%lu],add liveness count,old count[%d],add count[%d],new count[%d]", m_role_uid, old_count, val, m_liveness_count);
	sys_notice_manager_t::broadcast_system_msg_for_money(m_role_uid, proto::common::MONEY_TYPE_LIVENESS, val);
	red_point_manager_t::check_activity_open_box(m_role_uid);

	return true;
}

uint32_t activity_manager_t::get_activity_count(uint32_t activity_id)
{
	uint32_t count = 0;
	auto it_activity = m_activity_map.find(activity_id);
	if (it_activity != m_activity_map.end())
	{
		activity_ptr p_activity = it_activity->second;
		if (NULL != p_activity)
		{
			count = p_activity->get_count();
		}
	}
	return count;
}

bool activity_manager_t::is_finish_all_circle(uint32_t activity_id)
{
	auto p_activity_conf = GET_CONF(Activities, activity_id);
	if (NULL == p_activity_conf)
	{
		log_error("role[%lu] p_activity_conf[%d] invalid", m_role_uid, activity_id);
		return false;
	}

	auto it_activity = m_activity_map.find(activity_id);
	if (it_activity == m_activity_map.end())
		return false;
	
	auto p_activity = it_activity->second;
	if (NULL == p_activity)
		return false;

	uint32_t count = p_activity->get_count();

	if (p_activity_conf->times() > 0 && count >= p_activity_conf->times())
		return false;

	return true;
}
