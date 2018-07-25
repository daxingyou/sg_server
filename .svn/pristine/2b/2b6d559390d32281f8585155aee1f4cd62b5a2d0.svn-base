#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:GoldLuckydrawBase
/// @remark   记录大小:20 X32
///           记录大小:28 X64
///           字段数量:5
///           数据文件:y运营活动表.xlsx 黄金十连
///           排序主键:领取时间
#pragma pack(1)
struct GoldLuckydrawBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0510EBA9; ///< I|IVVI
#else
	static const int          FormatCheckCode = 0x05110DA9; ///< I|IvvI
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 领取时间 生成唯一键

	uint32_t                  id;                           ///< 领取时间

	uint32_t                  award;                        ///< 领取气运值
	const char*               desc;                         ///< 领取时间显示
	const char*               display;                      ///< 道具展示
	uint32_t                  day;                          ///< 开服天数
};
#pragma pack()

struct GoldLuckydraw : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  award()                       { return t_award; }
	const std::string&        desc()                        { return t_desc; }
	const std::string&        display()                     { return t_display; }
	uint32_t                  day()                         { return t_day; }

private:
	uint32_t                  t_id;                         ///< 领取时间

	uint32_t                  t_award;                      ///< 领取气运值
	std::string               t_desc;                       ///< 领取时间显示
	std::string               t_display;                    ///< 道具展示
	uint32_t                  t_day;                        ///< 开服天数

public:
	void reset();
	void fill( const GoldLuckydrawBase& base );
	const char* getClassName() const { return "y运营活动表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< GoldLuckydraw, GoldLuckydrawBase > GoldLuckydrawManager;

inline void GoldLuckydraw::reset()
{
	this->t_id = 0;
	this->t_award = 0;
	this->t_desc.clear();
	this->t_display.clear();
	this->t_day = 0;
};

inline void GoldLuckydraw::fill( const GoldLuckydrawBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_award = base.award;
	if( base.desc == NULL ) this->t_desc.clear(); else this->t_desc.assign( base.desc );
	if( base.display == NULL ) this->t_display.clear(); else this->t_display.assign( base.display );
	this->t_day = base.day;
};

inline const std::string GoldLuckydraw::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "GoldLuckydraw.tbls64";
#else
	return "GoldLuckydraw.tbls32";
#endif
};
