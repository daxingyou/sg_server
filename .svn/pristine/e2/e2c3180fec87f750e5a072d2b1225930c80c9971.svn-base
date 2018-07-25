#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:ArenaClassTableBase
/// @remark   记录大小:28 X32
///           记录大小:28 X64
///           字段数量:7
///           数据文件:j竞技场.xlsx arena_class
///           排序主键:段位
#pragma pack(1)
struct ArenaClassTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00DDDAC9; ///< i|IIIIII
#else
	static const int          FormatCheckCode = 0x00DDDAC9; ///< i|IIIIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return arena_level; }; // TODO: 请用 段位 生成唯一键

	int32_t                   arena_level;                  ///< 段位

	uint32_t                  score;                        ///< 积分线
	uint32_t                  win_honor;                    ///< 战斗胜利荣誉获得
	uint32_t                  lose_honor;                   ///< 战斗失败荣誉获得
	uint32_t                  max_honor;                    ///< 荣誉上限
	uint32_t                  award;                        ///< 段位奖励
	uint32_t                  reset_stage;                  ///< 重置后段位
};
#pragma pack()

struct ArenaClassTable : public Entry
{
	int32_t                   arena_level()                 { return t_arena_level; }

	uint32_t                  score()                       { return t_score; }
	uint32_t                  win_honor()                   { return t_win_honor; }
	uint32_t                  lose_honor()                  { return t_lose_honor; }
	uint32_t                  max_honor()                   { return t_max_honor; }
	uint32_t                  award()                       { return t_award; }
	uint32_t                  reset_stage()                 { return t_reset_stage; }

private:
	int32_t                   t_arena_level;                ///< 段位

	uint32_t                  t_score;                      ///< 积分线
	uint32_t                  t_win_honor;                  ///< 战斗胜利荣誉获得
	uint32_t                  t_lose_honor;                 ///< 战斗失败荣誉获得
	uint32_t                  t_max_honor;                  ///< 荣誉上限
	uint32_t                  t_award;                      ///< 段位奖励
	uint32_t                  t_reset_stage;                ///< 重置后段位

public:
	void reset();
	void fill( const ArenaClassTableBase& base );
	const char* getClassName() const { return "j竞技场"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< ArenaClassTable, ArenaClassTableBase > ArenaClassTableManager;

inline void ArenaClassTable::reset()
{
	this->t_arena_level = 0;
	this->t_score = 0;
	this->t_win_honor = 0;
	this->t_lose_honor = 0;
	this->t_max_honor = 0;
	this->t_award = 0;
	this->t_reset_stage = 0;
};

inline void ArenaClassTable::fill( const ArenaClassTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_arena_level = base.arena_level;
	this->t_score = base.score;
	this->t_win_honor = base.win_honor;
	this->t_lose_honor = base.lose_honor;
	this->t_max_honor = base.max_honor;
	this->t_award = base.award;
	this->t_reset_stage = base.reset_stage;
};

inline const std::string ArenaClassTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "ArenaClassTable.tbls64";
#else
	return "ArenaClassTable.tbls32";
#endif
};
