//#include <gperftools/profiler.h> 

#include "game_server.hpp"
#include "game_server_fwd.hpp"

game_server_t* env::server = NULL;
common::game_xml_t* env::xml = NULL;

USING_NS_NETWORK;
USING_NS_COMMON;

int main(int argc, char** argv)
{
    //ProfilerStart("out.prof"); 

	uint32_t game_id = 0;
	if (argc > 2)
	{
		printf("\033[31margc[%d] > 2\033[0m\n", argc);
		return 1;
	} 
	else if (argc == 2)
	{
		game_id = atoi(argv[1]);
	}

	game_server_t server;
	env::server = &server;
	env::server->set_id(game_id);
	server.run_server();

	printf("game server stop!\n");

	//ProfilerStop();  
    return 0;
}
