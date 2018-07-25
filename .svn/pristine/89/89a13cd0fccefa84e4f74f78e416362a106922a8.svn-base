#pragma once

#include "monster/monster.hpp"
#include "family_war_common.hpp"

#include "client.pb.h"
#include "role/role.hpp"

#include <boost/asio/deadline_timer.hpp>
#include <boost/system/error_code.hpp>
#include "fight/combat.hpp"

class family_war_long_t : public monster_t
{
public:
	family_war_long_t();
	virtual ~family_war_long_t();

	// 是否是家族战龙柱
	virtual bool is_family_war_long() { return true; }

	// 获取龙柱指针
	virtual family_war_long_ptr get_family_war_long() { return boost::dynamic_pointer_cast<family_war_long_t>(shared_from_this()); }

	// 获取对象场景属性
	virtual void get_object_info(proto::common::object_common_info* object_info);

	// 龙柱武将
public:
	struct long_hero_info
	{
		uint32_t m_id;
		uint32_t m_curr_hp;	// 当前血量
		uint32_t m_max_hp;	// 最大血量

		long_hero_info() : m_id(0), m_curr_hp(0), m_max_hp(0) {};
		long_hero_info(uint32_t id, uint32_t curr, uint32_t max) : m_id(id), m_curr_hp(curr), m_max_hp(max) {};
	};
	typedef std::map<uint32_t, long_hero_info> long_hero_map;

	typedef std::set<uint64_t> family_war_long_combat;

public:
	// 战斗，不能进入战斗直接返回0
	bool init_long(uint32_t monster_tid, const monster_common::monster_define_t& define, Monster* monster);
	virtual uint64_t fight_with_role(role_ptr p_role) { return 0; }

	// 获取最大血量
	virtual uint32_t get_max_hp() const { return m_max_hp; }
	virtual void set_max_hp(uint32_t value) { m_max_hp = value; }
	// 获取血量
	virtual uint32_t get_current_hp() const { return m_curr_hp; }
	virtual void set_current_hp(uint32_t value) { m_curr_hp = value; }

	// 进入场景
	virtual void on_enter_scene(scene_ptr p_scene);

	/**
	 * \brief 设置死亡
	 * \param obj_killer:谁杀了你
	 */
	virtual void set_object_die(uint64_t obj_killer = 0);
	/**
	 * \brief 设置死亡
	 */
	virtual void on_object_die();

	// 设置参战家族
	void set_family_war_family(family_war_side_type type, uint64_t family_id);

	// 是否是参战家族
	family_war_side_type get_family_war_side_type(uint64_t family_id);

	// 战斗管理
	bool get_hero_state_data(proto::common::hero_state_data* data);

	void add_combat(uint64_t id);

	void del_combat(uint64_t id);

	// 设置所有战斗失败
	void set_all_fight_failed(uint64_t exception = 0);

	//设置所有战斗成功
	void set_all_fight_success(uint64_t exception = 0);

	// 更新血量
	uint32_t update_hero_hp(const scene_ptr& p_scene, family_war_side_type side_type, uint64_t fight_id, const proto::common::fight_camp& enemy_camp, const combat_ptr& p_combat);

	// 同步所有血量
	void update_all_combat(uint64_t exception = 0);

	//同步所有战斗血量
	bool sync_all_boss_hp(uint64_t fight_id, const proto::common::hero_state_data& data);

	// 结算
	family_war_side_type get_win_side();

	// 得到伤害百分百
	float get_hurt_pre(family_war_side_type side_type);

	// 得到伤害数据
	void peek_long_hurt_info(proto::client::gc_long_hurt_info_reply& reply);

	// 是否已经开始结算
	bool is_family_occupied() const { return m_is_fight_end; }

	// 同步血量
	void broadcast_up(const scene_ptr& p_scene);

	void peek_hp_info(proto::client::gc_family_long_info_notify& ntf);

protected:
	long_hero_map m_hero_map;	// 武将信息
	uint32_t m_curr_hp = 0;
	uint32_t m_max_hp = 0;

	// 血量同步定时器
	common::cTimer m_sync_sec;

	family_war_long_combat m_field_boss_combat;

	// 参战家族
	uint64_t m_war_family[family_war_side_max] = { 0 };
	// 伤害
	uint32_t m_hurt_val[family_war_side_max] = { 0 };
	// 占领家族
	uint64_t m_occupy_family = 0;

	uint32_t m_revive_time = 0;

	bool m_is_fight_end = false;

protected:
	std::set<uint64_t> m_fight_uids;
};