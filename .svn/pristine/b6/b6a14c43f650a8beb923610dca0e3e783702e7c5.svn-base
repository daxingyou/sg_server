#include "sys_notice_manager.hpp"

#include "log.hpp"
#include "server.pb.h"
#include "client.pb.h"
#include "scene/scene.hpp"
#include "tblh/SysChannel.tbls.h"
#include "role/role_manager.hpp"
#include "role/role.hpp"

proto::client::gc_sys_init_notice_notify sys_notice_manager_t::init_notify;


bool sys_notice_manager_t::init()
{
	return reload();
}

bool sys_notice_manager_t::reload()
{
    proto::client::gc_sys_init_notice_notify temp;
    init_notify.Swap(&temp);

	std::map<uint32_t, SysNoticeTable*> confs;
	GET_ALL_CONF(SysNoticeTable, confs);
	for (std::map<uint32_t, SysNoticeTable*>::iterator itr = confs.begin();
		itr != confs.end(); ++itr)
	{
		SysNoticeTable* p_table = itr->second;
		if (NULL == p_table)
		{
			log_error("sys_notice[%u] null error", itr->first);
			continue;
		}

		// 为1 说明是初始发送
		if (p_table->is_init() == 1)
		{
			proto::common::sys_notice_data* data = init_notify.add_notices();
			if (NULL != data)
			{
				data->set_type((proto::common::system_notice_type)p_table->notice_type());
				data->set_priority(p_table->priority());
				data->set_text(p_table->text());
			}
		}
	}

	return true;
}

void sys_notice_manager_t::close()
{
    proto::client::gc_sys_init_notice_notify temp;
    init_notify.Swap(&temp);
}

void sys_notice_manager_t::on_role_login(role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("p_role null error");
		return;
	}

	p_role->send_msg_to_client(op_cmd::gc_sys_init_notice_notify, init_notify);
}

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
	proto::server::ga_broadcast_notify msg_to_gate;
	msg_to_gate.set_cmd(op_cmd::gc_sys_notice_notify);
	msg_to_gate.set_data(cmd_data);

	env::server->send_msg_to_all_gate(op_cmd::ga_broadcast_notify, 0, msg_to_gate);
}

void sys_notice_manager_t::broadcast_system_notice_to_scene(const scene_ptr& p_scene, SysNoticeTable* p_table, uint64_t uid, uint64_t family_id, uint64_t country_id, const std::string& text)
{
	if (NULL == p_table)
	{
		log_error("SysNoticeTable null error");
		return;
	}

	if (NULL == p_scene)
	{
		log_error("p_scene null error");
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

	p_scene->send_msg_to_scene(op_cmd::gc_sys_notice_notify, ntf);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool sys_notice_manager_t::broadcast_system_msg_for_money(const uint64_t role_uid, proto::common::MONEY_TYPE type, uint32_t param, uint32_t param_id /*= 0*/)
{
	role_ptr p_role = role_manager_t::find_role(role_uid);
	if (NULL == p_role)
	{
		log_error("role[%lu] null error",role_uid);
		return false;
	}

	if (0 == param)
	{
		return false;
	}
	
	BROADCAST_SYS_NOTICE_SYS_CHANNEL(role_uid, proto::common::sys_notify_type_money, type, param);

	/*switch (type)
	{
	case proto::common::MONEY_TYPE_LIFEFORCE:
	{
		BROADCAST_SYS_NOTICE_SYS_CHANNEL(role_uid, proto::common::sys_notify_type_add_lifeforce, type, param);
	}break;
	case proto::common::MONEY_TYPE_COPPER:
	{
		BROADCAST_SYS_NOTICE_SYS_CHANNEL(role_uid, proto::common::sys_notify_type_add_copper, type, param);
	}break;
	case proto::common::MONEY_TYPE_ENERGY:
	{
		BROADCAST_SYS_NOTICE_SYS_CHANNEL(role_uid, proto::common::sys_notify_type_add_energy, type, param);
	}break;
	case proto::common::MONEY_TYPE_SILVER:
	{
		BROADCAST_SYS_NOTICE_SYS_CHANNEL(role_uid, proto::common::sys_notify_type_add_sliver, type, param);
	}break;
	case proto::common::MONEY_TYPE_YUANBAO:
	{
		BROADCAST_SYS_NOTICE_SYS_CHANNEL(role_uid, proto::common::sys_notify_type_add_gold, type, param);
	}break;
	case proto::common::MONEY_TYPE_HONOR:
	{
		BROADCAST_SYS_NOTICE_SYS_CHANNEL(role_uid, proto::common::sys_notify_type_honor, type,  param);
	}break;
	case proto::common::MONEY_TYPE_CONTRIBUTION:
	{
		BROADCAST_SYS_NOTICE_SYS_CHANNEL(role_uid, proto::common::sys_notify_type_contribution, type, param);
	}break;
	case proto::common::MONEY_TYPE_LIFE_STONE:
	{
		BROADCAST_SYS_NOTICE_SYS_CHANNEL(role_uid, proto::common::sys_notify_type_life_stone, type, param);
	}break;
	case proto::common::MONEY_TYPE_HEROSOUL:
	{
		BROADCAST_SYS_NOTICE_SYS_CHANNEL(role_uid, proto::common::sys_notify_type_herosoul, type,  param);
	}break;
	case proto::common::MONEY_TYPE_PRESTIGE:
	{
		BROADCAST_SYS_NOTICE_SYS_CHANNEL(role_uid, proto::common::sys_notify_type_add_prestige, type,  param);
	}break;
	case proto::common::MONEY_TYPE_LIVENESS:
	{
		BROADCAST_SYS_NOTICE_SYS_CHANNEL(role_uid, proto::common::sys_notify_type_liveness, type,  param);
	}break;
	case proto::common::MONEY_TYPE_EXP:
	{
		BROADCAST_SYS_NOTICE_SYS_CHANNEL(role_uid, proto::common::sys_notify_type_add_exp, type, param);
	}break;
	case proto::common::MONEY_TYPE_ACHIEVE_POINT:
	{
		BROADCAST_SYS_NOTICE_SYS_CHANNEL(role_uid, proto::common::sys_notify_type_add_achieve_point, type, param);
	}break;
	case proto::common::MONEY_TYPE_VIGOUR:
	{
		BROADCAST_SYS_NOTICE_SYS_CHANNEL(role_uid, proto::common::sys_notify_type_vigour, type, param);
	}break;
	default:
		break;
	}*/
	return true;
}

void sys_notice_manager_t::broadcast_system_notice(uint64_t uid, const std::string& text, uint32_t label)
{
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("role[%lu] is null",uid);
		return;
	}

	if (text.empty())
	{
		log_error("text empty error");
		return;
	}

	proto::client::gc_sys_notice_notify ntf;
	proto::common::sys_notice_data* data = ntf.add_notices();
	if (NULL != data)
	{
		data->set_type(proto::common::system_notice_type::SYS_NOTICE_CHANNEL);
		data->set_text(text);
		data->set_role_id(common::string_util_t::uint64_to_string(uid));
		data->set_label(label);
	}

	p_role->send_msg_to_client(op_cmd::gc_sys_notice_notify, ntf);
}

void sys_notice_manager_t::broadcast_system_notice(SysNoticeTable* p_table, uint64_t uid, const std::string& text, uint32_t label /*= 0*/)
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
	}

	if (NULL == env::server)
	{
		log_error("env::server null error");
		return;
	}

	std::string cmd_data;
	network::wrap_msg(cmd_data, ntf);
	proto::server::ga_broadcast_notify msg_to_gate;
	msg_to_gate.set_cmd(op_cmd::gc_sys_notice_notify);
	msg_to_gate.set_data(cmd_data);

	env::server->send_msg_to_all_gate(op_cmd::ga_broadcast_notify, 0, msg_to_gate);
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

bool sys_notice_manager_t::update_broadcast_count(uint32_t type, uint32_t param)
{
	proto::common::sys_notity_single single;
	single.set_type(type);
	single.set_param(param);
	single.set_count(1);
	if (update_sys_notify_from_center(single,false))
	{
		sys_notice_ptr p_sys_notice = get_sys_notice(type, param);
		if (NULL == p_sys_notice)
		{
			log_error("NULL == p_sys_notice type[%d] param[%d]", type, param);
			return false;
		}
		proto::server::ge_check_sys_notice_request msg;
		proto::common::sys_notity_data* p_data = msg.mutable_sys_notice_list();
		proto::common::sys_notity_single* p_single = p_data->add_sys_notity_data_list();
		p_sys_notice->peek_data(p_single);
		env::server->send_msg_to_center(op_cmd::ge_check_sys_notice_request, 0, msg);
	}

	return true;
}

bool sys_notice_manager_t::update_sys_notify_from_center(const proto::common::sys_notity_single& single,bool is_from_center /* = true */)
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
		///log_warn("type[%d] param[%d] count[%d]", single.type(), single.param(), 1);
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
			///log_warn("type[%d] param[%d] count[%d]", single.type(), single.param(),1);
		}
		else
		{
			uint32_t cur_count = p_sys_notice->get_count();
			///log_warn("cur_count[%d] ,is_from_center[%d]", cur_count, is_from_center);
			if (is_from_center)
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

bool sys_notice_manager_t::load_sys_notify_from_center(const proto::common::sys_notity_data& data)
{
	m_sys_notice_map.clear();
	for (int32_t i = 0; i < data.sys_notity_data_list_size(); ++i)
	{
		sys_notice_ptr p_sys_notice = sys_notice_ptr(new sys_notice_t());
		if (NULL == p_sys_notice)
		{
			return false;
		}
			
		const proto::common::sys_notity_single& single = data.sys_notity_data_list(i);
		SysChannel* p_config = GET_CONF(SysChannel, hash_util_t::hash_to_uint32(single.type(), single.param()));
		if (NULL == p_config)
		{
			log_error("NULL == p_config,type[%d],param_id[%d]", single.type(), single.param());
			continue;
		}

		p_sys_notice->load_data(single);
		m_sys_notice_map.insert(std::make_pair(hash_util_t::hash_to_uint32(single.type(), single.param()), p_sys_notice));
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

void sys_notice_manager_t::clear_notice_list()
{
	m_sys_notice_map.clear();
}

void sys_notice_manager_t::gm_clear_notify_to_center()
{
	clear_notice_list();
	proto::server::ge_gm_clear_notice_request msg;
	env::server->send_msg_to_center(op_cmd::ge_gm_clear_notice_request, 0, msg);
}

void sys_notice_manager_t::sys_notice_notify(uint64_t uid, proto::common::sys_notify_type type, uint32_t param_id, std::vector<std::string>& str_list)
{
	SysChannel* p_config = GET_CONF(SysChannel, hash_util_t::hash_to_uint32((uint32_t)type, param_id));
	if (NULL == p_config)
	{
		log_error("NULL == p_config type[%d] param[%d]",type,param_id);
		return;
	}

	if (IS_NOTIFY_TYPE_2 == p_config->is_notify())
	{
		log_error("type[%d] param[%d] not notify",type,param_id);
		return;
	}

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("role[%lu] is null", uid);
		return;
	}

	proto::client::gc_sys_notice_notify ntf;
	proto::common::sys_notice_data* data = ntf.add_notices();
	data->set_label(p_config->label());
	data->set_type(proto::common::system_notice_type::SYS_NOTICE_CHANNEL);
	data->set_table_type_id((uint32_t)type);
	data->set_param(param_id);
	data->set_role_id(common::string_util_t::uint64_to_string(uid));

	for (auto& it : str_list)
	{
		data->add_param_str_list(it.c_str());
	}

	if (IS_NOTIFY_TYPE_0 == p_config->is_notify())
	{
		LanguageTable* p_notice = GET_CONF(LanguageTable, p_config->language_id());
		if (NULL == p_notice)
		{
			log_error("NULL == p_notice LanguageTable_ID[%d]", p_config->language_id());
			return;
		}

		if (p_notice->str().empty())
		{
			log_error("p_notice->str() empty LanguageTable_ID[%d]", p_config->language_id());
			return;
		}
		data->set_default_text(common::string_util_t::convert_to_utf8(p_notice->str()).c_str());
		p_role->send_msg_to_client(op_cmd::gc_sys_notice_notify, ntf);
	} 
	else
	{
		uint32_t count = sys_notice_manager_t::get_broadcast_count(type, param_id);
		if (0 == p_config->count() || count <= p_config->count())
		{
			SysNoticeTable* p_notice = GET_CONF(SysNoticeTable, p_config->notify_id());
			if (NULL == p_notice)
			{
				log_error("NULL == p_notice SysNoticeTable_ID[%d]", p_config->notify_id());
				return;
			}

			if (p_notice->text().empty())
			{
				log_error("p_notice->str() empty SysNoticeTable_ID[%d]", p_config->notify_id());
				return;
			}

			data->set_default_text(common::string_util_t::convert_to_utf8(p_notice->text()).c_str());
			data->set_type(proto::common::system_notice_type(p_notice->notice_type()));
			if (NULL == env::server)
			{
				log_error("env::server null error");
				return;
			}

			std::string cmd_data;
			network::wrap_msg(cmd_data, ntf);
			proto::server::ga_broadcast_notify msg_to_gate;
			msg_to_gate.set_cmd(op_cmd::gc_sys_notice_notify);
			msg_to_gate.set_data(cmd_data);

			env::server->send_msg_to_all_gate(op_cmd::ga_broadcast_notify, 0, msg_to_gate);
		}
		else 
		{
			LanguageTable* p_notice = GET_CONF(LanguageTable, p_config->language_id());
			if (NULL == p_notice)
			{
				log_error("NULL == p_notice LanguageTable_ID[%d]", p_config->language_id());
				return;
			}

			if (p_notice->str().empty())
			{
				log_error("p_notice->str() empty LanguageTable_ID[%d]", p_config->language_id());
				return;
			}

			data->set_default_text(common::string_util_t::convert_to_utf8(p_notice->str()).c_str());
			p_role->send_msg_to_client(op_cmd::gc_sys_notice_notify, ntf);
		}	
	}

	update_broadcast_count(type,param_id);
}



