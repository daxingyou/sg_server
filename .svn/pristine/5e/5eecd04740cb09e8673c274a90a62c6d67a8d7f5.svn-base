#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:SevendayLoginBase
/// @remark   记录大小:16 X32
///           记录大小:20 X64
///           字段数量:4
///           数据文件:y运营活动表.xlsx 七日签到
///           排序主键:时间
#pragma pack(1)
struct SevendayLoginBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00510DE6; ///< I|IIV
#else
	static const int          FormatCheckCode = 0x00510E06; ///< I|IIv
#endif

	//------------------------
	uint32_t getUniqueID() const { return day; }; // TODO: 请用 时间 生成唯一键

	uint32_t                  day;                          ///< 时间

	uint32_t                  drop_id;                      ///< 奖励ID
	uint32_t                  day_show_id;                  ///< 每日展示ID
	const char*               show_id_list;                 ///< 展示ID
};
#pragma pack()

struct SevendayLogin : public Entry
{
	uint32_t                  day()                         { return t_day; }

	uint32_t                  drop_id()                     { return t_drop_id; }
	uint32_t                  day_show_id()                 { return t_day_show_id; }
	const std::string&        show_id_list()                { return t_show_id_list; }

private:
	uint32_t                  t_day;                        ///< 时间

	uint32_t                  t_drop_id;                    ///< 奖励ID
	uint32_t                  t_day_show_id;                ///< 每日展示ID
	std::string               t_show_id_list;               ///< 展示ID

public:
	void reset();
	void fill( const SevendayLoginBase& base );
	const char* getClassName() const { return "y运营活动表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< SevendayLogin, SevendayLoginBase > SevendayLoginManager;

inline void SevendayLogin::reset()
{
	this->t_day = 0;
	this->t_drop_id = 0;
	this->t_day_show_id = 0;
	this->t_show_id_list.clear();
};

inline void SevendayLogin::fill( const SevendayLoginBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_day = base.day;
	this->t_drop_id = base.drop_id;
	this->t_day_show_id = base.day_show_id;
	if( base.show_id_list == NULL ) this->t_show_id_list.clear(); else this->t_show_id_list.assign( base.show_id_list );
};

inline const std::string SevendayLogin::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "SevendayLogin.tbls64";
#else
	return "SevendayLogin.tbls32";
#endif
};
