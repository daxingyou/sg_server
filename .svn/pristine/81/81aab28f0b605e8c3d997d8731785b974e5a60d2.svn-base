#include "game_msg_handler.hpp"
#include "main/gate_server.hpp"
#include "main/gate_server_fwd.hpp"
#include "user/user_manager.hpp"
#include "config/config_manager.hpp"
#include "common/DataManager.h"

USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<op_cmd::GA_BEGIN, op_cmd::GA_END, game_msg_handler_t::msg_handler_func> game_msg_handler_t::m_game_msg_handle;
std::set<uint16_t> game_msg_handler_t::m_cross_msg_register;
uint32_t game_msg_handler_t::m_is_batch_move;

bool game_msg_handler_t::init_msg_handler()
{
    m_game_msg_handle.register_func(op_cmd::ga_role_move_notify, handle_ga_role_move_notify);
    m_game_msg_handle.register_func(op_cmd::ga_role_into_vision_notify, handle_ga_role_into_vision_notify);
    m_game_msg_handle.register_func(op_cmd::ga_role_out_vision_notify, handle_ga_role_out_vision_notify);
    m_game_msg_handle.register_func(op_cmd::ga_syn_game_info_notify, handle_ga_syn_game_info_notify);
	m_game_msg_handle.register_func(op_cmd::ga_broadcast_notify, handle_ga_broadcast_notify);
	m_game_msg_handle.register_func(op_cmd::ga_reload_config_notify, handle_ga_reload_config_notify);
	m_game_msg_handle.register_func(op_cmd::ga_syn_cross_id_notify, handle_ga_syn_cross_id_notify);
	m_game_msg_handle.register_func(op_cmd::ga_unreg_user_scene_notify, handle_ga_unreg_user_scene_notify);
	m_game_msg_handle.register_func(op_cmd::ga_reg_user_success_notify, handle_ga_reg_user_success_notify);
	m_game_msg_handle.register_func(op_cmd::ga_user_cross_server_notify, handle_ga_user_cross_server_notify);
	
	init_cross_msg_handler();
    return true;
}

bool game_msg_handler_t::init_cross_msg_handler()
{
	m_cross_msg_register.insert(op_cmd::cg_combat_loading_end_ntf);
	m_cross_msg_register.insert(op_cmd::cg_combat_form_request);
	m_cross_msg_register.insert(op_cmd::cg_combat_play_end_ntf);
	m_cross_msg_register.insert(op_cmd::cg_combat_escape_ntf);
	m_cross_msg_register.insert(op_cmd::cg_combat_auto_fight_request);
	m_cross_msg_register.insert(op_cmd::cg_combat_before_form_ready_request);
	m_cross_msg_register.insert(op_cmd::cg_combat_recovery_ask_reply);
	m_cross_msg_register.insert(op_cmd::cg_combat_mark_target_notify);
	m_cross_msg_register.insert(op_cmd::cg_combat_speak_notify);
	m_cross_msg_register.insert(op_cmd::cg_combat_ask_escape_req);
	m_cross_msg_register.insert(op_cmd::cg_combat_recovery_ask_reply);

	m_cross_msg_register.insert(op_cmd::cg_update_formation_request);
	m_cross_msg_register.insert(op_cmd::cg_update_cur_form_id_request);
	m_cross_msg_register.insert(op_cmd::cg_fight_update_formation_request);
	m_cross_msg_register.insert(op_cmd::cg_fight_update_cur_form_id_request);
	m_cross_msg_register.insert(op_cmd::cg_get_fight_formation_req);
	

	m_cross_msg_register.insert(op_cmd::gc_combat_start_loading_ntf);
	m_cross_msg_register.insert(op_cmd::gc_combat_obj_state_ntf);
	m_cross_msg_register.insert(op_cmd::gc_combat_data_ntf);
	m_cross_msg_register.insert(op_cmd::gc_combat_before_form_ready_reply);
	m_cross_msg_register.insert(op_cmd::gc_combat_reset_form_notify);
	m_cross_msg_register.insert(op_cmd::gc_combat_auto_fight_reply);
	m_cross_msg_register.insert(op_cmd::gc_combat_recovery_notify);
	m_cross_msg_register.insert(op_cmd::gc_combat_result_ntf);
	m_cross_msg_register.insert(op_cmd::gc_combat_mark_target_notify);
	m_cross_msg_register.insert(op_cmd::gc_combat_speak_notify);
	m_cross_msg_register.insert(op_cmd::gc_combat_escape_ntf);
	

	m_cross_msg_register.insert(op_cmd::gc_update_formation_reply);
	m_cross_msg_register.insert(op_cmd::gc_update_cur_form_id_reply);
	m_cross_msg_register.insert(op_cmd::gc_fight_update_formation_reply);
	m_cross_msg_register.insert(op_cmd::gc_fight_update_cur_form_id_reply);
	m_cross_msg_register.insert(op_cmd::gc_get_fight_special_heroes_reply);

	m_cross_msg_register.insert(op_cmd::gc_arena_match_reply);

	return true;
}


void game_msg_handler_t::set_is_batch_move(uint32_t val)
{
	m_is_batch_move = val;
}

bool game_msg_handler_t::check_cross_msg(uint16_t cmd)
{
	if (m_cross_msg_register.find(cmd) != m_cross_msg_register.end())
	{
		return true;
	}

	return false;
}

bool game_msg_handler_t::handle_ga_role_move_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG(proto::server::ga_role_move_notify);

	uint64_t curr_time = 0;
	if (m_is_batch_move) curr_time = common::time_util_t::now_msecs();
    for (int32_t i = 0; i < msg.role_list_size(); ++i) {
        user_ptr p_user = user_manager_t::get_user(msg.role_list(i));
		if (p_user != NULL) {
			if (m_is_batch_move) p_user->send_move_to_client(msg.msg(), curr_time);
			else p_user->send_msg_to_client(op_cmd::gc_move_notify, msg.msg());
		}
    }
    return true;
}


bool game_msg_handler_t::handle_ga_role_into_vision_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG(proto::server::ga_role_into_vision_notify);
    proto::client::gc_role_into_vision_notify msg_to_client;
    msg_to_client.CopyFrom(msg.msg());
    for (int32_t i = 0; i < msg.role_list_size(); ++i)
    {
        user_ptr p_user = user_manager_t::get_user(msg.role_list(i));
		// 目前是所有gate都发，gate去做筛选
        if (NULL == p_user)
        {
            //log_warn("NULL == p_user[%lu]", user_id);
            continue;
        }
        p_user->send_msg_to_client(op_cmd::gc_role_into_vision_notify, msg_to_client);
    }
    return true;
}


bool game_msg_handler_t::handle_ga_role_out_vision_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG(proto::server::ga_role_out_vision_notify);
    proto::client::gc_role_outo_vision_notify msg_to_client;
    msg_to_client.CopyFrom(msg.msg());
    for (int32_t i = 0; i < msg.role_list_size(); ++i)
    {
        user_ptr p_user = user_manager_t::get_user(msg.role_list(i));
		// 目前是所有gate都发，gate去做筛选
        if (NULL == p_user)
        {
            //log_warn("NULL == p_user[%lu]", user_id);
            continue;
        }
        p_user->send_msg_to_client(op_cmd::gc_role_outo_vision_notify, msg_to_client);
    }
    return true;
}

bool game_msg_handler_t::handle_ga_syn_game_info_notify(const network::tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG(proto::server::ga_syn_game_info_notify);

    uint32_t game_id = msg.game_id();
	uint32_t game_type = msg.game_type();
    game_manager_t::set_game(s, game_id, game_type);

    return true;
}

#define BROADCAST_UID_MSG(msg_type) \
	msg_type msg_to_client; \
    network::unwrap_msg(msg_to_client, msg_data); \
	for (int32_t i = 0; i < msg.role_list_size(); ++i) \
	{ \
		user_ptr p_user = user_manager_t::get_user(msg.role_list(i)); \
		if (NULL == p_user) \
		{ \
			continue; \
		} \
		p_user->send_msg_to_client(msg_cmd, msg_to_client); \
	}

#define BROADCAST_ALL_MSG(msg_type) \
	msg_type msg_to_client; \
    network::unwrap_msg(msg_to_client, msg_data); \
	user_manager_t::send_to_all_users(msg_cmd, msg_to_client);

bool game_msg_handler_t::handle_ga_broadcast_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ga_broadcast_notify);

	uint16_t msg_cmd = (uint16_t)msg.cmd();
	const std::string& msg_data = msg.data();

	switch (msg_cmd)
	{
	case op_cmd::gc_field_boss_pos_info_notify:
		{
			BROADCAST_UID_MSG(proto::client::gc_field_boss_pos_info_notify);
		}
		break;
	case op_cmd::gc_system_hint_notify:
		{
			BROADCAST_UID_MSG(proto::client::gc_system_hint_notify);
		}
		break;
	case op_cmd::gc_change_sys_state_notify:
		{
			BROADCAST_UID_MSG(proto::client::gc_change_sys_state_notify);
		}
		break;
	case op_cmd::gc_sys_notice_notify:
		{
			if (msg.role_list_size() > 0)
			{
				BROADCAST_UID_MSG(proto::client::gc_sys_notice_notify);
			}
			else
			{
				BROADCAST_ALL_MSG(proto::client::gc_sys_notice_notify);
			}
		}
		break;
	case op_cmd::gc_scene_object_info_notify:
		{
			BROADCAST_UID_MSG(proto::client::gc_scene_object_info_notify);
		}
		break;
	case op_cmd::gc_update_long_info_notify:
		{
			BROADCAST_UID_MSG(proto::client::gc_update_long_info_notify);
		}
		break;
	case op_cmd::gc_king_war_scene_start_notify:
		{
			BROADCAST_UID_MSG(proto::client::gc_king_war_scene_start_notify);
		}
		break;
	case op_cmd::gc_artifact_left_time_notify:
		{
			BROADCAST_UID_MSG(proto::client::gc_artifact_left_time_notify);
		}
		break;
	case op_cmd::gc_object_hp_change_notify:
		{
			BROADCAST_UID_MSG(proto::client::gc_object_hp_change_notify);
		}
		break;
	case op_cmd::gc_dungeon_schedule_data_notify:
		{
			BROADCAST_UID_MSG(proto::client::gc_dungeon_schedule_data_notify);
		}
		break;
	case op_cmd::gc_notify_dungeon_result_notify:
		{
			BROADCAST_UID_MSG(proto::client::gc_notify_dungeon_result_notify);
		}
		break;
	case op_cmd::gc_object_scene_jump_notify:
		{
			BROADCAST_UID_MSG(proto::client::gc_object_scene_jump_notify);
		}
		break;
	case op_cmd::gc_family_war_fight_info_notify:
		{
			BROADCAST_UID_MSG(proto::client::gc_family_war_fight_info_notify);
		}
		break;
	case op_cmd::gc_long_hurt_flag_notify:
		{
			BROADCAST_UID_MSG(proto::client::gc_long_hurt_flag_notify);
		}
		break;
	case op_cmd::gc_platform_hurt_flag_notify:
		{
			BROADCAST_UID_MSG(proto::client::gc_platform_hurt_flag_notify);
		}
		break;
	default:
		{
			log_error("op_cmd is unknown! cmd=%d", msg_cmd);
			return false;
		}
		break;
	}

	return true;
}

bool game_msg_handler_t::handle_ga_reload_config_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	reloadAllDTManager(env::xml->config_path);
	return true;
}

bool game_msg_handler_t::handle_ga_syn_cross_id_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ga_syn_cross_id_notify);
	user_ptr p_user = user_manager_t::get_user(msg.user_id());
	if (NULL != p_user)
	{
		p_user->set_cross_id(msg.cross_id());
	}

	return true;
}

bool game_msg_handler_t::handle_ga_unreg_user_scene_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ga_unreg_user_scene_notify);

	const user_ptr& p_user = user_manager_t::get_user(uid);
	if (NULL != p_user)
	{
		switch (msg.unreg_code())
		{
		case common::UNREGUSER_RET_ERROR:		// 发生错误，直接与客户端断开，并且下线user
			{
				if (p_user->is_offline_state() || p_user->is_unreg_state()) // 已经下线或者注销
				{
					log_warn("user[%lu] is already offline or unreg", uid);
				}
				else
				{
					log_error("user[%lu] is unreg[%u] error, state[%u]", uid, msg.unreg_code(), p_user->get_game_state());

					if (NULL != p_user->get_client_socket())
					{
						user_manager_t::del_socket(p_user->get_client_socket(), true);
					}
					else
					{
						user_manager_t::del_user(uid);
					}
				}
			}
			break;
		default:
			log_error("user[%lu] error unreg_code[%u]", uid, msg.unreg_code());
			break;
		}
	}
	else
	{
		log_warn("user[%lu] unreg not find in gate[%u]", uid, env::server->get_id());
	}

	return true;
}

bool game_msg_handler_t::handle_ga_reg_user_success_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ga_reg_user_success_notify);

	const user_ptr& p_user = user_manager_t::get_user(uid);
	if (NULL != p_user)
	{
		if (p_user->is_login_state())
		{
			p_user->set_play_state();

			game_manager_t::add_user_to_game(s, uid);

			// 场景登陆通知cross是否有未完成的战斗
			proto::server::ag_role_enter_game_notify ntf;
			env::server->send_to_all_crosses(op_cmd::ag_role_enter_game_notify, uid, ntf);

			ROLE_LOG("gate[%u] user[%lu] login game[%u] success!", env::server->get_id(), uid, game_manager_t::get_game_id_by_socket(s));

			return true;
		}
		else if (p_user->is_wait_state())
		{
			p_user->set_play_state();

			game_manager_t::add_user_to_game(s, uid);

			ROLE_LOG("gate[%u] user[%lu] change game[%u] success!", env::server->get_id(), uid, game_manager_t::get_game_id_by_socket(s));

			return true;
		}
		else if (p_user->is_offline_state())
		{
			user_manager_t::notify_others_unreg_user(p_user, common::UNREGUSER_RET_OFFLINE);

			ROLE_LOG("gate[%u] user[%lu] is already offline!", env::server->get_id(), uid);

			return true;
		}
		else
		{
			log_error("gate[%u] user[%lu] state[%u] error, when recv[ga_reg_user_success_notify]", env::server->get_id(), uid, p_user->get_game_state());

			if (NULL != p_user->get_client_socket())
			{
				user_manager_t::del_socket(p_user->get_client_socket(), true);
			}
			else
			{
				user_manager_t::del_user(uid);
			}
		}
	}
	else
	{
		log_warn("user[%lu] from_gate[%u] unreg not find in gate[%u]", uid, msg.gate_id(), env::server->get_id());
	}

	proto::server::ae_unreg_user_scene_notify ae_ret;
	ae_ret.set_uid(uid);
	ae_ret.set_unreg_code(common::UNREGUSER_RET_ERROR);
	ae_ret.set_kick_reason(common::KICK_ROLE_REASON_ERROR);
	env::server->send_msg_to_center(op_cmd::ae_unreg_user_scene_notify, uid, ae_ret);

	proto::server::ag_unreg_user_scene_notify ag_ret;
	ag_ret.set_uid(uid);
	ag_ret.set_unreg_code(common::UNREGUSER_RET_ERROR);
	ag_ret.set_kick_reason(common::KICK_ROLE_REASON_ERROR);
	env::server->send_msg_to_game(s, op_cmd::ag_unreg_user_scene_notify, uid, ag_ret);

	return true;
}

bool game_msg_handler_t::handle_ga_user_cross_server_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ga_user_cross_server_notify);

	const user_ptr& p_user = user_manager_t::get_user(uid);
	if (p_user == NULL)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	if (!p_user->is_play_state())
	{
		log_error("user[%lu] state[%u] error when cross server", uid, p_user->get_game_state());
		return false;
	}

	// 通知center跨服
	proto::server::ae_user_cross_server_notify notify;
	proto::server::cross_server_data* p_data = notify.mutable_data();
	if (p_data) p_data->CopyFrom(msg.data());
	env::server->send_msg_to_center(op_cmd::ae_user_cross_server_notify, uid, notify);

	// 设置跨服状态
	p_user->set_wait_state();

	// 把玩家从当前game列表删除
	game_manager_t::del_user_from_game(uid);

	return true;
}
