#ifndef __GM_CENTER_CONNECTOR_HPP__
#define __GM_CENTER_CONNECTOR_HPP__

#include "network.hpp"
#include "main/gm_server_fwd.hpp"
#include "common.pb.h"
#include "gm.pb.h"

class center_connector_t
    : public network::connector_callback_t
{
public:
    center_connector_t() {}
    virtual ~center_connector_t() {}

    virtual const char* name() { return "center"; }
    virtual void on_connect(const network::tcp_socket_ptr& s);
    virtual void on_new_msg(const network::tcp_socket_ptr& s, const network::message_t& msg);
    virtual void on_close(const network::tcp_socket_ptr& s );

public:
    void send_msg_to_center(const network::msg_buf_ptr& msg);

    inline network::tcp_socket_ptr get_socket() const { return m_center_socket.lock(); }
    inline void set_socket(const network::tcp_socket_ptr& s) { m_center_socket = s; }
    inline void close_socket() { m_center_socket.reset(); }

private:
    network::tcp_socket_wptr m_center_socket;
};

#endif
