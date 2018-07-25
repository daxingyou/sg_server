#ifndef __CHAT_FAMILY_HPP__
#define __CHAT_FAMILY_HPP__
#include <map>
#include "user/user_manager.hpp"
#include "main/chat_server.hpp"

class family_t
{
public:
	family_t(uint64_t uid);
	~family_t();

	void add_user(uint64_t uid);
	void erase_user(uint64_t uid);

	template<typename T_MSG>
	void send_family_msg(uint16_t cmd, const T_MSG& msg, bool is_to_center = false)
	{
		for (uint64_t uid : m_family_users)
		{
			user_ptr p_user = user_manager_t::get_user(uid);
			if (p_user != nullptr)
			{
				if (is_to_center)
					p_user->send_msg_to_center(cmd, msg);
				else
					p_user->send_msg_to_client(cmd, msg);
			}
		}
	}

private:
	uint64_t m_uid;
	user_set_type m_family_users;
};

typedef boost::shared_ptr<family_t> family_ptr;

#endif // !__CHAT_FAMILY_HPP__