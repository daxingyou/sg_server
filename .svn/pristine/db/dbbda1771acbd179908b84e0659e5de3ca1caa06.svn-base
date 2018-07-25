#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:LevelAddExpRateTableBase
/// @remark   记录大小:16 X32
///           记录大小:16 X64
///           字段数量:4
///           数据文件:t天下大势.xlsx 经验加成
///           排序主键:id
#pragma pack(1)
struct LevelAddExpRateTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#else
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 id 生成唯一键

	uint32_t                  id;                           ///< id

	uint32_t                  level_up;                     ///< 落后区间up
	uint32_t                  level_down;                   ///< 落后区间down
	uint32_t                  buff_id;                      ///< buff_id
};
#pragma pack()

struct LevelAddExpRateTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  level_up()                    { return t_level_up; }
	uint32_t                  level_down()                  { return t_level_down; }
	uint32_t                  buff_id()                     { return t_buff_id; }

private:
	uint32_t                  t_id;                         ///< id

	uint32_t                  t_level_up;                   ///< 落后区间up
	uint32_t                  t_level_down;                 ///< 落后区间down
	uint32_t                  t_buff_id;                    ///< buff_id

public:
	void reset();
	void fill( const LevelAddExpRateTableBase& base );
	const char* getClassName() const { return "t天下大势"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< LevelAddExpRateTable, LevelAddExpRateTableBase > LevelAddExpRateTableManager;

inline void LevelAddExpRateTable::reset()
{
	this->t_id = 0;
	this->t_level_up = 0;
	this->t_level_down = 0;
	this->t_buff_id = 0;
};

inline void LevelAddExpRateTable::fill( const LevelAddExpRateTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_level_up = base.level_up;
	this->t_level_down = base.level_down;
	this->t_buff_id = base.buff_id;
};

inline const std::string LevelAddExpRateTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "LevelAddExpRateTable.tbls64";
#else
	return "LevelAddExpRateTable.tbls32";
#endif
};
