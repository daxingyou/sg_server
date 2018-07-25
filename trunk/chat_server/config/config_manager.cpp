#include "config_manager.hpp"
#include "main/chat_server.hpp"
#include "main/chat_server_fwd.hpp"
#include "tblh/ForbidWordTable.tbls.h"
#include "tblh/Comprehensive.tbls.h"
#include "tblh/TimeTable.tbls.h"

bool load_config()
{
	if (NULL != ForbidWordTableManager::getInstancePtr() &&
		!ForbidWordTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("ForbidWordTableManager");
	}

	if (NULL != ComprehensiveManager::getInstancePtr() &&
		!ComprehensiveManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("ComprehensiveManager");
	}

    if (NULL != TimeTableManager::getInstancePtr() &&
        !TimeTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
    {
        return false;
    }
    else
    {
        PUSH_TO_CONFIG_LIST("TimeTableManager");
    }

	log_info("load config success!");
	return true;
}

void release_config()
{
    ForbidWordTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("ForbidWordTableManager");

	ComprehensiveManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("ComprehensiveManager");

    TimeTableManager::delInstance();
    REMOVE_FROM_CONFIG_LIST("TimeTableManager");

	//¼ì²â±íµÄÊÍ·Å
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