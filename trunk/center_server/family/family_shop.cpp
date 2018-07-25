#include "family_shop.hpp"
#include "config/config_manager.hpp"
#include "time_manager.hpp"
#include "tblh/ShopItemTable.tbls.h"
#include "tblh/Comprehensive.tbls.h"
#include "tblh/RandomShopTable.tbls.h"
#include "redis/family_data_mgr.hpp"

family_shop_t::family_shop_t(uint64_t family_id)
	: m_family_id(family_id)
{

}

family_shop_t::~family_shop_t()
{

}

void family_shop_t::refresh_family_shop(uint32_t family_lv)
{
	Comprehensive* p_conf = GET_CONF(Comprehensive, comprehensive_common::family_shopid);
	uint32_t family_shopid = GET_COMPREHENSIVE_VALUE_1(p_conf);

	RandomShopTable* p_shop_table = GET_CONF(RandomShopTable, family_shopid);
	if (!p_shop_table) {
		log_error("family[%lu] random_shop[%u] not find", m_family_id, family_shopid);
		return;
	}

	uint32_t refresh_time = time_manager_t::get_next_refresh_time(p_shop_table->refresh_time());
	m_shop_data.set_next_refresh_time(refresh_time);

	m_shop_data.clear_items();
	const RandomShopTable::grid_vec& all_grids = p_shop_table->get_all_grids();
	for (uint32_t i = 0; i < all_grids.size(); ++i) {
		uint32_t item_id = RandomShopConfig_t::get_random_item(family_lv, all_grids[i]);
		ShopItemTable* p_shop_conf = GET_CONF(ShopItemTable, item_id);
		if (p_shop_conf) {
			proto::common::family_shop_item* p_shop_item = m_shop_data.add_items();
			if (p_shop_item) {
				p_shop_item->set_shop_item(item_id);
				p_shop_item->set_sold_count(0);
			}
		}
	}

	family_data_mgr_t::save_family_shop(m_family_id, m_shop_data);
}

void family_shop_t::add_shop_buy_count(uint32_t index, uint32_t num)
{
	if ((int32_t)index >= m_shop_data.items_size()) {
		log_error("family[%lu] family_shop index[%u] not find", m_family_id, index);
		return;
	}

	proto::common::family_shop_item* p_shop_item_data = m_shop_data.mutable_items(index);
	if (p_shop_item_data) {
		p_shop_item_data->set_sold_count(p_shop_item_data->sold_count() + num);
	}

	family_data_mgr_t::save_family_shop(m_family_id, m_shop_data);
}

void family_shop_t::shop_failed_back(uint32_t index, uint32_t num)
{
	if ((int32_t)index >= m_shop_data.items_size()) {
		log_error("family[%lu] family_shop index[%u] not find", m_family_id, index);
		return;
	}

	proto::common::family_shop_item* p_shop_item_data = m_shop_data.mutable_items(index);
	if (p_shop_item_data && p_shop_item_data->sold_count() >= num) {
		p_shop_item_data->set_sold_count(p_shop_item_data->sold_count() - num);
	}

	family_data_mgr_t::save_family_shop(m_family_id, m_shop_data);
}
