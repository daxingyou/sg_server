#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:BossBornTableBase
/// @remark   记录大小:44 X32
///           记录大小:64 X64
///           字段数量:11
///           数据文件:s首领刷新表.xlsx 首领刷新表
///           排序主键:排序编号
#pragma pack(1)
struct BossBornTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0BBD0009; ///< I|IVVIVIIVVI
#else
	static const int          FormatCheckCode = 0x05BD0019; ///< I|IvvIvIIvvI
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 排序编号 生成唯一键

	uint32_t                  id;                           ///< 排序编号

	uint32_t                  monster_tid;                  ///< 怪物ID
	const char*               weekday;                      ///< 刷新日期
	const char*               start_time;                   ///< 刷新时间
	uint32_t                  map_id;                       ///< 刷新地图
	const char*               map_pos;                      ///< 刷新位置
	uint32_t                  map_radius;                   ///< 随机半径
	uint32_t                  last_time;                    ///< 存活时间
	const char*               kill_rwds;                    ///< 击杀奖励
	const char*               treasure;                     ///< 家族宝箱
	uint32_t                  treasure_radius;              ///< 宝箱刷新范围
};
#pragma pack()

typedef std::tuple<uint32_t, uint32_t> boss_born_time;
typedef std::tuple<uint32_t, uint32_t, uint32_t> boss_born_pos;

struct BossBornTable : public Entry
{
	uint32_t                  id() { return t_id; }

	uint32_t                  monster_tid()					{ return t_monster_tid; }
// 	const std::string&        weekday()                     { return t_weekday; }
// 	const std::string&        start_time()                  { return t_start_time; }
	uint32_t                  map_id()                      { return t_map_id; }
//	const std::string&        map_pos()                     { return t_map_pos; }
	uint32_t                  map_radius()                  { return t_map_radius; }
	uint32_t                  last_time()                   { return t_last_time; }
	//const std::string&        kill_rwds() { return t_kill_rwds; }
	const std::string&        treasure() { return t_treasure; }
	uint32_t                  treasure_radius() { return t_treasure_radius; }
	
	const std::vector<uint32_t>& get_weekday() { return t_week_day; }
	const std::vector<boss_born_time>& get_boss_born_time() { return t_boss_born_time; }
	const std::vector<boss_born_pos>& get_boss_born_pos() { return t_boss_born_pos; }

	bool is_in_today(uint32_t week_day);
	bool get_one_boss_born_pos(boss_born_pos& pos);

	const std::map<uint32_t, uint32_t>& get_kill_rwds() { return t_kill_rwds; }

private:
	uint32_t                  t_id;                         ///< 排序编号

	uint32_t                  t_monster_tid;                ///< 怪物ID
// 	std::string               t_weekday;                    ///< 刷新日期
// 	std::string               t_start_time;                 ///< 刷新时间
	uint32_t                  t_map_id;                     ///< 刷新地图
//	std::string               t_map_pos;                    ///< 刷新位置
	uint32_t                  t_map_radius;                 ///< 随机半径
	uint32_t                  t_last_time;                  ///< 存活时间
	//std::string               t_kill_rwds;                  ///< 击杀奖励
	std::string               t_treasure;                   ///< 家族宝箱
	uint32_t                  t_treasure_radius;            ///< 宝箱刷新范围
	
	std::vector<uint32_t> t_week_day;				// 周几合集
	std::vector<boss_born_time> t_boss_born_time;	// 出生时间集
	std::vector<boss_born_pos> t_boss_born_pos;		// 出生点坐标集
	std::map<uint32_t, uint32_t> t_kill_rwds;		// 击杀奖励

public:
	void reset();
	void fill( const BossBornTableBase& base );
	const char* getClassName() const { return "s首领刷新表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< BossBornTable, BossBornTableBase > BossBornTableManager;

inline void BossBornTable::reset()
{
	this->t_id = 0;
	this->t_monster_tid = 0;
//	this->t_weekday.clear();
//	this->t_start_time.clear();
	this->t_map_id = 0;
//	this->t_map_pos.clear();
	this->t_map_radius = 0;
	this->t_last_time = 0;
	this->t_kill_rwds.clear();
	this->t_treasure.clear();
	this->t_treasure_radius = 0;
	
	this->t_week_day.clear();
	this->t_boss_born_time.clear();
	this->t_boss_born_pos.clear();
};

inline void BossBornTable::fill( const BossBornTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_monster_tid = base.monster_tid;
//	if( base.weekday == NULL ) this->t_weekday.clear(); else this->t_weekday.assign( base.weekday );
//	if( base.start_time == NULL ) this->t_start_time.clear(); else this->t_start_time.assign( base.start_time );
	this->t_map_id = base.map_id;
//	if( base.map_pos == NULL ) this->t_map_pos.clear(); else this->t_map_pos.assign( base.map_pos );
	this->t_map_radius = base.map_radius;
	this->t_last_time = base.last_time;
//	if (base.kill_rwds == NULL) this->t_kill_rwds.clear(); else this->t_kill_rwds.assign(base.kill_rwds);
	if (base.treasure == NULL) this->t_treasure.clear(); else this->t_treasure.assign(base.treasure);
	this->t_treasure_radius = base.treasure_radius;

	if (NULL != base.weekday)
	{
		common::string_util_t::split<uint32_t>(std::string(base.weekday), this->t_week_day, "$");
	}

	if (NULL != base.start_time)
	{
		std::vector<std::string> time_vec;
		common::string_util_t::split<std::string>(std::string(base.start_time), time_vec, "$");
		std::vector<uint32_t> time_v;

		for (uint32_t i = 0; i < time_vec.size(); ++i)
		{
			time_v.clear();
			common::string_util_t::split<uint32_t>(time_vec[i], time_v, ":");
			if (time_v.size() == 2)
			{
				t_boss_born_time.push_back(std::make_tuple(time_v[0], time_v[1]));
			}
		}
	}
	
	if (NULL != base.map_pos)
	{
		std::vector<std::string> pos_vec;
		common::string_util_t::split<std::string>(std::string(base.map_pos), pos_vec, "$");
		std::vector<uint32_t> pos_v;

		for (uint32_t i = 0; i < pos_vec.size(); ++i)
		{
			pos_v.clear();
			common::string_util_t::split<uint32_t>(pos_vec[i], pos_v, ":");
			if (pos_v.size() == 3)
			{
				t_boss_born_pos.push_back(std::make_tuple(pos_v[0], pos_v[1], pos_v[2]));
			}
		}
	}

	if (base.kill_rwds != NULL) {
		common::Misc::Parse2SeperatorDataInfo(std::string(base.kill_rwds), this->t_kill_rwds, "$");
	}
};

inline bool BossBornTable::is_in_today(uint32_t week_day)
{
	std::vector<uint32_t>::const_iterator citr = std::find_if(this->t_week_day.begin(), this->t_week_day.end(),
		[week_day](const uint32_t& val)
	{
		if (val == week_day) return true;
		return false;
	}
	);

	if (citr != this->t_week_day.end()) {
		return true;
	}

	return false;
};

inline bool BossBornTable::get_one_boss_born_pos(boss_born_pos& pos)
{
	if (t_boss_born_pos.empty())
	{
		log_error("have no boss born pos");
		return false;
	}

	int32_t born_index = random_util_t::randBetween(0, (int32_t)t_boss_born_pos.size() - 1);
	if (born_index == -1)
	{
		log_error("rand from 0 to [%d] error", (int32_t)t_boss_born_pos.size() - 1);
		return false;
	}

	if (born_index < (int32_t)t_boss_born_pos.size())
	{
		pos = t_boss_born_pos.at(born_index);
	}

	return true;
};

inline const std::string BossBornTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "BossBornTable.tbls64";
#else
	return "BossBornTable.tbls32";
#endif
};
