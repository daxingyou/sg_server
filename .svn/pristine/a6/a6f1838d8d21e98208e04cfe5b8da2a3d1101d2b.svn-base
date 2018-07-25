#include "timer_common.hpp"
#include "tblh/TimeTable.tbls.h"
#include "config_mgr.h"
#include "utility.hpp"
#include "time_manager.hpp"

bool time_item_t::do_init(uint32_t start, uint32_t last, uint32_t interval, uint32_t time /*= 0*/)
{
	_start = start;
	_last = last;
	_interval = interval;
	_time = time;

	if (!_interval) _interval = DAY_SECOND;
	if (!_time) _time = (uint32_t)-1;

	uint32_t cur = common::time_util_t::now_time();
	if ((_start + _last) <= cur) {
		uint32_t tt = (cur - _last - _start) / _interval + 1;
		if (_time < tt) {
			_start = 0;
		} else {
			_start += _interval * tt;
			_time -= tt;
		}
	}

	return true;
}

void time_item_t::timer(uint32_t cur)
{
	if (!_start) return;

	bool willStart = false;
	bool willEnd = false;
	bool oldIn = _inTime;

	if ((_start + _last) <= cur) {
		if (_inTime) _inTime = false;

		while (_time && (_start + _last) <= cur) {
			_start = next_time();
			_time--;
		}

		if (!_time) _start = 0;
	}

	if (!_inTime && _time && _start <= cur)
		_inTime = true;

	if (!oldIn && _inTime) willStart = true;
	if (oldIn && !_inTime) willEnd = true;

	if (willEnd) end();
	if (willStart) start();

	//on_timer(cur);
}

uint32_t time_item_t::register_func(on_timer_callback func)
{
	uint32_t key = ++m_keys;

	m_callback_funcs_map.insert(std::make_pair(key, func));

	return key;
}

void time_item_t::unregister(uint32_t id)
{
	callback_funcs_map::iterator itr = m_callback_funcs_map.find(id);
	if (itr != m_callback_funcs_map.end()) {
		m_callback_funcs_map.erase(itr);
	}
}

void time_item_t::unregister_all()
{
	m_callback_funcs_map.clear();
}

void time_item_t::start()
{
	_inTime = true;
	on_start();
}

void time_item_t::end()
{
	_inTime = false;
	on_end();
}

void time_item_t::on_start()
{
	for (callback_funcs_map::iterator itr = m_callback_funcs_map.begin();
		itr != m_callback_funcs_map.end(); ++itr) {
		itr->second(m_id, _inTime);
	}
}

void time_item_t::on_end()
{
	for (callback_funcs_map::iterator itr = m_callback_funcs_map.begin();
		itr != m_callback_funcs_map.end(); ++itr) {
		itr->second(m_id, _inTime);
	}
}

//////////////////////////////////////////////////////////////////////////

bool time_month_last_day_t::init(TimeTable* p_conf)
{
	if (NULL == p_conf) {
		log_error("p_conf null error");
		return false;
	}

	m_id = p_conf->id();

	if (p_conf->time_type() != time_type_last_day_of_month) {
		log_error("time id[%u] type[%u] error", p_conf->id(), p_conf->time_type());
		return false;
	}

	const std::vector<uint32_t>& start_time_vec = p_conf->start_time_vec();

	if (start_time_vec.size() != 4) {
		log_error("invalid time_month_last_day_t format");
		return false;
	}

	_day = (int32_t)start_time_vec[0];
	if (_day == 0 || _day > 28) {
		log_error("invalid time_month_last_day_t format");
		return false;
	}

	_hour = (int32_t)start_time_vec[1];
	_min = (int32_t)start_time_vec[2];
	_sec = (int32_t)start_time_vec[3];
	_time = (uint32_t)-1;
	_last = p_conf->time_duration();

	_start = next_time();

	return true;
}

bool time_month_last_day_t::check_same_period(uint32_t check_time)
{
	// TODO: 目前没有这个需求判断是否在一个周期内，这个类型的判断需要特殊处理，有需求了再去实现
	return false;
}

time_t time_month_last_day_t::next_time()
{
	struct tm day_time;
	common::time_util_t::get_local_time(day_time, common::time_util_t::now_time());

	int32_t days = common::time_util_t::get_month_days(day_time.tm_year + 1900, day_time.tm_mon + 1);

	day_time.tm_mday = days - _day + 1;
	day_time.tm_hour = _hour;
	day_time.tm_min = _min;
	day_time.tm_sec = _sec;

	time_t target_time = common::time_util_t::get_utc_time(day_time);
	if (target_time == (time_t)-1) {
		log_error("invalid get_utc_time error");
		return 0;
	}

	if (target_time + _last > common::time_util_t::now_time()) {
		return target_time;
	}

	if (day_time.tm_mon >= 11) {
		day_time.tm_year += 1;
		day_time.tm_mon = 0;
	} else {
		day_time.tm_mon += 1;
	}

	days = common::time_util_t::get_month_days(day_time.tm_year + 1900, day_time.tm_mon + 1);

	day_time.tm_mday = days - _day + 1;
	day_time.tm_hour = _hour;
	day_time.tm_min = _min;
	day_time.tm_sec = _sec;

	target_time = common::time_util_t::get_utc_time(day_time);
	if (target_time == (time_t)-1) {
		log_error("invalid get_utc_time error");
		return 0;
	}

	return target_time;
}

bool time_month_first_day_t::init(TimeTable* p_conf)
{
	if (NULL == p_conf) {
		log_error("p_conf null error");
		return false;
	}

	m_id = p_conf->id();

	if (p_conf->time_type() != time_type_first_day_of_month) {
		log_error("time id[%u] type[%u] error", p_conf->id(), p_conf->time_type());
		return false;
	}

	const std::vector<uint32_t>& start_time_vec = p_conf->start_time_vec();
	if (start_time_vec.size() != 4) {
		log_error("invalid time_type_first_day_of_month format");
		return false;
	}

	_day = (int32_t)start_time_vec[0];
	if (_day > 28) {
		log_error("invalid time_type_first_day_of_month format");
		return false;
	}

	_hour = (int32_t)start_time_vec[1];
	_min = (int32_t)start_time_vec[2];
	_sec = (int32_t)start_time_vec[3];
	_time = (uint32_t)-1;
	_last = p_conf->time_duration();

	_start = next_time();

	return true;
}

bool time_month_first_day_t::check_same_period(uint32_t check_time)
{
	uint32_t cur_time = common::time_util_t::now_time();
	struct tm day_time;
	common::time_util_t::get_local_time(day_time, cur_time);

	day_time.tm_mday = _day;
	day_time.tm_hour = _hour;
	day_time.tm_min = _min;
	day_time.tm_sec = _sec;

	time_t time1 = common::time_util_t::get_utc_time(day_time);
	if (time1 == (time_t)-1) {
		log_error("invalid get_utc_time error");
		return false;
	}

	time_t start_time, end_time;

	if (time1 <= cur_time) {
		if (day_time.tm_mon >= 11) {
			day_time.tm_year += 1;
			day_time.tm_mon = 0;
		} else {
			day_time.tm_mon += 1;
		}

		end_time = common::time_util_t::get_utc_time(day_time);
		if (end_time == (time_t)-1) {
			log_error("invalid get_utc_time error");
			return false;
		}

		start_time = time1;
	} else {
		if (day_time.tm_mon == 0) {
			day_time.tm_year -= 1;
			day_time.tm_mon = 11;
		} else {
			day_time.tm_mon -= 1;
		}

		start_time = common::time_util_t::get_utc_time(day_time);
		if (start_time == (time_t)-1) {
			log_error("invalid get_utc_time error");
			return false;
		}

		end_time = time1;
	}

	return start_time <= check_time && check_time < end_time;
}

time_t time_month_first_day_t::next_time()
{
	struct tm day_time;
	common::time_util_t::get_local_time(day_time, common::time_util_t::now_time());

	day_time.tm_mday = _day;
	day_time.tm_hour = _hour;
	day_time.tm_min = _min;
	day_time.tm_sec = _sec;

	time_t target_time = common::time_util_t::get_utc_time(day_time);
	if (target_time == (time_t)-1) {
		log_error("invalid get_utc_time error");
		return 0;
	}

	if (target_time + _last > common::time_util_t::now_time()) {
		return target_time;
	}

	if (day_time.tm_mon >= 11) {
		day_time.tm_year += 1;
		day_time.tm_mon = 0;
	} else {
		day_time.tm_mon += 1;
	}

	target_time = common::time_util_t::get_utc_time(day_time);
	if (target_time == (time_t)-1) {
		log_error("invalid get_utc_time error");
		return 0;
	}

	return target_time;
}

bool time_day_t::init(TimeTable* p_conf)
{
	if (NULL == p_conf) {
		log_error("p_conf null error");
		return false;
	}

	if (p_conf->time_type() != time_type_every_day) {
		log_error("p_conf[%u] type[%u] != time_type_every_day", p_conf->id(), p_conf->time_type());
		return false;
	}

	m_id = p_conf->id();

	const std::vector<uint32_t>& start_time_vec = p_conf->start_time_vec();
	if (start_time_vec.size() != 3) {
		log_error("invalid time_type_every_day format error");
		return false;
	}

	_hour = (int32_t)start_time_vec[0];
	_min = (int32_t)start_time_vec[1];
	_sec = (int32_t)start_time_vec[2];

	struct tm day_time;
	common::time_util_t::get_local_time(day_time, common::time_util_t::now_time());

	day_time.tm_hour = _hour;
	day_time.tm_min = _min;
	day_time.tm_sec = _sec;

	time_t target_time = common::time_util_t::get_utc_time(day_time);
	if (target_time == (time_t)-1) {
		log_error("invalid get_utc_time error");
		return false;
	}

	// 防止target_time已经是下个周期时间，导致如果还在持续时间内判断错误
	do_init(target_time - DAY_SECOND, p_conf->time_duration(), DAY_SECOND);

	return true;
}

bool time_day_t::check_same_period(uint32_t check_time)
{
	return get_day_id(check_time) == get_day_id(common::time_util_t::now_time());
}

bool time_day_t::check_prev_period(uint32_t curr_time, uint32_t check_time)
{
	return get_day_id(check_time) == (get_day_id(curr_time) - 1);
}

uint32_t time_day_t::get_day_id(uint32_t t)
{
	// 防止时间过小
	if (t < DAY_SECOND) return 0;

	uint32_t interval_time = _hour * 3600 + _min * 60 + _sec;

	return (t - time_util_t::get_timezone() - interval_time) / DAY_SECOND;
}

time_t time_day_t::next_time()
{
	return _start + _interval;
}

bool time_week_t::init(TimeTable* p_conf)
{
	if (NULL == p_conf) {
		log_error("p_conf null error");
		return false;
	}

	if (p_conf->time_type() != time_type_every_weekday) {
		log_error("p_conf[%u] type[%u] != time_type_every_weekday", p_conf->id(), p_conf->time_type());
		return false;
	}

	m_id = p_conf->id();

	const std::vector<uint32_t>& start_time_vec = p_conf->start_time_vec();
	if (start_time_vec.size() != 4 || start_time_vec[0] == 0 || start_time_vec[0] > 7)
	{
		log_error("time[%u] invalid time_type_every_weekday format error", m_id);
		return false;
	}

	_day = (int32_t)start_time_vec[0];
	_hour = (int32_t)start_time_vec[1];
	_min = (int32_t)start_time_vec[2];
	_sec = (int32_t)start_time_vec[3];

	struct tm day_time;
	common::time_util_t::get_local_time(day_time, common::time_util_t::now_time());

	day_time.tm_hour = _hour;
	day_time.tm_min = _min;
	day_time.tm_sec = _sec;

	time_t target_time = common::time_util_t::get_utc_time(day_time);
	if (target_time == (time_t)-1) {
		log_error("invalid get_utc_time error");
		return false;
	}

	int32_t curr_week = common::time_util_t::get_weekday();
	int32_t start_week = _day;
	if (curr_week >= start_week) {
		target_time -= (curr_week - start_week) * DAY_SECOND;
	} else {
		target_time += (start_week - curr_week) * DAY_SECOND;
	}

	// 防止target_time已经是下个周期时间，导致如果还在持续时间内判断错误
	do_init(target_time - WEEK_SECOND, p_conf->time_duration(), WEEK_SECOND);

	return true;
}

bool time_week_t::check_same_period(uint32_t check_time)
{
	return get_week_id(check_time) == get_week_id(common::time_util_t::now_time());
}

uint32_t time_week_t::get_week_id(uint32_t t)
{
	// 防止时间过小
	if (t < WEEK_SECOND) return 0;

	uint32_t interval_time = (_day - 1) * DAY_SECOND + _hour * 3600 + _min * 60 + _sec;

	return (t - time_util_t::get_timezone() - 4 * DAY_SECOND - interval_time) / WEEK_SECOND;
}

time_t time_week_t::next_time()
{
	return _start + _interval;
}

bool time_once_t::init(TimeTable* p_conf)
{
	if (NULL == p_conf) {
		log_error("p_conf null error");
		return false;
	}

	m_id = p_conf->id();

	const std::vector<uint32_t>& start_time_vec = p_conf->start_time_vec();
	switch (p_conf->time_type()) {
		case time_type_fixed_day: {
			if (start_time_vec.size() != 6) {
				log_error("invalid time_type_fixed_day format");
				return false;
			}

			struct tm day_time;
			day_time.tm_year = (int32_t)start_time_vec[0] - 1900;
			day_time.tm_mon = (int32_t)start_time_vec[1] - 1;
			day_time.tm_mday = (int32_t)start_time_vec[2];

			day_time.tm_hour = (int32_t)start_time_vec[3];
			day_time.tm_min = (int32_t)start_time_vec[4];
			day_time.tm_sec = (int32_t)start_time_vec[5];

			time_t target_time = common::time_util_t::get_utc_time(day_time);
			if (target_time == (time_t)-1) {
				log_error("invalid get_utc_time error");
				return false;
			}

			if (target_time + p_conf->time_duration() > common::time_util_t::now_time()) {
				do_init(target_time, p_conf->time_duration(), WEEK_SECOND, 1);
			}

			break;
		}
		case time_type_from_server_open: {
			const std::vector<uint32_t>& server_start_date = time_manager_t::get_server_start_date();
			if (server_start_date.size() != 3 || start_time_vec.size() != 4) {
				log_error("invalid time_type_from_server_open format");
				return false;
			}

			struct tm day_time;
			day_time.tm_year = (int32_t)server_start_date[0] - 1900;
			day_time.tm_mon = (int32_t)server_start_date[1] - 1;
			day_time.tm_mday = (int32_t)server_start_date[2];

			day_time.tm_hour = (int32_t)start_time_vec[1];
			day_time.tm_min = (int32_t)start_time_vec[2];
			day_time.tm_sec = (int32_t)start_time_vec[3];

			time_t target_time = common::time_util_t::get_utc_time(day_time);
			if (target_time == (time_t)-1) {
				log_error("invalid get_utc_time error");
				return false;
			}

			target_time += DAY_SECOND * start_time_vec[0];

			if (target_time + p_conf->time_duration() > common::time_util_t::now_time()) {
				do_init(target_time, p_conf->time_duration(), WEEK_SECOND, 1);
			}

			break;
		}
		default: {
			log_error("time id[%u] type[%u] error", p_conf->id(), p_conf->time_type());
			return false;
		}
	}

	return true;
}

bool time_once_t::check_same_period(uint32_t check_time)
{
	// 这个没有周期可言，就没有同时期的概念
	return false;
}