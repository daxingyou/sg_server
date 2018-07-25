#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"
#include "common/utility.hpp"

/// @brief    表格结构:MoneyTreeRewardBase
/// @remark   记录大小:8 X32
///           记录大小:12 X64
///           字段数量:2
///           数据文件:y摇钱树.xlsx 基础铜币
///           排序主键:玩家等级
#pragma pack(1)
struct MoneyTreeRewardBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005116; ///< I|V
#else
	static const int          FormatCheckCode = 0x00005136; ///< I|v
#endif

	//------------------------
	uint32_t getUniqueID() const { return lv; }; // TODO: 请用 玩家等级 生成唯一键

	uint32_t                  lv;                           ///< 玩家等级

	const char*               copper;                       ///< 摇钱树获得铜币
};
#pragma pack()

struct MoneyTreeReward : public Entry
{
	uint32_t                  lv()                          { return t_lv; }

	uint32_t get_consum_coin(uint32_t tree_level)
	{
		tree_level = tree_level - 1;
		if (tree_level > t_consum_coin.size())
		{
			return 0;
		}
		return t_consum_coin[tree_level];
	}
private:
	uint32_t                  t_lv;                         ///< 玩家等级

	std::vector<uint32_t>     t_consum_coin;	//获得的铜币
public:
	void reset();
	void fill( const MoneyTreeRewardBase& base );
	const char* getClassName() const { return "y摇钱树"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< MoneyTreeReward, MoneyTreeRewardBase > MoneyTreeRewardManager;

inline void MoneyTreeReward::reset()
{
	this->t_lv = 0;
	this->t_consum_coin.clear();
};

inline void MoneyTreeReward::fill( const MoneyTreeRewardBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_lv = base.lv;
	common::string_util_t::split<uint32_t>(std::string(base.copper ),this->t_consum_coin, "$");
};

inline const std::string MoneyTreeReward::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "MoneyTreeReward.tbls64";
#else
	return "MoneyTreeReward.tbls32";
#endif
};
