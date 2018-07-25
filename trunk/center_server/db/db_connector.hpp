#ifndef __CENTER_DB_CONNECTOR_HPP__
#define __CENTER_DB_CONNECTOR_HPP__

#include "network.hpp"

class db_connector_t
    : public network::connector_callback_t
{
public:
    db_connector_t() {}
    virtual ~db_connector_t() {}

    virtual const char* name() { return "db"; }
    virtual void on_connect(const network::tcp_socket_ptr& s);
    virtual void on_new_msg(const network::tcp_socket_ptr& s, const network::message_t& msg);
    virtual void on_close(const network::tcp_socket_ptr& s );

public:
    void send_msg_to_db(const network::msg_buf_ptr& msg);

    inline network::tcp_socket_ptr get_socket() const { return m_db_socket.lock(); }
    inline void set_socket(const network::tcp_socket_ptr& s) { m_db_socket = s; }
    inline void reset_socket() { m_db_socket.reset();  }

private:
    network::tcp_socket_wptr m_db_socket;
};

#endif
