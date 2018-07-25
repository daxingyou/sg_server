#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:TradeFloatBase
/// @remark   记录大小:16 X32
///           记录大小:16 X64
///           字段数量:4
///           数据文件:t跑商表.xlsx trade_float
///           排序主键:id
#pragma pack(1)
struct TradeFloatBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#else
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 id 生成唯一键

	uint32_t                  id;                           ///< id

	uint32_t                  sell_cnt;                     ///< 出售量
	uint32_t                  down_prob;                    ///< 向下波动概率(万分比)
	uint32_t                  stay_prob;                    ///< 波动不变概率(万分比)
};
#pragma pack()

struct TradeFloat : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  sell_cnt()                    { return t_sell_cnt; }
	uint32_t                  down_prob()                   { return t_down_prob; }
	uint32_t                  stay_prob()                   { return t_stay_prob; }

private:
	uint32_t                  t_id;                         ///< id

	uint32_t                  t_sell_cnt;                   ///< 出售量
	uint32_t                  t_down_prob;                  ///< 向下波动概率(万分比)
	uint32_t                  t_stay_prob;                  ///< 波动不变概率(万分比)

public:
	void reset();
	void fill( const TradeFloatBase& base );
	const char* getClassName() const { return "t跑商表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< TradeFloat, TradeFloatBase > TradeFloatManager;

inline void TradeFloat::reset()
{
	this->t_id = 0;
	this->t_sell_cnt = 0;
	this->t_down_prob = 0;
	this->t_stay_prob = 0;
};

inline void TradeFloat::fill( const TradeFloatBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_sell_cnt = base.sell_cnt;
	this->t_down_prob = base.down_prob;
	this->t_stay_prob = base.stay_prob;
};

inline const std::string TradeFloat::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "TradeFloat.tbls64";
#else
	return "TradeFloat.tbls32";
#endif
};
