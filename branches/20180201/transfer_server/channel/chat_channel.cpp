#include "chat_channel.hpp"
#include "config/config_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "chat_channel_manager.hpp"

void chat_channel_t::init_channel(uint32_t channel_id, std::string channel_name, proto::common::channel_type channel_type)
{
	m_uid = channel_id;
	m_name = channel_name;
	m_channel_type = channel_type;
	auto p_max_channel_users = GET_CONF(Comprehensive, comprehensive_common::max_channel_users);
	m_max_user_count = GET_COMPREHENSIVE_VALUE_1(p_max_channel_users);
}

bool chat_channel_t::add_user(chat_user_ptr p_user)
{
	if (NULL == p_user)
	{
		log_error("p_user NULL");
		return false;
	}
	if (m_max_user_count == m_user_list.size())
	{
		return false;
	}
	m_user_list.insert(std::make_pair(p_user->get_uid(), p_user));
	
	server_channel_user_map::iterator iter = m_server_user_list.find(p_user->get_server_id());

	if (m_server_user_list.end() == iter)
	{
		chat_user_map temp_server_channel_user;
		temp_server_channel_user.insert(std::make_pair(p_user->get_uid(), p_user));
		m_server_user_list.insert(std::make_pair(p_user->get_server_id(), temp_server_channel_user));
	}
	else
	{
		iter->second.insert(std::make_pair(p_user->get_uid(), p_user));
	}
	do 
	{
		if (!is_hot())
		{
			break;
		}

		if (m_channel_type != proto::common::channel_type_public)
		{
			break;
		}

		if (chat_channel_manager_t::is_need_add_channel())
		{
			std::string next_name = chat_channel_manager_t::make_next_name(m_name);
			if (next_name.empty())
			{
				break;
			}

			chat_channel_manager_t::add_channel(next_name, m_channel_type);
			log_debug("public channel is hot, need add channel!");
		}
	}
	while (false);

	return true;
}

void chat_channel_t::earse_user(uint64_t uid)
{
	chat_user_map::iterator iter_user = m_user_list.find(uid);
	if (m_user_list.end() == iter_user)
		return;

	// 查找该玩家所在的服务器用户组
	server_channel_user_map::iterator iter_server = m_server_user_list.find(iter_user->second->get_server_id());
	if (m_server_user_list.end() == iter_server)
		return;

	// 从玩家总表中删除 同步更新所在服务器用户组
	m_user_list.erase(iter_user);
	iter_server->second.erase(uid);
}

chat_user_ptr chat_channel_t::get_user(uint64_t uid)
{
	chat_user_map::iterator it = m_user_list.find(uid);
	if (it != m_user_list.end())
	{
		return it->second;
	}
	return chat_user_ptr();
}

void chat_channel_t::peek_data(proto::common::server_channel_single* p_single)
{
	p_single->set_uid(m_uid);
	p_single->set_name(m_name);
	p_single->set_type(m_channel_type);
	if (m_max_user_count != 0)
	{
		uint32_t base_count = m_max_user_count / 4;
		if (base_count == 0)
		{
			return;
		}
		if (m_user_list.size() == m_max_user_count)
		{
			p_single->set_state(proto::common::channel_full);
		}
		else if (m_user_list.size() < base_count)
		{
			p_single->set_state(proto::common::channel_free);
		}
		else if (m_user_list.size() < base_count * 2)
		{
			p_single->set_state(proto::common::channel_lively);
		}
		else
		{
			p_single->set_state(proto::common::channel_hot);
		}
	}
}

void chat_channel_t::peek_role_list(uint32_t server_id, proto::common::fh_channel_role_list* role_list)
{
	chat_user_ptr p_user = nullptr;
	role_list->clear_notify_role_arrary();

	server_channel_user_map::iterator iter_server = m_server_user_list.find(server_id);
	chat_user_map::iterator iter_user = iter_server->second.begin();
	for (; iter_user != iter_server->second.end(); ++iter_user)
	{
		p_user = iter_user->second;
		role_list->add_notify_role_arrary(p_user->get_uid());
	}
}