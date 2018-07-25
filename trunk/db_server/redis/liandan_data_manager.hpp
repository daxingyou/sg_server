#pragma once
#include "common/macros.hpp"
#include "protos_fwd.hpp"

class liandan_data_manager
{
public:
	liandan_data_manager();
	~liandan_data_manager();

	static bool save_liandan_data_to_redis(uint64_t uid, const proto::common::liandan_data& ld);
	static bool load_liandan_data_from_redis(uint64_t uid, proto::common::liandan_data* p_data);
};