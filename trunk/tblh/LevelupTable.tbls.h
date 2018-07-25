#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:LevelupTableBase
/// @remark   记录大小:12 X32
///           记录大小:12 X64
///           字段数量:3
///           数据文件:z主公经验表.xlsx Sheet1
///           排序主键:等级
#pragma pack(1)
struct LevelupTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x000510D9; ///< I|II
#else
	static const int          FormatCheckCode = 0x000510D9; ///< I|II
#endif

	//------------------------
	uint32_t getUniqueID() const { return level; }; // TODO: 请用 等级 生成唯一键

	uint32_t                  level;                        ///< 等级

	uint32_t                  level_up_exp;                 ///< 主公所需经验
	uint32_t                  exp_limit;                    ///< 主公经验上限
};
#pragma pack()

struct LevelupTable : public Entry
{
	uint32_t                  level()                       { return t_level; }

	uint32_t                  level_up_exp()                { return t_level_up_exp; }
	uint32_t                  exp_limit()                   { return t_exp_limit; }

private:
	uint32_t                  t_level;                      ///< 等级

	uint32_t                  t_level_up_exp;               ///< 主公所需经验
	uint32_t                  t_exp_limit;                  ///< 主公经验上限

public:
	void reset();
	void fill( const LevelupTableBase& base );
	const char* getClassName() const { return "z主公经验表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< LevelupTable, LevelupTableBase > LevelupTableManager;

inline void LevelupTable::reset()
{
	this->t_level = 0;
	this->t_level_up_exp = 0;
	this->t_exp_limit = 0;
};

inline void LevelupTable::fill( const LevelupTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_level = base.level;
	this->t_level_up_exp = base.level_up_exp;
	this->t_exp_limit = base.exp_limit;
};

inline const std::string LevelupTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "LevelupTable.tbls64";
#else
	return "LevelupTable.tbls32";
#endif
};
