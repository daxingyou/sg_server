#include "family.hpp"
#include "user/user_manager.hpp"

family_t::family_t(uint64_t uid) : m_uid(uid)
{
}

family_t::~family_t()
{
}

void family_t::add_user(uint64_t uid)
{
	m_family_users.insert(uid);
	user_ptr p_user = user_manager_t::get_user(uid);
	if (p_user != nullptr)
	{
		p_user->set_family_id(m_uid);
	}
}

void family_t::erase_user(uint64_t uid)
{
	m_family_users.erase(uid);
	user_ptr p_user = user_manager_t::get_user(uid);
	if (p_user != nullptr)
	{
		p_user->set_family_id(0);
	}
}
