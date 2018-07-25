#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"
#include "common/utility.hpp"
/// @brief    ���ṹ:HeadTableBase
/// @remark   ��¼��С:8 X32
///           ��¼��С:12 X64
///           �ֶ�����:2
///           �����ļ�:tͷ���.xlsx head
///           ��������:ͷ��id
#pragma pack(1)
struct HeadTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005116; ///< I|V
#else
	static const int          FormatCheckCode = 0x00005136; ///< I|v
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ͷ��id ����Ψһ��

	uint32_t                  id;                           ///< ͷ��id

	const char*               cond;                         ///< ��������
};
#pragma pack()

struct HeadTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::map<uint32_t, uint32_t>&        cond()       { return t_cond; }

private:
	uint32_t                  t_id;                         ///< ͷ��id

	std::map<uint32_t, uint32_t>   t_cond;                  ///< ��������

public:
	void reset();
	void fill( const HeadTableBase& base );
	const char* getClassName() const { return "tͷ���"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< HeadTable, HeadTableBase > HeadTableManager;

inline void HeadTable::reset()
{
	this->t_id = 0;
	this->t_cond.clear();
};

inline void HeadTable::fill( const HeadTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	///if( base.cond == NULL ) this->t_cond.clear(); else this->t_cond.assign( base.cond );
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_cond, base.cond, ":$");
};

inline const std::string HeadTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "HeadTable.tbls64";
#else
	return "HeadTable.tbls32";
#endif
};
