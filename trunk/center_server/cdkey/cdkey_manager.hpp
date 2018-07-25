#ifndef _CDKEY_MANAGER_HPP_
#define _CDKEY_MANAGER_HPP_

#include <string>
#include "user/global_user.hpp"

class cdkey_manager_t 
{
public:
	static void do_exchange_cdkey_request(const global_user_ptr &p_user, uint32_t channel_id, const std::string &key_code );
	
	static void do_exchange_cdkey_reply( const uint64_t role_uid, uint32_t key_type, uint32_t err_code );
};




#endif