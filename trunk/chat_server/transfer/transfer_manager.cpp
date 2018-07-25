#include "transfer_manager.hpp"
#include "main/chat_server.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

void transfer_t::reset()
{
    m_id = 0;
    env::server->post_network_close_socket(get_socket());
}

transfer_vec transfer_manager_t::m_transfer_list;

void transfer_manager_t::add_transfer(const tcp_socket_ptr& s)
{
    if (NULL == s)
    {
        log_error("NULL == s");
        return;
    }

    transfer_ptr p_transfer = get_transfer_by_socket(s);
    if (NULL == p_transfer)
    {
        p_transfer.reset(new transfer_t(s));
        m_transfer_list.push_back(p_transfer);
        return;
    }
    else
    {
        try
        {
            log_error("transfer socket[%s: %d] has already exit!", s->socket().remote_endpoint().address().to_string().c_str(), s->socket().native());
        }
        catch (boost::system::system_error &ec)
        {
            log_error("throw exception: %s", ec.what());
        }
    }
}

void transfer_manager_t::set_transfer(const tcp_socket_ptr& s, uint32_t transfer_id)
{
    for (size_t idx = 0; idx < m_transfer_list.size(); ++idx)
    {
        transfer_ptr p_transfer = m_transfer_list.at(idx);
        if (NULL != p_transfer)
        {
            if (transfer_id == p_transfer->get_id())
            {
                del_transfer(s);
                log_error("the transfer[%d] already exists in the transfer list!", transfer_id);
                return;
            }

            if (NULL != s && p_transfer->get_socket() == s)
            {
                p_transfer->set_id(transfer_id);
                log_info("add transfer[%d], has %d transfers", transfer_id, get_transfer_size());
                return;
            }
        }
    }
}

void transfer_manager_t::del_transfer(const tcp_socket_ptr& s)
{
    if (NULL == s)
    {
        log_error("NULL == s");
        return;
    }

    transfer_vec::iterator it = m_transfer_list.begin();
    transfer_vec::iterator it_end = m_transfer_list.end();
    for (; it != it_end; ++it)
    {
        transfer_ptr p_transfer = (*it);
        if (NULL != p_transfer && p_transfer->get_socket() == s)
        {
            uint32_t transfer_id = p_transfer->get_id();
            p_transfer->reset();
            m_transfer_list.erase(it);
            log_info("del transfer[%d], left %d transfers", transfer_id, get_transfer_size());
            return;
        }
    }
}

void transfer_manager_t::clear_transfer()
{
    for (auto p_transfer : m_transfer_list)
    {
        if (NULL != p_transfer)
        {
            p_transfer->reset();
        }
    }
    m_transfer_list.clear();
    log_info("clear all transfers success!");
}

//-------------------------------------------------------------------------------------------
transfer_ptr transfer_manager_t::get_transfer_by_socket(const tcp_socket_ptr& s)
{
    for (auto p_transfer : m_transfer_list)
    {
        if (NULL != p_transfer && (NULL != s && p_transfer->get_socket() == s))
        {
            return p_transfer;
        }
    }
    return transfer_ptr();
}

transfer_ptr transfer_manager_t::get_transfer_by_id(uint32_t transfer_id)
{
    for (auto p_transfer : m_transfer_list)
    {
        if (NULL != p_transfer && (0 != transfer_id && p_transfer->get_id() == transfer_id))
        {
            return p_transfer;
        }
    }
    return transfer_ptr();
}


transfer_ptr transfer_manager_t::get_transfer_by_rand()
{
    if (0 == get_transfer_size())
    {
        return transfer_ptr();
    }

    int32_t rand_index = rand() % get_transfer_size();
    return m_transfer_list[rand_index];
}

uint32_t transfer_manager_t::get_transfer_id_by_socket(const tcp_socket_ptr& s)
{
    if (NULL == s)
    {
        log_error("NULL == s");
        return 0;
    }

    for (auto p_transfer : m_transfer_list)
    {
        if (NULL != p_transfer && p_transfer->get_socket() == s)
        {
            return p_transfer->get_id();
        }
    }

    return 0;
}


const tcp_socket_ptr transfer_manager_t::get_transfer_socket_by_id(uint32_t transfer_id)
{
    for (auto p_transfer : m_transfer_list)
    {
        if (NULL != p_transfer && (0 != transfer_id && p_transfer->get_id() == transfer_id))
        {
            return p_transfer->get_socket();
        }
    }

    return NULL;
}