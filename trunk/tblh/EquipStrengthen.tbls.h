#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    表格结构:EquipStrengthenBase
/// @remark   记录大小:8 X32
///           记录大小:12 X64
///           字段数量:2
///           数据文件:z装备表.xlsx 装备强化
///           排序主键:装备等级
#pragma pack(1)
struct EquipStrengthenBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005116; ///< I|V
#else
	static const int          FormatCheckCode = 0x00005136; ///< I|v
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 装备等级 生成唯一键

	uint32_t                  id;                           ///< 装备等级

	const char*               material;                     ///< 材料消耗串
};
#pragma pack()

struct EquipStrengthen : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::vector<Dawn::quadruple<uint32_t, uint32_t, uint32_t, uint32_t>>&        material()                    { return t_material; }

private:
	uint32_t                  t_id;                         ///< 装备等级

	std::vector<Dawn::quadruple<uint32_t, uint32_t, uint32_t, uint32_t>>               t_material;                   ///< 材料消耗串

public:
	void reset();
	void fill( const EquipStrengthenBase& base );
	const char* getClassName() const { return "z装备表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< EquipStrengthen, EquipStrengthenBase > EquipStrengthenManager;

inline void EquipStrengthen::reset()
{
	this->t_id = 0;
	this->t_material.clear();
};

inline void EquipStrengthen::fill( const EquipStrengthenBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	//if( base.material == NULL ) this->t_material.clear(); else this->t_material.assign( base.material );
	Dawn::parseTupleContainer<Dawn::quadruple<uint32_t, uint32_t, uint32_t, uint32_t>>(this->t_material, base.material, ":$");
};

inline const std::string EquipStrengthen::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "EquipStrengthen.tbls64";
#else
	return "EquipStrengthen.tbls32";
#endif
};
