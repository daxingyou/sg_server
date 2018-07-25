#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:ShopTableBase
/// @remark   记录大小:44 X32
///           记录大小:48 X64
///           字段数量:11
///           数据文件:s商店表.xlsx Sheet1
///           排序主键:ID
#pragma pack(1)
struct ShopTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x08CD0036; ///< I|IIIIIIIIIV
#else
	static const int          FormatCheckCode = 0x08CD01D6; ///< I|IIIIIIIIIv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 ID 生成唯一键

	uint32_t                  id;                           ///< ID

	uint32_t                  shop_type;                    ///< 商店类型
	uint32_t                  money_type;                   ///< 购买货币类型
	uint32_t                  item_id;                      ///< 物品id
	uint32_t                  presonal_limit;               ///< 个人限购
	uint32_t                  server_limit;                 ///< 服务器限购
	uint32_t                  refresh_type;                 ///< 刷新方式
	uint32_t                  price;                        ///< 价格
	uint32_t                  puton_type;                   ///< 上架方式
	uint32_t                  puton_time;                   ///< 上架时间
	const char*               purchase_condition;           ///< 购买条件
};
#pragma pack()

struct ShopTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  shop_type()                   { return t_shop_type; }
	uint32_t                  money_type()                  { return t_money_type; }
	uint32_t                  item_id()                     { return t_item_id; }
	uint32_t                  presonal_limit()              { return t_presonal_limit; }
	uint32_t                  server_limit()                { return t_server_limit; }
	uint32_t                  refresh_type()                { return t_refresh_type; }
	uint32_t                  price()                       { return t_price; }
	uint32_t                  puton_type()                  { return t_puton_type; }
	uint32_t                  puton_time()                  { return t_puton_time; }
	const std::vector<std::pair<uint32_t, uint32_t>>& purchase_condition() const { return t_purchase_condition; }

private:
	uint32_t                  t_id;                         ///< ID

	uint32_t                  t_shop_type;                  ///< 商店类型
	uint32_t                  t_money_type;                 ///< 购买货币类型
	uint32_t                  t_item_id;                    ///< 物品id
	uint32_t                  t_presonal_limit;             ///< 个人限购
	uint32_t                  t_server_limit;               ///< 服务器限购
	uint32_t                  t_refresh_type;               ///< 刷新方式
	uint32_t                  t_price;                      ///< 价格
	uint32_t                  t_puton_type;                 ///< 上架方式
	uint32_t                  t_puton_time;                 ///< 上架时间
	std::vector<std::pair<uint32_t, uint32_t>> t_purchase_condition;	///< 前置解锁条件

public:
	void reset();
	void fill( const ShopTableBase& base );
	const char* getClassName() const { return "s商店表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< ShopTable, ShopTableBase > ShopTableManager;

inline void ShopTable::reset()
{
	this->t_id = 0;
	this->t_shop_type = 0;
	this->t_money_type = 0;
	this->t_item_id = 0;
	this->t_presonal_limit = 0;
	this->t_server_limit = 0;
	this->t_refresh_type = 0;
	this->t_price = 0;
	this->t_puton_type = 0;
	this->t_puton_time = 0;
	this->t_purchase_condition.clear();
};

inline void ShopTable::fill( const ShopTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_shop_type = base.shop_type;
	this->t_money_type = base.money_type;
	this->t_item_id = base.item_id;
	this->t_presonal_limit = base.presonal_limit;
	this->t_server_limit = base.server_limit;
	this->t_refresh_type = base.refresh_type;
	this->t_price = base.price;
	this->t_puton_type = base.puton_type;
	this->t_puton_time = base.puton_time;
	//if (base.purchase_condition == NULL) this->t_purchase_condition.clear(); else this->t_purchase_condition.assign(base.purchase_condition);
	if (base.purchase_condition != NULL)
	{
		std::vector<std::string> conditions;
		common::string_util_t::split<std::string>(std::string(base.purchase_condition), conditions, "$");
		std::vector<uint32_t> cond_v;

		for (uint32_t i = 0; i < conditions.size(); ++i)
		{
			// cond_v.clear();
			common::string_util_t::split<uint32_t>(conditions[i], cond_v, ":");
			if (cond_v.size() == 2)
			{
				t_purchase_condition.push_back(std::make_pair(cond_v[0], cond_v[1]));
			}
		}
	}
};

inline const std::string ShopTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "ShopTable.tbls64";
#else
	return "ShopTable.tbls32";
#endif
};
