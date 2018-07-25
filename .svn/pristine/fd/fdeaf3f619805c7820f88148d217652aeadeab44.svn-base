#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    表格结构:GoodsBase
/// @remark   记录大小:20 X32
///           记录大小:24 X64
///           字段数量:5
///           数据文件:j交易行.xlsx Sheet1
///           排序主键:ID
#pragma pack(1)
struct GoodsBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x011ADD89; ///< I|VIIII
#else
	static const int          FormatCheckCode = 0x013ADD89; ///< I|vIIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 ID 生成唯一键

	uint32_t                  id;                           ///< ID

	const char*               type;                         ///< 页签类型
	uint32_t                  item_tid;                     ///< 物品ID
	uint32_t                  price;                        ///< 默认价格
	uint32_t                  rarity_price;                 ///< 珍品价格
	uint32_t                  search_id;                    ///< 查询ID
};
#pragma pack()

struct Goods : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::vector<uint32_t>&        type()                        { return t_type; }
	uint32_t                  item_tid()                    { return t_item_tid; }
	uint32_t                  price()                       { return t_price; }
	uint32_t                  rarity_price()                { return t_rarity_price; }
	uint32_t                  search_id()                   { return t_search_id; }

private:
	uint32_t                  t_id;                         ///< ID

	std::vector<uint32_t>               t_type;                       ///< 页签类型
	uint32_t                  t_item_tid;                   ///< 物品ID
	uint32_t                  t_price;                      ///< 默认价格
	uint32_t                  t_rarity_price;               ///< 珍品价格
	uint32_t                  t_search_id;                  ///< 查询ID

public:
	void reset();
	void fill( const GoodsBase& base );
	const char* getClassName() const { return "j交易行"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Goods, GoodsBase > GoodsManager;

inline void Goods::reset()
{
	this->t_id = 0;
	this->t_type.clear();
	this->t_item_tid = 0;
	this->t_price = 0;
	this->t_rarity_price = 0;
	this->t_search_id = 0;
};

inline void Goods::fill( const GoodsBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	//if( base.type == NULL ) this->t_type.clear(); else this->t_type.assign( base.type );
	Dawn::parseContainer<uint32_t>(this->t_type, base.type, "$");
	this->t_item_tid = base.item_tid;
	this->t_price = base.price;
	this->t_rarity_price = base.rarity_price;
	this->t_search_id = base.search_id;
};

inline const std::string Goods::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Goods.tbls64";
#else
	return "Goods.tbls32";
#endif
};
