#pragma once
#include "common/macros.hpp"
#include "protos_fwd.hpp"

class tower_data_manager
{
public:
	tower_data_manager();
	~tower_data_manager();

	static bool save_tower_data_to_redis(uint64_t uid, const proto::common::tower_data& td);
	static bool load_tower_data_from_redis(uint64_t uid, proto::common::tower_data* p_data);
};