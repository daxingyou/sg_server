#include "gate_server.hpp"
#include "gate_server_fwd.hpp"
#include "user/user_manager.hpp"

gate_server_t* env::server = NULL;
common::gate_xml_t* env::xml = NULL;

USING_NS_NETWORK;
USING_NS_COMMON;

int main(int argc, char** argv)
{
	uint32_t gate_id = 0;
	if (argc > 2)
	{
		printf("\033[31margc[%d] > 2\033[0m\n", argc);
		return 1;
	} 
	else if (argc == 2)
	{
		gate_id = atoi(argv[1]);
	}

	gate_server_t server;
	env::server = &server;
	env::server->set_id(gate_id);
	server.run_server();

	printf("gate server stop!\n");

    return 0;
}
