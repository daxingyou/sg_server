#include "account_connector.hpp"
#include "protobuf.hpp"
#include "main/login_server_fwd.hpp"
#include "main/login_server.hpp"


USING_NS_NETWORK;
USING_NS_COMMON;

void account_connector_t::on_connect(const tcp_socket_ptr& s)
{
    env::server->get_ios().post(boost::bind(&login_server_t::on_add_account, env::server, s));
}

void account_connector_t::on_new_msg(const tcp_socket_ptr& s, const message_t& msg)
{
	if (msg.has_uid())
	{
		env::server->get_ios().post(boost::bind(&login_server_t::on_account_msg, env::server,msg.get_uid() ,msg.head.cmd, msg.body));
	} 
	else
	{
		env::server->get_ios().post(boost::bind(&login_server_t::on_account_msg, env::server, 0, msg.head.cmd, msg.body));
	}
    
}

void account_connector_t::on_close(const tcp_socket_ptr& s)
{
    env::server->get_ios().post(boost::bind(&login_server_t::on_del_account, env::server, s));
}

void account_connector_t::send_msg_to_account(const msg_buf_ptr& msg)
{
    env::server->post_network_send_msg(get_socket(), msg);
}