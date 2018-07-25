#include "config_manager.hpp"
#include "main/area_server.hpp"
#include "main/area_server_fwd.hpp"
#include "tblh/ForbidWordTable.tbls.h"
#include "tblh/Comprehensive.tbls.h"
#include "tblh/ArenaBattleAwardTable.tbls.h"
#include "tblh/ArenaClassTable.tbls.h"
#include "tblh/TimeTable.tbls.h"
#include "tblh/Monster.tbls.h"
#include "tblh/FightAgainst.tbls.h"
#include "tblh/Activities.tbls.h"
#include "tblh/RaidersTable.tbls.h"
#include "tblh/RankType.tbls.h"

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

	if (NULL != ArenaClassTableManager::getInstancePtr() &&
		!ArenaClassTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("ArenaClassTableManager");
	}

	if (NULL != ArenaBattleAwardTableManager::getInstancePtr() &&
		!ArenaBattleAwardTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("ArenaBattleAwardTableManager");
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

	if (NULL != MonsterManager::getInstancePtr() &&
		!MonsterManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("MonsterManager");
	}

	if (NULL != FightAgainstManager::getInstancePtr() &&
		!FightAgainstManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("FightAgainstManager");
	}

	if (NULL != ActivitiesManager::getInstancePtr() &&
		!ActivitiesManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("ActivitiesManager");
	}

	if (NULL != RaidersTableManager::getInstancePtr() &&
		!RaidersTableManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	}
	else
	{
		PUSH_TO_CONFIG_LIST("RaidersTableManager");
	}

	if (NULL != RankTypeManager::getInstancePtr() &&
		!RankTypeManager::getInstancePtr()->loadBinary(env::xml->config_path))
	{
		return false;
	} else
	{
		PUSH_TO_CONFIG_LIST("RankTypeManager");
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

	ArenaClassTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("ArenaClassTableManager");

	ArenaBattleAwardTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("ArenaBattleAwardTableManager");

	TimeTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("TimeTableManager");

	MonsterManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("MonsterManager");

	FightAgainstManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("FightAgainstManager");
	
	ActivitiesManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("ActivitiesManager");

	RaidersTableManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("RaidersTableManager");

	RankTypeManager::delInstance();
	REMOVE_FROM_CONFIG_LIST("RankTypeManager");

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