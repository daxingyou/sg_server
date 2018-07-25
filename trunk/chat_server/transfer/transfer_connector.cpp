#include "transfer_connector.hpp"
#include "main/chat_server_fwd.hpp"
#include "main/chat_server.hpp"
#include "protobuf.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

void transfer_connector_t::on_connect(const tcp_socket_ptr& s)
{
    env::server->get_ios().post(boost::bind(&chat_server_t::on_add_transfer, env::server, s));
    m_transfer_socket = s;
}

void transfer_connector_t::on_new_msg(const tcp_socket_ptr& s, const message_t& msg)
{
	if (msg.has_uid())
	{
		env::server->get_ios().post(boost::bind(&chat_server_t::on_transfer_msg, env::server, s, msg.get_uid(),msg.head.cmd, msg.body));
	} 
	else
	{
		env::server->get_ios().post(boost::bind(&chat_server_t::on_transfer_msg, env::server, s, 0, msg.head.cmd, msg.body));
	}
    
}

void transfer_connector_t::on_close(const tcp_socket_ptr& s)
{
    env::server->get_ios().post(boost::bind(&chat_server_t::on_del_transfer, env::server, s));
}

void transfer_connector_t::send_msg_to_transfer(const msg_buf_ptr& msg)
{
    env::server->post_network_send_msg(get_socket(), msg);
}
