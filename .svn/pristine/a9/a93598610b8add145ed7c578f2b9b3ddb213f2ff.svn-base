#ifndef __GAME_PK_MANAGER_H__
#define __GAME_PK_MANAGER_H__

#include "role/role.hpp"
#include "item/item.hpp"
#include "config/config_manager.hpp"

class pk_manager_t
{
public:
	static uint32_t do_pk(role_ptr p_role, role_ptr p_target, proto::common::role_change_data* p_data);
	static bool attacker_pk_result(uint64_t self_uid, uint64_t enemy_uid, bool is_win, proto::common::role_change_data* p_data);
	static bool defenser_pk_result(uint64_t self_uid, uint64_t enemy_uid, bool is_win, proto::common::role_change_data* p_data);
	//约战
	static uint32_t do_challenge(role_ptr p_role, uint64_t target_uid);
	//响应约战
	static uint32_t process_challenge(role_ptr p_role, uint32_t process);
	//开打
	static void challenge_fight(uint64_t challenger_uid, uint64_t target_uid);
public:
	//通缉犯PK值
	static uint32_t get_max_pk_value();
	//是否是通缉犯
	static bool is_criminal(uint32_t val);
private:
	//是否在安全区
	static bool is_safe(role_ptr p_role);
	//是否是通缉犯
	static bool is_criminal(role_ptr p_role);
	//是否是新手保护期
	static bool is_protected(role_ptr p_role);
	//PK道具
	static uint32_t get_pk_item();
	static uint32_t get_pk_item_num();
	//通缉犯PK值
	static uint32_t get_criminal_value();
	
	//击杀获得PK值
	static uint32_t get_pk_value_add();
	//最大等级差距
	static uint32_t get_delta_level();
	//PK最大距离
	static uint32_t get_pk_distance();
	
public:
	//降低PK值的时间
	static uint32_t get_pk_value_reduce_time();
	//每次降低多少PK值
	static uint32_t get_pk_value_reduce();
	//每日PK次数
	static uint32_t get_pk_count();
	//每日被PK次数
	static uint32_t get_target_count();
	//约战扣金币
	static uint32_t get_challenge_gold();
	//应战扣金币
	static uint32_t get_accept_challenge_gold();
	//进入监狱概率
	static uint32_t get_prison_rate();
	//皇城PK传送点ID
	static uint32_t get_challenge_point();
	//监狱地图ID
	static uint32_t get_prison_id();
	//监狱传送点ID
	static uint32_t get_prison_point();
	//获取侠义值奖励(根据目标的PK值)
	static uint32_t get_errantry(uint32_t pk_value);
	//获取经验惩罚(根据自己的PK值)
	static uint32_t get_exp_reduce(uint32_t pk_value);
	//监狱传送点ID(传出去的)
	static uint32_t get_prison_out_point();
	//获取攻击方失败被关进监狱的几率(根据自己的PK值)
	static uint32_t get_attacker_lose_prison_rate(uint32_t pk_value);
	//获取被攻击方失败被关进监狱的几率(根据自己的PK值)
	static uint32_t get_defenser_lose_prison_rate(uint32_t pk_value);
};

#endif
