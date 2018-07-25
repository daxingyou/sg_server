#include "arena_role.hpp"

arena_role_t::arena_role_t(uint64_t role_uid)
	: m_role_uid(role_uid)
{

}

arena_role_t::~arena_role_t()
{

}

void arena_role_t::set_score(uint32_t score)
{
	m_score = score;

}

const char* arena_role_t::get_role_name()
{
	return m_cross_data.rd().name().c_str();
}

uint32_t arena_role_t::get_role_server_id()
{
	return m_cross_data.rd().server().server_id();
}

const char* arena_role_t::get_role_server_name()
{
	return m_cross_data.rd().server().server_name().c_str();
}

void arena_role_t::set_cross_data(const proto::common::role_cross_data &cross_data)
{
	m_cross_data.CopyFrom(cross_data);
}

void arena_role_t::peek_data(proto::common::role_cross_data *cross_data)
{
	if (cross_data)
		cross_data->CopyFrom(m_cross_data);
}

