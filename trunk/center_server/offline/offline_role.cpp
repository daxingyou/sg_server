#include "offline_role.hpp"
#include "main/center_server.hpp"

#include "utility.hpp"
#include "redis_client.hpp"
#include "user/global_user_manager.hpp"

USING_NS_COMMON;

offline_role_t::offline_role_t( uint64_t uid ) :
	m_uid(uid)
{
}

offline_role_t::~offline_role_t()
{

}

void offline_role_t::online()
{
	bool need_notify = false;
	
	proto::server::eg_offline_data_notify ntf;
	proto::common::offline_role_data* p_data = ntf.mutable_ord();
	proto::common::tower_trigger_data* p_tower_trigger_data = p_data->mutable_tower_trigger();
	uint32_t now_time = time_util_t::now_time();
	for (auto tower_trigger : m_other_trigger_vec)
	{
		if (now_time >= tower_trigger.expired_time)
		{
			continue;
		}
		need_notify = true;
		proto::common::tower_trigger* p_tower_trigger = p_tower_trigger_data->add_other_trigger();
		p_tower_trigger->set_uid(tower_trigger.uid);
		p_tower_trigger->set_tid(tower_trigger.tid);
		p_tower_trigger->set_expired_time(tower_trigger.expired_time);
		p_tower_trigger->set_finder_name(tower_trigger.finder_name);
	}
	m_self_trigger_vec.clear();
	m_other_trigger_vec.clear();
	save_self();
	
	if (need_notify)
	{
		global_user_ptr p_role = global_user_manager_t::get_global_user_by_uid(m_uid);
		if (NULL != p_role)
		{
			p_role->send_msg_to_game(op_cmd::eg_offline_data_notify, ntf);
		}
	}
}

void offline_role_t::save_self()
{
	proto::common::offline_role_data ord;
	proto::common::tower_trigger_data* p_tower_trigger_data = ord.mutable_tower_trigger();
	uint32_t now_time = time_util_t::now_time();
	for (auto tower_trigger : m_other_trigger_vec)
	{
		if (now_time >= tower_trigger.expired_time)
		{
			continue;
		}
		proto::common::tower_trigger* p_tower_trigger = p_tower_trigger_data->add_other_trigger();
		p_tower_trigger->set_uid(tower_trigger.uid);
		p_tower_trigger->set_tid(tower_trigger.tid);
		p_tower_trigger->set_expired_time(tower_trigger.expired_time);
		p_tower_trigger->set_finder_name(tower_trigger.finder_name);
	}
	for (auto tower_trigger : m_self_trigger_vec)
	{
		if (now_time >= tower_trigger.expired_time)
		{
			continue;
		}
		proto::common::tower_trigger* p_tower_trigger = p_tower_trigger_data->add_self_trigger();
		p_tower_trigger->set_uid(tower_trigger.uid);
		p_tower_trigger->set_tid(tower_trigger.tid);
		p_tower_trigger->set_expired_time(tower_trigger.expired_time);
		p_tower_trigger->set_finder_name(tower_trigger.finder_name);
	}
	redis_client_t::set_protobuf("offline_role_data", "offline_role_data", m_uid, ord);
}

void offline_role_t::add_other_trigger(const proto::common::tower_trigger& tower_trigger)
{
	uint64_t uid = string_util_t::string_to_uint64(tower_trigger.uid());
	tower_trigger_data ttd;
	ttd.uid = uid;
	ttd.tid = tower_trigger.tid();
	ttd.expired_time = tower_trigger.expired_time();
	ttd.finder_name = tower_trigger.finder_name();
	m_other_trigger_vec.push_back(ttd);
}

void offline_role_t::add_other_trigger(const proto::server::ge_tower_trigger_notify& ntf)
{
	for (int32_t i = 0; i < ntf.tower_trigger_size(); ++i)
	{
		add_other_trigger(ntf.tower_trigger(i));
	}
}

void offline_role_t::offline(const proto::common::offline_role_data& ord)
{
	const proto::common::tower_trigger_data& ttd = ord.tower_trigger();
	for (int32_t i = 0; i < ttd.self_trigger_size(); ++i)
	{
		const proto::common::tower_trigger& tower_trigger = ttd.self_trigger(i);
		uint64_t uid = string_util_t::string_to_uint64(tower_trigger.uid());
		tower_trigger_data ttd;
		ttd.uid = uid;
		ttd.tid = tower_trigger.tid();
		ttd.expired_time = tower_trigger.expired_time();
		ttd.finder_name = tower_trigger.finder_name();
		m_self_trigger_vec.push_back(ttd);
	}
}

void offline_role_t::notify_self_trigger_to_friend(uint64_t friend_uid)
{
	global_user_ptr p_friend = global_user_manager_t::get_global_user_by_uid(friend_uid);
	if (NULL == p_friend)
	{
		log_error("NULL == p_friend[%lu]", friend_uid);
		return;
	}
	uint32_t now_time = time_util_t::now_time();
	proto::server::eg_tower_trigger_notify ntf;
	for (auto tower_trigger : m_self_trigger_vec)
	{
		if (now_time >= tower_trigger.expired_time)
		{
			continue;
		}
		proto::common::tower_trigger* p_tower_trigger = ntf.add_tower_trigger();
		p_tower_trigger->set_uid(tower_trigger.uid);
		p_tower_trigger->set_tid(tower_trigger.tid);
		p_tower_trigger->set_expired_time(tower_trigger.expired_time);
		p_tower_trigger->set_finder_name(tower_trigger.finder_name);
	}
	p_friend->send_msg_to_game(op_cmd::eg_tower_trigger_notify, ntf);
}
