#include "video_server.hpp"
#include "video_server_fwd.hpp"

video_server_t* env::server = NULL;
video_xml_t* env::xml = NULL;
ConnectionPool_S* env::conn_zdb_pool = NULL;

USING_NS_NETWORK;
USING_NS_COMMON;

int main(int argc, char** argv)
{
	video_server_t server;
	env::server = &server;
	server.run_server();

	printf("video server stop!\n");

	return 0;
}