#include "gate_manager.hpp"
#include "log.hpp"
#include "main/login_server.hpp"
#include "main/login_server_fwd.hpp"
#include "user/user_manager.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

gate_vec gate_manager_t::m_gate_list;

void gate_manager_t::add_gate(const tcp_socket_ptr& s)
{
    if (NULL == s)
    {
        log_error("NULL == s");
        return;
    }

    gate_ptr p_gate = get_gate_by_socket(s);
    if (NULL == p_gate)
    {
        p_gate.reset(new gate_t(s));
        m_gate_list.push_back(p_gate);
        return;
    }
    else
    {
        try
        {
            log_error("gate socket[%s: %d] has already exit!", s->socket().remote_endpoint().address().to_string().c_str(), s->socket().native());
        }
        catch (boost::system::system_error &ec)
        {
            log_error("throw exception: %s", ec.what());
        }
    }
}

void gate_manager_t::del_gate(const tcp_socket_ptr& s)
{
    if (NULL == s)
    {
        log_error("NULL == s");
        return;
    }

    gate_vec::iterator itr = m_gate_list.begin();
    gate_vec::iterator itr_end = m_gate_list.end();
    for (; itr != itr_end; ++itr)
    {
        gate_ptr p_gate = *itr;
        if (NULL != p_gate && (NULL != s && p_gate->get_socket() == s))
        {
            log_info("del gate[%d], left %d gates", p_gate->get_id(), (uint32_t)m_gate_list.size()-1);

            env::server->post_network_close_socket(p_gate->get_socket());
            m_gate_list.erase(itr);
            return;
        }
    }
}

void gate_manager_t::clear_gate()
{
    for (int32_t i = 0; i < (int32_t)m_gate_list.size(); ++i)
    {
        gate_ptr p_gate = m_gate_list[i];
        if (NULL != p_gate)
        {
            p_gate->clear_user();
            env::server->post_network_close_socket(p_gate->get_socket());
        }
    }
    m_gate_list.clear();

    log_info("clear all gates success!");
}

gate_ptr gate_manager_t::get_gate_by_socket(const tcp_socket_ptr& s)
{
    for (int32_t i = 0; i < (int32_t)m_gate_list.size(); ++i)
    {
        gate_ptr p_gate = m_gate_list[i];
        if (NULL != p_gate && (NULL != s && p_gate->get_socket() == s))
        {
            return p_gate;
        }
    }
    return gate_ptr();
}

gate_ptr gate_manager_t::get_gate_by_gate_id(uint32_t gate_id)
{
    for (int32_t i = 0; i < (int32_t)m_gate_list.size(); ++i)
    {
        gate_ptr p_gate = m_gate_list[i];
        if (NULL != p_gate && (0 != gate_id && p_gate->get_id() == gate_id))
        {
            return p_gate;
        }
    }
    return gate_ptr();
}

uint32_t gate_manager_t::get_gate_id_by_socket(const network::tcp_socket_ptr& s)
{
    for (size_t idx = 0; idx < m_gate_list.size(); ++idx)
    {
        gate_ptr p_gate = m_gate_list.at(idx);
        if (NULL != p_gate && (NULL != s && p_gate->get_socket() == s))
        {
            return p_gate->get_id();
        }
    }

    return 0;
}

const network::tcp_socket_ptr gate_manager_t::get_gate_socket_by_id(uint32_t gate_id)
{
    for (size_t idx = 0; idx < m_gate_list.size(); ++idx)
    {
        gate_ptr p_gate = m_gate_list.at(idx);
        if (NULL != p_gate && (0 != gate_id && p_gate->get_id() == gate_id))
        {
            return p_gate->get_socket();
        }
    }

    return NULL;
}

gate_ptr gate_manager_t::get_gate_by_user_uid(uint64_t uid)
{
    for (size_t idx = 0; idx < m_gate_list.size(); ++idx)
    {
        gate_ptr p_gate = m_gate_list.at(idx);
        if (NULL != p_gate && p_gate->find_user(uid))
        {
            return p_gate;
        }
    }
    return gate_ptr();
}

int32_t g_val = 1;
gate_ptr gate_manager_t::get_best_gate()
{
    int32_t size = (int32_t)m_gate_list.size();
    if (size <= 0)
    {
        log_error("gate size[%d] <= 0", size);
        return gate_ptr();
    }

    int32_t index = g_val % size - 1;
    if (index < 0)
    {
        index = size - 1;
    }
    g_val += 1;

    const gate_ptr& p_gate = m_gate_list[index];
    if (NULL == p_gate)
    {
        log_error("NULL == p_gate, index: %d", index);
        return gate_ptr();
    }
    return p_gate;
}

boost::tuple <uint32_t, std::string> gate_manager_t::get_all_online_users_num()
{
    uint32_t num = 0;
    std::string num_str = "";
    for (int32_t i = 0; i < (int32_t)m_gate_list.size(); ++i)
    {
        gate_ptr p_gate = m_gate_list[i];
        if (NULL != p_gate)
        {
            char str[256] = { 0 };
            snprintf(str, sizeof(str), " gate[%d]:%d ", p_gate->get_id(), p_gate->get_online_users_num());
            num_str.append(str);
            num += p_gate->get_online_users_num();
        }
    }
    return boost::make_tuple(num, num_str);
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
                log_info("add gate[%d], has %d gates", gate_id, (uint32_t)m_gate_list.size());
                return;
            }
        }
    }
}

void gate_manager_t::set_gate(uint64_t uid, uint32_t gate_id)
{
    const gate_ptr& p_gate = get_gate_by_user_uid(uid);
    if (NULL == p_gate)
    {
        log_error("NULL == p_gate");
        return;
    }
    uint32_t old_gate_id = p_gate->get_id();
    p_gate->set_id(gate_id);
    ROLE_LOG("set user[%lu] gate from [%d] to [%d]", uid, old_gate_id, p_gate->get_id());
}

void gate_manager_t::add_user_to_gate(const gate_ptr& p_gate, uint64_t uid)
{
    if (NULL == p_gate)
    {
        log_error("NULL == p_gate");
        return;
    }
    p_gate->add_user(uid);
}

void gate_manager_t::add_user_to_gate(uint32_t gate_id, uint64_t uid)
{
    const gate_ptr& p_gate = get_gate_by_gate_id(gate_id);
    if (NULL == p_gate)
    {
        log_error("NULL == p_gate");
        return;
    }
    p_gate->add_user(uid);
}

void gate_manager_t::del_user_from_gate(const network::tcp_socket_ptr& s, uint64_t uid)
{
    const gate_ptr& p_gate = get_gate_by_socket(s);
    if (NULL == p_gate)
    {
        log_error("NULL == p_gate");
        return;
    }
    p_gate->del_user(uid);
}

void gate_manager_t::del_all_gate_user(uint32_t gate_id)
{
	const gate_ptr& p_gate = get_gate_by_gate_id(gate_id);
	if (NULL != p_gate) {
		p_gate->clear_user();
	}
}
