#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"
#include "common/utility.hpp"
/// @brief    表格结构:HeadTableBase
/// @remark   记录大小:8 X32
///           记录大小:12 X64
///           字段数量:2
///           数据文件:t头像表.xlsx head
///           排序主键:头像id
#pragma pack(1)
struct HeadTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005116; ///< I|V
#else
	static const int          FormatCheckCode = 0x00005136; ///< I|v
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 头像id 生成唯一键

	uint32_t                  id;                           ///< 头像id

	const char*               cond;                         ///< 激活条件
};
#pragma pack()

struct HeadTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::map<uint32_t, uint32_t>&        cond()       { return t_cond; }

private:
	uint32_t                  t_id;                         ///< 头像id

	std::map<uint32_t, uint32_t>   t_cond;                  ///< 激活条件

public:
	void reset();
	void fill( const HeadTableBase& base );
	const char* getClassName() const { return "t头像表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< HeadTable, HeadTableBase > HeadTableManager;

inline void HeadTable::reset()
{
	this->t_id = 0;
	this->t_cond.clear();
};

inline void HeadTable::fill( const HeadTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	///if( base.cond == NULL ) this->t_cond.clear(); else this->t_cond.assign( base.cond );
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_cond, base.cond, ":$");
};

inline const std::string HeadTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "HeadTable.tbls64";
#else
	return "HeadTable.tbls32";
#endif
};
