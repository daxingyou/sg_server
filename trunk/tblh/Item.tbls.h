#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/MyTuple.h"
#include "common/ContainerParse.h"

/// @brief    表格结构:ItemBase
/// @remark   记录大小:88 X32
///           记录大小:108 X64
///           字段数量:22
///           数据文件:i物品表.xlsx ItemTable
///           排序主键:道具ID
#pragma pack(1)
struct ItemBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x08CF3509; ///< I|VIIIIIIVVIIIVIVIIIIIII
#else
	static const int          FormatCheckCode = 0x08F2CD09; ///< I|vIIIIIIvvIIIvIvIIIIIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 道具ID 生成唯一键

	uint32_t                  id;                           ///< 道具ID

	const char*               name_desc;                    ///< 道具名
	uint32_t                  search_id;                    ///< 查询ID
	uint32_t                  quality;                      ///< 道具品质
	uint32_t                  big_type;                     ///< 分类类型
	uint32_t                  type;                         ///< 道具类型
	uint32_t                  overlap_count;                ///< 堆叠数
	uint32_t                  use_type;                     ///< 使用类型
	const char*               param;                        ///< 效果参数
	const char*               use_pos;                      ///< 使用地点
	uint32_t                  transaction;                  ///< 交易初始价格
	uint32_t                  sell_price;                   ///< 出售价格
	uint32_t                  trade_type;                   ///< 跑商货物类型
	const char*               trade_price;                  ///< 跑商价格
	uint32_t                  use_cond_type;                ///< 使用权限
	const char*               use_cond_param;               ///< 权限参数
	uint32_t                  smelt;                        ///< 分解
	uint32_t                  compose;                      ///< 合成
	uint32_t                  bind_type;                    ///< 绑定类型
	uint32_t                  rarity;                       ///< 珍品
	uint32_t                  can_sell;                     ///< 交易
	uint32_t                  resolve_soul;                 ///< 分解将魂
	uint32_t                  package_choose_num;           ///< 礼包选择数量
};
#pragma pack()

struct Item : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::string&        name_desc()                   { return t_name_desc; }
	uint32_t                  search_id() { return t_search_id; }
	uint32_t                  quality()                     { return t_quality; }
	uint32_t                  big_type()                    { return t_big_type; }
	uint32_t                  type()                        { return t_type; }
	uint32_t                  overlap_count()               { return t_overlap_count; }
	uint32_t                  use_type()                    { return t_use_type; }
	const std::string&        param()                       { return t_param; }
	const Dawn::triple<uint32_t,uint32_t,uint32_t>& use_pos(){ return t_use_pos; }
	uint32_t                  transaction()                 { return t_transaction; }
	uint32_t                  sell_price()                  { return t_sell_price; }
	uint32_t                  trade_type()                  { return t_trade_type; }
	const std::map<uint32_t, uint32_t>&    trade_price()	{ return t_trade_price; }
	uint32_t                  smelt()						{ return t_smelt; }
	uint32_t                  compose()						{ return t_compose; }
	uint32_t                  bind_type()					{ return t_bind_type; }
	uint32_t                  rarity()						{ return t_rarity; }
	uint32_t                  can_sell() { return t_can_sell; }
	uint32_t				  param_size() { return (uint32_t)t_param_vec.size(); }
	uint32_t				  param(uint32_t pos) { return pos >= param_size() ? 0 : t_param_vec[pos]; }
	uint32_t                  use_cond_type() { return t_use_cond_type; }
	const std::string&        use_cond_param() { return t_use_cond_param; }
	uint32_t				  use_cond_param_size() { return (uint32_t)t_use_cond_param_vec.size(); }
	uint32_t				  use_cond_param(uint32_t pos) { return pos >= use_cond_param_size() ? 0 : t_use_cond_param_vec[pos]; }

	uint32_t				  get_trade_price(uint32_t id)
	{
		auto iter = t_trade_price.find(id);
		if (iter != t_trade_price.end())
		{
			return iter->second;
		}
		return 0;
	}
	uint32_t                  resolve_soul()                { return t_resolve_soul; }
	uint32_t                  package_choose_num()          { return t_package_choose_num; }
private:
	uint32_t                  t_id;                         ///< 道具ID

	std::string               t_name_desc;                  ///< 道具名
	uint32_t                  t_search_id;                  ///< 查询ID
	uint32_t                  t_quality;                    ///< 道具品质
	uint32_t                  t_big_type;                   ///< 分类类型
	uint32_t                  t_type;                       ///< 道具类型
	uint32_t                  t_overlap_count;              ///< 堆叠数
	uint32_t                  t_use_type;                   ///< 使用类型
	Dawn::triple<uint32_t, uint32_t, uint32_t> t_use_pos;  ///< 使用地点
	std::string               t_param;                      ///< 效果参数
	uint32_t                  t_transaction;                ///< 交易初始价格
	uint32_t                  t_sell_price;                 ///< 出售价格
	uint32_t                  t_trade_type;                 ///< 跑商货物类型
	std::map<uint32_t, uint32_t>   t_trade_price;			///< 跑商价格
	uint32_t                  t_use_cond_type;              ///< 使用权限
	std::string               t_use_cond_param;             ///< 权限参数
	uint32_t                  t_smelt;                      ///< 分解
	uint32_t                  t_compose;                    ///< 合成
	uint32_t                  t_bind_type;                  ///< 绑定类型
	uint32_t                  t_rarity;                     ///< 珍品
	uint32_t                  t_can_sell;                   ///< 交易
	uint32_t                  t_resolve_soul;               ///< 分解将魂
	std::vector<uint32_t>		t_param_vec;
	std::vector<uint32_t>		t_use_cond_param_vec;
	uint32_t                  t_package_choose_num;         ///< 礼包选择数量
public:
	void reset();
	void fill( const ItemBase& base );
	const char* getClassName() const { return "i物品表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Item, ItemBase > ItemManager;

inline void Item::reset()
{
	this->t_id = 0;
	this->t_name_desc.clear();
	this->t_search_id = 0;
	this->t_quality = 0;
	this->t_big_type = 0;
	this->t_type = 0;
	this->t_overlap_count = 0;
	this->t_use_type = 0;
	this->t_param.clear();
	this->t_use_pos.clear();
	this->t_transaction = 0;
	this->t_sell_price = 0;
	this->t_trade_type = 0;	
	this->t_use_cond_type = 0;
	this->t_trade_price.clear();
	this->t_use_cond_param.clear();
	this->t_smelt = 0;
	this->t_compose = 0;
	this->t_bind_type = 0;
	this->t_rarity = 0;
	this->t_can_sell = 0;
	this->t_param_vec.clear();
	this->t_use_cond_param_vec.clear();
	this->t_resolve_soul = 0;
	this->t_package_choose_num = 0;
};

inline void Item::fill( const ItemBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	if( base.name_desc == NULL ) this->t_name_desc.clear(); else this->t_name_desc.assign( base.name_desc );
	this->t_search_id = base.search_id;
	this->t_quality = base.quality;
	this->t_big_type = base.big_type;
	this->t_type = base.type;
	this->t_overlap_count = base.overlap_count;
	this->t_use_type = base.use_type;
	if( base.param == NULL ) this->t_param.clear(); else this->t_param.assign( base.param );
	//if (base.use_pos == NULL) this->t_use_pos.clear(); else this->t_use_pos.assign(base.use_pos);
	Dawn::parseTuple<uint32_t, uint32_t, uint32_t>(this->t_use_pos, base.use_pos, "$");
	this->t_transaction = base.transaction;
	this->t_sell_price = base.sell_price;
	this->t_trade_type = base.trade_type;
	//if (base.trade_price == NULL) this->t_trade_price.clear(); else this->t_trade_price.assign(base.trade_price);
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_trade_price, base.trade_price, ":$");
	this->t_use_cond_type = base.use_cond_type;
	if (base.use_cond_param == NULL) this->t_use_cond_param.clear(); else this->t_use_cond_param.assign(base.use_cond_param);
	this->t_smelt = base.smelt;
	this->t_compose = base.compose;
	this->t_bind_type = base.bind_type;
	this->t_rarity = base.rarity;
	this->t_can_sell = base.can_sell;
	if (base.param != NULL)
		string_util_t::split<uint32_t>(std::string(base.param), this->t_param_vec, "$");
	if (base.use_cond_param != NULL)
		string_util_t::split<uint32_t>(std::string(base.use_cond_param), this->t_use_cond_param_vec, ":");

	this->t_resolve_soul = base.resolve_soul;
	this->t_package_choose_num = base.package_choose_num;
};

inline const std::string Item::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Item.tbls64";
#else
	return "Item.tbls32";
#endif
};
