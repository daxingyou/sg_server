#ifndef __GAME_INTERIOR_MANAGER_H__
#define __GAME_INTERIOR_MANAGER_H__

#include "interior.hpp"
#include "protos_fwd.hpp"

class interior_manager_t
{

public:
	static void get_build_copper(uint64_t uid);
	static void upgrade_build(uint64_t uid);
	static void dispatch_task(uint64_t uid, const proto::client::cg_dispatch_task_request& dtr);
	static void refresh_task(uint64_t uid);
	static void upgrade_task_star_level(uint64_t uid);
	static void get_interior_reward(uint64_t uid);
	static void complete_interior_task(uint64_t uid);
	static uint32_t calc_reward_time(uint64_t uid);
	static void notify_offline_reward_info(uint64_t uid, bool is_instant_battle = false);
	static void update_offline_reward_info(uint64_t uid);
	static void update_garrison_city(uint64_t uid);
};

#endif
