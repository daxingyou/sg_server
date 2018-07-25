#ifndef __LOG_ACCOUNT_LISTENER_HPP__
#define __LOG_ACCOUNT_LISTENER_HPP__

#include "network.hpp"

class account_listener_t
    : public network::acceptor_callback_t
{
public:
    account_listener_t() {}
    virtual ~account_listener_t() {}

    virtual const char* name() { return "account"; }
    virtual void on_new_accept(const network::tcp_socket_ptr& s);
    virtual void on_new_msg(const network::tcp_socket_ptr& s, const network::message_t& msg);
    virtual void on_close(const network::tcp_socket_ptr& s);

public:
    inline network::tcp_socket_ptr get_socket() const { return m_account_socket.lock(); }
    inline void set_socket(const network::tcp_socket_ptr& s) { m_account_socket = s; }
    inline void close_socket() { get_socket().reset(); }

private:
    network::tcp_socket_wptr m_account_socket;
};

#endif
