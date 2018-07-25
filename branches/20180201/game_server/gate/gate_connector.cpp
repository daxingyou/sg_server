#include "gate_connector.hpp"
#include "main/game_server_fwd.hpp"
#include "main/game_server.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

void gate_connector_t::on_connect(const tcp_socket_ptr& s)
{
    env::server->get_ios().post(boost::bind(&game_server_t::on_add_gate, env::server, s));
}

void gate_connector_t::on_new_msg(const tcp_socket_ptr& s, const message_t& msg)
{
    if (msg.has_uid())
    {
        env::server->get_ios().post(boost::bind(&game_server_t::on_gate_msg, env::server, s, msg.head.cmd, msg.body, msg.get_uid()));
	}
	else
	{
		env::server->get_ios().post(boost::bind(&game_server_t::on_gate_msg, env::server, s, msg.head.cmd, msg.body, 0));
	}
}

void gate_connector_t::on_close(const tcp_socket_ptr& s)
{
    env::server->get_ios().post(boost::bind(&game_server_t::on_del_gate, env::server, s));
}