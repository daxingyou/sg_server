#ifndef __GM_ADMIN_LISTENER_HPP__
#define __GM_ADMIN_LISTENER_HPP__

#include "network.hpp"
#include "log.hpp"
#include <assert.h>

/**
 * 描述: 用于监听GM Tool的连接以及消息
 * 备注: GM Tool的连接是短连接, 一次请求-响应完成之后即由GM Tool主动关闭tcp连接,
 */
class admin_listener_t
    : public network::acceptor_callback_t
{
public:
    admin_listener_t() {}
    virtual ~admin_listener_t() {}

    virtual const char* name() { return "gm"; }
    virtual void on_new_accept( const network::tcp_socket_ptr& s );
    virtual void on_new_msg( const network::tcp_socket_ptr& s, const network::message_t& msg );
    virtual void on_close( const network::tcp_socket_ptr& s );

public:
    void send_msg_to_gm(const network::msg_buf_ptr& msg);

    inline network::tcp_socket_ptr get_socket() const { return m_admin_socket.lock(); }
    inline void set_socket(const network::tcp_socket_ptr& s) { m_admin_socket = s; }
    inline void close_socket() { get_socket().reset(); }

private:
    network::tcp_socket_wptr m_admin_socket;
};

#endif
