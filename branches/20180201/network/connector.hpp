#ifndef __NETWORK_CONNECTOR_HPP__
#define __NETWORK_CONNECTOR_HPP__

#include "tcp_socket.hpp"

NS_NETWORK_BEGIN

#define SERVER_PINGPONG_REQUEST 99

class connector_t
    : public io_handler_t
    , public boost::enable_shared_from_this<connector_t>
    , private boost::noncopyable
{
public:
    connector_t(boost::asio::io_service& ios, const std::string& conn_ip, uint16_t conn_port, io_callback_t* callback, uint32_t reconnect_interval_time, bool open_pingpong);
    virtual ~connector_t();

    virtual void on_connect_error(const tcp_socket_ptr& s, const boost::system::error_code& ec);
    virtual void on_read_error(const tcp_socket_ptr& s, const boost::system::error_code& ec);
    virtual void on_write_error(const tcp_socket_ptr& s, const boost::system::error_code& ec);

public:
    void connect();
    void reconnect();
	void pingpong();
    virtual void close();

private:
    static void handle_connect(const boost::weak_ptr<connector_t>& p_wconnector, const boost::system::error_code& ec);
    static void handle_reconnect(const boost::weak_ptr<connector_t>& p_wconnector, const boost::system::error_code& ec);
    static void handle_pingpong(const boost::weak_ptr<connector_t>& p_wconnector, const boost::system::error_code& ec);


public:
    const std::string& get_conn_ip() { return m_conn_ip; }
    uint16_t get_conn_port() { return m_conn_port; }
    const tcp_socket_ptr& get_socket() { return m_socket; }

private:
    tcp_socket_ptr m_socket;
    boost::asio::io_service::strand m_strand;
    std::string m_conn_ip;
    uint16_t m_conn_port;
    uint32_t m_reconnect_interval_time;
    bool m_open_pingpong = true;
    boost::asio::deadline_timer m_reconnect_timer;
	boost::asio::deadline_timer m_pingpong_timer;
};

typedef boost::shared_ptr<connector_t> connector_ptr_t;

NS_NETWORK_END
#endif
