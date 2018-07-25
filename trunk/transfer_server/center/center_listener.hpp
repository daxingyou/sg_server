#pragma once

#include "network.hpp"


class center_listener_t
    : public network::acceptor_callback_t
{
public:
    center_listener_t() {}
    virtual ~center_listener_t() {}

    virtual const char* name() { return "center"; }
    virtual void on_new_accept( const network::tcp_socket_ptr& s );
    virtual void on_new_msg( const network::tcp_socket_ptr& s, const network::message_t& msg );
    virtual void on_close( const network::tcp_socket_ptr& s );

public:
    void send_msg_to_center(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg);
};
