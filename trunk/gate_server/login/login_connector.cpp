#include "login_connector.hpp"
#include "protobuf.hpp"
#include "main/gate_server_fwd.hpp"
#include "main/gate_server.hpp"


USING_NS_NETWORK;
USING_NS_COMMON;

void login_connector_t::on_connect(const tcp_socket_ptr& s)
{
    env::server->get_ios().post(boost::bind(&gate_server_t::on_add_login, env::server, s));
}

void login_connector_t::on_new_msg(const tcp_socket_ptr& s, const message_t& msg)
{
	if (msg.has_uid())
	{
		env::server->get_ios().post(boost::bind(&gate_server_t::on_login_msg, env::server, msg.get_uid(),msg.head.cmd, msg.body));
	} 
	else
	{
		env::server->get_ios().post(boost::bind(&gate_server_t::on_login_msg, env::server, 0, msg.head.cmd, msg.body));
	}
    
}

void login_connector_t::on_close(const tcp_socket_ptr& s)
{
    env::server->get_ios().post(boost::bind(&gate_server_t::on_del_login, env::server, s));
}

void login_connector_t::send_msg_to_login(const msg_buf_ptr& msg)
{
    env::server->post_network_send_msg(get_socket(), msg);
}