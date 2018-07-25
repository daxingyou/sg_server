#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:ExpeditionShopBase
/// @remark   记录大小:20 X32
///           记录大小:24 X64
///           字段数量:5
///           数据文件:y远征表.xlsx 神秘商店
///           排序主键:ID
#pragma pack(1)
struct ExpeditionShopBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0511ADD9; ///< I|VIII
#else
	static const int          FormatCheckCode = 0x0513ADD9; ///< I|vIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 ID 生成唯一键

	uint32_t                  id;                           ///< ID

	const char*               item_id_count;                ///< 物品ID
	uint32_t                  rate;                         ///< 刷新几率
	uint32_t                  gold;                         ///< 金币价格
	uint32_t                  cont;                         ///< 帮贡价格
};
#pragma pack()

struct ExpeditionShop : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::pair<uint32_t,uint32_t>&        item_id_count()               { return t_item_id_count; }
	uint32_t                  rate()                        { return t_rate; }
	uint32_t                  gold()                        { return t_gold; }
	uint32_t                  cont()                        { return t_cont; }

private:
	uint32_t                  t_id;                         ///< ID

	std::pair<uint32_t, uint32_t>               t_item_id_count;              ///< 物品ID
	uint32_t                  t_rate;                       ///< 刷新几率
	uint32_t                  t_gold;                       ///< 金币价格
	uint32_t                  t_cont;                       ///< 帮贡价格

public:
	void reset();
	void fill( const ExpeditionShopBase& base );
	const char* getClassName() const { return "y远征表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< ExpeditionShop, ExpeditionShopBase > ExpeditionShopManager;

inline void ExpeditionShop::reset()
{
	this->t_id = 0;
	//this->t_item_id_count.clear();
	this->t_rate = 0;
	this->t_gold = 0;
	this->t_cont = 0;
};

inline void ExpeditionShop::fill( const ExpeditionShopBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	//if( base.item_id_count == NULL ) this->t_item_id_count.clear(); else this->t_item_id_count.assign( base.item_id_count );
	Dawn::parseTuple<uint32_t, uint32_t>(this->t_item_id_count, base.item_id_count, ":");
	this->t_rate = base.rate;
	this->t_gold = base.gold;
	this->t_cont = base.cont;
};

inline const std::string ExpeditionShop::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "ExpeditionShop.tbls64";
#else
	return "ExpeditionShop.tbls32";
#endif
};
