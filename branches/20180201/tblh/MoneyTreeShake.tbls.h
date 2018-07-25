#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    表格结构:MoneyTreeShakeBase
/// @remark   记录大小:8 X32
///           记录大小:12 X64
///           字段数量:2
///           数据文件:y摇钱树.xlsx 花费
///           排序主键:摇一摇次数
#pragma pack(1)
struct MoneyTreeShakeBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005116; ///< I|V
#else
	static const int          FormatCheckCode = 0x00005136; ///< I|v
#endif

	//------------------------
	uint32_t getUniqueID() const { return time; }; // TODO: 请用 摇一摇次数 生成唯一键

	uint32_t                  time;                         ///< 摇一摇次数

	const char*               consum;                       ///< 摇钱树花费元宝
};
#pragma pack()

struct MoneyTreeShake : public Entry
{
	uint32_t                  time()                        { return t_time; }

	uint32_t				 get_cost_money(uint32_t tree_level)
	{
		tree_level = tree_level - 1;
		if (tree_level > t_consum_money.size())
		{
			return 0;
		}
		return t_consum_money[tree_level];
	}
private:
	uint32_t                  t_time;                       ///< 摇一摇次数

	std::vector<uint32_t>     t_consum_money;
public:
	void reset();
	void fill( const MoneyTreeShakeBase& base );
	const char* getClassName() const { return "y摇钱树"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< MoneyTreeShake, MoneyTreeShakeBase > MoneyTreeShakeManager;

inline void MoneyTreeShake::reset()
{
	this->t_time = 0;
	this->t_consum_money.clear();
};

inline void MoneyTreeShake::fill( const MoneyTreeShakeBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_time = base.time;
	common::string_util_t::split<uint32_t>(std::string(base.consum), this->t_consum_money, "$");
};

inline const std::string MoneyTreeShake::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "MoneyTreeShake.tbls64";
#else
	return "MoneyTreeShake.tbls32";
#endif
};
