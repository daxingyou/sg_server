#pragma once

#include "common/macros.hpp"
#include "common.pb.h"
#include "server.pb.h"

class global_data_mgr_t
{
	// 每日更新时间记录
public:
	static uint32_t load_day_update_time();
	static void save_day_update_time(uint32_t update_time);

	// 家族战全局数据
public:
	static void load_family_war_data(proto::server::global_data_type type, proto::server::global_data_family_war& data);
	static void save_family_war_data(proto::server::global_data_type type, const proto::server::global_data_family_war& data);

	// 国家全局数据
public:
	static void load_country_data(proto::server::global_data_type type, proto::server::global_data_country_data& data);
	static void save_country_data(proto::server::global_data_type type, const proto::server::global_data_country_data& data);

	// 王城战全局数据
public:
	static void load_king_war_data(proto::server::global_data_type type, proto::server::global_data_king_war_data& data);
	static void save_king_war_data(proto::server::global_data_type type, const proto::server::global_data_king_war_data& data);
};