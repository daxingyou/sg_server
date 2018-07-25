#ifndef __GATE_USER_LISTENER_HPP__
#define __GATE_USER_LISTENER_HPP__

#include "network.hpp"

class user_listener_t
    : public network::acceptor_callback_t
{
public:
    virtual const char* name() { return "client"; }
    virtual void on_new_accept( const network::tcp_socket_ptr& s );
    virtual void on_new_msg( const network::tcp_socket_ptr& s, const network::message_t& msg );
    virtual void on_close( const network::tcp_socket_ptr& s );
};

#endif
