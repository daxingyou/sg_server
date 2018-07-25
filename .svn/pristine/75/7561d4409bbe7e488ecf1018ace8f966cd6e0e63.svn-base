#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    表格结构:HeroGradeBase
/// @remark   记录大小:24 X32
///           记录大小:32 X64
///           字段数量:6
///           数据文件:w武将品阶表.xlsx 武将品阶表
///           排序主键:武将id,武将品阶
#pragma pack(1)
struct HeroGradeBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x010DEE06; ///< II|IIVIV
#else
	static const int          FormatCheckCode = 0x010E0FE6; ///< II|IIvIv
#endif

	//------------------------
	uint32_t getUniqueID() const { return hash_util_t::hash_to_uint32(id, grade); }; // TODO: 请用 武将id,武将品阶 生成唯一键

	uint32_t                  id;                           ///< 武将id
	uint32_t                  grade;                        ///< 武将品阶

	uint32_t                  need_level;                   ///< 升阶等级条件
	uint32_t                  need_role_level;              ///< 升阶战队等级
	const char*               consume_material;             ///< 升阶需要材料/数量
	uint32_t                  consume_money;                ///< 升阶消耗铜币
	const char*               attrs;                        ///< 属性串(程序读取用)
};
#pragma pack()

struct HeroGrade : public Entry
{
	uint32_t                  id()                          { return t_id; }
	uint32_t                  grade()                       { return t_grade; }

	uint32_t                  need_level()                  { return t_need_level; }
	uint32_t                  need_role_level() { return t_need_role_level; }
	const std::map<uint32_t, uint32_t>&        consume_material()            { return t_consume_material; }
	uint32_t                  consume_money()               { return t_consume_money; }
	const std::map<uint32_t, uint32_t>&        attrs()                       { return t_attrs; }

private:
	uint32_t                  t_id;                         ///< 武将id
	uint32_t                  t_grade;                      ///< 武将品阶

	uint32_t                  t_need_level;                 ///< 升阶等级条件
	uint32_t                  t_need_role_level;            ///< 升阶战队等级
	std::map<uint32_t, uint32_t>               t_consume_material;           ///< 升阶需要材料/数量
	uint32_t                  t_consume_money;              ///< 升阶消耗铜币
	std::map<uint32_t, uint32_t>               t_attrs;                      ///< 属性串(程序读取用)

public:
	void reset();
	void fill( const HeroGradeBase& base );
	const char* getClassName() const { return "w武将品阶表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< HeroGrade, HeroGradeBase > HeroGradeManager;

inline void HeroGrade::reset()
{
	this->t_id = 0;
	this->t_grade = 0;
	this->t_need_level = 0;
	this->t_need_role_level = 0;
	this->t_consume_material.clear();
	this->t_consume_money = 0;
	this->t_attrs.clear();
};

inline void HeroGrade::fill( const HeroGradeBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_grade = base.grade;
	this->t_need_level = base.need_level;
	this->t_need_role_level = base.need_role_level;
	//if( base.consume_material == NULL ) this->t_consume_material.clear(); else this->t_consume_material.assign( base.consume_material );
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_consume_material, base.consume_material, ":$");
	this->t_consume_money = base.consume_money;
	//if( base.attrs == NULL ) this->t_attrs.clear(); else this->t_attrs.assign( base.attrs );
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_attrs, base.attrs, ":$");
};

inline const std::string HeroGrade::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "HeroGrade.tbls64";
#else
	return "HeroGrade.tbls32";
#endif
};
