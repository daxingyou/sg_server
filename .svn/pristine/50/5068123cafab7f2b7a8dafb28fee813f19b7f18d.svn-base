#include "family_build.hpp"
#include "utility.hpp"
#include "family_manager.hpp"

family_build::family_build(proto::common::family_build_type type, uint64_t family_id)
	: m_type(type)
	, m_family_id(family_id)
{
	m_base.set_type(type);

	m_key = common::string_util_t::uint64_to_string(family_id) + common::string_util_t::uint32_to_string((uint32_t)type);
}

void family_build::init()
{
	m_base.set_level(family_manager_t::get_build_define_level(m_type));
	m_base.set_cd_time(0);
}

void family_build::peek_data(proto::common::family_build_data* data)
{
	if (NULL != data)
	{
		data->CopyFrom(m_base);
	}
}

void family_build::upgrade(uint32_t time_now)
{
	m_base.set_level(m_base.level() + 1);
	m_base.set_cd_time(time_now);
}

void family_build::clear_cd()
{
	m_base.set_cd_time(0);
}
