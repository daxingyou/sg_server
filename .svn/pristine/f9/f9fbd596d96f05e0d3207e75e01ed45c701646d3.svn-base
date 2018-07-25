#include "transfer_listener.hpp"
#include "main/area_server_fwd.hpp"
#include "main/area_server.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

void transfer_listener_t::on_new_accept(const tcp_socket_ptr& s)
{
    env::server->get_ios().post(boost::bind(&area_server_t::on_add_transfer, env::server, s));
}

void transfer_listener_t::on_new_msg(const tcp_socket_ptr& s, const message_t& msg)
{
	if (msg.has_uid())
	{
		env::server->get_ios().post(boost::bind(&area_server_t::on_transfer_msg, env::server, s, msg.get_uid(), msg.head.cmd, msg.body));
	} 
	else
	{
		env::server->get_ios().post(boost::bind(&area_server_t::on_transfer_msg, env::server, s, 0, msg.head.cmd, msg.body));
	}
    
}

void transfer_listener_t::on_close(const tcp_socket_ptr& s)
{
    env::server->get_ios().post(boost::bind(&area_server_t::on_del_transfer, env::server, s));
}