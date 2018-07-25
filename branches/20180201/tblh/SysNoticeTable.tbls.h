#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:SysNoticeTableBase
/// @remark   ��¼��С:20 X32
///           ��¼��С:24 X64
///           �ֶ�����:5
///           �����ļ�:xϵͳ֪ͨ.xlsx Sheet
///           ��������:���ID
#pragma pack(1)
struct SysNoticeTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0511ADD9; ///< I|VIII
#else
	static const int          FormatCheckCode = 0x0513ADD9; ///< I|vIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ���ID ����Ψһ��

	uint32_t                  id;                           ///< ���ID

	const char*               text;                         ///< ������Ϣ
	uint32_t                  notice_type;                  ///< ��ʾ����
	uint32_t                  priority;                     ///< ���ȼ�
	uint32_t                  is_init;                      ///< �Ƿ�Ϊ��ʼ����
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
	uint32_t                  t_id;                         ///< ���ID

	std::string               t_text;                       ///< ������Ϣ
	uint32_t                  t_notice_type;                ///< ��ʾ����
	uint32_t                  t_priority;                   ///< ���ȼ�
	uint32_t                  t_is_init;                    ///< �Ƿ�Ϊ��ʼ����

public:
	void reset();
	void fill( const SysNoticeTableBase& base );
	const char* getClassName() const { return "xϵͳ֪ͨ"; }

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
