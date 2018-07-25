#include "area_connector.hpp"
#include "main/transfer_server_fwd.hpp"
#include "main/transfer_server.hpp"
#include "protobuf.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

void area_connector_t::on_connect(const tcp_socket_ptr& s)
{
    env::server->get_ios().post(boost::bind(&transfer_server_t::on_add_area, env::server, s));
    m_area_socket = s;
}

void area_connector_t::on_new_msg(const tcp_socket_ptr& s, const message_t& msg)
{
	if (msg.has_uid())
	{
		env::server->get_ios().post(boost::bind(&transfer_server_t::on_area_msg, env::server, s ,msg.get_uid() , msg.head.cmd, msg.body));
	} 
	else
	{
		env::server->get_ios().post(boost::bind(&transfer_server_t::on_area_msg, env::server, s, 0, msg.head.cmd, msg.body));
	}
    
}

void area_connector_t::on_close(const tcp_socket_ptr& s)
{
    env::server->get_ios().post(boost::bind(&transfer_server_t::on_del_area, env::server, s));
}

void area_connector_t::send_msg_to_area(const msg_buf_ptr& msg)
{
    env::server->post_network_send_msg(get_socket(), msg);
}
