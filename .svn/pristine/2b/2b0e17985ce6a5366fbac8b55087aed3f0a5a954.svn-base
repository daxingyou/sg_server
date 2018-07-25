#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:WsScoreTreasuryTableBase
/// @remark   记录大小:16 X32
///           记录大小:16 X64
///           字段数量:4
///           数据文件:y运营活动表.xlsx 武圣宝库兑换
///           排序主键:宝箱序号
#pragma pack(1)
struct WsScoreTreasuryTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#else
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 宝箱序号 生成唯一键

	uint32_t                  id;                           ///< 宝箱序号

	uint32_t                  cost_score;                   ///< 消耗积分
	uint32_t                  drop_id;                      ///< 奖励ID
	uint32_t                  show_item_id;                 ///< 展示奖励
};
#pragma pack()

struct WsScoreTreasuryTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  cost_score()                  { return t_cost_score; }
	uint32_t                  drop_id()                     { return t_drop_id; }
	uint32_t                  show_item_id()                { return t_show_item_id; }

private:
	uint32_t                  t_id;                         ///< 宝箱序号

	uint32_t                  t_cost_score;                 ///< 消耗积分
	uint32_t                  t_drop_id;                    ///< 奖励ID
	uint32_t                  t_show_item_id;               ///< 展示奖励

public:
	void reset();
	void fill( const WsScoreTreasuryTableBase& base );
	const char* getClassName() const { return "y运营活动表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< WsScoreTreasuryTable, WsScoreTreasuryTableBase > WsScoreTreasuryTableManager;

inline void WsScoreTreasuryTable::reset()
{
	this->t_id = 0;
	this->t_cost_score = 0;
	this->t_drop_id = 0;
	this->t_show_item_id = 0;
};

inline void WsScoreTreasuryTable::fill( const WsScoreTreasuryTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_cost_score = base.cost_score;
	this->t_drop_id = base.drop_id;
	this->t_show_item_id = base.show_item_id;
};

inline const std::string WsScoreTreasuryTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "WsScoreTreasuryTable.tbls64";
#else
	return "WsScoreTreasuryTable.tbls32";
#endif
};
