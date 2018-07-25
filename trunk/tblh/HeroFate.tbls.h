#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ±í¸ñ½á¹¹:HeroFateBase
/// @remark   ¼ÇÂ¼´óĞ¡:16 X32
///           ¼ÇÂ¼´óĞ¡:16 X64
///           ×Ö¶ÎÊıÁ¿:4
///           Êı¾İÎÄ¼ş:hero_fate.xlsx data
///           ÅÅĞòÖ÷¼ü:¿¨ÅÆ±àºÅ,î¿°íĞòºÅ
#pragma pack(1)
struct HeroFateBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x004E10D9; ///< II|II
#else
	static const int          FormatCheckCode = 0x004E10D9; ///< II|II
#endif

	//------------------------
	uint32_t getUniqueID() const { return hash_util_t::hash_to_uint32(id, rank); }; // TODO: ÇëÓÃ ¿¨ÅÆ±àºÅ,î¿°íĞòºÅ Éú³ÉÎ¨Ò»¼ü

	uint32_t                  id;                           ///< ¿¨ÅÆ±àºÅ
	uint32_t                  rank;                         ///< î¿°íĞòºÅ

	uint32_t                  fate_id;                      ///< î¿°í±àºÅ
	uint32_t                  fate_hero;                    ///< î¿°íÎä½«id
};
#pragma pack()

struct HeroFate : public Entry
{
	uint32_t                  id()                          { return t_id; }
	uint32_t                  rank()                        { return t_rank; }

	uint32_t                  fate_id()                     { return t_fate_id; }
	uint32_t                  fate_hero()                   { return t_fate_hero; }

private:
	uint32_t                  t_id;                         ///< ¿¨ÅÆ±àºÅ
	uint32_t                  t_rank;                       ///< î¿°íĞòºÅ

	uint32_t                  t_fate_id;                    ///< î¿°í±àºÅ
	uint32_t                  t_fate_hero;                  ///< î¿°íÎä½«id

public:
	void reset();
	void fill( const HeroFateBase& base );
	const char* getClassName() const { return "hero_fate"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< HeroFate, HeroFateBase > HeroFateManager;

inline void HeroFate::reset()
{
	this->t_id = 0;
	this->t_rank = 0;
	this->t_fate_id = 0;
	this->t_fate_hero = 0;
};

inline void HeroFate::fill( const HeroFateBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_rank = base.rank;
	this->t_fate_id = base.fate_id;
	this->t_fate_hero = base.fate_hero;
};

inline const std::string HeroFate::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "HeroFate.tbls64";
#else
	return "HeroFate.tbls32";
#endif
};
