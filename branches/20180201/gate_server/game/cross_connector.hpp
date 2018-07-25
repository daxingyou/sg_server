#ifndef __GATE_CROSS_connector_HPP__
#define __GATE_CROSS_connector_HPP__

#include "network.hpp"

class cross_connector_t
    : public network::connector_callback_t
{
public:
    cross_connector_t() {}
    virtual ~cross_connector_t() {}

    virtual const char* name() { return "cross"; }
    virtual void on_connect( const network::tcp_socket_ptr& s );
    virtual void on_new_msg( const network::tcp_socket_ptr& s, const network::message_t& msg );
    virtual void on_close( const network::tcp_socket_ptr& s );
};

#endif
