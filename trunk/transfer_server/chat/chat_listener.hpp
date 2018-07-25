#ifndef __TRANSFER_CHAT_LISTENER_HPP__
#define __TRANSFER_CHAT_LISTENER_HPP__

#include "network.hpp"

class chat_listener_t
    : public network::acceptor_callback_t
{
public:
    chat_listener_t() {}
    virtual ~chat_listener_t() {}

    virtual const char* name() { return "chat"; }
    virtual void on_new_accept( const network::tcp_socket_ptr& s );
    virtual void on_new_msg( const network::tcp_socket_ptr& s, const network::message_t& msg );
    virtual void on_close( const network::tcp_socket_ptr& s );
};

#endif
