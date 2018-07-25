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
};

