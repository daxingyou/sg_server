#pragma once
#include "common/macros.hpp"
#include "protos_fwd.hpp"

class activity_data_manager
{
public:
	activity_data_manager();
	~activity_data_manager();

	static bool save_activity_data_to_redis(uint64_t uid, const proto::common::activities_data& activities_data);
	static bool load_activity_data_from_redis(uint64_t uid, proto::common::activities_data* p_activities_data);
};