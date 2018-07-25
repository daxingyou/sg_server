#pragma once
#include "common/macros.hpp"
#include "protos_fwd.hpp"

class patrol_hire_data_manager
{
public:
	static bool save_patrol_hire_data_to_redis(uint64_t uid, const proto::common::patrol_hire_data& td);
	static bool load_patrol_hire_data_from_redis(uint64_t uid, proto::common::patrol_hire_data* p_data);
};