#pragma once
#include "role/role.hpp"
#include "protos_fwd.hpp"
/*
author:wys
date:2017.2.14
desc:������
*/


class arena_manager_t
{
public:
	//��ʼ��
	static bool init();
	// ���������ص�
	static void on_season(uint32_t id, bool is_in_time);
public:
	// ���뾺����
	static uint32_t enter_arena(role_ptr p_role);
	// �뿪������
	static void leave_arena(const role_ptr& p_role);
	// ���뾺����
	static uint32_t join_arena(role_ptr p_role, proto::common::role_change_data *p_data);
	// ������Ʊ
	static uint32_t buy_ticket(role_ptr p_role, proto::common::role_change_data *p_data);
	// ÿ������
	static void do_next_day(role_ptr p_role);
	// ƥ����λ��
	static uint32_t match_rank_for_battle(role_ptr p_role);
	// ȡ��ƥ��
	static void cancel_match(role_ptr p_role);
	// ƥ����λ�����
	static void deal_match_rank_result(role_ptr p_role, proto::server::rg_arena_match_reply& msg);
	// ��λ��ս�����
	static void deal_arena_rank_battle_result(role_ptr p_role, proto::server::rg_arena_battle_result_notify& msg);
	// ��������
	static void arena_season_end();
private:
	// ��ý�����(����)
	static int32_t  get_battle_reward_score(uint64_t role_uid, uint32_t own_score, uint32_t target_score, bool is_win);
	// ͨ������������¶�λ
	static uint32_t get_level_by_score(bool is_win, uint32_t score, uint32_t arena_level);
	// ��ȡ��Ʊ����ID
	static uint32_t get_ticket_item();
};