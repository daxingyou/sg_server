#ifndef __ACTIVITY_H__
#define __ACTIVITY_H__
#include "macros.hpp"
#include "protos_fwd.hpp"

enum activity_reset_type
{
	activity_reset_open = 0,		//活动开启重置
	activity_reset_day = 1,			//每日重置
	activity_reset_week = 2,		//每周重置
	activity_reset_month = 3,		//每月重置
};

class activity_t
{
public:
	activity_t(){}
	activity_t(uint32_t activity_id);

	void set_tid(uint32_t val) { m_tid = val; }
	uint32_t get_tid() { return m_tid; }
	void set_count(uint32_t val) { m_count = val; }
	uint32_t get_count() { return m_count; }
	void set_last_reset_time(uint32_t val) { m_last_reset_time = val; }
	uint32_t get_last_reset_time() { return m_last_reset_time; }
public:
	void load_data(const proto::common::activity_single& activity_single);
	void peek_data(proto::common::activity_single* p_activity_single);
private:
	uint32_t		m_tid = 0;
	uint32_t		m_count = 0;
	uint32_t		m_last_reset_time = 0;
};
typedef boost::shared_ptr<activity_t> activity_ptr;

#endif // !__ACTIVITY_H__

