#include "recharge_server.hpp"
#include "recharge_server_fwd.hpp"

recharge_server_t* env::server = NULL;
ConnectionPool_S* env::conn_zdb_pool = NULL;


USING_NS_NETWORK;
USING_NS_COMMON;

int main(int argc, char** argv)
{
	uint32_t recharge_id = 0;
	if (argc > 2)
	{
		printf("\033[31margc[%d] > 2\033[0m\n", argc);
		return 1;
	} 
	else if (argc == 2)
	{
		recharge_id = atoi(argv[1]);
	}

	account_server_t server;
	env::server = &server;
	env::server->set_id(recharge_id);
	server.run_server();

	printf("account server stop!\n");

    return 0;
}
