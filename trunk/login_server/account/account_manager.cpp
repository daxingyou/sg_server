#include "account_manager.hpp"
#include "main/login_server.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

void account_t::reset()
{
    m_id = 0;
    env::server->post_network_close_socket(get_socket());
}

account_vec account_manager_t::m_account_list;

void account_manager_t::add_account(const tcp_socket_ptr& s)
{
    if (NULL == s)
    {
        log_error("NULL == s");
        return;
    }

    account_ptr p_account = get_account_by_socket(s);
    if (NULL == p_account)
    {
        p_account.reset(new account_t(s));
        m_account_list.push_back(p_account);
        return;
    }
    else
    {
        try
        {
            log_error("account socket[%s: %d] has already exit!", s->socket().remote_endpoint().address().to_string().c_str(), s->socket().native());
        }
        catch (boost::system::system_error &ec)
        {
            log_error("throw exception: %s", ec.what());
        }
    }
}

void account_manager_t::set_account(const tcp_socket_ptr& s, uint32_t account_id)
{
    for (size_t idx = 0; idx < m_account_list.size(); ++idx)
    {
        account_ptr p_account = m_account_list.at(idx);
        if (NULL != p_account)
        {
            if (account_id == p_account->get_id())
            {
                del_account(s);
                log_error("the account[%d] already exists in the account list!", account_id);
                return;
            }

            if (NULL != s && p_account->get_socket() == s)
            {
                p_account->set_id(account_id);
                log_info("add account[%d], has %d accounts", account_id, (uint32_t)get_account_size());
                return;
            }
        }
    }
}

void account_manager_t::del_account(const tcp_socket_ptr& s)
{
    if (NULL == s)
    {
        log_error("NULL == s");
        return;
    }

    account_vec::iterator itr = m_account_list.begin();
    account_vec::iterator itr_end = m_account_list.end();
    for (; itr != itr_end; ++itr)
    {
        account_ptr p_account = *itr;
        if (NULL != p_account && (NULL != s && p_account->get_socket() == s))
        {
            uint32_t account_id = p_account->get_id();
			p_account->reset();
            m_account_list.erase(itr);
            log_info("del account[%d], left %d accounts", account_id, (uint32_t)get_account_size());
            return;
        }
    }
}

void account_manager_t::clear_account()
{
    for (int32_t i = 0; i < (int32_t)m_account_list.size(); ++i)
    {
        account_ptr p_account = m_account_list[i];
        if (NULL != p_account)
        {
            p_account->reset();
        }
    }
    m_account_list.clear();

    log_info("clear all accounts success!");
}

//----------------------------------------------------------------------------------------
account_ptr account_manager_t::get_account_by_socket(const tcp_socket_ptr& s)
{
    for (int32_t i = 0; i < (int32_t)m_account_list.size(); ++i)
    {
        account_ptr p_account = m_account_list[i];
        if (NULL != p_account && (NULL != s && p_account->get_socket() == s))
        {
            return p_account;
        }
    }
    return account_ptr();
}

account_ptr account_manager_t::get_account_by_account_id(uint32_t account_id)
{
    for (int32_t i = 0; i < (int32_t)m_account_list.size(); ++i)
    {
        account_ptr p_account = m_account_list[i];
        if (NULL != p_account && (0 != account_id && p_account->get_id() == account_id))
        {
            return p_account;
        }
    }
    return account_ptr();
}

uint32_t account_manager_t::get_account_id_by_socket(const tcp_socket_ptr& s)
{
    for (size_t idx = 0; idx < m_account_list.size(); ++idx)
    {
        account_ptr p_account = m_account_list.at(idx);
        if (NULL != p_account && (NULL != s && p_account->get_socket() == s))
        {
            return p_account->get_id();
        }
    }

    return 0;
}

const tcp_socket_ptr account_manager_t::get_account_socket_by_id(uint32_t account_id)
{
    for (size_t idx = 0; idx < m_account_list.size(); ++idx)
    {
        account_ptr p_account = m_account_list.at(idx);
        if (NULL != p_account && (0 != account_id && p_account->get_id() == account_id))
        {
			return p_account->get_socket();
        }
    }

    return NULL;
}

uint32_t account_manager_t::get_account_size()
{
    return (uint32_t)m_account_list.size();
}