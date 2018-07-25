#pragma once
#include "protos_fwd.hpp"

/*
author:wys
date:2017.2.14
desc:¾º¼¼³¡
*/


class arena_t
{
public:
	arena_t(uint64_t role_uid);
	~arena_t();
	
	void		init();

	uint32_t	get_win_times() { return m_win_times; }
	uint32_t	get_lose_times() { return m_lose_times; }
	uint32_t	get_score() { return m_score; }
	uint32_t	get_level() { return m_level; }
	uint32_t	get_last_fight_time() { return m_last_fight_time; }
	uint32_t	get_best_win_times() { return m_best_win_times; }
	uint32_t	get_last_reset_time() { return m_last_reset_time; }
	bool		is_start() { return m_is_start; }
	bool		is_fighting() { return m_is_fighting; }
	uint32_t	get_total_win_times() { return m_total_win_times; }
	uint32_t	get_total_fight_times() { return m_total_fight_times; }

	void		set_win_times(uint32_t win_times);
	void		set_lose_times(uint32_t lose_times);
	void		set_start(bool flag);
	void		set_is_fighting(bool fighting_flag) { m_is_fighting = fighting_flag; }
	void		set_score(uint32_t score);
	void		set_level(uint32_t arena_level);
	void		set_last_fight_time(uint32_t now_time) { m_last_fight_time = now_time; }
	void		set_best_win_times(uint32_t best_win_times) { m_best_win_times = best_win_times; }
	void		set_last_reset_time(uint32_t last_reset_time) { m_last_reset_time = last_reset_time; }
	void		set_total_win_times(uint32_t total_win_times) { m_total_win_times = total_win_times; }
	void		set_total_fight_times(uint32_t total_fight_times) { m_total_fight_times = total_fight_times; }
public:
	void		peek_data(proto::common::arena_role_data *arena_data);
	void		load_from_db(const proto::common::arena_role_data &arena_data);
	void		save_self();
	void		reset(uint32_t time_val);
	void		award();
private:
	std::string	m_key = "";
	uint64_t	m_role_uid = 0;
	uint32_t	m_win_times = 0;
	uint32_t	m_lose_times = 0;
	uint32_t	m_total_win_times = 0;
	uint32_t	m_total_fight_times = 0;
	uint32_t	m_score = 0;
	uint32_t	m_level = 0;
	uint32_t	m_last_fight_time = 0;
	uint32_t	m_season = 0;
	uint32_t	m_best_win_times = 0;
	uint32_t    m_last_reset_time = 0;
	bool		m_is_start = false;
	bool		m_is_fighting = false;
};