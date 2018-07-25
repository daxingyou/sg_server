#include "scene.hpp"

#include <algorithm>
#include "common_fwd.hpp"
#include "monster/monster_manager.hpp"
#include "scene/scene_manager.hpp"
#include "tblh/CollectTable.tbls.h"
#include "tblh/JumpTable.tbls.h"
#include "fight/fight_manager.hpp"
#include "tblh/errcode_enum.hpp"

scene_t::scene_t(uint64_t scene_id, uint32_t map_id, proto::server::em_scene_type scene_type, uint32_t type_param)
	: m_scene_id(scene_id)
	, m_map_tid(map_id)
	, m_scene_type(scene_type)
	, m_type_param(type_param)
{
	
}

scene_t::~scene_t()
{
	SAFE_DELETE_VEC(m_scene_objects);
}

bool scene_t::init()
{
	map_ptr p_map = scene_manager_t::find_map_by_id(m_map_tid);
	if (NULL == p_map)
	{
		log_error("map[%u] null error", m_map_tid);
		return false;
	}

	m_map_wptr = p_map;

	m_map_type = p_map->get_map_type();

	// 安全起见记录格子数
	m_grid_num = p_map->get_map_max_size();

	m_scene_objects = new scene_grid*[m_grid_num];
	for (uint32_t i = 0; i < m_grid_num; ++i)
	{
		m_scene_objects[i] = NULL;
	}

	return true;
}


bool scene_t::start()
{
	load_map_object();

	load_map_collect();

	set_scene_state(SCENE_STATE_NORMAL);

	//SCENE_LOG("scene[%lu] map[%u] start", m_scene_id, m_map_tid);

	return true;
}

void scene_t::load_map_object()
{
	map_ptr p_map = get_map_ptr();
	if (NULL == p_map)
	{
		log_error("map[%u] null error", m_map_tid);
		return;
	}

	const monster_define_vec& monster_vec = p_map->get_map_monsters();
	for (monster_define_vec::const_iterator itr = monster_vec.begin();
		itr != monster_vec.end(); ++itr)
	{
		const monster_common::monster_define_t& define = *itr;

		Monster* monster = GET_CONF(Monster, define.id);
		if (NULL == monster)
		{
			log_error("scene::load_map_object not found monster [%u]", define.id);
			continue;
		}

		monster_ptr p_monster(new monster_t());
		if (p_monster != NULL && p_monster->init_monster(define, monster))
		{
			init_object(p_monster, define);
		}
	}
}

void scene_t::load_map_collect()
{
	map_ptr p_map = get_map_ptr();
	if (NULL == p_map)
	{
		log_error("map[%u] null error", m_map_tid);
		return;
	}

	const collect_define_vec& collect_vec = p_map->get_map_collect_points();
	for (collect_define_vec::const_iterator itr = collect_vec.begin();
		itr != collect_vec.end(); ++itr)
	{
		const t_collect_define& define = *itr;

		CollectTable* p_collect_config = GET_CONF(CollectTable, define.m_collect_id);
		if (NULL == p_collect_config)
		{
			log_error("scene::load_map_object not found collect [%d]", define.m_collect_id);
			continue;
		}

		collect_point_ptr p_collect_point(new collect_point_t(define.m_collect_id));
		if (p_collect_point != NULL && p_collect_point->init_collect_point(define.m_x, define.m_y))
		{
			init_collect_point(p_collect_point);
		}
	}
}

bool scene_t::unload()
{
	// 卸载场景保证没人，有人的先把人提走
	if (!m_roles.empty())
	{
		log_error("scene[%lu] map[%u] have other role num[%u]", m_scene_id, m_map_tid, (uint32_t)m_roles.size());
	}

	clear();

	set_scene_state(SCENE_STATE_UNLOADED);

	SCENE_LOG("scene[%lu] map[%u] unload", m_scene_id, m_map_tid);

	return true;
}

// 获取区域ID
uint32_t scene_t::get_area_id(float x, float y)
{
	map_ptr p_map = get_map_ptr();
	if (NULL == p_map)
	{
		log_error("map[%u] null error", m_map_tid);
		return 0;
	}

	return p_map->get_area_id((uint32_t)x, (uint32_t)y);
}

uint32_t scene_t::get_revive_point()
{
	map_ptr p_map = get_map_ptr();
	if (NULL == p_map)
	{
		log_error("map[%u] null error", m_map_tid);
		return 0;
	}

	return p_map->get_revive_point();
}

void scene_t::clear()
{
	m_roles.clear();
	m_field_boss.clear();

	for (uint32_t i = 0; i < m_grid_num; ++i)
	{
		if (NULL != m_scene_objects[i])
		{
			for (scene_grid::const_iterator citr = m_scene_objects[i]->begin();
				citr != m_scene_objects[i]->end(); ++citr)
			{
				// 直接清空，因为没人了不需要9屏
				object_manager_t::delete_creature(*citr);
			}

			m_scene_objects[i]->clear();
			SAFE_DELETE_P(m_scene_objects[i]);
		}
	}
}

bool scene_t::on_enter_s(object_base_ptr p_object, float x, float y, float dir /*= 0.f*/)
{
	if (NULL == p_object)
	{
		log_error("p_object error null");
		return false;
	}

	map_ptr p_map = get_map_ptr();
	if (NULL == p_map)
	{
		log_error("map[%u] null error", m_map_tid);
		return false;
	}

	// 设置屏索引
	uint32_t nine_key = p_map->convert_xy_2_map_key(x, y);
	if (INVALID_INDEX == nine_key)
	{
		log_error("p_object[%lu] pos[%f, %f] error nine_key[%u]", p_object->get_object_id(), x, y, nine_key);
		return false;
	}

	// 设置位置信息
	p_object->set_scene_info(m_scene_id, m_map_tid, m_scene_type);
	p_object->set_current_position(x, y, dir);
	p_object->set_nine_key(nine_key);

	// 加入场景内
	if (NULL == m_scene_objects[nine_key])
	{
		m_scene_objects[nine_key] = new scene_grid();
	}
	m_scene_objects[nine_key]->insert(p_object->get_object_id_type());

	// 刷新9屏，人物的9屏进场景时候不处理
	object_into_vision(p_object, p_map->get_nine_keys(nine_key));

	// 生物进入场景
	p_object->on_enter_scene(shared_from_this());

	// 场景中有人进入
	if (p_object->is_role())
	{
		m_roles.insert(p_object->get_object_id());

		on_role_enter(p_object->get_role());
	}

	// SCENE_LOG("[scene] object [%lu, %d] try enter scene[%lu] map[%u]", p_object->get_object_id(), p_object->get_object_type(), m_scene_id, m_map_tid);

	return true;
}

void scene_t::on_leave_s(object_id_type object_id)
{
	object_base_ptr p_object = object_manager_t::find_object(object_id);
	if (NULL == p_object)
	{
		log_error("object[%lu] not find when leave scene[%lu] map[%u]", object_id.first, m_scene_id, m_map_tid);
		return;
	}

	on_leave_s(p_object);
}

void scene_t::on_leave_s(object_base_ptr p_object)
{
	if (NULL == p_object)
	{
		log_error("p_object error null");
		return;
	}

	map_ptr p_map = get_map_ptr();
	if (NULL == p_map)
	{
		log_error("map[%u] null error", m_map_tid);
		return;
	}

	uint32_t nine_key = p_object->get_nine_key();
	if (INVALID_INDEX == nine_key)
	{
		log_error("p_object[%lu] error nine_key[%u]", p_object->get_object_id(), nine_key);
		return;
	}

	p_object->on_leave_scene(shared_from_this());

	if (m_scene_objects[nine_key] != NULL)
	{
		m_scene_objects[nine_key]->erase(p_object->get_object_id_type());
	}

	object_out_of_vision(p_object, p_map->get_nine_keys(nine_key));

	if (p_object->is_role())
	{
		m_roles.erase(p_object->get_object_id());

		on_role_leave(p_object->get_role());
	}
}

bool scene_t::on_move_s(const object_base_ptr& p_object, float to_x, float to_y, float direction, uint64_t time_stamp, uint32_t high)
{
	if (NULL == p_object)
	{
		log_error("p_object null error");
		return false;
	}

	map_ptr p_map = get_map_ptr();
	if (NULL == p_map)
	{
		log_error("map[%u] null error", m_map_tid);
		return false;
	}

	if (fabs(p_object->get_current_x() - to_x) + fabs(p_object->get_current_y() - to_y) < 0.01f)
	{
		//SCENE_LOG("onject[%lu] move too near from[%f, %f] to[%f, %f] time_stamp[%lu] dir[%f]", object_id.first, p_object->get_current_x(), p_object->get_current_y(),
		//	to_x, to_y, time_stamp, direction);
		return false;
	}

	object_id_type object_id = p_object->get_object_id_type();

	uint32_t from_key = p_object->get_nine_key();
	uint32_t to_key = p_map->convert_xy_2_map_key(to_x, to_y);
	if (INVALID_INDEX == to_key)
	{
		log_error("p_object[%lu] pos[%f, %f] error nine_key[%u]", object_id.first, to_x, to_y, to_key);
		return false;
	}

	p_object->set_current_position(to_x, to_y, direction, time_stamp, high);

	if (from_key != to_key)
	{
		p_object->set_nine_key(to_key);

		if (m_scene_objects[from_key] != NULL)
		{
			m_scene_objects[from_key]->erase(object_id);
		}

		if (m_scene_objects[to_key] == NULL)
		{
			m_scene_objects[to_key] = new scene_grid();
		}
		m_scene_objects[to_key]->insert(object_id);

		int32_t dir = p_map->getScreenDirect(from_key, to_key);
		//printf("\033[31m from key:[%u] from key:[%u]\033[0m\n", from_key, to_key);

		const nine_vector& reversedirect_screen = dir != _DIR_NOT_CROSS ? p_map->get_reversedirect_screen(from_key, dir) : p_map->get_nine_keys(from_key);
		const nine_vector& direct_screen = dir != _DIR_NOT_CROSS ? p_map->get_direct_screen(to_key, dir) : p_map->get_nine_keys(to_key);

		if (object_id.second == proto::common::SCENEOBJECT_USER)
		{
			role_out_of_vision(p_object->get_role(), reversedirect_screen);

			role_into_vision(p_object->get_role(), direct_screen);
		}
		else
		{
			object_out_of_vision(p_object, reversedirect_screen);

			object_into_vision(p_object, direct_screen);
		}

		notify_nearbies_move_s(p_object, p_map->get_nine_keys(to_key));
	}
	else
	{
		//printf("\033[31m same key:[%u]\033[0m\n", from_key);

		notify_nearbies_move_s(p_object, p_map->get_nine_keys(from_key));
	}

	// 移动回掉
	p_object->on_move_event(shared_from_this());

	// SCENE_LOG("object[%lu] move to[%f, %f] time_stamp[%lu] dir[%f]", object_id.first, to_x, to_y, time_stamp, direction);

	return true;
}

bool scene_t::get_nearby_objects(float x, float y, std::vector<object_id_type>& objects_nearby, bool role_only/* = false*/, float distance /*= 0*/, bool check_block /*= false*/)
{
	map_ptr p_map = get_map_ptr();
	if (NULL == p_map)
	{
		log_error("map[%u] null error", m_map_tid);
		return false;
	}

	uint32_t nine_key = p_map->convert_xy_2_map_key(x, y);
	if (INVALID_INDEX == nine_key)
	{
		log_error("pos[%f, %f] error nine_key[%u]", x, y, nine_key);
		return false;
	}

	const nine_vector& nine_keys = p_map->get_nine_keys(nine_key);

	objects_nearby.clear();

	for (const auto& key : nine_keys)
	{
		auto ptr = m_scene_objects[key];
		if (ptr != NULL)
		{
			for (const auto& itr : *ptr)
			{
				// 只需要玩家
				if (role_only && itr.second != proto::common::SCENEOBJECT_USER)
					continue;

				// 距离判断
				if (distance > 0)
				{
					object_base_ptr p_nearby = object_manager_t::find_object(itr);
					if (NULL == p_nearby)
						continue;

					// 这个还可以优化，开平方太耗
					if (sqrt(pow(p_nearby->get_current_x() - x, 2) + pow(p_nearby->get_current_y() - y, 2)) > distance)
						continue;
				}

				objects_nearby.push_back(itr);
			}
		}
	}

	return true;
}

/**
 * \brief 除人物以外的生物视野处理或者人第一次进入场景的视野处理
 * \brief 人物跳场景一开始不需要通知视野所以和怪物的处理方式一样
 */
bool scene_t::object_into_vision(object_base_ptr p_object, const nine_vector& nines)
{
	if (NULL == p_object)
	{
		log_error("p_object null error");
		return false;
	}
 
	object_id_type object_id = p_object->get_object_id_type();

	proto::server::ga_role_into_vision_notify msg_to_gate;

	for (nine_vector::const_iterator ncitr = nines.begin();
		ncitr != nines.end(); ++ncitr)
	{
		scene_grid* p_grid = m_scene_objects[*ncitr];
		if (p_grid == NULL || p_grid->empty())
			continue;

		for (scene_grid::const_iterator citr = p_grid->begin();
			citr != p_grid->end(); ++citr)
		{
			if (object_id.first == citr->first)
				continue;

			if (citr->second == proto::common::SCENEOBJECT_USER)
			{
				msg_to_gate.add_role_list(citr->first);

				p_object->other_object_into_vision(*citr);
			}
			else
			{
				if (object_id.second == proto::common::SCENEOBJECT_USER)
				{
					object_base_ptr p_near_object = object_manager_t::find_object(*citr);
					if (p_near_object != NULL)
					{
						p_near_object->other_object_into_vision(object_id);
					}
				}
			}
		}
	}

	if (msg_to_gate.role_list_size() > 0)
	{
		proto::client::gc_role_into_vision_notify* p_scene_info_ntf = msg_to_gate.mutable_msg();
		if (p_scene_info_ntf != NULL)
		{
			p_object->get_object_info(p_scene_info_ntf->add_common_list());
		}

		env::server->send_msg_to_all_gate(op_cmd::ga_role_into_vision_notify, 0, msg_to_gate);
	}

	return true;
}

/**
 * \brief 收集9屏信息
 */
bool scene_t::send_nine_to_me(uint32_t nine_key, proto::client::gc_role_into_vision_notify* notify)
{
	if (notify == NULL)
	{
		log_error("notify null error");
		return false;
	}

	map_ptr p_map = get_map_ptr();
	if (NULL == p_map)
	{
		log_error("map[%u] null error", m_map_tid);
		return false;
	}

	if (INVALID_INDEX == nine_key)
	{
		log_error("nine_key[%u] error", nine_key);
		return false;
	}

	const nine_vector& nines = p_map->get_nine_keys(nine_key);

	for (nine_vector::const_iterator ncitr = nines.begin();
		ncitr != nines.end(); ++ncitr)
	{
		scene_grid* p_grid = m_scene_objects[*ncitr];
		if (p_grid == NULL || p_grid->empty())
			continue;

		for (scene_grid::const_iterator citr = p_grid->begin();
			citr != p_grid->end(); ++citr)
		{
			object_base_ptr p_near_object = object_manager_t::find_object(*citr);
			if (p_near_object == NULL)
				continue;

			p_near_object->get_object_info(notify->add_common_list());
		}
	}

	return true;
}

/**
* \brief 人物在移动中的视野处理，需要更新9屏数据给人
*/
bool scene_t::role_into_vision(role_ptr p_role, const nine_vector& nines)
{
	if (NULL == p_role)
	{
		log_error("error null p_role");
		return false;
	}

	object_id_type object_id = p_role->get_object_id_type();

	proto::server::ga_role_into_vision_notify msg_to_gate;
	proto::client::gc_role_into_vision_notify ntf_to_role;

	for (nine_vector::const_iterator ncitr = nines.begin();
		ncitr != nines.end(); ++ncitr)
	{
		scene_grid* p_grid = m_scene_objects[*ncitr];
		if (p_grid == NULL || p_grid->empty())
			continue;

		for (scene_grid::const_iterator citr = p_grid->begin();
			citr != p_grid->end(); ++citr)
		{
			object_base_ptr p_near_object = object_manager_t::find_object(*citr);
			if (p_near_object == NULL)
				continue;

			p_near_object->get_object_info(ntf_to_role.add_common_list());

			if (object_id.first == citr->first)
				continue;

			if (citr->second == proto::common::SCENEOBJECT_USER)
			{
				msg_to_gate.add_role_list(citr->first);
			}
			else
			{
				p_near_object->other_object_into_vision(object_id);
			}
		}
	}

	if (ntf_to_role.common_list_size() > 0)
		p_role->send_msg_to_client(op_cmd::gc_role_into_vision_notify, ntf_to_role);

	if (msg_to_gate.role_list_size() > 0)
	{
		proto::client::gc_role_into_vision_notify* p_scene_info_ntf = msg_to_gate.mutable_msg();
		if (p_scene_info_ntf != NULL)
		{
			p_role->get_object_info(p_scene_info_ntf->add_common_list());
		}

		env::server->send_msg_to_all_gate(op_cmd::ga_role_into_vision_notify, 0, msg_to_gate);
	}

	return true;
}

/**
* \brief 除人物以外的生物视野处理或者人离开场景的视野处理
* \brief 人物跳场景不需要把离开的生物告诉人物，客户端直接全部清除
*/
bool scene_t::object_out_of_vision(object_base_ptr p_object, const nine_vector& nines)
{
	if (NULL == p_object)
	{
		log_error("p_object null error");
		return false;
	}

	object_id_type object_id = p_object->get_object_id_type();

	proto::server::ga_role_out_vision_notify msg_to_gate;

	for (nine_vector::const_iterator ncitr = nines.begin();
		ncitr != nines.end(); ++ncitr)
	{
		scene_grid* p_grid = m_scene_objects[*ncitr];
		if (p_grid == NULL || p_grid->empty())
			continue;

		for (scene_grid::const_iterator citr = p_grid->begin();
			citr != p_grid->end(); ++citr)
		{
			if (object_id.first == citr->first)
				continue;

			if (citr->second == proto::common::SCENEOBJECT_USER)
			{
				msg_to_gate.add_role_list(citr->first);

				p_object->other_object_out_of_vision(*citr);
			}
			else
			{
				if (object_id.second == proto::common::SCENEOBJECT_USER)
				{
					object_base_ptr p_near_object = object_manager_t::find_object(*citr);
					if (p_near_object != NULL)
					{
						p_near_object->other_object_out_of_vision(object_id);
					}
				}
			}
		}
	}

	if (msg_to_gate.role_list_size() > 0)
	{
		proto::client::gc_role_outo_vision_notify* p_notify_msg_nearby = msg_to_gate.mutable_msg();
		if (p_notify_msg_nearby != NULL)
		{
			proto::common::object_common_info* obj_self = p_notify_msg_nearby->add_common_list();
			if (obj_self != NULL)
			{
				obj_self->set_object_id(string_util_t::uint64_to_string(object_id.first));
				obj_self->set_object_type(object_id.second);
			}
		}

		env::server->send_msg_to_all_gate(op_cmd::ga_role_out_vision_notify, 0, msg_to_gate);
	}

	return true;
}

bool scene_t::role_out_of_vision(role_ptr p_role, const nine_vector& nines)
{
	if (NULL == p_role)
	{
		log_error("error null p_role");
		return false;
	}

	object_id_type object_id = p_role->get_object_id_type();

	// 玩家列表
	proto::server::ga_role_out_vision_notify msg_to_gate;
	proto::client::gc_role_outo_vision_notify ntf_to_role;

	// 查找9屏内其他生物（不包括自己）
	for (nine_vector::const_iterator ncitr = nines.begin();
		ncitr != nines.end(); ++ncitr)
	{
		scene_grid* p_grid = m_scene_objects[*ncitr];
		if (p_grid == NULL || p_grid->empty())
			continue;

		for (scene_grid::const_iterator citr = p_grid->begin();
			citr != p_grid->end(); ++citr)
		{
			if (object_id.first == citr->first)
				continue;

			proto::common::object_common_info* object_info = ntf_to_role.add_common_list();
			if (object_info != NULL)
			{
				object_info->set_object_id(string_util_t::uint64_to_string(citr->first));
				object_info->set_object_type(citr->second);
			}

			if (citr->second == proto::common::SCENEOBJECT_USER)
			{
				msg_to_gate.add_role_list(citr->first);
			}
			else
			{
				object_base_ptr p_near_object = object_manager_t::find_object(*citr);
				if (p_near_object != NULL)
				{
					p_near_object->other_object_out_of_vision(object_id);
				}
			}
		}
	}

	if (msg_to_gate.role_list_size() > 0)
	{
		proto::client::gc_role_outo_vision_notify* p_notify_msg_nearby = msg_to_gate.mutable_msg();
		if (p_notify_msg_nearby != NULL)
		{
			proto::common::object_common_info* obj_self = p_notify_msg_nearby->add_common_list();
			if (obj_self != NULL)
			{
				obj_self->set_object_id(string_util_t::uint64_to_string(object_id.first));
				obj_self->set_object_type(object_id.second);
			}
		}

		env::server->send_msg_to_all_gate(op_cmd::ga_role_out_vision_notify, 0, msg_to_gate);
	}

	if (ntf_to_role.common_list_size() > 0)
		p_role->send_msg_to_client(op_cmd::gc_role_outo_vision_notify, ntf_to_role);

	return true;
}

void scene_t::notify_nearbies_move_s(object_base_ptr p_object, const nine_vector& nines)
{
	if (NULL == p_object)
	{
		log_error("error null p_object");
		return;
	}

	proto::server::ga_role_move_notify msg_to_gate;

	for (nine_vector::const_iterator ncitr = nines.begin();
		ncitr != nines.end(); ++ncitr)
	{	
		scene_grid* p_grid = m_scene_objects[*ncitr];
		if (p_grid == NULL || p_grid->empty())
			continue;

		for (scene_grid::const_iterator citr = p_grid->begin();
			citr != p_grid->end(); ++citr)
		{
			if (citr->second != proto::common::SCENEOBJECT_USER)
				continue;

			msg_to_gate.add_role_list(citr->first);
		}
	}

	// 给gate转发
	if (msg_to_gate.role_list_size() > 0)
	{
		p_object->get_object_pos_info(msg_to_gate.mutable_msg());

		env::server->send_msg_to_all_gate(op_cmd::ga_role_move_notify, 0, msg_to_gate);
	}
}

bool scene_t::init_object(monster_ptr p_monster, const monster_common::monster_define_t& define)
{
	if (NULL == p_monster)
	{
		log_error("monster null error");
		return false;
	}

	if (!monster_manager_t::add_monster(p_monster))
	{
		log_error("monster[%lu] add monster_manager error", p_monster->get_object_id());
		return false;
	}

	p_monster->set_object_born();

	if (!on_enter_s(p_monster, define.pos_x, define.pos_y, define.dir))
	{
		log_error("monster[%lu] enter scene[%lu] map[%u] error", p_monster->get_object_id(), m_scene_id, m_map_tid);
		monster_manager_t::delete_monster(p_monster->get_object_id());
		return false;
	}

	//SCENE_LOG("scene[%lu] map[%u] add object[%u, %d]", m_scene_id, m_map_tid, p_monster->get_monster_id(), p_monster->get_monster_type());

	return true;
}

bool scene_t::revive_object(monster_ptr p_monster, float x, float y, float dir)
{
	if (NULL == p_monster)
	{
		log_error("monster null error");
		return false;
	}

	if (!on_enter_s(p_monster, x, y, dir))
	{
		log_error("monster[%lu] revive error, scene[%lu] map[%u]", p_monster->get_object_id(), m_scene_id, m_map_tid);
		return false;
	}

	return true;
}

bool scene_t::init_collect_point(collect_point_ptr p_collect_point, bool is_new)
{
	if (NULL == p_collect_point)
	{
		log_error("p_collect_point null error");
		return false;
	}

	if (is_new && !collect_manager_t::add_collect_point(p_collect_point))
	{
		log_error("collect_point[%lu] add collect_manager_t error", p_collect_point->get_object_id());
		return false;
	}

	p_collect_point->set_object_born();

	if (!on_enter_s(p_collect_point, p_collect_point->get_current_x(), p_collect_point->get_current_y()))
	{
		log_error("collect_point[%lu] enter scene[%lu] map[%u] error", p_collect_point->get_object_id(), m_scene_id, m_map_tid);
		collect_manager_t::delete_collect_point(p_collect_point->get_object_id());
		return false;
	}

	// SCENE_LOG("scene[%lu] map[%u] add collect_point[%lu, %d]", m_scene_id, m_map_tid, p_collect_point->get_object_id(), p_collect_point->get_tid());

	return true;
}

void scene_t::add_field_boss(uint64_t object_id)
{
	m_field_boss.insert(object_id);
}

void scene_t::del_field_boss(uint64_t object_id)
{
	m_field_boss.erase(object_id);
}

void scene_t::on_role_enter(role_ptr p_role)
{
	if (NULL == p_role) {
		log_error("p_role null error");
		return;
	}

	// pk模式修改 目前默认安全区
	p_role->change_pk_mode(proto::common::PK_PEACE);

	// 场景中有boss
	if (scene_have_boss()) {
		// 通知场景中boss信息
		proto::client::gc_field_boss_pos_info_notify notify;

		for (std::set<uint64_t>::const_iterator itr = m_field_boss.begin();
			itr != m_field_boss.end(); ++itr) {
			uint64_t boss_object_id = *itr;
			monster_ptr p_monster = monster_manager_t::find_monster(boss_object_id);
			if (NULL == p_monster) {
				log_error("can not find p_monster[%lu]", boss_object_id);
				continue;
			}

			if (!p_monster->is_field_boss()) {
				log_error("object is not field boss");
				continue;
			}

			proto::common::field_boss_pos_info* info = notify.add_infos();
			if (NULL != info) {
				info->set_pos_act(proto::common::FIELD_BOSS_POS_ADD);
				info->set_object_id(common::string_util_t::uint64_to_string(boss_object_id));
				info->set_monster_id(p_monster->get_monster_id());
				info->set_pos_x((uint32_t)p_monster->get_current_x());
				info->set_pos_y((uint32_t)p_monster->get_current_y());
			}
		}

		p_role->send_msg_to_client(op_cmd::gc_field_boss_pos_info_notify, notify);
	}
}

void scene_t::on_role_leave(role_ptr p_role)
{
	
}

collect_point_ptr scene_t::summon_collect_point(uint32_t tid, float x, float y)
{
	CollectTable* p_config = GET_CONF(CollectTable, tid);
	if (NULL == p_config)
	{
		log_error("scene::load_map_object not found collect_point [%u]", tid);
		return NULL;
	}

	collect_point_ptr p_collect_point(new collect_point_t(tid));
	if (NULL == p_collect_point)
	{
		log_error("collect_point null error");
		return NULL;
	}

	if (!p_collect_point->init_collect_point(x, y))
	{
		log_error("collect_point[%d] init error", tid);
		return NULL;
	}

	if (!init_collect_point(p_collect_point))
	{
		log_error("collect_point[%d] scene init error", tid);
		return NULL;
	}

	SCENE_LOG("scene[%lu] map[%u] summon collect_point[%u] pos[%f, %f]", m_scene_id, m_map_tid, tid, x, y);

	return p_collect_point;
}

void scene_t::on_revive_role(role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("role null error");
		return;
	}

	// 拉到复活点附近
	scene_manager_t::role_jump_point(p_role, get_revive_point());
}

void scene_t::on_other_kill_me(role_ptr p_role, object_base_ptr p_enemy)
{
	if (p_role == NULL)
	{
		log_error("p_role null error");
		return;
	}

	if (p_enemy == NULL)
	{
		log_error("p_enemy null error");
		return;
	}

	p_role->set_object_die(p_enemy->get_object_id());

	show_revive_ui(p_role);
}

void scene_t::show_revive_ui(role_ptr p_role)
{
	if (p_role == NULL)
	{
		log_error("p_role null error");
		return;
	}

	// 默认复活方式
	proto::client::gc_object_death_notify notify;
	notify.set_revive_type(proto::common::object_revive_normal);
	notify.set_revive_time(0);
	p_role->send_msg_to_client(op_cmd::gc_object_death_notify, notify);
}

void scene_t::on_login_die(role_ptr p_role)
{
	if (p_role == NULL)
	{
		log_error("p_role null error");
		return;
	}

	// 没死亡
	if (!p_role->is_die())
	{
		log_error("role[%lu] not die", p_role->get_uid());
		return;
	}

	// 已经超过了复活时间，自动给他复活
	if (common::time_util_t::now_time() > p_role->get_revive_time())
	{
		p_role->set_object_revive(0);
	}
	else
	{
		// 弹出复活界面
		show_revive_ui(p_role);
	}
}

uint32_t scene_t::get_city_id()
{
	map_ptr p_map = get_map_ptr();
	if (NULL != p_map)
	{
		return p_map->get_city_id();
	}

	return 0;
}

void scene_t::send_msg_to_nine(uint32_t nine_key, uint16_t cmd, const std::string& cmd_data)
{
	if (INVALID_INDEX == nine_key)
	{
		log_error("nine_key[%u] error", nine_key);
		return;
	}

	map_ptr p_map = get_map_ptr();
	if (NULL == p_map)
	{
		log_error("map[%u] null error", m_map_tid);
		return;
	}

	const nine_vector& nines = p_map->get_nine_keys(nine_key);

	proto::server::ga_broadcast_notify msg_to_gate;

	for (nine_vector::const_iterator ncitr = nines.begin();
		ncitr != nines.end(); ++ncitr)
	{
		scene_grid* p_grid = m_scene_objects[*ncitr];
		if (p_grid == NULL || p_grid->empty())
			continue;

		for (scene_grid::const_iterator citr = p_grid->begin();
			citr != p_grid->end(); ++citr)
		{
			if (citr->second == proto::common::SCENEOBJECT_USER)
			{
				msg_to_gate.add_role_list(citr->first);
			}
		}
	}

	// 给gate转发
	if (msg_to_gate.role_list_size() > 0)
	{
		msg_to_gate.set_cmd(cmd);
		msg_to_gate.set_data(cmd_data);

		env::server->send_msg_to_all_gate(op_cmd::ga_broadcast_notify, 0, msg_to_gate);
	}
}

sPos scene_t::get_scene_born_position(const role_ptr& p_role)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		return sPos();
	}

	map_ptr p_jump_map = get_map_ptr();
	if (NULL == p_jump_map)
	{
		log_error("role[%lu] can not find jump map[%u] when jump", p_role->get_uid(), get_map_tid());
		return sPos();
	}
	uint32_t born_id = p_jump_map->get_born_point(p_role->get_level());
	JumpTable* p_jump_info = GET_CONF(JumpTable, born_id);
	if (NULL == p_jump_info)
	{
		log_error("role[%lu] scene[%lu,%u] can not find jump id[%u]", p_role->get_uid(), m_scene_id, m_map_tid, born_id);
		return sPos();
	}

	return sPos(p_jump_info->to_position_x(), p_jump_info->to_position_y(), p_jump_info->to_angle());
}

void scene_t::clear_all_monster()
{
	std::vector<object_id_type> del_vec;

	for (uint32_t i = 0; i < m_grid_num; ++i)
	{
		if (NULL != m_scene_objects[i])
		{
			for (scene_grid::const_iterator citr = m_scene_objects[i]->begin();
				citr != m_scene_objects[i]->end(); ++citr)
			{
				const object_id_type& id_type = *citr;

				// 加入除了人以外的生物
				if (id_type.second == proto::common::SCENEOBJECT_MONSTER)
				{
					del_vec.push_back(id_type);
				}
			}
		}
	}

	for (const auto& id_type : del_vec)
	{
		on_leave_s(id_type);
		object_manager_t::delete_creature(id_type);
	}

	m_field_boss.clear();
}

bool scene_t::is_have_fight()
{
	for (const auto& uid : m_roles)
	{
		role_ptr p_role = role_manager_t::find_role(uid);
		if (p_role != NULL && p_role->get_fight_uid() != 0)
		{
			// 有人在战斗
			return true;
		}
	}

	return false;
}

void scene_t::set_all_combat_failed()
{
	std::set<uint64_t> fights;
	for (std::set<uint64_t>::const_iterator citr = m_roles.begin();
		citr != m_roles.end(); ++citr)
	{
		role_ptr p_role = role_manager_t::find_role(*citr);
		if (NULL != p_role && p_role->get_fight_uid() != 0)
		{
			fights.insert(p_role->get_fight_uid());
		}
	}

	// 所有战斗强制失败
	for (std::set<uint64_t>::const_iterator citr = fights.begin();
		citr != fights.end(); ++citr)
	{
		// 击杀场景内玩家
		combat_ptr p_combat = fight_manager_t::get_combat(*citr);
		if (p_combat != NULL)
		{
			p_combat->all_fail();
		}
	}
}

uint32_t scene_t::get_map_group_id()
{
	const map_ptr& p_map = get_map_ptr();
	if (NULL == p_map)
	{
		log_error("map[%u] null error", m_map_tid);
		return 0;
	}

	return p_map->get_map_group_id();
}

uint32_t scene_t::check_scene_troop()
{
	switch (m_scene_type)
	{
	case proto::server::em_scene_type_none:
		{
			log_error("scene[%lu] type[%u] error", m_scene_id, m_scene_type);
			return common::errcode_enum::notice_unknown;
		}
		break;
	case proto::server::em_scene_type_prison:
		{
			SCENE_LOG("scene[%lu] type[%u] error", m_scene_id, m_scene_type);
			return common::errcode_enum::user_troop_err_prison;
		}
		break;
	case proto::server::em_scene_type_arena:
		{
			SCENE_LOG("scene[%lu] type[%u] error", m_scene_id, m_scene_type);
			return common::errcode_enum::user_troop_err_arena;
		}
		break;
	case proto::server::em_scene_type_dugeon:
		{
			SCENE_LOG("scene[%lu] type[%u] error", m_scene_id, m_scene_type);
			return common::errcode_enum::user_troop_err_dungeon;
		}
		break;
	case proto::server::em_scene_type_family_war:
		{
			SCENE_LOG("scene[%lu] type[%u] error", m_scene_id, m_scene_type);
			return common::errcode_enum::user_troop_err_family_war_scene;
		}
		break;
	default:
		break;
	}

	return common::errcode_enum::error_ok;
}

void scene_t::set_diversion(bool is_diversion, uint32_t num)
{
	m_is_diversion = is_diversion;
	m_scene_max_num = num;
}

