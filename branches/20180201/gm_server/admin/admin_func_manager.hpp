#ifndef __ADMIN_FUNC_MANAGER_HPP__
#define __ADMIN_FUNC_MANAGER_HPP__

#include "center/center_msg_handler.hpp"

class admin_func_manager_t {
public:
	static bool send_mail_to_center(std::string &json_data);

	static bool get_user_num_from_center( uint32_t admin_id );

	static bool send_user_num_to_tools( uint32_t admin_id, uint32_t num );
};

#endif

