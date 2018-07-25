#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:EquipSmeltBase
/// @remark   记录大小:8 X32
///           记录大小:8 X64
///           字段数量:2
///           数据文件:z装备表.xlsx 装备熔炼表
///           排序主键:熔炼获得ID
#pragma pack(1)
struct EquipSmeltBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#else
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 熔炼获得ID 生成唯一键

	uint32_t                  id;                           ///< 熔炼获得ID

	uint32_t                  drop_id;                      ///< 熔炼掉落id
};
#pragma pack()

struct EquipSmelt : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  drop_id()                     { return t_drop_id; }

private:
	uint32_t                  t_id;                         ///< 熔炼获得ID

	uint32_t                  t_drop_id;                    ///< 熔炼掉落id

public:
	void reset();
	void fill( const EquipSmeltBase& base );
	const char* getClassName() const { return "z装备表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< EquipSmelt, EquipSmeltBase > EquipSmeltManager;

inline void EquipSmelt::reset()
{
	this->t_id = 0;
	this->t_drop_id = 0;
};

inline void EquipSmelt::fill( const EquipSmeltBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_drop_id = base.drop_id;
};

inline const std::string EquipSmelt::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "EquipSmelt.tbls64";
#else
	return "EquipSmelt.tbls32";
#endif
};
