#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"
#include "common/utility.hpp"
/// @brief    表格结构:LifeStarBase
/// @remark   记录大小:24 X32
///           记录大小:36 X64
///           字段数量:6
///           数据文件:m命力境界表.xlsx 命星表
///           排序主键:命星集ID,命星序号
#pragma pack(1)
struct LifeStarBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0E11BBE9; ///< II|VVVI
#else
	static const int          FormatCheckCode = 0x0E13DDE9; ///< II|vvvI
#endif

	//------------------------
	uint32_t getUniqueID() const { return common::hash_util_t::hash_to_uint32(id, star_id); }; // TODO: 请用 命星集ID,命星序号 生成唯一键

	uint32_t                  id;                           ///< 命星集ID
	uint32_t                  star_id;                      ///< 命星序号

	const char*               need_item;                    ///< 命星消耗道具
	const char*               other_reward;                 ///< 非属性奖励
	const char*               attr_reward;                  ///< 属性奖励
	uint32_t                  limit_level;                  ///< 开放等级
};
#pragma pack()

struct LifeStar : public Entry
{
	uint32_t                  id()                          { return t_id; }
	uint32_t                  star_id()                     { return t_star_id; }

	const std::vector<uint32_t>&                need_item()                  { return t_need_item; }
	const std::map<uint32_t, uint32_t>&        other_reward()                { return t_other_reward; }
	const std::map<uint32_t, uint32_t>&        attr_reward()                 { return t_attr_reward; }
	uint32_t                  limit_level()                 { return t_limit_level; }

private:
	uint32_t                  t_id;                         ///< 命星集ID
	uint32_t                  t_star_id;                    ///< 命星序号

	std::vector<uint32_t>            t_need_item;          ///< 命星消耗道具
	std::map<uint32_t, uint32_t>     t_other_reward;        ///< 非属性奖励
	std::map<uint32_t, uint32_t>	 t_attr_reward;         ///< 属性奖励
	uint32_t                  t_limit_level;                ///< 开放等级

public:
	void reset();
	void fill( const LifeStarBase& base );
	const char* getClassName() const { return "m命力境界表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< LifeStar, LifeStarBase > LifeStarManager;

inline void LifeStar::reset()
{
	this->t_id = 0;
	this->t_star_id = 0;
	this->t_need_item.clear();
	this->t_other_reward.clear();
	this->t_attr_reward.clear();
	this->t_limit_level = 0;
};

inline void LifeStar::fill( const LifeStarBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_star_id = base.star_id;
	if (base.need_item != NULL)
		string_util_t::split<uint32_t>(std::string(base.need_item), this->t_need_item, ":");
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_other_reward, base.other_reward, ":|");
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_attr_reward, base.attr_reward, ":|");
	this->t_limit_level = base.limit_level;
};

inline const std::string LifeStar::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "LifeStar.tbls64";
#else
	return "LifeStar.tbls32";
#endif
};
