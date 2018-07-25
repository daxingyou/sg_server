#include "gate_manager.hpp"
#include "main/chat_server_fwd.hpp"
#include "main/chat_server.hpp"
#include "common/log.hpp"

USING_NS_COMMON;

std::vector<gate_ptr> gate_manager_t::m_gate_list;

void gate_manager_t::add_gate(const network::tcp_socket_ptr& s)
{
    for (size_t idx = 0; idx < m_gate_list.size(); ++idx)
    {
        gate_ptr p_gate = m_gate_list.at(idx);
        if (NULL != p_gate)
        {
            if (p_gate->get_socket() == s && NULL != s)
            {
                try
                {
                    log_error("gate socket[%s:%d] has already exist!", s->socket().remote_endpoint().address().to_string().c_str(), s->socket().native());
                }
                catch (boost::system::system_error &ec)
                {
                    log_error("throw exception:%s", ec.what());
                }

                return;
            }
        }
    }

    gate_ptr p_gate(new gate_t(s));
    if (NULL != p_gate)
    {
        m_gate_list.push_back(p_gate);
    }
}

void gate_manager_t::set_gate(const network::tcp_socket_ptr& s, uint32_t gate_id)
{
    for (size_t idx = 0; idx < m_gate_list.size(); ++idx)
    {
        gate_ptr p_gate = m_gate_list.at(idx);
        if (NULL != p_gate)
        {
            if (gate_id == p_gate->get_id())
            {
                del_gate(s);
                log_error("the gate[id=%d] already exists in the game list!", gate_id);
                return;
            }

            if (NULL != s && p_gate->get_socket() == s)
            {
                p_gate->set_id(gate_id);
                log_info("add gate[%d], has %d gates", gate_id, m_gate_list.size());
                return;
            }
        }
    }
}

void gate_manager_t::del_gate(const network::tcp_socket_ptr& s)
{
    std::vector<gate_ptr>::iterator itr = m_gate_list.begin();
    for ( ; itr != m_gate_list.end(); ++itr)
    {
        gate_ptr p_gate = *itr;
        if (NULL != p_gate && s == p_gate->get_socket())
        {
            log_info("del gate[%d], left %d gates", p_gate->get_id(), m_gate_list.size()-1);
            env::server->post_network_close_socket(p_gate->get_socket());
            m_gate_list.erase(itr);
            return;
        }
    }
}

void gate_manager_t::clear_gate()
{
    for (size_t i = 0; i < m_gate_list.size(); ++i)
    {
        gate_ptr p_gate = m_gate_list.at(i);
        if (NULL != p_gate)
        {
            env::server->post_network_close_socket(p_gate->get_socket());
        }
    }
    m_gate_list.clear();
}

uint32_t gate_manager_t::get_gate_size()
{
    return m_gate_list.size();
}

const network::tcp_socket_ptr gate_manager_t::get_gate_socket_by_id(uint32_t gate_id)
{
    for (size_t idx = 0; idx < m_gate_list.size(); ++idx)
    {
        gate_ptr p_gate = m_gate_list.at(idx);
        if (NULL != p_gate && p_gate->get_id() == gate_id)
        {
            return p_gate->get_socket();
        }
    }

    return nullptr;
}

uint32_t gate_manager_t::get_gate_id_by_socket(const network::tcp_socket_ptr& s)
{
    for (size_t idx = 0; idx < m_gate_list.size(); ++idx)
    {
        gate_ptr p_gate = m_gate_list.at(idx);
        if (NULL != p_gate && p_gate->get_socket() == s)
        {
            return p_gate->get_id();
        }
    }

    return 0;
}