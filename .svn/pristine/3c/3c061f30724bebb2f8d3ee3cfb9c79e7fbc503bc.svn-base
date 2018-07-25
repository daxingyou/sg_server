#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/MyTuple.h"
#include "common/log.hpp"
#include "common/utility.hpp"

/// @brief    表格结构:DungeonTableBase
/// @remark   记录大小:72 X32
///           记录大小:88 X64
///           字段数量:18
///           数据文件:f副本表.xlsx dungeon
///           排序主键:id
#pragma pack(1)
struct DungeonTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x025DCA89; ///< I|IIIIIIVIiIIVVIVII
#else
	static const int          FormatCheckCode = 0x0DDE2AB9; ///< I|IIIIIIvIiIIvvIvII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 id 生成唯一键

	uint32_t                  id;                           ///< 副本编号

	uint32_t                  map_tid;                      ///< 场景ID
	uint32_t                  dungeon_type;                 ///< 副本类型
	uint32_t                  refresh_type;                 ///< 刷新方式
	uint32_t                  team_type;                    ///< 组队条件
	uint32_t                  team_check;                   ///< 是否就位确认
	uint32_t                  level;                        ///< 等级限制
	const char*               open_condition;               ///< 前置解锁条件
	uint32_t                  time_limit;                   ///< 通关时间限制
	int32_t                   revieve_limit;                ///< 复活次数限制
	uint32_t                  pass_type;                    ///< 通关类型
	uint32_t                  pass_schedule;                ///< 进度要求
	const char*               reward_monster;               ///< 奖励怪物
	const char*               limit_time;                   ///< 开启时间
	uint32_t                  time_duration;                ///< 持续时间
	const char*               pass_reward;                  ///< 副本评价奖励
	uint32_t                  first_pass_reward;            ///< 副本首次通关奖励
	uint32_t                  cost_vigour;                  ///< 精力消耗
};
#pragma pack()

struct DungeonTable : public Entry
{
	typedef Dawn::triple<uint32_t, uint32_t, uint32_t> day_time_t;
	typedef std::vector<day_time_t> limit_time_vec;

	uint32_t                  id()                          { return t_id; }

	uint32_t                  map_tid()                     { return t_map_tid; }
	uint32_t                  dungeon_type()                { return t_dungeon_type; }
	uint32_t                  refresh_type()                { return t_refresh_type; }
	uint32_t                  team_type()                   { return t_team_type; }
	uint32_t                  team_check()					{ return t_team_check; }
	uint32_t                  level()                       { return t_level; }
	const std::vector<std::pair<uint32_t, uint32_t>>& open_condition() const { return t_open_condition; }
	uint32_t                  time_limit()                  { return t_time_limit; }
	int32_t                   revieve_limit()				{ return t_revieve_limit; }
	uint32_t                  pass_type()                   { return t_pass_type; }
	uint32_t                  pass_schedule()               { return t_pass_schedule; }
	const std::map<uint32_t, uint32_t>& reward_monster() const { return t_reward_monster; }
	uint32_t                  cost_vigour()					{ return t_cost_vigour; }

	uint32_t                  first_pass_reward()           { return t_first_pass_reward; }

	uint32_t get_reward_monster(uint32_t id) const;

	bool is_limit_time_dungeon() const						{ return t_is_limit_time_dungeon; }
	bool is_in_open_time(uint32_t now_time) const;
	uint32_t get_pass_reward(uint32_t score) const;

private:
	bool is_time_in(uint32_t start, uint32_t last, uint32_t interval, uint32_t now_time) const;

private:
	uint32_t                  t_id;                         ///< 副本编号

	uint32_t                  t_map_tid;                    ///< 场景ID
	uint32_t                  t_dungeon_type;               ///< 副本类型
	uint32_t                  t_refresh_type;               ///< 刷新方式
	uint32_t                  t_team_type;                  ///< 组队条件
	uint32_t                  t_team_check;                 ///< 是否就位确认
	uint32_t                  t_level;                      ///< 等级限制
	std::vector<std::pair<uint32_t, uint32_t>> t_open_condition;	///< 前置解锁条件
	uint32_t                  t_time_limit;                 ///< 通关时间限制
	int32_t                   t_revieve_limit;              ///< 复活次数限制
	uint32_t                  t_pass_type;                  ///< 通关类型
	uint32_t                  t_pass_schedule;              ///< 进度要求
	std::map<uint32_t, uint32_t> t_reward_monster;			///< 奖励怪物
	limit_time_vec            t_limit_time_vec;             ///< 开启时间
	std::map<uint32_t, uint32_t> t_pass_reward;				///< 副本评价奖励
	uint32_t                  t_first_pass_reward;          ///< 副本首次通关奖励
	uint32_t                  t_cost_vigour;                ///< 精力消耗

	bool					  t_is_limit_time_dungeon;		///< 是否是限时副本

public:
	void reset();
	void fill( const DungeonTableBase& base );
	void fill_day_time(uint32_t hour, uint32_t min, uint32_t sec, uint32_t duration, uint32_t interval = DAY_SECOND);
	void fill_week_day_time(uint32_t week, uint32_t hour, uint32_t min, uint32_t sec, uint32_t duration, uint32_t interval = WEEK_SECOND);
	const char* getClassName() const { return "f副本表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< DungeonTable, DungeonTableBase > DungeonTableManager;

inline
uint32_t DungeonTable::get_reward_monster(uint32_t id) const
{
	std::map<uint32_t, uint32_t>::const_iterator citr = t_reward_monster.find(id);
	if (citr != t_reward_monster.end())
	{
		return citr->second;
	}

	return 0;
}

inline
bool DungeonTable::is_in_open_time(uint32_t now_time) const
{
	for (limit_time_vec::const_iterator citr = t_limit_time_vec.begin();
		citr != t_limit_time_vec.end(); ++citr)
	{
		const day_time_t& t = *citr;

		if (is_time_in(t.first, t.second, t.third, now_time))
		{
			return true;
		}
	}

	return false;
}

inline
uint32_t DungeonTable::get_pass_reward(uint32_t score) const
{
	std::map<uint32_t, uint32_t>::const_iterator citr = t_pass_reward.find(score);
	if (citr != t_pass_reward.end())
	{
		return citr->second;
	}

	return 0;
}

inline
bool DungeonTable::is_time_in(uint32_t start, uint32_t last, uint32_t interval, uint32_t now_time) const
{
	if (!interval) interval = DAY_SECOND;

	if ((start + last) <= now_time)
	{
		uint32_t tt = (now_time - last - start) / interval + 1;
		start += interval * tt;
	}

	return start <= now_time &&  now_time < start + last;
}

inline void DungeonTable::reset()
{
	this->t_id = 0;
	this->t_map_tid = 0;
	this->t_dungeon_type = 0;
	this->t_refresh_type = 0;
	this->t_team_type = 0;
	this->t_team_check = 0;
	this->t_level = 0;
	this->t_open_condition.clear();
	this->t_time_limit = 0;
	this->t_revieve_limit = 0;
	this->t_pass_type = 0;
	this->t_pass_schedule = 0;
	this->t_reward_monster.clear();
	this->t_limit_time_vec.clear();
	this->t_pass_reward.clear();
	this->t_first_pass_reward = 0;
	this->t_cost_vigour = 0;
	this->t_is_limit_time_dungeon = false;
};

inline void DungeonTable::fill( const DungeonTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_map_tid = base.map_tid;
	this->t_dungeon_type = base.dungeon_type;
	this->t_refresh_type = base.refresh_type;
	this->t_team_type = base.team_type;
	this->t_team_check = base.team_check;
	this->t_level = base.level;
	//if( base.open_condition == NULL ) this->t_open_condition.clear(); else this->t_open_condition.assign( base.open_condition );
	if (base.open_condition != NULL)
	{
		std::vector<std::string> conditions;
		common::string_util_t::split<std::string>(std::string(base.open_condition), conditions, "$");
		std::vector<uint32_t> cond_v;

		for (uint32_t i = 0; i < conditions.size(); ++i)
		{
			// cond_v.clear();
			common::string_util_t::split<uint32_t>(conditions[i], cond_v, ":");
			if (cond_v.size() == 2)
			{
				t_open_condition.push_back(std::make_pair(cond_v[0], cond_v[1]));
			}
		}
	}
	this->t_time_limit = base.time_limit;
	this->t_revieve_limit = base.revieve_limit;
	this->t_pass_type = base.pass_type;
	this->t_pass_schedule = base.pass_schedule;
	//if( base.reward_monster == NULL ) this->t_reward_monster.clear(); else this->t_reward_monster.assign( base.reward_monster );
	if (base.reward_monster != NULL)
	{
		std::vector<std::string> monsters;
		common::string_util_t::split<std::string>(std::string(base.reward_monster), monsters, "$");
		std::vector<uint32_t> monster_v;

		for (uint32_t i = 0; i < monsters.size(); ++i)
		{
			// monster_v.clear();
			common::string_util_t::split<uint32_t>(monsters[i], monster_v, ":");
			if (monster_v.size() == 2)
			{
				t_reward_monster.insert(std::make_pair(monster_v[0], monster_v[1]));
			}
		}
	}

	if (this->t_refresh_type && base.limit_time != NULL)
	{
		std::vector<std::string> limit_times;

		common::string_util_t::split<std::string>(std::string(base.limit_time), limit_times, "$");

		if (!limit_times.empty())
		{
			this->t_is_limit_time_dungeon = true;

			std::vector<uint32_t> times_v;

			for (uint32_t i = 0; i < limit_times.size(); ++i)
			{
				common::string_util_t::split<uint32_t>(limit_times[i], times_v, ":");

				if (this->t_refresh_type == 1 && times_v.size() == 3)
				{
					fill_day_time(times_v[0], times_v[1], times_v[2], base.time_duration, DAY_SECOND);
				}
				else if (this->t_refresh_type == 2 && times_v.size() == 4)
				{
					fill_week_day_time(times_v[0], times_v[1], times_v[2], times_v[3], base.time_duration, WEEK_SECOND);
				}
				else
				{
					log_error("DungeonTable read ID[%u] refresh_type error", this->t_id);
				}
			}
		}
	}

	if (base.pass_reward != NULL)
	{
		common::Misc::Parse2SeperatorDataInfo(std::string(base.pass_reward), this->t_pass_reward, "$");
	}

	this->t_first_pass_reward = base.first_pass_reward;
	this->t_cost_vigour = base.cost_vigour;
};

inline void DungeonTable::fill_day_time(uint32_t hour, uint32_t min, uint32_t sec, uint32_t duration, uint32_t interval /*= DAY_SECOND*/)
{
	time_t cur_time = common::time_util_t::now_time();

	struct tm day_time;
	common::time_util_t::get_local_time(day_time, cur_time);

	day_time.tm_hour = (int32_t)hour;
	day_time.tm_min = (int32_t)min;
	day_time.tm_sec = (int32_t)sec;

	time_t target_time = common::time_util_t::get_utc_time(day_time);
	if (target_time == (time_t)-1)
	{
		log_error("DungeonTable invalid get_utc_time error, ID[%u]", this->t_id);
		return;
	}

	t_limit_time_vec.push_back(day_time_t(target_time - interval, duration, interval));
}

inline void DungeonTable::fill_week_day_time(uint32_t week, uint32_t hour, uint32_t min, uint32_t sec, uint32_t duration, uint32_t interval/* = WEEK_SECOND*/)
{
	struct tm day_time;
	common::time_util_t::get_local_time(day_time, common::time_util_t::now_time());

	day_time.tm_hour = (int32_t)hour;
	day_time.tm_min = (int32_t)min;
	day_time.tm_sec = (int32_t)sec;

	time_t target_time = common::time_util_t::get_utc_time(day_time);
	if (target_time == (time_t)-1)
	{
		log_error("DungeonTable invalid get_utc_time error, ID[%u]", this->t_id);
		return;
	}

	int32_t curr_week = common::time_util_t::get_weekday();
	int32_t start_week = (int32_t)week;
	if (curr_week >= start_week) {
		target_time -= (curr_week - start_week) * DAY_SECOND;
	} else {
		target_time += (start_week - curr_week) * DAY_SECOND;
	}

	t_limit_time_vec.push_back(day_time_t(target_time - interval, duration, interval));
}

inline const std::string DungeonTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "DungeonTable.tbls64";
#else
	return "DungeonTable.tbls32";
#endif
};
