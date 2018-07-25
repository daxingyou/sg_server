#ifndef __NETWORK_CONNECTOR_HPP__
#define __NETWORK_CONNECTOR_HPP__

#include "io_base.hpp"
#include "tcp_socket.hpp"

NS_NETWORK_BEGIN

class connector_t
	: public io_handler_t
	, public boost::enable_shared_from_this<connector_t>
	, private boost::noncopyable
{
public:
	explicit connector_t(boost::asio::io_service& ios, const std::string& conn_ip, uint16_t conn_port, io_callback_t* io_callback, uint32_t reconnect_interval_time, bool open_pingpong);
    virtual ~connector_t() = default;

public:	
	void on_read_error(const tcp_socket_ptr& s, const boost::system::error_code& ec);
	void on_write_error(const tcp_socket_ptr& s, const boost::system::error_code& ec);
	void on_del_socket(const tcp_socket_ptr& s, bool is_del_forced);
	void on_socket_close(const tcp_socket_ptr& s);

	void stop();
	void clear();

public:
	const std::string& get_conn_ip() { return m_conn_ip; }
	uint16_t get_conn_port() { return m_conn_port; }

protected:
	void connect();
	void reconnect();
	void pingpong();

protected:
	boost::asio::io_service::strand m_strand;
	boost::asio::ip::tcp::socket m_new_socket;
	tcp_socket_ptr m_conn_tcp_socket;
    std::string m_conn_ip;
    uint16_t m_conn_port;
    uint32_t m_reconnect_interval_time;
    bool m_open_pingpong;
    boost::asio::deadline_timer m_timer;
	bool m_is_open = true;
};

typedef boost::shared_ptr<connector_t> connector_ptr_t;

NS_NETWORK_END
#endif
