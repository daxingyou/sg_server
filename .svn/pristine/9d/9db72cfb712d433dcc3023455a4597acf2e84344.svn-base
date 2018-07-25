#pragma once

#include "partner.hpp"
#include "user_troop/user_troop.hpp"
#include "hero/hero.hpp"

class partner_shadow_t : public partner_t
{
public:
	partner_shadow_t(uint64_t object_id);
	virtual ~partner_shadow_t();

	// 清理
	virtual void del_clear() {};

	// 是否是导师
	virtual bool is_shadow() { return true; }

	// 获取对象类型
	proto::common::EM_SCENEOBJECT_TYPE get_object_type() { return proto::common::SCENEOBJECT_USERSHADOW; }

	// 导师指针
	virtual partner_shadow_ptr get_partner_shadow() { return boost::dynamic_pointer_cast<partner_shadow_t>(shared_from_this()); }

	// 获取对象场景属性
	virtual void get_object_info(proto::common::object_common_info* object_info);

	// 离开场景
	virtual void on_leave_scene(scene_ptr p_scene);

	virtual uint32_t get_level()
	{
		return m_level;
	}

	///=====================================================================
	///						战斗数据相关填充接口开始
	///=====================================================================
	virtual bool fill_fight_camp(
		uint32_t camp_index,
		uint64_t unique_offset,
		uint32_t team_member_size,
		proto::common::fight_camp* camp,
		proto::common::fight_type fight_type,
		proto::common::hero_state_data* state,
		int32_t team_member_pos = -1);

	// 填充武将列表
	virtual bool fill_hero_data(
		uint32_t camp_index,
		uint64_t unique_offset,
		uint32_t team_member_size,
		proto::common::fight_type fight_type,
		proto::common::hero_data* fhd,
		proto::common::hero_state_data* state = NULL,
		int32_t team_member_pos = -1);

	virtual hero_ptr get_hero_by_unique_id(uint64_t unique_id);

	virtual hero_map* get_hero_list();

	virtual bool get_hero_list(uint32_t fight_type, uint32_t team_member_size, std::vector<uint64_t>& list);
	///=====================================================================
	///						战斗数据相关填充接口结束
	///=====================================================================
	virtual std::string get_name() { return m_name; }

	virtual uint32_t get_head_id() { return m_head; }
public:
	// 初始化
	bool init_shadow(const troop_t::troop_member_ptr& p_member);

	bool init_hero_list();

protected:
	std::string m_name;
	uint32_t m_head = 0;
	uint32_t m_plugin = 0;
	uint32_t m_fighting = 0;
	uint32_t m_level = 0;
	uint32_t m_country = 0;
	uint32_t m_country_officer = 0;
	uint64_t m_family_id = 0;
	std::string m_family_name;
	uint32_t m_family_officer = 0;

	//proto::common::hero_data m_hero_data;	add by hy 提到partner.hpp 中

	hero_map m_hero_list;
};