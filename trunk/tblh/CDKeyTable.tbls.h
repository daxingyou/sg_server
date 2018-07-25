#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    ���ṹ:CDKeyTableBase
/// @remark   ��¼��С:12 X32
///           ��¼��С:16 X64
///           �ֶ�����:3
///           �����ļ�:d�һ���.xlsx �һ���
///           ��������:�������id
#pragma pack(1)
struct CDKeyTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x000510E6; ///< I|IV
#else
	static const int          FormatCheckCode = 0x00051106; ///< I|Iv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� �������id ����Ψһ��

	uint32_t                  id;                           ///< �������id

	uint32_t                  mail_id;                      ///< �ʼ�id
	const char*               drop_list;                    ///< ����id
};
#pragma pack()

struct CDKeyTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  mail_id()                     { return t_mail_id; }
	//const std::string&        drop_list()                   { return t_drop_list; }
	const std::map<uint32_t, uint32_t>&        drop_list()  { return t_drop_list; }

private:
	uint32_t                  t_id;                         ///< �������id

	uint32_t                  t_mail_id;                    ///< �ʼ�id
	//std::string               t_drop_list;                  ///< ����id
	std::map<uint32_t, uint32_t> t_drop_list;				///������
public:
	void reset();
	void fill( const CDKeyTableBase& base );
	const char* getClassName() const { return "d�һ���"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< CDKeyTable, CDKeyTableBase > CDKeyTableManager;

inline void CDKeyTable::reset()
{
	this->t_id = 0;
	this->t_mail_id = 0;
	this->t_drop_list.clear();
};

inline void CDKeyTable::fill( const CDKeyTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_mail_id = base.mail_id;
	//if( base.drop_list == NULL ) this->t_drop_list.clear(); else this->t_drop_list.assign( base.drop_list );
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_drop_list, base.drop_list, ":$");
};

inline const std::string CDKeyTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "CDKeyTable.tbls64";
#else
	return "CDKeyTable.tbls32";
#endif
};
