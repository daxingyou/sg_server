#ifndef __CENTER_OFFLINE_ARENA_RECORD_H__
#define __CENTER_OFFLINE_ARENA_RECORD_H__

#include "macros.hpp"

#include "server.pb.h"
#include "common.pb.h"
#include <map>

class offline_arena_record_t
{
public:
	offline_arena_record_t(){}
public:
	void set_enemy_type(uint32_t val) { m_enemy_type = val; }
	void set_enemy_uid(const std::string& val) { m_enemy_uid = val; }
	void set_enemy_name(const std::string& val) { m_enemy_name = val; }
	void set_enemy_fighting(uint32_t val) { m_enemy_fighting = val; }
	void set_enemy_plugin(uint32_t val) { m_enemy_plugin = val; }
	void set_enemy_level(uint32_t val) { m_enemy_level = val; }
	void set_record_time(uint32_t val) { m_record_time = val; }
	void set_is_win(uint32_t val) { m_is_win = val; }
	void set_old_rank(uint32_t val) { m_old_rank = val; }
	void set_new_rank(uint32_t val) { m_new_rank = val; }
	void set_side(uint32_t val) { m_side = val; }
	void set_enemy_headpic(uint32_t val) { m_enemy_headpic = val; }
public:
	void peek_data(proto::common::offline_arena_record* p_single);
private:
	uint32_t m_enemy_type = 0;				// 类型
	std::string m_enemy_uid = "";			// 玩家UID(如果是机器人直接给机器人id)
	std::string m_enemy_name = "";			// 玩家名字
	uint32_t m_enemy_fighting = 0;			// 战力
	uint32_t m_enemy_plugin = 0;			// 模型
	uint32_t m_enemy_headpic = 0;			// 头像
	uint32_t m_enemy_level = 0;				// 等级
	uint32_t m_record_time = 0;				// 战斗时间
	uint32_t m_is_win = 0;					// 战斗结果
	uint32_t m_old_rank = 0;				// 战斗前排名
	uint32_t m_new_rank = 0;				// 战斗后排名
	uint32_t m_side = 0;					// 0为攻击1为防御
};
typedef boost::shared_ptr<offline_arena_record_t> offline_arena_record_ptr;

#endif