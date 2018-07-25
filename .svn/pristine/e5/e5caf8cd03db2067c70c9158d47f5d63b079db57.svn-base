#pragma once
#include "common/macros.hpp"
#include "protos_fwd.hpp"

class expedition_data_manager
{
public:
	expedition_data_manager();
	~expedition_data_manager();

	static bool save_expedition_data_to_redis(uint64_t uid, const proto::common::expedition_data& ed);
	static bool load_expedition_data_from_redis(uint64_t uid, proto::common::expedition_data* p_data);
};