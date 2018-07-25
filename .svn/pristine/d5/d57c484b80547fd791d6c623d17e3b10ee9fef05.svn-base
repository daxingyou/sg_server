#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:EquipExchangePoolBase
/// @remark   记录大小:16 X32
///           记录大小:16 X64
///           字段数量:4
///           数据文件:z装备表.xlsx 兑换池表
///           排序主键:兑换池id编号
#pragma pack(1)
struct EquipExchangePoolBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#else
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 兑换池id编号 生成唯一键

	uint32_t                  id;                           ///< 兑换池id编号

	uint32_t                  pool;                         ///< 兑换池id
	uint32_t                  equip_id;                     ///< 装备id
	uint32_t                  weight;                       ///< 权重
};
#pragma pack()

struct EquipExchangePool : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  pool()                        { return t_pool; }
	uint32_t                  equip_id()                    { return t_equip_id; }
	uint32_t                  weight()                      { return t_weight; }

private:
	uint32_t                  t_id;                         ///< 兑换池id编号

	uint32_t                  t_pool;                       ///< 兑换池id
	uint32_t                  t_equip_id;                   ///< 装备id
	uint32_t                  t_weight;                     ///< 权重

public:
	void reset();
	void fill( const EquipExchangePoolBase& base );
	const char* getClassName() const { return "z装备表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< EquipExchangePool, EquipExchangePoolBase > EquipExchangePoolManager;

inline void EquipExchangePool::reset()
{
	this->t_id = 0;
	this->t_pool = 0;
	this->t_equip_id = 0;
	this->t_weight = 0;
};

inline void EquipExchangePool::fill( const EquipExchangePoolBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_pool = base.pool;
	this->t_equip_id = base.equip_id;
	this->t_weight = base.weight;
};

inline const std::string EquipExchangePool::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "EquipExchangePool.tbls64";
#else
	return "EquipExchangePool.tbls32";
#endif
};
