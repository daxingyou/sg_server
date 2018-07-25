#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:LevelRewardTableBase
/// @remark   记录大小:12 X32
///           记录大小:12 X64
///           字段数量:3
///           数据文件:d等级目标奖励.xlsx Sheet1
///           排序主键:ID
#pragma pack(1)
struct LevelRewardTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x000510D9; ///< I|II
#else
	static const int          FormatCheckCode = 0x000510D9; ///< I|II
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 ID 生成唯一键

	uint32_t                  id;                           ///< ID

	uint32_t                  level;                        ///< 领取等级
	uint32_t                  drop;                         ///< 掉落ID
};
#pragma pack()

struct LevelRewardTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  level()                       { return t_level; }
	uint32_t                  drop()                        { return t_drop; }

private:
	uint32_t                  t_id;                         ///< ID

	uint32_t                  t_level;                      ///< 领取等级
	uint32_t                  t_drop;                       ///< 掉落ID

public:
	void reset();
	void fill( const LevelRewardTableBase& base );
	const char* getClassName() const { return "d等级目标奖励"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< LevelRewardTable, LevelRewardTableBase > LevelRewardTableManager;

inline void LevelRewardTable::reset()
{
	this->t_id = 0;
	this->t_level = 0;
	this->t_drop = 0;
};

inline void LevelRewardTable::fill( const LevelRewardTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_level = base.level;
	this->t_drop = base.drop;
};

inline const std::string LevelRewardTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "LevelRewardTable.tbls64";
#else
	return "LevelRewardTable.tbls32";
#endif
};
