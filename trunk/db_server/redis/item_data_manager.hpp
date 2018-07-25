#ifndef __ITEM_DATA_MANAGER__
#define __ITEM_DATA_MANAGER__

#include "common/macros.hpp"
#include "protos_fwd.hpp"

#define equip_prefix 21
class item_data_manager
{
public:
	item_data_manager();
	~item_data_manager();

	static bool save_item_data_to_redis(uint64_t uid, const proto::common::item_single& single);
	static bool load_item_data_from_redis(uint64_t uid, proto::common::item_data* data);
private:
	static bool is_equip(uint32_t tid);
};

#endif//__ITEM_DATA_MANAGER__