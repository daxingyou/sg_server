#include "family_manager.hpp"

family_manager_t::family_map_type family_manager_t::m_familys;
void family_manager_t::add_user(uint64_t family_id, uint64_t uid)
{
	family_ptr p_family = get_family(family_id);
	if (p_family == nullptr)
	{
		p_family = family_ptr(new family_t(family_id));
		if (p_family != nullptr)
		{
			m_familys[family_id] = p_family;
		}
	}

	if (p_family != nullptr)
	{
		p_family->add_user(uid);
		user_ptr p_user = user_manager_t::get_user(uid);
		if (p_user != nullptr)
		{
			p_user->set_family_id(family_id);
		}
	}
}

void family_manager_t::earse_user(uint64_t family_id, uint64_t uid)
{
	family_ptr p_family = get_family(family_id);
	if (p_family != nullptr)
	{
		p_family->erase_user(uid);
	}
}

void family_manager_t::clear_all_family()
{
	m_familys.clear();
}

family_ptr family_manager_t::get_family(uint64_t family_id)
{
	auto it = m_familys.find(family_id);
	if (it != m_familys.end())
	{
		return (it->second);
	}
	return family_ptr();
}