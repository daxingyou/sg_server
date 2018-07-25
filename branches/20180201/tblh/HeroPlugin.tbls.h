#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"
#include "common/utility.hpp"
/// @brief    ���ṹ:HeroPluginBase
/// @remark   ��¼��С:20 X32
///           ��¼��С:24 X64
///           �ֶ�����:5
///           �����ļ�:w�佫Ƥ��������.xlsx Sheet1
///           ��������:ģ��id
#pragma pack(1)
struct HeroPluginBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0510DDE6; ///< I|IIIV
#else
	static const int          FormatCheckCode = 0x0510DE06; ///< I|IIIv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ģ��id ����Ψһ��

	uint32_t                  id;                           ///< ģ��id

	uint32_t                  hero_id;                      ///< �佫id
	uint32_t                  type;                         ///< ģ������
	uint32_t                  unlock_cond;                  ///< ��������
	const char*               attr;                         ///< ʱװ������
};
#pragma pack()

struct HeroPlugin : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  hero_id()                     { return t_hero_id; }
	uint32_t                  type()                        { return t_type; }
	uint32_t                  unlock_cond()                 { return t_unlock_cond; }
	const std::map<uint32_t, uint32_t>&  attr()             { return t_attr; }

private:
	uint32_t                  t_id;                         ///< ģ��id

	uint32_t                  t_hero_id;                    ///< �佫id
	uint32_t                  t_type;                       ///< ģ������
	uint32_t                  t_unlock_cond;                ///< ��������
	std::map<uint32_t, uint32_t>     t_attr;                ///< ʱװ������

public:
	void reset();
	void fill( const HeroPluginBase& base );
	const char* getClassName() const { return "w�佫Ƥ��������"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< HeroPlugin, HeroPluginBase > HeroPluginManager;

inline void HeroPlugin::reset()
{
	this->t_id = 0;
	this->t_hero_id = 0;
	this->t_type = 0;
	this->t_unlock_cond = 0;
	this->t_attr.clear();
};

inline void HeroPlugin::fill( const HeroPluginBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_hero_id = base.hero_id;
	this->t_type = base.type;
	this->t_unlock_cond = base.unlock_cond;
	///if( base.attr == NULL ) this->t_attr.clear(); else this->t_attr.assign( base.attr );
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_attr, base.attr, ":|");
};

inline const std::string HeroPlugin::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "HeroPlugin.tbls64";
#else
	return "HeroPlugin.tbls32";
#endif
};
