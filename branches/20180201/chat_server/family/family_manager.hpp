#ifndef __CHAT_FAMILY_MANAGER_HPP__
#define __CHAT_FAMILY_MANAGER_HPP__
#include <map>
#include "family.hpp"

class family_manager_t
{
public:
	static void add_user(uint64_t family_id, uint64_t uid);
	static void earse_user(uint64_t family_id, uint64_t uid);
	static family_ptr get_family(uint64_t family_id);

	template<typename T_MSG>
	static int send_family_msg(uint64_t family_id, uint16_t cmd, const T_MSG& msg, bool is_to_center = false)
	{
		family_ptr p_family = get_family(family_id);
		if (p_family != nullptr)
		{
			p_family->send_family_msg(cmd, msg, is_to_center);
			return 0;
		}
		return 1;
	}

private:
	typedef std::map<uint64_t, family_ptr> family_map_type;
	static family_map_type m_familys;
};

#endif // !__CHAT_FAMILY_MANAGER_HPP__