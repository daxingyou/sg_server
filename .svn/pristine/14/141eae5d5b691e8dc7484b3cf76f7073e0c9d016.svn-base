#include "config_manager.hpp"
#include "main/gate_server.hpp"
#include "main/gate_server_fwd.hpp"
#include "user/user_msg_checker.hpp"
#include "tblh/Comprehensive.tbls.h"

bool load_config()
{
	if (NULL != ComprehensiveManager::getInstancePtr() &&
		!ComprehensiveManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
    else
    {
		PUSH_TO_CONFIG_LIST("ComprehensiveManager");
        if (!user_msg_checker_t::init())
        {
            return false;
        }
    }

	log_info("load config success!");
	return true;
}

void release_config()
{
	ComprehensiveManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("ComprehensiveManager");


	//检查表的释放
	CHECK_CONFIG_LIST_SIZE();
    log_info("release config success!");
}

void print_config_list()
{
	for (auto& it : config_list)
	{
		log_error("[%s]table release fail!", it.c_str());
	}
}