#include "arena.hpp"
#include "cache_key.hpp"
#include "role/role_unify_save.hpp"
#include "role/role_manager.hpp"
#include "config/config_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "tblh/Activities.tbls.h"
#include "tblh/ArenaClassTable.tbls.h"
#include "time_manager.hpp"
#include "tblh/ArenaWeekAwardTable.tbls.h"
#include "mail/mail_manager.hpp"

arena_t::arena_t(uint64_t role_uid)
{
	m_role_uid = role_uid;
	m_key = cache_key_t::create(m_role_uid, cache_name::arena);
}

arena_t::~arena_t()
{

}

void arena_t::init()
{
	// 没有参与过竞技场 给一个初始数值
	if (get_score() == 0 && get_level() == 0)
	{
		auto init_score_conf = GET_CONF(Comprehensive, comprehensive_common::arena_initial_score);
		auto init_level_conf = GET_CONF(Comprehensive, comprehensive_common::arena_basic_class);
		uint32_t init_score = GET_COMPREHENSIVE_VALUE_1(init_score_conf);
		uint32_t init_level = GET_COMPREHENSIVE_VALUE_1(init_level_conf);

		set_score(init_score);
		set_level(init_level);
		auto p_conf = GET_CONF(ArenaClassTable, m_level);
		if (NULL != p_conf)
		{
			role_ptr p_role = role_manager_t::find_role(m_role_uid);
			if (NULL != p_role)
			{
				money_ptr p_money = p_role->get_money();
				if (NULL != p_money)
				{
					p_money->set_honor_week(0);
					p_money->set_honor_limit(p_conf->max_honor());
				}
			}
		}
		save_self();
	}
}

void arena_t::set_win_times(uint32_t win_times)
{
	m_win_times = win_times;
}

void arena_t::set_lose_times(uint32_t lose_times)
{
	m_lose_times = lose_times;
}

void arena_t::set_start(bool flag)
{
	m_is_start = flag;
	save_self();
}

void arena_t::set_score(uint32_t score)
{
	m_score = score;
}

void arena_t::set_level(uint32_t arena_level)
{
	m_level = arena_level;
	save_self();
}

void arena_t::peek_data(proto::common::arena_role_data *arena_data)
{
	if (arena_data)
	{
		arena_data->set_win_times(m_win_times);
		arena_data->set_lose_times(m_lose_times);
		arena_data->set_is_start(m_is_start);
		arena_data->set_score(m_score);
		arena_data->set_arena_level(m_level);
		arena_data->set_best_win_times(m_best_win_times);
		arena_data->set_last_reset_time(m_last_reset_time);
		arena_data->set_all_fight_times(m_total_fight_times);
		arena_data->set_all_win_times(m_total_win_times);
		role_ptr p_role = role_manager_t::find_role(m_role_uid);
		if (NULL != p_role)
		{
			money_ptr p_money = p_role->get_money();
			if (NULL != p_money)
			{
				arena_data->set_honor(p_money->get_honor_week());
				arena_data->set_honor_limit(p_money->get_honor_limit());
			}
		}
	}
}

void arena_t::load_from_db(const proto::common::arena_role_data &arena_data)
{
	m_win_times = arena_data.win_times();
	m_lose_times = arena_data.lose_times();
	m_is_start = arena_data.is_start();
	m_score = arena_data.score();
	m_level = arena_data.arena_level();
	m_best_win_times = arena_data.best_win_times();
	m_last_reset_time = arena_data.last_reset_time();
	m_total_fight_times = arena_data.all_fight_times();
	m_total_win_times = arena_data.all_win_times();
	//读取活动表
	auto p_activities_conf = GET_CONF(Activities, activity_arena);
	if (NULL == p_activities_conf)
	{
		log_error("NULL == p_activities_conf[%d]", activity_arena);
		return;
	}

	if (p_activities_conf->time_id().empty())
	{
		log_error("p_activities_conf[%d] time_id null", activity_arena);
		return;
	}
	uint32_t time_id = p_activities_conf->time_id().at(0);
	if (m_last_reset_time > 0 && !time_manager_t::is_same_period(time_id, m_last_reset_time))
	{
		award();
		reset(time_util_t::now_time());
	}
}

void arena_t::save_self()
{
	proto::common::arena_role_data arena_data;
	peek_data(&arena_data);
	role_unify_save::add_task(m_role_uid, m_key, arena_data);
}

void arena_t::reset(uint32_t time_val)
{
	m_win_times = 0;
	m_lose_times = 0;
	m_is_start = false;
	m_best_win_times = 0;
	m_last_reset_time = time_val;
	m_total_fight_times = 0;
	m_total_win_times = 0;

	auto p_conf = GET_CONF(ArenaClassTable, m_level);
	if (NULL == p_conf)
	{
		log_error("NULL == p_conf[%d]", m_level);
		return;
	}
	m_level = p_conf->reset_stage();

	p_conf = GET_CONF(ArenaClassTable, m_level);
	if (NULL == p_conf)
	{
		log_error("NULL == p_conf[%d]", m_level);
		return;
	}
	m_score = p_conf->score();
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL != p_role)
	{
		money_ptr p_money = p_role->get_money();
		if (NULL != p_money)
		{
			p_money->set_honor_week(0);
			p_money->set_honor_limit(p_conf->max_honor());
		}
	}
	save_self();
}

void arena_t::award()
{
	auto p_conf = GET_CONF(ArenaWeekAwardTable, m_level);
	if (NULL == p_conf)
	{
		log_error("NULL == p_conf[%d]", m_level);
		return;
	}
	mail_manager_t::send_mail(m_role_uid, proto::common::MAIL_TYPE_SYS, p_conf->mail_id());
}

