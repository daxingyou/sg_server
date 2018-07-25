#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    表格结构:EquipSuitBase
/// @remark   记录大小:12 X32
///           记录大小:20 X64
///           字段数量:3
///           数据文件:z装备表.xlsx 装备套装表
///           排序主键:装备套装id
#pragma pack(1)
struct EquipSuitBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x000511B6; ///< I|VV
#else
	static const int          FormatCheckCode = 0x000513D6; ///< I|vv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 装备套装id 生成唯一键

	uint32_t                  id;                           ///< 装备套装id

	const char*               two_attr;                     ///< 二件套装属性
	const char*               three_attr;                   ///< 三件套装属性
};
#pragma pack()

struct EquipSuit : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::map<uint32_t,uint32_t>&        two_attr()                    { return t_two_attr; }
	const std::vector<uint32_t>&        three_attr()                  { return t_three_attr; }

private:
	uint32_t                  t_id;                         ///< 装备套装id

	std::map<uint32_t, uint32_t>               t_two_attr;                   ///< 二件套装属性
	std::vector<uint32_t>               t_three_attr;                 ///< 三件套装属性

public:
	void reset();
	void fill( const EquipSuitBase& base );
	const char* getClassName() const { return "z装备表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< EquipSuit, EquipSuitBase > EquipSuitManager;

inline void EquipSuit::reset()
{
	this->t_id = 0;
	this->t_two_attr.clear();
	this->t_three_attr.clear();
};

inline void EquipSuit::fill( const EquipSuitBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	//if( base.two_attr == NULL ) this->t_two_attr.clear(); else this->t_two_attr.assign( base.two_attr );
	Dawn::parseMapContainer(this->t_two_attr, base.two_attr, ":$");
	//if( base.three_attr == NULL ) this->t_three_attr.clear(); else this->t_three_attr.assign( base.three_attr );
	Dawn::parseContainer<uint32_t>(this->t_three_attr, base.three_attr, "$");
};

inline const std::string EquipSuit::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "EquipSuit.tbls64";
#else
	return "EquipSuit.tbls32";
#endif
};
