#pragma once

#include "collect_point.hpp"
#include "family/family_war_common.hpp"

#include <map>

// �ա�����̨
class family_war_platform_t : public collect_point_t
{
public:
	family_war_platform_t(uint32_t tid);
	virtual ~family_war_platform_t();

	virtual family_war_platform_ptr get_family_war_platform() { return boost::dynamic_pointer_cast<family_war_platform_t>(shared_from_this()); }

public:
	// 9��
	virtual void get_object_info(proto::common::object_common_info* object_info);

	virtual uint32_t can_collect(role_ptr p_role);
	virtual uint32_t start_collect(role_ptr p_role);
	virtual uint32_t end_collect(role_ptr p_role, proto::common::role_change_data *rcd);
	virtual void cancel_collect(const role_ptr& p_role);

	// ��������
	void set_platform_type(family_war_platform_type type) { m_platform_type = type; }
	// ���ò�ս����
	void set_family_war_family(family_war_side_type type, uint64_t family_id);
	// �Ƿ��ǲ�ս����
	family_war_side_type get_family_war_side_type(uint64_t family_id);

	// ֪ͨ9������Լ��ĳ�����Ϣ
	void notify_object_common_info_for_nearbies();

private:
	// ���вɼ����
	std::map<uint64_t, uint32_t> m_collect_map;

	// ��ս����
	uint64_t m_war_family[family_war_side_max] = { 0 };

	// ռ�����
	uint64_t m_occupy_family = 0;

	// ����
	family_war_platform_type m_platform_type;
};