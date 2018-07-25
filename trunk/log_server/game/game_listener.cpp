#include "game_listener.hpp"
#include "main/log_server_fwd.hpp"
#include "main/log_server.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

void game_listener_t::on_new_accept(const tcp_socket_ptr& s)
{
    env::server->get_ios().post(boost::bind(&log_server_t::on_add_game, env::server, s));
}

void game_listener_t::on_new_msg(const tcp_socket_ptr& s, const message_t& msg)
{
	if (msg.has_uid())
	{
		env::server->get_ios().post(boost::bind(&log_server_t::on_game_msg, env::server, s,msg.head.cmd, msg.body, msg.get_uid()));
	} 
	else
	{
		env::server->get_ios().post(boost::bind(&log_server_t::on_game_msg, env::server, s, msg.head.cmd, msg.body, 0));
	}
    
}

void game_listener_t::on_close(const tcp_socket_ptr& s)
{
    env::server->get_ios().post(boost::bind(&log_server_t::on_del_game, env::server, s));
}