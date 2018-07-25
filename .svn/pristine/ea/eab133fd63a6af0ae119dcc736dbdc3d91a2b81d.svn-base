#pragma once
#include "common/macros.hpp"
#include "protos_fwd.hpp"

class country_bounty_data_manager
{
public:
	country_bounty_data_manager();
	~country_bounty_data_manager();

	static bool save_country_bounty_data_to_redis(uint64_t uid, const proto::common::country_bounty_data& country_bounty);
	static bool load_country_bounty_data_from_redis(uint64_t uid, proto::common::country_bounty_data* p_country_bounty);
};