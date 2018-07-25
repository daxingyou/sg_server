#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:TradeMarketBase
/// @remark   记录大小:16 X32
///           记录大小:16 X64
///           字段数量:4
///           数据文件:t跑商表.xlsx trade_market
///           排序主键:货物类型id
#pragma pack(1)
struct TradeMarketBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#else
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 货物类型id 生成唯一键

	uint32_t                  id;                           ///< 货物类型id

	uint32_t                  market_float;                 ///< 行情浮动(万分比)
	uint32_t                  market_lower;                 ///< 行情下限(万分比)
	uint32_t                  market_upper;                 ///< 行情上限(万分比)
};
#pragma pack()

struct TradeMarket : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  market_float()                { return t_market_float; }
	uint32_t                  market_lower()                { return t_market_lower; }
	uint32_t                  market_upper()                { return t_market_upper; }

private:
	uint32_t                  t_id;                         ///< 货物类型id

	uint32_t                  t_market_float;               ///< 行情浮动(万分比)
	uint32_t                  t_market_lower;               ///< 行情下限(万分比)
	uint32_t                  t_market_upper;               ///< 行情上限(万分比)

public:
	void reset();
	void fill( const TradeMarketBase& base );
	const char* getClassName() const { return "t跑商表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< TradeMarket, TradeMarketBase > TradeMarketManager;

inline void TradeMarket::reset()
{
	this->t_id = 0;
	this->t_market_float = 0;
	this->t_market_lower = 0;
	this->t_market_upper = 0;
};

inline void TradeMarket::fill( const TradeMarketBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_market_float = base.market_float;
	this->t_market_lower = base.market_lower;
	this->t_market_upper = base.market_upper;
};

inline const std::string TradeMarket::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "TradeMarket.tbls64";
#else
	return "TradeMarket.tbls32";
#endif
};
