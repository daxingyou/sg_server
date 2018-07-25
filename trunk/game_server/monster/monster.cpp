#include "monster.hpp"
#include "config/config_manager.hpp"
#include "object/object_manager.hpp"
#include "monster_manager.hpp"
#include "role/role.hpp"
#include "fight/fight_manager.hpp"
#include "scene/scene.hpp"
#include "scene/scene_manager.hpp"
#include "dungeon/dungeon_scene.hpp"

monster_t::~monster_t()
{

}

/**
* \brief 怪物初始化
* object_id ！= 0 表示有其他服务器召唤的怪物
*/
bool monster_t::init_monster(const monster_common::monster_define_t& define, Monster* monster, uint64_t object_id /*= 0*/)
{
	if (monster == NULL)
	{
		log_error("monster null error");
		return false;
	}

	m_monster_tid = define.id;
	m_monster_type = (proto::common::EM_MONSTER_TYPE)monster->type();
	m_level = monster->level();
	m_flags = monster->flag();

	// 设置temp_id
	if (object_id != 0)
	{
		set_object_id(object_id);
	}
	else
	{
		set_object_id(monster_manager_t::get_monster_uid());
	}

	if (define.ai_id != 0)
	{
		boost::scoped_ptr<ai_controller_t> p_ai_controller(new ai_controller_t(get_monster(), define.ai_id));
		if (!p_ai_controller->load_ai(define, monster))
		{
			log_error("monster[%u] init ai[%u] failed", m_monster_tid, define.ai_id);
			return false;
		}

		m_ai_contrl.swap(p_ai_controller);

		monster_manager_t::add_special_monster(m_object_id);
	}

	return true;
}

void monster_t::other_object_into_vision(const object_id_type& object)
{
	if (object.second != proto::common::SCENEOBJECT_USER)
	{
		return;
	}

	if (m_flags & MF_ACTIVE_MODE)
	{
		m_vision_objects.insert(object);
	}
}

void monster_t::other_object_out_of_vision(const object_id_type& object)
{
	if (object.second != proto::common::SCENEOBJECT_USER)
	{
		return;
	}

	if (m_flags & MF_ACTIVE_MODE)
	{
		m_vision_objects.erase(object);
	}
}

void monster_t::on_enter_scene(scene_ptr p_scene)
{
	object_base_t::on_enter_scene(p_scene);
}

void monster_t::on_leave_scene(scene_ptr p_scene)
{
	object_base_t::on_leave_scene(p_scene);
}

void monster_t::get_object_info(proto::common::object_common_info* object_info)
{
	if (NULL == object_info)
	{
		log_error("object_info null error");
		return;
	}

	object_base_t::get_object_info(object_info);
	object_info->set_level(m_level);
	object_info->set_npc_tid(m_monster_tid);

	if (m_object_state == proto::common::object_state_alert)
	{
		proto::common::object_state_info* object_state = object_info->mutable_object_state();
		if (object_state != NULL)
		{
			object_state->set_obj_state_param(m_alert_name);
			object_state->set_alert_time(m_alert_start_time);
		}
	}


	//object_info->set_role_type(m_hero_tid);		// wsy todo 如果要加怪物主公ID就加载这
}

uint32_t monster_t::get_against_form_id()
{
	Monster* monster = GET_CONF(Monster, m_monster_tid);
	if (NULL != monster)
	{
		return monster->get_fight_id();
	}

	log_error("monster[%u] not find conf", m_monster_tid);
	return 0;
}

uint64_t monster_t::fight_with_role(role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		return 0;
	}

    if (!p_role->can_enter_fight())
    {
        log_error("role already in fight %lu", p_role->get_uid());
        return 0;
    }

	uint32_t against_form_id = get_against_form_id();
	if (against_form_id == 0)
	{
		log_warn("monster_tid[%u] against_form_id == 0", m_monster_tid);
		return 0;
	}

	proto::common::fight_param fp;
	fp.set_type(proto::common::fight_type_light_monster);
	fp.mutable_light()->set_monster_id(get_monster_id());
	fp.mutable_light()->set_object_id(common::string_util_t::uint64_to_string(m_object_id));

	uint64_t fight_id = fight_manager_t::fight_pve(p_role, against_form_id, &fp);
	if (fight_id == 0)
	{
		log_error("role[%lu] fight_pve monster[%u] error", p_role->get_uid(), get_monster_id());
		return 0;
	}

	// 设置进入战斗
	enter_fight(p_role->get_uid(), fight_id);

	return fight_id;
}

void monster_t::notify_object_common_info_for_nearbies()
{
	proto::client::gc_scene_object_info_notify ntf;
	get_object_info(ntf.mutable_common_info());
	send_msg_to_nine(op_cmd::gc_scene_object_info_notify, ntf);
}

void monster_t::set_object_state(proto::common::object_state_type newstate)
{
	// 相同状态不用变
	if (newstate == m_object_state)
	{
		return;
	}

	object_base_t::set_object_state(newstate);

	if (m_ai_contrl)
	{
		m_ai_contrl->change_state(newstate);
	}

	if (m_is_in_scene)
	{
		notify_object_common_info_for_nearbies();
	}
}

void monster_t::enter_fight(uint64_t uid, uint64_t fight_id)
{
	// 独占的话显示正在战斗状态
	if (m_flags & MF_EXCLUSIVE_MODE)
	{
		m_fight_uid = fight_id;
		m_fight_role = uid;
	}

	// ai 不处理进入战斗事件按默认处理方式
	if (!m_ai_contrl || !m_ai_contrl->on_event(ai::ai_event_enter_fight))
	{
		if (m_flags & MF_EXCLUSIVE_MODE)
		{
			set_object_state(proto::common::object_state_fighting);
		}
		else
		{
			set_object_state(proto::common::object_state_normal);
		}
	}
}

void monster_t::leave_fight(bool is_win, uint32_t fight_time, uint32_t death_value)
{
	if (m_flags & MF_EXCLUSIVE_MODE)
	{
		m_fight_uid = 0;
		m_fight_role = 0;
	}

	ai::ai_event evt = is_win ? ai::ai_event_win_fight : ai::ai_event_killed;

	if (!m_ai_contrl || !m_ai_contrl->on_event(evt))
	{
		set_object_state(proto::common::object_state_normal);
	}

	// 在副本中的特殊处理
	if (is_in_dungeon())
	{
		const scene_ptr& p_scene = scene_manager_t::find_scene(m_scene_id);
		if (p_scene == NULL)
		{
			log_error("monster[%lu] scene[%lu] not find", m_object_id, m_scene_id);
			return;
		}

		dungeon_scene_ptr p_dungeon_scene = p_scene->get_dungeon_scene_ptr();
		if (p_dungeon_scene == NULL)
		{
			log_error("monster[%lu] scene[%lu] is not dungeon scene", m_object_id, m_scene_id);
			return;
		}

		p_dungeon_scene->on_monster_fight_result(get_monster(), is_win, fight_time, death_value);
	}
}

void monster_t::on_object_die()
{
	object_base_t::on_object_die();

	m_death_time = common::time_util_t::now_time();
}

void monster_t::on_tick(uint64_t msecs)
{
	if (m_ai_contrl)
	{
		m_ai_contrl->on_action(msecs);
	}
}

bool monster_t::alert_round(uint32_t radius)
{
	if (m_vision_objects.empty())
	{
		return false;
	}

	uint32_t min_distance = 0;
	uint32_t radius2 = radius * radius;

	for (std::set<object_id_type>::const_iterator citr = m_vision_objects.begin();
		citr != m_vision_objects.end(); ++citr)
	{
		role_ptr p_role = role_manager_t::find_role(citr->first);
		if (NULL != p_role)
		{
			if (!p_role->can_enter_fight(false))
			{
				continue;
			}

			if (fabs(p_role->get_current_x() - m_current_x) > radius)
			{
				continue;
			}

			if (fabs(p_role->get_current_y() - m_current_y) > radius)
			{
				continue;
			}

			uint32_t distance2 = ((int32_t)(p_role->get_current_x() - m_current_x) * (int32_t)(p_role->get_current_x() - m_current_x)) +
				((int32_t)(p_role->get_current_y() - m_current_y) * (int32_t)(p_role->get_current_y() - m_current_y));

			if (distance2 <= radius2)
			{
				if (min_distance == 0 || distance2 < min_distance)
				{
					min_distance = distance2;

					// 设置警戒目标
					m_alert_uid = citr->first;
					m_alert_name = p_role->get_name();
				}
			}
		}
	}

	return m_alert_uid != 0;
}

void monster_t::clear_alert()
{
	m_alert_start_time = 0;
	m_alert_uid = 0;
	m_alert_name.clear();
}

void monster_t::start_alert()
{
	if (m_alert_uid)
	{
		m_alert_start_time = common::time_util_t::now_time();

		set_object_state(proto::common::object_state_alert);
	}
}

bool monster_t::check_alert(uint32_t radius)
{
	role_ptr p_role = role_manager_t::find_role(m_alert_uid);
	// 下线了可能
	if (NULL == p_role)
	{
		return false;
	}

	// 不在同一个场景
	if (p_role->get_scene_id() != get_scene_id())
	{
		return false;
	}

	uint32_t distance2 = ((int32_t)(p_role->get_current_x() - m_current_x) * (int32_t)(p_role->get_current_x() - m_current_x)) +
		((int32_t)(p_role->get_current_y() - m_current_y) * (int32_t)(p_role->get_current_y() - m_current_y));

	// 距离超出
	if (distance2 > radius * radius)
	{
		return false;
	}

	// 不能进入战斗了
	if (!p_role->can_enter_fight(false))
	{
		return false;
	}

	return true;
}

bool monster_t::fight_with_alert_role()
{
	role_ptr p_role = role_manager_t::find_role(m_alert_uid);
	// 下线了可能
	if (NULL == p_role)
	{
		log_error("role[%lu] not find", m_alert_uid);
		return false;
	}

	// 不在同一个场景
	if (p_role->get_scene_id() != get_scene_id())
	{
		log_error("role[%lu] not in same scene", m_alert_uid);
		return false;
	}

	// 不能进入战斗了
	if (!p_role->can_enter_fight(false))
	{
		log_error("role[%lu] can not enter fight", m_alert_uid);
		return false;
	}

	if (fight_with_role(p_role) == 0)
	{
		log_error("role[%lu] fight monser fail", m_alert_uid);
		return false;
	}

	return true;
}

void monster_t::set_normal_state()
{
	set_object_state(proto::common::object_state_normal);
}

void monster_t::on_object_revive()
{
	object_base_t::on_object_revive();

	if (!m_is_in_scene)
	{
		const scene_ptr& p_scene = scene_manager_t::find_scene(m_scene_id);
		if (p_scene != NULL)
		{
			// 原地复活
			p_scene->revive_object(get_monster(), m_current_x, m_current_y, m_direction);
		}
	}
}

