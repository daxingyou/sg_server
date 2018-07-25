#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:MailTableBase
/// @remark   记录大小:20 X32
///           记录大小:36 X64
///           字段数量:5
///           数据文件:y邮件表.xlsx 邮件
///           排序主键:编号ID
#pragma pack(1)
struct MailTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0511BBB6; ///< I|VVVV
#else
	static const int          FormatCheckCode = 0x0513DDD6; ///< I|vvvv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 编号ID 生成唯一键

	uint32_t                  id;                           ///< 编号ID

	const char*               from_name;                    ///< 发送者
	const char*               title;                        ///< 标题
	const char*               content;                      ///< 内容
	const char*               addenda;                      ///< 附件
};
#pragma pack()

struct MailTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::string&        from_name()                   { return t_from_name; }
	const std::string&        title()                       { return t_title; }
	const std::string&        content()                     { return t_content; }
	const std::string&        addenda()                     { return t_addenda; }

private:
	uint32_t                  t_id;                         ///< 编号ID

	std::string               t_from_name;                  ///< 发送者
	std::string               t_title;                      ///< 标题
	std::string               t_content;                    ///< 内容
	std::string               t_addenda;                    ///< 附件

public:
	void reset();
	void fill( const MailTableBase& base );
	const char* getClassName() const { return "y邮件表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< MailTable, MailTableBase > MailTableManager;

inline void MailTable::reset()
{
	this->t_id = 0;
	this->t_from_name.clear();
	this->t_title.clear();
	this->t_content.clear();
	this->t_addenda.clear();
};

inline void MailTable::fill( const MailTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	if( base.from_name == NULL ) this->t_from_name.clear(); else this->t_from_name.assign( base.from_name );
	if( base.title == NULL ) this->t_title.clear(); else this->t_title.assign( base.title );
	if( base.content == NULL ) this->t_content.clear(); else this->t_content.assign( base.content );
	if( base.addenda == NULL ) this->t_addenda.clear(); else this->t_addenda.assign( base.addenda );
};

inline const std::string MailTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "MailTable.tbls64";
#else
	return "MailTable.tbls32";
#endif
};
