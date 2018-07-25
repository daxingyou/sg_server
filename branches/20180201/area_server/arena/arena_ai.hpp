#pragma once

#include <boost/shared_ptr.hpp>
#include "protos_fwd.hpp"

/*
author:wys
date:2017.2.28
desc:竞技场AI
*/

class arena_ai_t
{
public:
	arena_ai_t(uint32_t id);
	arena_ai_t(uint32_t id, uint32_t tid, uint32_t score, uint32_t arena_level, const std::string &name, uint32_t level, uint32_t formation);
	~arena_ai_t();
	
	void set_data(proto::common::mirror_fight_data &cross_data);
	void peek_data(proto::common::mirror_fight_data *cross_data);

	void reset();
	void set_dynamic_ai_data(uint32_t score, uint32_t arena_level, const char *name, uint32_t level);

	uint32_t get_id() { return m_id; }
	uint32_t get_tid() { return m_tid; }
	const char* get_name() { return m_name.c_str(); }
	uint32_t get_score() { return m_score; }
	uint32_t get_level() { return m_level; }
	uint32_t get_arena_level() { return m_arena_level; }
	uint32_t get_formation() { return m_formation; }
	uint32_t get_win_times() { return m_win_times; }
	uint32_t get_lose_times() { return m_lose_times; }

	void	 set_pools_pos(uint32_t pools_pos) { m_pools_pos = pools_pos; }
	void	 set_pool_pos(uint32_t pool_pos) { m_pool_pos = pool_pos; }
	int32_t  get_pools_pos() { return m_pools_pos; }
	int32_t  get_pool_pos() { return m_pool_pos; }

	void	 deal_battle_result(bool is_win);
	void	 reset_battle_data();

	bool	 is_dynamic() { return m_is_dynamic; }
private:
	proto::common::mirror_fight_data			m_cross_data;		// 跨服战数据
	std::string						m_name = "";		// 名字
	uint32_t						m_id = 0;			// 唯一ID(自增长)
	uint32_t						m_tid = 0;			// tid
	uint32_t						m_formation = 0;	// 阵型
	uint32_t						m_score = 0;		// 分数
	uint32_t						m_level = 0;		// 等级
	uint32_t						m_arena_level = 0;	// 段位
	uint32_t						m_win_times = 0;	// 赢的次数
	uint32_t						m_lose_times = 0;	// 输次数
	int32_t							m_pools_pos = -1;	// 在哪个匹配池
	int32_t							m_pool_pos = -1;		// 在匹配池中的位置
	bool							m_is_dynamic = false; // 是否是动态AI
};

typedef boost::shared_ptr<arena_ai_t> arena_ai_ptr;