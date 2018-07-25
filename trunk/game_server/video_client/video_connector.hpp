#pragma once

#include "network.hpp"

class video_connector_t
	: public network::connector_callback_t
{
public:
	video_connector_t() {}
	virtual ~video_connector_t() {}

	virtual const char* name() { return "video"; }
	virtual void on_connect(const network::tcp_socket_ptr& s);
	virtual void on_new_msg(const network::tcp_socket_ptr& s, const network::message_t& msg);
	virtual void on_close(const network::tcp_socket_ptr& s);
};
