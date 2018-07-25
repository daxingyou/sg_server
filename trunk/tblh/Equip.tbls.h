#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    表格结构:EquipBase
/// @remark   记录大小:68 X32
///           记录大小:80 X64
///           字段数量:17
///           数据文件:z装备表.xlsx 装备基础表
///           排序主键:装备id
#pragma pack(1)
struct EquipBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0660EBA9; ///< I|IIIIIIIVVIVIIIII
#else
	static const int          FormatCheckCode = 0x08630D59; ///< I|IIIIIIIvvIvIIIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 装备id 生成唯一键

	uint32_t                  id;                           ///< 装备id

	uint32_t                  search_id;                    ///< 寄售查询ID
	uint32_t                  equip_type;                   ///< 装备类型
	uint32_t                  star;                         ///< 装备星级
	uint32_t                  need_level;                   ///< 需求等级
	uint32_t                  base_atk;                     ///< 基础攻击
	uint32_t                  base_def;                     ///< 基础防御
	uint32_t                  base_hp;                      ///< 基础最大血量
	const char*               attr_growth;                  ///< 装备成长属性串
	const char*               name_pool;                    ///< 词缀池
	uint32_t                  exattr_pool;                  ///< 判词池ID
	const char*               special_pool;                 ///< 特技池ID
	uint32_t                  suit_id;                      ///< 套装id
	uint32_t                  smelt_id;                     ///< 熔炼获得ID
	uint32_t                  can_remake;                   ///< 是否可洗练
	uint32_t                  can_smelt;                    ///< 是否可熔炼
	uint32_t                  score;                        ///< 装备评分
};
#pragma pack()

struct Equip : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  search_id()                   { return t_search_id; }
	uint32_t                  equip_type()                  { return t_equip_type; }
	uint32_t                  star() { return t_star; }
	uint32_t                  need_level() { return t_need_level; }
	uint32_t                  base_atk()                    { return t_base_atk; }
	uint32_t                  base_def()                    { return t_base_def; }
	uint32_t                  base_hp()                     { return t_base_hp; }
	const std::map<uint32_t,uint32_t>&        attr_growth()                 { return t_attr_growth; }
	const std::pair<uint32_t,uint32_t>&        name_pool()                   { return t_name_pool; }
	uint32_t                  exattr_pool()                 { return t_exattr_pool; }
	const std::pair<uint32_t, uint32_t>&        special_pool()                { return t_special_pool; }
	uint32_t                  suit_id()                     { return t_suit_id; }
	uint32_t                  smelt_id()                    { return t_smelt_id; }
	uint32_t                  can_remake() { return t_can_remake; }
	uint32_t                  can_smelt() { return t_can_smelt; }
	uint32_t                  score() { return t_score; }

private:
	uint32_t                  t_id;                         ///< 装备id

	uint32_t                  t_search_id;                  ///< 寄售查询ID
	uint32_t                  t_equip_type;                 ///< 装备类型
	uint32_t                  t_star;                       ///< 装备星级
	uint32_t                  t_need_level;                 ///< 需求等级
	uint32_t                  t_base_atk;                   ///< 基础攻击
	uint32_t                  t_base_def;                   ///< 基础防御
	uint32_t                  t_base_hp;                    ///< 基础最大血量
	std::map<uint32_t, uint32_t>               t_attr_growth;                ///< 装备成长属性串
	std::pair<uint32_t, uint32_t>               t_name_pool;                  ///< 词缀池
	uint32_t                  t_exattr_pool;                ///< 判词池ID
	std::pair<uint32_t, uint32_t>               t_special_pool;               ///< 特技池ID
	uint32_t                  t_suit_id;                    ///< 套装id
	uint32_t                  t_smelt_id;                   ///< 熔炼获得ID
	uint32_t                  t_can_remake;                 ///< 是否可洗练
	uint32_t                  t_can_smelt;                  ///< 是否可熔炼
	uint32_t                  t_score;                      ///< 装备评分

public:
	void reset();
	void fill( const EquipBase& base );
	const char* getClassName() const { return "z装备表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Equip, EquipBase > EquipManager;

inline void Equip::reset()
{
	this->t_id = 0;
	this->t_search_id = 0;
	this->t_equip_type = 0;
	this->t_star = 0;
	this->t_need_level = 0;
	this->t_base_atk = 0;
	this->t_base_def = 0;
	this->t_base_hp = 0;
	this->t_attr_growth.clear();
	
	this->t_exattr_pool = 0;
	
	this->t_suit_id = 0;
	this->t_smelt_id = 0;
	this->t_can_remake = 0;
	this->t_can_smelt = 0;
	this->t_score = 0;
};

inline void Equip::fill( const EquipBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_search_id = base.search_id;
	this->t_equip_type = base.equip_type;
	this->t_star = base.star;
	this->t_need_level = base.need_level;
	this->t_base_atk = base.base_atk;
	this->t_base_def = base.base_def;
	this->t_base_hp = base.base_hp;
	//if( base.attr_growth == NULL ) this->t_attr_growth.clear(); else this->t_attr_growth.assign( base.attr_growth );
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_attr_growth, base.attr_growth, ":$");
	//if( base.name_pool == NULL ) this->t_name_pool.clear(); else this->t_name_pool.assign( base.name_pool );
	Dawn::parseTuple(this->t_name_pool, base.name_pool, ":");
	this->t_exattr_pool = base.exattr_pool;
	//if( base.special_pool == NULL ) this->t_special_pool.clear(); else this->t_special_pool.assign( base.special_pool );
	Dawn::parseTuple(this->t_special_pool, base.special_pool, ":");
	this->t_suit_id = base.suit_id;
	this->t_smelt_id = base.smelt_id;
	this->t_can_remake = base.can_remake;
	this->t_can_smelt = base.can_smelt;
	this->t_score = base.score;
};

inline const std::string Equip::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Equip.tbls64";
#else
	return "Equip.tbls32";
#endif
};
