#ifndef __GAME_SHOP_MANAGER_H__
#define __GAME_SHOP_MANAGER_H__

#include "macros.hpp"

#include "server.pb.h"
#include "client.pb.h"
#include "common.pb.h"

#include "role/role.hpp"

// 购买验证
enum em_goods_check_type
{
	goods_check_type_center = 0,	// 表示从center过来
	goods_check_type_game = 1,
};

class shop_manager_t
{
public:
	// 检测能不能买某个商品，没有限制的直接扣钱给东西，有限制的通知center计数
	static uint32_t check_user_buy_goods(const role_ptr& p_role, const proto::common::user_buy_goods& info, em_goods_check_type type, uint32_t buy_count, proto::common::role_change_data* p_data);

	// center验证不通过还钱
	static bool back_role_money(role_ptr p_role, const proto::common::user_buy_goods& info, uint32_t buy_count);

	// center验证给道具
	static uint32_t give_role_item(role_ptr p_role, const proto::common::user_buy_goods& info, uint32_t buy_count, proto::common::role_change_data* p_data);

	// 玩家购买了一个商品
	static void role_buy_one_goods(uint64_t uid, uint32_t shop_type, uint32_t goods_id, uint32_t goods_num, uint32_t cost_type, uint32_t cost_money);

	//------------------------------------寄售行分隔符-----------------------------------------------
	//请求购买寄售商品，先扣钱，然后去center购买
	static uint32_t goods_buy_request(role_ptr p_role, uint64_t goods_uid, uint32_t item_tid, uint32_t buy_count, uint32_t price, const proto::common::ex_reason *p_reason = NULL );
	//处理center回复的购买返回消息
	static void goods_buy_reply(role_ptr p_role, const proto::server::eg_goods_buy_reply& msg, proto::client::gc_goods_buy_reply& reply);

	//请求寄售商品，先扣物品和手续费，然后去center寄售
	static uint32_t goods_sell_request(role_ptr p_role, uint64_t item_uid, uint32_t sell_count, uint32_t price);
	//处理center回复的出售返回消息
	static void goods_sell_reply(role_ptr p_role, const proto::server::eg_goods_sell_reply& msg, proto::client::gc_goods_sell_reply& reply);
	//寄售手续费
	static uint32_t get_sell_cost(uint32_t sell_count, uint32_t price);
	//请求取回寄售商品
	static uint32_t goods_return_request(role_ptr p_role, uint64_t goods_uid, uint32_t item_tid, uint32_t item_num);
	//处理center回复的取回寄售商品返回消息
	static void goods_return_reply(role_ptr p_role, const proto::common::goods_item& item_info, proto::common::role_change_data* p_data);
	//请求重新上架商品，先扣手续费，然后去center重新上架
	static uint32_t goods_resell_request(role_ptr p_role, uint64_t goods_uid, uint32_t sell_count, uint32_t price);
	//处理center回复的重新上架返回消息
	static void goods_resell_reply(role_ptr p_role, const proto::server::eg_goods_resell_reply& msg, proto::client::gc_goods_resell_reply& reply);
	//请求领取收益
	static void goods_profit_request(uint64_t uid);
	//处理center回复的领取收益返回消息
	static void goods_profit_reply(role_ptr p_role, uint32_t profit, proto::common::role_change_data* p_data);

	//购买指定tid的道具(用于任务自动完成)
	static uint32_t auto_buy_task_item( role_ptr p_role, uint32_t task_id );
	//reason 1 表示任务，ex_id 表示任务id
	static void destine_buy_request(role_ptr p_role, uint32_t item_tid, uint32_t buy_count, uint32_t reason, uint32_t ex_id);
	//预定返回
	static uint32_t destine_buy_reply( role_ptr p_role, uint64_t goods_uid, uint32_t item_tid, uint32_t buy_count, uint32_t price, const proto::common::ex_reason &reason);
private:
	static uint32_t get_item_cooldown();
	static uint32_t get_rarity_cooldown();
};

#endif
