#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:WsTreasuryTableBase
/// @remark   ��¼��С:52 X32
///           ��¼��С:56 X64
///           �ֶ�����:13
///           �����ļ�:y��Ӫ���.xlsx ��ʥ����
///           ��������:���
#pragma pack(1)
struct WsTreasuryTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0F222706; ///< I|IIIiiiiiiIIV
#else
	static const int          FormatCheckCode = 0x0F2228E6; ///< I|IIIiiiiiiIIv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ��� ����Ψһ��

	uint32_t                  id;                           ///< ���

	uint32_t                  drop_id;                      ///< ����ID
	uint32_t                  show_item_id;                 ///< չʾ����
	uint32_t                  show_item_num;                ///< չʾ����
	int32_t                   init_percent;                 ///< ��ʼȨ��
	int32_t                   dec_percent;                  ///< �鵽�������Ȩ��
	int32_t                   lucky_add_percent_val;        ///< ����ֵ����Ȩ��ϵ��
	int32_t                   add_lucky;                    ///< �鵽��������ֵ
	int32_t                   dec_lucky;                    ///< �鵽��������ֵ
	int32_t                   add_score;                    ///< �鵽��û���
	uint32_t                  notice;                       ///< ȫ������
	uint32_t                  notice_id;                    ///< ����ID
	const char*               item_name;                    ///< ���Ʊ�ע
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
	uint32_t                  t_id;                         ///< ���

	uint32_t                  t_drop_id;                    ///< ����ID
	uint32_t                  t_show_item_id;               ///< չʾ����
	uint32_t                  t_show_item_num;              ///< չʾ����
	int32_t                   t_init_percent;               ///< ��ʼȨ��
	int32_t                   t_dec_percent;                ///< �鵽�������Ȩ��
	int32_t                   t_lucky_add_percent_val;      ///< ����ֵ����Ȩ��ϵ��
	int32_t                   t_add_lucky;                  ///< �鵽��������ֵ
	int32_t                   t_dec_lucky;                  ///< �鵽��������ֵ
	int32_t                   t_add_score;                  ///< �鵽��û���
	uint32_t                  t_notice;                     ///< ȫ������
	uint32_t                  t_notice_id;                  ///< ����ID
	std::string               t_item_name;                  ///< ���Ʊ�ע

public:
	void reset();
	void fill( const WsTreasuryTableBase& base );
	const char* getClassName() const { return "y��Ӫ���"; }

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
