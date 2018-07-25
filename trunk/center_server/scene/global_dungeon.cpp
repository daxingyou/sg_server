#include "global_dungeon.hpp"
#include "tblh/errcode_enum.hpp"
#include "tblh/DungeonTable.tbls.h"
#include "config_mgr.h"

global_dungeon_t::global_dungeon_t(uint64_t scene_id, uint32_t map_tid, const proto::server::dungeon_create_data& data)
	: global_scene_t(scene_id, map_tid)
{
	m_create_data.CopyFrom(data);
}

// uint32_t global_dungeon_t::check_troop_join()
// {
// 	DungeonTable* p_dungeon_table = GET_CONF(DungeonTable, m_create_data.dungeon_id());
// 	if (NULL == p_dungeon_table)
// 	{
// 		log_error("dungeon_table id[%u] not find", m_create_data.dungeon_id());
// 		return common::errcode_enum::dungeon_id_not_find;
// 	}
// 
// 	if (p_dungeon_table->team_type() == 1)
// 	{
// 		log_error("dungeon_table id[%u] is single dungeon", m_create_data.dungeon_id());
// 		return common::errcode_enum::user_troop_err_single_dungeon;
// 	}
// 
// 	return common::errcode_enum::error_ok;
// }
