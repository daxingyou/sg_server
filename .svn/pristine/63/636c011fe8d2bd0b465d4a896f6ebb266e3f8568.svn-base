#include "db_server.hpp"
#include "db_server_fwd.hpp"

db_server_t* env::server = NULL;
common::db_xml_t* env::xml = NULL;
ConnectionPool_S* env::conn_game_zdb_pool = NULL;

USING_NS_NETWORK;
USING_NS_COMMON;

int main(int argc, char** argv)
{
	db_server_t server;
	env::server = &server;
	server.run_server();

	printf("db server stop!\n");

    return 0;
}
