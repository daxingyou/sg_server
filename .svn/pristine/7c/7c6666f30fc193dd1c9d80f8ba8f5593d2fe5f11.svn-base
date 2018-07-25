#include "login_listener.hpp"
#include "log.hpp"
#include "main/account_server_fwd.hpp"
#include "main/account_server.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

void login_listener_t::on_new_accept(const tcp_socket_ptr& s)
{
    if (NULL == s)
    {
        log_error("socket is NULL!");
        return;
    }
	env::server->get_ios().post(boost::bind(&account_server_t::on_add_login, env::server, s));
}

void login_listener_t::on_new_msg(const tcp_socket_ptr& s, const message_t& msg)
{
    if (NULL == s)
    {
        log_error("socket is NULL!");
        return;
    }
	if (msg.has_uid())
	{
		env::server->get_ios().post(boost::bind(&account_server_t::on_login_msg, env::server, s, msg.get_uid(), msg.head.cmd, msg.body));
	} 
	else
	{
		env::server->get_ios().post(boost::bind(&account_server_t::on_login_msg, env::server, s, 0, msg.head.cmd, msg.body));
	}
	
}

void login_listener_t::on_close(const tcp_socket_ptr& s)
{
    if (NULL == s)
    {
        log_error("socket is NULL!");
        return;
    }
	env::server->get_ios().post(boost::bind(&account_server_t::on_del_login, env::server, s));
}
