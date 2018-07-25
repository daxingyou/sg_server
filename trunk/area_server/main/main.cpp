#include "area_server.hpp"
#include "area_server_fwd.hpp"

area_server_t* env::server = NULL;
common::area_xml_t* env::xml = NULL;
struct ConnectionPool_S* env::conn_zdb_pool = NULL;

USING_NS_NETWORK;
USING_NS_COMMON;

int main(int argc, char** argv)
{
	area_server_t server;
	env::server = &server;
	server.run_server();

	printf("area server stop!\n");
    return 0;
}
