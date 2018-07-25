#pragma once

#include "network.hpp"

class transfer_listener_t
    : public network::acceptor_callback_t
{
public:
	transfer_listener_t() {}
    virtual ~transfer_listener_t() {}

    virtual const char* name() { return "transfer"; }
    virtual void on_new_accept( const network::tcp_socket_ptr& s );
    virtual void on_new_msg( const network::tcp_socket_ptr& s, const network::message_t& msg );
    virtual void on_close( const network::tcp_socket_ptr& s );
};
