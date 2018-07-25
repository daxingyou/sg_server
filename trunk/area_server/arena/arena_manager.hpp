#pragma once

/*
author:wys
date:2017.2.15
desc:������
*/

#include "protos_fwd.hpp"
#include "role/arena_role.hpp"
#include "arena_ai.hpp"
#include <vector>

// ƥ���Ԥ����С
#define arena_match_pool_init_size 100
// ��̬������AI�������
#define DYNAMIC_AI_CREATE_MAX_NUM 500
//�������ID
#define activity_arena 3

typedef std::map<uint64_t, arena_role_ptr> ARENA_MATCH_POOL;

struct arena_match_pool
{
	ARENA_MATCH_POOL	m_arena_match_pool;
};

struct arena_ai_match_pool
{
	arena_ai_match_pool() { m_arena_ai_match_pool.reserve(arena_match_pool_init_size); }
	std::vector<arena_ai_ptr>	m_arena_ai_match_pool;
};

class arena_manager_t
{
public:
	typedef std::vector<arena_match_pool> match_pools;
	typedef std::vector<arena_ai_match_pool> ai_match_pools;
	typedef std::map<uint32_t, arena_ai_ptr> arena_ai_map;
	typedef std::vector<arena_ai_ptr> arena_ai_cache_vec;
public:
	static void		init();
	// ƥ��
	static uint32_t match(uint64_t role_uid, uint32_t score, uint32_t arena_level, 
						  const proto::common::role_cross_data &cross_data);
	// ��������ս�����
	static void		deal_arena_battle_result(proto::server::sr_arena_battle_result_notify &msg);
	// �����������
	static void		deal_role_logout(uint64_t role_uid);
	// ȡ��ƥ��
	static void		cancel_match(uint64_t role_uid);
	// ���������ص�
	static void on_season(uint32_t id, bool is_in_time);
	// ������ص�
	static void on_close(uint32_t id, bool is_in_time);
private:
	// ��ʼ��AI
	static void		init_ai(uint32_t min_score, uint32_t match_pool_interval_score);
	// ƥ��AI role_levelΪ��ɫ�ȼ� �Ǿ�������λ
	static void		match_ai(arena_role_ptr p_role, uint32_t min_score, uint32_t score, uint32_t role_level);
	// ƥ�����
	static void		match_role(arena_role_ptr p_role, uint32_t min_score, uint32_t match_max_num, uint32_t match_pool_interval_score);
	// ��ý�����(����)
	static int32_t  get_battle_reward_score(uint32_t own_score, uint32_t target_score, bool is_win);
	// ͨ������������¶�λ
	static uint32_t get_level_by_score(bool is_win, uint32_t score, uint32_t arena_level);

	// ��������ս�����
	static void		deal_arena_battle_obj_result(arena_role_ptr user, bool is_win, uint32_t target_score, uint32_t fighting_value,
												 const proto::common::hero_data &self_heros);
private:
	static uint32_t add_role(uint32_t pool_pos, arena_role_ptr p_role);
	static uint32_t remove_role(int32_t pool_pos, uint64_t role_uid);
private:
	static uint32_t	gen_ai_id() { return ++m_ai_id; }
	static arena_ai_ptr get_arena_ai(uint32_t id);
	// ����Ƿ�Ҫ������̬AI��ͬʱ����̬AI�Ļ��գ�
	static void		check_need_create_ai(arena_ai_ptr p_ai, arena_role_ptr target);
private:
	static arena_ai_ptr	pop_arena_ai_from_cache();
	static void			push_back_arena_ai_to_cache(arena_ai_ptr p_ai);
	static void			clear_match_pool();

private:
	static match_pools m_match_pools;
	static ai_match_pools m_ai_match_pools;
	static arena_ai_map m_ai_map;
	static arena_ai_cache_vec m_ai_cache_vec;			// ��̬AI�ڴ�أ�����Ƶ��new delete��
//	static uint32_t		m_dynamic_create_ai_count;		// ��̬������AI��
	static uint32_t		m_ai_id;						// ai ID ������
};
