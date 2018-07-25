#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:DropBagTableBase
/// @remark   记录大小:24 X32
///           记录大小:28 X64
///           字段数量:6
///           数据文件:d掉落表.xlsx 掉落包
///           排序主键:id
#pragma pack(1)
struct DropBagTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x010DDEF9; ///< I|IIIVI
#else
	static const int          FormatCheckCode = 0x010DE0F9; ///< I|IIIvI
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 id 生成唯一键

	uint32_t                  id;                           ///< id

	uint32_t                  drop_exp;                     ///< 获得经验
	uint32_t                  drop_copper;                  ///< 获得铜币
	uint32_t                  drop_silver;                  ///< 获得银币
	const char*               base_drop_ids;                ///< 基础道具包编号
	uint32_t                  holiday_drop;                 ///< 节日掉落
};
#pragma pack()

struct DropBagTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  drop_exp()                    { return t_drop_exp; }
	uint32_t                  drop_copper()					{ return t_drop_copper; }
	uint32_t                  drop_silver()					{ return t_drop_silver; }
	const std::string&        base_drop_ids()               { return t_base_drop_ids; }
	uint32_t                  holiday_drop() { return t_holiday_drop; }

	uint32_t				  drop_ids_size() { return t_drop_ids_vec.size(); }
	uint32_t				  drop_ids(uint32_t pos) { return pos >= drop_ids_size() ? 0 : t_drop_ids_vec[pos]; }

private:
	uint32_t                  t_id;                         ///< id

	uint32_t                  t_drop_exp;                   ///< 获得经验
	uint32_t                  t_drop_copper;                ///< 获得铜币
	uint32_t                  t_drop_silver;                ///< 获得银币
	std::string               t_base_drop_ids;              ///< 基础道具包编号
	std::vector<uint32_t>	  t_drop_ids_vec;				///< 掉落组	
	uint32_t                  t_holiday_drop;               ///< 节日掉落

public:
	void reset();
	void fill( const DropBagTableBase& base );
	const char* getClassName() const { return "d掉落表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< DropBagTable, DropBagTableBase > DropBagTableManager;

inline void DropBagTable::reset()
{
	this->t_id = 0;
	this->t_drop_exp = 0;
	this->t_drop_copper = 0;
	this->t_drop_silver = 0;
	this->t_base_drop_ids.clear();
	this->t_drop_ids_vec.clear();
	this->t_holiday_drop = 0;
};

inline void DropBagTable::fill( const DropBagTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_drop_exp = base.drop_exp;
	this->t_drop_copper = base.drop_copper;
	this->t_drop_silver = base.drop_silver;
	if( base.base_drop_ids == NULL ) this->t_base_drop_ids.clear(); else this->t_base_drop_ids.assign( base.base_drop_ids );
	if (base.base_drop_ids != NULL)
	{
		string_util_t::split<uint32_t>(std::string(base.base_drop_ids), t_drop_ids_vec, "$");
	}
	this->t_holiday_drop = base.holiday_drop;
};

inline const std::string DropBagTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "DropBagTable.tbls64";
#else
	return "DropBagTable.tbls32";
#endif
};
