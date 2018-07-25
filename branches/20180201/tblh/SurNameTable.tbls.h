#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:SurNameTableBase
/// @remark   ��¼��С:8 X32
///           ��¼��С:12 X64
///           �ֶ�����:2
///           �����ļ�:m���Ʊ�.xlsx surname
///           ��������:id
#pragma pack(1)
struct SurNameTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005116; ///< I|V
#else
	static const int          FormatCheckCode = 0x00005136; ///< I|v
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� id ����Ψһ��

	uint32_t                  id;                           ///< ID

	const char*               name;                         ///< ��
};
#pragma pack()

struct SurNameTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::string&        name()                        { return t_name; }

private:
	uint32_t                  t_id;                         ///< ID

	std::string               t_name;                       ///< ��

public:
	void reset();
	void fill( const SurNameTableBase& base );
	const char* getClassName() const { return "m���Ʊ�"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< SurNameTable, SurNameTableBase > SurNameTableManager;

inline void SurNameTable::reset()
{
	this->t_id = 0;
	this->t_name.clear();
};

inline void SurNameTable::fill( const SurNameTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	if( base.name == NULL ) this->t_name.clear(); else this->t_name.assign( base.name );
};

inline const std::string SurNameTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "SurNameTable.tbls64";
#else
	return "SurNameTable.tbls32";
#endif
};
