#ifndef __ROBOT_MSG_HANDLER_HPP__
#define __ROBOT_MSG_HANDLER_HPP__

#include "network/msg_handler.hpp"

#include <boost/shared_ptr.hpp>

class robot_t;
typedef boost::shared_ptr<robot_t> robot_ptr;

class robot_msg_handler_t
{
public:
    static bool init_msg_handler();
    static bool handle_server_msg(robot_ptr robot, uint16_t cmd, const network::msg_buf_ptr& msg_buf);
	// 不需要处理的消息直接用这个
	static bool handle_null_handle_msg(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	// =========================================================================
	static bool handle_nc_server_info_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
    // =========================================================================
    static bool handle_lc_login_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_lc_login_queue_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf);

    // =========================================================================
    static bool handle_ac_connect_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
    static bool handle_ac_kick_role_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_ac_pingpong_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_ac_login_game_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_ac_create_role_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
    
    // =========================================================================
    static bool handle_gc_enter_game_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
    static bool handle_gc_enter_scene_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_jump_scene_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
    static bool handle_gc_area_id_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_role_into_vision_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_role_outo_vision_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_scene_object_info_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_accept_task_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_commit_task_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_task_change_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_help_task_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_hero_cultivate_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_hero_recruit_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	//static bool handle_gc_send_luck_gift_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	//static bool handle_gc_grab_luck_gift_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_country_info_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_enter_scene_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_move_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_batch_move_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
    static bool handle_gc_system_hint_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_gm_cmd_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_trade_buy_item_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_trade_sell_item_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_get_city_trade_shop_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_next_day_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_use_mount_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_country_bounty_data_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_country_bounty_accept_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_country_bounty_finish_ntf(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_country_bounty_refresh_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_role_change_data_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_arena_join_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_arena_match_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_arena_battle_result_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_mail_get_addenda_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_sys_init_notice_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
    static bool handle_gc_sys_notice_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
    static bool handle_gc_red_point_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
    static bool handle_gc_update_fighting_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_open_dungeon_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_dungeon_schedule_data_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_notify_dungeon_result_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_combat_data_ntf(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_combat_start_loading_ntf(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_combat_obj_state_ntf(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_enter_fight_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_combat_auto_fight_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);

	static bool handle_gc_combat_before_form_ready_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_combat_reset_form_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf);

	static bool handle_gc_combat_result_ntf(robot_ptr robot, const network::msg_buf_ptr& msg_buf);

    static bool handle_gc_achieve_finish_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
    static bool handle_gc_active_reward_finish_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf);

	static bool handle_gc_enter_game_for_hero_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_gc_enter_game_for_item_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);

    // =========================================================================
	static bool handle_hc_test_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_hc_channel_chat_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_hc_channel_chat_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
    static bool handle_gc_create_family_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);

	static bool handle_hc_get_server_channel_list_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_hc_change_server_channel_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_hc_change_server_channel_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_hc_server_channel_chat_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_hc_server_channel_chat_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf);

	//static bool handle_ec_send_luck_gift_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf);

	// =========================================================================
	static bool handle_ec_new_mail_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_ec_mail_list_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_ec_mail_state_change_notify(robot_ptr robot, const network::msg_buf_ptr& msg_buf);
	static bool handle_ec_mail_content_reply(robot_ptr robot, const network::msg_buf_ptr& msg_buf);

private:
    typedef bool (*msg_handler_func)(robot_ptr robot, const network::msg_buf_ptr& msg_buf);

	static network::server_msg_handler_t<op_cmd::NC_BEGIN, op_cmd::NC_END, msg_handler_func> m_msg_handler_for_account;
    static network::server_msg_handler_t<op_cmd::LC_BEGIN, op_cmd::LC_END, msg_handler_func> m_msg_handler_for_login;
    static network::server_msg_handler_t<op_cmd::AC_BEGIN, op_cmd::AC_END, msg_handler_func> m_msg_handler_for_gate;
    static network::server_msg_handler_t<op_cmd::GC_BEGIN, op_cmd::GC_END, msg_handler_func> m_msg_handler_for_game;
	static network::server_msg_handler_t<op_cmd::HC_BEGIN, op_cmd::HC_END, msg_handler_func> m_msg_handler_for_chat;
	static network::server_msg_handler_t<op_cmd::EC_BEGIN, op_cmd::EC_END, msg_handler_func> m_msg_handler_for_center;
};



#endif  //__ROBOT_MSG_HANDLER_HPP__
