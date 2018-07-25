#include "network.hpp"
#include "log.hpp"
#include <boost/asio.hpp>
#include <boost/chrono.hpp>
#include <boost/thread.hpp>

NS_NETWORK_BEGIN
USING_NS_COMMON;

network_t::network_t()
    : m_strand(m_network_io_service)
{
}

network_t::~network_t()
{
}

bool network_t::init(size_t thread_num, const std::vector<std::string>& ip_white_list)
{
    m_work.reset(new boost::asio::io_service::work(m_network_io_service));
    for (size_t i = 0; i < thread_num; ++i)
    {
        m_thread_group.create_thread(boost::bind(&boost::asio::io_service::run, &m_network_io_service));
    }

    if (!ip_white_list.empty())
    {
        m_ip_white_list = ip_white_list;
    }

    return true;
}

void network_t::close()
{
    close_all_listen();
    close_all_connect();

    m_network_io_service.stop();
    //m_network_io_service.stop();
    m_work.reset();

    m_thread_group.join_all();
}

//-----------------------------------------------------------------------------------------------------
bool network_t::listen_at(const std::string& ip, uint16_t port, acceptor_callback_t* acceptor_cb)
{
    listen_list_type::iterator itr = m_listen_list.find(port);
    if (itr != m_listen_list.end())
    {
        log_error("listen at port=%d duplicate!", port);
        return false;
    }

    acceptor_ptr_t new_acceptor(new acceptor_t(m_network_io_service, ip, port, acceptor_cb, m_ip_white_list));
    new_acceptor->listen();
    m_listen_list.insert(std::make_pair(port, new_acceptor));

    return true;
}

void network_t::close_listen(uint16_t port)
{
    listen_list_type::iterator itr = m_listen_list.find(port);
    if (itr != m_listen_list.end())
    {
        acceptor_ptr_t& listener = itr->second;
        if (NULL != listener)
        {
            listener->close();
        }
        m_listen_list.erase(itr);
        
    }
    else
    {
        log_error("listen at port[%d] is already closed!", port);
    }
}

void network_t::close_all_listen()
{
    listen_list_type::iterator itr = m_listen_list.begin();
    for (; itr != m_listen_list.end(); ++itr)
    {
        acceptor_ptr_t& listener = itr->second;
        if (NULL != listener)
        {
            listener->close();
        }
    }
    m_listen_list.clear();
}

//-----------------------------------------------------------------------------------------------------
bool network_t::connect_to(const std::string& ip, uint16_t port, connector_callback_t* connector_cb, uint32_t reconnect_interval_time, bool duplicate_conn, bool open_pingpong)
{
    //duplicate_conn’Î∂‘robot π”√
    if (!duplicate_conn)
    {
        size_t list_size = m_connect_list.size();
        for (size_t i = 0; i < list_size; ++i)
        {
            connector_ptr_t& conn = m_connect_list[i];
            if (conn && conn->get_conn_port() == port && 0 == strcmp(conn->get_conn_ip().c_str(), ip.c_str()))
            {
                log_error("repeat connect to the address[%s: %d]!", ip.c_str(), port);
                return false;
            }
        }
    }

    connector_ptr_t new_connector(new connector_t(m_network_io_service, ip, port, connector_cb, reconnect_interval_time, open_pingpong));
    new_connector->connect();
    m_connect_list.push_back(new_connector);

    return true;
}

void network_t::close_connect(uint16_t port)
{
    connect_list_type::iterator itr = m_connect_list.begin();
    connect_list_type::iterator itr_end = m_connect_list.end();
    for (; itr != itr_end; ++itr)
    {
        connector_ptr_t& conn = *itr;
        if (conn && conn->get_conn_port() == port)
        {
            conn->close();
            m_connect_list.erase(itr);
            return;
        }
    }

    log_error("connect to port[%d] is already closed!", port);
}

void network_t::close_connect(uint16_t port, const tcp_socket_ptr& s)
{
    connect_list_type::iterator itr = m_connect_list.begin();
    connect_list_type::iterator itr_end = m_connect_list.end();
    for (; itr != itr_end; ++itr)
    {
        connector_ptr_t& conn = *itr;
        if (conn && conn->get_conn_port() == port && conn->get_socket() == s)
        {
            conn->close();
            m_connect_list.erase(itr);
            return;
        }
    }
}

void network_t::close_all_connect()
{
    connect_list_type::iterator itr = m_connect_list.begin();
    connect_list_type::iterator itr_end = m_connect_list.end();
    for (; itr != itr_end; ++itr)
    {
        connector_ptr_t& conn = *itr;
        if (conn)
        {
            conn->close();
        }
    }
    m_connect_list.clear();
}

void network_t::post_send_msg(const tcp_socket_ptr& s, const msg_buf_ptr& buf)
{
    send_msg(s, buf);
}

void network_t::send_msg(const tcp_socket_ptr& s, const msg_buf_ptr& buf)
{
    if (NULL == s)
    {
        log_error("NULL == s");
        return;
    }
    
    s->write(buf);
}

void network_t::post_close_socket(const tcp_socket_ptr& s)
{
    close_socket(s);
}

void network_t::close_socket(const tcp_socket_ptr& s)
{
    if (NULL == s)
    {
        log_error("NULL == s");
        return;
    }
    s->close();
}
NS_NETWORK_END

