#ifndef __FAMILY_MSG_HANDLE_H__
#define __FAMILY_MSG_HANDLE_H__

#include "message.hpp"
#include "network/tcp_socket.hpp"

class family_msg_handle_t
{
public:
    static bool init_gate_msg_handler();
	static bool init_center_msg_handler();

public:
	static bool handle_cg_create_family_requsest(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
    static bool handle_cg_impeach_family_general_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
    static bool handle_eg_create_family_reply(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_update_family_info_notify(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_family_country_change_notify(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_family_level_change_notify(const network::msg_buf_ptr& msg_buf);

    static bool handle_eg_impeach_family_general_reply( const network::msg_buf_ptr& msg_buf);
    static bool handle_eg_notify_game_impeach_result(const network::msg_buf_ptr& msg_buf);

	static bool handle_cg_get_family_activities_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	// 家族战消息
public:
	static bool handle_cg_leave_family_war_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_family_war_member_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_ask_formations_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_formations_hero_update_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_formations_hero_revive_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_family_war_attack_long_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_family_war_fight_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_ask_enter_battlefield_requet(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_long_hurt_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_family_war_result_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	static bool handle_eg_prepare_family_war_request(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_enter_family_war_scene_request(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_family_war_start_notify(const network::msg_buf_ptr& msg_buf);
	static bool handle_eg_family_war_end_notify(const network::msg_buf_ptr& msg_buf);

	// 家族祈愿
public:
	static bool handle_cg_family_prayer_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_family_prayer_gift_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	static bool handle_eg_family_prayer_gift_failed_notify(const network::msg_buf_ptr& msg_buf);

	// 家族商店
public:
	static bool handle_cg_family_shop_buy_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	static bool handle_eg_family_shop_buy_reply(const network::msg_buf_ptr& msg_buf);
};

#endif
