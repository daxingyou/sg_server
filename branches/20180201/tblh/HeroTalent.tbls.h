#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:HeroTalentBase
/// @remark   记录大小:20 X32
///           记录大小:20 X64
///           字段数量:5
///           数据文件:w武将天赋表.xlsx 天赋基本表
///           排序主键:天赋id,天赋觉醒数
#pragma pack(1)
struct HeroTalentBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x04E10DD9; ///< II|III
#else
	static const int          FormatCheckCode = 0x04E10DD9; ///< II|III
#endif

	//------------------------
	uint32_t getUniqueID() const { return hash_util_t::hash_to_uint32(id, wake); }; // TODO: 请用 天赋id,天赋觉醒数 生成唯一键

	uint32_t                  id;                           ///< 天赋id
	uint32_t                  wake;                         ///< 天赋觉醒数

	uint32_t                  consume_talent_point;         ///< 觉醒消耗天赋点
	uint32_t                  max_level;                    ///< 天赋最大等级
	uint32_t                  need_talent_use;              ///< 前置天赋点数
};
#pragma pack()

struct HeroTalent : public Entry
{
	uint32_t                  id()                          { return t_id; }
	uint32_t                  wake()                        { return t_wake; }

	uint32_t                  consume_talent_point()        { return t_consume_talent_point; }
	uint32_t                  max_level()                   { return t_max_level; }
	uint32_t                  need_talent_use()             { return t_need_talent_use; }

private:
	uint32_t                  t_id;                         ///< 天赋id
	uint32_t                  t_wake;                       ///< 天赋觉醒数

	uint32_t                  t_consume_talent_point;       ///< 觉醒消耗天赋点
	uint32_t                  t_max_level;                  ///< 天赋最大等级
	uint32_t                  t_need_talent_use;            ///< 前置天赋点数

public:
	void reset();
	void fill( const HeroTalentBase& base );
	const char* getClassName() const { return "w武将天赋表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< HeroTalent, HeroTalentBase > HeroTalentManager;

inline void HeroTalent::reset()
{
	this->t_id = 0;
	this->t_wake = 0;
	this->t_consume_talent_point = 0;
	this->t_max_level = 0;
	this->t_need_talent_use = 0;
};

inline void HeroTalent::fill( const HeroTalentBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_wake = base.wake;
	this->t_consume_talent_point = base.consume_talent_point;
	this->t_max_level = base.max_level;
	this->t_need_talent_use = base.need_talent_use;
};

inline const std::string HeroTalent::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "HeroTalent.tbls64";
#else
	return "HeroTalent.tbls32";
#endif
};
