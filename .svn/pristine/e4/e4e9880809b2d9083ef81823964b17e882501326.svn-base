#ifndef __CENTER_SHOP_MANAGER_HPP__
#define __CENTER_SHOP_MANAGER_HPP__

#include <boost/shared_ptr.hpp>
#include <map>
#include <set>

#include "shop_common.hpp"
#include "tblh/ShopTable.tbls.h"

#include "common.pb.h"
#include "client.pb.h"
#include "server.pb.h"
#include "user/global_user.hpp"

class shop_manager_t
{
public:
	// 商店所有特殊物品（包括限购、有上架方式）
	typedef boost::shared_ptr<shop_goods_t> shop_goods_ptr;
	typedef std::map<uint32_t, shop_goods_ptr> goods_map;
	// 商店所有限购物品列表（为了加速发消息）
	typedef std::map<uint32_t, proto::common::shop_goods_list> goods_list_map;
	// 上架的定时器回调
	typedef std::map<uint32_t, std::set<uint32_t>> puton_timer_map;

public:
	// 加载所有商店数据
	static bool init();
	// 清理
	static bool close();
	// 重新加载表
	static bool reload();

public:
	static shop_goods_ptr get_shop_goods(uint32_t id);

	// 更新一个特殊商品，限购时间,上架时间等
	static shop_goods_ptr update_goods_time(uint32_t id, uint32_t time_now, ShopTable* p_shop_conf);
	// 得到一个更新时间
	static uint32_t get_next_refresh_time(proto::common::shop_refresh_type type);

public:
	// 注册上架定时器(如果是重新加载需要清理)
	static void register_puton_timer();
	// 清理已经注册的定时器
	static void clear_register_puton_timer();

	// 每天刷新的回调
	static void day_refresh_callback(uint32_t id, bool is_in_time);
	// 每周
	static void week_refresh_callback(uint32_t id, bool is_in_time);
	// 每月
	static void month_refresh_callback(uint32_t id, bool is_in_time);

	// 上架时间的回调
	static void puton_timer_callback(uint32_t id, bool is_in_time);

	// 刷新列表中的信息
	static void update_list_info(const shop_goods_ptr p_shop_goods);

public:
	// 得到商品列表
	static bool get_shop_list_by_type(uint32_t type, proto::client::ec_shop_goods_reply& reply);

	// 用户购买某个商品
	static uint32_t user_want_buy_goods(global_user_ptr p_user, const proto::common::user_buy_goods& info, proto::client::ec_buy_goods_reply& reply);

	// game 验证通过，计数
	static uint32_t user_game_check_back(global_user_ptr p_user, const proto::common::user_buy_goods& info, proto::server::eg_give_user_goods_notify& notify);

public:
	static goods_map		m_goods_map;		// <货物id, 货物数据>
	static goods_list_map	m_goods_list_map;	// <货物类型, 该货物数据> 加速消息组织
	static puton_timer_map	m_puton_time_map;	// 上架的定时器
};


#endif//__SHOP_MANAGER_HPP__
