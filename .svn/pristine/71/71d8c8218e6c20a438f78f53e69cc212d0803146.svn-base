#ifndef __GATE_LOGIN_CONNECTOR_HPP__
#define __GATE_LOGIN_CONNECTOR_HPP__

#include "network.hpp"

class login_connector_t
    : public network::connector_callback_t
{
public:
    login_connector_t() {}
    virtual ~login_connector_t() {}

    virtual const char* name() { return "login"; }
    virtual void on_connect(const network::tcp_socket_ptr& s);
    virtual void on_new_msg(const network::tcp_socket_ptr& s, const network::message_t& msg);
    virtual void on_close(const network::tcp_socket_ptr& s );

public:
    void send_msg_to_login(const network::msg_buf_ptr& msg);

    inline network::tcp_socket_ptr get_socket() const { return m_login_socket.lock(); }
    inline void set_socket(const network::tcp_socket_ptr& s) { m_login_socket = s; }
    inline void close_socket() { m_login_socket.reset(); }

private:
    network::tcp_socket_wptr m_login_socket;
};

#endif
