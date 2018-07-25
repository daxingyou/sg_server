#ifndef __DB_RANDOM_SHOP_DATA_MANAGER__
#define __DB_RANDOM_SHOP_DATA_MANAGER__

#include "common/macros.hpp"
#include "protos_fwd.hpp"

class random_shop_manager
{
public:
	static bool save_random_shop_to_redis(uint64_t uid, const proto::common::random_shop_data& data);
	static bool load_random_shop_from_redis(uint64_t uid, proto::common::role_random_shop_data* data);
};

#endif //__DB_RANDOM_SHOP_DATA_MANAGER__
