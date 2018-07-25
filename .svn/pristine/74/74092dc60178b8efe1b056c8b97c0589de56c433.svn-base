#ifndef __GAME_MONSTER_DEFINE_H__
#define __GAME_MONSTER_DEFINE_H__

#include "macros.hpp"
#include "common.pb.h"
#include <vector>

namespace monster_common
{
	// 巡逻配置
	struct alert_path_t
	{
		uint32_t pos_x = 0;
		uint32_t pos_y = 0;
		uint32_t stop_time = 0;
	};

	typedef std::vector<alert_path_t> alert_path_vec;

	enum mob_monster_type {
		mob_monster_normal = 0,		// 默认
		mob_monster_schedule = 1,	// 副本进度怪物
	};

	struct monster_define_t
	{
		uint32_t id = 0;												// id
		float pos_x = 0.f;												// 初始位置
		float pos_y = 0.f;												// 初始位置
		float dir = 0.f;												// 初始方向
		uint32_t type = 0;												// 怪物类型
		uint32_t ai_id = 0;												// ai id
		uint32_t interval = 0;											// 复活间隔
		//alert_path_vec paths;											// 警戒路径

		monster_define_t& operator= (const monster_define_t& def)
		{
			id = def.id;
			pos_x = def.pos_x;
			pos_y = def.pos_y;
			dir = def.dir;
			ai_id = def.ai_id;
			interval = def.interval;
			//paths.assign(def.paths.begin(), def.paths.end());

			return *this;
		}
	};

	struct king_war_monster : public monster_define_t
	{
		uint32_t hp = 0;												// 血量（王城争夺战用到）
		uint32_t area_id = 0;											// 区域id（王城争夺战用到）
		uint32_t king_war_type = 0;										// 什么类型的龙脉

		king_war_monster& operator= (const king_war_monster& def)
		{
			monster_define_t::operator =(def);
			hp = def.hp;
			area_id = def.area_id;
			king_war_type = def.king_war_type;

			return *this;
		}
	};

	struct dungeon_summon_dynamic : public monster_define_t
	{
		dungeon_summon_dynamic& operator= (const dungeon_summon_dynamic& def)
		{
			monster_define_t::operator =(def);

			return *this;
		}
	};

	struct dungeon_summon_delay : public monster_define_t
	{
		uint32_t delay = 0;												// 召唤数量限制

		dungeon_summon_delay& operator= (const dungeon_summon_delay& def)
		{
			monster_define_t::operator =(def);
			delay = def.delay;

			return *this;
		}
	};
}

#endif