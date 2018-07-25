#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    表格结构:FamousBase
/// @remark   记录大小:8 X32
///           记录大小:12 X64
///           字段数量:2
///           数据文件:m名将册.xlsx 名将册属性
///           排序主键:名将属性id
#pragma pack(1)
struct FamousBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005116; ///< I|V
#else
	static const int          FormatCheckCode = 0x00005136; ///< I|v
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 名将属性id 生成唯一键

	uint32_t                  id;                           ///< 名将属性id

	const char*               add_attr;                     ///< 属性
};
#pragma pack()

struct Famous : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::map<uint32_t,uint32_t>&        add_attr()                    { return t_add_attr; }

private:
	uint32_t                  t_id;                         ///< 名将属性id

	std::map<uint32_t, uint32_t>               t_add_attr;                   ///< 属性

public:
	void reset();
	void fill( const FamousBase& base );
	const char* getClassName() const { return "m名将册"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Famous, FamousBase > FamousManager;

inline void Famous::reset()
{
	this->t_id = 0;
	this->t_add_attr.clear();
};

inline void Famous::fill( const FamousBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	//if( base.add_attr == NULL ) this->t_add_attr.clear(); else this->t_add_attr.assign( base.add_attr );
	Dawn::parseMapContainer(this->t_add_attr, base.add_attr, ":$");
};

inline const std::string Famous::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Famous.tbls64";
#else
	return "Famous.tbls32";
#endif
};
