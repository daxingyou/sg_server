#pragma once

#include "common/macros.hpp"
#include "protos_fwd.hpp"


/*
分开存，一起读取设计
*/
class business_act_data_manager_t
{
public:
	static bool load_business_act_data_to_redis(uint64_t uid, proto::common::bussiness_act_data* p_list);

	//存储摇钱树数据
	static bool save_money_tree_data_to_redis(uint64_t uid, const proto::common::money_tree_data & list);

	//存储7日登陆数据
	static bool save_sevenday_login_data_to_redis(uint64_t uid, const proto::common::sevenday_login_save_data & list);

	//月卡存储
	static bool save_month_card_data_to_redis(uint64_t, const proto::common::month_card_save_data &list);

	//基金存储
	static bool save_level_fund_data_to_redis(uint64_t, const proto::common::level_fund_save_info  &list);

	//限购
	static bool save_limit_buy_data_to_redis(uint64_t, const proto::common::limit_buy_save_data  &list);

	//兑换
	static bool save_exchange_item_data_to_redis(uint64_t, const proto::common::exchange_item_save_data  &list);

	//七日目标存储
	static bool save_sevenday_target_data_to_redis(uint64_t, const proto::common::sd_target_save_data &list);

	//统计数据
	static bool save_statistical_data_to_redis(uint64_t uid, const proto::common::statistical_save_data &list);

	//累充奖励
	static bool save_recharger_total_data_to_redis(uint64_t uid, const proto::common::recharge_total_data &list);
	//黄金十连
	static bool save_gold_luckydraw_data_to_redis(uint64_t uid, const proto::common::gold_luckydraw_data &list);
};

