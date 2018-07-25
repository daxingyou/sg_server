#ifndef __LOG_CENTER_LISTENER_HPP__
#define __LOG_CENTER_LISTENER_HPP__

#include "network.hpp"

class center_listener_t
    : public network::acceptor_callback_t
{
public:
    center_listener_t() {}
    virtual ~center_listener_t() {}

    virtual const char* name() { return "center"; }
    virtual void on_new_accept(const network::tcp_socket_ptr& s);
    virtual void on_new_msg(const network::tcp_socket_ptr& s, const network::message_t& msg);
    virtual void on_close(const network::tcp_socket_ptr& s);

public:
    inline network::tcp_socket_ptr get_socket() const { return m_center_socket.lock(); }
    inline void set_socket(const network::tcp_socket_ptr& s) { m_center_socket = s; }
    inline void close_socket() { m_center_socket.reset(); }

private:
    network::tcp_socket_wptr m_center_socket;
};

#endif
