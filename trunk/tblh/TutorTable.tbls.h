#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:TutorTableBase
/// @remark   ��¼��С:32 X32
///           ��¼��С:40 X64
///           �ֶ�����:8
///           �����ļ�:d��ʦ��.xlsx Sheet1
///           ��������:ID
#pragma pack(1)
struct TutorTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0ADD8FB9; ///< I|VIIIIVI
#else
	static const int          FormatCheckCode = 0x0ADD9199; ///< I|vIIIIvI
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ID ����Ψһ��

	uint32_t                  id;                           ///< ID

	const char*               name;                         ///< ��ʾ��
	uint32_t                  plugin;                       ///< ����ģ��
	uint32_t                  level;                        ///< �ȼ�
	uint32_t                  power;                        ///< ս��
	uint32_t                  fight_id;                     ///< ����
	const char*               title;                        ///< �ƺ�
	uint32_t                  head_id;                      ///< ͷ��id
};
#pragma pack()

struct TutorTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::string&        name()                        { return t_name; }
	uint32_t                  plugin()                      { return t_plugin; }
	uint32_t                  level()                       { return t_level; }
	uint32_t                  power()                       { return t_power; }
	uint32_t                  fight_id()                    { return t_fight_id; }
	const std::string&        title()                       { return t_title; }
	uint32_t                  head_id()                     { return t_head_id; }

private:
	uint32_t                  t_id;                         ///< ID

	std::string               t_name;                       ///< ��ʾ��
	uint32_t                  t_plugin;                     ///< ����ģ��
	uint32_t                  t_level;                      ///< �ȼ�
	uint32_t                  t_power;                      ///< ս��
	uint32_t                  t_fight_id;                   ///< ����
	std::string               t_title;                      ///< �ƺ�
	uint32_t                  t_head_id;                    ///< ͷ��id

public:
	void reset();
	void fill( const TutorTableBase& base );
	const char* getClassName() const { return "d��ʦ��"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< TutorTable, TutorTableBase > TutorTableManager;

inline void TutorTable::reset()
{
	this->t_id = 0;
	this->t_name.clear();
	this->t_plugin = 0;
	this->t_level = 0;
	this->t_power = 0;
	this->t_fight_id = 0;
	this->t_title.clear();
	this->t_head_id = 0;
};

inline void TutorTable::fill( const TutorTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	if( base.name == NULL ) this->t_name.clear(); else this->t_name.assign( base.name );
	this->t_plugin = base.plugin;
	this->t_level = base.level;
	this->t_power = base.power;
	this->t_fight_id = base.fight_id;
	if( base.title == NULL ) this->t_title.clear(); else this->t_title.assign( base.title );
	this->t_head_id = base.head_id;
};

inline const std::string TutorTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "TutorTable.tbls64";
#else
	return "TutorTable.tbls32";
#endif
};
