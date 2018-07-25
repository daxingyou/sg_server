#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:HeroTalentPosBase
/// @remark   ��¼��С:8 X32
///           ��¼��С:12 X64
///           �ֶ�����:2
///           �����ļ�:w�佫�츳�±�.xlsx �츳������
///           ��������:�츳λ��id
#pragma pack(1)
struct HeroTalentPosBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005116; ///< I|V
#else
	static const int          FormatCheckCode = 0x00005136; ///< I|v
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� �츳λ��id ����Ψһ��

	uint32_t                  id;                           ///< �츳λ��id

	const char*               cond;                         ///< ������Ҫλ��
};
#pragma pack()

struct HeroTalentPos : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::map<uint32_t,uint32_t>&        cond()                        { return t_cond; }

private:
	uint32_t                  t_id;                         ///< �츳λ��id

	std::map<uint32_t, uint32_t>               t_cond;                       ///< ������Ҫλ��

public:
	void reset();
	void fill( const HeroTalentPosBase& base );
	const char* getClassName() const { return "w�佫�츳�±�"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< HeroTalentPos, HeroTalentPosBase > HeroTalentPosManager;

inline void HeroTalentPos::reset()
{
	this->t_id = 0;
	this->t_cond.clear();
};

inline void HeroTalentPos::fill( const HeroTalentPosBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	//if( base.cond == NULL ) this->t_cond.clear(); else this->t_cond.assign( base.cond );
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_cond, base.cond, ":$");
};

inline const std::string HeroTalentPos::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "HeroTalentPos.tbls64";
#else
	return "HeroTalentPos.tbls32";
#endif
};
