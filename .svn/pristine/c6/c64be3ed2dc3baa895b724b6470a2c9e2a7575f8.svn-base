#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:SDTargetTableBase
/// @remark   记录大小:12 X32
///           记录大小:12 X64
///           字段数量:3
///           数据文件:y运营活动表.xlsx 七日目标
///           排序主键:序号ID
#pragma pack(1)
struct SDTargetTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x000510D9; ///< I|II
#else
	static const int          FormatCheckCode = 0x000510D9; ///< I|II
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 序号ID 生成唯一键

	uint32_t                  id;                           ///< 序号ID

	uint32_t                  day;                          ///< 时间
	uint32_t                  target_id;                    ///< 成就ID
};
#pragma pack()

struct SDTargetTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  day()                         { return t_day; }
	uint32_t                  target_id()                   { return t_target_id; }

private:
	uint32_t                  t_id;                         ///< 序号ID

	uint32_t                  t_day;                        ///< 时间
	uint32_t                  t_target_id;                  ///< 成就ID

public:
	void reset();
	void fill( const SDTargetTableBase& base );
	const char* getClassName() const { return "y运营活动表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< SDTargetTable, SDTargetTableBase > SDTargetTableManager;

inline void SDTargetTable::reset()
{
	this->t_id = 0;
	this->t_day = 0;
	this->t_target_id = 0;
};

inline void SDTargetTable::fill( const SDTargetTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_day = base.day;
	this->t_target_id = base.target_id;
};

inline const std::string SDTargetTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "SDTargetTable.tbls64";
#else
	return "SDTargetTable.tbls32";
#endif
};
