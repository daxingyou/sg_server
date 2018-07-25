#ifndef __NETWORK_ACCEPTOR_HPP__
#define __NETWORK_ACCEPTOR_HPP__

#include "tcp_socket.hpp"

NS_NETWORK_BEGIN

class acceptor_t
    : public io_handler_t
    , public boost::enable_shared_from_this<acceptor_t>
    , private boost::noncopyable
{
public:
    acceptor_t(boost::asio::io_service& ios, const std::string& listen_ip, uint16_t listen_port, io_callback_t* io_callback, const std::vector<std::string>& ip_white_list);
    virtual ~acceptor_t();

    virtual void on_connect_error( const tcp_socket_ptr& s, const boost::system::error_code& ec );
    virtual void on_read_error( const tcp_socket_ptr& s, const boost::system::error_code& ec );
    virtual void on_write_error( const tcp_socket_ptr& s, const boost::system::error_code& ec );

    void listen();
    void accecpt();
    virtual void close();

private:
    static void handle_accept(const boost::weak_ptr<acceptor_t>& p_wacceptor, const tcp_socket_ptr& s, const boost::system::error_code& ec);

private:
    bool is_on_white_list(const std::string& connect_ip);

private:
    boost::asio::ip::tcp::acceptor m_tcp_acceptor;
    std::string m_listen_ip;
    uint16_t m_listen_port;
    std::vector<std::string> m_ip_white_list;
};

typedef boost::shared_ptr<acceptor_t> acceptor_ptr_t;

NS_NETWORK_END
#endif
