#include "robot_server.hpp"
#include "robot_server_fwd.hpp"
#include "robot_common/robot_common_struct.hpp"
#include "robot_common/sTime.hpp"

#include <boost/bind.hpp>
#include <boost/system/error_code.hpp>

robot_server_t* env::server = NULL;
struct robot_config_s* env::robot_config = NULL;

USING_NS_NETWORK;
USING_NS_COMMON;

int main(int argc, char** argv)
{
	robot_config_t robot_config;
	if (argc > 3)
	{
		printf("\033[31margc[%d] > 3\033[0m\n", argc);
		return 1;
	} 
	else if (argc == 3)
	{
		int32_t start_id = atoi(argv[1]);
		int32_t robot_num = atoi(argv[2]);

		if (start_id == 0 || robot_num == 0)
		{
			printf("\033[31margv error start_id[%d] robot_num[%d] \033[0m\n", start_id, robot_num);
			return 1;
		}

		robot_config.start_id = start_id;
		robot_config.number = robot_num;
	} 
	else if (argc == 2)
	{
		robot_config.sepcial = argv[1];

		if (robot_config.sepcial.empty())
		{
			printf("\033[31m Robot sepcial string empty \033[0m\n");
			return 1;
		}
	} 
	else
	{
		printf("\033[31m./robot_server [robot_id]|[robot start id] [robot number] \033[0m\n");
		return 1;
	}

	robot_server_t robot_server;
	env::server = &robot_server;
	env::robot_config = &robot_config;
	robot_server.run_server();

	printf("robot server stop!\n");

    return 0;
}