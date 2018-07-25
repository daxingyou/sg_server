#include "country_manager.hpp"
#include "user/user_manager.hpp"

country_manager_t::country_map_type country_manager_t::m_countrys;
void country_manager_t::add_user(uint32_t country_id, uint64_t uid)
{
	country_ptr p_country = get_country(country_id);
	if (p_country == nullptr)
	{
		p_country = country_ptr(new country_t(country_id));

		if (p_country != nullptr)
		{
			m_countrys.insert(std::make_pair(country_id, p_country));
		}
	}
	if (p_country != nullptr)
	{
		p_country->add_user(uid);
		user_ptr p_user = user_manager_t::get_user(uid);
		if (p_user != nullptr)
		{
			p_user->set_country_id(country_id);
		}
	}
}

void country_manager_t::earse_user(uint32_t country_id, uint64_t uid)
{
	country_ptr p_country = get_country(country_id);
	if (p_country != nullptr)
	{
		p_country->erase_user(uid);
		user_ptr p_user = user_manager_t::get_user(uid);
		if (p_user != nullptr)
		{
			p_user->set_country_id(0);
		}
	}
}

country_ptr country_manager_t::get_country(uint32_t country_id)
{
	auto it = m_countrys.find(country_id);
	if (it != m_countrys.end())
	{
		return it->second;
	}
	return country_ptr();
}