#include "admin_manager.hpp"

admin_map_type admin_manager_t::m_admin_map;

uint32_t admin_manager_t::s_admin_id = 0;

admin_ptr admin_manager_t::get_admin(const network::tcp_socket_ptr& s)
{
    admin_map_type::iterator it = m_admin_map.begin();
    admin_map_type::iterator it_end   = m_admin_map.end();
    for ( ; it != it_end; it++)
    {
        if ( (NULL != it->second) && (it->second->get_socket() == s) )
        {
            return it->second;
        }
    }

    return admin_ptr();
}


admin_ptr admin_manager_t::get_admin(uint32_t admin_id)
{
    admin_map_type::iterator it = m_admin_map.find(admin_id);
    if (it != m_admin_map.end())
    {
        return it->second;
    }

    return admin_ptr();
}


bool admin_manager_t::add_admin(uint32_t admin_id, std::string admin_name, const network::tcp_socket_ptr& s)
{
    if (get_admin(admin_id) != NULL)
    {
        log_error("add admin failed! admin[%lu] has been existed!", admin_id);
        return false;
    }

    admin_ptr admin(new admin_t(admin_id, admin_name, s));
    m_admin_map.insert(std::pair<uint32_t, admin_ptr>(admin_id, admin));
    log_debug("add new admin[%lu] success!", admin_id);

    return true;
}


bool admin_manager_t::del_admin(const network::tcp_socket_ptr& s)
{
    admin_ptr admin = get_admin(s);

    if (!admin)
    {
        log_error("del admin failed! couldn't find admin with the competent socket!");
        return false;
    }

    return del_admin(admin->get_admin_id());
}

bool admin_manager_t::del_admin(uint32_t admin_id)
{
    admin_map_type::iterator it = m_admin_map.find(admin_id);
    if (it == m_admin_map.end())
    {
        log_error("delete admin failed! admin[%lu] doesn't exist!", admin_id);
        return false;
    }

    admin_ptr admin = it->second;
    if (!admin)
    {
        log_error("delete admin failed! admin[%lu] is NULL!", admin_id);
        return false;
    }

    //先关闭连接
    admin->disconnect_with_gm_client();
    m_admin_map.erase(it);
    log_debug("delete admin[%lu] success!", admin_id);

    return true;
}


bool admin_manager_t::clear_admin()
{
    admin_map_type::iterator it_index = m_admin_map.begin();
    admin_map_type::iterator it_end   = m_admin_map.end();
    for ( ; it_index != it_end; it_index++)
    {
        admin_ptr admin = it_index->second;
        if (admin)
        {
            admin->disconnect_with_gm_client();
        }
    }

    m_admin_map.clear();
    log_info("admin map has been cleared!");

    return true;
}


bool admin_manager_t::is_admin_login(const network::tcp_socket_ptr& s, uint32_t &admin_id)
{
    admin_ptr p_admin = get_admin(s);
    if (NULL == p_admin)
    {
        return false;
    }
    else
    {
        admin_id = p_admin->get_admin_id();
    }

    return true;
}


bool admin_manager_t::is_admin_login(const uint32_t admin_id)
{
    return (NULL != get_admin(admin_id));
}

