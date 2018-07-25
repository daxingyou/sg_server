#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:ForbidWordTableBase
/// @remark   记录大小:8 X32
///           记录大小:12 X64
///           字段数量:2
///           数据文件:p屏蔽字.xlsx Sheet1
///           排序主键:id
#pragma pack(1)
struct ForbidWordTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005116; ///< I|V
#else
	static const int          FormatCheckCode = 0x00005136; ///< I|v
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 id 生成唯一键

	uint32_t                  id;                           ///< 屏蔽字ID

	const char*               word;                         ///< 屏蔽字内容
};
#pragma pack()

struct ForbidWordTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::string&        word()                        { return t_word; }

private:
	uint32_t                  t_id;                         ///< 屏蔽字ID

	std::string               t_word;                       ///< 屏蔽字内容

public:
	void reset();
	void fill( const ForbidWordTableBase& base );
	const char* getClassName() const { return "p屏蔽字"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< ForbidWordTable, ForbidWordTableBase > ForbidWordTableManager;

inline void ForbidWordTable::reset()
{
	this->t_id = 0;
	this->t_word.clear();
};

inline void ForbidWordTable::fill( const ForbidWordTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	if( base.word == NULL ) this->t_word.clear(); else this->t_word.assign( base.word );
};

inline const std::string ForbidWordTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "ForbidWordTable.tbls64";
#else
	return "ForbidWordTable.tbls32";
#endif
};
