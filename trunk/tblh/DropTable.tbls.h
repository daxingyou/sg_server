#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "tblh/DropTableConfig.hpp"

/// @brief    ���ṹ:DropTableBase
/// @remark   ��¼��С:36 X32
///           ��¼��С:36 X64
///           �ֶ�����:9
///           �����ļ�:d�����.xlsx ���߰���
///           ��������:id
#pragma pack(1)
struct DropTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0CD00059; ///< I|IIIIIIIIIII
#else
	static const int          FormatCheckCode = 0x0CD00059; ///< I|IIIIIIIIIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� id ����Ψһ��

	uint32_t                  id;                           ///< id

	uint32_t                  drop_item_type;               ///< ��Ʒ����
	uint32_t                  group_id;                     ///< ���߰���ID
	uint32_t                  drop_num;                     ///< ��������С��������
	uint32_t                  prob_type;                    ///< ��������
	uint32_t                  rand_type;                    ///< ��������
	uint32_t                  drop_item_tid;                ///< �������ID
	uint32_t                  drop_prob;                    ///< �������
	uint32_t                  drop_item_num;                ///< ��������
	uint32_t                  cond_type;                    ///< ������������
	uint32_t                  cond_param1;                  ///< ���Ʋ���1
	uint32_t                  cond_param2;                  ///< ���Ʋ���2
};
#pragma pack()

struct DropTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  drop_item_type()				{ return t_drop_item_type; }
	uint32_t                  group_id()                    { return t_group_id; }
	uint32_t                  drop_num()                    { return t_drop_num; }
	uint32_t                  prob_type()                   { return t_prob_type; }
	uint32_t                  rand_type()                   { return t_rand_type; }
	uint32_t                  drop_item_tid()               { return t_drop_item_tid; }
	uint32_t                  drop_prob()                   { return t_drop_prob; }
	uint32_t                  drop_item_num()               { return t_drop_item_num; }
	uint32_t                  cond_type()                   { return t_cond_type; }
	uint32_t                  cond_param1()                 { return t_cond_param1; }
	uint32_t                  cond_param2()                 { return t_cond_param2; }

private:
	uint32_t                  t_id;                         ///< id
	uint32_t                  t_drop_item_type;             ///< ��Ʒ����
	uint32_t                  t_group_id;                   ///< ���߰���ID
	uint32_t                  t_drop_num;                   ///< ��������С��������
	uint32_t                  t_prob_type;                  ///< ��������
	uint32_t                  t_rand_type;                  ///< ��������
	uint32_t                  t_drop_item_tid;              ///< �������ID
	uint32_t                  t_drop_prob;                  ///< �������
	uint32_t                  t_drop_item_num;              ///< ��������
	uint32_t                  t_cond_type;                  ///< ������������
	uint32_t                  t_cond_param1;                ///< ���Ʋ���1
	uint32_t                  t_cond_param2;                ///< ���Ʋ���2

public:
	void reset();
	void fill( const DropTableBase& base );
	const char* getClassName() const { return "d�����"; }

	static const std::string getBinaryName();
};


typedef DataTableManager< DropTable, DropTableBase > DropTableManager;

inline void DropTable::reset()
{
	this->t_id = 0;
	this->t_drop_item_type = 0;
	this->t_group_id = 0;
	this->t_drop_num = 0;
	this->t_prob_type = 0;
	this->t_rand_type = 0;
	this->t_drop_item_tid = 0;
	this->t_drop_prob = 0;
	this->t_drop_item_num = 0;
	this->t_cond_type = 0;
	this->t_cond_param1 = 0;
	this->t_cond_param2 = 0;
};

inline void DropTable::fill( const DropTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_drop_item_type = base.drop_item_type;
	this->t_group_id = base.group_id;
	this->t_drop_num = base.drop_num;
	this->t_prob_type = base.prob_type;
	this->t_rand_type = base.rand_type;
	this->t_drop_item_tid = base.drop_item_tid;
	this->t_drop_prob = base.drop_prob;
	this->t_drop_item_num = base.drop_item_num;
	this->t_cond_type = base.cond_type;
	this->t_cond_param1 = base.cond_param1;
	this->t_cond_param2 = base.cond_param2;

	DropTableConfig_t::fill_drop_table(this->t_group_id, this->t_id);

};

inline const std::string DropTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "DropTable.tbls64";
#else
	return "DropTable.tbls32";
#endif
};
