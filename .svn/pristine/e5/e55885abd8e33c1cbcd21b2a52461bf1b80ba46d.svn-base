#include "send_game_msg.hpp"
#include "main/chat_server.hpp"
#include "main/chat_server_fwd.hpp"
#include "log.hpp"
#include "game/game_manager.hpp"



void send_game_msg_t::send_task_seek_help_request(uint32_t game_id, uint64_t uid, uint32_t task_tid)
{
	proto::server::hg_task_seek_help_request msg_req;
	msg_req.set_task_id(task_tid);
	env::server->send_msg_to_game(game_manager_t::get_game_socket_by_id(game_id), op_cmd::hg_task_seek_help_request, uid, msg_req);
}


void send_game_msg_t::send_get_nearby_roles_request(uint32_t game_id, uint64_t uid, uint32_t head_icon, uint32_t role_level,
													proto::common::chat_channel channel, const std::string& chat_info,
													const std::string& param, const proto::common::chat_common_role_info& chat_data)
{
	proto::server::hg_get_nearby_roles_request msg_req;
	msg_req.set_chat_channel(channel);
	msg_req.set_chat_info(chat_info);
	msg_req.set_head_icon_id(head_icon);
	msg_req.set_role_level(role_level);
	msg_req.set_param(param);
	proto::common::chat_common_role_info* p_data  = msg_req.mutable_chat_role_info();
	p_data->CopyFrom(chat_data);
	env::server->send_msg_to_game(game_manager_t::get_game_socket_by_id(game_id), op_cmd::hg_get_nearby_roles_request, uid, msg_req);
}

void send_game_msg_t::send_user_channel_notify(uint32_t game_id, uint64_t uid, uint32_t channel_id)
{
    proto::server::hg_user_channel_notify msg_ntf;
    msg_ntf.set_user_uid(uid);
    msg_ntf.set_channel_id(channel_id);
    env::server->send_msg_to_game(game_manager_t::get_game_socket_by_id(game_id), op_cmd::hg_user_channel_notify, uid, msg_ntf);
}

void send_game_msg_t::send_get_online_user_list_request()
{
	proto::server::hg_syn_online_role_list_request msg_req;
	env::server->send_msg_to_all_games(op_cmd::hg_syn_online_role_list_request, msg_req);
}

void send_game_msg_t::send_chat_get_role_info_request(uint32_t game_id, uint64_t uid,const proto::common::chat_common_data& chat_info, 
														std::vector<proto::common::chat_param>& param_list)
{
	proto::server::hg_chat_get_role_info_request msg_req;
	if (param_list.empty())
	{
		log_error("role[%lu] chat data get role info chat_param empty",uid);
		return;
	}
	for (auto& it : param_list)
	{
		proto::common::chat_param* p_param = msg_req.add_param_list();
		p_param->CopyFrom(it);
	}
	proto::common::chat_common_data* p_chat_data = msg_req.mutable_chat_notify();
	p_chat_data->CopyFrom(chat_info);
	env::server->send_msg_to_game(game_manager_t::get_game_socket_by_id(game_id), op_cmd::hg_chat_get_role_info_request, uid, msg_req);
}
