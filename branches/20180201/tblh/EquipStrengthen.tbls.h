#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    ���ṹ:EquipStrengthenBase
/// @remark   ��¼��С:8 X32
///           ��¼��С:12 X64
///           �ֶ�����:2
///           �����ļ�:zװ����.xlsx װ��ǿ��
///           ��������:װ���ȼ�
#pragma pack(1)
struct EquipStrengthenBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005116; ///< I|V
#else
	static const int          FormatCheckCode = 0x00005136; ///< I|v
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� װ���ȼ� ����Ψһ��

	uint32_t                  id;                           ///< װ���ȼ�

	const char*               material;                     ///< �������Ĵ�
};
#pragma pack()

struct EquipStrengthen : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::vector<Dawn::quadruple<uint32_t, uint32_t, uint32_t, uint32_t>>&        material()                    { return t_material; }

private:
	uint32_t                  t_id;                         ///< װ���ȼ�

	std::vector<Dawn::quadruple<uint32_t, uint32_t, uint32_t, uint32_t>>               t_material;                   ///< �������Ĵ�

public:
	void reset();
	void fill( const EquipStrengthenBase& base );
	const char* getClassName() const { return "zװ����"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< EquipStrengthen, EquipStrengthenBase > EquipStrengthenManager;

inline void EquipStrengthen::reset()
{
	this->t_id = 0;
	this->t_material.clear();
};

inline void EquipStrengthen::fill( const EquipStrengthenBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	//if( base.material == NULL ) this->t_material.clear(); else this->t_material.assign( base.material );
	Dawn::parseTupleContainer<Dawn::quadruple<uint32_t, uint32_t, uint32_t, uint32_t>>(this->t_material, base.material, ":$");
};

inline const std::string EquipStrengthen::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "EquipStrengthen.tbls64";
#else
	return "EquipStrengthen.tbls32";
#endif
};
