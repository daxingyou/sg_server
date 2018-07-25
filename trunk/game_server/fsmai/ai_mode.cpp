#include "ai_mode.hpp"
#include "log.hpp"
#include "monster/monster.hpp"
#include "config/config_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "config_mgr.h"

ai_mode_ptr ai_mode_t::create_ai_mode(ai::ai_mode type)
{
	switch (type)
	{
	case ai::ai_mode_time_disappear:
		{
			ai_mode_ptr p_mode(new mode_time_disappear());
			return p_mode;
		}
		break;
	case ai::ai_mode_time_revive:
		{
			ai_mode_ptr p_mode(new mode_time_revive());
			return p_mode;
		}
		break;
	case ai::ai_mode_trigger_alert:
		{
			ai_mode_ptr p_mode(new mode_trigger_alert());
			return p_mode;
		}
		break;
	case ai::ai_mode_timer_alert:
		{
			ai_mode_ptr p_mode(new mode_time_alert());
			return p_mode;
		}
		break;
	default:
		{
			log_error("ai_mode type[%u] not find", type);
		}
		break;
	}

	return NULL;
}

bool ai_mode_t::init(const fsm::ai_mode& node, const monster_common::monster_define_t& define, Monster* monster)
{
	m_type = (ai::ai_mode)node.mode_type;

	for (uint32_t i = 0; i < node.acts.size(); ++i)
	{
		const fsm::ai_act& act = node.acts[i];
		ai_action_ptr p_act = ai_action_t::create_ai_action((ai::ai_act)act.act_type);
		if (p_act != NULL && p_act->init(act))
		{
			m_ai_action_vec.push_back(p_act);
		}
	}

	return true;
}

void ai_mode_t::exe_action(const monster_ptr& p_monster)
{
	if (NULL == p_monster)
	{
		log_error("p_monster null error");
		return;
	}

	if (m_ai_action_vec.empty())
		return;

	for (ai_action_vec::iterator itr = m_ai_action_vec.begin();
		itr != m_ai_action_vec.end(); ++itr)
	{
		if ((*itr) != NULL)
		{
			(*itr)->do_act(p_monster);
		}
	}
}

bool mode_time_disappear::init(const fsm::ai_mode& node, const monster_common::monster_define_t& define, Monster* monster)
{
	if (monster == NULL)
	{
		log_error("monster table null error");
		return false;
	}

	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::dispear_monsterbody_time);
	m_disappear_time = GET_COMPREHENSIVE_VALUE_1(p_conf);

	return ai_mode_t::init(node, define, monster);
}

void mode_time_disappear::start_up(const monster_ptr& p_monster)
{
	m_valid = true;
}

void mode_time_disappear::do_mode(uint64_t msecs, const monster_ptr& p_monster)
{
	if (p_monster == NULL)
	{
		log_error("p_monster null error");
		return;
	}

	uint32_t sec = msecs / 1000L;
	if (sec >= p_monster->get_death_time() + m_disappear_time)
	{
		exe_action(p_monster);
		m_valid = false;
	}
}

bool mode_time_revive::init(const fsm::ai_mode& node, const monster_common::monster_define_t& define, Monster* monster)
{
	if (monster == NULL)
	{
		log_error("monster table null error");
		return false;
	}

	m_revive_interval = define.interval;

	return ai_mode_t::init(node, define, monster);
}

void mode_time_revive::start_up(const monster_ptr& p_monster)
{
	m_valid = true;
}

void mode_time_revive::do_mode(uint64_t msecs, const monster_ptr& p_monster)
{
	if (p_monster == NULL)
	{
		log_error("p_monster null error");
		return;
	}

	uint32_t sec = msecs / 1000L;
	if (sec >= p_monster->get_death_time() + m_revive_interval)
	{
		exe_action(p_monster);
		m_valid = false;
	}
}

bool mode_trigger_alert::init(const fsm::ai_mode& node, const monster_common::monster_define_t& define, Monster* monster)
{
	if (monster == NULL)
	{
		log_error("monster table null error");
		return false;
	}

	m_alert_radius = monster->alert_radius();

	return ai_mode_t::init(node, define, monster);
}

void mode_trigger_alert::start_up(const monster_ptr& p_monster)
{
	if (p_monster == NULL)
	{
		log_error("p_monster null error");
		return;
	}

	p_monster->clear_alert();
}

void mode_trigger_alert::do_mode(uint64_t msecs, const monster_ptr& p_monster)
{
	if (p_monster == NULL)
	{
		log_error("p_monster null error");
		return;
	}

	if (p_monster->alert_round(m_alert_radius))
	{
		exe_action(p_monster);
	}
}

bool mode_time_alert::init(const fsm::ai_mode& node, const monster_common::monster_define_t& define, Monster* monster)
{
	if (monster == NULL)
	{
		log_error("monster table null error");
		return false;
	}

	m_alert_radius = monster->alert_radius();
	m_alert_interval = monster->alert_time();

	return ai_mode_t::init(node, define, monster);
}

void mode_time_alert::start_up(const monster_ptr& p_monster)
{
	if (p_monster == NULL)
	{
		log_error("p_monster null error");
		return;
	}

	m_valid = true;

	m_alert_end_time = p_monster->get_start_alert_time() + m_alert_interval;
}

void mode_time_alert::do_mode(uint64_t msecs, const monster_ptr& p_monster)
{
	if (p_monster == NULL)
	{
		log_error("p_monster null error");
		return;
	}

	// ¾¯½äÊ§°Ü
	if (!p_monster->check_alert(m_alert_radius))
	{
		p_monster->set_normal_state();
		return;
	}

	uint32_t sec = msecs / 1000L;
	if (m_alert_end_time < sec)
	{
		exe_action(p_monster);
	}
}
