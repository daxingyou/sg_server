#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:BAExchangeItemBase
/// @remark   记录大小:16 X32
///           记录大小:24 X64
///           字段数量:4
///           数据文件:h合成表.xlsx 兑换
///           排序主键:ID
#pragma pack(1)
struct BAExchangeItemBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00511BA9; ///< I|VVI
#else
	static const int          FormatCheckCode = 0x00513DA9; ///< I|vvI
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 ID 生成唯一键

	uint32_t                  id;                           ///< ID

	const char*               need_item;                    ///< 需要道具
	const char*               make_item;                    ///< 合成道具
	uint32_t                  max_num;                      ///< 最大次数
};
#pragma pack()

struct BAExchangeItem : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::string&        need_item()                   { return t_need_item; }
	const std::string&        make_item()                   { return t_make_item; }
	uint32_t                  max_num()                     { return t_max_num; }
	std::map<uint32_t, uint32_t> &get_need_item() { return t_need_item_map; }
	std::map<uint32_t, uint32_t> &get_make_item() { return t_make_item_map; }
private:
	uint32_t                  t_id;                         ///< ID

	std::string               t_need_item;                  ///< 需要道具
	std::string               t_make_item;                  ///< 合成道具
	uint32_t                  t_max_num;                    ///< 最大次数

public:
	void reset();
	void fill( const BAExchangeItemBase& base );
	const char* getClassName() const { return "h合成表"; }

	std::map<uint32_t, uint32_t> t_need_item_map;
	std::map<uint32_t, uint32_t> t_make_item_map;
	static const std::string getBinaryName();
};

typedef DataTableManager< BAExchangeItem, BAExchangeItemBase > BAExchangeItemManager;

inline void BAExchangeItem::reset()
{
	this->t_id = 0;
	this->t_need_item.clear();
	this->t_make_item.clear();
	this->t_max_num = 0;
	this->t_need_item_map.clear();
	this->t_make_item_map.clear();
};

inline void BAExchangeItem::fill( const BAExchangeItemBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	if( base.need_item == NULL ) this->t_need_item.clear(); else this->t_need_item.assign( base.need_item );
	if( base.make_item == NULL ) this->t_make_item.clear(); else this->t_make_item.assign( base.make_item );
	this->t_max_num = base.max_num;

	if (base.need_item!= NULL)
		Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_need_item_map, base.need_item, ":$");
	
	if( base.make_item != NULL )
		Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_make_item_map, base.make_item, ":$");

};

inline const std::string BAExchangeItem::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "BAExchangeItem.tbls64";
#else
	return "BAExchangeItem.tbls32";
#endif
};
