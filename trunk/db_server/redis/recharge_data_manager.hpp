#pragma once
#include "common/macros.hpp"
#include "protos_fwd.hpp"

class recharge_data_manager
{
public:
	recharge_data_manager();
	~recharge_data_manager();

	static bool save_recharge_data_to_redis(uint64_t uid, const proto::common::recharge_data& rd);
	static bool load_recharge_data_from_redis(uint64_t uid, proto::common::recharge_data* p_data);
};