#include "country.hpp"

country_t::country_t(uint32_t uid) : m_uid(uid)
{
}

country_t::~country_t()
{
}

void country_t::add_user(uint64_t uid)
{
	m_country_users.insert(uid);
}

void country_t::erase_user(uint64_t uid)
{
	m_country_users.erase(uid);
}