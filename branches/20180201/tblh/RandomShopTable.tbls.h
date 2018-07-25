#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:RandomShopTableBase
/// @remark   记录大小:32 X32
///           记录大小:36 X64
///           字段数量:8
///           数据文件:s商店表.xlsx Sheet2
///           排序主键:ID
#pragma pack(1)
struct RandomShopTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0FFD8CE6; ///< I|IiiIIIV
#else
	static const int          FormatCheckCode = 0x0FFD8D06; ///< I|IiiIIIv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 ID 生成唯一键

	uint32_t                  id;                           ///< ID

	uint32_t                  shop_type;                    ///< 商店类型
	int32_t                   pay_refresh;                  ///< 付费刷新次数
	int32_t                   free_refresh;                 ///< 免费刷新次数
	uint32_t                  refresh_time;                 ///< 自动刷新时间
	uint32_t                  cost_item;                    ///< 消耗道具
	uint32_t                  cost_value;                   ///< 道具数量
	const char*               goods_pool;                   ///< 商品随机池
};
#pragma pack()

struct RandomShopTable : public Entry
{
	typedef std::vector<uint32_t> shop_pool_vec;
	typedef std::vector<shop_pool_vec> grid_vec;

	uint32_t                  id()                          { return t_id; }

	uint32_t                  shop_type()					{ return t_shop_type; }
	int32_t                   pay_refresh()                 { return t_pay_refresh; }
	int32_t                   free_refresh()                { return t_free_refresh; }
	uint32_t                  refresh_time()                { return t_refresh_time; }
	uint32_t                  cost_item()                   { return t_cost_item; }
	uint32_t                  cost_value()                  { return t_cost_value; }
	const grid_vec&			  get_all_grids()				{ return t_grid_vec; }

private:
	uint32_t                  t_id;                         ///< ID

	uint32_t                  t_shop_type;                  ///< 商店类型
	int32_t                   t_pay_refresh;                ///< 付费刷新次数
	int32_t                   t_free_refresh;               ///< 免费刷新次数
	uint32_t                  t_refresh_time;               ///< 自动刷新时间
	uint32_t                  t_cost_item;                  ///< 消耗道具
	uint32_t                  t_cost_value;                 ///< 道具数量
	grid_vec				  t_grid_vec;					///< 商品随机池

public:
	void reset();
	void fill( const RandomShopTableBase& base );
	const char* getClassName() const { return "s商店表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< RandomShopTable, RandomShopTableBase > RandomShopTableManager;

inline void RandomShopTable::reset()
{
	this->t_id = 0;
	this->t_shop_type = 0;
	this->t_pay_refresh = 0;
	this->t_free_refresh = 0;
	this->t_refresh_time = 0;
	this->t_cost_item = 0;
	this->t_cost_value = 0;
	this->t_grid_vec.clear();
};

inline void RandomShopTable::fill( const RandomShopTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_shop_type = base.shop_type;
	this->t_pay_refresh = base.pay_refresh;
	this->t_free_refresh = base.free_refresh;
	this->t_refresh_time = base.refresh_time;
	this->t_cost_item = base.cost_item;
	this->t_cost_value = base.cost_value;
	if ( base.goods_pool != NULL )
	{
		std::vector<std::string> grids;
		common::string_util_t::split<std::string>(std::string(base.goods_pool), grids, ";");
		std::vector<uint32_t> shop_pool;

		for (const auto& grid : grids)
		{
			common::string_util_t::split<uint32_t>(grid, shop_pool, "$");
			t_grid_vec.push_back(shop_pool);
		}
	}
};

inline const std::string RandomShopTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "RandomShopTable.tbls64";
#else
	return "RandomShopTable.tbls32";
#endif
};
