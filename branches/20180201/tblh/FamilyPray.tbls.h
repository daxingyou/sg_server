#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:FamilyPrayBase
/// @remark   记录大小:8 X32
///           记录大小:12 X64
///           字段数量:2
///           数据文件:j家族表.xlsx family_pray
///           排序主键:赠予次数
#pragma pack(1)
struct FamilyPrayBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005116; ///< I|V
#else
	static const int          FormatCheckCode = 0x00005136; ///< I|v
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 赠予次数 生成唯一键

	uint32_t                  id;                           ///< 赠予次数

	const char*               drop;                         ///< 奖励道具id
};
#pragma pack()

struct FamilyPray : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::map<uint32_t, uint32_t>& drop()              { return t_drop; }

private:
	uint32_t                  t_id;                         ///< 赠予次数

	std::map<uint32_t, uint32_t> t_drop;					///< 奖励道具id

public:
	void reset();
	void fill( const FamilyPrayBase& base );
	const char* getClassName() const { return "j家族表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< FamilyPray, FamilyPrayBase > FamilyPrayManager;

inline void FamilyPray::reset()
{
	this->t_id = 0;
	this->t_drop.clear();
};

inline void FamilyPray::fill( const FamilyPrayBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;

	if (base.drop != NULL)
	{
		common::Misc::Parse2SeperatorDataInfo(std::string(base.drop), this->t_drop, "$");
	}
};

inline const std::string FamilyPray::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "FamilyPray.tbls64";
#else
	return "FamilyPray.tbls32";
#endif
};
