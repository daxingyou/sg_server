#include "login_server.hpp"
#include "login_server_fwd.hpp"
#include "user/user_manager.hpp"

login_server_t* env::server = NULL;
common::login_xml_t* env::xml = NULL;
ConnectionPool_S* env::conn_zdb_pool = NULL;

USING_NS_NETWORK;
USING_NS_COMMON;

int main(int argc, char** argv)
{
	login_server_t server;
	env::server = &server;
	server.run_server();

	printf("login server stop!\n");

    return 0;
}
