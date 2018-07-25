#ifndef __NETWORK_NETWORK_HPP__
#define __NETWORK_NETWORK_HPP__

#include "acceptor.hpp"
#include "connector.hpp"
#include <map>
#include <vector>
#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>


NS_NETWORK_BEGIN


class network_t
{
public:
    network_t();
    ~network_t();
    
    bool init(size_t thread_num, const std::vector<std::string>& ip_white_list);
    void close();

    bool listen_at(const std::string& ip, uint16_t port, acceptor_callback_t* acceptor_cb);
    void close_listen(uint16_t port);
    void close_all_listen();

    bool connect_to(const std::string& ip, uint16_t port, connector_callback_t* connector_cb, uint32_t reconnect_interval_time, bool duplicate_conn=false, bool open_pingpong = true);
    void close_connect(uint16_t port);
    void close_connect(uint16_t port, const network::tcp_socket_ptr& s);
    void close_all_connect();

    void post_send_msg(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& buf);
    void send_msg(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& buf);

    void post_close_socket(const network::tcp_socket_ptr& s);
    void close_socket(const network::tcp_socket_ptr& s);
private:
    boost::asio::io_service m_network_io_service;
    boost::asio::io_service::strand m_strand;
    boost::scoped_ptr<boost::asio::io_service::work> m_work;
    boost::thread_group m_thread_group;

    typedef std::map<uint16_t, acceptor_ptr_t> listen_list_type;
    listen_list_type m_listen_list;

    typedef std::vector<connector_ptr_t> connect_list_type;
    connect_list_type m_connect_list;

    std::vector<std::string> m_ip_white_list;
};


NS_NETWORK_END
#endif //__NETWORK_NETWORK_HPP__
