#include "center_server.hpp"
#include "center_server_fwd.hpp"

center_server_t* env::server = NULL;
common::center_xml_t* env::xml = NULL;
// ConnectionPool_S* env::conn_zdb_pool = NULL;

USING_NS_NETWORK;
USING_NS_COMMON;

int main(int argc, char** argv)
{
	center_server_t server;
	env::server = &server;
	server.run_server();

	printf("center server stop!\n");
	
    return 0;
}
