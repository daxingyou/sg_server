#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:LimitBuyTableBase
/// @remark   记录大小:36 X32
///           记录大小:40 X64
///           字段数量:9
///           数据文件:y运营活动表.xlsx 每日抢购
///           排序主键:时间
#pragma pack(1)
struct LimitBuyTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0DEFCD09; ///< I|IIIVIIII
#else
	static const int          FormatCheckCode = 0x0E0FCD09; ///< I|IIIvIIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 时间 生成唯一键

	uint32_t                  id;                           ///< 时间

	uint32_t                  drop_id;                      ///< 掉落ID
	uint32_t                  show_item_id;                 ///< 展示ID
	uint32_t                  show_item_num;                ///< 展示数量
	const char*               show_prize;                   ///< 商品描述
	uint32_t                  money_type;                   ///< 货币类型
	uint32_t                  money;                        ///< 货币数量
	uint32_t                  max_num;                      ///< 抢购次数
	uint32_t                  show_moeny;                   ///< 显示原价
};
#pragma pack()

struct LimitBuyTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  drop_id()                     { return t_drop_id; }
	uint32_t                  show_item_id()                { return t_show_item_id; }
	uint32_t                  show_item_num()               { return t_show_item_num; }
	const std::string&        show_prize()                  { return t_show_prize; }
	uint32_t                  money_type()                  { return t_money_type; }
	uint32_t                  money()                       { return t_money; }
	uint32_t                  max_num()                     { return t_max_num; }
	uint32_t                  show_moeny()                  { return t_show_moeny; }

private:
	uint32_t                  t_id;                         ///< 时间

	uint32_t                  t_drop_id;                    ///< 掉落ID
	uint32_t                  t_show_item_id;               ///< 展示ID
	uint32_t                  t_show_item_num;              ///< 展示数量
	std::string               t_show_prize;                 ///< 商品描述
	uint32_t                  t_money_type;                 ///< 货币类型
	uint32_t                  t_money;                      ///< 货币数量
	uint32_t                  t_max_num;                    ///< 抢购次数
	uint32_t                  t_show_moeny;                 ///< 显示原价

public:
	void reset();
	void fill( const LimitBuyTableBase& base );
	const char* getClassName() const { return "y运营活动表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< LimitBuyTable, LimitBuyTableBase > LimitBuyTableManager;

inline void LimitBuyTable::reset()
{
	this->t_id = 0;
	this->t_drop_id = 0;
	this->t_show_item_id = 0;
	this->t_show_item_num = 0;
	this->t_show_prize.clear();
	this->t_money_type = 0;
	this->t_money = 0;
	this->t_max_num = 0;
	this->t_show_moeny = 0;
};

inline void LimitBuyTable::fill( const LimitBuyTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_drop_id = base.drop_id;
	this->t_show_item_id = base.show_item_id;
	this->t_show_item_num = base.show_item_num;
	if( base.show_prize == NULL ) this->t_show_prize.clear(); else this->t_show_prize.assign( base.show_prize );
	this->t_money_type = base.money_type;
	this->t_money = base.money;
	this->t_max_num = base.max_num;
	this->t_show_moeny = base.show_moeny;
};

inline const std::string LimitBuyTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "LimitBuyTable.tbls64";
#else
	return "LimitBuyTable.tbls32";
#endif
};
