#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:LiandanluTableBase
/// @remark   ��¼��С:8 X32
///           ��¼��С:12 X64
///           �ֶ�����:2
///           �����ļ�:h�ϳɱ�.xlsx ����¯
///           ��������:������λID
#pragma pack(1)
struct LiandanluTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005116; ///< I|V
#else
	static const int          FormatCheckCode = 0x00005136; ///< I|v
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ������λID ����Ψһ��

	uint32_t                  id;                           ///< ������λID

	const char*               material;                     ///< ��������
};
#pragma pack()

struct LiandanluTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::map<uint32_t, uint32_t>&        material()                    { return t_material; }

private:
	uint32_t                  t_id;                         ///< ������λID

	std::map<uint32_t, uint32_t>               t_material;                   ///< ��������

public:
	void reset();
	void fill( const LiandanluTableBase& base );
	const char* getClassName() const { return "h�ϳɱ�"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< LiandanluTable, LiandanluTableBase > LiandanluTableManager;

inline void LiandanluTable::reset()
{
	this->t_id = 0;
	this->t_material.clear();
};

inline void LiandanluTable::fill( const LiandanluTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	//if( base.material == NULL ) this->t_material.clear(); else this->t_material.assign( base.material );
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_material, base.material, ":$");
};

inline const std::string LiandanluTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "LiandanluTable.tbls64";
#else
	return "LiandanluTable.tbls32";
#endif
};
