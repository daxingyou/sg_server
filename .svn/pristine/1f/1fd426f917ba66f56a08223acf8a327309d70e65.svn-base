#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:FamilySkillBase
/// @remark   记录大小:24 X32
///           记录大小:32 X64
///           字段数量:6
///           数据文件:j家族技能.xlsx 后端
///           排序主键:技能编号,等级
#pragma pack(1)
struct FamilySkillBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0E10DEF6; ///< II|IIVV
#else
	static const int          FormatCheckCode = 0x0E10E096; ///< II|IIvv
#endif

	//------------------------
	uint32_t getUniqueID() const { return hash_util_t::hash_to_uint32(id, level); }; // TODO: 请用 技能编号,等级 生成唯一键

	uint32_t                  id;                           ///< 技能编号
	uint32_t                  level;                        ///< 等级

	uint32_t                  need_role_level;              ///< 需求角色等级
	uint32_t                  need_family_level;            ///< 需求家族等级
	const char*               cost_money;                   ///< 升级消耗
	const char*               attr;                         ///< 增加属性
};
#pragma pack()

struct FamilySkill : public Entry
{
	uint32_t                  id()                          { return t_id; }
	uint32_t                  level()                       { return t_level; }

	uint32_t                  need_role_level()             { return t_need_role_level; }
	uint32_t                  need_family_level()           { return t_need_family_level; }
	const std::pair<uint32_t,uint32_t>&        cost_money()                  { return t_cost_money; }
	const std::map<uint32_t,uint32_t>&        attr()                        { return t_attr; }

private:
	uint32_t                  t_id;                         ///< 技能编号
	uint32_t                  t_level;                      ///< 等级

	uint32_t                  t_need_role_level;            ///< 需求角色等级
	uint32_t                  t_need_family_level;          ///< 需求家族等级
	std::pair<uint32_t, uint32_t>               t_cost_money;                 ///< 升级消耗
	std::map<uint32_t, uint32_t>               t_attr;                       ///< 增加属性

public:
	void reset();
	void fill( const FamilySkillBase& base );
	const char* getClassName() const { return "j家族技能"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< FamilySkill, FamilySkillBase > FamilySkillManager;

inline void FamilySkill::reset()
{
	this->t_id = 0;
	this->t_level = 0;
	this->t_need_role_level = 0;
	this->t_need_family_level = 0;
	//this->t_cost_money.clear();
	this->t_attr.clear();
};

inline void FamilySkill::fill( const FamilySkillBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_level = base.level;
	this->t_need_role_level = base.need_role_level;
	this->t_need_family_level = base.need_family_level;
	//if( base.cost_money == NULL ) this->t_cost_money.clear(); else this->t_cost_money.assign( base.cost_money );
	Dawn::parseTuple(this->t_cost_money, base.cost_money, ":");
	//if( base.attr == NULL ) this->t_attr.clear(); else this->t_attr.assign( base.attr );
	Dawn::parseMapContainer(this->t_attr, base.attr, ":$");
};

inline const std::string FamilySkill::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "FamilySkill.tbls64";
#else
	return "FamilySkill.tbls32";
#endif
};
