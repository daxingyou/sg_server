#ifndef __GAME_ROLE_MSG_HANDLE_H__
#define __GAME_ROLE_MSG_HANDLE_H__

#include "gate/gate_msg_handler.hpp"

class role_msg_handle_t
{
public:
    static bool init_msg_handler();
    
	static bool handle_cg_gm_cmd_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	// 角色移动
	static bool handle_cg_role_move_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
   /* static bool handle_cg_cost_item_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);*/
	static bool handle_cg_hero_cultivate_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	static bool handle_cg_hero_recruit_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	// 进入战斗请求
	static bool handle_cg_enter_fight_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	// 更换PK模式
	static bool handle_cg_change_pk_mode_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	// 兑换货币
	static bool handle_cg_exchange_money_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	// 改变玩家状态
	static bool handle_cg_change_sys_state_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	// 请求复活
	static bool handle_cg_object_revive_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	// 请求参加演武大赛
	static bool handle_cg_world_cup_play_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	// 请求投注
	static bool handle_cg_world_cup_bet_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	// 请求领取投注奖励
	static bool handle_cg_world_cup_bet_reward_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
    // 完成新手引导
    static bool handle_cg_finish_new_role_guide_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	// 切换模型请求
	static bool handle_cg_change_hero_plugin_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	// 充值
	static bool handle_cg_recharge_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	// 巡逻
	static bool handle_cg_patrol_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	// 获得等级奖励
	static bool handle_cg_get_level_reward_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	// 发红包
	static bool handle_cg_redbag_send_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	//购买离线竞技场挑战次数
	static bool handle_cg_offline_arena_buy_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	// 充值
	static bool handle_cg_recharge_reward_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	// 主角武将解锁
	static bool handle_cg_role_main_hero_unlock_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	// 主角切换
	static bool handle_cg_role_main_hero_change_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);

	// 购买精力
	static bool handle_cg_buy_vigour_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
	
	
	//设置GM权限
	static bool handle_cg_set_gm_level(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf);
};

#endif // !__GAME_ROLE_MSG_HANDLE_H__