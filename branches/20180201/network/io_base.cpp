#include "io_base.hpp"
#include "connector.hpp"
#include "log.hpp"

NS_NETWORK_BEGIN
USING_NS_COMMON;

io_handler_t::io_handler_t( boost::asio::io_service& ios, io_callback_t* io_callback )
    : m_ios(ios)
    , m_io_callback(io_callback)
{

}

io_handler_t::~io_handler_t()
{

}

void io_handler_t::on_connect_error( const tcp_socket_ptr& s, const boost::system::error_code& ec )
{

}

void io_handler_t::on_new_msg( const tcp_socket_ptr& s, const message_t& msg )
{
    if (m_io_callback)
    {
		if (SERVER_PINGPONG_REQUEST == msg.head.cmd)
		{
            //if (0 == strcmp(m_io_callback->name(), "gate"))
            //{
            //    log_warn("recv pingpong from gate");
            //}
			return;
		}
        m_io_callback->on_new_msg(s, msg);
    }
}

void io_handler_t::close()
{
}
NS_NETWORK_END
