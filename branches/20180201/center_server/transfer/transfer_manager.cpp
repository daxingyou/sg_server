#include "transfer_manager.hpp"
#include "main/center_server.hpp"
#include "main/center_server_fwd.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

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
        p_transfer.reset(new transfer_t());
        p_transfer->set_socket(s);
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
    if (NULL == s)
    {
        log_error("NULL == s");
        return;
    }
    if (transfer_id == 0)
    {
        log_error("transfer_id == 0");
        return;
    }
    transfer_ptr p_transfer = get_transfer_by_socket(s);
    if (NULL == p_transfer)
    {
        log_error("NULL == p_transfer");
        return;
    }

    p_transfer->set_id(transfer_id);

    // 同步center服务器信息给该transfer服务器
    proto::server::ef_syn_center_info_notify noti;
    noti.set_server_id(env::xml->srv_group.group_id);
    env::server->send_msg_to_transfer(s, op_cmd::ef_syn_center_info_notify, 0, noti);

    log_info("add transfer[%d], syn to the transfer, has %d transfers", transfer_id, m_transfer_list.size());
}

void transfer_manager_t::del_transfer(const tcp_socket_ptr& s)
{
    if (NULL == s)
    {
        log_error("NULL == s");
        return;
    }

    auto it = m_transfer_list.begin();
    auto it_end = m_transfer_list.end();

    for (; it != it_end; ++it)
    {
        transfer_ptr p_transfer = (*it);
        if (NULL != p_transfer && p_transfer->get_socket() == s)
        {
            log_info("del transfer[%d] socket, left %d transfers", p_transfer->get_id(), m_transfer_list.size()-1);
            env::server->post_network_close_socket(p_transfer->get_socket());
            m_transfer_list.erase(it);
            break;
        }
    }
}

void transfer_manager_t::clear_transfer()
{
    for (auto p_transfer : m_transfer_list)
    {
        if (NULL != p_transfer)
        {
            env::server->post_network_close_socket(p_transfer->get_socket());
        }
    }
    m_transfer_list.clear();
}

//-------------------------------------------------------------------------------------------
transfer_ptr transfer_manager_t::get_transfer_by_socket(const tcp_socket_ptr& s)
{
    if (NULL == s)
    {
        log_error("NULL == s");
        return transfer_ptr();
    }

    for (auto p_transfer : m_transfer_list)
    {
        if (NULL != p_transfer && p_transfer->get_socket() == s)
        {
            return p_transfer;
        }
    }
    return transfer_ptr();
}

transfer_ptr transfer_manager_t::get_transfer_by_id(uint32_t transfer_id)
{
    if (0 == transfer_id)
    {
        log_error("0 == transfer_id");
        return transfer_ptr();
    }

    for (auto p_transfer : m_transfer_list)
    {
        if (NULL != p_transfer && p_transfer->get_id() == transfer_id)
        {
            return p_transfer;
        }
    }
    return transfer_ptr();
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
    if (0 == transfer_id)
    {
        log_error("0 == transfer_id");
        return NULL;
    }

    for (auto p_transfer : m_transfer_list)
    {
        if (NULL != p_transfer && p_transfer->get_id() == transfer_id)
        {
            return p_transfer->get_socket();
        }
    }

    return NULL;
}