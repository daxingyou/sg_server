#pragma once
#include "common/macros.hpp"
#include "protos_fwd.hpp"

class equip_plan_data_manager
{
public:
	equip_plan_data_manager();
	~equip_plan_data_manager();

	static bool save_equip_plan_data_to_redis(uint64_t uid, const proto::common::equip_plan_data& ld);
	static bool load_equip_plan_data_from_redis(uint64_t uid, proto::common::equip_plan_data* p_data);
};