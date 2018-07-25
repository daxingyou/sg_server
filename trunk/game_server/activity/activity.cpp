#include "activity.hpp"

activity_t::activity_t(uint32_t activity_id)
	: m_tid(activity_id)
{
}

void activity_t::load_data(const proto::common::activity_single& activity_single)
{
	m_tid = activity_single.activity_id();
	m_count = activity_single.count();
	m_add_count = activity_single.add_count();
	m_last_reset_time = activity_single.last_reset_time();
}

void activity_t::peek_data(proto::common::activity_single* p_activity_single)
{
	if (NULL == p_activity_single)
	{
		return;
	}
	p_activity_single->set_activity_id(m_tid);
	p_activity_single->set_count(m_count);
	p_activity_single->set_add_count(m_add_count);
	p_activity_single->set_last_reset_time(m_last_reset_time);
}
