#include "collect_platform.hpp"
#include "config/config_manager.hpp"
#include "tblh/CollectTable.tbls.h"
#include "tblh/errcode_enum.hpp"
#include "scene/scene_manager.hpp"
#include "collect_manager.hpp"
#include "family/family_war_scene.hpp"

family_war_platform_t::family_war_platform_t(uint32_t tid)
	: collect_point_t(tid)
{

}

family_war_platform_t::~family_war_platform_t()
{

}

void family_war_platform_t::get_object_info(proto::common::object_common_info* object_info)
{
	if (NULL == object_info)
	{
		log_error("object_info null error");
		return;
	}

	collect_point_t::get_object_info(object_info);

	object_info->set_family_id(string_util_t::uint64_to_string(m_occupy_family));
}

uint32_t family_war_platform_t::can_collect(role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("collect p_role NULL");
		return errcode_enum::notice_unknown;
	}

	// 死亡不能采集
	if (p_role->is_die())
	{
		log_error("role[%lu] is die", p_role->get_object_id());
		return errcode_enum::sys_notice_you_dead;
	}

	if (p_role->is_troop_member_not_leave()) {
		log_error("role[%lu] troop member can't collect", p_role->get_uid());
		return errcode_enum::notice_unknown;
	}

	// 判断状态
	if (is_die())
	{
		return errcode_enum::family_err_war_platform_error;
	}

	// 是否是参战家族
	family_war_side_type side_type = get_family_war_side_type(p_role->get_family_id());
	if (side_type == family_war_side_none)
	{
		log_error("role[%lu] not family war family", p_role->get_uid());
		return errcode_enum::family_err_war_platform_error;
	}

	if (m_war_family[side_type] == m_occupy_family)
	{
		FAMILY_LOG("role[%lu] family is already occupy", p_role->get_uid());
		return errcode_enum::family_err_war_is_already_occupied;
	}

	return errcode_enum::error_ok;
}

uint32_t family_war_platform_t::start_collect(role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("collect p_role NULL");
		return errcode_enum::notice_unknown;
	}

	m_collect_map[p_role->get_uid()] = common::time_util_t::now_time();

	return errcode_enum::error_ok;
}

uint32_t family_war_platform_t::end_collect(role_ptr p_role, proto::common::role_change_data *rcd)
{
	if (NULL == p_role)
	{
		log_error("collect p_role NULL");
		return errcode_enum::notice_unknown;
	}

	family_war_side_type side_type = get_family_war_side_type(p_role->get_family_id());
	if (side_type == family_war_side_none)
	{
		log_error("role[%lu] not family war family", p_role->get_uid());
		return errcode_enum::family_err_war_platform_error;
	}

	std::map<uint64_t, uint32_t>::iterator itr = m_collect_map.find(p_role->get_uid());
	if (itr == m_collect_map.end())
	{
		if (m_occupy_family != 0)
		{
			log_warn("role[%lu] collect is already occupied", p_role->get_uid());
			return errcode_enum::family_err_war_platform_already_error;
		}
		else
		{
			log_error("role[%lu] collect failed", p_role->get_uid());
			return errcode_enum::family_err_war_platform_error;
		}
	}

	if (common::time_util_t::now_time() < itr->second + m_collect_time)
	{
		log_warn("role[%lu] collect time failed", p_role->get_uid());
		return errcode_enum::notice_is_collecting_time;
	}

	const scene_ptr& p_scene = scene_manager_t::find_scene(m_scene_id);
	if (NULL == p_scene)
	{
		log_error("scene[%lu] can not find", m_scene_id);
		return errcode_enum::notice_unknown;
	}

	if (!p_scene->is_family_war_scene())
	{
		log_error("scene[%lu] is not family war scene", m_scene_id);
		return errcode_enum::notice_unknown;
	}

	family_war_scene_ptr p_family_war_scene = p_scene->get_family_war_scene_ptr();
	if (p_family_war_scene == NULL)
	{
		log_error("scene[%lu] is not family war scene", m_scene_id);
		return errcode_enum::notice_unknown;
	}

	m_occupy_family = m_war_family[side_type];

	// 9屏通知
	notify_object_common_info_for_nearbies();

	p_family_war_scene->family_occupied_platform(p_role, m_platform_type, side_type);

	// 清空采集表
	m_collect_map.clear();

	return errcode_enum::error_ok;
}

void family_war_platform_t::cancel_collect(const role_ptr& p_role)
{
	if (NULL == p_role)
	{
		log_error("collect p_role NULL");
		return;
	}

	std::map<uint64_t, uint32_t>::iterator itr = m_collect_map.find(p_role->get_uid());
	if (itr != m_collect_map.end())
	{
		m_collect_map.erase(itr);
	}
}

void family_war_platform_t::set_family_war_family(family_war_side_type type, uint64_t family_id)
{
	if (type >= family_war_side_max)
	{
		log_error("family type[%u] error", type);
		return;
	}

	m_war_family[type] = family_id;
}

family_war_side_type family_war_platform_t::get_family_war_side_type(uint64_t family_id)
{
	if (family_id == m_war_family[family_war_side_left])
	{
		return family_war_side_left;
	}
	else if (family_id == m_war_family[family_war_side_right])
	{
		return family_war_side_right;
	}

	return family_war_side_none;
}

void family_war_platform_t::notify_object_common_info_for_nearbies()
{
	proto::client::gc_scene_object_info_notify ntf;
	get_object_info(ntf.mutable_common_info());
	send_msg_to_nine(op_cmd::gc_scene_object_info_notify, ntf);
}
