#pragma once
#include "common/macros.hpp"
#include <map>
#include <vector>
#include "RandomShopTable.tbls.h"

struct ShopItemTable;

class RandomShopConfig_t
{
public:
	struct item_info
	{
		uint32_t m_id = 0;
		uint32_t m_min_lv = 0;
		uint32_t m_max_lv = 0;
		uint32_t m_proportion = 0;

		item_info(uint32_t id, uint32_t min_lv, uint32_t max_lv, uint32_t proportion) : m_id(id), m_min_lv(min_lv), m_max_lv(max_lv), m_proportion(proportion) {};
	};

	typedef std::vector<item_info> pool_items;
	typedef std::map<uint32_t, pool_items> random_shop_pools;

	static random_shop_pools m_shop_pools;
	static pool_items _default_pool_items;

	static const pool_items& get_pool_items_by_id(uint32_t pool_id);

	static void add_pool_item(ShopItemTable* p_shop_item);

	static uint32_t get_random_item(uint32_t lv, const RandomShopTable::shop_pool_vec& pool_vec);
};
