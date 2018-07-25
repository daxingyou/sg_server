#include "scene_state.hpp"

#include "config/config_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "config_mgr.h"

scene_state_ptr scene_state_controler::create_scene_state(SceneActiveState type)
{
	switch (type)
	{
	case SceneActiveState::None_State:
		{
			scene_state_ptr p_state(new scene_state_none_t());
			return p_state;
		}
		break;
	case SceneActiveState::Run_State:
		{
			scene_state_ptr p_state(new scene_state_run_t());
			return p_state;
		}
		break;
	case SceneActiveState::Stop_State:
		{
			scene_state_ptr p_state(new scene_state_stop_t());
			return p_state;
		}
		break;
	case SceneActiveState::End_State:
		{
			scene_state_ptr p_state(new scene_state_end_t());
			return p_state;
		}
		break;
	default:
		log_error("scene state[%u] not define", type);
		break;
	}

	return NULL;
}

bool scene_state_controler::init(const scene_ptr& p_scene, SceneActiveState init_state)
{
	if (p_scene == NULL)
	{
		log_error("p_scene null error");
		return false;
	}

	m_scene_wptr = p_scene;

	change_state(init_state);

	return true;
}

void scene_state_controler::action(uint32_t curr_time)
{
	if (NULL == m_curr_state)
		return;

	m_curr_state->handle(*this, curr_time);
}

void scene_state_controler::change_state(SceneActiveState new_state)
{
	scene_state_map::iterator itr = m_scene_state_map.find(new_state);
	if (itr == m_scene_state_map.end())
	{
		scene_state_ptr p_new_state = create_scene_state(new_state);
		if (NULL == p_new_state)
		{
			log_error("scene state change state[%u] error", new_state);
			return;
		}

		std::pair<scene_state_map::iterator, bool> ret = m_scene_state_map.insert(std::make_pair(new_state, p_new_state));
		if (ret.second == false) {
			log_error("scene state change state[%u] instert error", new_state);
			return;
		}

		itr = ret.first;
	}

	if (itr->second == NULL)
	{
		log_error("scene_state_controler itr->second null error");
		return;
	}

	m_curr_state = itr->second;
	m_curr_state->on_enter(*this);
}

void scene_state_run_t::handle(scene_state_controler& controler, uint32_t curr_time)
{
	scene_ptr p_scene = controler.get_scene();
	if (NULL == p_scene)
	{
		log_error("p_scene null error");
		return;
	}

	if (p_scene->reach_max_time(curr_time)) // 到达最大上线无论什么情况，都直接进入删除流程
	{
		controler.change_state(SceneActiveState::End_State);
	}
	else if (p_scene->reach_end_time(curr_time))
	{
		controler.change_state(SceneActiveState::Stop_State);
	}
}

void scene_state_stop_t::on_enter(scene_state_controler& controler)
{
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::dungeon_wait_time);
	uint32_t wait_time = GET_COMPREHENSIVE_VALUE_1(p_conf);
	m_left_time = wait_time + common::time_util_t::now_time();
}

void scene_state_stop_t::handle(scene_state_controler& controler, uint32_t curr_time)
{
	scene_ptr p_scene = controler.get_scene();
	if (NULL == p_scene) {
		log_error("p_scene null error");
		return;
	}

	if (p_scene->reach_max_time(curr_time)) { // 到达最大上线无论什么情况，都直接进入删除流程
		controler.change_state(SceneActiveState::End_State);
	}
	else if (0 == p_scene->get_role_size()) {
		controler.change_state(SceneActiveState::End_State);
	}
	else if (curr_time >= m_left_time) {
		controler.change_state(SceneActiveState::End_State);
	}
}

void scene_state_end_t::handle(scene_state_controler& controler, uint32_t curr_time)
{
	scene_ptr p_scene = controler.get_scene();
	if (NULL == p_scene)
	{
		log_error("p_scene null error");
		return;
	}

	if (0 == p_scene->get_role_size()) 
	{
		controler.change_state(SceneActiveState::None_State);

		// 卸载场景
		p_scene->unload();
		
		return;
	}

	if (!p_scene->is_have_fight())
	{
		// 强制所有人都离开
		p_scene->forced_all_leave();

		controler.change_state(SceneActiveState::None_State);

		// 卸载场景
		p_scene->unload();

		return;
	}
	else
	{
		// 设置所有战斗失败
		p_scene->set_all_combat_failed();
	}
}
