#include "config_manager.hpp"
#include "main/robot_server.hpp"
#include "main/robot_server_fwd.hpp"
#include "tblh/Task.tbls.h"
#include "tblh/TaskCircle.tbls.h"
#include "tblh/Bounty.tbls.h"
#include "main/robot_server_fwd.hpp"
#include "tblh/PrefixNameTable.tbls.h"
#include "tblh/SurNameTable.tbls.h"
#include "tblh/FemaleNameTable.tbls.h"
#include "tblh/MaleNameTable.tbls.h"
#include "tblh/DungeonTable.tbls.h"

bool load_config()
{
	if (NULL != TaskManager::getInstancePtr() &&
		!TaskManager::getInstancePtr()->loadBinary("../config"))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("TaskManager");
	}

	if (NULL != TaskCircleManager::getInstancePtr() &&
		!TaskCircleManager::getInstancePtr()->loadBinary("../config"))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("TaskCircleManager");
	}

	if (NULL != BountyManager::getInstancePtr() &&
		!BountyManager::getInstancePtr()->loadBinary("../config"))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("BountyManager");
	}

	if (NULL != PrefixNameTableManager::getInstancePtr() &&
		!PrefixNameTableManager::getInstancePtr()->loadBinary("../config"))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("PrefixNameTableManager");
	}

	if (NULL != SurNameTableManager::getInstancePtr() &&
		!SurNameTableManager::getInstancePtr()->loadBinary("../config"))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("SurNameTableManager");
	}

	if (NULL != FemaleNameTableManager::getInstancePtr() &&
		!FemaleNameTableManager::getInstancePtr()->loadBinary("../config"))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("FemaleNameTableManager");
	}

	if (NULL != MaleNameTableManager::getInstancePtr() &&
		!MaleNameTableManager::getInstancePtr()->loadBinary("../config"))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("MaleNameTableManager");
	}

	if (NULL != DungeonTableManager::getInstancePtr() &&
		!DungeonTableManager::getInstancePtr()->loadBinary("../config"))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("DungeonTableManager");
	}

	log_info("load config success!");
	return true;
}

void release_config()
{
    TaskManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("TaskManager");

    TaskCircleManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("TaskCircleManager");

    BountyManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("BountyManager");

	PrefixNameTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("PrefixNameTableManager");

	SurNameTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("SurNameTableManager");

	FemaleNameTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("FemaleNameTableManager");

	MaleNameTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("MaleNameTableManager");

	DungeonTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("DungeonTableManager");


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