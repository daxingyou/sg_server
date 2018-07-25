#include "dungeon_state.hpp"

#include "config/config_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "config_mgr.h"
#include "dungeon/dungeon_scene.hpp"
#include <boost/assign/std/map.hpp>

bool dungeon_state_controler::init(const dungeon_scene_ptr& p_scene, proto::common::dungeon_state_type init_state)
{
	if (p_scene == NULL)
	{
		log_error("p_scene null error");
		return false;
	}

	m_dungeon_scene = p_scene;
	
	boost::assign::insert(m_dungeon_state_map)
		(proto::common::dungeon_state_none, dungeon_state_ptr(new dungeon_state_none_t()))
		(proto::common::dungeon_state_wait, dungeon_state_ptr(new dungeon_state_wait_t()))
		(proto::common::dungeon_state_start, dungeon_state_ptr(new dungeon_state_start_t()))
		(proto::common::dungeon_state_stop, dungeon_state_ptr(new dungeon_state_stop_t()));

	change_state(init_state);

	return true;
}

void dungeon_state_controler::action(uint32_t curr_time)
{
	if (NULL == m_curr_state)
		return;

	m_curr_state->handle(*this, curr_time);
}

void dungeon_state_controler::change_state(proto::common::dungeon_state_type new_state)
{
	dungeon_state_map::iterator itr = m_dungeon_state_map.find(new_state);
	if (itr == m_dungeon_state_map.end())
	{
		log_error("new_state[%u] state null error", new_state);
		return;
	}

	if (itr->second == NULL)
	{
		log_error("itr->second null error");
		return;
	}

	m_curr_state = itr->second;
	m_curr_state->on_enter(*this);
}

void dungeon_state_wait_t::on_enter(dungeon_state_controler& controler)
{
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::dungeon_wait_time);
	uint32_t wait_time = GET_COMPREHENSIVE_VALUE_1(p_conf);
	m_idle_time = wait_time + common::time_util_t::now_time();
}

void dungeon_state_wait_t::handle(dungeon_state_controler& controler, uint32_t curr_time)
{
	dungeon_scene_ptr p_scene = controler.get_scene();
	if (NULL == p_scene)
	{
		log_error("p_scene null error");
		return;
	}

	// 到达最大等待时间，进入销毁流程
	if (m_idle_time <= curr_time)
	{
		DUNGEON_LOG("dungeon[%u] is already arrive max idle time when wait state", p_scene->get_dungeon_id());
		
		controler.change_state(proto::common::dungeon_state_stop);
	}
}

void dungeon_state_start_t::handle(dungeon_state_controler& controler, uint32_t curr_time)
{
	dungeon_scene_ptr p_scene = controler.get_scene();
	if (NULL == p_scene)
	{
		log_error("p_scene null error");
		return;
	}

	if (p_scene->reach_max_time(curr_time)) // 到达最大上线无论什么情况，都直接进入删除流程
	{
		controler.change_state(proto::common::dungeon_state_stop);
	}
	else if (p_scene->reach_end_time(curr_time))
	{
		// 有人在战斗等人结束战斗在结算
		if (p_scene->is_have_fight())
		{
			return;
		}

		p_scene->failed_time_long();
	}
	else if (p_scene->reach_idle_time(curr_time))
	{
		// 空闲时间到了直接进入回收
		controler.change_state(proto::common::dungeon_state_stop);
	}
	else
	{
		p_scene->check_daley_summon(curr_time);
	}
}

void dungeon_state_stop_t::on_enter(dungeon_state_controler& controler)
{
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::dungeon_wait_time);
	uint32_t wait_time = GET_COMPREHENSIVE_VALUE_1(p_conf);
	m_left_time = wait_time + common::time_util_t::now_time();
}

void dungeon_state_stop_t::handle(dungeon_state_controler& controler, uint32_t curr_time)
{
	dungeon_scene_ptr p_scene = controler.get_scene();
	if (NULL == p_scene)
	{
		log_error("p_scene null error");
		return;
	}

	if (0 == p_scene->get_role_size())
	{
		controler.change_state(proto::common::dungeon_state_none);

		// 卸载场景
		p_scene->unload();

		DUNGEON_LOG("dungeon[%u] unload success", p_scene->get_dungeon_id());

		return;
	}

	// 有玩家在，则走清理等待流程
	if (curr_time < m_left_time)
	{
		return;
	}

	if (!p_scene->is_have_fight())
	{
		// 强制所有人都离开
		p_scene->forced_all_leave();

		controler.change_state(proto::common::dungeon_state_none);

		// 卸载场景
		p_scene->unload();

		DUNGEON_LOG("dungeon[%u] unload success", p_scene->get_dungeon_id());

		return;
	}
	else
	{
		// 设置所有战斗失败
		p_scene->set_all_combat_failed();
	}
}
