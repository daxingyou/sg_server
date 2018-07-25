#include "monster_ai.hpp"
#include "monster.hpp"
#include "fsmai/fsm_xml_manager.hpp"

ai_controller_t::ai_controller_t(const monster_ptr& p_monster, uint32_t id)
	: m_monster_wptr(p_monster)
	, m_ai_id(id)
{

}

bool ai_controller_t::load_ai(const monster_common::monster_define_t& define, Monster* monster)
{
	if (monster == NULL)
	{
		log_error("monster table null error");
		return false;
	}

	const ai_state_conf_ptr& p_ai_state = fsm_xml_manager_t::get_fsm_ai_state(m_ai_id);
	if (p_ai_state == NULL)
	{
		log_error("ai[%u] not find", m_ai_id);
		return false;
	}

	boost::scoped_ptr<ai_state_t> p_root_state(new ai_state_t());
	if (!p_root_state->init(state_type_enum::state_type_root, p_ai_state->m_states, define, monster))
	{
		log_error("ai[%u] state init error", m_ai_id);
		return false;
	}

	m_rootstate.swap(p_root_state);

	return true;
}

void ai_controller_t::reset()
{
	if (m_rootstate)
	{
		m_rootstate->reset();
	}
}

bool ai_controller_t::on_event(ai::ai_event evt)
{
	if (m_rootstate)
	{
		return m_rootstate->on_event(get_monster_ptr(), evt);
	}

	return false;
}
	
void ai_controller_t::change_state(proto::common::object_state_type newstate)
{
	if (m_rootstate)
	{
		m_rootstate->change_state(get_monster_ptr(), newstate);
	}
}

void ai_controller_t::on_action(uint64_t msecs)
{
	if (m_rootstate)
	{
		m_rootstate->on_action(msecs, get_monster_ptr());
	}
}
