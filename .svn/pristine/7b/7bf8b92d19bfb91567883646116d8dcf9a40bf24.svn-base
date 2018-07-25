#include "sys_notice_manager.hpp"

#include "log.hpp"
#include "server.pb.h"
#include "client.pb.h"
#include "main/center_server_fwd.hpp"
#include "main/center_server.hpp"
#include "op_cmd.hpp"
#include "protobuf.hpp"
#include "user/global_user_manager.hpp"

void sys_notice_manager_t::broadcast_system_notice(SysNoticeTable* p_table, uint64_t uid, uint64_t family_id, uint64_t country_id, const std::string& text)
{
	if (NULL == p_table)
	{
		log_error("SysNoticeTable null error");
		return;
	}

	proto::client::gc_sys_notice_notify ntf;
	proto::common::sys_notice_data* data = ntf.add_notices();

	if (NULL != data)
	{
		data->set_type((proto::common::system_notice_type)p_table->notice_type());
		data->set_priority(p_table->priority());
		data->set_text(text);

		if (uid != 0)
			data->set_role_id(common::string_util_t::uint64_to_string(uid));

		if (family_id != 0)
			data->set_family_id(common::string_util_t::uint64_to_string(family_id));

		if (country_id != 0)
			data->set_country_id(common::string_util_t::uint64_to_string(country_id));
	}

	if (NULL == env::server)
	{
		log_error("env::server null error");
		return;
	}

	std::string cmd_data;
	network::wrap_msg(cmd_data, ntf);
	proto::server::ea_broadcast_notify msg_to_gate;
	msg_to_gate.set_cmd(op_cmd::gc_sys_notice_notify);
	msg_to_gate.set_data(cmd_data);

	env::server->send_msg_to_all_gate(op_cmd::ea_broadcast_notify, 0, msg_to_gate);
}

//////////////////////////////////////////////////////////////////////////

void sys_notice_manager_t::broadcast_system_notice(SysNoticeTable* p_table, uint64_t uid, const std::string& text, uint32_t label)
{
	if (NULL == p_table)
	{
		log_error("SysNoticeTable null error");
		return;
	}

	proto::client::gc_sys_notice_notify ntf;
	proto::common::sys_notice_data* data = ntf.add_notices();

	if (NULL != data)
	{
		data->set_type((proto::common::system_notice_type)p_table->notice_type());
		data->set_priority(p_table->priority());
		data->set_text(text);
		data->set_label(label);

		if (uid != 0)
			data->set_role_id(common::string_util_t::uint64_to_string(uid));
	}

	if (NULL == env::server)
	{
		log_error("env::server null error");
		return;
	}

	std::string cmd_data;
	network::wrap_msg(cmd_data, ntf);
	proto::server::ea_broadcast_notify msg_to_gate;
	msg_to_gate.set_cmd(op_cmd::gc_sys_notice_notify);
	msg_to_gate.set_data(cmd_data);

	env::server->send_msg_to_all_gate(op_cmd::ea_broadcast_notify, 0, msg_to_gate);
}

void sys_notice_manager_t::broadcast_system_notice(uint64_t uid, const std::string& text, uint32_t label)
{
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return;
	}

	if (text.empty())
	{
		log_error("text empty error");
		return;
	}

	proto::client::ec_sys_notice_notify ntf;
	proto::common::sys_notice_data* data = ntf.add_notices();

	if (NULL != data)
	{
		data->set_type(proto::common::system_notice_type::SYS_NOTICE_CHANNEL);
		data->set_text(text);
		data->set_label(label);
		data->set_role_id(common::string_util_t::uint64_to_string(uid));		
	}

	p_user->send_msg_to_client(op_cmd::ec_sys_notice_notify, ntf);
}

bool sys_notice_manager_t::update_limit_count(const proto::common::sys_notity_single& single,bool is_from_game /* = true */)
{
	SysChannel* p_config = GET_CONF(SysChannel, hash_util_t::hash_to_uint32(single.type(), single.param()));
	if (NULL == p_config)
	{
		log_error("NULL == p_config,type[%d],param[%d]", single.type(), single.param());
		return false;
	}

	sys_notice_map::iterator iter = m_sys_notice_map.find(hash_util_t::hash_to_uint32(single.type(), single.param()));
	if (iter == m_sys_notice_map.end())
	{
		sys_notice_ptr p_sys_notice = sys_notice_ptr(new sys_notice_t());
		if (NULL == p_sys_notice)
			return false;
		p_sys_notice->load_data(single);
		p_sys_notice->set_count(1);
		m_sys_notice_map.insert(std::make_pair(hash_util_t::hash_to_uint32(single.type(), single.param()), p_sys_notice));
	}
	else
	{
		sys_notice_ptr p_sys_notice = iter->second;
		if (NULL == p_sys_notice)
		{
			sys_notice_ptr p_notice = sys_notice_ptr(new sys_notice_t());
			if (NULL == p_notice)
				return false;
			p_notice->load_data(single);
			p_notice->set_count(1);
			m_sys_notice_map.insert(std::make_pair(hash_util_t::hash_to_uint32(single.type(), single.param()), p_notice));
		}
		else
		{
			uint32_t cur_count = p_sys_notice->get_count();
			///log_warn("cur_count[%d] is_from_game[%d] single.count[%d]", cur_count, is_from_game, single.count());
			if (is_from_game)
			{
				p_sys_notice->set_count((single.count() > cur_count ? single.count() : cur_count));
			}
			else
			{
				p_sys_notice->set_count(single.count() + cur_count);
			}
			///log_warn("type[%d] param[%d] count[%d]", single.type(), single.param(), p_sys_notice->get_count());
		}
	}
	
	return true;

}

sys_notice_ptr sys_notice_manager_t::get_sys_notice(uint32_t type, uint32_t param)
{
	sys_notice_map::iterator iter = m_sys_notice_map.find(hash_util_t::hash_to_uint32(type, param));
	if (iter != m_sys_notice_map.end())
	{
		sys_notice_ptr p_sys_notice = iter->second;
		if (NULL != p_sys_notice)
		{
			return p_sys_notice;
		}
	}
	///log_warn("NULL != p_sys_notice type[%d] param[%d]", type, param);
	return sys_notice_ptr();
}

bool sys_notice_manager_t::handle_ge_get_sys_notice_list_request(const network::tcp_socket_ptr& s)
{
	proto::server::eg_syn_sys_notice_notify msg;
	proto::common::sys_notity_data* p_data = msg.mutable_sys_notice_list();
	sys_notice_ptr p_sys_notice = sys_notice_ptr();

	for (auto& it : m_sys_notice_map)
	{
		p_sys_notice = it.second;
		if (NULL == p_sys_notice)
		{
			log_error("NULL == p_sys_notice");
			return false;
		}

		if (0 == p_sys_notice->get_type() ||
			0 == p_sys_notice->get_tid())
		{
			continue;
		}

		proto::common::sys_notity_single* p_single = p_data->add_sys_notity_data_list();
		p_sys_notice->peek_data(p_single);
	}
	env::server->send_msg_to_game(s, op_cmd::eg_syn_sys_notice_notify, 0, msg);
	return true;
}

void sys_notice_manager_t::update_broadcast_count(uint32_t type, uint32_t param)
{
	proto::common::sys_notity_single single;
	single.set_type(type);
	single.set_param(param);
	single.set_count(1);

	if (update_limit_count(single,false))
	{
		sys_notice_ptr p_sys_notice = get_sys_notice(type, param);
		if (NULL == p_sys_notice)
		{
			log_error("NULL == p_sys_notice type[%d] param[%d]",type,param);
			return;
		}
		proto::server::eg_check_sys_notice_reply msg_reply;
		proto::common::sys_notity_data* p_data = msg_reply.mutable_sys_notice_list();
		proto::common::sys_notity_single* p_single = p_data->add_sys_notity_data_list();
		p_sys_notice->peek_data(p_single);
		env::server->send_msg_to_all_games(op_cmd::eg_check_sys_notice_reply, 0, msg_reply);
	}
}

uint32_t sys_notice_manager_t::get_broadcast_count(uint32_t type, uint32_t param)
{
	sys_notice_map::iterator iter = m_sys_notice_map.find(hash_util_t::hash_to_uint32(type, param));
	if (iter == m_sys_notice_map.end())
	{
		return 0;
	}
	else
	{
		sys_notice_ptr p_sys_notice = iter->second;
		if (NULL == p_sys_notice)
		{
			return 0;
		}
		return p_sys_notice->get_count();
	}
}

void sys_notice_manager_t::clear_notice_list()
{
	m_sys_notice_map.clear();
}

