#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    ���ṹ:HeroFamousBase
/// @remark   ��¼��С:16 X32
///           ��¼��С:20 X64
///           �ֶ�����:4
///           �����ļ�:m������.xlsx ��������
///           ��������:�������
#pragma pack(1)
struct HeroFamousBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00510EA9; ///< I|IVI
#else
	static const int          FormatCheckCode = 0x005110A9; ///< I|IvI
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ������� ����Ψһ��

	uint32_t                  id;                           ///< �������

	uint32_t                  hero_id;                      ///< �佫id
	const char*               need_hero;                    ///< �����佫id
	uint32_t                  famous;                       ///< ����������id
};
#pragma pack()

struct HeroFamous : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  hero_id()                     { return t_hero_id; }
	const std::vector<uint32_t>&        need_hero()                   { return t_need_hero; }
	uint32_t                  famous()                      { return t_famous; }

private:
	uint32_t                  t_id;                         ///< �������

	uint32_t                  t_hero_id;                    ///< �佫id
	std::vector<uint32_t>               t_need_hero;                  ///< �����佫id
	uint32_t                  t_famous;                     ///< ����������id

public:
	void reset();
	void fill( const HeroFamousBase& base );
	const char* getClassName() const { return "m������"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< HeroFamous, HeroFamousBase > HeroFamousManager;

inline void HeroFamous::reset()
{
	this->t_id = 0;
	this->t_hero_id = 0;
	this->t_need_hero.clear();
	this->t_famous = 0;
};

inline void HeroFamous::fill( const HeroFamousBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_hero_id = base.hero_id;
	//if( base.need_hero == NULL ) this->t_need_hero.clear(); else this->t_need_hero.assign( base.need_hero );
	Dawn::parseContainer<uint32_t>(this->t_need_hero, base.need_hero, "$");
	this->t_famous = base.famous;
};

inline const std::string HeroFamous::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "HeroFamous.tbls64";
#else
	return "HeroFamous.tbls32";
#endif
};
