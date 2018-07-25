#ifndef __ACHIEVE_DATA_MANAGER__
#define __ACHIEVE_DATA_MANAGER__

#include "common/macros.hpp"
#include "protos_fwd.hpp"

class achieve_data_manager
{
public:
	achieve_data_manager();
	~achieve_data_manager();

	static bool save_achieve_data_to_redis(uint64_t uid, const proto::common::achieve_data_single& data);
	static bool load_achieve_data_from_redis(uint64_t uid, proto::common::achieve_data* data);
};

#endif //__ACHIEVE_DATA_MANAGER__