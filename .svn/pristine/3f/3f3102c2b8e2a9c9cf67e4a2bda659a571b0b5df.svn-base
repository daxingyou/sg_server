#ifndef __GAME_GATE_CONNECTOR_HPP__
#define __GAME_GATE_CONNECTOR_HPP__

#include "network.hpp"

class gate_connector_t
    : public network::connector_callback_t
{
public:
    gate_connector_t() {}
    virtual ~gate_connector_t() {}

    virtual const char* name() { return "gate"; }
    virtual void on_connect(const network::tcp_socket_ptr& s);
    virtual void on_new_msg(const network::tcp_socket_ptr& s, const network::message_t& msg);
    virtual void on_close(const network::tcp_socket_ptr& s );
};

#endif
