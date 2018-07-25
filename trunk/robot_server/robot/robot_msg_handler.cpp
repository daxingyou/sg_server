#include "robot_msg_handler.hpp"

#include "common/log.hpp"
#include "main/robot_server.hpp"
#include "network/protobuf.hpp"
#include "protocpp/client.pb.h"
#include "robot.hpp"
#include "robot_manager.hpp"
#include "cmd_desc.hpp"
#include "utility.hpp"
#include "tblh/errcode_enum.hpp"
#include "msg_count.hpp"
#include "network/command.hpp"

USING_NS_COMMON;
USING_NS_NETWORK;

network::server_msg_handler_t<op_cmd::NC_BEGIN, op_cmd::NC_END, robot_msg_handler_t::msg_handler_func> robot_msg_handler_t::m_msg_handler_for_account;
network::server_msg_handler_t<op_cmd::LC_BEGIN, op_cmd::LC_END, robot_msg_handler_t::msg_handler_func> robot_msg_handler_t::m_msg_handler_for_login;
network::server_msg_handler_t<op_cmd::AC_BEGIN, op_cmd::AC_END, robot_msg_handler_t::msg_handler_func> robot_msg_handler_t::m_msg_handler_for_gate;
network::server_msg_handler_t<op_cmd::GC_BEGIN, op_cmd::GC_END, robot_msg_handler_t::msg_handler_func> robot_msg_handler_t::m_msg_handler_for_game;
network::server_msg_handler_t<op_cmd::HC_BEGIN, op_cmd::HC_END, robot_msg_handler_t::msg_handler_func> robot_msg_handler_t::m_msg_handler_for_chat;
network::server_msg_handler_t<op_cmd::EC_BEGIN, op_cmd::EC_END, robot_msg_handler_t::msg_handler_func> robot_msg_handler_t::m_msg_handler_for_center;

bool robot_msg_handler_t::init_msg_handler()
{
    bool ret = true;
    // from account
	ret &= m_msg_handler_for_account.register_func(op_cmd::nc_server_info_reply, handle_nc_server_info_reply);

    // from login
    ret &= m_msg_handler_for_login.register_func(op_cmd::lc_login_reply,				handle_lc_login_reply);
	ret &= m_msg_handler_for_login.register_func(op_cmd::lc_login_queue_notify,			handle_lc_login_queue_notify);
    
    // from gate
    ret &= m_msg_handler_for_gate.register_func(op_cmd::ac_connect_reply,				handle_ac_connect_reply);
    ret &= m_msg_handler_for_gate.register_func(op_cmd::ac_kick_role_notify,			handle_ac_kick_role_notify);
	ret &= m_msg_handler_for_gate.register_func(op_cmd::ac_pingpong_reply,				handle_ac_pingpong_reply);
	ret &= m_msg_handler_for_gate.register_func(op_cmd::ac_login_game_reply,			handle_ac_login_game_reply);
	ret &= m_msg_handler_for_gate.register_func(op_cmd::ac_create_role_reply,			handle_ac_create_role_reply);
    
    // from game
    ret &= m_msg_handler_for_game.register_func(op_cmd::gc_enter_game_reply,			handle_gc_enter_game_reply);
    ret &= m_msg_handler_for_game.register_func(op_cmd::gc_enter_scene_notify,			handle_gc_enter_scene_notify);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_jump_scene_reply,			handle_gc_jump_scene_reply);
    ret &= m_msg_handler_for_game.register_func(op_cmd::gc_area_id_notify,				handle_gc_area_id_notify);
    ret &= m_msg_handler_for_game.register_func(op_cmd::gc_create_family_reply,			handle_gc_create_family_reply);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_role_into_vision_notify,		handle_gc_role_into_vision_notify);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_role_outo_vision_notify,		handle_gc_role_outo_vision_notify);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_scene_object_info_notify,	handle_gc_scene_object_info_notify);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_accept_task_reply,			handle_gc_accept_task_reply);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_commit_task_reply,			handle_gc_commit_task_reply);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_task_change_ntf,				handle_gc_task_change_notify);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_help_task_reply,				handle_gc_help_task_reply);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_hero_cultivate_reply,		handle_gc_hero_cultivate_reply);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_hero_recruit_reply,			handle_gc_hero_recruit_reply);
	//ret &= m_msg_handler_for_game.register_func(op_cmd::gc_send_luck_gift_reply,		handle_gc_send_luck_gift_reply);
	//ret &= m_msg_handler_for_game.register_func(op_cmd::gc_grab_luck_gift_reply,		handle_gc_grab_luck_gift_reply);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_move_notify,					handle_gc_move_notify);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_batch_move_notify,			handle_gc_batch_move_notify);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_enter_scene_reply,			handle_gc_enter_scene_reply);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_system_hint_notify,			handle_gc_system_hint_notify);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_gm_cmd_reply,				handle_gc_gm_cmd_reply);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_trade_buy_item_reply,		handle_gc_trade_buy_item_reply);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_trade_sell_item_reply,		handle_gc_trade_sell_item_reply);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_get_city_trade_shop_reply,	handle_gc_get_city_trade_shop_reply);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_next_day_notify,				handle_gc_next_day_notify);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_use_mount_reply,				handle_gc_use_mount_reply);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_country_bounty_data_reply,	handle_gc_country_bounty_data_reply);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_country_bounty_accept_reply, handle_gc_country_bounty_accept_reply);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_country_bounty_finish_ntf,	handle_gc_country_bounty_finish_ntf);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_country_bounty_refresh_reply,handle_gc_country_bounty_refresh_reply);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_role_change_data_notify,		handle_gc_role_change_data_notify);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_mail_get_addenda_reply, handle_gc_mail_get_addenda_reply);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_sys_init_notice_notify, handle_gc_sys_init_notice_notify);
    ret &= m_msg_handler_for_game.register_func(op_cmd::gc_sys_notice_notify, handle_gc_sys_notice_notify);
    ret &= m_msg_handler_for_game.register_func(op_cmd::gc_red_point_notify, handle_gc_red_point_notify);
    ret &= m_msg_handler_for_game.register_func(op_cmd::gc_update_fighting_notify, handle_gc_update_fighting_notify);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_open_dungeon_reply , handle_gc_open_dungeon_reply);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_dungeon_schedule_data_notify, handle_gc_dungeon_schedule_data_notify);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_notify_dungeon_result_notify, handle_gc_notify_dungeon_result_notify);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_combat_start_loading_ntf, handle_gc_combat_start_loading_ntf);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_combat_data_ntf, handle_gc_combat_data_ntf);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_combat_obj_state_ntf, handle_gc_combat_obj_state_ntf);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_enter_fight_reply, handle_gc_enter_fight_reply);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_combat_auto_fight_reply, handle_gc_combat_auto_fight_reply);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_combat_before_form_ready_reply, handle_gc_combat_before_form_ready_reply);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_combat_reset_form_notify, handle_gc_combat_reset_form_notify);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_combat_result_ntf, handle_gc_combat_result_ntf);
    ret &= m_msg_handler_for_game.register_func(op_cmd::gc_achieve_finish_notify, handle_gc_achieve_finish_notify);
    ret &= m_msg_handler_for_game.register_func(op_cmd::gc_active_reward_finish_notify, handle_gc_active_reward_finish_notify);

	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_enter_game_for_hero_reply, handle_gc_enter_game_for_hero_reply);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_enter_game_for_item_reply, handle_gc_enter_game_for_item_reply);

    // from chat
    ret &= m_msg_handler_for_chat.register_func(op_cmd::hc_test_reply,						handle_hc_test_reply);
	ret &= m_msg_handler_for_chat.register_func(op_cmd::hc_channel_chat_reply,				handle_hc_channel_chat_reply);
	ret &= m_msg_handler_for_chat.register_func(op_cmd::hc_channel_chat_notify,				handle_hc_channel_chat_notify);
	ret &= m_msg_handler_for_chat.register_func(op_cmd::hc_get_server_channel_list_reply,	handle_hc_get_server_channel_list_reply);
	ret &= m_msg_handler_for_chat.register_func(op_cmd::hc_change_server_channel_reply,		handle_hc_change_server_channel_reply);
	ret &= m_msg_handler_for_chat.register_func(op_cmd::hc_change_server_channel_notify,	handle_hc_change_server_channel_notify);
	ret &= m_msg_handler_for_chat.register_func(op_cmd::hc_server_channel_chat_reply,		handle_hc_server_channel_chat_reply);
	ret &= m_msg_handler_for_chat.register_func(op_cmd::hc_server_channel_chat_notify,		handle_hc_server_channel_chat_notify);

	// from center
	//ret &= m_msg_handler_for_center.register_func(op_cmd::ec_send_luck_gift_notify,			handle_ec_send_luck_gift_notify);

	ret &= m_msg_handler_for_center.register_func(op_cmd::ec_new_mail_notify, handle_ec_new_mail_notify);
	ret &= m_msg_handler_for_center.register_func(op_cmd::ec_mail_list_notify, handle_ec_mail_list_notify);
	ret &= m_msg_handler_for_center.register_func(op_cmd::ec_mail_state_change_notify, handle_ec_mail_state_change_notify);
	ret &= m_msg_handler_for_center.register_func(op_cmd::ec_mail_content_reply, handle_ec_mail_content_reply);
	ret &= m_msg_handler_for_center.register_func(op_cmd::ec_get_friends_reply, handle_null_handle_msg);
	ret &= m_msg_handler_for_center.register_func(op_cmd::ec_get_black_friends_reply, handle_null_handle_msg);
	//ret &= m_msg_handler_for_center.register_func(op_cmd::ec_country_open_notify, handle_null_handle_msg);

	//世界boss
	ret &= m_msg_handler_for_center.register_func(op_cmd::ec_field_boss_info_notify, handle_ec_field_boss_info_notify);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_field_boss_attack_reply, handle_gc_field_boss_attack_reply);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_sync_enemy_hurt_hp, handle_null_handle_msg);

	//跨服战斗
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_arena_join_reply, handle_gc_arena_join_reply);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_arena_match_reply, handle_gc_arena_match_reply);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_arena_battle_result_notify, handle_gc_arena_battle_result_notify);

	//商城购买
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_buy_month_card_reply, handle_gc_buy_month_card_reply);

	// 无用消息的处理
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_field_boss_pos_info_notify, handle_null_handle_msg);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_update_function_code_ntf, handle_null_handle_msg);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_update_npc_code_ntf, handle_null_handle_msg);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_change_sys_state_notify, handle_null_handle_msg);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_reconnect_combat_result_ntf, handle_null_handle_msg);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_dungeon_reward_limit_notify, handle_null_handle_msg);
    ret &= m_msg_handler_for_game.register_func(op_cmd::gc_trade_info_notify, handle_null_handle_msg);
    ret &= m_msg_handler_for_game.register_func(op_cmd::gc_touch_new_role_guide_notify, handle_null_handle_msg);
    ret &= m_msg_handler_for_game.register_func(op_cmd::gc_update_formation_reply, handle_null_handle_msg);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_section_online_reward_notify, handle_null_handle_msg);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_role_buff_update_ntf, handle_null_handle_msg);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_section_task_finish_notify, handle_null_handle_msg);
    ret &= m_msg_handler_for_game.register_func(op_cmd::gc_level_reward_info_notify, handle_null_handle_msg);
    ret &= m_msg_handler_for_game.register_func(op_cmd::gc_init_active_info_reply, handle_null_handle_msg);
    ret &= m_msg_handler_for_game.register_func(op_cmd::gc_combat_loading_end_reply, handle_null_handle_msg);
	ret &= m_msg_handler_for_game.register_func(op_cmd::gc_active_update_reply, handle_null_handle_msg); 
    return ret;
}


bool robot_msg_handler_t::handle_server_msg(robot_ptr robot, uint16_t cmd, const network::msg_buf_ptr& msg_buf)
{
    if ( op_cmd::ac_pingpong_reply != cmd && op_cmd::gc_role_into_vision_notify != cmd )
    {
        log_trace("recv role[%lu] cmd[%s:%d] from server!", robot->get_uid(), CMD_DESC(cmd), cmd);
    }

	msg_count_t::push_msg_recv_info(msg_buf, cmd, env::robot_config->wlog_msg_count);

    msg_handler_func func = NULL;
	if (cmd > op_cmd::NC_BEGIN && cmd < op_cmd::NC_END)
	{
		func = m_msg_handler_for_account.find_func(cmd);
	}
    else if (cmd > op_cmd::LC_BEGIN && cmd < op_cmd::LC_END)
    {
        func = m_msg_handler_for_login.find_func(cmd);
    }
    else if (cmd > op_cmd::AC_BEGIN && cmd < op_cmd::AC_END)
    {
        func = m_msg_handler_for_gate.find_func(cmd);
    }
    else if (cmd > op_cmd::HC_BEGIN && cmd < op_cmd::HC_END)
    {
        func = m_msg_handler_for_chat.find_func(cmd);
    }
    else if (cmd > op_cmd::GC_BEGIN && cmd < op_cmd::GC_END)
    {
        func = m_msg_handler_for_game.find_func(cmd);
	}
	else if (cmd > op_cmd::EC_BEGIN && cmd < op_cmd::EC_END)
	{
		func = m_msg_handler_for_center.find_func(cmd);
	}
    else
    {
        // COMMON_ASSERT(false);
    }
    
    if (func)
    {
        func(robot, msg_buf);
        return true;
    }

    log_error("couldn't find cmd[%s:%d]'s msg handler!", CMD_DESC(cmd), cmd);
    return false;
}

bool robot_msg_handler_t::handle_null_handle_msg(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	return true;
}

// from account ==================================================================
bool robot_msg_handler_t::handle_nc_server_info_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2C_MSG(proto::client::nc_server_info_reply);

	robot->set_login_state(LOGIN_STATE_LOGIN_PRE2);

	if (msg.reply_code() > 0)
	{
		log_error("robot[%lu] login account_server failed! reply_code: %d", robot->get_robot_id(), msg.reply_code());
	}
	else
	{
		//log_debug("robot[%lu] login account_server successfully, get server num[%d]", robot->get_robot_id(), msg.server_list_size());

		std::string login_ip = "";
		uint32_t login_port = 0;
		uint64_t role_uid = 0;
		for (uint16_t i = 0; i < msg.server_list_size(); ++i)
		{
			const proto::common::server_info& server_info = msg.server_list(i);
            log_debug("server[id=%d name=%s ip=%s port=%d role_num=%d]", server_info.server_id(), server_info.server_name().c_str(), server_info.login_ip().c_str(), server_info.login_port(), server_info.role_list_size());
			if (server_info.server_id() == env::robot_config->server_group_id)
			{
				login_ip = server_info.login_ip();
				login_port = server_info.login_port();
				if (server_info.role_list_size() == 0)
				{
                    role_uid = string_util_t::string_to_uint64(server_info.new_role_id());
				}
				else
				{
                    role_uid = string_util_t::string_to_uint64(server_info.role_list(0).uid());
				}
			}
		}
		if (login_ip.empty() || login_port == 0 || role_uid == 0)
		{
			log_error("server_id[%u] not find !", env::robot_config->server_group_id);
			return false;
		}

		robot->set_uid(role_uid);
		robot->set_login_state(LOGIN_STATE_LOGIN_ACCOUNT);

		robot->disconnect();
		if (!robot->connect(login_ip, login_port))
		{
			log_error("robot[%s:%lu] connect to login_server[%d:%s:%d] failed!", robot->get_guid().c_str(), robot->get_uid(), env::robot_config->server_group_id, login_ip.c_str(), login_port);
			return false;
		}
		log_debug("robot[%s:%lu] connect to login_server[%d:%s:%d] success!", robot->get_guid().c_str(), robot->get_uid(), env::robot_config->server_group_id, login_ip.c_str(), login_port);
	}
	return true;
}

// from login ==================================================================
bool robot_msg_handler_t::handle_lc_login_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
    PRE_S2C_MSG(proto::client::lc_login_reply);
    
	if (msg.reply_code() > 0)
	{
		log_error("robot[%lu] login login_server failed! reply_code: %d", robot->get_robot_id(), msg.reply_code());
	}
	else
	{
		//log_debug("robot[%lu] login login_server successfully!", robot->get_robot_id());

		robot->set_login_state(LOGIN_STATE_LOGIN_LOGIN);
		robot->set_session_id(msg.general().session_id());
		robot->disconnect();

		//log_debug("robot[%lu] connect to gate_server...", robot->get_robot_id());
		if (!robot->connect(msg.gate_ip(), msg.gate_port()))
		{
			log_error("robot[%lu] connect to gate[%s:%d] failed!", robot->get_robot_id(), msg.gate_ip().c_str(), msg.gate_port());
			return false;
		}
	}
    
    return true;
}

bool robot_msg_handler_t::handle_lc_login_queue_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2C_MSG(proto::client::lc_login_queue_notify);
	log_debug("robot[%lu] login queueing, pos[%d]", robot->get_robot_id(), msg.pos());

	return true;
}

// from gate ===================================================================
bool robot_msg_handler_t::handle_ac_connect_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
    PRE_S2C_MSG(proto::client::ac_connect_reply);
    //log_debug("robot[%lu] get ac_connect_reply from gate! reply_code=%d", 
     //   robot->get_robot_id(), msg.reply_code());
    
    if (0 == msg.reply_code())
    {
        robot->set_login_state(LOGIN_STATE_LOGIN_GATE);

        robot->enter_game();
    }
    else
    {
        log_error("login gate_server failed!");
    }

    return true;
}

bool robot_msg_handler_t::handle_ac_kick_role_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
//     PRE_S2C_MSG(proto::client::ac_kick_role_notify);
// 
//     robot->disconnect();

    return true;
}

bool robot_msg_handler_t::handle_ac_pingpong_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	return true;
}

// from game ==================================================================
bool robot_msg_handler_t::handle_gc_enter_game_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
    PRE_S2C_MSG(proto::client::gc_enter_game_reply);

    if (msg.reply_code() == 0)
    {
		//log_debug("robot[%lu] uid:%lu enter game success!", robot->get_robot_id(), robot->get_uid());
		robot->set_login_state(LOGIN_STATE_LOAD_ROLE);
		robot->update_robot_info(msg);
    }
	else
	{
		log_error("robot[%lu] gc_enter_game_reply error", robot->get_robot_id());
	}

    return true;
}

bool robot_msg_handler_t::handle_ac_login_game_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2C_MSG(proto::client::ac_login_game_reply);

	if (msg.reply_code() != 0)
	{
		if (LOGIN_STATE_LOGIN_GATE == robot->get_login_state())
		{
			robot->create_new_role();
		}
		else
		{
			log_error("robot[%lu] enter game failed! reply_code: %d", robot->get_robot_id(), msg.reply_code());
		}
	}
	else
	{
		log_error("robot[%lu] enter game failed! reply_code: %d", robot->get_robot_id(), msg.reply_code());
	}

	return true;
}

bool robot_msg_handler_t::handle_gc_create_family_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{   
    return true;
}

bool robot_msg_handler_t::handle_ac_create_role_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2C_MSG(proto::client::ac_create_role_reply);

	if (0 == msg.reply_code())
	{
		if (LOGIN_STATE_CREATE_USER == robot->get_login_state())
		{
			robot->enter_game();
			return true;
		}
	}

	log_error("robot[%lu] create user failed! reply_code=%d, login_state=%d",
		robot->get_robot_id(), msg.reply_code(), (uint32_t)robot->get_login_state());

	return true;
}

bool robot_msg_handler_t::handle_gc_enter_scene_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2C_MSG(proto::client::gc_enter_scene_notify);

	robot->update_scene_info(msg);

	return true;
}

bool robot_msg_handler_t::handle_gc_jump_scene_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	return true;
}

bool robot_msg_handler_t::handle_gc_area_id_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
    return true;
}

bool robot_msg_handler_t::handle_gc_role_into_vision_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
    PRE_S2C_MSG(proto::client::gc_role_into_vision_notify);
	robot->update_into_vision(msg);
    return true;
}

bool robot_msg_handler_t::handle_gc_role_outo_vision_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2C_MSG(proto::client::gc_role_outo_vision_notify);
	robot->update_out_of_vision(msg);
	return true;
}

bool robot_msg_handler_t::handle_gc_scene_object_info_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2C_MSG(proto::client::gc_scene_object_info_notify);
	robot->update_vision_info(msg);
	return true;
}

bool robot_msg_handler_t::handle_gc_country_bounty_data_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	return true;
}

bool robot_msg_handler_t::handle_gc_country_bounty_accept_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	return true;
}

bool robot_msg_handler_t::handle_gc_country_bounty_finish_ntf(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	return true;
}

bool robot_msg_handler_t::handle_gc_country_bounty_refresh_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	return true;
}

bool robot_msg_handler_t::handle_gc_role_change_data_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	return true;
}

bool robot_msg_handler_t::handle_gc_mail_get_addenda_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	return true;
}

bool robot_msg_handler_t::handle_gc_sys_init_notice_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	return true;
}

bool robot_msg_handler_t::handle_gc_sys_notice_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
//	PRE_S2C_MSG(proto::client::gc_sys_notice_notify);
// 	for (int32_t i = 0; i < msg.notices_size(); ++i)
// 	{
// 		const proto::common::sys_notice_data& data = msg.notices(i);
// 
// 		log_debug("robot[%lu] recv sys_init, type[%d] priority[%u] text[%s]", robot->get_robot_id(), data.type(), data.priority(), data.text().c_str());
// 	}

	return true;
}

bool robot_msg_handler_t::handle_gc_red_point_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
    return true;
}

bool robot_msg_handler_t::handle_gc_update_fighting_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
    return true;
}

bool robot_msg_handler_t::handle_gc_open_dungeon_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2C_MSG(proto::client::gc_open_dungeon_reply);
	robot->open_dungeon_failed(msg.reply_code());
	return true;
}

bool robot_msg_handler_t::handle_gc_dungeon_schedule_data_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2C_MSG(proto::client::gc_dungeon_schedule_data_notify);
	robot->notify_dungeon_schedule(msg);
	return true;
}

bool robot_msg_handler_t::handle_gc_notify_dungeon_result_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2C_MSG(proto::client::gc_notify_dungeon_result_notify);
	robot->notify_dungeon_schedule(msg);
	return true;
}

bool robot_msg_handler_t::handle_gc_combat_obj_state_ntf(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	//PRE_S2C_MSG(proto::client::gc_combat_obj_state_ntf);
	/*const proto::common::fight_obj_state_data& data = msg.data();
	for (int32_t i = 0; i < data.states_size(); ++i)
	{
		if (data.states(i).state() == proto::common::combat_obj_state_play_end)
		{
			robot->reset_fight_uid();
			break;
		}
	}*/
	
	return true;
}

bool robot_msg_handler_t::handle_gc_enter_fight_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2C_MSG(proto::client::gc_enter_fight_reply);
	log_error("robot_name[%s],reply_code[%d]", robot->get_nickname().c_str(),msg.reply_code());
	return true;
}

bool robot_msg_handler_t::handle_gc_combat_auto_fight_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2C_MSG(proto::client::gc_combat_auto_fight_reply);
	if (msg.reply_code() != 0 || msg.auto_fight() == 0)
	{
		log_error("request enter auto fight fail! code:auto_fight[%d : %d]", msg.reply_code(), msg.auto_fight());
	}

	return true;
}

bool robot_msg_handler_t::handle_gc_combat_before_form_ready_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	return true;
}

bool robot_msg_handler_t::handle_gc_combat_reset_form_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	return true;
}

bool robot_msg_handler_t::handle_gc_combat_result_ntf(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2C_MSG(proto::client::gc_combat_result_ntf);
	robot->fight_end_update_info(msg);

	// 发消息通知状态改变
	proto::client::cg_change_sys_state_notify notify;
	notify.set_obj_state(proto::common::object_state_normal);
	send_msg(robot->get_socket(), op_cmd::cg_change_sys_state_notify, notify);
	return true;
}

bool robot_msg_handler_t::handle_gc_achieve_finish_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	return true;
}

bool robot_msg_handler_t::handle_gc_active_reward_finish_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
    return true;
}

bool robot_msg_handler_t::handle_gc_enter_game_for_hero_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2C_MSG(proto::client::gc_enter_game_for_hero_reply);
	robot->update_robot_hero_info(msg);
	return true;
}

bool robot_msg_handler_t::handle_gc_enter_game_for_item_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2C_MSG(proto::client::gc_enter_game_for_item_reply);
	robot->update_robot_item_info(msg);
	return true;
}

bool robot_msg_handler_t::handle_gc_combat_data_ntf(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	//PRE_S2C_MSG(proto::client::gc_combat_data_ntf);
	//log_warn("robot_name[%s],fight_uid[%s]", robot->get_nickname().c_str(), msg.data().fight_uid().c_str());
	//robot->set_fight_uid(msg.data().fight_uid());
	return true;
}

bool robot_msg_handler_t::handle_gc_combat_start_loading_ntf(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2C_MSG(proto::client::gc_combat_start_loading_ntf);
	log_debug("robot_name[%s],fight_uid[%s]", robot->get_nickname().c_str(), msg.combat().fight_uid().c_str());
	robot->set_fight_uid(common::string_util_t::string_to_uint64(msg.combat().fight_uid()));

	proto::client::cg_combat_loading_end_ntf msg_request;
	msg_request.set_fight_uid(msg.combat().fight_uid());
	send_msg(robot->get_socket(), op_cmd::cg_combat_loading_end_ntf, msg_request);
	return true;
}

bool robot_msg_handler_t::handle_hc_test_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
    return true;
}

bool robot_msg_handler_t::handle_hc_channel_chat_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	return true;
}

bool robot_msg_handler_t::handle_hc_channel_chat_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2C_MSG(proto::client::hc_channel_chat_notify);
	log_debug("robot[%lu] recv chat msg from chat!", robot->get_robot_id());

	log_debug("chat msg info channel[%d], src_uid[%lu], src_name[%s], info[%s]", msg.chat_channel(),common::string_util_t::string_to_uint64(msg.src_uid()), msg.src_name().c_str(), msg.chat_info().c_str());
	return true;
}


//bool robot_msg_handler_t::handle_gc_send_luck_gift_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
//{
//	return true;
//}
//
//bool robot_msg_handler_t::handle_ec_send_luck_gift_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
//{
//	return true;
//}

bool robot_msg_handler_t::handle_ec_new_mail_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2C_MSG(proto::client::ec_new_mail_notify);
	if (msg.is_have_new())
	{
		log_debug("robot[%lu] have new mail", robot->get_robot_id());
	}
	else
	{
		log_debug("robot[%lu] no new mail", robot->get_robot_id());
	}

	return true;
}

bool robot_msg_handler_t::handle_ec_mail_list_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2C_MSG(proto::client::ec_mail_list_notify);

	robot->update_mail_list(msg);

	return true;
}

bool robot_msg_handler_t::handle_ec_mail_state_change_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2C_MSG(proto::client::ec_mail_state_change_notify);

	robot->update_mail_state_chage(msg);

	return true;
}

bool robot_msg_handler_t::handle_ec_mail_content_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2C_MSG(proto::client::ec_mail_content_reply);

	robot->update_mail_content(msg);

	return true;
}

bool robot_msg_handler_t::handle_ec_field_boss_info_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2C_MSG(proto::client::ec_field_boss_info_notify);
	robot->reply_boss_info(msg);
	log_debug("robot[%lu] receive field boss info notify", robot->get_robot_id());

	return true;
}

bool robot_msg_handler_t::handle_gc_field_boss_attack_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2C_MSG(proto::client::gc_field_boss_attack_reply);
	uint32_t ret = msg.reply_code();
	if (ret == errcode_enum::notice_field_boss_cd)
	{
		log_debug("field boss attack reply in cd, robot[%lu], boss id[%lu], error_num[%d]",
			robot->get_robot_id(), common::string_util_t::string_to_uint64(msg.object_id()), msg.reply_code());
	}
	else if (ret != 0)
	{
		log_error("field boss attack reply error, robot[%lu], boss id[%lu], error_num[%d]",
			robot->get_robot_id(), common::string_util_t::string_to_uint64(msg.object_id()), msg.reply_code());
	}

	return true;
}

bool robot_msg_handler_t::handle_gc_arena_join_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2C_MSG(proto::client::gc_arena_join_reply);

	if (msg.reply_code() == 0)
	{
		log_debug("robot[%lu] recv gc_arena_join_reply from game reply_code[%d]", robot->get_robot_id(), msg.reply_code());
		robot->set_join_arena(true);
		//robot->arena_match();
	}
	else
	{
		log_error("robot[%lu] recv gc_arena_join_reply from game error! reply_code[%d]", robot->get_robot_id(), msg.reply_code());
	}

	return true;
}

bool robot_msg_handler_t::handle_gc_arena_match_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2C_MSG(proto::client::gc_arena_match_reply);
	robot->arena_match_reply(msg);
	return true;
}

bool robot_msg_handler_t::handle_gc_buy_month_card_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2C_MSG(proto::client::gc_arena_match_reply);
	log_error("receive gc_buy_month_card_reply, reply_code: %d", msg.reply_code());
	return true;
}

bool robot_msg_handler_t::handle_gc_arena_battle_result_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2C_MSG(proto::client::gc_arena_battle_result_notify);

	robot->arena_battle_finish(msg);
	return true;
}

//
//bool robot_msg_handler_t::handle_gc_grab_luck_gift_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
//{
//	PRE_S2C_MSG(proto::client::gc_grab_luck_gift_reply);
//	log_debug("robot[%lu] gc_grab_luck_gift_reply from chat!", robot->get_robot_id());
//
//	const proto::common::gift_data data = msg.gift_data();
//	log_debug("gift info role name [%s] count [%u] value [%u]", data.src_name().c_str(), data.gift_count(), data.gift_value());
//	for (auto gift_single : data.gift_single())
//	{
//		log_debug("grap info user [%s, %s] value[%u]", gift_single.uid().c_str(), gift_single.name().c_str(), gift_single.value());
//	}
//
//	return true;
//}

bool robot_msg_handler_t::handle_gc_accept_task_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	return true;
}

bool robot_msg_handler_t::handle_gc_commit_task_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	return true;
}

bool robot_msg_handler_t::handle_gc_task_change_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	return true;
}

bool robot_msg_handler_t::handle_gc_help_task_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	return true;
}

bool robot_msg_handler_t::handle_hc_get_server_channel_list_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	return true;
}

bool robot_msg_handler_t::handle_hc_change_server_channel_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	return true;
}

bool robot_msg_handler_t::handle_hc_change_server_channel_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{	
	return true;
}

bool robot_msg_handler_t::handle_hc_server_channel_chat_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	return true;
}

bool robot_msg_handler_t::handle_hc_server_channel_chat_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2C_MSG(proto::client::hc_server_channel_chat_notify);
	
	uint32_t src_chat_id = msg.src_chat_id();
	std::string src_chat_name = msg.src_chat_name();
	uint64_t user_uid = string_util_t::string_to_uint64(msg.user_uid());
	std::string user_name = msg.user_name();
	std::string chat_info = msg.chat_info();
	log_debug("src chat[%u, %s], user[%lu, %s], chat_info [%s]", src_chat_id, src_chat_name.c_str(), user_uid, user_name.c_str(), chat_info.c_str());

	return true;
}

bool robot_msg_handler_t::handle_gc_trade_buy_item_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	return true;
}

bool robot_msg_handler_t::handle_gc_trade_sell_item_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	return true;
}

bool robot_msg_handler_t::handle_gc_get_city_trade_shop_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	return true;
}

bool robot_msg_handler_t::handle_gc_next_day_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	return true;
}

bool robot_msg_handler_t::handle_gc_use_mount_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	return true;
}

bool robot_msg_handler_t::handle_gc_hero_cultivate_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	return true;
}

bool robot_msg_handler_t::handle_gc_hero_recruit_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	return true;
}

bool robot_msg_handler_t::handle_gc_country_info_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	return true;
}

bool robot_msg_handler_t::handle_gc_enter_scene_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2C_MSG(proto::client::gc_enter_scene_reply);

	if (msg.reply_code() > 0)
	{
		log_error("robot[%lu] handle_gc_enter_scene_reply failed! reply_code: %d", robot->get_robot_id(), msg.reply_code());
	}
	else
	{
		//log_debug("robot[%lu] enter scene[area=%u] success!", robot->get_uid(), msg.area_tid());
		robot->enter_scene_reply(msg);
		robot->set_login_state(LOGIN_STATE_ENTER_SCENE);
		// 这个不要注释掉，不然机器人会报错
		robot->set_login_success();
	}
	
	return true;
}

bool robot_msg_handler_t::handle_gc_move_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
// 	PRE_S2C_MSG(proto::client::gc_move_notify);
// 
// 	// 自己的移动，别人的抛掉
// 	if (robot->get_uid() == msg.object_id())
// 	{
// 		robot->set_real_pos(msg.pos_x(), msg.pos_y());
// 	}

	return true;
}

bool robot_msg_handler_t::handle_gc_batch_move_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	// 其他人的移动暂时都不需要
// 	uint32_t count = msg_buf->size() / MOVE_CMD_SIZE;
// 	gc_user_move_cmd* p_batch = (gc_user_move_cmd*)msg_buf->begin();
// 
// 	for (uint32_t i = 0; i < count; ++i) {
// 		if (p_batch[i].object_id == robot->get_uid()) {
// 			log_info("robot_t::move_to_postion x:%f y:%f", p_batch[i].pos_x / 100.f, p_batch[i].pos_y / 100.f);
// 		}
// 	}

	return true;
}

bool robot_msg_handler_t::handle_gc_system_hint_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	return true;
}

bool robot_msg_handler_t::handle_gc_gm_cmd_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2C_MSG(proto::client::gc_gm_cmd_reply);

    if (msg.reply_code() != 0)
    {
		log_error("robot[%lu] gc_gm_cmd_reply failed! reply_code: %d", robot->get_robot_id(), msg.reply_code());
    }

    return true;
}
