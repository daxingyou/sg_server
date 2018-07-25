#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:ArenaBattleAwardTableBase
/// @remark   记录大小:8 X32
///           记录大小:8 X64
///           字段数量:2
///           数据文件:j竞技场.xlsx arena_battle_awards
///           排序主键:胜利次数
#pragma pack(1)
struct ArenaBattleAwardTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00007129; ///< i|i
#else
	static const int          FormatCheckCode = 0x00007129; ///< i|i
#endif

	//------------------------
	uint32_t getUniqueID() const { return win_times; }; // TODO: 请用 胜利次数 生成唯一键

	int32_t                   win_times;                    ///< 胜利次数

	int32_t                   drop_id;                      ///< 奖励掉落包
};
#pragma pack()

struct ArenaBattleAwardTable : public Entry
{
	int32_t                   win_times()                   { return t_win_times; }

	int32_t                   drop_id()                     { return t_drop_id; }

private:
	int32_t                   t_win_times;                  ///< 胜利次数

	int32_t                   t_drop_id;                    ///< 奖励掉落包

public:
	void reset();
	void fill( const ArenaBattleAwardTableBase& base );
	const char* getClassName() const { return "j竞技场"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< ArenaBattleAwardTable, ArenaBattleAwardTableBase > ArenaBattleAwardTableManager;

inline void ArenaBattleAwardTable::reset()
{
	this->t_win_times = 0;
	this->t_drop_id = 0;
};

inline void ArenaBattleAwardTable::fill( const ArenaBattleAwardTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_win_times = base.win_times;
	this->t_drop_id = base.drop_id;
};

inline const std::string ArenaBattleAwardTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "ArenaBattleAwardTable.tbls64";
#else
	return "ArenaBattleAwardTable.tbls32";
#endif
};
