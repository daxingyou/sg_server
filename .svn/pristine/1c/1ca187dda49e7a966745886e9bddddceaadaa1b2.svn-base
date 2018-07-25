#include "video_connector.hpp"
#include "main/chat_server_fwd.hpp"
#include "main/chat_server.hpp"

void video_connector_t::on_connect(const network::tcp_socket_ptr& s)
{
	env::server->get_ios().post(boost::bind(&chat_server_t::on_add_video, env::server, s));
}

void video_connector_t::on_new_msg(const network::tcp_socket_ptr& s, const network::message_t& msg)
{
	// pass
}

void video_connector_t::on_close(const network::tcp_socket_ptr& s)
{
	env::server->get_ios().post(boost::bind(&chat_server_t::on_del_video, env::server, s));
}
