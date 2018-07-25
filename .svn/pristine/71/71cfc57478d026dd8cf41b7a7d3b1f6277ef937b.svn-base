#ifndef __CHAT_USER_MANAGER_HPP__
#define __CHAT_USER_MANAGER_HPP__

#include <map>

#include "user.hpp"

class user_manager_t
{
public:
    user_manager_t() {}
    ~user_manager_t() {}


    static bool add_user(user_ptr p_user, uint32_t gate_id, uint32_t game_id);
    static user_ptr get_user(uint64_t uid);
    static void kick_user(uint64_t uid);
    static void kick_all_users();

	template<typename T_MSG>
	static void send_to_all_users(uint16_t cmd, const T_MSG& msg)
	{
		user_map_type::iterator itr = m_user_list.begin();
		user_map_type::iterator itr_end = m_user_list.end();
		for (; itr != itr_end; ++itr)
		{
			user_ptr p_user = itr->second;
			if (NULL == p_user)
			{
				continue;
			}
			p_user->send_msg_to_client(cmd, msg);
		}
	}

private:
    typedef std::map<uint64_t, user_ptr> user_map_type;
    static user_map_type m_user_list;
};

#endif
