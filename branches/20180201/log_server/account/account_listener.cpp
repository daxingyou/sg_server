#include "account_listener.hpp"
#include "main/log_server_fwd.hpp"
#include "main/log_server.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

void account_listener_t::on_new_accept(const tcp_socket_ptr& s)
{
    env::server->get_ios().post(boost::bind(&log_server_t::on_add_account, env::server, s));
}

void account_listener_t::on_new_msg(const tcp_socket_ptr& s, const message_t& msg)
{
    env::server->get_ios().post(boost::bind(&log_server_t::on_account_msg, env::server, s, msg.head.cmd, msg.body, msg.get_uid()));
}

void account_listener_t::on_close(const tcp_socket_ptr& s)
{
    env::server->get_ios().post(boost::bind(&log_server_t::on_del_account, env::server, s));
}