#ifndef __CENTER_GAME_LISTENER_HPP__
#define __CENTER_GAME_LISTENER_HPP__

#include "network.hpp"

class game_listener_t
    : public network::acceptor_callback_t
{
public:
    game_listener_t() {}
    virtual ~game_listener_t() {}

    virtual const char* name() { return "game"; }
    virtual void on_new_accept( const network::tcp_socket_ptr& s );
    virtual void on_new_msg( const network::tcp_socket_ptr& s, const network::message_t& msg );
    virtual void on_close( const network::tcp_socket_ptr& s );

public:
    void send_msg_to_game(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg);
};

#endif
