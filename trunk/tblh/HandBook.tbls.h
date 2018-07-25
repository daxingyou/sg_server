#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    ���ṹ:HandBookBase
/// @remark   ��¼��С:16 X32
///           ��¼��С:24 X64
///           �ֶ�����:4
///           �����ļ�:s�ռ�������.xlsx Sheet1
///           ��������:�ռ�id
#pragma pack(1)
struct HandBookBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00511BA9; ///< I|VVI
#else
	static const int          FormatCheckCode = 0x00513DA9; ///< I|vvI
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� �ռ�id ����Ψһ��

	uint32_t                  id;                           ///< �ռ�id

	const char*               heros;                        ///< �佫id
	const char*               attr_add;                     ///< ���Խ���
	uint32_t                  drop;                         ///< ���߽���
};
#pragma pack()

struct HandBook : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::vector<uint32_t>&        heros()                       { return t_heros; }
	const std::map<uint32_t,uint32_t>&        attr_add()                    { return t_attr_add; }
	uint32_t                  drop()                        { return t_drop; }

private:
	uint32_t                  t_id;                         ///< �ռ�id

	std::vector<uint32_t>               t_heros;                      ///< �佫id
	std::map<uint32_t, uint32_t>               t_attr_add;                   ///< ���Խ���
	uint32_t                  t_drop;                       ///< ���߽���

public:
	void reset();
	void fill( const HandBookBase& base );
	const char* getClassName() const { return "s�ռ�������"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< HandBook, HandBookBase > HandBookManager;

inline void HandBook::reset()
{
	this->t_id = 0;
	this->t_heros.clear();
	this->t_attr_add.clear();
	this->t_drop = 0;
};

inline void HandBook::fill( const HandBookBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	//if( base.heros == NULL ) this->t_heros.clear(); else this->t_heros.assign( base.heros );
	Dawn::parseContainer<uint32_t>(this->t_heros, base.heros, "$");
	//if( base.attr_add == NULL ) this->t_attr_add.clear(); else this->t_attr_add.assign( base.attr_add );
	Dawn::parseMapContainer<uint32_t,uint32_t>(this->t_attr_add, base.attr_add, ":$");
	this->t_drop = base.drop;
};

inline const std::string HandBook::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "HandBook.tbls64";
#else
	return "HandBook.tbls32";
#endif
};
