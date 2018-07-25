#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:HeroTalentLevelBase
/// @remark   ��¼��С:20 X32
///           ��¼��С:24 X64
///           �ֶ�����:5
///           �����ļ�:w�佫�츳�±�.xlsx �츳������
///           ��������:�츳id,�츳�ȼ�
#pragma pack(1)
struct HeroTalentLevelBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x04E11AD9; ///< II|VII
#else
	static const int          FormatCheckCode = 0x04E13AD9; ///< II|vII
#endif

	//------------------------
	uint32_t getUniqueID() const { return hash_util_t::hash_to_uint32(id, level); }; // TODO: ���� �츳id,�츳�ȼ� ����Ψһ��

	uint32_t                  id;                           ///< �츳id
	uint32_t                  level;                        ///< �츳�ȼ�

	const char*               add_value;                    ///< ��������
	uint32_t                  consume_talent_point;         ///< ���������츳��
	uint32_t                  need_level;                   ///< �佫�ȼ�����
};
#pragma pack()

struct HeroTalentLevel : public Entry
{
	uint32_t                  id()                          { return t_id; }
	uint32_t                  level()                       { return t_level; }

	const std::map<uint32_t,uint32_t>&        add_value()                   { return t_add_value; }
	uint32_t                  consume_talent_point()        { return t_consume_talent_point; }
	uint32_t                  need_level()                  { return t_need_level; }

private:
	uint32_t                  t_id;                         ///< �츳id
	uint32_t                  t_level;                      ///< �츳�ȼ�

	std::map<uint32_t, uint32_t>               t_add_value;                  ///< ��������
	uint32_t                  t_consume_talent_point;       ///< ���������츳��
	uint32_t                  t_need_level;                 ///< �佫�ȼ�����

public:
	void reset();
	void fill( const HeroTalentLevelBase& base );
	const char* getClassName() const { return "w�佫�츳�±�"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< HeroTalentLevel, HeroTalentLevelBase > HeroTalentLevelManager;

inline void HeroTalentLevel::reset()
{
	this->t_id = 0;
	this->t_level = 0;
	this->t_add_value.clear();
	this->t_consume_talent_point = 0;
	this->t_need_level = 0;
};

inline void HeroTalentLevel::fill( const HeroTalentLevelBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_level = base.level;
	//if( base.add_value == NULL ) this->t_add_value.clear(); else this->t_add_value.assign( base.add_value );
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_add_value, base.add_value, ":$");
	this->t_consume_talent_point = base.consume_talent_point;
	this->t_need_level = base.need_level;
};

inline const std::string HeroTalentLevel::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "HeroTalentLevel.tbls64";
#else
	return "HeroTalentLevel.tbls32";
#endif
};
