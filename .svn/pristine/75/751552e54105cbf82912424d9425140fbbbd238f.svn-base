#include "ai_action.hpp"

#include "log.hpp"
#include "monster/monster.hpp"
#include "fsm_xml_manager.hpp"

ai_action_ptr ai_action_t::create_ai_action(ai::ai_act type)
{
	switch (type)
	{
	case ai::ai_act_death:
		{
			ai_action_ptr p_act(new act_death());
			return p_act;
		}
		break;
	case ai::ai_act_disappear:
		{
			ai_action_ptr p_act(new act_leave_scene());
			return p_act;
		}
		break;
	case ai::ai_act_delete:
		{
			ai_action_ptr p_act(new act_delete_obj());
			return p_act;
		}
		break;
	case ai::ai_act_revive:
		{
			ai_action_ptr p_act(new act_revive());
			return p_act;
		}
		break;
	case ai::ai_act_start_alert:
		{
			ai_action_ptr p_act(new act_start_alert());
			return p_act;
		}
		break;
	case ai::ai_act_start_fight:
		{
			ai_action_ptr p_act(new act_start_fight());
			return p_act;
		}
		break;
	case ai::ai_act_change_state:
		{
			ai_action_ptr p_act(new act_change_state());
			return p_act;
		}
		break;
	default:
		{
			log_error("ai_action type[%u] not find", type);
		}
		break;
	}

	return NULL;
}

bool ai_action_t::init(const fsm::ai_act& node)
{
	m_type = (ai::ai_act)node.act_type;

	return true;
}

bool act_death::do_act(const monster_ptr& p_monster)
{
	if (NULL == p_monster)
	{
		log_error("p_monster null error");
		return false;
	}

	if (!p_monster->is_die())
		p_monster->set_object_die();

	return true;
}

bool act_leave_scene::do_act(const monster_ptr& p_monster)
{
	if (NULL == p_monster)
	{
		log_error("p_monster null error");
		return false;
	}

	p_monster->leave_scene();

	return true;
}

bool act_delete_obj::do_act(const monster_ptr& p_monster)
{
	if (NULL == p_monster)
	{
		log_error("p_monster null error");
		return false;
	}

	p_monster->set_need_clear();

	return true;
}

bool act_revive::do_act(const monster_ptr& p_monster)
{
	if (NULL == p_monster)
	{
		log_error("p_monster null error");
		return false;
	}

	if (p_monster->is_die())
	{
		p_monster->set_object_revive();
	}

	return true;
}

bool act_start_alert::do_act(const monster_ptr& p_monster)
{
	if (NULL == p_monster)
	{
		log_error("p_monster null error");
		return false;
	}

	p_monster->start_alert();
	
	return true;
}

bool act_start_fight::do_act(const monster_ptr& p_monster)
{
	if (NULL == p_monster)
	{
		log_error("p_monster null error");
		return false;
	}

	if (!p_monster->fight_with_alert_role())
	{
		p_monster->set_normal_state();
	}
	
	return true;
}

bool act_change_state::init(const fsm::ai_act& node)
{
	ai_action_t::init(node);

	m_new_state = fsm_xml_manager_t::get_state_type(node.act_param);

	return true;
}

bool act_change_state::do_act(const monster_ptr& p_monster)
{
	if (NULL == p_monster)
	{
		log_error("p_monster null error");
		return false;
	}

	p_monster->set_object_state(m_new_state);

	return true;
}
