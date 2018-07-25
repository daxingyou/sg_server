#pragma once
#include "common/macros.hpp"
#include "protos_fwd.hpp"

class office_data_manager
{
public:
	office_data_manager();
	~office_data_manager();

	static bool save_office_data_to_redis(uint64_t uid, const proto::common::office_data& rd);
	static bool load_office_data_from_redis(uint64_t uid, proto::common::office_data* p_data);
};