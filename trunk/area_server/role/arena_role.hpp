#pragma once

#include <boost/shared_ptr.hpp>
#include "protos_fwd.hpp"

/*
author:wys
date:2017.2.16
desc:竞技场角色
*/

class arena_role_t
{
public:
	arena_role_t(uint64_t role_uid);
	~arena_role_t();

	void set_score(uint32_t score);
	void set_arena_level(uint32_t arena_level) { m_arena_level = arena_level; }
	void set_last_play_time(uint32_t last_play_time) { m_last_play_time = last_play_time; }
	void set_target_uid(uint64_t role_traget_uid) { m_role_target_uid = role_traget_uid; }
	void add_match_times() { ++m_match_times; }

	void reset_match_times() { m_match_times = 0; }

	uint64_t get_role_uid() { return m_role_uid; }
	uint64_t get_target_uid() { return m_role_target_uid; }
	uint32_t get_score() { return m_score; }
	uint32_t get_arena_level() { return m_arena_level; }
	uint32_t get_last_play_time() { return m_last_play_time; }
	uint32_t get_match_times() { return m_match_times; }
	const char* get_role_name();
	uint32_t get_role_server_id();
	const char* get_role_server_name();

	uint32_t get_fighting_value() {	return get_cross_data().rd().obj_info().fighting_value(); }
	uint32_t get_rank_lv();

public:
	void	 set_in_pools_pos(int32_t pos) { m_in_pools_pos = pos; }
	int32_t  get_in_pools_pos() { return m_in_pools_pos; }

	void set_cross_data(const proto::common::role_cross_data &cross_data);
	void peek_data(proto::common::role_cross_data *cross_data);
	const proto::common::role_cross_data& get_cross_data() { return m_cross_data; }
private:
	proto::common::role_cross_data m_cross_data;		// 跨服战数据
	uint64_t m_role_uid = 0;			// uid
	uint64_t m_role_target_uid = 0;		// 战斗记录目标UID
	uint32_t m_match_times = 0;			// 匹配次数
	uint32_t m_score = 0;				// 分数
	uint32_t m_arena_level = 0;			// 段位
	uint32_t m_last_play_time = 0;		// 上次参与时间
	int32_t m_in_pools_pos = -1;		// 在哪个匹配池
//	uint32_t m_level = 0;
};

typedef boost::shared_ptr<arena_role_t> arena_role_ptr;