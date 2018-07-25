#include "active_reward_data_manager.hpp"
#include "common/redis_client.hpp"
#include "config_mgr.h"

USING_NS_COMMON;
bool active_reward_data_manager::save_active_reward_data_to_redis(uint64_t uid, const proto::common::active_reward_data_single& single)
{
	redis_client_t::set_uint64("active_reward", "role_uid", single.uid(), uid);
	redis_client_t::set_uint32("active_reward", "tid", single.uid(), single.tid());
	redis_client_t::set_uint32("active_reward", "type", single.uid(), single.type());
	redis_client_t::set_uint32("active_reward", "event_id", single.uid(), single.event_id());
	redis_client_t::set_uint32("active_reward", "count", single.uid(), single.count());
	redis_client_t::set_uint32("active_reward", "is_finish", single.uid(), single.is_finish());
	redis_client_t::set_uint32("active_reward", "is_grant", single.uid(), single.is_grant());

	return true;
}

bool active_reward_data_manager::load_active_reward_data_from_redis(uint64_t role_uid, proto::common::active_reward_data* data)
{
	if (NULL == data)
	{
		log_error("load_active_reward_data_from_redis [%lu] data == null", role_uid);
		return false;
	}

	std::vector<uint64_t> active_reward_uids;
	redis_client_t::get_keys_by_field_condition<uint64_t, uint64_t, std::vector<uint64_t>>("active_reward", "role_uid", role_uid, active_reward_uids);

	for (auto& active_reward_uid : active_reward_uids)
	{
		std::string uid = string_util_t::uint64_to_string(active_reward_uid);
		uint32_t tid = redis_client_t::get_uint32("active_reward", "tid", active_reward_uid);
		uint32_t type = redis_client_t::get_uint32("active_reward", "type", active_reward_uid);
		uint32_t event_id = redis_client_t::get_uint32("active_reward", "event_id", active_reward_uid);
		uint32_t count = redis_client_t::get_uint32("active_reward", "count", active_reward_uid);
		uint32_t is_finish = redis_client_t::get_uint32("active_reward", "is_finish", active_reward_uid);
		uint32_t is_grant = redis_client_t::get_uint32("active_reward", "is_grant", active_reward_uid);
	
		proto::common::active_reward_data_single* p_single = data->add_active_reward();
		if (p_single == NULL)
			continue;

		p_single->set_uid(uid);
		p_single->set_tid(tid);
		p_single->set_type(type);
		p_single->set_count(count);
		p_single->set_event_id(event_id);
		p_single->set_is_finish(is_finish);
		p_single->set_is_grant(is_grant);
	}

	return true;
}

