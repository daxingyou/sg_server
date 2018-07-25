#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:FamilySkillBase
/// @remark   ��¼��С:24 X32
///           ��¼��С:32 X64
///           �ֶ�����:6
///           �����ļ�:j���弼��.xlsx ���
///           ��������:���ܱ��,�ȼ�
#pragma pack(1)
struct FamilySkillBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0E10DEF6; ///< II|IIVV
#else
	static const int          FormatCheckCode = 0x0E10E096; ///< II|IIvv
#endif

	//------------------------
	uint32_t getUniqueID() const { return hash_util_t::hash_to_uint32(id, level); }; // TODO: ���� ���ܱ��,�ȼ� ����Ψһ��

	uint32_t                  id;                           ///< ���ܱ��
	uint32_t                  level;                        ///< �ȼ�

	uint32_t                  need_role_level;              ///< �����ɫ�ȼ�
	uint32_t                  need_family_level;            ///< �������ȼ�
	const char*               cost_money;                   ///< ��������
	const char*               attr;                         ///< ��������
};
#pragma pack()

struct FamilySkill : public Entry
{
	uint32_t                  id()                          { return t_id; }
	uint32_t                  level()                       { return t_level; }

	uint32_t                  need_role_level()             { return t_need_role_level; }
	uint32_t                  need_family_level()           { return t_need_family_level; }
	const std::pair<uint32_t,uint32_t>&        cost_money()                  { return t_cost_money; }
	const std::map<uint32_t,uint32_t>&        attr()                        { return t_attr; }

private:
	uint32_t                  t_id;                         ///< ���ܱ��
	uint32_t                  t_level;                      ///< �ȼ�

	uint32_t                  t_need_role_level;            ///< �����ɫ�ȼ�
	uint32_t                  t_need_family_level;          ///< �������ȼ�
	std::pair<uint32_t, uint32_t>               t_cost_money;                 ///< ��������
	std::map<uint32_t, uint32_t>               t_attr;                       ///< ��������

public:
	void reset();
	void fill( const FamilySkillBase& base );
	const char* getClassName() const { return "j���弼��"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< FamilySkill, FamilySkillBase > FamilySkillManager;

inline void FamilySkill::reset()
{
	this->t_id = 0;
	this->t_level = 0;
	this->t_need_role_level = 0;
	this->t_need_family_level = 0;
	//this->t_cost_money.clear();
	this->t_attr.clear();
};

inline void FamilySkill::fill( const FamilySkillBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_level = base.level;
	this->t_need_role_level = base.need_role_level;
	this->t_need_family_level = base.need_family_level;
	//if( base.cost_money == NULL ) this->t_cost_money.clear(); else this->t_cost_money.assign( base.cost_money );
	Dawn::parseTuple(this->t_cost_money, base.cost_money, ":");
	//if( base.attr == NULL ) this->t_attr.clear(); else this->t_attr.assign( base.attr );
	Dawn::parseMapContainer(this->t_attr, base.attr, ":$");
};

inline const std::string FamilySkill::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "FamilySkill.tbls64";
#else
	return "FamilySkill.tbls32";
#endif
};
