#include "user_listener.hpp"
#include "log.hpp"
#include "main/account_server_fwd.hpp"
#include "main/account_server.hpp"
#include "user_manager.hpp"
#include "protos_fwd.hpp"


USING_NS_NETWORK;
USING_NS_COMMON;

void user_listener_t::on_new_accept(const tcp_socket_ptr& s)
{
    env::server->get_ios().post(boost::bind(&account_server_t::on_add_client, env::server, s));
}

void user_listener_t::on_new_msg( const tcp_socket_ptr& s, const message_t& msg )
{
	if (msg.has_uid())
	{
		env::server->get_ios().post(boost::bind(&account_server_t::on_client_msg, env::server, s, msg.get_uid(), msg.head.cmd, msg.body));
	} 
	else
	{
		env::server->get_ios().post(boost::bind(&account_server_t::on_client_msg, env::server, s, 0, msg.head.cmd, msg.body));
	}
    
}

void user_listener_t::on_close(const tcp_socket_ptr& s)
{
    env::server->get_ios().post(boost::bind(&account_server_t::on_del_client, env::server, s));
}