#ifndef __GAME_TOWER_MANAGER_H__
#define __GAME_TOWER_MANAGER_H__

#include "tower.hpp"
#include "protos_fwd.hpp"
#include "role/role.hpp"

class tower_manager_t
{

public:
	//手动爬塔
	static uint32_t fight(role_ptr p_role);
	//手动爬塔结果
	static void fight_result(role_ptr p_role, bool iswin, uint64_t trigger_uid, proto::common::role_change_data* rcd);
	//扫荡
	static uint32_t auto_fight(role_ptr p_role, uint32_t& auto_fight_time);
	//立即扫荡
	static uint32_t auto_fight_immediately(role_ptr p_role, uint32_t& auto_fight_time, proto::common::role_change_data* p_data);
	//领取扫荡奖励
	static uint32_t auto_fight_reward(role_ptr p_role, proto::common::tower_data* p_info, proto::common::role_change_data* p_data);
	//重置
	static uint32_t reset(role_ptr p_role, uint32_t& current_tower, uint32_t& reset_count);
	//领取成就奖励
	static uint32_t achieve_reward(role_ptr p_role, uint32_t& current_achieve, proto::common::role_change_data* p_data);
	//攻打隐藏关卡
	static uint32_t fight_trigger(role_ptr p_role, uint64_t trigger_uid);
private:
	static uint32_t get_auto_fight_immediately_gold();
};

#endif
