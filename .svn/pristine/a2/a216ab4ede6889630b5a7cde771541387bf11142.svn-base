#pragma once
#include "common/macros.hpp"
#include "protos_fwd.hpp"

class luckydraw_data_manager
{
public:
	luckydraw_data_manager();
	~luckydraw_data_manager();

	static bool save_luckydraw_data_to_redis(uint64_t uid, const proto::common::luckydraw_data& ld);
	static bool load_luckydraw_data_from_redis(uint64_t uid, proto::common::luckydraw_data* p_data);
};