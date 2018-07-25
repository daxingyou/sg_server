#include "office.hpp"
#include "main/game_server_fwd.hpp"
#include "main/game_server.hpp"
#include "cache_key.hpp"
#include "role/role_unify_save.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "config/config_manager.hpp"
#include "tblh/Office.tbls.h"
#include "role/role_manager.hpp"
#include "log/log_wrapper.hpp"
#include "tblh/errcode_enum.hpp"
#include "item/drop_manager.hpp"

USING_NS_COMMON;

office_t::office_t(uint64_t uid)
	: m_owner(uid)
{
	m_key = cache_key_t::create(m_owner, cache_name::office);
}

office_t::~office_t()
{

}

void office_t::load_data(const proto::common::office_data& rd)
{
	m_level = rd.level();
	m_exp = rd.exp();
	m_award = rd.award();
}

void office_t::one_day()
{
	m_award = 0;
	save_self();
}


uint32_t office_t::get_red_num()
{
	uint32_t red_num = 0;

	return red_num;
}

void office_t::init()
{
	m_level = 1;

	save_self();
}

void office_t::peek_client_data(proto::common::office_client_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return;
	}
	p_data->set_level(m_level);
	p_data->set_exp(m_exp);
	p_data->set_award(m_award);
	Office* p_conf = GET_CONF(Office, m_level);
	if (NULL == p_conf)
	{
		log_error("NULL == p_conf Office[%d]", m_level);
		return;
	}
	role_ptr p_role = role_manager_t::find_role(m_owner);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_owner);
		return;
	}
	task_mgr_ptr p_task_mgr = p_role->get_task_mgr();
	if (NULL == p_task_mgr)
	{
		log_error("NULL == p_role[%lu]", m_owner);
		return;
	}
	//log_warn("p_conf->tasks task_num[%d]", (uint32_t)p_conf->tasks().size());
	for (auto task_id : p_conf->tasks())
	{
		proto::common::office_task_single* p_single = p_data->add_tasks();
		p_single->set_task_id(task_id);
		if (p_task_mgr->is_comp_task(task_id))
		{
			p_single->set_state(0);
		}
		else
		{
			p_single->set_state(1);
		}
	}
	//log_warn("task_num[%d]", p_data->tasks_size());
}

void office_t::check_levelup()
{
	Office* p_conf = GET_CONF(Office, m_level);
	if (NULL == p_conf)
	{
		log_error("NULL == p_conf Office[%d]", m_level);
		return;
	}
	if (m_exp < p_conf->exp())
	{
		//log_warn("exp[%d] need_exp[%d]", m_exp, p_conf->exp());
		return;
	}
	role_ptr p_role = role_manager_t::find_role(m_owner);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_owner);
		return;
	}
	if (p_role->get_level() < p_conf->need_level())
	{
		return;
	}
	task_mgr_ptr p_task_mgr = p_role->get_task_mgr();
	if (NULL == p_task_mgr)
	{
		log_error("NULL == p_role[%lu]", m_owner);
		return;
	}
	for (auto task_id : p_conf->tasks())
	{
		if (!p_task_mgr->is_comp_task(task_id))
		{
			//log_warn("task[%d] not comp", task_id);
			return;
		}
	}
	Office* p_next_conf = GET_CONF(Office, m_level+1);
	if (NULL == p_next_conf)
	{
		log_error("NULL == p_conf Office[%d]", m_level+1);
		return;
	}
	proto::client::gc_office_levelup_notify ntf;
	if (0 == m_award)
	{
		drop_manager_t::drop(p_role, p_conf->day_award(), log_enum::source_type_role_office_award, proto::common::drop_sys_type_lifeforce, m_level, ntf.mutable_rcd(), true);
	}
	m_level += 1;
	m_exp = 0;
	save_self();
	p_task_mgr->active_task_by_cond(*ntf.mutable_rcd(), true);
	peek_client_data(ntf.mutable_office_data());
	p_role->send_msg_to_client(op_cmd::gc_office_levelup_notify, ntf);
}


uint32_t office_t::award(proto::common::role_change_data* p_data)
{
	if (m_award > 0)
	{
		log_error("role[%lu] already award", m_owner);
		return errcode_enum::notice_unknown;
	}
	Office* p_conf = GET_CONF(Office, m_level);
	if (NULL == p_conf)
	{
		log_error("NULL == p_conf Office[%d]", m_level);
		return errcode_enum::notice_unknown;
	}
	role_ptr p_role = role_manager_t::find_role(m_owner);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_owner);
		return errcode_enum::notice_unknown;
	}
	drop_manager_t::drop(p_role, p_conf->day_award(), log_enum::source_type_role_office_award, proto::common::drop_sys_type_lifeforce, m_level, p_data, true);
	m_award = 1;
	save_self();
	return errcode_enum::error_ok;
}


void office_t::add_exp(uint32_t val)
{
	m_exp += val;
	check_levelup();
}

void office_t::peek_data(proto::common::office_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return;
	}
	p_data->set_level(m_level);
	p_data->set_exp(m_exp);
	p_data->set_award(m_award);
}

void office_t::save_self()
{
	proto::common::office_data od;
	peek_data(&od);
	role_unify_save::add_task(m_owner, m_key, od);
}
