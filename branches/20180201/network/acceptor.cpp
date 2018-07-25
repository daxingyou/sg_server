#include "acceptor.hpp"
#include "log.hpp"


NS_NETWORK_BEGIN
USING_NS_COMMON;

acceptor_t::acceptor_t(boost::asio::io_service& ios, const std::string& listen_ip, uint16_t listen_port, io_callback_t* io_callback, const std::vector<std::string>& ip_white_list)
    : io_handler_t(ios, io_callback)
    , m_tcp_acceptor(ios)
    , m_listen_ip(listen_ip)
    , m_listen_port(listen_port)
    , m_ip_white_list(ip_white_list)
{
}

acceptor_t::~acceptor_t()
{
}

void acceptor_t::on_connect_error( const tcp_socket_ptr& s, const boost::system::error_code& ec )
{
    if (NULL == s)
    {
        log_error("%s has been disconnected, error[NULL == s][%s]!", m_io_callback->name(), ec.value(), ec.message().c_str());
        return;
    }

    try
    {
        if (2 == ec.value() && 0 == strcmp(m_io_callback->name(), "client"))
        {
            log_debug("%s[%s: %d] has been disconnected, error[%d, %s]!", m_io_callback->name(), s->socket().remote_endpoint().address().to_string().c_str(), s->socket().native(), ec.value(), ec.message().c_str());
        }
        else
        {
            log_error("%s[%s: %d] has been disconnected, error[%d, %s]!", m_io_callback->name(), s->socket().remote_endpoint().address().to_string().c_str(), s->socket().native(), ec.value(), ec.message().c_str());
        }
    }
    catch (boost::system::system_error &ec)
    {
        log_error("%s has been disconnected, error[%s]!", m_io_callback->name(), ec.what());
    }

    m_io_callback->on_close(s);
}

void acceptor_t::on_read_error( const tcp_socket_ptr& s, const boost::system::error_code& ec )
{
    if (NULL == s)
    {
        log_error("%s has been disconnected, error[NULL == s][%s]!", m_io_callback->name(), ec.value(), ec.message().c_str());
        return;
    }

    try
    {
        if (2 == ec.value() && 0 == strcmp(m_io_callback->name(), "client"))
        {
            log_debug("%s[%s: %d] has been disconnected, error[%d, %s]!", m_io_callback->name(), s->socket().remote_endpoint().address().to_string().c_str(), s->socket().native(), ec.value(), ec.message().c_str());
        }
        else
        {
            log_error("%s[%s: %d] has been disconnected, error[%d, %s]!", m_io_callback->name(), s->socket().remote_endpoint().address().to_string().c_str(), s->socket().native(), ec.value(), ec.message().c_str());
        }
    }
    catch (boost::system::system_error &ec)
    {
        log_error("%s has been disconnected, error[%s]!", m_io_callback->name(), ec.what());
    }
    m_io_callback->on_close(s);
}

void acceptor_t::on_write_error( const tcp_socket_ptr& s, const boost::system::error_code& ec )
{
    if (NULL == s)
    {
        log_error("%s has been disconnected, error[NULL == s][%s]!", m_io_callback->name(), ec.value(), ec.message().c_str());
        return;
    }

    try
    {
        if (2 == ec.value() && 0 == strcmp(m_io_callback->name(), "client"))
        {
            log_debug("%s[%s: %d] has been disconnected, error[%d, %s]!", m_io_callback->name(), s->socket().remote_endpoint().address().to_string().c_str(), s->socket().native(), ec.value(), ec.message().c_str());
        }
        else
        {
            log_error("%s[%s: %d] has been disconnected, error[%d, %s]!", m_io_callback->name(), s->socket().remote_endpoint().address().to_string().c_str(), s->socket().native(), ec.value(), ec.message().c_str());
        }
    }
    catch (boost::system::system_error &ec)
    {
        log_error("%s has been disconnected, error[%s]!", m_io_callback->name(), ec.what());
    }
    m_io_callback->on_close(s);
}

void acceptor_t::listen()
{
    try
    {
        boost::asio::ip::tcp::resolver res(m_ios);
        //boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(m_listen_ip), m_listen_port);
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string("0.0.0.0"), m_listen_port);
        m_tcp_acceptor.open(ep.protocol());
		m_tcp_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		//m_socket->socket().set_option(boost::asio::ip::tcp::no_delay(false));
		//m_tcp_acceptor.set_option(boost::asio::socket_base::send_buffer_size(80000));
        m_tcp_acceptor.bind(ep);
        m_tcp_acceptor.listen();
        log_info("listen for %s at[%s:%u]......", m_io_callback->name(), m_listen_ip.c_str(), m_listen_port);
    }
    catch (boost::system::system_error &ec)
    {
        log_error("listen at[%s:%u] error: %s", m_listen_ip.c_str(), m_listen_port, ec.what());
        COMMON_ASSERT(false);
        return;
    }

    accecpt();
}

void acceptor_t::accecpt()
{
    if (!m_tcp_acceptor.is_open())
    {
        log_error("acceptor[%d] is not open!", m_listen_port);
        return;
    }

    tcp_socket_ptr new_connector(new tcp_socket_t(m_ios, this));

    m_tcp_acceptor.async_accept(new_connector->socket(), boost::bind(&acceptor_t::handle_accept, boost::weak_ptr<acceptor_t>(shared_from_this()), new_connector, boost::asio::placeholders::error));
}

void acceptor_t::handle_accept(const boost::weak_ptr<acceptor_t>& p_wacceptor, const tcp_socket_ptr& s, const boost::system::error_code& ec)
{
    acceptor_ptr_t p_acceptor(p_wacceptor.lock());
    if (NULL == p_acceptor)
    {
        log_error("NULL == p_acceptor");
        return;
    }

    if (ec)
    {
        log_error("accept error[%d, %s]", ec.value(), ec.message().c_str());
        return;
    }

    if (NULL != p_acceptor->m_io_callback && NULL != s)
    {
        try
        {
            std::string connect_ip = s->socket().remote_endpoint().address().to_string();
			uint32_t connect_port = s->socket().remote_endpoint().port();

            if (0 != strcmp(p_acceptor->m_io_callback->name(), "client"))
            {
                if (!p_acceptor->is_on_white_list(connect_ip))
                {
                    log_error("illegal %s ip[%s] connection, need to close it!", p_acceptor->m_io_callback->name(), connect_ip.c_str());
                    p_acceptor->m_io_callback->on_close(s);
                    p_acceptor->accecpt();
                    return;
                }

                log_info("new %s[%s: %u, %d] connected!", p_acceptor->m_io_callback->name(), connect_ip.c_str(), connect_port, s->socket().native());
            }
            else
            {
                log_debug("new %s[%s: %u, %d] connected!", p_acceptor->m_io_callback->name(), connect_ip.c_str(), connect_port, s->socket().native());
            }

            s->set_socket_id();
            p_acceptor->m_io_callback->on_new_accept(s);
            s->read();

            p_acceptor->accecpt();

        }
        catch (boost::system::system_error &ec)
        {
            log_error("throw exception: %s", ec.what());
            p_acceptor->accecpt();
            return;
        }
    }
}

void acceptor_t::close()
{
	io_handler_t::close();
    if (!m_tcp_acceptor.is_open())
    {
        return;
    }
    m_tcp_acceptor.close();
    log_info("listen port[%d] closed!", m_listen_port);
}

bool acceptor_t::is_on_white_list(const std::string& connect_ip)
{
    std::vector<std::string>::iterator it = std::find(m_ip_white_list.begin(), m_ip_white_list.end(), connect_ip);
    if (it != m_ip_white_list.end())
    {
        return true;
    }
    return false;
}

NS_NETWORK_END
