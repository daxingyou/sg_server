#include "connector.hpp"
#include "log.hpp"
#include "protobuf.hpp"
#include "server.pb.h"

NS_NETWORK_BEGIN
USING_NS_COMMON;

#define SERVER_PINGPONG_INTERVAL_TIME 180 //second

connector_t::connector_t(boost::asio::io_service& ios, const std::string& conn_ip, uint16_t conn_port, io_callback_t* callback, uint32_t reconnect_interval_time, bool open_pingpong)
    : io_handler_t(ios, callback)
    , m_socket(new tcp_socket_t(ios, this))
    , m_strand(ios)
    , m_conn_ip(conn_ip)
    , m_conn_port(conn_port)
    , m_reconnect_interval_time(reconnect_interval_time)
    , m_open_pingpong(open_pingpong)
    , m_reconnect_timer(ios)
	, m_pingpong_timer(ios)
{
}

connector_t::~connector_t()
{
}

void connector_t::on_connect_error(const tcp_socket_ptr& s, const boost::system::error_code& ec)
{
    if (NULL == s)
    {
        log_error("connect to %s[%s: %d] failed, error[%d, %s]", m_io_callback->name(), m_conn_ip.c_str(), m_conn_port, ec.value(), ec.message().c_str());
        return;
    }

    try
    {
        log_error("connect to %s[%s: %d: %d] failed, error[%d, %s]", m_io_callback->name(), m_conn_ip.c_str(), m_conn_port, s->socket().native(), ec.value(), ec.message().c_str());
    }
    catch (boost::system::system_error &ec)
    {
        log_error("connect to %s[%s: %d] failed, error[%s]", m_io_callback->name(), m_conn_ip.c_str(), m_conn_port, ec.what());
    }

    reconnect();
}

void connector_t::on_read_error(const tcp_socket_ptr& s, const boost::system::error_code& ec)
{
    if (NULL == s)
    {
        log_error("disconnect to %s[%s: %d], error[%d, %s]", m_io_callback->name(), m_conn_ip.c_str(), m_conn_port, ec.value(), ec.message().c_str());
        return;
    }

    try
    {
        log_error("disconnect to %s[%s: %d: %d], error[%d, %s]", m_io_callback->name(), m_conn_ip.c_str(), m_conn_port, s->socket().native(), ec.value(), ec.message().c_str());
    }
    catch (boost::system::system_error &ec)
    {
        log_error("disconnect to %s[%s: %d], error[%s]", m_io_callback->name(), m_conn_ip.c_str(), m_conn_port, ec.what());
    }

    m_io_callback->on_close(s);
    reconnect();
}

void connector_t::on_write_error(const tcp_socket_ptr& s, const boost::system::error_code& ec)
{
    if (NULL == s)
    {
        log_error("disconnect to %s[%s: %d], error[%d, %s]", m_io_callback->name(), m_conn_ip.c_str(), m_conn_port, ec.value(), ec.message().c_str());
        return;
    }

    try
    {
        log_error("disconnect to %s[%s: %d: %d], error[%d, %s]", m_io_callback->name(), m_conn_ip.c_str(), m_conn_port, s->socket().native(), ec.value(), ec.message().c_str());
    }
    catch (boost::system::system_error &ec)
    {
        log_error("disconnect to %s[%s: %d], error[%s]", m_io_callback->name(), m_conn_ip.c_str(), m_conn_port, ec.what());
    }

    m_io_callback->on_close(s);
    reconnect();
}

void connector_t::connect()
{
    if (NULL == m_socket)
    {
        log_error("m_socket is null");
        return;
    }

    try
    {
        boost::system::error_code ec;
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(m_conn_ip), m_conn_port);
		//m_socket->socket().set_option(boost::asio::socket_base::send_buffer_size(80000));
		//m_socket->socket().set_option(boost::asio::ip::tcp::no_delay(false));
        m_socket->socket().async_connect(ep, boost::bind(&connector_t::handle_connect, boost::weak_ptr<connector_t>(shared_from_this()), boost::asio::placeholders::error));
    }
    catch (boost::system::system_error &ec)
    {
        log_error("connect throw error[%s]", ec.what());
        return;
    }
}

void connector_t::handle_connect(const boost::weak_ptr<connector_t>& p_wconnector, const boost::system::error_code& ec)
{
    connector_ptr_t p_connector(p_wconnector.lock());
    if (NULL == p_connector)
    {
        //log_error("NULL == p_connector");
        return;
    }

    if (NULL == p_connector->m_socket)
    {
        log_error("m_socket is null");
        return;
    }

    if (ec)
    {
        p_connector->on_connect_error(p_connector->m_socket, ec);
        return;
    }
    
    try
    {
        p_connector->m_socket->set_socket_id();
        if (0 != strcmp(p_connector->m_io_callback->name(), "robot"))
        {
            log_info("connect to %s[%s: %d: %d] success!", p_connector->m_io_callback->name(), p_connector->m_conn_ip.c_str(), p_connector->m_conn_port, p_connector->m_socket->socket().native());
        }
    }
    catch (boost::system::system_error &ec)
    {
        log_error("throw exception: %s", ec.what());
        return;
    }

    p_connector->m_io_callback->on_connect(p_connector->m_socket);
    p_connector->m_socket->read();

    p_connector->pingpong();

    return;
}

void connector_t::close()
{
	io_handler_t::close();
    m_reconnect_timer.cancel();
    m_pingpong_timer.cancel();

    if (NULL != m_socket)
    {
        m_socket->close();
    }
}

void connector_t::reconnect()
{
    if (m_reconnect_interval_time == 0)
    {
        return;
    }
    m_reconnect_timer.expires_from_now(boost::posix_time::seconds(m_reconnect_interval_time));
    m_reconnect_timer.async_wait(m_strand.wrap(boost::bind(&connector_t::handle_reconnect, boost::weak_ptr<connector_t>(shared_from_this()), boost::asio::placeholders::error)));
}

void connector_t::handle_reconnect(const boost::weak_ptr<connector_t>& p_wconnector, const boost::system::error_code& ec)
{
    connector_ptr_t p_connector(p_wconnector.lock());
    if (NULL == p_connector)
    {
        //log_error("NULL == p_connector");
        return;
    }

    if (ec == boost::asio::error::operation_aborted)
    {
        
        return;
    }
    else if (ec)
    {
        log_error("Timer error: %s", ec.message().c_str());
        return;
    }

    if (NULL != p_connector->m_socket && p_connector->m_socket->is_closed())
    {
        p_connector->m_socket.reset(new tcp_socket_t(p_connector->m_ios, p_connector.get()));
    }

    p_connector->connect();
}

void connector_t::pingpong()
{
    if (!m_open_pingpong)
    {
        return;
    }
    m_pingpong_timer.expires_from_now(boost::posix_time::seconds(SERVER_PINGPONG_INTERVAL_TIME));
    m_pingpong_timer.async_wait(m_strand.wrap(boost::bind(&connector_t::handle_pingpong, boost::weak_ptr<connector_t>(shared_from_this()), boost::asio::placeholders::error)));
}

void connector_t::handle_pingpong(const boost::weak_ptr<connector_t>& p_wconnector, const boost::system::error_code& ec)
{
    connector_ptr_t p_connector(p_wconnector.lock());
    if (NULL == p_connector)
    {
        //log_error("NULL == p_connector");
        return;
    }

    if (ec == boost::asio::error::operation_aborted)
    {
        
        return;
    }
    else if (ec)
    {
        log_error("Timer error: %s", ec.message().c_str());
        return;
    }

    if (NULL == p_connector->m_socket)
    {
        return;
    }

    proto::server::server_pingpong_request req;
    network::msg_buf_ptr send_buf = network::wrap_msg<proto::server::server_pingpong_request>(SERVER_PINGPONG_REQUEST, 0, req);
    if (NULL == send_buf)
    {
        log_error("NULL == send_buf cmd:%d", SERVER_PINGPONG_REQUEST);
        return;
    }
    p_connector->m_socket->write(send_buf);
    //if (0 == strcmp(p_connector->m_io_callback->name(), "cross"))
    //{
    //    log_warn("send pingpong to cross");
    //}
    p_connector->pingpong();
}

NS_NETWORK_END
