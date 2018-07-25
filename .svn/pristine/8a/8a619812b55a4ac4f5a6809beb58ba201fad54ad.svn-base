#include "bounty.hpp"
#include "tblh/Bounty.tbls.h"
#include "config_mgr.h"

bounty_t::bounty_t(uint32_t bounty_id)
	: m_tid(bounty_id)
{
	

}

void bounty_t::load_data(const proto::common::bounty_single& bounty_single)
{
	m_tid = bounty_single.bounty_id();
	m_state = bounty_single.state();
	auto p_bounty_conf = GET_CONF(Bounty, m_tid);
	if (NULL != p_bounty_conf)
	{
		m_star = p_bounty_conf->star();
	}
}

void bounty_t::peek_data(proto::common::bounty_single* p_bounty_single)
{
	if (NULL == p_bounty_single)
	{
		return;
	}
	p_bounty_single->set_bounty_id(m_tid);
	p_bounty_single->set_state(m_state);
	p_bounty_single->set_star(m_star);
}
