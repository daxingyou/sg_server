#include "ai_state.hpp"
#include "log.hpp"
#include "monster/monster.hpp"

bool ai_state_t::init(state_type_enum type, const fsm::ai_state& node, const monster_common::monster_define_t& define, Monster* monster)
{
	m_state = node.state_type;
	m_state_type = type;

	for (uint32_t i = 0; i < node.events.size(); ++i)
	{
		const fsm::ai_event& event = node.events[i];
		ai_event_ptr p_event(new ai_event_t());
		if (p_event != NULL && p_event->init(event))
		{
			m_ai_event_map.insert(std::make_pair((ai::ai_event)event.event_type, p_event));
		}
	}

	for (uint32_t i = 0; i < node.modes.size(); ++i)
	{
		const fsm::ai_mode& mode = node.modes[i];
		ai_mode_ptr p_mode = ai_mode_t::create_ai_mode((ai::ai_mode)mode.mode_type);
		if (p_mode != NULL && p_mode->init(mode, define, monster))
		{
			m_ai_mode_vec.push_back(p_mode);
		}
	}

	for (uint32_t i = 0; i < node.children.size(); ++i)
	{
		const fsm::ai_state& state = node.children[i];
		ai_state_ptr p_state(new ai_state_t());
		if (p_state != NULL && p_state->init(state_type_enum::state_type_child, state, define, monster))
		{
			m_ai_state_map.insert(std::make_pair((proto::common::object_state_type)state.state_type, p_state));
		}
	}

	return true;
}

void ai_state_t::reset()
{
	if (m_state_type == state_type_root)
	{
		m_state = proto::common::object_state_none;
		m_curr_state = NULL;
	}

	for (ai_mode_vec::iterator itr = m_ai_mode_vec.begin();
		itr != m_ai_mode_vec.end(); ++itr)
	{
		if ((*itr) != NULL)
		{
			(*itr)->reset();
		}
	}

	for (ai_state_map::iterator itr = m_ai_state_map.begin();
		itr != m_ai_state_map.end(); ++itr)
	{
		if (itr->second != NULL)
		{
			itr->second->reset();
		}
	}
}

void ai_state_t::change_state(const monster_ptr& p_monster, proto::common::object_state_type newstate)
{
	if (p_monster == NULL)
	{
		log_error("p_monster null error");
		return;
	}

	if (m_curr_state != NULL)
	{
		m_curr_state->on_leave();
	}

	proto::common::object_state_type old_state = m_state;
	m_state = newstate;

	m_curr_state = get_ai_state(newstate);

	if (m_curr_state != NULL)
	{
		m_curr_state->on_enetr(p_monster, old_state);
	}
}

void ai_state_t::on_enetr(const monster_ptr& p_monster, proto::common::object_state_type old /*= proto::common::object_state_none*/)
{
	for (ai_mode_vec::iterator itr = m_ai_mode_vec.begin();
		itr != m_ai_mode_vec.end(); ++itr)
	{
		if ((*itr) != NULL)
		{
			(*itr)->start_up(p_monster);
		}
	}
}

void ai_state_t::on_leave()
{

}

void ai_state_t::on_action(uint64_t msecs, const monster_ptr& p_monster)
{
	if (p_monster == NULL)
	{
		log_error("p_monster null error");
		return;
	}

	if (!m_ai_mode_vec.empty())
	{
		for (ai_mode_vec::iterator itr = m_ai_mode_vec.begin();
			itr != m_ai_mode_vec.end(); ++itr)
		{
			if ((*itr) != NULL && (*itr)->is_valid())
			{
				(*itr)->do_mode(msecs, p_monster);
			}
		}
	}

	if (m_curr_state != NULL)
	{
		m_curr_state->on_action(msecs, p_monster);
	}
}

bool ai_state_t::on_event(const monster_ptr& p_monster, ai::ai_event event)
{
	if (p_monster == NULL)
	{
		log_error("p_monster null error");
		return false;
	}

	const ai_event_ptr& p_event = get_ai_event(event);
	if (NULL != p_event)
	{
		return p_event->do_evnet(p_monster);
	}

	if (m_curr_state != NULL)
	{
		return m_curr_state->on_event(p_monster, event);
	}

	return false;
}

ai_event_ptr ai_state_t::get_ai_event(ai::ai_event event_id)
{
	ai_event_map::const_iterator citr = m_ai_event_map.find(event_id);
	if (citr != m_ai_event_map.end())
	{
		return citr->second;
	}

	return ai_event_ptr();
}

ai_state_ptr ai_state_t::get_ai_state(proto::common::object_state_type state)
{
	ai_state_map::const_iterator citr = m_ai_state_map.find(state);
	if (citr != m_ai_state_map.end())
	{
		return citr->second;
	}

	return ai_state_ptr();
}
