#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:HeroTalentLevelBase
/// @remark   记录大小:20 X32
///           记录大小:24 X64
///           字段数量:5
///           数据文件:w武将天赋新表.xlsx 天赋基础表
///           排序主键:天赋id,天赋等级
#pragma pack(1)
struct HeroTalentLevelBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x04E11AD9; ///< II|VII
#else
	static const int          FormatCheckCode = 0x04E13AD9; ///< II|vII
#endif

	//------------------------
	uint32_t getUniqueID() const { return hash_util_t::hash_to_uint32(id, level); }; // TODO: 请用 天赋id,天赋等级 生成唯一键

	uint32_t                  id;                           ///< 天赋id
	uint32_t                  level;                        ///< 天赋等级

	const char*               add_value;                    ///< 附加属性
	uint32_t                  consume_talent_point;         ///< 升级消耗天赋点
	uint32_t                  need_level;                   ///< 武将等级限制
};
#pragma pack()

struct HeroTalentLevel : public Entry
{
	uint32_t                  id()                          { return t_id; }
	uint32_t                  level()                       { return t_level; }

	const std::map<uint32_t,uint32_t>&        add_value()                   { return t_add_value; }
	uint32_t                  consume_talent_point()        { return t_consume_talent_point; }
	uint32_t                  need_level()                  { return t_need_level; }

private:
	uint32_t                  t_id;                         ///< 天赋id
	uint32_t                  t_level;                      ///< 天赋等级

	std::map<uint32_t, uint32_t>               t_add_value;                  ///< 附加属性
	uint32_t                  t_consume_talent_point;       ///< 升级消耗天赋点
	uint32_t                  t_need_level;                 ///< 武将等级限制

public:
	void reset();
	void fill( const HeroTalentLevelBase& base );
	const char* getClassName() const { return "w武将天赋新表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< HeroTalentLevel, HeroTalentLevelBase > HeroTalentLevelManager;

inline void HeroTalentLevel::reset()
{
	this->t_id = 0;
	this->t_level = 0;
	this->t_add_value.clear();
	this->t_consume_talent_point = 0;
	this->t_need_level = 0;
};

inline void HeroTalentLevel::fill( const HeroTalentLevelBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_level = base.level;
	//if( base.add_value == NULL ) this->t_add_value.clear(); else this->t_add_value.assign( base.add_value );
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_add_value, base.add_value, ":$");
	this->t_consume_talent_point = base.consume_talent_point;
	this->t_need_level = base.need_level;
};

inline const std::string HeroTalentLevel::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "HeroTalentLevel.tbls64";
#else
	return "HeroTalentLevel.tbls32";
#endif
};
