#pragma once

#include "macros.hpp"

#include <unordered_map>
#include <functional>

enum time_type
{
	time_type_every_weekday = 1,
	time_type_every_day = 2,
	time_type_fixed_day = 3,
	time_type_from_server_open = 4,
	time_type_last_day_of_month = 5,
	time_type_first_day_of_month = 6,
};

class TimeTable;

typedef std::function<void(uint32_t, bool)> on_timer_callback;

class time_item_t
{
public:
	time_item_t() :_start(0), _last(0), _interval(0), _time(0), _inTime(false) {}
	virtual ~time_item_t() {};

	virtual bool init(TimeTable* p_conf) = 0;

	virtual bool check_same_period(uint32_t check_time) = 0;

	virtual bool check_prev_period(uint32_t curr_time, uint32_t check_time) { return false; }

	void timer(uint32_t cur);

	virtual time_t next_time() = 0;

	uint32_t start_time() const { return _start; }
	uint32_t end_time() const { return _start + _last; }
	bool in_time() const { return _inTime; }

	uint32_t get_left_times() const { return _time; }

	uint32_t register_func(on_timer_callback func);
	void unregister(uint32_t id);
	void unregister_all();

	// 如果需要每秒调用到时在加
	//virtual void on_timer(uint32_t cur);

protected:
	bool do_init(uint32_t start, uint32_t last, uint32_t interval, uint32_t time = 0);

	void start();
	void end();

	void on_start();
	void on_end();

protected:
	uint32_t _start, _last, _interval, _time;
	bool _inTime;

protected:
	uint32_t m_id = 0;
	typedef std::unordered_map<uint32_t, on_timer_callback> callback_funcs_map;
	callback_funcs_map m_callback_funcs_map;
	uint32_t m_keys = 0;
};

//////////////////////////////////////////////////////////////////////////
// 每天
class time_day_t : public time_item_t
{
public:
	virtual bool init(TimeTable* p_conf);

	virtual bool check_same_period(uint32_t check_time);

	virtual bool check_prev_period(uint32_t curr_time, uint32_t check_time);

protected:
	// 根据得到一个周期id
	uint32_t get_day_id(uint32_t t);

	virtual time_t next_time();

protected:
	int32_t _hour, _min, _sec;
};
// 每周
class time_week_t : public time_item_t
{
public:
	virtual bool init(TimeTable* p_conf);

	virtual bool check_same_period(uint32_t check_time);

protected:
	// 根据得到一个周期id
	uint32_t get_week_id(uint32_t t);

	virtual time_t next_time();

protected:
	int32_t _day, _hour, _min, _sec;
};
// 只触发一次
class time_once_t : public time_item_t
{
public:
	virtual bool init(TimeTable* p_conf);

	virtual bool check_same_period(uint32_t check_time);

protected:
	virtual time_t next_time() { return 0; }
};

// 每月最后几天（注意2月份润年）
class time_month_last_day_t : public time_item_t
{
public:
	virtual bool init(TimeTable* p_conf);

	virtual bool check_same_period(uint32_t check_time);

protected:
	virtual time_t next_time();

protected:
	int32_t _day, _hour, _min, _sec;
};

// 每月第几天（注意2月份润年）
class time_month_first_day_t : public time_item_t
{
public:
	virtual bool init(TimeTable* p_conf);

	virtual bool check_same_period(uint32_t check_time);

protected:
	virtual time_t next_time();

protected:
	int32_t _day, _hour, _min, _sec;
};