#ifndef __DB_LOGIN_LISTENER_HPP__
#define __DB_LOGIN_LISTENER_HPP__

#include "network.hpp"

class db_listener_t
    : public network::acceptor_callback_t
{
public:
	db_listener_t() {}
    virtual ~db_listener_t() {}

    virtual const char* name() { return "db"; }
    virtual void on_new_accept( const network::tcp_socket_ptr& s );
    virtual void on_new_msg( const network::tcp_socket_ptr& s, const network::message_t& msg );
    virtual void on_close( const network::tcp_socket_ptr& s );
};

#endif
