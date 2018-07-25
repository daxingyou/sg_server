#include "io_base.hpp"
#include "tcp_socket.hpp"
#include "log.hpp"
#include "cmd_desc.hpp"

NS_NETWORK_BEGIN
void io_handler_t::on_new_msg(const tcp_socket_ptr& s, const message_t& msg)
{
	if (op_cmd::ss_pingpang_notify == msg.head.cmd) { // Ã²ËÆÊ²Ã´Ã»×ö
		return;
	}

	m_io_callback->on_new_msg(s, msg);
}

NS_NETWORK_END
