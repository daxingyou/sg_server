#include "offline_role_manager.hpp"
#include "main/center_server.hpp"
#include "common/redis_client.hpp"

USING_NS_COMMON;

offline_role_map offline_role_manager_t::m_offline_role_map;

void offline_role_manager_t::online(uint64_t role_uid)
{
	offline_role_map::iterator it = m_offline_role_map.find(role_uid);
	if (it == m_offline_role_map.end())
	{
		return;
	}
	offline_role_ptr p_offline_role = it->second;
	if (NULL == p_offline_role)
	{
		return;
	}
	p_offline_role->online();
}

void offline_role_manager_t::offline(uint64_t role_uid, const proto::common::offline_role_data& ord)
{
	offline_role_ptr p_offline_role;
	offline_role_map::iterator it = m_offline_role_map.find(role_uid);
	if (it == m_offline_role_map.end())
	{
		p_offline_role = offline_role_ptr(new offline_role_t(role_uid));
	}
	else
	{
		p_offline_role = it->second;
	}
	if (NULL == p_offline_role)
	{
		return;
	}
	p_offline_role->offline(ord);
}

void offline_role_manager_t::add_other_trigger(uint64_t role_uid, const proto::common::tower_trigger& tower_trigger)
{
	offline_role_ptr p_offline_role;
	offline_role_map::iterator it = m_offline_role_map.find(role_uid);
	if (it == m_offline_role_map.end())
	{
		p_offline_role = offline_role_ptr(new offline_role_t(role_uid));
	}
	else
	{
		p_offline_role = it->second;
	}
	if (NULL == p_offline_role)
	{
		return;
	}
	p_offline_role->add_other_trigger(tower_trigger);
}

void offline_role_manager_t::add_other_trigger(uint64_t role_uid, const proto::server::ge_tower_trigger_notify& ntf)
{
	offline_role_ptr p_offline_role;
	offline_role_map::iterator it = m_offline_role_map.find(role_uid);
	if (it == m_offline_role_map.end())
	{
		p_offline_role = offline_role_ptr(new offline_role_t(role_uid));
	}
	else
	{
		p_offline_role = it->second;
	}
	if (NULL == p_offline_role)
	{
		return;
	}
	p_offline_role->add_other_trigger(ntf);
}

void offline_role_manager_t::notify_self_trigger_to_friend(uint64_t self_uid, uint64_t friend_uid)
{
	offline_role_map::iterator it = m_offline_role_map.find(self_uid);
	if (it == m_offline_role_map.end())
	{
		return;
	}
	offline_role_ptr p_offline_role = it->second;
	if (NULL == p_offline_role)
	{
		return;
	}
	p_offline_role->notify_self_trigger_to_friend(friend_uid);
}
