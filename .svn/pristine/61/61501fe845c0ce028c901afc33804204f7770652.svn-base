#include "transfer_server.hpp"
#include "transfer_server_fwd.hpp"

transfer_server_t* env::server = NULL;
common::transfer_xml_t* env::xml = NULL;

USING_NS_NETWORK;
USING_NS_COMMON;

int main(int argc, char** argv)
{
	uint32_t transfer_id = 0;
	if (argc > 2)
	{
		printf("\033[31margc[%d] > 2\033[0m\n", argc);
		return 1;
	} 
	else if (argc == 2)
	{
		transfer_id = atoi(argv[1]);
	}

	transfer_server_t server;
	env::server = &server;
	env::server->set_id(transfer_id);
	server.run_server();

	printf("transfer server stop!\n");

    return 0;
}
