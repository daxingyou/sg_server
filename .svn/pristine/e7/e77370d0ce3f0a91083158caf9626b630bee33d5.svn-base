#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:WsTreasuryTableBase
/// @remark   记录大小:52 X32
///           记录大小:56 X64
///           字段数量:13
///           数据文件:y运营活动表.xlsx 武圣宝库
///           排序主键:序号
#pragma pack(1)
struct WsTreasuryTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0F222706; ///< I|IIIiiiiiiIIV
#else
	static const int          FormatCheckCode = 0x0F2228E6; ///< I|IIIiiiiiiIIv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 序号 生成唯一键

	uint32_t                  id;                           ///< 序号

	uint32_t                  drop_id;                      ///< 奖励ID
	uint32_t                  show_item_id;                 ///< 展示奖励
	uint32_t                  show_item_num;                ///< 展示数量
	int32_t                   init_percent;                 ///< 初始权重
	int32_t                   dec_percent;                  ///< 抽到自身减少权重
	int32_t                   lucky_add_percent_val;        ///< 幸运值附加权重系数
	int32_t                   add_lucky;                    ///< 抽到增加幸运值
	int32_t                   dec_lucky;                    ///< 抽到减少幸运值
	int32_t                   add_score;                    ///< 抽到获得积分
	uint32_t                  notice;                       ///< 全服公告
	uint32_t                  notice_id;                    ///< 公告ID
	const char*               item_name;                    ///< 名称备注
};
#pragma pack()

struct WsTreasuryTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  drop_id()                     { return t_drop_id; }
	uint32_t                  show_item_id()                { return t_show_item_id; }
	uint32_t                  show_item_num()               { return t_show_item_num; }
	int32_t                   init_percent()                { return t_init_percent; }
	int32_t                   dec_percent()                 { return t_dec_percent; }
	int32_t                   lucky_add_percent_val()       { return t_lucky_add_percent_val; }
	int32_t                   add_lucky()                   { return t_add_lucky; }
	int32_t                   dec_lucky()                   { return t_dec_lucky; }
	int32_t                   add_score()                   { return t_add_score; }
	uint32_t                  notice()                      { return t_notice; }
	uint32_t                  notice_id()                   { return t_notice_id; }
	const std::string&        item_name()                   { return t_item_name; }

private:
	uint32_t                  t_id;                         ///< 序号

	uint32_t                  t_drop_id;                    ///< 奖励ID
	uint32_t                  t_show_item_id;               ///< 展示奖励
	uint32_t                  t_show_item_num;              ///< 展示数量
	int32_t                   t_init_percent;               ///< 初始权重
	int32_t                   t_dec_percent;                ///< 抽到自身减少权重
	int32_t                   t_lucky_add_percent_val;      ///< 幸运值附加权重系数
	int32_t                   t_add_lucky;                  ///< 抽到增加幸运值
	int32_t                   t_dec_lucky;                  ///< 抽到减少幸运值
	int32_t                   t_add_score;                  ///< 抽到获得积分
	uint32_t                  t_notice;                     ///< 全服公告
	uint32_t                  t_notice_id;                  ///< 公告ID
	std::string               t_item_name;                  ///< 名称备注

public:
	void reset();
	void fill( const WsTreasuryTableBase& base );
	const char* getClassName() const { return "y运营活动表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< WsTreasuryTable, WsTreasuryTableBase > WsTreasuryTableManager;

inline void WsTreasuryTable::reset()
{
	this->t_id = 0;
	this->t_drop_id = 0;
	this->t_show_item_id = 0;
	this->t_show_item_num = 0;
	this->t_init_percent = 0;
	this->t_dec_percent = 0;
	this->t_lucky_add_percent_val = 0;
	this->t_add_lucky = 0;
	this->t_dec_lucky = 0;
	this->t_add_score = 0;
	this->t_notice = 0;
	this->t_notice_id = 0;
	this->t_item_name.clear();
};

inline void WsTreasuryTable::fill( const WsTreasuryTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_drop_id = base.drop_id;
	this->t_show_item_id = base.show_item_id;
	this->t_show_item_num = base.show_item_num;
	this->t_init_percent = base.init_percent;
	this->t_dec_percent = base.dec_percent;
	this->t_lucky_add_percent_val = base.lucky_add_percent_val;
	this->t_add_lucky = base.add_lucky;
	this->t_dec_lucky = base.dec_lucky;
	this->t_add_score = base.add_score;
	this->t_notice = base.notice;
	this->t_notice_id = base.notice_id;
	if( base.item_name == NULL ) this->t_item_name.clear(); else this->t_item_name.assign( base.item_name );
};

inline const std::string WsTreasuryTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "WsTreasuryTable.tbls64";
#else
	return "WsTreasuryTable.tbls32";
#endif
};
