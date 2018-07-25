#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:HeroTalentAddBase
/// @remark   记录大小:28 X32
///           记录大小:28 X64
///           字段数量:7
///           数据文件:w武将天赋觉悟表.xlsx Sheet1
///           排序主键:觉悟层级
#pragma pack(1)
struct HeroTalentAddBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00DDD8C9; ///< I|IIIIII
#else
	static const int          FormatCheckCode = 0x00DDD8C9; ///< I|IIIIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 觉悟层级 生成唯一键

	uint32_t                  id;                           ///< 觉悟层级

	uint32_t                  point_award;                  ///< 每次奖励天赋点
	uint32_t                  level_award;                  ///< 升级奖励天赋点
	uint32_t                  num;                          ///< 升级所需点数
	uint32_t                  material;                     ///< 消耗材料id
	uint32_t                  consume;                      ///< 消耗道具数量
	uint32_t                  money;                        ///< 消耗铜币
};
#pragma pack()

struct HeroTalentAdd : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  point_award()                 { return t_point_award; }
	uint32_t                  level_award()                 { return t_level_award; }
	uint32_t                  num()                         { return t_num; }
	uint32_t                  material()                    { return t_material; }
	uint32_t                  consume()                     { return t_consume; }
	uint32_t                  money()                       { return t_money; }

private:
	uint32_t                  t_id;                         ///< 觉悟层级

	uint32_t                  t_point_award;                ///< 每次奖励天赋点
	uint32_t                  t_level_award;                ///< 升级奖励天赋点
	uint32_t                  t_num;                        ///< 升级所需点数
	uint32_t                  t_material;                   ///< 消耗材料id
	uint32_t                  t_consume;                    ///< 消耗道具数量
	uint32_t                  t_money;                      ///< 消耗铜币

public:
	void reset();
	void fill( const HeroTalentAddBase& base );
	const char* getClassName() const { return "w武将天赋觉悟表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< HeroTalentAdd, HeroTalentAddBase > HeroTalentAddManager;

inline void HeroTalentAdd::reset()
{
	this->t_id = 0;
	this->t_point_award = 0;
	this->t_level_award = 0;
	this->t_num = 0;
	this->t_material = 0;
	this->t_consume = 0;
	this->t_money = 0;
};

inline void HeroTalentAdd::fill( const HeroTalentAddBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_point_award = base.point_award;
	this->t_level_award = base.level_award;
	this->t_num = base.num;
	this->t_material = base.material;
	this->t_consume = base.consume;
	this->t_money = base.money;
};

inline const std::string HeroTalentAdd::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "HeroTalentAdd.tbls64";
#else
	return "HeroTalentAdd.tbls32";
#endif
};
