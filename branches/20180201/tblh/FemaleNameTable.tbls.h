#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:FemaleNameTableBase
/// @remark   ��¼��С:8 X32
///           ��¼��С:12 X64
///           �ֶ�����:2
///           �����ļ�:m���Ʊ�.xlsx femalename
///           ��������:id
#pragma pack(1)
struct FemaleNameTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005116; ///< I|V
#else
	static const int          FormatCheckCode = 0x00005136; ///< I|v
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� id ����Ψһ��

	uint32_t                  id;                           ///< ID

	const char*               name;                         ///< Ů��
};
#pragma pack()

struct FemaleNameTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::string&        name()                        { return t_name; }

private:
	uint32_t                  t_id;                         ///< ID

	std::string               t_name;                       ///< Ů��

public:
	void reset();
	void fill( const FemaleNameTableBase& base );
	const char* getClassName() const { return "m���Ʊ�"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< FemaleNameTable, FemaleNameTableBase > FemaleNameTableManager;

inline void FemaleNameTable::reset()
{
	this->t_id = 0;
	this->t_name.clear();
};

inline void FemaleNameTable::fill( const FemaleNameTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	if( base.name == NULL ) this->t_name.clear(); else this->t_name.assign( base.name );
};

inline const std::string FemaleNameTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "FemaleNameTable.tbls64";
#else
	return "FemaleNameTable.tbls32";
#endif
};
