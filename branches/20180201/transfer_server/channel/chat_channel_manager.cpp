#include "chat_channel_manager.hpp"
#include "log.hpp"
#include "config/config_manager.hpp"
#include "tblh/Comprehensive.tbls.h"

uint32_t chat_channel_manager_t::m_start_channel_id = 1000000001;
chat_channel_map chat_channel_manager_t::m_chat_channels;
chat_channel_name chat_channel_manager_t::m_channel_names;
composite_channel chat_channel_manager_t::m_public_channels;

void chat_channel_manager_t::init_channel()
{
	auto p_public_channel_name = GET_CONF(Comprehensive, comprehensive_common::public_channel_name);
	std::string public_public_channel_name = (NULL != p_public_channel_name && !p_public_channel_name->parameter1().empty()) ? p_public_channel_name->parameter1().c_str() : "";

	std::string first_public_channel_name = make_first_name(public_public_channel_name);
	chat_channel_manager_t::add_channel(first_public_channel_name, proto::common::channel_type_public);

    log_info("init chat channel success!");
}

uint64_t chat_channel_manager_t::add_channel(std::string channel_name, proto::common::channel_type channel_type)
{
	if (m_channel_names.find(channel_name) != m_channel_names.end())
	{
		log_error("add chat_channel[%s] fail! name is exist!", channel_name.c_str());
		return 0;
	}
	uint32_t channel_id = 0;
	chat_channel_ptr p_channel = chat_channel_ptr(new chat_channel_t());
	if (nullptr == p_channel)
	{
        log_error("add chat_channel[%s] fail! nullptr == p_channel", channel_name.c_str());
		return 0;
	}
	channel_id = m_start_channel_id++;
	p_channel->init_channel(channel_id, channel_name, channel_type);
	m_chat_channels.insert(std::make_pair(channel_id, p_channel));
	m_channel_names.insert(std::make_pair(channel_name, channel_id));
	if (channel_type == proto::common::channel_type_public)
	{
		m_public_channels.push_back(p_channel);
	}
	log_debug("add chat_channel[%s] channel_type[%u] succ!", channel_name.c_str(), channel_type);
	return channel_id;
}

chat_channel_ptr chat_channel_manager_t::get_channel(uint32_t channel_id)
{
	auto it = m_chat_channels.find(channel_id);
	if (it != m_chat_channels.end())
	{
		return it->second;
	}
	return chat_channel_ptr();
}

void chat_channel_manager_t::erase_channel(uint32_t channel_id)
{
	chat_channel_ptr p_channel = get_channel(channel_id);
	if (nullptr == p_channel)
	{
		log_error("channel[%d] not find", channel_id);
		return;
	}
	m_channel_names.erase(p_channel->get_channel_name());
	m_chat_channels.erase(channel_id);
}

bool chat_channel_manager_t::add_user(chat_user_ptr p_user, uint32_t channel_id)
{
	chat_channel_ptr p_channel = get_channel(channel_id);
	if (nullptr == p_channel)
	{
		return false;
	}
	return p_channel->add_user(p_user);
}

void chat_channel_manager_t::earse_user(uint64_t uid, uint32_t channel_id)
{
	chat_channel_ptr p_channel = get_channel(channel_id);
	if (nullptr == p_channel)
	{
		return;
	}
	p_channel->earse_user(uid);
}

chat_user_ptr chat_channel_manager_t::get_user(uint64_t uid, uint32_t channel_id)
{
	chat_channel_ptr p_channel = get_channel(channel_id);
	if (nullptr != p_channel)
	{
		return p_channel->get_user(uid);
	}
	return chat_user_ptr();
}

void chat_channel_manager_t::peek_all_datas(proto::common::server_channel_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("p_data NULL");
		return;
	}
	for (auto it : m_chat_channels)
	{
		chat_channel_ptr p_chat_channel = it.second;
		if (nullptr == p_chat_channel)
		{
			continue;
		}
		proto::common::server_channel_single* p_single = p_data->add_server_channel_single();
		if (nullptr == p_single)
		{
			continue;
		}
		p_chat_channel->peek_data(p_single);
	}
}

bool chat_channel_manager_t::is_need_add_channel()
{
	if (m_public_channels.size() > 10)
	{
		return false;
	}

	for (auto channel : m_public_channels)
	{
		if (!channel->is_hot())
		{
			return false;
		}
	}
	return true;
}

std::string chat_channel_manager_t::make_first_name(std::string channel_name)
{
	char name_fomat[32] = { 0 };
    snprintf(name_fomat, sizeof(name_fomat), "%s %d", channel_name.c_str(), 1);
	return std::string(name_fomat);
}

std::string chat_channel_manager_t::make_next_name(std::string channel_name)
{
	auto pos = channel_name.find_last_of(" ");
	if (std::string::npos != pos)
	{
		uint32_t index = boost::lexical_cast<uint32_t>(channel_name.substr(pos + 1, channel_name.size()).c_str());
		std::string name = channel_name.substr(0, pos);
		char name_fomat[32] = { 0 };
        snprintf(name_fomat, sizeof(name_fomat), "%s %u", name.c_str(), index + 1);
		return std::string(name_fomat);
	}
	return std::string("");
}
