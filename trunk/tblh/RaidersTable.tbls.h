#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:RaidersTableBase
/// @remark   记录大小:4 X32
///           记录大小:4 X64
///           字段数量:1
///           数据文件:w武将攻略推荐配置.xlsx 攻略推荐配置
///           排序主键:武将ID
#pragma pack(1)
struct RaidersTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0000050C; ///< I|
#else
	static const int          FormatCheckCode = 0x0000050C; ///< I|
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 武将ID 生成唯一键

	uint32_t                  id;                           ///< 武将ID

};
#pragma pack()

struct RaidersTable : public Entry
{
	uint32_t                  id()                          { return t_id; }


private:
	uint32_t                  t_id;                         ///< 武将ID


public:
	void reset();
	void fill( const RaidersTableBase& base );
	const char* getClassName() const { return "w武将攻略推荐配置"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< RaidersTable, RaidersTableBase > RaidersTableManager;

inline void RaidersTable::reset()
{
	this->t_id = 0;
};

inline void RaidersTable::fill( const RaidersTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
};

inline const std::string RaidersTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "RaidersTable.tbls64";
#else
	return "RaidersTable.tbls32";
#endif
};
