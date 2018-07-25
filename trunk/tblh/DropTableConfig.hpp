#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/*
author:wys
date:2016.12.19
desc:掉落表 二次封装
*/

typedef std::map<uint32_t, std::set<uint32_t>> DROP_GROUP_MAP;

class DropTableConfig_t
{
public:

	static void get_drop_ids_by_group_id(uint32_t drop_group_id, std::set<uint32_t> &drop_ids)
	{
		DROP_GROUP_MAP::iterator iter = t_drop_group_map.find(drop_group_id);
		if (iter != t_drop_group_map.end())
			drop_ids = iter->second;
	}

	static void fill_drop_table(uint32_t drop_group_id, uint32_t drop_id)
	{
		DROP_GROUP_MAP::iterator iter = t_drop_group_map.find(drop_group_id);
		if (iter == t_drop_group_map.end())
		{
			std::set<uint32_t> id_set;
			id_set.insert(drop_id);
			t_drop_group_map.insert(std::make_pair(drop_group_id, id_set));
		}
		else
		{
			iter->second.insert(drop_id);
		}
	}

private:
	// 自定义 DropTable的group_id为key vector里是DropTable的id
	static DROP_GROUP_MAP	  t_drop_group_map;
};