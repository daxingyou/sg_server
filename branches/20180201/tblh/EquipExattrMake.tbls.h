#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:EquipExattrMakeBase
/// @remark   记录大小:8 X32
///           记录大小:8 X64
///           字段数量:2
///           数据文件:z装备表.xlsx 判词洗练
///           排序主键:装备品质
#pragma pack(1)
struct EquipExattrMakeBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#else
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 装备品质 生成唯一键

	uint32_t                  id;                           ///< 装备品质

	uint32_t                  count;                        ///< 判词数量
};
#pragma pack()

struct EquipExattrMake : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  count()                       { return t_count; }

private:
	uint32_t                  t_id;                         ///< 装备品质

	uint32_t                  t_count;                      ///< 判词数量

public:
	void reset();
	void fill( const EquipExattrMakeBase& base );
	const char* getClassName() const { return "z装备表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< EquipExattrMake, EquipExattrMakeBase > EquipExattrMakeManager;

inline void EquipExattrMake::reset()
{
	this->t_id = 0;
	this->t_count = 0;
};

inline void EquipExattrMake::fill( const EquipExattrMakeBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_count = base.count;
};

inline const std::string EquipExattrMake::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "EquipExattrMake.tbls64";
#else
	return "EquipExattrMake.tbls32";
#endif
};
