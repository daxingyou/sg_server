#pragma once
#include "common/macros.hpp"
#include "protos_fwd.hpp"

class family_skill_data_manager
{
public:
	family_skill_data_manager();
	~family_skill_data_manager();

	static bool save_family_skill_data_to_redis(uint64_t uid, const proto::common::family_skill_data& ld);
	static bool load_family_skill_data_from_redis(uint64_t uid, proto::common::family_skill_data* p_data);
};