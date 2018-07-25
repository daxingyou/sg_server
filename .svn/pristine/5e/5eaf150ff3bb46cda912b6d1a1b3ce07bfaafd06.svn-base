#pragma once

#include "common/macros.hpp"
#include "protos_fwd.hpp"

class active_reward_data_manager
{
public:
	active_reward_data_manager() {}
	~active_reward_data_manager(){}

	/*static bool save_active_reward_data_to_redis(uint64_t uid, const proto::common::active_reward_data_single& data);
	static bool load_active_reward_data_from_redis(uint64_t uid, proto::common::active_reward_data* data);*/

	static bool save_active_reward_data_to_redis(uint64_t uid, const proto::common::active_reward_data& data);
	static bool load_active_reward_data_from_redis(uint64_t uid, proto::common::active_reward_data* p_data);
};
