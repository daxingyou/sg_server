#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    表格结构:MatchBase
/// @remark   记录大小:28 X32
///           记录大小:52 X64
///           字段数量:7
///           数据文件:y远征表.xlsx 匹配表
///           排序主键:ID
#pragma pack(1)
struct MatchBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x01BBBEA6; ///< I|VVVVVV
#else
	static const int          FormatCheckCode = 0x03DDD8C6; ///< I|vvvvvv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 ID 生成唯一键

	uint32_t                  id;                           ///< ID

	const char*               level_range;                  ///< 等级
	const char*               fight_level_sss;              ///< SSS
	const char*               fight_level_ss;               ///< SS
	const char*               fight_level_s;                ///< S
	const char*               fight_level_a;                ///< A
	const char*               fight_level_b;                ///< B
};
#pragma pack()

struct Match : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::pair<uint32_t,uint32_t>&        level_range()                 { return t_level_range; }
	const std::vector<uint32_t>&        fight_level_sss()             { return t_fight_level_sss; }
	const std::vector<uint32_t>&        fight_level_ss()              { return t_fight_level_ss; }
	const std::vector<uint32_t>&        fight_level_s()               { return t_fight_level_s; }
	const std::vector<uint32_t>&        fight_level_a()               { return t_fight_level_a; }
	const std::vector<uint32_t>&        fight_level_b()               { return t_fight_level_b; }

private:
	uint32_t                  t_id;                         ///< ID

	std::pair<uint32_t, uint32_t>               t_level_range;                ///< 等级
	std::vector<uint32_t>               t_fight_level_sss;            ///< SSS
	std::vector<uint32_t>               t_fight_level_ss;             ///< SS
	std::vector<uint32_t>               t_fight_level_s;              ///< S
	std::vector<uint32_t>				t_fight_level_a;              ///< A
	std::vector<uint32_t>               t_fight_level_b;              ///< B

public:
	void reset();
	void fill( const MatchBase& base );
	const char* getClassName() const { return "y远征表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Match, MatchBase > MatchManager;

inline void Match::reset()
{
	this->t_id = 0;
	//this->t_level_range.clear();
	this->t_fight_level_sss.clear();
	this->t_fight_level_ss.clear();
	this->t_fight_level_s.clear();
	this->t_fight_level_a.clear();
	this->t_fight_level_b.clear();
};

inline void Match::fill( const MatchBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	//if( base.level_range == NULL ) this->t_level_range.clear(); else this->t_level_range.assign( base.level_range );
	Dawn::parseTuple<uint32_t,uint32_t>(this->t_level_range, base.level_range, "~");
	//if( base.fight_level_sss == NULL ) this->t_fight_level_sss.clear(); else this->t_fight_level_sss.assign( base.fight_level_sss );
	Dawn::parseContainer<uint32_t>(this->t_fight_level_sss, base.fight_level_sss, "$");
	//if( base.fight_level_ss == NULL ) this->t_fight_level_ss.clear(); else this->t_fight_level_ss.assign( base.fight_level_ss );
	Dawn::parseContainer<uint32_t>(this->t_fight_level_ss, base.fight_level_ss, "$");
	//if( base.fight_level_s == NULL ) this->t_fight_level_s.clear(); else this->t_fight_level_s.assign( base.fight_level_s );
	Dawn::parseContainer<uint32_t>(this->t_fight_level_s, base.fight_level_s, "$");
	//if( base.fight_level_a == NULL ) this->t_fight_level_a.clear(); else this->t_fight_level_a.assign( base.fight_level_a );
	Dawn::parseContainer<uint32_t>(this->t_fight_level_a, base.fight_level_a, "$");
	//if( base.fight_level_b == NULL ) this->t_fight_level_b.clear(); else this->t_fight_level_b.assign( base.fight_level_b );
	Dawn::parseContainer<uint32_t>(this->t_fight_level_b, base.fight_level_b, "$");
};

inline const std::string Match::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Match.tbls64";
#else
	return "Match.tbls32";
#endif
};
