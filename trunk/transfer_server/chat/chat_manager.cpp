#include "chat_manager.hpp"
#include "log.hpp"
#include "main/transfer_server.hpp"
#include "main/transfer_server_fwd.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

chat_vector chat_manager_t::m_chat_list;

void chat_manager_t::add_chat(const network::tcp_socket_ptr& sokcet)
{
    if (NULL == sokcet)
    {
        log_error("NULL == sokcet");
        return;
    }

    chat_ptr p_chat(new chat_t(sokcet));
    m_chat_list.push_back(p_chat);
}

void chat_manager_t::del_chat(const network::tcp_socket_ptr& socket)
{
    if (NULL == socket)
    {
        log_error("NULL == socket");
        return;
    }
    chat_vector::iterator it = m_chat_list.begin();
    for (; it != m_chat_list.end(); ++it)
    {
        chat_ptr p_chat = (*it);
        if (NULL != p_chat && p_chat->get_socket() == socket)
        {
            log_info("delete chat of server_id[%d]", p_chat->get_server_id());
            env::server->post_network_close_socket(socket);
            m_chat_list.erase(it);
            return;
        }
    }
}

void chat_manager_t::clear_chat()
{
    m_chat_list.clear();
    log_info("clear all chats");
}

chat_ptr chat_manager_t::get_chat(const network::tcp_socket_ptr& socket)
{
    if (NULL == socket)
    {
        log_error("NULL == socket");
        return chat_ptr();
    }

    for (int32_t i = 0; i < (int32_t)m_chat_list.size(); ++i)
    {
        chat_ptr p_chat = m_chat_list[i];
        if (NULL != p_chat && p_chat->get_socket() == socket)
        {
            return p_chat;
        }
    }

    return chat_ptr();
}

chat_ptr chat_manager_t::get_chat(uint32_t server_id)
{
	if (server_id == 0)
	{
		log_error("server_id <= 0");
		return chat_ptr();
	}

	for (int32_t i = 0; i < (int32_t)m_chat_list.size(); ++i)
	{
		chat_ptr p_chat = m_chat_list[i];
		if (NULL != p_chat && p_chat->get_server_id() == server_id)
		{
			return p_chat;
		}
	}

	return chat_ptr();
}


tcp_socket_ptr chat_manager_t::get_chat_socket(uint32_t server_id)
{
    for (int32_t i = 0; i < (int32_t)m_chat_list.size(); ++i)
    {
        chat_ptr p_chat = m_chat_list[i];
        if (NULL != p_chat && p_chat->get_server_id() == server_id)
        {
            return p_chat->get_socket();
        }
    }

    return NULL;
}

void chat_manager_t::set_chat_server_info(const network::tcp_socket_ptr& socket, uint32_t server_id, const std::string& server_name)
{
	for (size_t idx = 0; idx < m_chat_list.size(); ++idx)
	{
		chat_ptr p_chat = m_chat_list.at(idx);
		if (NULL != p_chat && (NULL != socket && p_chat->get_socket() == socket))
		{
			p_chat->set_server_id(server_id);
			p_chat->set_server_name(server_name);
			log_info("add chat[%d], has %d chats", server_id, (uint32_t)m_chat_list.size());
			return;
		}
	}
}