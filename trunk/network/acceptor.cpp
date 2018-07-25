#include "acceptor.hpp"
#include "log.hpp"
#include "tcp_socket.hpp"

NS_NETWORK_BEGIN
USING_NS_COMMON;

acceptor_t::acceptor_t(boost::asio::io_service& ios, const std::string& listen_ip, uint16_t listen_port, io_callback_t* io_callback, const std::vector<std::string>& ip_white_list)
    : io_handler_t(ios, io_callback)
	, m_strand(ios)
    , m_tcp_acceptor(ios)
	, m_new_socket(ios)
    , m_listen_ip(listen_ip)
    , m_listen_port(listen_port)
    , m_ip_white_list(ip_white_list)
{
	
}

bool acceptor_t::listen()
{
    try {
        boost::asio::ip::tcp::resolver res(m_ios);
        //boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(m_listen_ip), m_listen_port);
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string("0.0.0.0"), m_listen_port);
        m_tcp_acceptor.open(ep.protocol());
		m_tcp_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        m_tcp_acceptor.bind(ep);
        m_tcp_acceptor.listen();
		accecpt();
        log_info("listen for %s at[%s:%u]......", m_io_callback->name(), m_listen_ip.c_str(), m_listen_port);
    } catch (boost::system::system_error &ec) {
        log_error("listen at[%s:%u] error: %s", m_listen_ip.c_str(), m_listen_port, ec.what());
        COMMON_ASSERT(false);
        return false;
    }

	return true;
}

void acceptor_t::accecpt()
{
	m_tcp_acceptor.async_accept(m_new_socket, m_new_endpoint,
		m_strand.wrap([this](boost::system::error_code ec) 
	{
		if (ec == boost::asio::error::operation_aborted) { // 主动取消
			return;
		}

		if (!m_tcp_acceptor.is_open()) {
			return;
		}

		if (!ec) {
			do {
				std::string connect_ip = m_new_endpoint.address().to_string();
				uint32_t connect_port = m_new_endpoint.port();

				if (0 != strcmp(m_io_callback->name(), "client")) {
					if (!is_on_white_list(connect_ip)) {
						log_error("illegal %s ip[%s] connection, need to close it!", m_io_callback->name(), connect_ip.c_str());
						boost::system::error_code ignored_ec;
						m_new_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
						m_new_socket.close(ignored_ec);
						break;
					}
				}

				log_debug("new %s[%s: %u, %d] connected!", m_io_callback->name(), connect_ip.c_str(), connect_port, m_new_socket.native());

				tcp_socket_ptr new_connector(new tcp_socket_t(m_ios, std::move(m_new_socket), m_new_endpoint, this));
				m_tcp_socket_list.push_back(new_connector);
				m_io_callback->on_new_accept(new_connector);
				new_connector->read();
			} while (0);
		}

		accecpt();
	}));
}

void acceptor_t::stop()
{
	m_strand.post([this]() {
		if (!m_tcp_acceptor.is_open()) {
			return;
		}

		boost::system::error_code ignored_ec;
		m_tcp_acceptor.close(ignored_ec);
		log_info("listen port[%d] closed!", m_listen_port);
	});
}

bool acceptor_t::is_on_white_list(const std::string& connect_ip)
{
    std::vector<std::string>::iterator it = std::find(m_ip_white_list.begin(), m_ip_white_list.end(), connect_ip);
    if (it != m_ip_white_list.end()) {
        return true;
    }

    return false;
}

void acceptor_t::on_read_error(const tcp_socket_ptr& s, const boost::system::error_code& ec)
{
	if (2 == ec.value() && 0 == strcmp(m_io_callback->name(), "client")) {
		log_debug("%s[%s: %d] has been disconnected, error[%d, %s]!", m_io_callback->name(), s->addr().address().to_string().c_str(), s->get_socket_id(), ec.value(), ec.message().c_str());
	} else {
		log_error("%s[%s: %d] has been disconnected, error[%d, %s]!", m_io_callback->name(), s->addr().address().to_string().c_str(), s->get_socket_id(), ec.value(), ec.message().c_str());
	}
}

void acceptor_t::on_write_error(const tcp_socket_ptr& s, const boost::system::error_code& ec)
{
	if (2 == ec.value() && 0 == strcmp(m_io_callback->name(), "client")) {
		log_debug("%s[%s: %d] has been disconnected, error[%d, %s]!", m_io_callback->name(), s->addr().address().to_string().c_str(), s->get_socket_id(), ec.value(), ec.message().c_str());
	} else {
		log_error("%s[%s: %d] has been disconnected, error[%d, %s]!", m_io_callback->name(), s->addr().address().to_string().c_str(), s->get_socket_id(), ec.value(), ec.message().c_str());
	}
}

void acceptor_t::on_del_socket(const tcp_socket_ptr& s, bool is_del_forced)
{
	if (!s) {
		log_error("s == NULL");
		return;
	}

	m_strand.post([this, s, is_del_forced]() {
		tcp_socket_list::iterator itr = std::find_if(m_tcp_socket_list.begin(), m_tcp_socket_list.end(),
			[s](const tcp_socket_ptr& p_tcp_socket)
		{
			if (s == p_tcp_socket) return true;
			return false;
		});

		if (itr == m_tcp_socket_list.end()) {
			return;
		}

		s->close(is_del_forced);

		m_tcp_socket_list.erase(itr);
	});
}

void acceptor_t::on_socket_close(const tcp_socket_ptr& s)
{
	if (!s) {
		log_error("s == NULL");
		return;
	}

	m_strand.post([this, s]()
	{
		tcp_socket_list::iterator itr = std::find_if(m_tcp_socket_list.begin(), m_tcp_socket_list.end(),
			[s](const tcp_socket_ptr& p_tcp_socket)
		{
			if (s == p_tcp_socket) return true;
			return false;
		});

		if (itr == m_tcp_socket_list.end()) {
			return;
		}

		m_io_callback->on_close(s);

		m_tcp_socket_list.erase(itr);
	});
}

void acceptor_t::clear()
{
	m_strand.post([this]()
	{
		for (tcp_socket_list::iterator itr = m_tcp_socket_list.begin();
			itr != m_tcp_socket_list.end(); ++itr) {
			const tcp_socket_ptr& s = *itr;
			if (s) {
				s->close(true);
			}
		}

		m_tcp_socket_list.clear();
	});
}

NS_NETWORK_END
