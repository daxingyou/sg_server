#pragma once

#include "protos_fwd.hpp"
#include "utility.hpp"
#include "role/role.hpp"
#include "tblh/PatrolHire.tbls.h"

class patrol_hire_manager_t
{
public:
	static uint32_t get_patrol_hire_reward(const role_ptr& p_role, proto::client::patrol_hire_rwd* p_rwd);
	static void fast_patrol_hire(const role_ptr& p_role, uint32_t map_id, proto::client::gc_fast_patrol_hire_reply& reply);
	static void change_patrol_hire_map(const role_ptr& p_role, uint32_t map_id, proto::client::gc_change_patrol_hire_reply& reply);

private:
	// 得到星级加成
	static uint32_t get_star_add(uint32_t star);
	// 发送奖励
	static void send_patrol_hire_reward(const role_ptr& p_role, PatrolHire* p_conf, uint32_t count, proto::common::role_change_data* p_rcd);
};

