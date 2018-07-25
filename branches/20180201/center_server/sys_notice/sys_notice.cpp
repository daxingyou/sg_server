#include "sys_notice.hpp"
#include "utility.hpp"
#include "cache_key.hpp"
#include "log.hpp"

sys_notice_t::sys_notice_t(uint64_t uid, uint32_t type, uint32_t param)
	: m_uid(uid)
	, m_type(type)
	, m_param(param)
{

}

void sys_notice_t::load_data(const proto::common::sys_notity_single& single)
{
	m_uid = common::string_util_t::string_to_uint64(single.uid());
	m_type = single.type();
	m_param = single.param();
	m_count = single.count();
}

void sys_notice_t::peek_data(proto::common::sys_notity_single* p_single)
{
	if (NULL == p_single)
	{
		log_error("NULL == p_single");
		return;
	}

	p_single->set_uid(common::string_util_t::uint64_to_string(m_uid));
	p_single->set_type(m_type);
	p_single->set_param(m_param);
	p_single->set_count(m_count);
}
