#include "tower_trigger.hpp"
#include "protos_fwd.hpp"
#include "utility.hpp"
#include "log.hpp"

USING_NS_COMMON;
void tower_trigger_t::load_data(const proto::common::tower_trigger& single)
{
	m_uid = string_util_t::string_to_uint64(single.uid());
	m_tid = single.tid();
	m_expired_time = single.expired_time();
	m_finder_name = single.finder_name();
}

void tower_trigger_t::peek_data(proto::common::tower_trigger* p_single)
{
	if (NULL == p_single)
	{
		log_error("NULL == p_single");
		return;
	}
	p_single->set_uid(string_util_t::uint64_to_string(m_uid));
	p_single->set_tid(m_tid);
	p_single->set_expired_time(m_expired_time);
	p_single->set_finder_name(m_finder_name);
}
