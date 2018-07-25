#pragma once

#include "main/game_server.hpp"
#include "main/game_server_fwd.hpp"
#include "msg_handler.hpp"
#include "protos_fwd.hpp"
#include "role/role_manager.hpp"
#include "cmd_desc.hpp"

class chat_msg_manager_t
{
public:
	static bool chat_get_role_info(const uint64_t role_uid,proto::common::chat_common_role_info* p_data,std::vector<proto::common::chat_param>& param_vec);

};
