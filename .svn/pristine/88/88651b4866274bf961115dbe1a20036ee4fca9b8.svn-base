#include "role_scene_info.hpp"
#include "common/log.hpp"
#include "utility.hpp"
#include "scene/map.hpp"
#include "scene/scene_manager.hpp"
#include "role.hpp"

USING_NS_COMMON;

role_scene_t::role_scene_t(uint64_t owner)
: m_owner(owner)
{

}

void role_scene_t::role_enter_scene(const role_ptr& p_role, const scene_ptr& p_scene)
{
	if (NULL == p_role) {
		log_error("p_role null error");
		return;
	}

	if (NULL == p_scene) {
		log_error("p_scene null error");
		return;
	}

	m_map_tid = p_scene->get_map_tid();
	m_dungeon_id = p_scene->get_dungeon_id();

	update_area(p_role, p_scene);
}

void role_scene_t::role_reenter_scene(const role_ptr& p_role, const scene_ptr& p_scene)
{
	if (NULL == p_role) {
		log_error("p_role null error");
		return;
	}

	if (NULL == p_scene) {
		log_error("p_scene null error");
		return;
	}

	// 区域重新通知客户端
	proto::client::gc_area_id_notify ntf;
	ntf.set_area_id(m_current_area_tid);
	p_role->send_msg_to_client(op_cmd::gc_area_id_notify, ntf);
}

void role_scene_t::role_leave_scene()
{
	set_patrol_state(false);

	clear_area_trrigger_info();
}

void role_scene_t::set_last_scene_info(uint32_t last_map_tid, float last_pos_x, float last_pos_y, float last_dir)
{
	m_last_map_tid = last_map_tid;
	m_last_x = last_pos_x;
	m_last_y = last_pos_y;
	m_last_dir = last_dir;
}

void role_scene_t::set_last_scene_info(const proto::common::last_static_map& info)
{
	m_last_map_tid = info.last_map_tid();
	m_last_x = info.last_pos_x();
	m_last_y = info.last_pos_y();
	m_last_dir = info.last_dir();
}

void role_scene_t::get_last_scene_info(proto::common::last_static_map* info)
{
	if (NULL != info)
	{
		info->set_last_map_tid(m_last_map_tid);
		info->set_last_pos_x(m_last_x);
		info->set_last_pos_y(m_last_y);
		info->set_last_dir(m_last_dir);
	}
}

void role_scene_t::update_area(role_ptr p_role, scene_ptr p_scene)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		return;
	}

	if (NULL == p_scene)
	{
		log_error("p_scene null error");
		return;
	}

	uint32_t curr_area_id = p_scene->get_area_id(p_role->get_current_x(), p_role->get_current_y());

	if (curr_area_id == 0 || m_current_area_tid == curr_area_id)
		return;

	// 离开某个区域
	if (m_current_area_tid != 0)
		p_scene->role_leave_area(p_role, m_current_area_tid);

	// 设置新的区域
	clear_area_trrigger_info();

	map_ptr p_map = p_scene->get_map_ptr();
	if (p_map == NULL)
	{
		log_error("map[%u] error null", m_map_tid);
		return;
	}

	m_current_area_tid = curr_area_id;

	map_area_ptr area_info = p_map->get_map_area_info(m_current_area_tid);
	if (area_info != NULL)
	{
		m_trigger_min_time = area_info->get_trigger_min();
		m_trigger_max_time = area_info->get_trigger_max();

		m_monster_trigger = area_info->is_have_dark();

		std::pair<uint32_t, uint32_t> task_dark = area_info->has_task_dark(p_role);
		if (task_dark.first != 0)
		{
			m_task_id = task_dark.first;
			m_task_trigger_monster = task_dark.second;
		}

		if (m_monster_trigger || m_task_trigger_monster != 0)
		{
			reset_area_trigger_time();
		}
	}

	// 进入某个新的区域
	p_scene->role_enter_area(p_role, curr_area_id);

	// 区域通知
	proto::client::gc_area_id_notify ntf;
	ntf.set_area_id(curr_area_id);
	p_role->send_msg_to_client(op_cmd::gc_area_id_notify, ntf);

	// 脚本事件
	p_role->script_event_enter_area(curr_area_id);

	SCENE_LOG("role[%lu] enter area [%d]", p_role->get_uid(), m_current_area_tid);
}

bool role_scene_t::is_in_safe_area() const
{
	return scene_manager_t::is_safe_area(m_current_area_tid);
}

void role_scene_t::update_area_trigger(const role_ptr& p_role)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		return;
	}

	map_ptr p_map = scene_manager_t::find_map_by_id(m_map_tid);
	if (p_map == NULL)
	{
		log_error("map[%u] error null", m_map_tid);
		return;
	}

	map_area_ptr area_info = p_map->get_map_area_info(m_current_area_tid);
	if (area_info != NULL)
	{
		std::pair<uint32_t, uint32_t> task_dark = area_info->has_task_dark(p_role);
		m_task_id = task_dark.first;
		m_task_trigger_monster = task_dark.second;
	}

	if (m_monster_trigger || m_task_trigger_monster != 0)
	{
		reset_area_trigger_time();
	}
	else
	{
		m_trigger_time = 0;
	}
}

bool role_scene_t::get_scene_trigger_status()
{
	return m_is_patrol || m_task_trigger_monster != 0 || m_monster_trigger;
}

void role_scene_t::reset_area_trigger_time()
{
	if (m_trigger_max_time >= m_trigger_min_time)
	{
		m_trigger_time = random_util_t::randBetween(m_trigger_min_time, m_trigger_max_time);
	}
	else
	{
		m_trigger_time = random_util_t::randBetween(m_trigger_max_time, m_trigger_min_time);
	}
}

void role_scene_t::reset_trigger_time()
{
	if (m_is_patrol)
	{
		reset_patrol_trigger_time();
	}
	else
	{
		reset_area_trigger_time();
	}
}

uint32_t role_scene_t::trigger_time_pass()
{
	if (m_is_patrol)
	{
		//log_warn("m_patrol_time:%u", m_patrol_time);
		if (m_patrol_time > 0)
			m_patrol_time--;

		return m_patrol_time;
	}
	else
	{
		if (m_trigger_time > 0)
			m_trigger_time--;

		return m_trigger_time;
	}

	return 0;
}

void role_scene_t::reset_patrol_trigger_time()
{
	if (m_patrol_max_time >= m_patrol_min_time)
	{
		m_patrol_time = random_util_t::randBetween(m_patrol_min_time, m_patrol_max_time);
	}
	else
	{
		m_patrol_time = random_util_t::randBetween(m_patrol_max_time, m_patrol_min_time);
	}
}

uint32_t role_scene_t::get_trigger_monster_id()
{
	if (m_is_patrol)
	{
		map_ptr p_map = scene_manager_t::find_map_by_id(m_map_tid);
		if (p_map != NULL)
		{
			const map_patrol_ptr& p_map_patrol = p_map->get_map_patrol_info();
			if (p_map_patrol != NULL)
			{
				return p_map_patrol->random_dark_monster();
			}
		}
	}
	
	if (m_task_trigger_monster != 0)
	{
		return m_task_trigger_monster;
	}
	
	if(m_monster_trigger)
	{
		map_ptr p_map = scene_manager_t::find_map_by_id(m_map_tid);
		if (p_map != NULL)
		{
			map_area_ptr area_info = p_map->get_map_area_info(m_current_area_tid);
			if (area_info != NULL)
			{
				return area_info->random_dark_monster();
			}
		}
	}

	log_error("role[%lu] no trigger montser id", m_owner);

	return 0;
}

void role_scene_t::clear_area_trrigger_info()
{
	m_current_area_tid = 0;
	m_trigger_min_time = 0;
	m_trigger_max_time = 0;

	m_monster_trigger = false;
	m_trigger_time = 0;

	m_task_id = 0;
	m_task_trigger_monster = 0;
}

bool role_scene_t::set_patrol_state(bool is_patrol)
{
	if (is_patrol)
	{
		map_ptr p_map = scene_manager_t::find_map_by_id(m_map_tid);
		if (p_map == NULL)
		{
			log_error("map[%u] error null", m_map_tid);
			return false;
		}

		const map_patrol_ptr& p_map_patrol = p_map->get_map_patrol_info();
		if (p_map_patrol == NULL)
		{
			log_error("map[%u] map_patrol_ptr == null error", m_map_tid);
			return false;
		}

		m_is_patrol = true;

		m_patrol_min_time = p_map_patrol->get_trigger_min();
		m_patrol_max_time = p_map_patrol->get_trigger_max();

		reset_patrol_trigger_time();
	}
	else
	{
		m_is_patrol = false;

		m_patrol_min_time = 0;
		m_patrol_max_time = 0;

		m_patrol_time = 0;
	}

	return true;
}
