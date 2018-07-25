#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:LanguageTableBase
/// @remark   记录大小:8 X32
///           记录大小:12 X64
///           字段数量:2
///           数据文件:y语言表.xlsx Sheet2
///           排序主键:ID
#pragma pack(1)
struct LanguageTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005116; ///< I|V
#else
	static const int          FormatCheckCode = 0x00005136; ///< I|v
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 ID 生成唯一键

	uint32_t                  id;                           ///< ID

	const char*               str;                          ///< 中文描述
};
#pragma pack()

struct LanguageTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::string&        str()                         { return t_str; }

private:
	uint32_t                  t_id;                         ///< ID

	std::string               t_str;                        ///< 中文描述

public:
	void reset();
	void fill( const LanguageTableBase& base );
	const char* getClassName() const { return "y语言表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< LanguageTable, LanguageTableBase > LanguageTableManager;

inline void LanguageTable::reset()
{
	this->t_id = 0;
	this->t_str.clear();
};

inline void LanguageTable::fill( const LanguageTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	if( base.str == NULL ) this->t_str.clear(); else this->t_str.assign( base.str );
};

inline const std::string LanguageTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "LanguageTable.tbls64";
#else
	return "LanguageTable.tbls32";
#endif
};
