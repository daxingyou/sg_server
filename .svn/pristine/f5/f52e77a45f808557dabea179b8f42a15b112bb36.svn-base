#ifndef __DB_DUNGEON_DATA_MANAGER__
#define __DB_DUNGEON_DATA_MANAGER__

#include "common/macros.hpp"
#include "protos_fwd.hpp"

class dungeon_data_manager
{
public:
	static bool save_dungeon_data_to_redis(uint64_t uid, const proto::common::role_dungeon_data& data);
	static bool load_dungeon_data_from_redis(uint64_t uid, proto::common::role_all_dungeon_data* data);
};

#endif //__DB_DUNGEON_DATA_MANAGER__
