#include "user_manager.hpp"
#include "protos_fwd.hpp"
#include "country/country_manager.hpp"
#include "family/family_manager.hpp"
//#include "offline_friend_msg_mgr/offline_friend_msg_mgr.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

user_manager_t::user_map_type user_manager_t::m_user_list;

bool user_manager_t::add_user(user_ptr p_user, uint32_t gate_id, uint32_t game_id)
{
    if (NULL == p_user)
    {
        log_error("NULL == p_user");
        return false;
    }
    p_user->set_gate_id(gate_id);
    p_user->set_game_id(game_id);
    m_user_list.insert(user_map_type::value_type(p_user->get_uid(), p_user));

    return true;
}

user_ptr user_manager_t::get_user(uint64_t uid)
{
    user_map_type::iterator itr = m_user_list.find(uid);
    if (itr != m_user_list.end())
    {
        return (itr->second);
    }
    return user_ptr();
}

void user_manager_t::kick_user(uint64_t uid)
{
    user_map_type::iterator itr = m_user_list.find(uid);
    if (itr != m_user_list.end())
    {
        user_ptr p_user = itr->second;
        if (NULL != p_user)
        {
            m_user_list.erase(itr);
            //log_warn("delete user[%lu] from gate[%d] and game[%d]", p_user->get_uid(), p_user->get_gate_id(), p_user->get_game_id());
        }
		if (p_user->get_family_id() != 0)
		{
			family_manager_t::earse_user(p_user->get_family_id(),uid);
		}

		if (p_user->get_country_id() != 0)
		{
			country_manager_t::earse_user(p_user->get_country_id(), uid);
		}
    }
}

void user_manager_t::kick_all_users()
{
    m_user_list.clear();
    log_info("clear all users success!");
}
