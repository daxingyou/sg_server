#ifndef __CHAT_TRANSFER_CONNECTOR_HPP__
#define __CHAT_TRANSFER_CONNECTOR_HPP__

#include "network.hpp"

class transfer_connector_t
    : public network::connector_callback_t
{
public:
    transfer_connector_t() {}
    virtual ~transfer_connector_t() {}

    virtual const char* name() { return "transfer"; }
    virtual void on_connect(const network::tcp_socket_ptr& s);
    virtual void on_new_msg(const network::tcp_socket_ptr& s, const network::message_t& msg);
    virtual void on_close(const network::tcp_socket_ptr& s );

public:
    void send_msg_to_transfer(const network::msg_buf_ptr& msg);

    inline network::tcp_socket_ptr get_socket() const { return m_transfer_socket.lock(); }
    inline void set_socket(const network::tcp_socket_ptr& s) { m_transfer_socket = s; }
    inline void close_socket() { m_transfer_socket.reset(); }

private:
    network::tcp_socket_wptr m_transfer_socket;
};

#endif
