#include "admin.hpp"

admin_t::admin_t(const uint32_t admin_id, const std::string admin_name)
    : m_admin_id(admin_id)
    , m_admin_name(admin_name)
{
}

admin_t::admin_t(const uint32_t admin_id, const std::string admin_name, const network::tcp_socket_ptr& s)
    : m_admin_id(admin_id)
    , m_admin_name(admin_name)
    , m_socket(s)
{
}

admin_t::~admin_t()
{
}


void admin_t::disconnect_with_gm_client()
{
    get_socket()->close();
}