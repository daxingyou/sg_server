#include "level_reward_manager.hpp"
#include "utility.hpp"
#include "common/redis_client.hpp"

bool level_reward_manager::save_level_reward_to_redis(uint64_t uid, const proto::common::level_reward_recived_list& list)
{
	std::ostringstream ss;
	for (int32_t i = 0; i < list.data_size(); ++i) {
		if (!ss.str().empty()) {
			ss << "$";
		}
		ss << list.data(i);
	}

	redis_client_t::set_string("role_ex", "level_reward", uid, ss.str());

	return true;
}

bool level_reward_manager::load_level_reward_to_redis(uint64_t uid, proto::common::level_reward_recived_list* p_list)
{
	if (!p_list) {
		log_error("load_level_reward_to_redis [%lu] data == null", uid);
		return false;
	}

	std::string str_list = redis_client_t::get_string("role_ex", "level_reward", uid);
	if (!str_list.empty()) {
		std::vector<std::string> list_vec;
		common::string_util_t::split<std::string>(str_list, list_vec, "$");

		for (const auto& item : list_vec) {
			p_list->add_data(common::string_util_t::string_to_uint32(item));
		}
	}

	return true;
}
