#include "account_server.hpp"
#include "account_server_fwd.hpp"

account_server_t* env::server = NULL;
common::account_xml_t* env::xml = NULL;
ConnectionPool_S* env::conn_zdb_pool = NULL;

USING_NS_NETWORK;
USING_NS_COMMON;

int main(int argc, char** argv)
{
	uint32_t account_id = 0;
	if (argc > 2)
	{
		printf("\033[31margc[%d] > 2\033[0m\n", argc);
		return 1;
	} 
	else if (argc == 2)
	{
		account_id = atoi(argv[1]);
	}

	account_server_t server;
	env::server = &server;
	env::server->set_id(account_id);
	server.run_server();

	printf("account server stop!\n");
    return 0;
}
