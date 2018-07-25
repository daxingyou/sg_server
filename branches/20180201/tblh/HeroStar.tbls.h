#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    ���ṹ:HeroStarBase
/// @remark   ��¼��С:36 X32
///           ��¼��С:44 X64
///           �ֶ�����:9
///           �����ļ�:w�佫�Ǽ���.xlsx �佫�Ǽ���
///           ��������:�佫id,�佫�Ǽ�
#pragma pack(1)
struct HeroStarBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0CB0DD19; ///< II|iViiViii
#else
	static const int          FormatCheckCode = 0x0CCEDCF9; ///< II|iviiviii
#endif

	//------------------------
	uint32_t getUniqueID() const { return hash_util_t::hash_to_uint32(id, star); }; // TODO: ���� �佫id,�佫�Ǽ� ����Ψһ��

	uint32_t                  id;                           ///< �佫id
	uint32_t                  star;                         ///< �佫�Ǽ�

	int32_t                   need_level;                   ///< ���ǵȼ�����
	const char*               consume_material;             ///< ������Ҫ����/����
	int32_t                   consume_money;                ///< ��������ͭ��
	int32_t                   consume_herosoul;             ///< �������Ľ���
	const char*               attrs;                        ///< ���Դ�(�����ȡ��)
	int32_t                   atk_addval;                   ///< �����ɳ�
	int32_t                   def_addval;                   ///< �����ɳ�
	int32_t                   hp_addval;                    ///< ����ֵ�ɳ�
};
#pragma pack()

struct HeroStar : public Entry
{
	uint32_t                  id()                          { return t_id; }
	uint32_t                  star()                        { return t_star; }

	int32_t                   need_level()                  { return t_need_level; }
	const std::map<uint32_t, uint32_t>&        consume_material()            { return t_consume_material; }
	int32_t                   consume_money()               { return t_consume_money; }
	int32_t                   consume_herosoul() { return t_consume_herosoul; }
	const std::map<uint32_t, uint32_t>&        attrs()                       { return t_attrs; }
	int32_t                   atk_addval()                  { return t_atk_addval; }
	int32_t                   def_addval()                  { return t_def_addval; }
	int32_t                   hp_addval()                   { return t_hp_addval; }

private:
	uint32_t                  t_id;                         ///< �佫id
	uint32_t                  t_star;                       ///< �佫�Ǽ�

	int32_t                   t_need_level;                 ///< ���ǵȼ�����
	std::map<uint32_t, uint32_t>               t_consume_material;           ///< ������Ҫ����/����
	int32_t                   t_consume_money;              ///< ��������ͭ��
	int32_t                   t_consume_herosoul;           ///< �������Ľ���
	std::map<uint32_t, uint32_t>               t_attrs;                      ///< ���Դ�(�����ȡ��)
	int32_t                   t_atk_addval;                 ///< �����ɳ�
	int32_t                   t_def_addval;                 ///< �����ɳ�
	int32_t                   t_hp_addval;                  ///< ����ֵ�ɳ�

public:
	void reset();
	void fill( const HeroStarBase& base );
	const char* getClassName() const { return "w�佫�Ǽ���"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< HeroStar, HeroStarBase > HeroStarManager;

inline void HeroStar::reset()
{
	this->t_id = 0;
	this->t_star = 0;
	this->t_need_level = 0;
	this->t_consume_material.clear();
	this->t_consume_money = 0;
	this->t_consume_herosoul = 0;
	this->t_attrs.clear();
	this->t_atk_addval = 0;
	this->t_def_addval = 0;
	this->t_hp_addval = 0;
};

inline void HeroStar::fill( const HeroStarBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_star = base.star;
	this->t_need_level = base.need_level;
	//if( base.consume_material == NULL ) this->t_consume_material.clear(); else this->t_consume_material.assign( base.consume_material );
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_consume_material, base.consume_material, ":$");
	this->t_consume_money = base.consume_money;
	this->t_consume_herosoul = base.consume_herosoul;
	//if( base.attrs == NULL ) this->t_attrs.clear(); else this->t_attrs.assign( base.attrs );
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_attrs, base.attrs, ":$");
	this->t_atk_addval = base.atk_addval;
	this->t_def_addval = base.def_addval;
	this->t_hp_addval = base.hp_addval;
};

inline const std::string HeroStar::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "HeroStar.tbls64";
#else
	return "HeroStar.tbls32";
#endif
};
