#include "client_listen.hpp"
#include "cross/cross_arena_video.hpp"
#include "op_cmd.hpp"
#include "log.hpp"

void client_listener_t::on_new_accept(const network::tcp_socket_ptr& s)
{

}

void client_listener_t::on_new_msg(const network::tcp_socket_ptr& s, const network::message_t& msg)
{
	if (!msg.has_uid()) {
		log_error("cross_listener_t uid == 0");
		return;
	}

	switch (msg.head.cmd) {
		case op_cmd::sv_cross_area_video_notify: {
			cross_arena_service::getInstance().recv_video_message(msg.get_uid(), msg.body);
			break;
		}
		default:
			break;
	}
}

void client_listener_t::on_close(const network::tcp_socket_ptr& s)
{

}
