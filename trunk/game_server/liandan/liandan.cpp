#include "liandan.hpp"
#include "main/game_server_fwd.hpp"
#include "utility.hpp"

USING_NS_COMMON;
liandan_t::liandan_t()
{

}


liandan_t::~liandan_t()
{

}

void liandan_t::load_data(const proto::common::liandan_single& ls)
{
	m_state = ls.state();
	m_free_time = ls.free_time();
	m_liandan_tid = ls.liandan_tid();
	m_count = ls.count();
	check_doing();
}

void liandan_t::peek_data(proto::common::liandan_single* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return;
	}
	check_doing();
	p_data->set_state(m_state);
	p_data->set_free_time(m_free_time);
	p_data->set_liandan_tid(m_liandan_tid);
	p_data->set_count(m_count);
}

void liandan_t::check_doing()
{
	if (m_state == proto::common::liandan_state_doing)
	{
		if (time_util_t::now_time() >= m_free_time)
		{
			//时间到了改成炼丹完成状态
			m_state = proto::common::liandan_state_done;
			m_free_time = 0;
		}
	}
}

