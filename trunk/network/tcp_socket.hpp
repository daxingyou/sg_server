#ifndef __NETWORK_TCP_SOCKET_HPP__
#define __NETWORK_TCP_SOCKET_HPP__

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <deque>

#include "message.hpp"

NS_NETWORK_BEGIN

class io_handler_t;

class tcp_socket_t;
typedef boost::shared_ptr<tcp_socket_t> tcp_socket_ptr;
typedef boost::weak_ptr<tcp_socket_t> tcp_socket_wptr;

class tcp_socket_t 
	: public boost::enable_shared_from_this<tcp_socket_t>
	, private boost::noncopyable
{
	friend class acceptor_t;
	friend class connector_t;

public:
	explicit tcp_socket_t(boost::asio::io_service& ios, boost::asio::ip::tcp::socket socket, const boost::asio::ip::tcp::endpoint& addr, io_handler_t* io_handler);
	~tcp_socket_t();

	/**
	 * \brief socket状态
	 */
	enum socket_state
	{
		closed = 0,				/**< 连接关闭状态 */
		open = 1,				/**< 连接正常状态 */
		shutdown_writing = 2,	/**< 正在关闭写状态 */
		shutdown_write = 3,		/**< 连接关闭写状态 */
	};

public:
	boost::asio::ip::tcp::socket& socket() { return m_inner_socket; }
	const boost::asio::ip::tcp::endpoint& addr() const { return m_addr; }
	int32_t get_socket_id() const { return m_socket_id; }
	inline io_handler_t* get_io_handler() const { return m_io_handler; }

    void read();
    void write(const msg_buf_ptr& buf);

protected:
	void close(bool is_forced = true);
	void do_close(const tcp_socket_ptr& self);
	void do_read(const tcp_socket_ptr& self);
	void do_write(const tcp_socket_ptr& self);
	void do_shutdown(const tcp_socket_ptr& self);
	void on_read_error(const tcp_socket_ptr& self, const boost::system::error_code& ec);
	void on_write_error(const tcp_socket_ptr& self, const boost::system::error_code& ec);

	inline uint8_t another_index(uint8_t idx)
	{
		return idx ^= 1;
	}

	void wrap_idle_buffers();

	// TODO:附加数据操作
public:
	void set_bind_data(void* data) { m_bind_data = data; }
	void* get_bind_data() { return m_bind_data; }

    void* m_iter = NULL;
    void* m_bind_data = NULL;

protected:
	boost::asio::io_service::strand m_strand;
    boost::asio::ip::tcp::socket m_inner_socket;
	int32_t m_socket_id;
	io_handler_t* m_io_handler;
	boost::asio::ip::tcp::endpoint m_addr;
	socket_state m_socket_state;
	bool m_is_close_notify;

private:
	message_parser_t m_msg_parser;
	boost::array<char, MAX_TCP_PACKET_SIZE> m_read_buf;

private:
	bool m_write_in_pending;
	uint8_t m_idle_index;
	std::vector<boost::asio::const_buffer> m_wrap_buf;
	std::deque<msg_buf_ptr> m_write_buf[2];
};

NS_NETWORK_END
#endif //__NETWORK_TCP_SOCKET_HPP__
