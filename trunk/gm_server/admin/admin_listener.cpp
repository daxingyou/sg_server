#include "admin_listener.hpp"
#include "main/gm_server_fwd.hpp"
#include "main/gm_server.hpp"
#include "admin_manager.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

void admin_listener_t::on_new_accept(const tcp_socket_ptr& s)
{
    env::server->get_ios().post(boost::bind(&gm_server_t::on_add_admin, env::server, s));
	uint32_t admin_id = admin_manager_t::gen_admin_id();
	admin_manager_t::add_admin( admin_id, "test_gm", s);
}

void admin_listener_t::on_new_msg(const tcp_socket_ptr& s, const message_t& msg)
{
	if (msg.has_uid())
	{
		env::server->get_ios().post(boost::bind(&gm_server_t::on_admin_msg, env::server, s, msg.get_uid(),msg.head.cmd, msg.body));
	} 
	else
	{
		env::server->get_ios().post(boost::bind(&gm_server_t::on_admin_msg, env::server, s, 0,msg.head.cmd, msg.body));
	}
    
}

void admin_listener_t::on_close(const tcp_socket_ptr& s)
{
    env::server->get_ios().post(boost::bind(&gm_server_t::on_del_admin, env::server, s));
	admin_ptr p_admin = admin_manager_t::get_admin(s);
	if ( NULL != p_admin)
		admin_manager_t::del_admin( p_admin->get_admin_id() );
}

void admin_listener_t::send_msg_to_gm(const msg_buf_ptr& msg)
{
    env::server->post_network_send_msg(get_socket(), msg);
}


