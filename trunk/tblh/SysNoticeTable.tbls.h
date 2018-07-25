#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:SysNoticeTableBase
/// @remark   记录大小:20 X32
///           记录大小:24 X64
///           字段数量:5
///           数据文件:x系统通知.xlsx Sheet
///           排序主键:编号ID
#pragma pack(1)
struct SysNoticeTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0511ADD9; ///< I|VIII
#else
	static const int          FormatCheckCode = 0x0513ADD9; ///< I|vIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 编号ID 生成唯一键

	uint32_t                  id;                           ///< 编号ID

	const char*               text;                         ///< 文字信息
	uint32_t                  notice_type;                  ///< 显示区域
	uint32_t                  priority;                     ///< 优先级
	uint32_t                  is_init;                      ///< 是否为初始公告
};
#pragma pack()

struct SysNoticeTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::string&        text()                        { return t_text; }
	uint32_t                  notice_type()                 { return t_notice_type; }
	uint32_t                  priority()                    { return t_priority; }
	uint32_t                  is_init()                     { return t_is_init; }

private:
	uint32_t                  t_id;                         ///< 编号ID

	std::string               t_text;                       ///< 文字信息
	uint32_t                  t_notice_type;                ///< 显示区域
	uint32_t                  t_priority;                   ///< 优先级
	uint32_t                  t_is_init;                    ///< 是否为初始公告

public:
	void reset();
	void fill( const SysNoticeTableBase& base );
	const char* getClassName() const { return "x系统通知"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< SysNoticeTable, SysNoticeTableBase > SysNoticeTableManager;

inline void SysNoticeTable::reset()
{
	this->t_id = 0;
	this->t_text.clear();
	this->t_notice_type = 0;
	this->t_priority = 0;
	this->t_is_init = 0;
};

inline void SysNoticeTable::fill( const SysNoticeTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	if( base.text == NULL ) this->t_text.clear(); else this->t_text.assign( base.text );
	this->t_notice_type = base.notice_type;
	this->t_priority = base.priority;
	this->t_is_init = base.is_init;
};

inline const std::string SysNoticeTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "SysNoticeTable.tbls64";
#else
	return "SysNoticeTable.tbls32";
#endif
};
