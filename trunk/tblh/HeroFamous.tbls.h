#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    表格结构:HeroFamousBase
/// @remark   记录大小:16 X32
///           记录大小:20 X64
///           字段数量:4
///           数据文件:m名将册.xlsx 名将关联
///           排序主键:关联编号
#pragma pack(1)
struct HeroFamousBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00510EA9; ///< I|IVI
#else
	static const int          FormatCheckCode = 0x005110A9; ///< I|IvI
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 关联编号 生成唯一键

	uint32_t                  id;                           ///< 关联编号

	uint32_t                  hero_id;                      ///< 武将id
	const char*               need_hero;                    ///< 关联武将id
	uint32_t                  famous;                       ///< 名将册属性id
};
#pragma pack()

struct HeroFamous : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  hero_id()                     { return t_hero_id; }
	const std::vector<uint32_t>&        need_hero()                   { return t_need_hero; }
	uint32_t                  famous()                      { return t_famous; }

private:
	uint32_t                  t_id;                         ///< 关联编号

	uint32_t                  t_hero_id;                    ///< 武将id
	std::vector<uint32_t>               t_need_hero;                  ///< 关联武将id
	uint32_t                  t_famous;                     ///< 名将册属性id

public:
	void reset();
	void fill( const HeroFamousBase& base );
	const char* getClassName() const { return "m名将册"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< HeroFamous, HeroFamousBase > HeroFamousManager;

inline void HeroFamous::reset()
{
	this->t_id = 0;
	this->t_hero_id = 0;
	this->t_need_hero.clear();
	this->t_famous = 0;
};

inline void HeroFamous::fill( const HeroFamousBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_hero_id = base.hero_id;
	//if( base.need_hero == NULL ) this->t_need_hero.clear(); else this->t_need_hero.assign( base.need_hero );
	Dawn::parseContainer<uint32_t>(this->t_need_hero, base.need_hero, "$");
	this->t_famous = base.famous;
};

inline const std::string HeroFamous::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "HeroFamous.tbls64";
#else
	return "HeroFamous.tbls32";
#endif
};
