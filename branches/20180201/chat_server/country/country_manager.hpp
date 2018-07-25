#ifndef __CHAT_COUNTRY_MANAGER_HPP__
#define __CHAT_COUNTRY_MANAGER_HPP__
#include <map>
#include "country.hpp"

class country_manager_t
{
public:
	static void add_user(uint32_t country_id, uint64_t uid);
	static void earse_user(uint32_t country_id, uint64_t uid);
	static country_ptr get_country(uint32_t country_id);


	template<typename T_MSG>
	static int send_country_msg(uint32_t country_id, uint16_t cmd, const T_MSG& msg)
	{
		country_ptr p_country = get_country(country_id);
		if (p_country != nullptr)
		{
			p_country->send_country_msg(cmd, msg);
			return 0;
		}
		return 1;
	}

private:
	typedef std::map<uint32_t, country_ptr> country_map_type;
	static country_map_type m_countrys;
};

#endif // !__CHAT_COUNTRY_MANAGER_HPP__