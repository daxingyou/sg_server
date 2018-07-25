#ifndef __CDKEY_MANAGER_HPP__
#define __CDKEY_MANAGER_HPP__

#include <string>

class cdkey_manager_t 
{
public:
	static bool get_use_cdkey_requeset( uint64_t role_uid, uint32_t channel_id, const std::string &key_code );
	
	static bool do_cdkey_to_user(uint64_t role_uid, std::string &json_data);
};

#endif