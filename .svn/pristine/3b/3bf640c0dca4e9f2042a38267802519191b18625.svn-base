#ifndef __GAME_MONSTER_H__
#define __GAME_MONSTER_H__

#include <set>
#include <vector>

#include "object/object_position.hpp"
#include "tblh/Monster.tbls.h"
#include "utility.hpp"
#include "monster_define.hpp"
#include "monster_ai.hpp"

// 前置声明

class field_boss_t;
typedef boost::shared_ptr<field_boss_t> field_boss_ptr;

class long_pulse_t;
typedef boost::shared_ptr<long_pulse_t> long_pulse_ptr;

class family_war_long_t;
typedef boost::shared_ptr<family_war_long_t> family_war_long_ptr;

class monster_t : public object_base_t
{
public:
	monster_t() {}
	virtual ~monster_t();

	// 清理
	virtual void del_clear() {};

	// 是否是怪物
	virtual bool is_monster() { return true; }
	// 是否是野外boss
	virtual bool is_field_boss() { return false; }
	// 是否是龙脉
	virtual bool is_long_pulse() { return false; }
	// 是否是家族战龙柱
	virtual bool is_family_war_long() { return false; }
	// 获取对象类型
	virtual proto::common::EM_SCENEOBJECT_TYPE get_object_type() { return proto::common::SCENEOBJECT_MONSTER; }
	// 获取对象指针
	virtual monster_ptr get_monster() { return boost::dynamic_pointer_cast<monster_t>(shared_from_this()); }
	// 获取野外boss指针
	virtual field_boss_ptr get_field_boss() { return field_boss_ptr(); }
	// 获取龙脉指针
	virtual long_pulse_ptr get_long_pulse() { return long_pulse_ptr(); }
	// 获取龙柱指针
	virtual family_war_long_ptr get_family_war_long() { return family_war_long_ptr(); }
	// 获得npcId
	uint32_t get_monster_id() const { return m_monster_tid; }
	// 获得npc类型
	proto::common::EM_MONSTER_TYPE get_monster_type() { return m_monster_type; }
	// 获取对象场景属性
	virtual void get_object_info(proto::common::object_common_info* object_info);

	virtual uint32_t get_level() { return m_level; }
public:
	// 怪物初始化
	virtual bool init_monster(const monster_common::monster_define_t& define, Monster* monster, uint64_t object_id = 0);

	inline bool is_activated() const { return m_activated; }
	bool set_activated(bool is_activate) { return m_activated = is_activate; }

	inline bool is_need_clear() { return m_need_clear; }
	void set_need_clear() { m_need_clear = true; }

	bool has_flag(monster_flag flag) { return (m_flags&flag) != 0; }

	// 场景相关
public:
	// 进入场景
	virtual void on_enter_scene(scene_ptr p_scene);
	// 离开场景
	virtual void on_leave_scene(scene_ptr p_scene);
	// 生物进入视野
	virtual void other_object_into_vision(const object_id_type& object);
	// 通知离开视野
	virtual void other_object_out_of_vision(const object_id_type& object);

	// 通知9屏玩家自己的场景信息
	void notify_object_common_info_for_nearbies();

public:
	// 获取最大血量
	virtual uint32_t get_max_hp() const { return 0; }
	virtual void set_max_hp(uint32_t value) {};
	// 获取血量
	virtual uint32_t get_current_hp() const { return 0; }
	virtual void set_current_hp(uint32_t value) {};

	// 获得死亡时间
	uint32_t get_death_time() { return m_death_time; }

	virtual void set_object_state(proto::common::object_state_type newstate);

	void set_normal_state();

	/**
	* \brief 设置死亡
	*/
	virtual void on_object_die();
	/**
	* \brief 复活操作
	*/
	virtual void on_object_revive();

	/**
	 * \brief 进入战斗
	 */
	void enter_fight(uint64_t uid, uint64_t fight_id);
	/**
	 * \brief 怪物离开战斗处理
	 * \param is_win：是否胜利 fight_time：时间 death_value：死亡武将数量
	 */
	void leave_fight(bool is_win, uint32_t fight_time, uint32_t death_value);

public:
	// 对战id实时计算
	virtual uint32_t get_against_form_id();
	
	// 战斗 
	virtual uint64_t fight_with_role(role_ptr p_role);

	// ai
	virtual void on_tick(uint64_t msecs);

	// 警戒附近
	bool alert_round(uint32_t radius);
	bool check_alert(uint32_t radius);
	void start_alert();
	void clear_alert();
	uint32_t get_start_alert_time() const { return m_alert_start_time; }
	bool fight_with_alert_role();

protected:
	uint32_t m_monster_tid = 0;								// tid
	proto::common::EM_MONSTER_TYPE m_monster_type = proto::common::EM_NONE; // 类型
	uint32_t m_level = 0;									// 等级

	std::set<object_id_type> m_vision_objects;				// 视野内生物

protected:
	uint32_t m_flags = 0;									// 怪物的一些标记
	bool m_activated = true;								// 怪物是否被激活，没有激活不会走ai
	bool m_need_clear = false;								// 从管理器删除标志						

	boost::scoped_ptr<ai_controller_t> m_ai_contrl;			// 怪物控制器

	// 一些数据
protected:
	uint32_t m_death_time = 0;								// 死亡时间
	uint64_t m_fight_uid = 0;								// 战斗id，如果是独占模式才有用
	uint64_t m_fight_role = 0;								// 和这个怪物发生战斗的玩家

	uint32_t m_alert_start_time = 0;						// 警戒开始时间
	uint64_t m_alert_uid = 0;								// 警戒对象
	std::string m_alert_name = "";							// 警戒对象名字
};

#endif // !__GAME_MONSTER_H__
