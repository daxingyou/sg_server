#include "gate_listener.hpp"
#include "log.hpp"
#include "main/login_server_fwd.hpp"
#include "main/login_server.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

void gate_listener_t::on_new_accept(const tcp_socket_ptr& s)
{
    env::server->get_ios().post(boost::bind(&login_server_t::on_add_gate, env::server, s));
}

void gate_listener_t::on_new_msg(const tcp_socket_ptr& s, const message_t& msg)
{
	if (msg.has_uid())
	{
		env::server->get_ios().post(boost::bind(&login_server_t::on_gate_msg, env::server, s, msg.get_uid(),msg.head.cmd, msg.body));
	} 
	else
	{
		env::server->get_ios().post(boost::bind(&login_server_t::on_gate_msg, env::server, s, 0,msg.head.cmd, msg.body));
	}
    
}

void gate_listener_t::on_close(const tcp_socket_ptr& s)
{
    env::server->get_ios().post(boost::bind(&login_server_t::on_del_gate, env::server, s));
}

void gate_listener_t::send_msg_to_gate(const tcp_socket_ptr& s, const msg_buf_ptr& msg)
{
    env::server->post_network_send_msg(s, msg);
}