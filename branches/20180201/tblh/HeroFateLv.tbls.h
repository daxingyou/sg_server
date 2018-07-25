#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:HeroFateLvBase
/// @remark   记录大小:28 X32
///           记录大小:28 X64
///           字段数量:7
///           数据文件:hero_fate_lv.xlsx Sheet1
///           排序主键:羁绊编号,羁绊等级
#pragma pack(1)
struct HeroFateLvBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x010DD939; ///< II|IIIII
#else
	static const int          FormatCheckCode = 0x010DD939; ///< II|IIIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return hash_util_t::hash_to_uint32(id, level); }; // TODO: 请用 羁绊编号,羁绊等级 生成唯一键

	uint32_t                  id;                           ///< 羁绊编号
	uint32_t                  level;                        ///< 羁绊等级

	uint32_t                  status_type;                  ///< 属性类型
	uint32_t                  value_lv;                     ///< 羁绊数值
	uint32_t                  material;                     ///< 消耗材料id
	uint32_t                  consume;                      ///< 消耗数量
	uint32_t                  copper;                       ///< 羁绊1级需要铜币
};
#pragma pack()

struct HeroFateLv : public Entry
{
	uint32_t                  id()                          { return t_id; }
	uint32_t                  level()                       { return t_level; }

	uint32_t                  status_type()                 { return t_status_type; }
	uint32_t                  value_lv()                    { return t_value_lv; }
	uint32_t                  material()                    { return t_material; }
	uint32_t                  consume()                     { return t_consume; }
	uint32_t                  copper()                      { return t_copper; }

private:
	uint32_t                  t_id;                         ///< 羁绊编号
	uint32_t                  t_level;                      ///< 羁绊等级

	uint32_t                  t_status_type;                ///< 属性类型
	uint32_t                  t_value_lv;                   ///< 羁绊数值
	uint32_t                  t_material;                   ///< 消耗材料id
	uint32_t                  t_consume;                    ///< 消耗数量
	uint32_t                  t_copper;                     ///< 羁绊1级需要铜币

public:
	void reset();
	void fill( const HeroFateLvBase& base );
	const char* getClassName() const { return "hero_fate_lv"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< HeroFateLv, HeroFateLvBase > HeroFateLvManager;

inline void HeroFateLv::reset()
{
	this->t_id = 0;
	this->t_level = 0;
	this->t_status_type = 0;
	this->t_value_lv = 0;
	this->t_material = 0;
	this->t_consume = 0;
	this->t_copper = 0;
};

inline void HeroFateLv::fill( const HeroFateLvBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_level = base.level;
	this->t_status_type = base.status_type;
	this->t_value_lv = base.value_lv;
	this->t_material = base.material;
	this->t_consume = base.consume;
	this->t_copper = base.copper;
};

inline const std::string HeroFateLv::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "HeroFateLv.tbls64";
#else
	return "HeroFateLv.tbls32";
#endif
};
