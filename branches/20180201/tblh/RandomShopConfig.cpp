#include "RandomShopConfig.hpp"
#include "config/config_manager.hpp"
#include "log.hpp"
#include "ShopItemTable.tbls.h"

RandomShopConfig_t::random_shop_pools RandomShopConfig_t::m_shop_pools;

RandomShopConfig_t::pool_items RandomShopConfig_t::_default_pool_items;

const RandomShopConfig_t::pool_items& RandomShopConfig_t::get_pool_items_by_id(uint32_t pool_id)
{
	random_shop_pools::const_iterator citr = m_shop_pools.find(pool_id);
	if (citr != m_shop_pools.end())
	{
		return citr->second;
	}

	return _default_pool_items;
}

void RandomShopConfig_t::add_pool_item(ShopItemTable* p_shop_item)
{
	if (p_shop_item)
	{
		m_shop_pools[p_shop_item->pool_id()].push_back(item_info(p_shop_item->id(), p_shop_item->lv_min(), p_shop_item->lv_max(), p_shop_item->proportion()));
	}
}

uint32_t RandomShopConfig_t::get_random_item(uint32_t lv, const RandomShopTable::shop_pool_vec& pool_vec)
{
	if (pool_vec.empty())
	{
		log_error("pool_vec is empty");
		return 0;
	}

	uint32_t rate = 1;
	std::vector<std::pair<uint32_t, uint32_t>> rate_vec;

	for (const auto& pool : pool_vec)
	{
		const pool_items& items = get_pool_items_by_id(pool);

		if (!items.empty())
		{
			for (const auto& item : items)
			{
				if (item.m_min_lv <= lv && lv <= item.m_max_lv && item.m_proportion > 0)
				{
					rate_vec.push_back(std::pair<uint32_t, uint32_t>(item.m_id, rate + item.m_proportion - 1));

					rate += item.m_proportion;
				}
			}
		}
	}

	if (rate_vec.empty())
	{
		log_error("have no rate ramdom shop_item");
		return 0;
	}

	int32_t rate_index = random_util_t::randBetween(1, rate - 1);
	if (rate_index == -1)
	{
		log_error("rand from 1 to [%u] error", rate - 1);
		return 0;
	}

	for (const auto& rate_item : rate_vec)
	{
		if (rate_index <= (int32_t)rate_item.second)
		{
			return rate_item.first;
		}
	}

	log_error("have no rate ramdom shop_item");
	return 0;
}

