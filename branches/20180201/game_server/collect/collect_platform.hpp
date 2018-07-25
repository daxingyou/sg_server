#pragma once

#include "collect_point.hpp"
#include "family/family_war_common.hpp"

#include <map>

// 日、月曜台
class family_war_platform_t : public collect_point_t
{
public:
	family_war_platform_t(uint32_t tid);
	virtual ~family_war_platform_t();

	virtual family_war_platform_ptr get_family_war_platform() { return boost::dynamic_pointer_cast<family_war_platform_t>(shared_from_this()); }

public:
	// 9屏
	virtual void get_object_info(proto::common::object_common_info* object_info);

	virtual uint32_t can_collect(role_ptr p_role);
	virtual uint32_t start_collect(role_ptr p_role);
	virtual uint32_t end_collect(role_ptr p_role, proto::common::role_change_data *rcd);
	virtual void cancel_collect(const role_ptr& p_role);

	// 设置类型
	void set_platform_type(family_war_platform_type type) { m_platform_type = type; }
	// 设置参战家族
	void set_family_war_family(family_war_side_type type, uint64_t family_id);
	// 是否是参战家族
	family_war_side_type get_family_war_side_type(uint64_t family_id);

	// 通知9屏玩家自己的场景信息
	void notify_object_common_info_for_nearbies();

private:
	// 所有采集玩家
	std::map<uint64_t, uint32_t> m_collect_map;

	// 参战家族
	uint64_t m_war_family[family_war_side_max] = { 0 };

	// 占领家族
	uint64_t m_occupy_family = 0;

	// 类型
	family_war_platform_type m_platform_type;
};