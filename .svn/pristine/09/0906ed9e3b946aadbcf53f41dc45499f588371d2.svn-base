#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:HeroTalentAddBase
/// @remark   记录大小:20 X32
///           记录大小:28 X64
///           字段数量:5
///           数据文件:w武将天赋觉悟新表.xlsx Sheet1
///           排序主键:觉悟等级
#pragma pack(1)
struct HeroTalentAddBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x010DDEE6; ///< I|IIIVV
#else
	static const int          FormatCheckCode = 0x010DE086; ///< I|IIIvv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 觉悟等级 生成唯一键

	uint32_t                  id;                           ///< 觉悟等级

	uint32_t                  level_award;                  ///< 升级奖励天赋点
	uint32_t                  degree_award;                 ///< 升段奖励天赋点
	uint32_t                  degree;                       ///< 觉悟等级分段
	const char*               level_consume;                ///< 升级消耗材料
	const char*               degree_consume;               ///< 升段消耗材料
};
#pragma pack()

struct HeroTalentAdd : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  level_award()                 { return t_level_award; }
	uint32_t                  degree_award()                { return t_degree_award; }
	uint32_t                  degree() { return t_degree; }
	const std::map<uint32_t,uint32_t>&        level_consume()               { return t_level_consume; }
	const std::map<uint32_t, uint32_t>&        degree_consume()              { return t_degree_consume; }

private:
	uint32_t                  t_id;                         ///< 觉悟等级

	uint32_t                  t_level_award;                ///< 升级奖励天赋点
	uint32_t                  t_degree_award;               ///< 升段奖励天赋点
	uint32_t                  t_degree;                     ///< 觉悟等级分段
	std::map<uint32_t, uint32_t>               t_level_consume;              ///< 升级消耗材料
	std::map<uint32_t, uint32_t>               t_degree_consume;             ///< 升段消耗材料

public:
	void reset();
	void fill( const HeroTalentAddBase& base );
	const char* getClassName() const { return "w武将天赋觉悟新表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< HeroTalentAdd, HeroTalentAddBase > HeroTalentAddManager;

inline void HeroTalentAdd::reset()
{
	this->t_id = 0;
	this->t_level_award = 0;
	this->t_degree_award = 0;
	this->t_degree = 0;
	this->t_level_consume.clear();
	this->t_degree_consume.clear();
};

inline void HeroTalentAdd::fill( const HeroTalentAddBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_level_award = base.level_award;
	this->t_degree_award = base.degree_award;
	this->t_degree = base.degree;
	//if( base.level_consume == NULL ) this->t_level_consume.clear(); else this->t_level_consume.assign( base.level_consume );
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_level_consume, base.level_consume, ":$");
	//if( base.degree_consume == NULL ) this->t_degree_consume.clear(); else this->t_degree_consume.assign( base.degree_consume );
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_degree_consume, base.degree_consume, ":$");
};

inline const std::string HeroTalentAdd::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "HeroTalentAdd.tbls64";
#else
	return "HeroTalentAdd.tbls32";
#endif
};
