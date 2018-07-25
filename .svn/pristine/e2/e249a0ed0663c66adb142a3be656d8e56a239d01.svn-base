#include "tactic_manager.hpp"
#include "tblh/Tactic.tbls.h"
#include "tblh/Comprehensive.tbls.h"
#include "config_mgr.h"
#include "tblh/errcode_enum.hpp"
#include "role/role_manager.hpp"

USING_NS_COMMON;

tactic_manager_t::tactic_manager_t(uint64_t role_uid)
	: m_role_uid(role_uid)
{
	
}
void tactic_manager_t::load_data(const proto::common::tactic_data& tactic_data)
{
	for (int i = 0; i < tactic_data.tactics_size(); ++i)
	{
		const proto::common::tactic_single& ts = tactic_data.tactics(i);
		tactic_ptr p_tactic = tactic_ptr(new tactic_t(ts.tactic_id()));
		p_tactic->set_level(ts.tactic_level());
		m_tactic_map.insert(std::make_pair(p_tactic->get_tid(), p_tactic));
	}
}

void tactic_manager_t::peek_data(proto::common::tactic_data* p_tactic_data)
{
	if (NULL == p_tactic_data)
	{
		return;
	}
	for (auto it : m_tactic_map)
	{
		tactic_ptr p_tactic = it.second;
		if (NULL == p_tactic)
		{
			log_error("NULL == p_tactic");
			continue;
		}
		proto::common::tactic_single* p_single = p_tactic_data->add_tactics();
		p_tactic->peek_data(p_single);
	}
}

uint32_t tactic_manager_t::add_tactic(uint32_t tactic_id, bool notify)
{
	if (m_tactic_map.find(tactic_id) != m_tactic_map.end())
	{
		log_error("role[%lu] tactic[%d] already exist", m_role_uid, tactic_id);
		return errcode_enum::notice_unknown;
	}
	auto p_tactic_conf = GET_CONF(Tactic, tactic_id);
	if (NULL == p_tactic_conf)
	{
		log_error("NULL == p_tactic_conf[%d]", tactic_id);
		return errcode_enum::notice_unknown;
	}
	tactic_ptr p_tactic = tactic_ptr(new tactic_t(tactic_id));
	p_tactic->set_level(1);
	m_tactic_map.insert(std::make_pair(tactic_id, p_tactic));
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL != p_role)
	{
		p_role->save_self();
		if (notify)
		{
			proto::client::gc_tactic_update_ntf ntf;
			proto::common::tactic_single* p_tactic_single = ntf.add_tactics();
			p_tactic->peek_data(p_tactic_single);
			p_role->send_msg_to_client(op_cmd::gc_tactic_update_ntf, ntf);
		}
	}
	return errcode_enum::error_ok;
}

bool tactic_manager_t::has_tactic(uint32_t tactic_id)
{
	return m_tactic_map.find(tactic_id) != m_tactic_map.end();
}

