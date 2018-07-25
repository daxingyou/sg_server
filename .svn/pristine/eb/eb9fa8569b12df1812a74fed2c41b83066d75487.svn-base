#include "object_position.hpp"
#include "main/game_server_fwd.hpp"
#include "main/game_server.hpp"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <boost/asio/time_traits.hpp>
#include "scene/scene.hpp"
#include "scene/scene_manager.hpp"
#include "monster/monster_manager.hpp"
#include "object_manager.hpp"
#include "role/role_manager.hpp"
#include "user_troop/user_troop_mgr.hpp"

#include <math.h>

object_base_t::object_base_t()
{
	
}

object_base_t::object_base_t(uint64_t unique_id) : m_object_id(unique_id)
{
}

object_base_t::~object_base_t()
{
}

void object_base_t::init(uint64_t unique_id)
{
	m_object_id = unique_id;
}

// 是否在地图的某个点附近
bool object_base_t::is_nearby(uint32_t map_id, float x, float y, float r)
{
	if (m_map_tid == map_id && 
		(this->m_current_x - x) * (this->m_current_x - x) + (this->m_current_y - y) * (this->m_current_y - y) <= r*r)
	{
		return true;
	}

	return false;
}

// 是在某个点附近
bool object_base_t::is_nearby_point(std::pair<float, float> point, float r)
{
	return (point.first - m_current_x) * (point.first - m_current_x) + (point.second - m_current_y) * (point.second - m_current_y) <= r * r;
}

// 是在某个点附近
bool object_base_t::is_nearby_point(std::pair<uint32_t, uint32_t> point, uint32_t r)
{
	return (point.first - m_current_x) * (point.first - m_current_x) + (point.second - m_current_y) * (point.second - m_current_y) <= r * r;
}

// 是否是附近的生物
bool object_base_t::is_nearby_object(object_base_ptr p_target, float radius)
{
	if (NULL == p_target)
	{
		log_error("pos NULL error");
		return false;
	}

	if (p_target->m_scene_id == m_scene_id)
	{
		return (p_target->m_current_x - m_current_x) * (p_target->m_current_x - m_current_x) + (p_target->m_current_y - m_current_y) * (p_target->m_current_y - m_current_y) <= radius * radius;
	}

	return false;
}

// 获取附近的生物信息（可以只获取玩家）
bool object_base_t::get_nearby_objects(std::vector<object_id_type>& objects_nearby, bool role_only/* = false*/, float distance/* = 0*/)
{
	const scene_ptr& p_scene = scene_manager_t::find_scene(m_scene_id);
	if (NULL == p_scene)
	{
		log_error("object[%lu] can not find scene[%lu] map[%u]", m_object_id, m_scene_id, m_map_tid);
		return false;
	}

	if (!p_scene->get_nearby_objects(m_current_x, m_current_y, objects_nearby, role_only, distance))
	{
		log_error("object[%lu] can not find nearby_objects scene[%lu] map[%u]", m_object_id, m_scene_id, m_map_tid);
		return false;
	}

	return true;
}

// 设置对象场景属性
void object_base_t::get_object_info(proto::common::object_common_info* object_info)
{
	if (NULL == object_info)
	{
		log_error("object_info null error");
		return;
	}

	object_info->set_object_id(string_util_t::uint64_to_string(m_object_id));
	object_info->set_object_type(get_object_type());
	object_info->set_current_x(m_current_x);
	object_info->set_current_y(m_current_y);
	object_info->set_direction(m_direction);
	object_info->set_speed(get_real_speed());
	//object_info->set_role_type(m_hero_tid);	wsy 注释 放到子类做去

	proto::common::object_state_info* object_state = object_info->mutable_object_state();
	if (object_state != NULL)
	{
		object_state->set_obj_state(m_object_state);
	}
}

void object_base_t::get_object_pos_info(proto::client::gc_move_notify* pos_info)
{
	if (pos_info) {
		pos_info->set_object_id(m_object_id);
		pos_info->set_pos_x(m_current_x * 100);
		pos_info->set_pos_y(m_current_y * 100);
		pos_info->set_direction(m_direction * 100);
		pos_info->set_speed(m_real_speed);
		pos_info->set_high(m_current_high);
	}
}

void object_base_t::set_object_born(uint64_t father /*= 0*/)
{
	on_object_born();
}

void object_base_t::on_object_born()
{
	// 设置
	set_object_state(proto::common::object_state_normal);
}

void object_base_t::set_object_die(uint64_t obj_killer)
{
	on_object_die();
}

void object_base_t::on_object_die()
{
	// 状态设置为死亡
	set_object_state(proto::common::object_state_death);
}

void object_base_t::set_object_revive(uint64_t obj_god /*= 0*/)
{
	on_object_revive();
}

void object_base_t::on_object_revive()
{
	// 设置
	set_object_state(proto::common::object_state_normal);
}

void object_base_t::send_msg_to_nine(uint16_t cmd, const std::string& cmd_data)
{
	const scene_ptr& p_scene = scene_manager_t::find_scene(m_scene_id);
	if (NULL == p_scene)
	{
		log_error("object[%lu] can not find scene[%lu] map[%u]", m_object_id, m_scene_id, m_map_tid);
		return;
	}

	p_scene->send_msg_to_nine(m_nine_key, cmd, cmd_data);
}

void object_base_t::leave_scene()
{
	const scene_ptr& p_old_scene = scene_manager_t::find_scene(m_scene_id);
	if (NULL != p_old_scene)
	{
		p_old_scene->on_leave_s(shared_from_this());
	}
}
