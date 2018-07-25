#include "random_shop_manager.hpp"
#include "utility.hpp"
#include "common/redis_client.hpp"

bool random_shop_manager::save_random_shop_to_redis(uint64_t uid, const proto::common::random_shop_data& data)
{
	std::ostringstream key;
	key << uid << data.shop_id();

	std::ostringstream ss;
	for (int32_t i = 0; i < data.items_size(); ++i)
	{
		if (!ss.str().empty())
		{
			ss << "$";
		}

		ss << data.items(i).shop_item() << ":" << (uint32_t)data.items(i).state();
	}

	redis_client_t::set_uint64("random_shop", "role_uid", key.str(), uid);
	redis_client_t::set_uint32("random_shop", "shop_id", key.str(), data.shop_id());
	redis_client_t::set_uint32("random_shop", "free_refresh", key.str(), data.free_refresh());
	redis_client_t::set_uint32("random_shop", "pay_refresh", key.str(), data.pay_refresh());
	redis_client_t::set_uint32("random_shop", "next_refresh_time", key.str(), data.next_refresh_time());
	redis_client_t::set_string("random_shop", "shop_item", key.str(), ss.str());

	return true;
}

bool random_shop_manager::load_random_shop_from_redis(uint64_t uid, proto::common::role_random_shop_data* data)
{
	if (data == NULL)
	{
		log_error("load_random_shop_from_redis [%lu] data == null", uid);
		return false;
	}

	std::vector<std::string> keys;
	redis_client_t::get_keys_by_field_condition<std::string, uint64_t, std::vector<std::string>>("random_shop", "role_uid", uid, keys);

	for (const auto& key : keys)
	{
		proto::common::random_shop_data* p_shop_data = data->add_datas();
		if (NULL == p_shop_data)
		{
			continue;
		}

		uint32_t shop_id = redis_client_t::get_uint32("random_shop", "shop_id", key);
		p_shop_data->set_shop_id(shop_id);
		p_shop_data->set_free_refresh(redis_client_t::get_uint32("random_shop", "free_refresh", key));
		p_shop_data->set_pay_refresh(redis_client_t::get_uint32("random_shop", "pay_refresh", key));
		p_shop_data->set_next_refresh_time(redis_client_t::get_uint32("random_shop", "next_refresh_time", key));

		std::string shop_items = redis_client_t::get_string("random_shop", "shop_item", key);
		if (!shop_items.empty())
		{
			std::vector<std::string> items_vec;
			common::string_util_t::split<std::string>(shop_items, items_vec, "$");

			std::vector<uint32_t> item_vec;

			for (const auto& item : items_vec)
			{
				common::string_util_t::split<uint32_t>(item, item_vec, ":");

				if (item_vec.size() != 2)
				{
					log_error("role[%lu] load shop_item_data error, shop_item[%u]", uid, shop_id);
					continue;
				}

				proto::common::random_shop_item* p_item = p_shop_data->add_items();
				if (NULL != p_item)
				{
					p_item->set_shop_item(item_vec[0]);

					if (proto::common::random_shop_state_IsValid(item_vec[1]))
					{
						p_item->set_state((proto::common::random_shop_state)item_vec[1]);
					}
				}
			}
		}
	}

	return true;
}
