#include "trade_item_data_manager.hpp"
#include "redis_client.hpp"
#include "common/config_mgr.h"
#include "tblh/Item.tbls.h"

trade_item_data_manager::trade_item_data_manager()
{
}

trade_item_data_manager::~trade_item_data_manager()
{
}

bool trade_item_data_manager::save_trade_item_data_to_redis(uint64_t uid, const proto::common::role_trade_item& trade_item_data)
{
	std::ostringstream ss;

	for (int i = 0; i < trade_item_data.trade_item_list_size(); ++i)
	{
		ss.str("");
		ss << uid << trade_item_data.trade_item_list(i).item_tid();

		REDIS_BATCH_START
		redis_client_t::set_uint64("role_trade_item", "role_uid", ss.str(), uid);
		redis_client_t::set_uint32("role_trade_item", "item_tid", ss.str(), trade_item_data.trade_item_list(i).item_tid());
		redis_client_t::set_uint32("role_trade_item", "item_buy_num", ss.str(), trade_item_data.trade_item_list(i).item_buy_num());
		redis_client_t::set_uint32("role_trade_item", "item_buy_price", ss.str(), trade_item_data.trade_item_list(i).item_buy_price());
		redis_client_t::set_uint32("role_trade_item", "item_max_buy_num", ss.str(), trade_item_data.trade_item_list(i).item_max_buy_num());
		redis_client_t::set_uint32("role_trade_item", "item_max_buy_num_addi", ss.str(), trade_item_data.trade_item_list(i).item_max_buy_num_addi());
		REDIS_BATCH_DONE
	}

	return true;
}

bool trade_item_data_manager::load_trade_item_data_from_redis(uint64_t uid, proto::common::role_trade_item* trade_item_data)
{
	if (trade_item_data == NULL)
	{
		log_error("load_trade_item_data_from_redis [%lu] data == null", uid);
		return false;
	}

	std::vector<std::string> trade_item_tids;
	redis_client_t::get_keys_by_field_condition<std::string, uint64_t, std::vector<std::string>>("role_trade_item", "role_uid", uid, trade_item_tids);

	for (auto trade_item_tid : trade_item_tids)
	{
		uint32_t item_tid = redis_client_t::get_uint32("role_trade_item", "item_tid", trade_item_tid);
		if (NULL == GET_CONF(Item, item_tid))
		{
			log_warn("[wys] not find this item:%d", item_tid);
			continue;
		}
		// 购买数等于0的认为已经没有这个贸易物品了 优化掉 不往game发
		uint32_t item_buy_num = redis_client_t::get_uint32("role_trade_item", "item_buy_num", trade_item_tid);
		if(0 == item_buy_num)
			continue;
		proto::common::role_trade_item_single* trade_item = trade_item_data->add_trade_item_list();
		if (trade_item == NULL)
			continue;

		trade_item->set_item_tid(item_tid);
		trade_item->set_item_buy_num(item_buy_num);
		trade_item->set_item_max_buy_num(redis_client_t::get_uint32("role_trade_item", "item_max_buy_num", trade_item_tid));
		trade_item->set_item_max_buy_num_addi(redis_client_t::get_uint32("role_trade_item", "item_max_buy_num_addi", trade_item_tid));
		trade_item->set_item_buy_price(redis_client_t::get_uint32("role_trade_item", "item_buy_price", trade_item_tid));

	}

	return true;
}
