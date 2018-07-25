#ifndef __CENTER_OFFLINE_ARENA_H__
#define __CENTER_OFFLINE_ARENA_H__

#include "macros.hpp"

#include "server.pb.h"
#include "common.pb.h"
#include <map>
#include "offline_arena_record.hpp"

class offline_arena_t
{
public:
	offline_arena_t(){}
public:
	void set_uid(uint64_t val) { m_uid = val; }
	uint64_t get_uid() { return m_uid; }
	
	void set_cur_rank(uint32_t val) { m_cur_rank = val; }
	uint32_t get_cur_rank() { return m_cur_rank; }

	void set_cur_class(uint32_t val) { m_cur_class = val; }
	uint32_t get_cur_class() { return m_cur_class; }

	void set_old_class(uint32_t val) { m_old_class = val; }
	uint32_t get_old_class() { return m_old_class; }

	void set_old_record_rank(uint32_t val) { m_old_record_rank = val; }
	uint32_t get_old_record_rank() { return m_old_record_rank; }

	void set_record_rank(uint32_t val) { m_record_rank = val; }
	uint32_t get_record_rank() { return m_record_rank; }

	void set_fight_count(uint32_t val) { m_fight_count = val; }
	uint32_t get_fight_count() { return m_fight_count; }

	void set_buy_count(uint32_t val) { m_buy_count = val; }
	uint32_t get_buy_count() { return m_buy_count; }

	void set_enemy_rank(uint32_t val) { m_enemy_rank = val; }
	uint32_t get_enemy_rank() { return m_enemy_rank; }
	void set_enemy_type(uint32_t val) { m_enemy_type = val; }
	uint32_t get_enemy_type() { return m_enemy_type; }
	void set_enemy_id(uint64_t val) { m_enemy_id = val; }
	uint64_t get_enemy_id() { return m_enemy_id; }
	void set_score(uint32_t val) { m_score = val; }
	uint32_t get_score() { return m_score; }
	void add_score_award(uint32_t val) { m_score_award.push_back(val); }

	void add_record(offline_arena_record_ptr p_record);

	void peek_record_data(proto::common::offline_arena_record_data* p_data);
	void save_record_data();

	void peek_data(proto::common::offline_arena_single* p_single);

	void save_self();

	void one_day();

	void award();

	void score_award(uint32_t score);
private:
	uint32_t get_max_record_num();
private:
	uint64_t m_uid = 0;	
	uint32_t m_cur_rank = 0;				// 当前排名
	uint32_t m_cur_class = 0;				// 当前级别
	uint32_t m_old_record_rank = 0;			// 旧的最高排名
	uint32_t m_record_rank = 0;				// 最高排名
	uint32_t m_old_class = 0;				// 旧的级别（用于判断领取晋级奖励）
	uint32_t m_fight_count = 0;				// 剩余攻打次数
	uint32_t m_buy_count = 0;				// 购买次数
	uint32_t m_enemy_rank = 0;				// 当前攻打目标的排名(用来做战斗状态检测和战斗结算)
	uint32_t m_enemy_type = 0;				// 当前攻打目标的类型(用来做战斗状态检测和战斗结算)
	uint64_t m_enemy_id = 0;				// 当前攻打目标的id(用来做战斗状态检测和战斗结算)
	std::vector<offline_arena_record_ptr> m_record_data;
	uint32_t m_score = 0;					// 每日积分
	std::vector<uint32_t> m_score_award;	// 已经领取的积分奖励
};
typedef boost::shared_ptr<offline_arena_t> offline_arena_ptr;

#endif