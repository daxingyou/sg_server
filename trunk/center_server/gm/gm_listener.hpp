#ifndef __CENTER_GM_LISTENER_HPP__
#define __CENTER_GM_LISTENER_HPP__

#include "network.hpp"

class gm_listener_t
    : public network::acceptor_callback_t
{
public:
    gm_listener_t() {}
    virtual ~gm_listener_t() {}

    virtual const char* name() { return "gm"; }
    virtual void on_new_accept( const network::tcp_socket_ptr& s );
    virtual void on_new_msg( const network::tcp_socket_ptr& s, const network::message_t& msg );
    virtual void on_close( const network::tcp_socket_ptr& s );

public:
    void send_msg_to_gm(const network::msg_buf_ptr& msg);

    inline network::tcp_socket_ptr get_socket() const { return m_gm_socket.lock(); }
    inline void set_socket(const network::tcp_socket_ptr& s) { m_gm_socket = s; }
    inline void close_socket() { m_gm_socket.reset(); }

private:
    network::tcp_socket_wptr m_gm_socket;
};

#endif
