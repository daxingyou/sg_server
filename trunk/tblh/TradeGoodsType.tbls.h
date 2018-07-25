#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:TradeGoodsTypeBase
/// @remark   记录大小:24 X32
///           记录大小:28 X64
///           字段数量:6
///           数据文件:t跑商表.xlsx trade_goods_type
///           排序主键:货物类型id
#pragma pack(1)
struct TradeGoodsTypeBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x011ADD89; ///< I|VIIII
#else
	static const int          FormatCheckCode = 0x013ADD89; ///< I|vIIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 货物类型id 生成唯一键

	uint32_t                  id;                           ///< 货物类型id
	const char*               goods_id;                     ///< 包含的货物id
	uint32_t                  market_base_value;            ///< 行情基准值(万分比)
	uint32_t                  market_float;                 ///< 行情浮动(万分比)
	uint32_t                  market_lower;                 ///< 行情下限(万分比)
	uint32_t                  market_upper;                 ///< 行情上限(万分比)
};
#pragma pack()

struct TradeGoodsType : public Entry
{
	uint32_t                  id() { return t_id; }
	//	const std::string&        goods_id()                    { return t_goods_id; }
	uint32_t				  goods_id_size() { return (uint32_t)t_goods_id_vec.size(); }
	uint32_t				  goods_id(uint32_t pos) { return pos >= goods_id_size() ? 0 : t_goods_id_vec[pos]; }
	uint32_t                  market_base_value()           { return t_market_base_value; }
	uint32_t                  market_float()                { return t_market_float; }
	uint32_t                  market_lower()                { return t_market_lower; }
	uint32_t                  market_upper()                { return t_market_upper; }

private:
	uint32_t                  t_id;                         ///< 货物类型id
	std::string               t_goods_id;                   ///< 包含的货物id
	std::vector<uint32_t>	  t_goods_id_vec;				
	uint32_t                  t_market_base_value;          ///< 行情基准值(万分比)
	uint32_t                  t_market_float;               ///< 行情浮动(万分比)
	uint32_t                  t_market_lower;               ///< 行情下限(万分比)
	uint32_t                  t_market_upper;               ///< 行情上限(万分比)
public:
	void reset();
	void fill( const TradeGoodsTypeBase& base );
	const char* getClassName() const { return "t跑商表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< TradeGoodsType, TradeGoodsTypeBase > TradeGoodsTypeManager;

inline void TradeGoodsType::reset()
{
	this->t_id = 0;
	this->t_goods_id.clear();
	this->t_market_base_value = 0;
	this->t_market_float = 0;
	this->t_market_lower = 0;
	this->t_market_upper = 0;
};

inline void TradeGoodsType::fill( const TradeGoodsTypeBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	if( base.goods_id == NULL ) this->t_goods_id.clear(); else this->t_goods_id.assign( base.goods_id );
	this->t_market_base_value = base.market_base_value;
	this->t_market_float = base.market_float;
	this->t_market_lower = base.market_lower;
	this->t_market_upper = base.market_upper;
	if (base.goods_id != NULL)
	{
		common::string_util_t::split<uint32_t>(std::string(base.goods_id), t_goods_id_vec, "$");
	}
};

inline const std::string TradeGoodsType::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "TradeGoodsType.tbls64";
#else
	return "TradeGoodsType.tbls32";
#endif
};
