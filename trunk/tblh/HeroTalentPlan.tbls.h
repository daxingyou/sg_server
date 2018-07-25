#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "MyTuple.h"

/// @brief    表格结构:HeroTalentPlanBase
/// @remark   记录大小:8 X32
///           记录大小:12 X64
///           字段数量:2
///           数据文件:w武将天赋新表.xlsx 天赋方案表
///           排序主键:天赋方案id
#pragma pack(1)
struct HeroTalentPlanBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005116; ///< I|V
#else
	static const int          FormatCheckCode = 0x00005136; ///< I|v
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 天赋方案id 生成唯一键

	uint32_t                  id;                           ///< 天赋方案id

	const char*               plan;                         ///< 天赋方案
};
#pragma pack()

struct HeroTalentPlan : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::vector<std::pair<uint32_t,uint32_t>>&        plan()                        { return t_plan; }

private:
	uint32_t                  t_id;                         ///< 天赋方案id

	std::vector<std::pair<uint32_t, uint32_t>>               t_plan;                       ///< 天赋方案

public:
	void reset();
	void fill( const HeroTalentPlanBase& base );
	const char* getClassName() const { return "w武将天赋新表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< HeroTalentPlan, HeroTalentPlanBase > HeroTalentPlanManager;

inline void HeroTalentPlan::reset()
{
	this->t_id = 0;
	this->t_plan.clear();
};

inline void HeroTalentPlan::fill( const HeroTalentPlanBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	//if( base.plan == NULL ) this->t_plan.clear(); else this->t_plan.assign( base.plan );
	Dawn::parseTupleContainer<std::pair<uint32_t,uint32_t>>(this->t_plan, base.plan, ":$");
};

inline const std::string HeroTalentPlan::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "HeroTalentPlan.tbls64";
#else
	return "HeroTalentPlan.tbls32";
#endif
};
