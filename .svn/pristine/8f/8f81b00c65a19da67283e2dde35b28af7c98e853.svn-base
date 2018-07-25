#include "tcp_socket.hpp"
#include "log.hpp"
#include "io_base.hpp"
#include "op_cmd.hpp"

NS_NETWORK_BEGIN
USING_NS_COMMON;

tcp_socket_t::tcp_socket_t(boost::asio::io_service& ios, boost::asio::ip::tcp::socket socket, const boost::asio::ip::tcp::endpoint& addr, io_handler_t* io_handler)
	: m_strand(ios)
	, m_inner_socket(std::move(socket))
	, m_socket_id(m_inner_socket.native())
	, m_io_handler(io_handler)
	, m_addr(addr)
	, m_write_in_pending(false)
	, m_idle_index(0)
	, m_socket_state(socket_state::open)
	, m_is_close_notify(true)
{
	// ¹Ø±Õnagle
	m_inner_socket.set_option(boost::asio::ip::tcp::no_delay(true));
}

tcp_socket_t::~tcp_socket_t()
{
	if (m_inner_socket.is_open()) {
		log_error("addr[%s:%u:%d] socket is not close, when destruct",
			m_addr.address().to_string().c_str(), m_addr.port(), m_socket_id);
		boost::system::error_code ignored_ec;
		m_inner_socket.close(ignored_ec);
	}
}

void tcp_socket_t::read()
{
	auto self(shared_from_this());
	do_read(self);
}

void tcp_socket_t::do_read(const tcp_socket_ptr& self)
{
	m_inner_socket.async_read_some(boost::asio::buffer(m_read_buf),
		m_strand.wrap([this, self](boost::system::error_code ec, std::size_t bytes_transferred)
	{
		if (m_socket_state == socket_state::closed) {
			return;
		}

		if (ec) {
			if (ec != boost::asio::error::operation_aborted) {
				on_read_error(self, ec);
			}
		} else {
			if (m_socket_state == socket_state::open) {
				int32_t ret = m_msg_parser.parse(m_read_buf.begin(), bytes_transferred);
				if (ret == -1) {
					on_read_error(self, boost::system::errc::make_error_code(boost::system::errc::bad_message));
					return;
				} else if (ret > 0) {
					std::vector<message_t>& msg_vec = m_msg_parser.packs();
					for (size_t i = 0; i < msg_vec.size(); ++i) {
						m_io_handler->on_new_msg(self, msg_vec[i]);
					}
					msg_vec.clear();
				}
			}

			// read again
			do_read(self);
		}
	}));
}

void tcp_socket_t::write(const msg_buf_ptr& buf)
{
	auto self(shared_from_this());
	m_strand.post([this, self, buf]()
	{
		if (m_socket_state != socket_state::open) {
			log_warn("addr[%s:%u:%d] socket state[%u] != open when write!", m_addr.address().to_string().c_str(), m_addr.port(), m_socket_id, m_socket_state);
			return;
		}

		m_write_buf[m_idle_index].push_back(buf);
		do_write(self);
	});
}

void tcp_socket_t::do_write(const tcp_socket_ptr& self)
{
	if (!m_write_in_pending) {
		if (!m_write_buf[m_idle_index].empty()) {
			m_write_in_pending = true;
			wrap_idle_buffers();
			m_idle_index = another_index(m_idle_index);
			boost::asio::async_write(m_inner_socket, m_wrap_buf,
				m_strand.wrap([this, self](boost::system::error_code ec, std::size_t bytes)
			{
				if (m_socket_state == socket_state::closed) {
					return;
				}

				if (ec) {
					if (ec != boost::asio::error::operation_aborted) {
						on_write_error(self, ec);
					}
				} else {
					uint8_t pending_index = another_index(m_idle_index);
					m_write_buf[pending_index].clear();
					m_wrap_buf.clear();

					m_write_in_pending = false;
					do_write(self);
				}
			}));
		} else {
			if (m_socket_state == socket_state::shutdown_writing) {
				do_shutdown(self);
			}
		}
	}
}

void tcp_socket_t::do_shutdown(const tcp_socket_ptr& self)
{
	if (m_socket_state != socket_state::shutdown_writing) {
		log_error("socket state != shutdown_writing");
		return;
	}

	log_debug("addr[%s:%u:%d] socket shutdown write", m_addr.address().to_string().c_str(), m_addr.port(), m_socket_id);

	boost::system::error_code ignored_ec;
	m_inner_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ignored_ec);
	m_socket_state = socket_state::shutdown_write;
}

void tcp_socket_t::close(bool is_forced/* = true*/)
{
	auto self(shared_from_this());
	m_strand.post([this, self, is_forced]()
	{
		if (m_socket_state == socket_state::closed)
			return;

		if (is_forced) {
			m_is_close_notify = false;
			do_close(self);
		} else {
			if (m_socket_state == socket_state::shutdown_writing ||
				m_socket_state == socket_state::shutdown_write) {
				log_error("addr[%s:%u:%d] socket is already closing now!",
					m_addr.address().to_string().c_str(), m_addr.port(), m_socket_id);
				return;
			}

			m_is_close_notify = false;

			if (!m_write_in_pending) {
				do_close(self);
			} else {
				m_socket_state = socket_state::shutdown_writing;
			}
		}
	});
}

void tcp_socket_t::do_close(const tcp_socket_ptr& self)
{
	if (m_socket_state == socket_state::closed) {
		return;
	}

	m_socket_state = socket_state::closed;

	log_debug("addr[%s:%u:%d] socket closed", m_addr.address().to_string().c_str(), m_addr.port(), m_socket_id);

	boost::system::error_code ignored_ec;
	if (m_socket_state == socket_state::shutdown_write) {
		m_inner_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_receive, ignored_ec);
	} else {
		m_inner_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
	}

	m_inner_socket.close(ignored_ec);

	if (m_is_close_notify) m_io_handler->on_socket_close(self);
}

void tcp_socket_t::wrap_idle_buffers()
{
	m_wrap_buf.reserve(m_write_buf[m_idle_index].size());
	std::deque<msg_buf_ptr>::iterator itr = m_write_buf[m_idle_index].begin();
	for (; itr != m_write_buf[m_idle_index].end(); ++itr)
	{
		m_wrap_buf.push_back(boost::asio::buffer((*itr)->data(), (*itr)->size()));
	}
}

void tcp_socket_t::on_read_error(const tcp_socket_ptr& self, const boost::system::error_code& ec)
{
	m_io_handler->on_read_error(self, ec);
	do_close(self);
}

void tcp_socket_t::on_write_error(const tcp_socket_ptr& self, const boost::system::error_code& ec)
{
	m_io_handler->on_write_error(self, ec);
	do_close(self);
}

NS_NETWORK_END
