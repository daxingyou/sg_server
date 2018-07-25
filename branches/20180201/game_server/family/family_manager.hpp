#pragma once

#include "role/role.hpp"
#include "family_war_common.hpp"

#include <map>

class family_manager_t
{
public:
	static uint32_t create_family_request(const role_ptr& p_role, const std::string& name, const std::string& declaration);

	// 家族战
public:
	// 准备家族战
	static void prepare_family_war_request(const proto::server::eg_prepare_family_war_request& request);

	// 设置家族战开战
	static void family_war_start(uint32_t end_time);

	// 设置家族战结束
	static void family_war_end();

	// 家族战人数
	static void get_family_war_member(const role_ptr& p_role, proto::client::gc_family_war_member_reply& reply);

	// 家族战武将信息
	static void get_role_formations_info(const role_ptr& p_role, proto::client::gc_ask_formations_info_reply& reply);

	// 上阵武将
	static uint32_t ask_formations_hero_up(const role_ptr& p_role, const proto::client::cg_formations_hero_up_request& request);

	// 下阵武将
	static uint32_t ask_formations_hero_down(const role_ptr& p_role, const proto::client::cg_formations_hero_down_request& request);

	// 复活武将
	static uint32_t ask_formations_revive(const role_ptr& p_role, const proto::client::cg_formations_hero_revive_request& request);

	// 请求攻击龙柱
	static uint32_t ask_attack_long(const role_ptr& p_role);

	// 提早结束家族战斗
	static void family_war_early_end(uint32_t war_id, uint64_t win_family_id, uint32_t left_score, uint32_t right_score);

	// 得到家族战信息
	static void get_family_war_fight_data(const role_ptr& p_role, proto::client::gc_family_war_fight_data_reply& reply);

	// 传送进入战场
	static uint32_t ask_enter_battlefield(role_ptr p_role);

	// 得到龙柱伤害信息
	static void get_long_hurt_info(const role_ptr& p_role, uint64_t object_id);

	// 得到家族战结果
	static void get_family_war_result(const role_ptr& p_role, proto::client::gc_family_war_result_info_reply& reply);

	// 家族祈愿
public:
	static uint32_t ask_family_prayer_request(const role_ptr& p_role, const proto::client::cg_family_prayer_request& request);

	static uint32_t family_prayer_gift_request(const role_ptr& p_role, const proto::client::cg_family_prayer_gift_request& request);

	static void family_prayer_gift_reply(const role_ptr& p_role, const proto::server::eg_family_prayer_gift_failed_notify& notify, proto::client::gc_family_prayer_gift_reply& reply);

	// 家族商店
public:
	static uint32_t family_shop_buy_request(const role_ptr& p_role, uint32_t index, uint32_t shop_id, uint32_t num);
	static void family_shop_center_check_back(const role_ptr& p_role, const proto::server::eg_family_shop_buy_reply& center_reply, proto::client::gc_family_shop_buy_reply& reply);

public:
	// <家族战id 家族战场景id>
	typedef std::map<uint32_t, uint64_t> family_war_info_map;
	static family_war_info_map m_family_war_info_map;
};