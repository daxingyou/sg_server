#include "shop_data_mgr.hpp"

#include "log.hpp"
#include "redis_client.hpp"
#include "config_mgr.h"
#include "tblh/ShopTable.tbls.h"

bool shop_data_mgr_t::load_shop_goods(shop_manager_t::goods_map& res)
{
	res.clear();

	std::vector<uint32_t> all_goods;
	redis_client_t::get_all_keys<uint32_t, std::vector<uint32_t>>("shop_goods", all_goods);

	for (auto goods_id : all_goods)
	{
		ShopTable* p_conf = GET_CONF(ShopTable, goods_id);
		if (NULL == p_conf)
		{
			log_warn("load_shop_goods ShopTable id[%u] not exsit", goods_id);	// 可能这个商品已经不在配置表了
			continue;
		}

		// 服务器不限购，并且没有上架类型,可能已经有限购现在不限购了，抛弃这些记录
		if (p_conf->server_limit() == 0 && p_conf->puton_type() == shop_puton_type::shop_puton_type_none)
		{
			continue;
		}

		shop_manager_t::shop_goods_ptr p_goods = shop_manager_t::shop_goods_ptr(new shop_goods_t());
		if (NULL == p_goods)
		{
			log_error("p_goods null error");
			continue;
		}

		p_goods->m_id = goods_id;
		// 类型
		p_goods->m_shoptype = p_conf->shop_type();
		// 限购刷新方式
		p_goods->m_refresh_type = (proto::common::shop_refresh_type)p_conf->refresh_type();
		// 记录一下刷新的id
		p_goods->m_puton_time = p_conf->puton_time();

		p_goods->m_id = goods_id;
		p_goods->m_buy_count = redis_client_t::get_uint32("shop_goods", "buy_count", goods_id);
		p_goods->m_start_time = redis_client_t::get_uint32("shop_goods", "start_time", goods_id);
		p_goods->m_end_time = redis_client_t::get_uint32("shop_goods", "end_time", goods_id);
		p_goods->m_last_refresh_time = redis_client_t::get_uint32("shop_goods", "refresh_time", goods_id);

		//log_warn("goods[%u, %u] time[%u, %u] last_refresh[%u]", p_goods->m_id, p_goods->m_buy_count, p_goods->m_start_time, p_goods->m_end_time, p_goods->m_last_refresh_time);

		res.insert(std::make_pair(goods_id, p_goods));
	}

	return true;
}

void shop_data_mgr_t::save_shop_goods(const shop_manager_t::shop_goods_ptr& p_goods)
{
	if (NULL == p_goods)
	{
		log_error("p_goods null error");
		return;
	}

	redis_client_t::set_uint32("shop_goods", "buy_count", p_goods->m_id, p_goods->m_buy_count);
	redis_client_t::set_uint32("shop_goods", "start_time", p_goods->m_id, p_goods->m_start_time);
	redis_client_t::set_uint32("shop_goods", "end_time", p_goods->m_id, p_goods->m_end_time);
	redis_client_t::set_uint32("shop_goods", "refresh_time", p_goods->m_id, p_goods->m_last_refresh_time);
}

void shop_data_mgr_t::load_user_goods(uint64_t uid, shop_user_data_t::shop_user_map& res)
{
	res.clear();

	std::vector<std::string> keys;
	redis_client_t::get_keys_by_field_condition<std::string, uint64_t, std::vector<std::string>>("user_goods", "role_uid", uid, keys);

	for (const auto& key : keys)
	{
		uint32_t goods_id = redis_client_t::get_uint32("user_goods", "goods_tid", key);

		ShopTable* p_conf = GET_CONF(ShopTable, goods_id);
		if (NULL == p_conf)
		{
			log_warn("load_shop_goods ShopTable id[%u] not exsit", goods_id);	// 可能这个商品已经不在配置表了
			continue;
		}

		// 个人不限购
		if (p_conf->presonal_limit() == 0)
		{
			redis_client_t::remove_key("user_goods", key);
			continue;
		}

		shop_user_data_t::shop_user_ptr p_goods = shop_user_data_t::shop_user_ptr(new shop_user_t());
		if (NULL == p_goods)
		{
			log_error("p_goods null error");
			continue;
		}

		// 类型
		p_goods->m_shoptype = p_conf->shop_type();

		p_goods->m_user_goods.set_goods_id(goods_id);
		p_goods->m_user_goods.set_buy_count(redis_client_t::get_uint32("user_goods", "buy_count", key));
		p_goods->m_user_goods.set_last_refresh_time(redis_client_t::get_uint32("user_goods", "refresh_time", key));

		res.insert(std::make_pair(goods_id, p_goods));
	}
}

void shop_data_mgr_t::save_user_goods(uint64_t uid, uint32_t tid, const shop_user_data_t::shop_user_ptr& p_goods)
{
	if (NULL == p_goods)
	{
		log_error("p_goods null error");
		return;
	}

	std::string rk;
	std::ostringstream oss;
	oss << uid << tid;
	rk = oss.str();

	redis_client_t::set_uint32("user_goods", "goods_tid", rk, tid);
	redis_client_t::set_uint64("user_goods", "role_uid", rk, uid);
	redis_client_t::set_uint32("user_goods", "buy_count", rk, p_goods->m_user_goods.buy_count());
	redis_client_t::set_uint32("user_goods", "refresh_time", rk, p_goods->m_user_goods.last_refresh_time());
}
