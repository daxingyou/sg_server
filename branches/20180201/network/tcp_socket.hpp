#ifndef __NETWORK_TCP_SOCKET_HPP__
#define __NETWORK_TCP_SOCKET_HPP__

#include <boost/array.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <string>
#include <deque>
#include "common/list.hpp"
#include "io_base.hpp"

NS_NETWORK_BEGIN

class tcp_socket_t : public boost::enable_shared_from_this<tcp_socket_t>
{
public:
    tcp_socket_t(const tcp_socket_t&);
    tcp_socket_t(boost::asio::io_service& ios, io_handler_t* handler);
    ~tcp_socket_t();
    tcp_socket_t& operator=(const tcp_socket_t&);

public:
    void read();
    void write(const msg_buf_ptr& buf);
    void close();

private:
    inline size_t another_index(size_t idx)
    {
        return idx ^= 1;
    }

    void handle_read(const boost::system::error_code& ec, size_t bytes);
    
    void do_write(const msg_buf_ptr& buf);
    void handle_write(const boost::system::error_code& ec, size_t bytes);
    void wrap_idle_buffers();
    bool rewrap_idle_buffers(size_t bytes);

    void do_close();

public:
    boost::asio::ip::tcp::socket& socket() { return m_inner_socket; }
    boost::asio::io_service::strand& strand() { return m_strand; }
    
    void set_bind_data(void* data){ m_bind_data = data; }
    void* get_bind_data(){ return m_bind_data; }

    bool is_closed() { return m_inner_socket.native() < 0; }

    void set_socket_id() { m_socket_id = m_inner_socket.native(); }
    int32_t get_socket_id() { return m_socket_id; }

public:
    void* m_iter = NULL;
    void* m_bind_data = NULL;
private:
    boost::asio::ip::tcp::socket m_inner_socket;
    boost::asio::io_service& m_io_service;
    boost::asio::io_service::strand m_strand;
    bool m_write_in_pending;
    size_t m_idle_index;
    io_handler_t* m_io_handler;
    std::vector<boost::asio::const_buffer> m_wrap_buf;
    std::deque<msg_buf_ptr> m_write_buf[2];
    message_parser_t m_msg_parser;
    boost::array<char, 1024> m_read_buf;
    int32_t m_socket_id = -1;
};
typedef boost::shared_ptr<tcp_socket_t> tcp_socket_ptr;
typedef boost::weak_ptr<tcp_socket_t> tcp_socket_wptr;

struct ITER_T
{
    ITER_T(list_t<tcp_socket_wptr>::type::iterator _i)
        : i(_i){}

    list_t<tcp_socket_wptr>::type::iterator i;
};

NS_NETWORK_END
#endif //__NETWORK_TCP_SOCKET_HPP__
