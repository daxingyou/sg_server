#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:PatrolstarBase
/// @remark   记录大小:8 X32
///           记录大小:8 X64
///           字段数量:2
///           数据文件:x巡逻表.xlsx 星级加成
///           排序主键:武将星级
#pragma pack(1)
struct PatrolstarBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#else
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 武将星级 生成唯一键

	uint32_t                  id;                           ///< 武将星级

	uint32_t                  drop_add;                     ///< 加成万分比
};
#pragma pack()

struct Patrolstar : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  drop_add()                    { return t_drop_add; }

private:
	uint32_t                  t_id;                         ///< 武将星级

	uint32_t                  t_drop_add;                   ///< 加成万分比

public:
	void reset();
	void fill( const PatrolstarBase& base );
	const char* getClassName() const { return "x巡逻表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Patrolstar, PatrolstarBase > PatrolstarManager;

inline void Patrolstar::reset()
{
	this->t_id = 0;
	this->t_drop_add = 0;
};

inline void Patrolstar::fill( const PatrolstarBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_drop_add = base.drop_add;
};

inline const std::string Patrolstar::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Patrolstar.tbls64";
#else
	return "Patrolstar.tbls32";
#endif
};
