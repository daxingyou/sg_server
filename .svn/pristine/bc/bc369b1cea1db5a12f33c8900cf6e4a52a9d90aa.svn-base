#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:VirtualNoticeTableBase
/// @remark   记录大小:20 X32
///           记录大小:24 X64
///           字段数量:5
///           数据文件:x虚假广播.xlsx Sheet
///           排序主键:编号ID
#pragma pack(1)
struct VirtualNoticeTableBase
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
	uint32_t                  params;                       ///< 参数个数
};
#pragma pack()

struct VirtualNoticeTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::string&        text()                        { return t_text; }
	uint32_t                  notice_type()                 { return t_notice_type; }
	uint32_t                  priority()                    { return t_priority; }
	uint32_t                  params()                      { return t_params; }

private:
	uint32_t                  t_id;                         ///< 编号ID

	std::string               t_text;                       ///< 文字信息
	uint32_t                  t_notice_type;                ///< 显示区域
	uint32_t                  t_priority;                   ///< 优先级
	uint32_t                  t_params;                     ///< 参数个数

public:
	void reset();
	void fill( const VirtualNoticeTableBase& base );
	const char* getClassName() const { return "x虚假广播"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< VirtualNoticeTable, VirtualNoticeTableBase > VirtualNoticeTableManager;

inline void VirtualNoticeTable::reset()
{
	this->t_id = 0;
	this->t_text.clear();
	this->t_notice_type = 0;
	this->t_priority = 0;
	this->t_params = 0;
};

inline void VirtualNoticeTable::fill( const VirtualNoticeTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	if( base.text == NULL ) this->t_text.clear(); else this->t_text.assign( base.text );
	this->t_notice_type = base.notice_type;
	this->t_priority = base.priority;
	this->t_params = base.params;
};

inline const std::string VirtualNoticeTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "VirtualNoticeTable.tbls64";
#else
	return "VirtualNoticeTable.tbls32";
#endif
};
