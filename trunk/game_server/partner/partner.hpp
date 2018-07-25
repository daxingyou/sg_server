#pragma once
#include "object/object_position.hpp"

class partner_tutor_t;
typedef boost::shared_ptr<partner_tutor_t> partner_tutor_ptr;

class partner_shadow_t;
typedef boost::shared_ptr<partner_shadow_t> partner_shadow_ptr;

class partner_t : public object_base_t
{
public:
	partner_t(uint64_t object_id);
	virtual ~partner_t();

	// 是否是伙伴
	virtual bool is_partner() { return true; }

	// 是否是导师
	virtual bool is_tutor() { return false; }

	// 是否是化身
	virtual bool is_shadow() { return false; }

	// 伙伴指针
	virtual partner_ptr get_partner() { return boost::dynamic_pointer_cast<partner_t>(shared_from_this()); }

	// 导师指针
	virtual partner_tutor_ptr get_partner_tutor() { return partner_tutor_ptr(); }

	// 化身指针
	virtual partner_shadow_ptr get_partner_shadow() { return partner_shadow_ptr(); }

public:
	// 设置组队id
	void set_troop_uid(uint64_t uid) { m_troop_id = uid; }
	// 得到队伍id
	uint64_t get_troop_uid() const { return m_troop_id; }

	// 获取对象场景属性
	virtual void get_object_info(proto::common::object_common_info* object_info);

public:
	// 战斗信息相关
	virtual uint32_t get_camp_unique_offset(uint32_t camp);

protected:
	uint64_t m_troop_id = 0;
};
