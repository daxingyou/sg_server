#include "center_manager.hpp"
#include "main/transfer_server.hpp"
#include "main/transfer_server_fwd.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

void center_t::reset()
{
    m_id = 0;
    env::server->post_network_close_socket(get_socket());
}

center_vec center_manager_t::m_center_list;

void center_manager_t::add_center(const tcp_socket_ptr& s)
{
    if (NULL == s)
    {
        log_error("NULL == s");
        return;
    }

    center_ptr p_center = get_center_by_socket(s);
    if (NULL == p_center)
    {
        p_center.reset(new center_t(s));
        m_center_list.push_back(p_center);
        return;
    }
    else
    {
        try
        {
            log_error("center socket[%s: %d] has already exit!", s->socket().remote_endpoint().address().to_string().c_str(), s->socket().native());
        }
        catch (boost::system::system_error &ec)
        {
            log_error("throw exception: %s", ec.what());
        }
    }
}

void center_manager_t::set_center(const tcp_socket_ptr& s, uint32_t center_id)
{
    for (size_t idx = 0; idx < m_center_list.size(); ++idx)
    {
        center_ptr p_center = m_center_list.at(idx);
        if (NULL != p_center && (NULL != s && p_center->get_socket() == s))
        {
            p_center->set_id(center_id);
            log_info("add center[%d], has %d centers", center_id, (uint32_t)m_center_list.size());
            break;
        }
    }
	//proto::server::er_world_cup_data_request req;
	//req.set_server_id(center_id);
	//env::server->send_msg_to_area(op_cmd::er_world_cup_data_request, 0, req);
}

void center_manager_t::del_center(const tcp_socket_ptr& s)
{
    center_vec::iterator itr = m_center_list.begin();
    center_vec::iterator itr_end = m_center_list.end();
    for (; itr != itr_end; ++itr)
    {
        center_ptr p_center = *itr;
        if (NULL != p_center && (NULL != s && p_center->get_socket() == s))
        {
            uint32_t id = p_center->get_id();
            p_center->reset();
            m_center_list.erase(itr);
            log_info("del center[%d], left %d centers", id, (uint32_t)m_center_list.size());
            return;
        }
    }
}

void center_manager_t::clear_center()
{
    for (int32_t i = 0; i < (int32_t)m_center_list.size(); ++i)
    {
        center_ptr p_center = m_center_list[i];
        if (NULL != p_center)
        {
            p_center->reset();
        }
    }
    m_center_list.clear();
}

//----------------------------------------------------------------------------------------
center_ptr center_manager_t::get_center_by_socket(const tcp_socket_ptr& s)
{
    for (int32_t i = 0; i < (int32_t)m_center_list.size(); ++i)
    {
        center_ptr p_center = m_center_list[i];
        if (NULL != p_center && (NULL != s && p_center->get_socket() == s))
        {
            return p_center;
        }
    }
    return center_ptr();
}

center_ptr center_manager_t::get_center_by_id(uint32_t center_id)
{
    for (int32_t i = 0; i < (int32_t)m_center_list.size(); ++i)
    {
        center_ptr p_center = m_center_list[i];
        if (NULL != p_center && (0 != center_id && p_center->get_id() == center_id))
        {
            return p_center;
        }
    }
    return center_ptr();
}

uint32_t center_manager_t::get_center_id_by_socket(const tcp_socket_ptr& s)
{
    for (size_t idx = 0; idx < m_center_list.size(); ++idx)
    {
        center_ptr p_center = m_center_list.at(idx);
        if (NULL != p_center && (NULL != s && p_center->get_socket() == s))
        {
            return p_center->get_id();
        }
    }

    return 0;
}

const tcp_socket_ptr center_manager_t::get_center_socket_by_id(uint32_t center_id)
{
    for (size_t idx = 0; idx < m_center_list.size(); ++idx)
    {
        center_ptr p_center = m_center_list.at(idx);
        if (NULL != p_center && (0 != center_id && p_center->get_id() == center_id))
        {
            return p_center->get_socket();
        }
    }

    return NULL;
}