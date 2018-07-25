#ifndef __TRADE_ITEM_DATA_MANAGER__
#define __TRADE_ITEM_DATA_MANAGER__

#include "common/macros.hpp"
#include "protos_fwd.hpp"

class trade_item_data_manager
{
public:
	trade_item_data_manager();
	~trade_item_data_manager();

	static bool save_trade_item_data_to_redis(uint64_t uid, const proto::common::role_trade_item& trade_item_data);
	static bool load_trade_item_data_from_redis(uint64_t uid, proto::common::role_trade_item* trade_item_data);
};

#endif//__TRADE_ITEM_DATA_MANAGER__