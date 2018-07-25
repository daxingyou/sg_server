#pragma once
#include "common/macros.hpp"
#include "protos_fwd.hpp"

class arena_data_manager
{
public:
	arena_data_manager();
	~arena_data_manager();

	static bool save_arena_data_to_redis(uint64_t uid, const proto::common::arena_role_data& arena_data);
	static bool load_arena_data_from_redis(uint64_t uid, proto::common::arena_role_data* arena_data);
};