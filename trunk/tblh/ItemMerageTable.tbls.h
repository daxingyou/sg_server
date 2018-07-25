#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include <map>
#include <vector>
#include <string>

/// @brief    表格结构:ItemMerageTableBase
/// @remark   记录大小:12 X32
///           记录大小:20 X64
///           字段数量:3
///           数据文件:h合成表.xlsx 合成表
///           排序主键:武将信物ID
#pragma pack(1)
struct ItemMerageTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x000511B6; ///< I|VV
#else
	static const int          FormatCheckCode = 0x000513D6; ///< I|vv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 武将信物ID 生成唯一键

	uint32_t                  id;                           ///< 武将信物ID

	const char*               item_condition;               ///< 兑换材料
	const char*               money_cost;                   ///< 兑换铜币
};
#pragma pack()

struct ItemMerageTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::string&        item_condition()              { return t_item_condition; }
	const std::string&        money_cost()                  { return t_money_cost; }

	const std::map<uint32_t, uint32_t>& money_const_map()	{ return t_money_const_map;  }
	const std::map<uint32_t, uint32_t>& item_condition_map(){ return t_item_condition_map;  }

private:
	uint32_t                  t_id;                         ///< 武将信物ID

	std::string               t_item_condition;             ///< 兑换材料
	std::string               t_money_cost;                 ///< 兑换铜币

	std::map<uint32_t, uint32_t> t_money_const_map;	
	std::map<uint32_t, uint32_t> t_item_condition_map;
public:
	void reset();
	void fill( const ItemMerageTableBase& base );
	const char* getClassName() const { return "h合成表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< ItemMerageTable, ItemMerageTableBase > ItemMerageTableManager;

inline void ItemMerageTable::reset()
{
	this->t_id = 0;
	this->t_item_condition.clear();
	this->t_money_cost.clear();
};

inline void ItemMerageTable::fill( const ItemMerageTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	if( base.item_condition == NULL ) this->t_item_condition.clear(); else this->t_item_condition.assign( base.item_condition );
	if( base.money_cost == NULL ) this->t_money_cost.clear(); else this->t_money_cost.assign( base.money_cost );

	if (NULL != base.item_condition) {
		std::vector<std::string> conditions;
		std::vector<uint32_t> cond_vec;

		string_util_t::split<std::string>(std::string(base.item_condition), conditions, "$");
		for (uint32_t i = 0; i < conditions.size(); ++i) {
			string_util_t::split<uint32_t>( conditions[i], cond_vec , ":");
			if (cond_vec.size() == 2) {
				t_item_condition_map[cond_vec[0]] = cond_vec[1];
			}
		}
	}

	if (NULL != base.money_cost) {
		std::vector<uint32_t> cond_vec;
		string_util_t::split<uint32_t>( base.money_cost, cond_vec , ":");

		if (cond_vec.size() == 2) {
			t_money_const_map[cond_vec[0]] = cond_vec[1];
		}
	}
};

inline const std::string ItemMerageTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "ItemMerageTable.tbls64";
#else
	return "ItemMerageTable.tbls32";
#endif
};
