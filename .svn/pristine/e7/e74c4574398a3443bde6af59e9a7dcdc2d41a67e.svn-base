#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"
#include "common/utility.hpp"
/// @brief    表格结构:HeroPluginBase
/// @remark   记录大小:20 X32
///           记录大小:24 X64
///           字段数量:5
///           数据文件:w武将皮肤关联表.xlsx Sheet1
///           排序主键:模型id
#pragma pack(1)
struct HeroPluginBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0510DDE6; ///< I|IIIV
#else
	static const int          FormatCheckCode = 0x0510DE06; ///< I|IIIv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 模型id 生成唯一键

	uint32_t                  id;                           ///< 模型id

	uint32_t                  hero_id;                      ///< 武将id
	uint32_t                  type;                         ///< 模型类型
	uint32_t                  unlock_cond;                  ///< 解锁条件
	const char*               attr;                         ///< 时装属性列
};
#pragma pack()

struct HeroPlugin : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  hero_id()                     { return t_hero_id; }
	uint32_t                  type()                        { return t_type; }
	uint32_t                  unlock_cond()                 { return t_unlock_cond; }
	const std::map<uint32_t, uint32_t>&  attr()             { return t_attr; }

private:
	uint32_t                  t_id;                         ///< 模型id

	uint32_t                  t_hero_id;                    ///< 武将id
	uint32_t                  t_type;                       ///< 模型类型
	uint32_t                  t_unlock_cond;                ///< 解锁条件
	std::map<uint32_t, uint32_t>     t_attr;                ///< 时装属性列

public:
	void reset();
	void fill( const HeroPluginBase& base );
	const char* getClassName() const { return "w武将皮肤关联表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< HeroPlugin, HeroPluginBase > HeroPluginManager;

inline void HeroPlugin::reset()
{
	this->t_id = 0;
	this->t_hero_id = 0;
	this->t_type = 0;
	this->t_unlock_cond = 0;
	this->t_attr.clear();
};

inline void HeroPlugin::fill( const HeroPluginBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_hero_id = base.hero_id;
	this->t_type = base.type;
	this->t_unlock_cond = base.unlock_cond;
	///if( base.attr == NULL ) this->t_attr.clear(); else this->t_attr.assign( base.attr );
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_attr, base.attr, ":|");
};

inline const std::string HeroPlugin::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "HeroPlugin.tbls64";
#else
	return "HeroPlugin.tbls32";
#endif
};
