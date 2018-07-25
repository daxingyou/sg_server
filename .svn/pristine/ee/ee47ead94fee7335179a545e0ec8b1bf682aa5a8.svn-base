#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:EquipExattrRemakeBase
/// @remark   记录大小:16 X32
///           记录大小:16 X64
///           字段数量:4
///           数据文件:z装备表.xlsx 判词洗练消耗
///           排序主键:洗练锁定数量
#pragma pack(1)
struct EquipExattrRemakeBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#else
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 洗练锁定数量 生成唯一键

	uint32_t                  id;                           ///< 洗练锁定数量

	uint32_t                  material_count;               ///< 消耗材料数量
	uint32_t                  lock_count;                   ///< 锁定消耗材料数量
	uint32_t                  money;                        ///< 消耗材铜币数量
};
#pragma pack()

struct EquipExattrRemake : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  material_count()              { return t_material_count; }
	uint32_t                  lock_count()                  { return t_lock_count; }
	uint32_t                  money()                       { return t_money; }

private:
	uint32_t                  t_id;                         ///< 洗练锁定数量

	uint32_t                  t_material_count;             ///< 消耗材料数量
	uint32_t                  t_lock_count;                 ///< 锁定消耗材料数量
	uint32_t                  t_money;                      ///< 消耗材铜币数量

public:
	void reset();
	void fill( const EquipExattrRemakeBase& base );
	const char* getClassName() const { return "z装备表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< EquipExattrRemake, EquipExattrRemakeBase > EquipExattrRemakeManager;

inline void EquipExattrRemake::reset()
{
	this->t_id = 0;
	this->t_material_count = 0;
	this->t_lock_count = 0;
	this->t_money = 0;
};

inline void EquipExattrRemake::fill( const EquipExattrRemakeBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_material_count = base.material_count;
	this->t_lock_count = base.lock_count;
	this->t_money = base.money;
};

inline const std::string EquipExattrRemake::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "EquipExattrRemake.tbls64";
#else
	return "EquipExattrRemake.tbls32";
#endif
};
