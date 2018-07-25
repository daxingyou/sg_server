#include "log_connector.hpp"
#include "main/center_server_fwd.hpp"
#include "main/center_server.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

void log_connector_t::on_connect(const tcp_socket_ptr& s)
{
    env::server->get_ios().post(boost::bind(&center_server_t::on_add_log, env::server, s));
}

void log_connector_t::on_new_msg(const tcp_socket_ptr& s, const message_t& msg)
{
    //log_error("do nothing!");
}

void log_connector_t::on_close(const tcp_socket_ptr& s)
{
    env::server->get_ios().post(boost::bind(&center_server_t::on_del_log, env::server, s));
}

void log_connector_t::send_msg_to_log(const msg_buf_ptr& msg)
{
    env::server->post_network_send_msg(get_socket(), msg);
}