#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    表格结构:MoneyTreeTableBase
/// @remark   记录大小:20 X32
///           记录大小:28 X64
///           字段数量:5
///           数据文件:y摇钱树.xlsx 摇钱树
///           排序主键:摇钱树等级
#pragma pack(1)
struct MoneyTreeTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0511ADE6; ///< I|VIIV
#else
	static const int          FormatCheckCode = 0x0513AE06; ///< I|vIIv
#endif

	//------------------------
	uint32_t getUniqueID() const { return lv; }; // TODO: 请用 摇钱树等级 生成唯一键

	uint32_t                  lv;                           ///< 摇钱树等级

	const char*               consum;                       ///< 升级消耗道具和数量
	uint32_t                  max_count;                    ///< 摇一摇次数
	uint32_t                  exGet;                        ///< 额外获得
	const char*               crit_chance;                  ///< 暴击倍数及概率
};
#pragma pack()

struct MoneyTreeTable : public Entry
{
	uint32_t                  lv()                          { return t_lv; }

	uint32_t                  max_count()                   { return t_max_count; }
	uint32_t                  exGet()                       { return t_exGet; }

	std::map<uint32_t, uint32_t>    &get_levelup_cost_map() { return t_levelup_cost; };				//升级消耗
	std::map<uint32_t, uint32_t>    &get_crit_chance_map() { return t_crit_chance;  };				//升级消耗
private:
	uint32_t                  t_lv;                         ///< 摇钱树等级

	uint32_t                  t_max_count;                  ///< 摇一摇次数
	uint32_t                  t_exGet;                      ///< 额外获得

	std::map<uint32_t,uint32_t>     t_levelup_cost;				//升级消耗
	std::map<uint32_t,uint32_t>     t_crit_chance;				//升级消耗
public:
	void reset();
	void fill( const MoneyTreeTableBase& base );
	const char* getClassName() const { return "y摇钱树"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< MoneyTreeTable, MoneyTreeTableBase > MoneyTreeTableManager;

inline void MoneyTreeTable::reset()
{
	this->t_lv = 0;
	this->t_max_count = 0;
	this->t_exGet = 0;
	this->t_crit_chance.clear();
	this->t_levelup_cost.clear();
};

inline void MoneyTreeTable::fill( const MoneyTreeTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_lv = base.lv;
	this->t_max_count = base.max_count;
	this->t_exGet = base.exGet;

	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_crit_chance, base.crit_chance, "$:");
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_levelup_cost, base.consum, "$:");
};

inline const std::string MoneyTreeTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "MoneyTreeTable.tbls64";
#else
	return "MoneyTreeTable.tbls32";
#endif
};
