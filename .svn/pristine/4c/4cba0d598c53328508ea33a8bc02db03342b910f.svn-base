#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:StarDemadBase
/// @remark   记录大小:24 X32
///           记录大小:24 X64
///           字段数量:6
///           数据文件:star_demad.xlsx Sheet1
///           排序主键:曲线编号,星级
#pragma pack(1)
struct StarDemadBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0E10DD99; ///< II|IIII
#else
	static const int          FormatCheckCode = 0x0E10DD99; ///< II|IIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return hash_util_t::hash_to_uint32(id, hero_star); }; // TODO: 请用 曲线编号,星级 生成唯一键

	uint32_t                  id;                           ///< 曲线编号
	uint32_t                  hero_star;                    ///< 星级

	uint32_t                  need_level;                   ///< 升级需求等级
	uint32_t                  consume;                      ///< 消耗材料数量
	uint32_t                  status_value;                 ///< 增加属性类型
	uint32_t                  grade_value;                  ///< 增加数值
};
#pragma pack()

struct StarDemad : public Entry
{
	uint32_t                  id()                          { return t_id; }
	uint32_t                  hero_star()                   { return t_hero_star; }

	uint32_t                  need_level()                  { return t_need_level; }
	uint32_t                  consume()                     { return t_consume; }
	uint32_t                  status_value()                { return t_status_value; }
	uint32_t                  grade_value()                 { return t_grade_value; }

private:
	uint32_t                  t_id;                         ///< 曲线编号
	uint32_t                  t_hero_star;                  ///< 星级

	uint32_t                  t_need_level;                 ///< 升级需求等级
	uint32_t                  t_consume;                    ///< 消耗材料数量
	uint32_t                  t_status_value;               ///< 增加属性类型
	uint32_t                  t_grade_value;                ///< 增加数值

public:
	void reset();
	void fill( const StarDemadBase& base );
	const char* getClassName() const { return "star_demad"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< StarDemad, StarDemadBase > StarDemadManager;

inline void StarDemad::reset()
{
	this->t_id = 0;
	this->t_hero_star = 0;
	this->t_need_level = 0;
	this->t_consume = 0;
	this->t_status_value = 0;
	this->t_grade_value = 0;
};

inline void StarDemad::fill( const StarDemadBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_hero_star = base.hero_star;
	this->t_need_level = base.need_level;
	this->t_consume = base.consume;
	this->t_status_value = base.status_value;
	this->t_grade_value = base.grade_value;
};

inline const std::string StarDemad::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "StarDemad.tbls64";
#else
	return "StarDemad.tbls32";
#endif
};
