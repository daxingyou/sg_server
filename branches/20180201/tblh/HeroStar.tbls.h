#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    表格结构:HeroStarBase
/// @remark   记录大小:36 X32
///           记录大小:44 X64
///           字段数量:9
///           数据文件:w武将星级表.xlsx 武将星级表
///           排序主键:武将id,武将星级
#pragma pack(1)
struct HeroStarBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0CB0DD19; ///< II|iViiViii
#else
	static const int          FormatCheckCode = 0x0CCEDCF9; ///< II|iviiviii
#endif

	//------------------------
	uint32_t getUniqueID() const { return hash_util_t::hash_to_uint32(id, star); }; // TODO: 请用 武将id,武将星级 生成唯一键

	uint32_t                  id;                           ///< 武将id
	uint32_t                  star;                         ///< 武将星级

	int32_t                   need_level;                   ///< 升星等级条件
	const char*               consume_material;             ///< 升星需要材料/数量
	int32_t                   consume_money;                ///< 升星消耗铜币
	int32_t                   consume_herosoul;             ///< 升星消耗将魂
	const char*               attrs;                        ///< 属性串(程序读取用)
	int32_t                   atk_addval;                   ///< 攻击成长
	int32_t                   def_addval;                   ///< 防御成长
	int32_t                   hp_addval;                    ///< 生命值成长
};
#pragma pack()

struct HeroStar : public Entry
{
	uint32_t                  id()                          { return t_id; }
	uint32_t                  star()                        { return t_star; }

	int32_t                   need_level()                  { return t_need_level; }
	const std::map<uint32_t, uint32_t>&        consume_material()            { return t_consume_material; }
	int32_t                   consume_money()               { return t_consume_money; }
	int32_t                   consume_herosoul() { return t_consume_herosoul; }
	const std::map<uint32_t, uint32_t>&        attrs()                       { return t_attrs; }
	int32_t                   atk_addval()                  { return t_atk_addval; }
	int32_t                   def_addval()                  { return t_def_addval; }
	int32_t                   hp_addval()                   { return t_hp_addval; }

private:
	uint32_t                  t_id;                         ///< 武将id
	uint32_t                  t_star;                       ///< 武将星级

	int32_t                   t_need_level;                 ///< 升星等级条件
	std::map<uint32_t, uint32_t>               t_consume_material;           ///< 升星需要材料/数量
	int32_t                   t_consume_money;              ///< 升星消耗铜币
	int32_t                   t_consume_herosoul;           ///< 升星消耗将魂
	std::map<uint32_t, uint32_t>               t_attrs;                      ///< 属性串(程序读取用)
	int32_t                   t_atk_addval;                 ///< 攻击成长
	int32_t                   t_def_addval;                 ///< 防御成长
	int32_t                   t_hp_addval;                  ///< 生命值成长

public:
	void reset();
	void fill( const HeroStarBase& base );
	const char* getClassName() const { return "w武将星级表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< HeroStar, HeroStarBase > HeroStarManager;

inline void HeroStar::reset()
{
	this->t_id = 0;
	this->t_star = 0;
	this->t_need_level = 0;
	this->t_consume_material.clear();
	this->t_consume_money = 0;
	this->t_consume_herosoul = 0;
	this->t_attrs.clear();
	this->t_atk_addval = 0;
	this->t_def_addval = 0;
	this->t_hp_addval = 0;
};

inline void HeroStar::fill( const HeroStarBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_star = base.star;
	this->t_need_level = base.need_level;
	//if( base.consume_material == NULL ) this->t_consume_material.clear(); else this->t_consume_material.assign( base.consume_material );
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_consume_material, base.consume_material, ":$");
	this->t_consume_money = base.consume_money;
	this->t_consume_herosoul = base.consume_herosoul;
	//if( base.attrs == NULL ) this->t_attrs.clear(); else this->t_attrs.assign( base.attrs );
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_attrs, base.attrs, ":$");
	this->t_atk_addval = base.atk_addval;
	this->t_def_addval = base.def_addval;
	this->t_hp_addval = base.hp_addval;
};

inline const std::string HeroStar::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "HeroStar.tbls64";
#else
	return "HeroStar.tbls32";
#endif
};
