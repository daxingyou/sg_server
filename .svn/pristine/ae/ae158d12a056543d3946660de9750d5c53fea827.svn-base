#ifndef __HERO_DATA_MANAGER__
#define __HERO_DATA_MANAGER__

#include "common/macros.hpp"
#include "protos_fwd.hpp"

class hero_data_manager
{
public:
	hero_data_manager();
	~hero_data_manager();

	static bool save_hero_data_to_redis(uint64_t role_uid, const proto::common::hero_single& single);
	static bool load_hero_data_from_redis(uint64_t role_uid, proto::common::hero_data* data);
};

#endif//__HERO_DATA_MANAGER__