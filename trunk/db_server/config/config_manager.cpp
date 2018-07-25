#include "config_manager.hpp"
#include "main/db_server.hpp"
#include "main/db_server_fwd.hpp"
#include "tblh/MountTable.tbls.h"
#include "tblh/Item.tbls.h"
#include "tblh/Task.tbls.h"
#include "tblh/Hero.tbls.h"
#include "tblh/HeroJobTransfer.tbls.h"
#include "tblh/Comprehensive.tbls.h"
#include "tblh/Achievelist.tbls.h"

bool load_config()
{
	if (NULL != TaskManager::getInstancePtr() &&
		!TaskManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("TaskManager");
	}

	if (NULL != HeroManager::getInstancePtr() &&
		!HeroManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("HeroManager");
	}

	if (NULL != HeroJobTransferManager::getInstancePtr() &&
		!HeroJobTransferManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("HeroJobTransferManager");
	}

	if (NULL != MountTableManager::getInstancePtr() &&
		!MountTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("MountTableManager");
	}

	if (NULL != ItemManager::getInstancePtr() &&
		!ItemManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("ItemManager");
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

	if (NULL != AchievelistManager::getInstancePtr() &&
		!AchievelistManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("AchievelistManager");
	}

	log_info("load config success!");
	return true;
}

void release_config()
{
	TaskManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("TaskManager");

	HeroManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("HeroManager");

	HeroJobTransferManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("HeroJobTransferManager");
	
	MountTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("MountTableManager");

	ItemManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("ItemManager");

	ComprehensiveManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("ComprehensiveManager");

	AchievelistManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("AchievelistManager");


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