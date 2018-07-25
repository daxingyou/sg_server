#include "global_scene_msg_handle.hpp"

#include "gate/gate_msg_handler.hpp"
#include "game/game_msg_handler.hpp"

#include "main/center_server_fwd.hpp"
#include "main/center_server.hpp"

#include "global_scene.hpp"
#include "global_scene_manager.hpp"
#include "user/global_user_manager.hpp"
#include "tblh/errcode_enum.hpp"

bool global_scene_msg_handle_t::init_gate_msg_handler()
{
	bool bresult = true;

	return bresult;
}

bool global_scene_msg_handle_t::init_game_msg_handler()
{
	bool bresult = true;

 	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_role_scene_info_notify, handle_ge_role_scene_info_notify);
 	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_game_scene_unload_notify, handle_ge_game_scene_unload_notify);
 	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_open_dungeon_request, handle_ge_open_dungeon_request);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_create_dugeon_reply, handle_ge_create_dugeon_reply);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_scene_state_notify, handle_ge_scene_state_notify);

	return bresult;
}

bool global_scene_msg_handle_t::handle_ge_role_scene_info_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_role_scene_info_notify);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (p_user != NULL)
	{
		p_user->sync_scene_info(msg.scene_id(), msg.map_tid());
	}

	return true;
}

bool global_scene_msg_handle_t::handle_ge_game_scene_unload_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_game_scene_unload_notify);

	global_scene_manager_t::del_scene(msg.scene_id());

	return true;
}

bool global_scene_msg_handle_t::handle_ge_open_dungeon_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_open_dungeon_request);

	if (!msg.has_data())
	{
		log_error("data null error");
		return false;
	}

	global_scene_manager_t::ask_open_dungeon(msg.data());

	return true;
}

bool global_scene_msg_handle_t::handle_ge_create_dugeon_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_create_dugeon_reply);

	proto::server::eg_open_dungeon_reply reply;

	if (msg.reply_code() == common::errcode_enum::error_ok)
	{
		uint32_t socket_game_id = game_manager_t::get_game_id_by_socket(s);

		if (!global_scene_manager_t::add_one_dungeon(socket_game_id, msg))
		{
			log_error("game[%u] dungeon[%u] scene[%lu] registered error!!!!!!!!!!!!!!!!!!!!!!", socket_game_id, msg.data().dungeon_id(), msg.info().scene_id());
			reply.set_reply_code(common::errcode_enum::dungeon_create_error);
		}
		else
		{
			proto::server::dungeon_create_data* p_create_data = reply.mutable_data();
			if (p_create_data != NULL)
			{
				p_create_data->CopyFrom(msg.data());
			}

			proto::server::game_scene_reg_info* p_reg_info = reply.mutable_info();
			if (p_reg_info != NULL)
			{
				p_reg_info->CopyFrom(msg.info());
			}

			reply.set_reply_code(common::errcode_enum::error_ok);
		}
	}
	else
	{
		reply.set_reply_code(msg.reply_code());
	}

	// 创建成功
	const network::tcp_socket_ptr create_game_s = game_manager_t::get_game_socket_by_id(msg.data().game_id());
	if (NULL == create_game_s)
	{
		log_error("create game[%u] not find!!!!!!!!!!!!!", msg.data().game_id());
		return false;
	}

	if (env::server)
		env::server->send_msg_to_game(create_game_s, op_cmd::eg_open_dungeon_reply, 0, reply);

	return true;
}

bool global_scene_msg_handle_t::handle_ge_scene_state_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_scene_state_notify);

	const global_scene_ptr& p_global_scene = global_scene_manager_t::get_scene_by_id(msg.scene_id());
	if (p_global_scene == NULL)
	{
		log_error("scene[%lu] not find", msg.scene_id());
		return false;
	}

	// 设置状态
	p_global_scene->set_scene_state(msg.state());

	return true;
}
