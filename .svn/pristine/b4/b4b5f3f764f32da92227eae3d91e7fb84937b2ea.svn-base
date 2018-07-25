#ifndef __NETWORK_IO_BASE_HPP__
#define __NETWORK_IO_BASE_HPP__

#include "common/common_fwd.hpp"
#include "message.hpp"
#include <boost/asio.hpp>

NS_NETWORK_BEGIN

class tcp_socket_t;
typedef boost::shared_ptr<tcp_socket_t> tcp_socket_ptr;

class io_callback_t
{
public:
    io_callback_t() {}
    virtual ~io_callback_t() {}

    virtual const char* name() = 0;
    virtual void on_new_accept(const tcp_socket_ptr& s) = 0;
    virtual void on_new_msg(const tcp_socket_ptr& s, const message_t& msg) = 0;
    virtual void on_connect(const tcp_socket_ptr& s) = 0;
    virtual void on_close(const tcp_socket_ptr& s) = 0;
    virtual bool on_read(const tcp_socket_ptr& s, char* data, size_t bytes) = 0;
};

class acceptor_callback_t : public io_callback_t
{
public:
    acceptor_callback_t() {}
    virtual ~acceptor_callback_t() {}

    virtual const char* name() = 0;
    virtual void on_new_accept(const tcp_socket_ptr& s) = 0;
    virtual void on_new_msg(const tcp_socket_ptr& s, const message_t& msg) = 0;
    virtual void on_close(const tcp_socket_ptr& s) = 0;
    virtual bool on_read(const tcp_socket_ptr& s, char* data, size_t bytes) { return false; }//此处一定return false; 参见tcp_socket.cpp:65
    //acceptor do not need;
    virtual void on_connect(const tcp_socket_ptr& s) {} 
};

class connector_callback_t : public io_callback_t
{
public:
    connector_callback_t() {}
    virtual ~connector_callback_t() {}

    virtual const char* name() = 0;
    virtual void on_connect(const tcp_socket_ptr& s) = 0;
    virtual void on_new_msg(const tcp_socket_ptr& s, const message_t& msg) = 0;
    virtual void on_close(const tcp_socket_ptr& s) = 0;
    virtual bool on_read(const tcp_socket_ptr& s, char* data, size_t bytes) { return false; }//此处一定return false; 参见tcp_socket.cpp:65
    //connector do not need;
    virtual void on_new_accept(const tcp_socket_ptr& s) {} 
};

class io_handler_t
{
public:
    io_handler_t(boost::asio::io_service& ios, io_callback_t* io_callback);
    virtual ~io_handler_t();

    virtual void on_connect_error(const tcp_socket_ptr& s, const boost::system::error_code& ec);
    virtual void on_read_error(const tcp_socket_ptr& s, const boost::system::error_code& ec) = 0;
    virtual void on_write_error(const tcp_socket_ptr& s, const boost::system::error_code& ec) = 0;
    virtual void on_new_msg(const tcp_socket_ptr& s, const message_t& msg);
    virtual bool on_read(const tcp_socket_ptr& s, char* data, size_t bytes) { return m_io_callback->on_read(s, data, bytes); }

	virtual void close();
    boost::asio::io_service& m_ios;
    io_callback_t* m_io_callback;
};

NS_NETWORK_END
#endif //__NETWORK_IO_BASE_HPP__
