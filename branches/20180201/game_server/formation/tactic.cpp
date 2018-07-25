#include "tactic.hpp"

tactic_t::tactic_t(uint32_t tactic_id)
	: m_tid(tactic_id)
{
	

}

void tactic_t::load_data(const proto::common::tactic_single& tactic_single)
{
	m_tid = tactic_single.tactic_id();
	m_level = tactic_single.tactic_level();
}

void tactic_t::peek_data(proto::common::tactic_single* p_tactic_single)
{
	if (NULL == p_tactic_single)
	{
		return;
	}
	p_tactic_single->set_tactic_id(m_tid);
	p_tactic_single->set_tactic_level(m_level);
}
