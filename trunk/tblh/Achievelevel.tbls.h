#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:AchievelevelBase
/// @remark   记录大小:12 X32
///           记录大小:12 X64
///           字段数量:3
///           数据文件:c成就表.xlsx 成就等级
///           排序主键:个人成就等级
#pragma pack(1)
struct AchievelevelBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x000510D9; ///< I|II
#else
	static const int          FormatCheckCode = 0x000510D9; ///< I|II
#endif

	//------------------------
	uint32_t getUniqueID() const { return level; }; // TODO: 请用 个人成就等级 生成唯一键

	uint32_t                  level;                        ///< 个人成就等级

	uint32_t                  level_up_point;               ///< 需求成就点
	uint32_t                  drop_id;                      ///< 奖励道具（掉落ID)
};
#pragma pack()

struct Achievelevel : public Entry
{
	uint32_t                  level()                       { return t_level; }

	uint32_t                  level_up_point()              { return t_level_up_point; }
	uint32_t                  drop_id()                     { return t_drop_id; }

private:
	uint32_t                  t_level;                      ///< 个人成就等级

	uint32_t                  t_level_up_point;             ///< 需求成就点
	uint32_t                  t_drop_id;                    ///< 奖励道具（掉落ID)

public:
	void reset();
	void fill( const AchievelevelBase& base );
	const char* getClassName() const { return "c成就表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Achievelevel, AchievelevelBase > AchievelevelManager;

inline void Achievelevel::reset()
{
	this->t_level = 0;
	this->t_level_up_point = 0;
	this->t_drop_id = 0;
};

inline void Achievelevel::fill( const AchievelevelBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_level = base.level;
	this->t_level_up_point = base.level_up_point;
	this->t_drop_id = base.drop_id;
};

inline const std::string Achievelevel::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Achievelevel.tbls64";
#else
	return "Achievelevel.tbls32";
#endif
};
