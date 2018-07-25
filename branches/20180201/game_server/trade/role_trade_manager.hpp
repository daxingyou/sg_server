#pragma once
#include "role/role.hpp"
#include "city/city.hpp"
#include "trade/trade.hpp"
#include "protos_fwd.hpp"

/*
author: wys
time:2016.11.17
desc:role trade manager
*/


class role_trade_manager_t
{
public:
	role_trade_manager_t();
	~role_trade_manager_t();

	// 批量购买物品
	static uint32_t			trade_buy_all_items(role_ptr role, const proto::client::cg_trade_buy_item_request &msg_req,
		proto::client::gc_trade_buy_item_reply &msg_reply);
	// 批量出售物品
	static uint32_t			trade_sell_all_items(role_ptr role, const proto::client::cg_trade_sell_item_request &msg_req,
		proto::client::gc_trade_sell_item_reply &msg_reply);
	// 获取城市贸易商店数据
	static uint32_t			get_city_trade_shop_info(uint32_t city_id, role_ptr role, proto::client::gc_get_city_trade_shop_reply &reply);
	// 补货
	static uint32_t			trade_supplement_item(role_ptr role, uint32_t trade_id, uint64_t use_item_uid,
		proto::common::role_change_data *change_data,
		proto::common::city_trade_shop_info *p_data);
	// 获取所有城市贸易事件
	static uint32_t			get_all_city_trade_event(proto::client::gc_trade_event_reply &msg_reply,uint32_t type = 0);
private:
	// 购买单一物品
	static uint32_t			trade_buy_item(trade_ptr p_trade, role_ptr role, uint32_t item_tid,
		uint32_t item_num, proto::common::role_change_data *p_data);
	// 出售单一物品
	static uint32_t			trade_sell_item(trade_ptr p_trade, role_ptr role, uint32_t item_tid,
		uint32_t item_num, uint32_t trade_sell_tax,
		proto::common::role_change_data *p_data,uint32_t& total_gain);
	// 获取城市贸易商店数据
	static uint32_t			get_one_city_trade_shop_info(uint32_t trade_id, role_ptr role, proto::common::city_trade_shop_info *p_data,
		bool &b_is_need_reset_buy_num, bool b_is_need_send_all_item = false);

};
