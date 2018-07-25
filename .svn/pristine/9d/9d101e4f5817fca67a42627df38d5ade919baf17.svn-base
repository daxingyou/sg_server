#include "transfer_manager.hpp"
#include "log.hpp"
#include "main/area_server.hpp"
#include "main/area_server_fwd.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

transfer_vector transfer_manager_t::m_transfer_list;
size_t transfer_manager_t::m_cur_index = 0;

bool transfer_manager_t::add_transfer(const network::tcp_socket_ptr& s)
{
    if (NULL == s)
    {
        log_error("NULL == sokcet");
        return false;
	}
	for (size_t idx = 0; idx < m_transfer_list.size(); ++idx)
	{
		transfer_ptr p_transfer = m_transfer_list.at(idx);
		if (NULL != p_transfer)
		{
			if (p_transfer->get_socket() == s && NULL != s)
			{
				try
				{
					log_error("transfer socket[%s:%d] has already exist!", s->socket().remote_endpoint().address().to_string().c_str(), s->socket().native());
				}
				catch (boost::system::system_error &ec)
				{
					log_error("throw exception:%s", ec.what());
				}

				return false;
			}
		}
	}


    transfer_ptr p_chat(new transfer_t(s));
	m_transfer_list.push_back(p_chat);
	return true;
}

void transfer_manager_t::del_transfer(const network::tcp_socket_ptr& s)
{
    if (NULL == s)
    {
        log_error("NULL == socket");
        return;
    }
	transfer_vector::iterator it = m_transfer_list.begin();
    for (; it != m_transfer_list.end(); ++it)
    {
        transfer_ptr p_transfer = (*it);
        if (NULL != p_transfer && p_transfer->get_socket() == s)
        {
            log_info("delete transfer of server_id[%d]", p_transfer->get_id());
            env::server->post_network_close_socket(s);
			m_transfer_list.erase(it);
            return;
        }
    }
}

void transfer_manager_t::clear_transfer()
{
    m_transfer_list.clear();
    log_info("clear all transfer");
}

transfer_ptr transfer_manager_t::get_transfer(const network::tcp_socket_ptr& s)
{
    if (NULL == s)
    {
        log_error("NULL == socket");
        return transfer_ptr();
    }

    for (int32_t i = 0; i < (int32_t)m_transfer_list.size(); ++i)
    {
        transfer_ptr p_transfer = m_transfer_list[i];
        if (NULL != p_transfer && p_transfer->get_socket() == s)
        {
            return p_transfer;
        }
    }

    return transfer_ptr();
}

transfer_ptr transfer_manager_t::get_transfer(uint32_t server_id)
{
	if (server_id <= 0)
	{
		log_error("server_id <= 0");
		return transfer_ptr();
	}

	for (int32_t i = 0; i < (int32_t)m_transfer_list.size(); ++i)
	{
		transfer_ptr p_transfer = m_transfer_list[i];
		if (NULL != p_transfer && p_transfer->get_id() == server_id)
		{
			return p_transfer;
		}
	}

	return transfer_ptr();
}


uint32_t transfer_manager_t::get_transfer_id_by_socket(const network::tcp_socket_ptr& s)
{
	transfer_ptr p_transfer = get_transfer(s);
	if (p_transfer)
	{
		return p_transfer->get_id();
	}
	return 0;
}

const network::tcp_socket_ptr transfer_manager_t::get_transfer_socket(uint32_t server_id)
{
	transfer_ptr p_transfer = get_transfer(server_id);
	if (p_transfer)
	{
		return p_transfer->get_socket();
	}
    return network::tcp_socket_ptr();
}

void transfer_manager_t::set_transfer_server_info(const network::tcp_socket_ptr& s, uint32_t server_id)
{
    for (size_t idx = 0; idx < m_transfer_list.size(); ++idx)
    {
        transfer_ptr p_transfer = m_transfer_list.at(idx);
        if (NULL != p_transfer)
        {
            if (server_id == p_transfer->get_id())
            {
                del_transfer(s);
                log_error("the transfer[id=%d] already exists in the transfer list!", server_id);
                return;
            }

            if (NULL != s && p_transfer->get_socket() == s)
            {
                p_transfer->set_id(server_id);
                log_info("add transfer[%d], has %d transfer", server_id, m_transfer_list.size());
                return;
            }
        }
    }
}

const network::tcp_socket_ptr transfer_manager_t::get_avg_transfer_socket()
{
	if (m_transfer_list.empty())
	{
		return network::tcp_socket_ptr();
	}

	if (m_cur_index < (m_transfer_list.size() - 1))
	{
		++m_cur_index;
	}
	else
	{
		m_cur_index = 0;
	}

	transfer_ptr p_transfer = m_transfer_list.at(m_cur_index);
	if (p_transfer != NULL)
	{
		if (p_transfer->get_id() > 0)
			return p_transfer->get_socket();
		else
			del_transfer(p_transfer->get_socket());
	}
	else
		m_transfer_list.erase(m_transfer_list.begin() + m_cur_index);
	return get_avg_transfer_socket();
}
