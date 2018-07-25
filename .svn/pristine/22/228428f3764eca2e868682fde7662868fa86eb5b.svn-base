#pragma once

#include "network.hpp"

class client_listener_t
	: public network::acceptor_callback_t
{
public:
	client_listener_t() {}
	virtual ~client_listener_t() {}

	virtual const char* name() { return "client"; }
	virtual void on_new_accept(const network::tcp_socket_ptr& s);
	virtual void on_new_msg(const network::tcp_socket_ptr& s, const network::message_t& msg);
	virtual void on_close(const network::tcp_socket_ptr& s);
};