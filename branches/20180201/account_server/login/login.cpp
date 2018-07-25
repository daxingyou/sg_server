#include "login.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

tcp_socket_ptr login_t::reset_socket(tcp_socket_ptr const& s)
{
    tcp_socket_ptr old_socket;
    if (NULL != get_socket())
    {
        login_socket_data_t* data = (login_socket_data_t*)get_socket()->get_bind_data();
        if (NULL != data)
        {
            delete data;
            data = NULL;
        }
        get_socket()->set_bind_data(NULL);
        old_socket = get_socket();
    }
    m_socket = s;
    return old_socket;
}