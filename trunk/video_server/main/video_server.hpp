#ifndef __VIDEO_SERVER_HPP__
#define __VIDEO_SERVER_HPP__

#include "server_base.hpp"
#include "common/zdb_ex.hpp"
#include "network.hpp"
#include "cmd_desc.hpp"
#include "listen/client_listen.hpp"

class video_server_t
	: public common::server_base_t
{
public:
	video_server_t() {};
	virtual ~video_server_t() {};

	const char* server_name() { return "video"; }
	bool on_init_server();
	bool on_close_server();
	void on_run_server();

private:
	bool load_xml();
	bool load_server_xml();
	void close_xml();

	bool init_zdb();
	void close_zdb();

	bool init_network();
	void close_network();
	void stop_network();

	bool init_logic();
	void clear_logic();

private:
	bool init_video();
	void close_video();

private:
	URL_T m_db_url;
	network::network_t m_network;
	client_listener_t m_client_listener;
};

#endif