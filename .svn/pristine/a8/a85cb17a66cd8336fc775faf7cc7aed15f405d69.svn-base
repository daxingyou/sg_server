#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:HeroTalentPosBase
/// @remark   记录大小:8 X32
///           记录大小:12 X64
///           字段数量:2
///           数据文件:w武将天赋新表.xlsx 天赋方案表
///           排序主键:天赋位置id
#pragma pack(1)
struct HeroTalentPosBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005116; ///< I|V
#else
	static const int          FormatCheckCode = 0x00005136; ///< I|v
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 天赋位置id 生成唯一键

	uint32_t                  id;                           ///< 天赋位置id

	const char*               cond;                         ///< 解锁需要位置
};
#pragma pack()

struct HeroTalentPos : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::map<uint32_t,uint32_t>&        cond()                        { return t_cond; }

private:
	uint32_t                  t_id;                         ///< 天赋位置id

	std::map<uint32_t, uint32_t>               t_cond;                       ///< 解锁需要位置

public:
	void reset();
	void fill( const HeroTalentPosBase& base );
	const char* getClassName() const { return "w武将天赋新表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< HeroTalentPos, HeroTalentPosBase > HeroTalentPosManager;

inline void HeroTalentPos::reset()
{
	this->t_id = 0;
	this->t_cond.clear();
};

inline void HeroTalentPos::fill( const HeroTalentPosBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	//if( base.cond == NULL ) this->t_cond.clear(); else this->t_cond.assign( base.cond );
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_cond, base.cond, ":$");
};

inline const std::string HeroTalentPos::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "HeroTalentPos.tbls64";
#else
	return "HeroTalentPos.tbls32";
#endif
};
