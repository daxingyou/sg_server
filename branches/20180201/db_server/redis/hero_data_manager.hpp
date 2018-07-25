#ifndef __HERO_DATA_MANAGER__
#define __HERO_DATA_MANAGER__

#include "common/macros.hpp"
#include "protos_fwd.hpp"

class hero_data_manager
{
public:
	hero_data_manager();
	~hero_data_manager();

	static bool save_hero_data_to_redis(uint64_t role_uid, const proto::common::hero_single& data);
	static bool load_hero_data_from_redis(uint64_t role_uid, proto::common::hero_data* data);

	static bool save_talent_data_to_redis(uint64_t hero_uid, const proto::common::talent_data& td);
	static bool load_talent_data_from_redis(uint64_t hero_uid, proto::common::talent_data* td);
};

#endif//__HERO_DATA_MANAGER__