#ifndef __CHAT_COUNTRY_HPP__
#define __CHAT_COUNTRY_HPP__
#include <map>
#include "user/user_manager.hpp"
#include "main/chat_server.hpp"

typedef std::map<uint64_t, user_ptr> user_map_type;

class country_t
{
public:
	country_t(uint32_t uid);
	~country_t();

	void add_user(uint64_t uid);
	void erase_user(uint64_t uid);

	template<typename T_MSG>
	void send_country_msg(uint16_t cmd, const T_MSG& msg)
	{
		for (uint64_t uid : m_country_users)
		{
			user_ptr p_user = user_manager_t::get_user(uid);
			if (p_user != nullptr)
			{
				p_user->send_msg_to_client(cmd, msg);
			}
		}
	}
private:
	uint32_t m_uid;
	user_set_type m_country_users;
};
typedef boost::shared_ptr<country_t> country_ptr;

#endif // !__CHAT_COUNTRY_HPP__