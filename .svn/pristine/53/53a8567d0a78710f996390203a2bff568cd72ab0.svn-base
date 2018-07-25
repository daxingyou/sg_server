#pragma once
#include "role/role.hpp"
#include "protos_fwd.hpp"
/*
author:wys
date:2017.2.14
desc:竞技场
*/


class arena_manager_t
{
public:
	//初始化
	static bool init();
	// 赛季结束回调
	static void on_season(uint32_t id, bool is_in_time);
public:
	// 进入竞技场
	static uint32_t enter_arena(role_ptr p_role);
	// 离开竞技场
	static void leave_arena(const role_ptr& p_role);
	// 加入竞技场
	static uint32_t join_arena(role_ptr p_role, proto::common::role_change_data *p_data);
	// 购买门票
	static uint32_t buy_ticket(role_ptr p_role, proto::common::role_change_data *p_data);
	// 每日重置
	static void do_next_day(role_ptr p_role);
	// 匹配排位赛
	static uint32_t match_rank_for_battle(role_ptr p_role);
	// 取消匹配
	static void cancel_match(role_ptr p_role);
	// 匹配排位赛结果
	static void deal_match_rank_result(role_ptr p_role, proto::server::rg_arena_match_reply& msg);
	// 排位赛战斗结果
	static void deal_arena_rank_battle_result(role_ptr p_role, proto::server::rg_arena_battle_result_notify& msg);
	// 赛季结束
	static void arena_season_end();
private:
	// 获得奖励分(正负)
	static int32_t  get_battle_reward_score(uint64_t role_uid, uint32_t own_score, uint32_t target_score, bool is_win);
	// 通过分数获得最新段位
	static uint32_t get_level_by_score(bool is_win, uint32_t score, uint32_t arena_level);
	// 获取门票道具ID
	static uint32_t get_ticket_item();
};