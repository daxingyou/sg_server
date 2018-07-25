#ifndef __SHOP_DATA_MGR_HPP__
#define __SHOP_DATA_MGR_HPP__

#include "shop/shop_manager.hpp"
#include "shop/shop_user_data.hpp"

class shop_data_mgr_t
{
public:
	// 加载所有限购物品数据
	static bool load_shop_goods(shop_manager_t::goods_map& res);
	// 保存一条数据
	static void save_shop_goods(const shop_manager_t::shop_goods_ptr& p_goods);

public:
	// 加载所有用户物品数据
	static void load_user_goods(uint64_t uid, shop_user_data_t::shop_user_map& res);
	// 保存一条数据
	static void save_user_goods(uint64_t uid, uint32_t tid, const shop_user_data_t::shop_user_ptr& p_goods);
};

#endif//__SHOP_DATA_MGR_HPP__