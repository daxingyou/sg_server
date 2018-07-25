#include "log_server.hpp"
#include "log_server_fwd.hpp"

log_server_t* env::server = NULL;
common::log_xml_t* env::xml = NULL;
ConnectionPool_S* env::conn_zdb_pool = NULL;

USING_NS_NETWORK;
USING_NS_COMMON;

int main(int argc, char** argv)
{
	log_server_t server;
	env::server = &server;
	server.run_server();

	printf("log server stop!\n");

    return 0;
}
