#pragma once

#include "partner.hpp"
#include "tblh/TutorTable.tbls.h"
#include "common.pb.h"
#include "hero/hero.hpp"

class partner_tutor_t : public partner_t
{
public:
	partner_tutor_t(uint64_t object_id);
	virtual ~partner_tutor_t();

	// 清理
	virtual void del_clear() {};

	// 是否是导师
	virtual bool is_tutor() { return true; }

	// 获取对象类型
	proto::common::EM_SCENEOBJECT_TYPE get_object_type() { return proto::common::SCENEOBJECT_TUTOR; }

	// 导师指针
	virtual partner_tutor_ptr get_partner_tutor() { return boost::dynamic_pointer_cast<partner_tutor_t>(shared_from_this()); }

	// 获取对象场景属性
	virtual void get_object_info(proto::common::object_common_info* object_info);

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

public:
	// 初始化
	bool init_tutor(TutorTable* p_conf, const std::string& name, uint32_t level );

	bool init_hero_list();

protected:
	uint32_t m_tutor_tid = 0;	// tid
	std::string m_name;			// 名字
	uint32_t m_plugin = 0;		// 头像
	uint32_t m_level = 0;		// 等级
	uint32_t m_power = 0;		// 战力
	uint32_t m_fight_id = 0;	// 对战id
	hero_map m_hero_list;
};