#include "active_reward.hpp"
#include "common/log.hpp"
#include "cache_key.hpp"
#include "utility.hpp"
#include "main/game_server_fwd.hpp"
#include "tblh/ActiveReward.tbls.h"
#include "achieve_common.hpp"
#include "role/role_unify_save.hpp"

active_reward_t::active_reward_t(uint64_t uid, uint32_t tid, uint64_t role_uid, uint32_t type)
	: m_role_uid(role_uid)
	, m_tid(tid)
	, m_uid(uid)
	, m_type(type)
{
	m_key = cache_key_t::create(m_uid, cache_name::active_reward);
}


void active_reward_t::load_data(uint64_t role_uid, const proto::common::active_reward_data_single& single)
{
	m_uid = string_util_t::string_to_uint64(single.uid());
	m_key = cache_key_t::create(m_uid, cache_name::active_reward);
	m_tid = single.tid();
	m_type = single.type();
	m_count = single.count();
	m_event_id = single.event_id();
	m_is_finish = single.is_finish();
	m_is_grant = single.is_grant();
	m_role_uid = role_uid;
}


void active_reward_t::peek_data(proto::common::active_reward_data_single* p_single, bool is_client /* = true */)
{
	if (NULL == p_single)
	{
		log_error("NULL == p_single role[%lu]",m_role_uid);
		return;
	}

	///common
	{
		p_single->set_tid(m_tid);
		p_single->set_count(m_count);
		p_single->set_is_finish(m_is_finish);
		p_single->set_is_grant(m_is_grant);
	}

	if (!is_client)
	{
		p_single->set_uid(string_util_t::uint64_to_string(m_uid));

		p_single->set_type(m_type);
		p_single->set_event_id(m_event_id);
	}
}


void active_reward_t::save_self(uint64_t role_uid, bool is_new /*= false*/)
{
	proto::common::active_reward_data_single single;
	peek_data(&single, false);
	role_unify_save::add_task(m_key, single, is_new, false, role_uid);
}


void active_reward_t::set_count(const uint32_t val)
{
	ActiveReward* p_config =  GET_CONF(ActiveReward, m_tid);
	if (NULL ==  p_config)
	{
		log_error("NULL ==  p_config id[%d]",m_tid);
		return;
	}
	const uint32_t target = achieve_common_t::get_target_count(achieve_common_active_reward, m_tid);

	m_count = val > target ? target : val;
}

bool active_reward_t::compare_is_same(const uint32_t id, const uint32_t type, const uint32_t event_id)
{
	if (id == m_tid && type != m_type && event_id != m_event_id)
		return true;
	return false;
}

