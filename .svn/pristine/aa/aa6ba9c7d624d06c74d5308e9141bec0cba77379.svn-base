#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/MyTuple.h"
#include "common/ContainerParse.h"
#include "common/utility.hpp"
#include <set>
/// @brief    表格结构:ActiveRewardBase
/// @remark   记录大小:20 X32
///           记录大小:28 X64
///           字段数量:5
///           数据文件:h活动表.xlsx 活跃列表
///           排序主键:ID
#pragma pack(1)
struct ActiveRewardBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0511AEA9; ///< I|VIVI
#else
	static const int          FormatCheckCode = 0x0513B0A9; ///< I|vIvI
#endif

	//------------------------
	uint32_t getUniqueID() const { return ID; }; // TODO: 请用 ID 生成唯一键

	uint32_t                  ID;                           ///< ID

	const char*               condition;                    ///< 完成条件
	uint32_t                  drop_id;                      ///< 完成奖励
	const char*               level_range;                  ///< 等级段
	uint32_t                  liveness;                     ///< 活跃值
};
#pragma pack()

struct ActiveReward : public Entry
{
	uint32_t                  ID()                          { return t_ID; }

	const Dawn::triple< uint32_t, uint32_t, uint32_t>&        condition()                   { return t_condition; }
	uint32_t                  drop_id()                     { return t_drop_id; }
	const std::string&        level_range()                 { return t_level_range; }
	uint32_t                  liveness()                    { return t_liveness; }
	const std::set<uint32_t>&		  level_range_set()				{ return t_level_range_set; }
private:
	uint32_t                  t_ID;                         ///< ID

	Dawn::triple< uint32_t, uint32_t, uint32_t>               t_condition;                  ///< 完成条件
	uint32_t                  t_drop_id;                    ///< 完成奖励
	std::string               t_level_range;                ///< 等级段
	uint32_t                  t_liveness;                   ///< 活跃值
	std::set<uint32_t>		  t_level_range_set;			///< 等级段列表

public:
	void reset();
	void fill( const ActiveRewardBase& base );
	const char* getClassName() const { return "h活动表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< ActiveReward, ActiveRewardBase > ActiveRewardManager;

inline void ActiveReward::reset()
{
	this->t_ID = 0;
	this->t_condition.clear();
	this->t_drop_id = 0;
	this->t_level_range.clear();
	this->t_liveness = 0;
	this->t_level_range_set.clear();
};

inline void ActiveReward::fill( const ActiveRewardBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_ID = base.ID;
	//if( base.condition == NULL ) this->t_condition.clear(); else this->t_condition.assign( base.condition );
	Dawn::parseTuple(this->t_condition, base.condition, ":");
	this->t_drop_id = base.drop_id;
	if( base.level_range == NULL ) this->t_level_range.clear(); else this->t_level_range.assign( base.level_range );
	this->t_liveness = base.liveness;
	if (NULL != base.level_range)
	{
		std::vector<uint32_t> vec;
		common::string_util_t::split<uint32_t>(std::string(base.level_range), vec, "$");
		for (auto& it : vec)
		{
			t_level_range_set.insert(it);
		}
	}
};

inline const std::string ActiveReward::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "ActiveReward.tbls64";
#else
	return "ActiveReward.tbls32";
#endif
};
