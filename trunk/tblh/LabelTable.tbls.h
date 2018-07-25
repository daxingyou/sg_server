#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"
#include "common/utility.hpp"
/// @brief    ���ṹ:LabelTableBase
/// @remark   ��¼��С:20 X32
///           ��¼��С:32 X64
///           �ֶ�����:5
///           �����ļ�:tͷ�α�.xlsx Sheet1
///           ��������:��ʦ�ƺ�ID
#pragma pack(1)
struct LabelTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0510EBB6; ///< I|IVVV
#else
	static const int          FormatCheckCode = 0x05110DD6; ///< I|Ivvv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ��ʦ�ƺ�ID ����Ψһ��

	uint32_t                  id;                           ///< ��ʦ�ƺ�ID

	uint32_t                  fight_add;                    ///< ս���ӳ�
	const char*               attr_add;                     ///< ���Լӳ�
	const char*               up_level_cond;                ///< ����Ѫ������
	const char*               up_item_cond;                 ///< ������������
};
#pragma pack()

struct LabelTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  fight_add()                   { return t_fight_add; }
	const std::map<uint32_t, uint32_t>&        attr_add()                    { return t_attr_add; }
	const std::map<uint32_t, uint32_t>&        up_level_cond()               { return t_up_level_cond; }
	const std::map<uint32_t, uint32_t>&        up_item_cond()                { return t_up_item_cond; }

private:
	uint32_t                  t_id;                         ///< ��ʦ�ƺ�ID

	uint32_t                  t_fight_add;                  ///< ս���ӳ�
	std::map<uint32_t, uint32_t>      t_attr_add;           ///< ���Լӳ�
	std::map<uint32_t, uint32_t>      t_up_level_cond;      ///< ����Ѫ������
	std::map<uint32_t, uint32_t>      t_up_item_cond;       ///< ������������

public:
	void reset();
	void fill( const LabelTableBase& base );
	const char* getClassName() const { return "tͷ�α�"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< LabelTable, LabelTableBase > LabelTableManager;

inline void LabelTable::reset()
{
	this->t_id = 0;
	this->t_fight_add = 0;
	this->t_attr_add.clear();
	this->t_up_level_cond.clear();
	this->t_up_item_cond.clear();
};

inline void LabelTable::fill( const LabelTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_fight_add = base.fight_add;
	///if( base.attr_add == NULL ) this->t_attr_add.clear(); else this->t_attr_add.assign( base.attr_add );
	///if( base.up_level_cond == NULL ) this->t_up_level_cond.clear(); else this->t_up_level_cond.assign( base.up_level_cond );
	///if( base.up_item_cond == NULL ) this->t_up_item_cond.clear(); else this->t_up_item_cond.assign( base.up_item_cond );
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_attr_add, base.attr_add, ":$");
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_up_level_cond, base.up_level_cond, ":$");
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_up_item_cond, base.up_item_cond, ":$");
};

inline const std::string LabelTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "LabelTable.tbls64";
#else
	return "LabelTable.tbls32";
#endif
};
