#ifndef __NETWORK_ACCEPTOR_HPP__
#define __NETWORK_ACCEPTOR_HPP__

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <list>

#include "io_base.hpp"

NS_NETWORK_BEGIN

class acceptor_t
	: public io_handler_t
	, public boost::enable_shared_from_this<acceptor_t>
    , private boost::noncopyable
{
public:
	explicit acceptor_t(boost::asio::io_service& ios, const std::string& listen_ip, uint16_t listen_port, io_callback_t* io_callback, const std::vector<std::string>& ip_white_list);
    virtual ~acceptor_t() = default;

public:
	void on_read_error(const tcp_socket_ptr& s, const boost::system::error_code& ec);
	void on_write_error(const tcp_socket_ptr& s, const boost::system::error_code& ec);
	void on_del_socket(const tcp_socket_ptr& s, bool is_del_forced);
	void on_socket_close(const tcp_socket_ptr& s);
    
	bool listen();
	void stop();
	void clear();

private:
	void accecpt();
    bool is_on_white_list(const std::string& connect_ip);

private:
	boost::asio::io_service::strand m_strand;
	boost::asio::ip::tcp::acceptor m_tcp_acceptor;
	boost::asio::ip::tcp::socket m_new_socket;
	boost::asio::ip::tcp::endpoint m_new_endpoint;
    std::string m_listen_ip;
    uint16_t m_listen_port;
    std::vector<std::string> m_ip_white_list;

private:
	typedef std::list<tcp_socket_ptr> tcp_socket_list;
	tcp_socket_list m_tcp_socket_list;
};

typedef boost::shared_ptr<acceptor_t> acceptor_ptr_t;

NS_NETWORK_END
#endif
