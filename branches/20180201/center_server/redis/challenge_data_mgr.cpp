#include "challenge_data_mgr.hpp"

#include "common/redis_client.hpp"
#include "utility.hpp"

USING_NS_COMMON;

void challenge_data_mgr_t::add_challenge(challenge_ptr p_challenge)
{
	if (NULL == p_challenge)
	{
		log_error("NULL == p_challenge");
		return;
	}
	uint64_t uid = p_challenge->get_uid();
	redis_client_t::set_uint64("challenge", "challenger_uid", uid, p_challenge->get_challenger_uid());
	redis_client_t::set_string("challenge", "challenger_name", uid, p_challenge->get_challenger_name());
	redis_client_t::set_uint64("challenge", "target_uid", uid, p_challenge->get_target_uid());
	redis_client_t::set_string("challenge", "target_name", uid, p_challenge->get_target_name());
	redis_client_t::set_uint32("challenge", "challenge_time", uid, p_challenge->get_challenge_time());
	redis_client_t::set_uint32("challenge", "state", uid, p_challenge->get_state());
}

void challenge_data_mgr_t::update_challenge_state(uint64_t uid, uint32_t state)
{
	redis_client_t::set_uint32("challenge", "state", uid, state);
}

void challenge_data_mgr_t::load_challenge_by_day(uint32_t day, challenge_map& challenge_data)
{
	std::vector<std::uint64_t> keys;
	redis_client_t::get_keys_by_field_condition<uint64_t, uint32_t, std::vector<std::uint64_t>>("challenge", "challenge_time", day, keys);
	
	for (auto key : keys)
	{
		uint32_t state = redis_client_t::get_uint32("challenge", "state", key);
		if (state != proto::common::challenge_state_win && state != proto::common::challenge_state_lose && state != proto::common::challenge_state_accept)
		{
			continue;
		}
		challenge_ptr p_challenge = challenge_ptr(new challenge_t);
		p_challenge->set_uid(key);
		p_challenge->set_challenger_uid(redis_client_t::get_uint64("challenge", "challenger_uid", key));
		p_challenge->set_challenger_name(redis_client_t::get_string("challenge", "challenger_name", key));
		p_challenge->set_target_uid(redis_client_t::get_uint64("challenge", "target_uid", key));
		p_challenge->set_target_name(redis_client_t::get_string("challenge", "target_name", key));
		p_challenge->set_challenge_time(day);
		p_challenge->set_state(state);
		challenge_data.insert(std::make_pair(key, p_challenge));
	}
}

void challenge_data_mgr_t::load_challenge_record(uint32_t day_count, uint32_t record_count, challenge_record& record)
{
	std::vector<std::uint64_t> keys;
	for (uint32_t i = 0; i < day_count; ++i)
	{
		uint32_t day = time_util_t::get_pass_day(i);
		std::vector<std::uint64_t> temp_keys;
		redis_client_t::get_keys_by_field_condition<uint64_t, uint32_t, std::vector<std::uint64_t>>("challenge", "challenge_time", day, temp_keys);
		if (!temp_keys.empty())
		{
			keys.insert(keys.begin(), temp_keys.begin(), temp_keys.end());
		}
		if (keys.size() >= record_count)
		{
			break;
		}
	}
	uint32_t start_index = keys.size() > record_count ? keys.size() - record_count : 0;
	for (uint32_t i = start_index; i < keys.size(); ++i)
	{
		uint64_t key = keys[i];
		uint32_t state = redis_client_t::get_uint32("challenge", "state", key);
		if (state != proto::common::challenge_state_win && state != proto::common::challenge_state_lose)
		{
			continue;
		}
		challenge_ptr p_challenge = challenge_ptr(new challenge_t);
		p_challenge->set_uid(key);
		p_challenge->set_challenger_uid(redis_client_t::get_uint64("challenge", "challenger_uid", key));
		p_challenge->set_challenger_name(redis_client_t::get_string("challenge", "challenger_name", key));
		p_challenge->set_target_uid(redis_client_t::get_uint64("challenge", "target_uid", key));
		p_challenge->set_target_name(redis_client_t::get_string("challenge", "target_name", key));
		p_challenge->set_challenge_time(redis_client_t::get_uint32("challenge", "challenge_time", key));
		p_challenge->set_state(state);
		record.push_back(p_challenge);
	}
}
