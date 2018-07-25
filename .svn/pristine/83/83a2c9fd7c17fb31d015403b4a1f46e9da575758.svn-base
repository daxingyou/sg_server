#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:MonsterBaseAttrTableBase
/// @remark   记录大小:8 X32
///           记录大小:12 X64
///           字段数量:2
///           数据文件:g怪物表.xlsx 怪物标准能力
///           排序主键:等级
#pragma pack(1)
struct MonsterBaseAttrTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005116; ///< I|V
#else
	static const int          FormatCheckCode = 0x00005136; ///< I|v
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 等级 生成唯一键

	uint32_t                  id;                           ///< 等级

	const char*               attrs;                        ///< 属性
};
#pragma pack()

struct MonsterBaseAttrTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::map<uint32_t, uint32_t>&        attrs() { return t_attrs; }

private:
	uint32_t                  t_id;                         ///< 等级

	std::map<uint32_t, uint32_t>               t_attrs;                      ///< 属性

public:
	void reset();
	void fill( const MonsterBaseAttrTableBase& base );
	const char* getClassName() const { return "g怪物表"; }

	static const std::string getBinaryName();

	int get_attr(uint32_t type)
	{
		std::map<uint32_t, uint32_t>::const_iterator it = t_attrs.find(type);
		if (it != t_attrs.end())
		{
			return it->second;
		}

		return 0;
	}
};

typedef DataTableManager< MonsterBaseAttrTable, MonsterBaseAttrTableBase > MonsterBaseAttrTableManager;

inline void MonsterBaseAttrTable::reset()
{
	this->t_id = 0;
	this->t_attrs.clear();
};

inline void MonsterBaseAttrTable::fill( const MonsterBaseAttrTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	//if( base.attrs == NULL ) this->t_attrs.clear(); else this->t_attrs.assign( base.attrs );
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_attrs, base.attrs, ":$");
};

inline const std::string MonsterBaseAttrTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "MonsterBaseAttrTable.tbls64";
#else
	return "MonsterBaseAttrTable.tbls32";
#endif
};
