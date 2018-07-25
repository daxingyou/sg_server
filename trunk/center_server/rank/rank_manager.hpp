#pragma once

#include "rank_level.hpp"
#include "rank_tower.hpp"
#include "rank_criminal.hpp"
#include "rank_achieve.hpp"
#include "rank_family.hpp"
#include "rank_charm.hpp"
#include "rank_recruit.hpp"
#include "rank_copper.hpp"
#include "rank_gold.hpp"
#include "rank_power.hpp"
#include "server.pb.h"

class rank_manager_t
{
public:
	static bool init();

	static rank_sort_ptr get_rank_sort_by_type(uint32_t type);

	static uint32_t get_rank_by_uid(uint32_t type, uint64_t uid);

	static void update_rank(const proto::server::ge_update_rank_notify& notify);

	static void request_rank(const global_user_ptr& p_user, uint32_t rank_type, uint32_t page, proto::client::ec_rank_reply& reply);
	 
	static void request_rank_time(proto::client::ec_rank_time_reply& reply);
	//限时排行榜定时器
	static void on_time_rank(uint32_t id, bool is_in_time);
	
public:
	// 等级榜
	static rank_level_ptr get_rank_level();
	static void update_rank_level(uint64_t uid, uint32_t lv);
	//魅力榜
	static rank_charm_ptr get_rank_charm();
	static void update_rank_charm(uint64_t uid, uint32_t charm);
	//战力榜
	static rank_power_ptr get_rank_power();
	static void update_rank_power(uint64_t uid, uint32_t power);
public:
	// 家族榜
	static rank_family_ptr get_rank_family();
	static void update_rank_family(uint64_t family_id, uint32_t prestige);
	static void rank_del_family(uint64_t family_id);

protected:
	typedef std::map<uint32_t, rank_sort_ptr> rank_sort_map;
	static rank_sort_map m_rank_sort_map;
};