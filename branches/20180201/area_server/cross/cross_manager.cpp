#include "cross_manager.hpp"
#include "log.hpp"
#include "main/area_server.hpp"
#include "main/area_server_fwd.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

std::vector<cross_ptr> cross_manager_t::m_cross_list;
size_t cross_manager_t::m_cur_index = 0;

bool cross_manager_t::add_cross(const network::tcp_socket_ptr& s)
{
    for (size_t idx = 0; idx < m_cross_list.size(); ++idx)
    {
        cross_ptr p_cross = m_cross_list.at(idx);
        if (NULL != p_cross)
        {
            if (p_cross->get_socket() == s && NULL != s)
            {
                try
                {
                    log_error("cross socket[%s:%d] has already exist!", s->socket().remote_endpoint().address().to_string().c_str(), s->socket().native());
                }
                catch (boost::system::system_error &ec)
                {
                    log_error("throw exception:%s", ec.what());
                }

                return false;
            }
        }
    }

    cross_ptr p_cross(new cross_t(s));
    if (NULL == p_cross)
    {
		log_error("p_cross null error");
		return false;
    }

	m_cross_list.push_back(p_cross);

	return true;
}

void cross_manager_t::del_cross(const network::tcp_socket_ptr& s)
{
    std::vector<cross_ptr>::iterator itr = m_cross_list.begin();
    for (; itr != m_cross_list.end(); ++itr)
    {
        cross_ptr p_cross = *itr;
        if (NULL != p_cross && s == p_cross->get_socket())
        {
            log_info("del cross[%d], left %d crosss", p_cross->get_id(), m_cross_list.size()-1);

            env::server->post_network_close_socket(p_cross->get_socket());
            m_cross_list.erase(itr);
            return;
        }
    }
}

void cross_manager_t::clear_cross()
{
    for (size_t i = 0; i < m_cross_list.size(); ++i)
    {
        cross_ptr p_cross = m_cross_list.at(i);
        if (NULL != p_cross)
        {
            env::server->post_network_close_socket(p_cross->get_socket());
        }
    }
    m_cross_list.clear();
}

void cross_manager_t::set_cross(const network::tcp_socket_ptr& s, uint32_t cross_id)
{
    for (size_t idx = 0; idx < m_cross_list.size(); ++idx)
    {
        cross_ptr p_cross = m_cross_list.at(idx);
        if (NULL != p_cross)
        {
            if (cross_id == p_cross->get_id())
            {
                del_cross(s);
                log_error("the cross[id=%d] already exists in the cross list!", cross_id);
                return;
            }

            if (NULL != s && p_cross->get_socket() == s)
            {
                p_cross->set_id(cross_id);
                log_info("add cross[%d], has %d crosss", cross_id, m_cross_list.size());
                return;
            }
        }
    }
}

uint32_t cross_manager_t::get_cross_id_by_socket(const network::tcp_socket_ptr& s)
{
    for (size_t idx = 0; idx < m_cross_list.size(); ++idx)
    {
        cross_ptr p_cross = m_cross_list.at(idx);
        if (NULL != p_cross)
        {
            if (p_cross->get_socket() == s)
            {
                return p_cross->get_id();
            }
        }
    }

    return 0;
}

const network::tcp_socket_ptr cross_manager_t::get_cross_socket_by_id(uint32_t cross_id)
{
    for (size_t idx = 0; idx < m_cross_list.size(); ++idx)
    {
        cross_ptr p_cross = m_cross_list.at(idx);
        if (NULL != p_cross)
        {
            if (p_cross->get_id() == cross_id)
            {
                return p_cross->get_socket();
            }
        }
    }

    return nullptr;
}

const network::tcp_socket_ptr cross_manager_t::get_avg_cross_socket()
{
	if (m_cross_list.empty())
	{
		return network::tcp_socket_ptr();
	}

	if (m_cur_index < (m_cross_list.size() - 1))
	{
		++m_cur_index;
	}
	else
	{
		m_cur_index = 0;
	}
	
	cross_ptr p_cross = m_cross_list.at(m_cur_index);
	if (p_cross != NULL)
	{
		if(p_cross->get_id() > 0)
			return p_cross->get_socket();
		else
			del_cross(p_cross->get_socket());
	}
	else
		m_cross_list.erase(m_cross_list.begin() + m_cur_index);
	return get_avg_cross_socket();
}
