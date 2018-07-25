#ifndef __GAME_KING_WAR_MAN_H__
#define __GAME_KING_WAR_MAN_H__

#include <map>

#include "king_war_common.hpp"
#include <boost/shared_ptr.hpp>
#include "server.pb.h"
#include "client.pb.h"
#include "role/role.hpp"
#include "scene/scene_manager.hpp"
#include <boost/asio/deadline_timer.hpp>
#include <boost/system/error_code.hpp>

typedef boost::shared_ptr<country_scene_info_t> country_scene_info_ptr;
typedef std::map<uint32_t, country_scene_info_ptr> country_scene_map;

class king_war_manager_t
{
public:
	static bool close_clear();

public:
	// 场景加载添加王城
	static bool add_country_scene(uint32_t country_id, uint64_t scene_id);

	// 通知王城争夺战状态
	static void update_king_war_state(uint32_t country_id, const proto::server::eg_king_war_state_notify& notify);

	// 通知王城战结束，时间到了
	static void king_war_ending(uint32_t country_id);

	// 结算奖励
	static void king_war_settlement(uint32_t country_id);

	// 得到自己国家场景中王城战人数
	static void get_king_war_family_num(uint32_t country_id, proto::client::gc_king_war_scene_num_reply& notify);

	// 请求进入王城战场景
	static uint32_t ask_enter_king_war_scene(role_ptr p_role);

	// 请求进入防御队列
	static uint32_t ask_defence_long(role_ptr p_role, uint64_t object_id);

	// 请求攻击龙脉
	static uint32_t ask_attack_long(role_ptr p_role, uint64_t object_id);

	// 攻击龙脉结束
	static void attack_long_end(role_ptr p_role, uint64_t object_id);

	// 请求造成伤害(判断)
	static void ask_hurt_long(role_ptr p_role, uint64_t object_id, proto::client::gc_king_war_attack_end_reply& reply);

	// 请求使用神器
	static void user_artifacts(role_ptr p_role, uint32_t index);

	// 请求王城战结果
	static void ask_king_war_result(role_ptr p_role, proto::client::gc_king_war_result_reply& reply);

public:
	// 王城战龙气增加
	static void add_long_gas(uint32_t country_id, uint64_t family_id, uint32_t val);

	// 玩家击杀别人
	static void role_kill_other(role_ptr p_role, role_ptr p_enemy);

	// 自己被人击杀
	static void other_kill_self(role_ptr p_role, role_ptr p_enemy);

	// 立即复活计数
	static bool role_fast_revive(role_ptr p_role);

	// 攻击龙脉增加战功
	static void attack_long_add(role_ptr p_role);

	// gm增加战功
	static void gm_add_exploit(role_ptr p_role, uint32_t val);

public:
	static country_scene_info_ptr get_country_info_by_id(uint32_t country_id);

	static king_war_role_ptr get_king_war_role_ex(uint32_t country_id, uint64_t uid);
	static king_war_role_ptr get_king_war_role(uint32_t country_id, uint64_t uid);

	// 击杀获得的奖励
	static std::pair<uint32_t, uint32_t> get_reward_by_self_title(em_kill_title title);
	// 终结获得的奖励
	static std::pair<uint32_t, uint32_t> get_reward_by_other_title(em_kill_title title);
	// 击杀官员的奖励
	static std::pair<uint32_t, uint32_t> get_reward_by_other_offer(uint32_t offer);

	// 得到title
	static em_kill_title get_kill_title(uint32_t kill_num);

public:
	static bool exploit_rank_sort(const king_war_role_ptr& h1, const king_war_role_ptr& h2);

	// 回调放在静态类，防止time cancel时候，可能会出现cancel失败，因为定时器已经在调用队列里了
public:
	static void on_long_revive_timer_call_back(const boost::system::error_code& ec, uint64_t object_id);

	static void on_long_occupied_timer_call_back(const boost::system::error_code& ec, uint64_t object_id);

public:
	static country_scene_map m_country_scene_map;
};

#endif
