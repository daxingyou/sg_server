#ifndef __CHAT_SEND_GAME_MSG_HPP__
#define __CHAT_SEND_GAME_MSG_HPP__

#include "protos_fwd.hpp"

class send_game_msg_t
{
public:
	static void send_task_seek_help_request(uint32_t game_id, uint64_t uid, uint32_t task_tid);
	static void send_get_nearby_roles_request(uint32_t game_id, uint64_t uid, uint32_t head_icon, uint32_t role_level,
											  proto::common::chat_channel channel, const std::string& chat_info, 
											  const std::string& param, const proto::common::chat_common_role_info& chat_data);
	static void send_user_channel_notify(uint32_t game_id, uint64_t uid, uint32_t channel_id);
    static void send_get_online_user_list_request();
	static void send_chat_get_role_info_request(uint32_t game_id, uint64_t uid,const proto::common::chat_common_data& chat_info,
												std::vector<proto::common::chat_param>& param_list);
};

#endif // !__CHAT_SEND_GAME_MSG_HPP__