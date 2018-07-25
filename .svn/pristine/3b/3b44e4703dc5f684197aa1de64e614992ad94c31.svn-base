#ifndef __GAME_DUNDEON_COMMON_H__
#define __GAME_DUNDEON_COMMON_H__

#include "macros.hpp"

namespace dungeon
{
	enum dungeon_type_enum : uint32_t
	{
		dungeon_type_none = 0,			// 无效
		dungeon_type_plot = 1,			// 剧情
		dungeon_type_daily = 2,			// 日常
		dungeon_type_tower = 5,			// 爬塔专用
	};

	enum refresh_type_enum : uint32_t
	{
		refresh_type_none = 0,	// 不能刷新
		refresh_type_day = 1,	// 每日
		refresh_type_week = 2,	// 每周
	};

	enum pass_type_enum : uint32_t
	{
		pass_type_none = 0,	// 无效
		pass_type_kill_any = 1,		// 击杀任意怪物到进度值
		pass_type_kill_all = 2,		// 击杀所有怪物到进度值
	};

	enum team_type_enum : uint32_t
	{
		team_type_none = 0,			// 无条件
		team_type_single = 1,		// 必须单人
		team_type_team_2 = 2,		// 组队 人数>=2
		team_type_team_3 = 3,		// 组队 人数>=3
	};

	enum team_check_enum : uint32_t
	{
		team_need_team_check = 1,		// 就位确认
		team_no_team_check = 2,			// 不需要就位确认
	};
}

#define MAX_DUNGEON_FIGHT_TIME	100		// 一个副本最大挑战次数

#endif
