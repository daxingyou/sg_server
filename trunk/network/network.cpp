#include "network.hpp"
#include "log.hpp"
#include <boost/asio.hpp>

NS_NETWORK_BEGIN
USING_NS_COMMON;

network_t::network_t()
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
	log_info("close network_t start!");
	clear_all_listen();
	clear_all_connect();

    m_work.reset();
    // m_network_io_service.stop(); // 不能强制退出
    m_thread_group.join_all();

	m_listen_list.clear();
	m_connect_list.clear();
	log_info("close network_t success!");
}

void network_t::stop()
{
	stop_all_listen();
	stop_all_connect();
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
	if (!new_acceptor->listen()) {
		log_error("listen at port=%d failed!", port);
		return false;
	}

    m_listen_list.insert(std::make_pair(port, new_acceptor));
    return true;
}

void network_t::stop_all_listen()
{
	listen_list_type::iterator ibegin = m_listen_list.begin();
	listen_list_type::iterator iend = m_listen_list.end();
	for (; ibegin != iend; ++ibegin) {
		if (ibegin->second) {
			ibegin->second->stop();
		}
	}
}

void network_t::clear_all_listen()
{
	listen_list_type::iterator ibegin = m_listen_list.begin();
	listen_list_type::iterator iend = m_listen_list.end();
	for (; ibegin != iend; ++ibegin) {
		if (ibegin->second) {
			ibegin->second->clear();
		}
	}
}

//-----------------------------------------------------------------------------------------------------
bool network_t::connect_to(const std::string& ip, uint16_t port, connector_callback_t* connector_cb, uint32_t reconnect_interval_time, bool duplicate_conn, bool open_pingpong)
{
    //duplicate_conn针对robot使用
    if (!duplicate_conn) {
        for (size_t i = 0; i < m_connect_list.size(); ++i) {
            const connector_ptr_t& conn = m_connect_list[i];
            if (conn && conn->get_conn_port() == port && 0 == strcmp(conn->get_conn_ip().c_str(), ip.c_str())) {
                log_error("repeat connect to the address[%s: %d]!", ip.c_str(), port);
                return false;
            }
        }
    }

    connector_ptr_t new_connector(new connector_t(m_network_io_service, ip, port, connector_cb, reconnect_interval_time, open_pingpong));
    m_connect_list.push_back(new_connector);

    return true;
}

void network_t::stop_all_connect()
{
	connect_list_type::iterator itr = m_connect_list.begin();
	connect_list_type::iterator itr_end = m_connect_list.end();
	for (; itr != itr_end; ++itr) {
		const connector_ptr_t& conn = *itr;
		if (conn) {
			conn->stop();
		}
	}
}

void network_t::clear_all_connect()
{
	connect_list_type::iterator itr = m_connect_list.begin();
	connect_list_type::iterator itr_end = m_connect_list.end();
	for (; itr != itr_end; ++itr) {
		const connector_ptr_t& conn = *itr;
		if (conn) {
			conn->clear();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void network_t::post_send_msg(const tcp_socket_ptr& s, const msg_buf_ptr& buf)
{
	if (!s) {
		log_error("NULL == s");
		return;
	}

	s->write(buf);
}

void network_t::post_close_socket(const network::tcp_socket_ptr& s, bool is_del_forced/* = true*/)
{
	if (!s) {
		log_error("NULL == s");
		return;
	}

	io_handler_t* io_handler = s->get_io_handler();
	if (!io_handler) {
		log_error("NULL == io_handler");
		return;
	}

	io_handler->on_del_socket(s, is_del_forced);
}

NS_NETWORK_END

