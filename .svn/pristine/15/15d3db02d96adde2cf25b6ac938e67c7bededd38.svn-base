#include "connector.hpp"
#include "log.hpp"
#include "server.pb.h"
#include "protobuf.hpp"

NS_NETWORK_BEGIN
USING_NS_COMMON;

#define SERVER_PINGPONG_INTERVAL_TIME 180 //second

connector_t::connector_t(boost::asio::io_service& ios, const std::string& conn_ip, uint16_t conn_port, io_callback_t* io_callback, uint32_t reconnect_interval_time, bool open_pingpong)
	: io_handler_t(ios, io_callback)
	, m_strand(ios)
	, m_new_socket(ios)
	, m_conn_tcp_socket(NULL)
    , m_conn_ip(conn_ip)
    , m_conn_port(conn_port)
    , m_reconnect_interval_time(reconnect_interval_time)
    , m_open_pingpong(open_pingpong)
    , m_timer(ios)
{
	connect();
}

void connector_t::stop()
{
	m_strand.post([this]()
	{
		m_is_open = false; // 不会再重连

		if (!m_conn_tcp_socket) { // 未连接成功
			boost::system::error_code ignored_ec;
			m_new_socket.cancel(ignored_ec);
			m_timer.cancel();
		}
	});
}

void connector_t::connect()
{
	boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(m_conn_ip), m_conn_port);
	m_new_socket.async_connect(ep, m_strand.wrap([this, ep](boost::system::error_code ec)
	{
		if (ec) {
			log_error("connect to %s[%s: %u: %d] failed, error[%d, %s]", m_io_callback->name(), ep.address().to_string().c_str(), ep.port(), m_new_socket.native(), ec.value(), ec.message().c_str());
			boost::system::error_code ignored_ec;
			m_new_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
			m_new_socket.close(ignored_ec);

			if (ec == boost::asio::error::operation_aborted) { // 主动取消
				return;
			}

			reconnect();
		} else {
			log_debug("connect to %s[%s: %u, %d] success!", m_io_callback->name(), m_conn_ip.c_str(), m_conn_port, m_new_socket.native());

			m_conn_tcp_socket.reset(new tcp_socket_t(m_ios, std::move(m_new_socket), ep, this));
			m_io_callback->on_connect(m_conn_tcp_socket);
			m_conn_tcp_socket->read();
			pingpong();
		}
	}));
}

void connector_t::pingpong()
{
	if (!m_open_pingpong) {
		return;
	}

	m_timer.expires_from_now(boost::posix_time::seconds(SERVER_PINGPONG_INTERVAL_TIME));
	m_timer.async_wait(m_strand.wrap([this](boost::system::error_code ec) 
	{
		if (ec == boost::asio::error::operation_aborted) {
			return;
		} else if (ec) {
			log_error("Timer error: %s", ec.message().c_str());
			return;
		}

		if (!m_conn_tcp_socket) {
			return;
		}

		proto::server::server_pingpong_request pingpang;
		network::msg_buf_ptr send_buf = network::wrap_msg<proto::server::server_pingpong_request>(op_cmd::ss_pingpang_notify, 0, pingpang);
		if (NULL == send_buf) {
			log_error("NULL == send_buf cmd:%d", op_cmd::ss_pingpang_notify);
			return;
		}
		m_conn_tcp_socket->write(send_buf);

		pingpong();
	}));
}

void connector_t::reconnect()
{
	if (m_conn_tcp_socket || !m_is_open) {
		return;
	}

	if (m_reconnect_interval_time == 0) {
		return;
	}

	m_timer.expires_from_now(boost::posix_time::seconds(m_reconnect_interval_time));
	m_timer.async_wait(m_strand.wrap([this](boost::system::error_code ec)
	{
		if (ec == boost::asio::error::operation_aborted) {
			return;
		} else if (ec) {
			log_error("Timer error: %s", ec.message().c_str());
			return;
		}

		if (m_conn_tcp_socket || !m_is_open) {
			return;
		}

		connect();
	}));
}

void connector_t::on_read_error(const tcp_socket_ptr& s, const boost::system::error_code& ec)
{
	log_error("disconnect to %s[%s: %d: %d] failed, error[%d, %s]", m_io_callback->name(), m_conn_ip.c_str(), m_conn_port, s->get_socket_id(), ec.value(), ec.message().c_str());
}

void connector_t::on_write_error(const tcp_socket_ptr& s, const boost::system::error_code& ec)
{
	log_error("disconnect to %s[%s: %d: %d] failed, error[%d, %s]", m_io_callback->name(), m_conn_ip.c_str(), m_conn_port, s->get_socket_id(), ec.value(), ec.message().c_str());
}

void connector_t::on_del_socket(const tcp_socket_ptr& s, bool is_del_forced)
{
	if (!s) {
		log_error("s == NULL");
		return;
	}

	m_strand.post([this, s, is_del_forced]() {
		if (!m_conn_tcp_socket) {
			return;
		}

		if (s != m_conn_tcp_socket) {
			log_error("s != m_conn_tcp_socket error");
			return;
		}

		m_conn_tcp_socket->close(is_del_forced);
		m_conn_tcp_socket.reset();
		if (m_open_pingpong) {
			m_timer.cancel();
		}

		reconnect();
	});
}

void connector_t::on_socket_close(const tcp_socket_ptr& s)
{
	if (!s) {
		log_error("s == NULL");
		return;
	}

	m_strand.post([this, s]()
	{
		if (!m_conn_tcp_socket) {
			return;
		}

		if (s != m_conn_tcp_socket) {
			log_error("s != m_conn_tcp_socket error");
			return;
		}

		m_io_callback->on_close(s);
		m_conn_tcp_socket.reset();
		if (m_open_pingpong) {
			m_timer.cancel();
		}

		reconnect();
	});
}

void connector_t::clear()
{
	m_strand.post([this]()
	{
		if (m_conn_tcp_socket) {
			m_conn_tcp_socket->close(false);
			m_conn_tcp_socket.reset();
		}

		m_timer.cancel();
	});
}


NS_NETWORK_END
