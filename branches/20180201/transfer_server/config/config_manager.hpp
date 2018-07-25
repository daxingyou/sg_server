#ifndef __GAME_CONFIG_MANAGER_H__
#define __GAME_CONFIG_MANAGER_H__

#include "common/config_mgr.h"

//传入表名
#define REMOVE_FROM_CONFIG_LIST(table_name) \
	config_list.erase(table_name);

//传入表名
#define PUSH_TO_CONFIG_LIST(table_name) \
	config_list.insert(table_name);

#define CHECK_CONFIG_LIST_SIZE()	\
		if(!config_list.empty())	\
		{							\
			print_config_list();	\
		}

bool load_config();
void release_config();

void print_config_list();
//用于检测加载配置表是否正常释放
static std::set<std::string> config_list;

#endif//__GAME_CONFIG_MANAGER_H__
