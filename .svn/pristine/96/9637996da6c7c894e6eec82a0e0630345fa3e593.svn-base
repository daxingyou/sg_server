#include "chat_server.hpp"
#include "chat_server_fwd.hpp"

chat_server_t* env::server = NULL;
common::chat_xml_t* env::xml = NULL;

USING_NS_NETWORK;
USING_NS_COMMON;

int main(int argc, char** argv)
{
	chat_server_t server;
	env::server = &server;
	server.run_server();

	printf("chat server stop!\n");

    return 0;
}
