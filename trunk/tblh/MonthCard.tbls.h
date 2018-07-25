#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:MonthCardBase
/// @remark   记录大小:32 X32
///           记录大小:44 X64
///           字段数量:8
///           数据文件:y运营活动表.xlsx 月卡
///           排序主键:月卡类型
#pragma pack(1)
struct MonthCardBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0DDEEAA9; ///< I|IIIVVVI
#else
	static const int          FormatCheckCode = 0x0DE08CA9; ///< I|IIIvvvI
#endif

	//------------------------
	uint32_t getUniqueID() const { return type; }; // TODO: 请用 月卡类型 生成唯一键

	uint32_t                  type;                         ///< 月卡类型

	uint32_t                  cz_id;                        ///< 充值ID
	uint32_t                  add_day;                      ///< 持续天数
	uint32_t                  ed_prize;                     ///< 每日奖励ID
	const char*               show_mingshi;                 ///< 展示命石
	const char*               show_yuanbao;                 ///< 展示元宝
	const char*               show_gift;                    ///< 展示礼包
	uint32_t                  add_limit;                    ///< 续费少于天数
};
#pragma pack()

struct MonthCard : public Entry
{
	uint32_t                  type()                        { return t_type; }

	uint32_t                  cz_id()                       { return t_cz_id; }
	uint32_t                  add_day()                     { return (t_add_day * 86400); }
	uint32_t                  ed_prize()                    { return t_ed_prize; }
	const std::string&        show_mingshi()                { return t_show_mingshi; }
	const std::string&        show_yuanbao()                { return t_show_yuanbao; }
	const std::string&        show_gift()                   { return t_show_gift; }
	uint32_t                  add_limit()                   { return t_add_limit * 86400; }

private:
	uint32_t                  t_type;                       ///< 月卡类型

	uint32_t                  t_cz_id;                      ///< 充值ID
	uint32_t                  t_add_day;                    ///< 持续天数
	uint32_t                  t_ed_prize;                   ///< 每日奖励ID
	std::string               t_show_mingshi;               ///< 展示命石
	std::string               t_show_yuanbao;               ///< 展示元宝
	std::string               t_show_gift;                  ///< 展示礼包
	uint32_t                  t_add_limit;                  ///< 续费少于天数

public:
	void reset();
	void fill( const MonthCardBase& base );
	const char* getClassName() const { return "y运营活动表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< MonthCard, MonthCardBase > MonthCardManager;

inline void MonthCard::reset()
{
	this->t_type = 0;
	this->t_cz_id = 0;
	this->t_add_day = 0;
	this->t_ed_prize = 0;
	this->t_show_mingshi.clear();
	this->t_show_yuanbao.clear();
	this->t_show_gift.clear();
	this->t_add_limit = 0;
};

inline void MonthCard::fill( const MonthCardBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_type = base.type;
	this->t_cz_id = base.cz_id;
	this->t_add_day = base.add_day;
	this->t_ed_prize = base.ed_prize;
	if( base.show_mingshi == NULL ) this->t_show_mingshi.clear(); else this->t_show_mingshi.assign( base.show_mingshi );
	if( base.show_yuanbao == NULL ) this->t_show_yuanbao.clear(); else this->t_show_yuanbao.assign( base.show_yuanbao );
	if( base.show_gift == NULL ) this->t_show_gift.clear(); else this->t_show_gift.assign( base.show_gift );
	this->t_add_limit = base.add_limit;
};

inline const std::string MonthCard::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "MonthCard.tbls64";
#else
	return "MonthCard.tbls32";
#endif
};
