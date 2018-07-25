#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    表格结构:EquipStrengthenMasterBase
/// @remark   记录大小:12 X32
///           记录大小:16 X64
///           字段数量:3
///           数据文件:z装备表.xlsx 强化大师
///           排序主键:强化大师等级
#pragma pack(1)
struct EquipStrengthenMasterBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x000510E6; ///< I|IV
#else
	static const int          FormatCheckCode = 0x00051106; ///< I|Iv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 强化大师等级 生成唯一键

	uint32_t                  id;                           ///< 强化大师等级

	uint32_t                  equip_level;                  ///< 装备等级
	const char*               award;                        ///< 强化大师串
};
#pragma pack()

struct EquipStrengthenMaster : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  equip_level()                 { return t_equip_level; }
	const std::map<uint32_t, uint32_t>&        award()                       { return t_award; }

private:
	uint32_t                  t_id;                         ///< 强化大师等级

	uint32_t                  t_equip_level;                ///< 装备等级
	std::map<uint32_t, uint32_t>               t_award;                      ///< 强化大师串

public:
	void reset();
	void fill( const EquipStrengthenMasterBase& base );
	const char* getClassName() const { return "z装备表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< EquipStrengthenMaster, EquipStrengthenMasterBase > EquipStrengthenMasterManager;

inline void EquipStrengthenMaster::reset()
{
	this->t_id = 0;
	this->t_equip_level = 0;
	this->t_award.clear();
};

inline void EquipStrengthenMaster::fill( const EquipStrengthenMasterBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_equip_level = base.equip_level;
	//if( base.award == NULL ) this->t_award.clear(); else this->t_award.assign( base.award );
	Dawn::parseMapContainer(this->t_award, base.award, ":$");
};

inline const std::string EquipStrengthenMaster::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "EquipStrengthenMaster.tbls64";
#else
	return "EquipStrengthenMaster.tbls32";
#endif
};
