#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:DungeonTypeBase
/// @remark   记录大小:16 X32
///           记录大小:20 X64
///           字段数量:4
///           数据文件:f副本表.xlsx dungeon_type
///           排序主键:副本分类
#pragma pack(1)
struct DungeonTypeBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00510DE6; ///< I|IIV
#else
	static const int          FormatCheckCode = 0x00510E06; ///< I|IIv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 副本分类 生成唯一键

	uint32_t                  id;                           ///< 副本分类

	uint32_t                  fight_times;                  ///< 每日进入次数
	uint32_t                  buy_times;                    ///< 购买次数上限
	const char*               buy_cost;                     ///< 购买花费元宝
};
#pragma pack()

struct DungeonType : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  fight_times()                 { return t_fight_times; }
	uint32_t                  buy_times()                   { return t_buy_times; }
	const std::vector<uint32_t>&        buy_cost()                    { return t_buy_cost; }

private:
	uint32_t                  t_id;                         ///< 副本分类

	uint32_t                  t_fight_times;                ///< 每日进入次数
	uint32_t                  t_buy_times;                  ///< 购买次数上限
	std::vector<uint32_t>               t_buy_cost;                   ///< 购买花费元宝

public:
	void reset();
	void fill( const DungeonTypeBase& base );
	const char* getClassName() const { return "f副本表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< DungeonType, DungeonTypeBase > DungeonTypeManager;

inline void DungeonType::reset()
{
	this->t_id = 0;
	this->t_fight_times = 0;
	this->t_buy_times = 0;
	this->t_buy_cost.clear();
};

inline void DungeonType::fill( const DungeonTypeBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_fight_times = base.fight_times;
	this->t_buy_times = base.buy_times;
	//if( base.buy_cost == NULL ) this->t_buy_cost.clear(); else this->t_buy_cost.assign( base.buy_cost );
	Dawn::parseContainer<uint32_t>(this->t_buy_cost, base.buy_cost, "$");
};

inline const std::string DungeonType::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "DungeonType.tbls64";
#else
	return "DungeonType.tbls32";
#endif
};
