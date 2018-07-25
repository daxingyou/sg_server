#include "arena_ai.hpp"


arena_ai_t::arena_ai_t(uint32_t id):
	m_id(id)
{

}

arena_ai_t::arena_ai_t(uint32_t id, uint32_t tid, uint32_t score, uint32_t arena_level, const std::string &name, uint32_t level,
						uint32_t formation):
	m_id(id),
	m_tid(tid),
	m_score(score),
	m_arena_level(arena_level),
	m_name(name),
	m_level(level),
	m_formation(formation)
{
}

arena_ai_t::~arena_ai_t()
{

}
		
void arena_ai_t::set_data(proto::common::mirror_fight_data &cross_data)
{
	m_is_dynamic = true;
	m_cross_data.CopyFrom(cross_data);
}

void arena_ai_t::peek_data(proto::common::mirror_fight_data *cross_data)
{
	if (cross_data)
	{
		cross_data->CopyFrom(m_cross_data);
	}
}

void arena_ai_t::reset()
{
	m_cross_data.Clear();
	m_name = "";		// 名字
	m_tid = 0;			// tid
	m_formation = 0;	// 阵型
	m_score = 0;		// 分数
	m_level = 0;		// 等级
	m_arena_level = 0;	// 段位
	m_win_times = 0;	// 赢的次数
	m_lose_times = 0;	// 输次数
	m_pools_pos = -1;	// 在哪个匹配池
	m_pool_pos = -1;	// 在匹配池中的位置
}

void arena_ai_t::set_dynamic_ai_data(uint32_t score, uint32_t arena_level, const char *name, uint32_t level)
{
	m_score = score;
	m_arena_level = arena_level;
	m_name = name;
	m_level = level;
}

void arena_ai_t::deal_battle_result(bool is_win)
{
	if (is_win)
		++m_win_times;
	else
		++m_lose_times;
}

void arena_ai_t::reset_battle_data()
{
	m_win_times = 0;
	m_lose_times = 0;
}
