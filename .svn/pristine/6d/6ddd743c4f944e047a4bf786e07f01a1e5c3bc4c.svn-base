#ifndef __NETWORK_IO_BASE_HPP__
#define __NETWORK_IO_BASE_HPP__

#include <boost/asio.hpp>
#include "message.hpp"

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
    //connector do not need;
    virtual void on_new_accept(const tcp_socket_ptr& s) {} 
};

class io_handler_t
{
public:
	explicit io_handler_t(boost::asio::io_service& ios, io_callback_t* io_callback) : m_ios(ios), m_io_callback(io_callback) {};
	virtual ~io_handler_t() = default;

	virtual void on_read_error(const tcp_socket_ptr& s, const boost::system::error_code& ec) = 0;
	virtual void on_write_error(const tcp_socket_ptr& s, const boost::system::error_code& ec) = 0;
	virtual void on_del_socket(const tcp_socket_ptr& s, bool is_del_forced) = 0;
	virtual void on_socket_close(const tcp_socket_ptr& s) = 0;
	virtual void on_new_msg(const tcp_socket_ptr& s, const message_t& msg);

public:
	virtual void stop() = 0;
	virtual void clear() = 0;

protected:
	boost::asio::io_service& m_ios;
	io_callback_t* m_io_callback = NULL;
};

NS_NETWORK_END
#endif //__NETWORK_IO_BASE_HPP__
