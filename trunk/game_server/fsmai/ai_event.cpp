#include "ai_event.hpp"
#include "log.hpp"

bool ai_event_t::init(const fsm::ai_event& node)
{
	m_type = (ai::ai_event)node.event_type;

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

bool ai_event_t::do_evnet(const monster_ptr& p_monster)
{
	if (NULL == p_monster)
	{
		log_error("p_monster null error");
		return false;
	}

	if (m_ai_action_vec.empty())
	{
		return false;
	}

	for (ai_action_vec::iterator itr = m_ai_action_vec.begin();
		itr != m_ai_action_vec.end(); ++itr)
	{
		if ((*itr) != NULL)
		{
			(*itr)->do_act(p_monster);
		}
	}

	return true;
}

