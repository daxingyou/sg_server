#include "gm_server.hpp"
#include "gm_server_fwd.hpp"

gm_server_t* env::server = NULL;
common::gm_xml_t* env::xml = NULL;
struct ConnectionPool_S *env::conn_zdb_pool = NULL;

USING_NS_NETWORK;
USING_NS_COMMON;

int main(int argc, char** argv)
{
	gm_server_t server;
	env::server = &server;
	server.run_server();

	printf("gm server stop!\n");

    return 0;
}
