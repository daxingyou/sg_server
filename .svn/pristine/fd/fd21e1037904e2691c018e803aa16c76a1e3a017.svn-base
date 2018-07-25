#include "role_buff_manager.hpp"
#include "tblh/RoleBuff.tbls.h"
#include "tblh/RoleBuffRule.tbls.h"
#include "config_mgr.h"
#include "tblh/errcode_enum.hpp"
#include "role/role_manager.hpp"
#include "log/log_wrapper.hpp"

USING_NS_COMMON;

role_buff_manager_t::role_buff_manager_t(uint64_t role_uid)
	: m_role_uid(role_uid)
{
	
}
void role_buff_manager_t::load_data(const proto::common::role_buff_data& buff_data)
{
	for (int i = 0; i < buff_data.role_buffs_size(); ++i)
	{
		const proto::common::role_buff_single& rbs = buff_data.role_buffs(i);
		role_buff_ptr p_buff = role_buff_ptr(new role_buff_t(m_role_uid));
		p_buff->load_data(rbs);
		m_buff_map.insert(std::make_pair(p_buff->get_tid(), p_buff));
	}
}

void role_buff_manager_t::peek_data(proto::common::role_buff_data* p_buff_data)
{
	if (NULL == p_buff_data)
	{
		return;
	}
	for (auto it : m_buff_map)
	{
		role_buff_ptr p_buff = it.second;
		if (NULL == p_buff)
		{
			log_error("NULL == p_buff");
			continue;
		}
		proto::common::role_buff_single* p_single = p_buff_data->add_role_buffs();
		p_buff->peek_data(p_single);
	}
}

void role_buff_manager_t::peek_obj_data(proto::common::object_common_info* p_obj_info)
{
	if (NULL == p_obj_info)
	{
		return;
	}
	for (auto it : m_buff_map)
	{
		role_buff_ptr p_buff = it.second;
		if (NULL == p_buff)
		{
			log_error("NULL == p_buff");
			continue;
		}
		p_obj_info->add_role_buff(p_buff->get_tid());
	}
}

bool role_buff_manager_t::add_role_buff(uint32_t buff_id, uint32_t source_type, uint32_t source_param)
{
	auto p_buff_conf = GET_CONF(RoleBuff, buff_id);
	if (NULL == p_buff_conf)
	{
		log_error("NULL == p_buff_conf[%d]", buff_id);
		return false;
	}
	uint32_t buff_type = p_buff_conf->type();
	uint32_t remain_time = p_buff_conf->time();
	role_buff_ptr p_exist_buff = NULL;
	for (auto it : m_buff_map)
	{
		auto p_buff = it.second;
		if (NULL == p_buff)
		{
			continue;
		}
		p_buff_conf = GET_CONF(RoleBuff, p_buff->get_tid());
		if (NULL == p_buff_conf)
		{
			log_error("NULL == p_buff_conf[%d]", p_buff->get_tid());
			continue;
		}
		if (buff_type == p_buff_conf->type())
		{
			p_exist_buff = p_buff;
			break;
		}
	}
	if (NULL != p_exist_buff)
	{
		auto p_buff_rule_conf = GET_CONF(RoleBuffRule, buff_type);
		if (NULL == p_buff_rule_conf)
		{
			log_error("NULL == p_buff_rule_conf[%d]", buff_type);
			return false;
		}
		if (p_buff_rule_conf->mutex() > 0)
		{
			ROLE_BUFF_LOG("role[%lu] addbuff[%d] failed, mutex", m_role_uid, buff_id);
			return false;
		}
		else if(p_buff_rule_conf->replace() > 0)
		{
			del_role_buff(p_exist_buff->get_tid(), log_enum::source_type_role_buff_replace, buff_id);
		}
	}
	role_buff_ptr p_new_buff = role_buff_ptr(new role_buff_t(m_role_uid));
	p_new_buff->set_tid(buff_id);
	m_buff_map.insert(std::make_pair(p_new_buff->get_tid(), p_new_buff));
	p_new_buff->do_effect(remain_time);
	ROLE_BUFF_LOG("role[%lu] addbuff[%d]", m_role_uid, buff_id);
	
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL != p_role)
	{
		log_wrapper_t::send_role_buff_log(m_role_uid, buff_id, log_enum::opt_type_add, p_role->get_real_speed(), source_type, source_param);
		p_role->save_self();
		proto::client::gc_role_buff_update_ntf ntf;
		proto::common::role_buff_data* p_buff_data = ntf.mutable_role_buffs();
		peek_data(p_buff_data);
		p_role->send_msg_to_client(op_cmd::gc_role_buff_update_ntf, ntf);
		p_role->notify_object_common_info_for_nearbies();
	}
	return true;
}

void role_buff_manager_t::del_role_buff(uint32_t buff_id, uint32_t source_type, uint32_t source_param, bool notify)
{
	auto it = m_buff_map.find(buff_id);
	if (it == m_buff_map.end())
	{
		log_error("role[%lu] NULL == p_buff[%d]", m_role_uid, buff_id);
		return;
	}
	auto p_buff = it->second;
	if (NULL == p_buff)
	{
		log_error("role[%lu] NULL == p_buff[%d]", m_role_uid, buff_id);
		return;
	}
	p_buff->lose_effect(notify);
	m_buff_map.erase(it);
	ROLE_BUFF_LOG("role[%lu] delbuff[%d]", m_role_uid, buff_id);
	role_ptr p_role = role_manager_t::find_role(m_role_uid);
	if (NULL != p_role)
	{
		log_wrapper_t::send_role_buff_log(m_role_uid, buff_id, log_enum::opt_type_del, p_role->get_real_speed(), source_type, source_param);
		p_role->save_self();
		if (notify)
		{
			proto::client::gc_role_buff_update_ntf ntf;
			proto::common::role_buff_data* p_buff_data = ntf.mutable_role_buffs();
			peek_data(p_buff_data);
			p_role->send_msg_to_client(op_cmd::gc_role_buff_update_ntf, ntf);
			p_role->notify_object_common_info_for_nearbies();
		}
	}
}
