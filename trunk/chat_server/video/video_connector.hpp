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

public:
	inline network::tcp_socket_ptr get_socket() const { return m_video_socket.lock(); }
	inline void set_socket(const network::tcp_socket_ptr& s) { m_video_socket = s; }
	inline void close_socket() { m_video_socket.reset(); }

private:
	network::tcp_socket_wptr m_video_socket;
};
