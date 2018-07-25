#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:CountryBase
/// @remark   ��¼��С:28 X32
///           ��¼��С:32 X64
///           �ֶ�����:7
///           �����ļ�:g���ұ�.xlsx country
///           ��������:ID
#pragma pack(1)
struct CountryBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x01ADD8C9; ///< I|VIIIII
#else
	static const int          FormatCheckCode = 0x03ADD8C9; ///< I|vIIIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ID ����Ψһ��

	uint32_t                  id;                           ///< id

	const char*               name;                         ///< ��ʼ��������
	uint32_t                  capital;                      ///< ����
	uint32_t                  att_area_id;                  ///< ����������
	uint32_t                  def_area_id;                  ///< ���ط�����
	uint32_t                  att_jump_id;                  ///< ��������ת��
	uint32_t                  def_jump_id;                  ///< ���ط���ת��
};
#pragma pack()

struct Country : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::string&        name()                        { return t_name; }
	uint32_t                  capital()                     { return t_capital; }
	uint32_t                  att_area_id()                 { return t_att_area_id; }
	uint32_t                  def_area_id()                 { return t_def_area_id; }
	uint32_t                  att_jump_id()                 { return t_att_jump_id; }
	uint32_t                  def_jump_id()                 { return t_def_jump_id; }

private:
	uint32_t                  t_id;                         ///< id

	std::string               t_name;                       ///< ��������
	uint32_t                  t_capital;                    ///< ����
	uint32_t                  t_att_area_id;                ///< ����������
	uint32_t                  t_def_area_id;                ///< ���ط�����
	uint32_t                  t_att_jump_id;                ///< ��������ת��
	uint32_t                  t_def_jump_id;                ///< ���ط���ת��

public:
	void reset();
	void fill( const CountryBase& base );
	const char* getClassName() const { return "g���ұ�"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Country, CountryBase > CountryManager;

inline void Country::reset()
{
	this->t_id = 0;
	this->t_name.clear();
	this->t_capital = 0;
	this->t_att_area_id = 0;
	this->t_def_area_id = 0;
	this->t_att_jump_id = 0;
	this->t_def_jump_id = 0;
};

inline void Country::fill( const CountryBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	if( base.name == NULL ) this->t_name.clear(); else this->t_name.assign( base.name );
	this->t_capital = base.capital;
	this->t_att_area_id = base.att_area_id;
	this->t_def_area_id = base.def_area_id;
	this->t_att_jump_id = base.att_jump_id;
	this->t_def_jump_id = base.def_jump_id;
};

inline const std::string Country::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Country.tbls64";
#else
	return "Country.tbls32";
#endif
};
