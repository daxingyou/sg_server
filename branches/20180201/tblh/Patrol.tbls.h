#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:PatrolBase
/// @remark   记录大小:8 X32
///           记录大小:12 X64
///           字段数量:2
///           数据文件:x巡逻表.xlsx 巡逻基础
///           排序主键:场景ID
#pragma pack(1)
struct PatrolBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005116; ///< I|V
#else
	static const int          FormatCheckCode = 0x00005136; ///< I|v
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 场景ID 生成唯一键

	uint32_t                  id;                           ///< 场景ID

	const char*               level;                        ///< 推荐等级
};
#pragma pack()

struct Patrol : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::pair<uint32_t, uint32_t>&        level()                       { return t_level; }

private:
	uint32_t                  t_id;                         ///< 场景ID

	std::pair<uint32_t, uint32_t>               t_level;                      ///< 推荐等级

public:
	void reset();
	void fill( const PatrolBase& base );
	const char* getClassName() const { return "x巡逻表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Patrol, PatrolBase > PatrolManager;

inline void Patrol::reset()
{
	this->t_id = 0;
	//this->t_level.clear();
};

inline void Patrol::fill( const PatrolBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	//if( base.level == NULL ) this->t_level.clear(); else this->t_level.assign( base.level );
	Dawn::parseTuple<uint32_t, uint32_t>(this->t_level, base.level, "~");
};

inline const std::string Patrol::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Patrol.tbls64";
#else
	return "Patrol.tbls32";
#endif
};
