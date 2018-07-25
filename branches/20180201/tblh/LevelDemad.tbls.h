#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:LevelDemadBase
/// @remark   记录大小:16 X32
///           记录大小:16 X64
///           字段数量:4
///           数据文件:w武将升级消耗表.xlsx Sheet2
///           排序主键:曲线编号,武将等级
#pragma pack(1)
struct LevelDemadBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x004E10D9; ///< II|II
#else
	static const int          FormatCheckCode = 0x004E10D9; ///< II|II
#endif

	//------------------------
	uint32_t getUniqueID() const { return hash_util_t::hash_to_uint32(id, level); }; // TODO: 请用 曲线编号,武将等级 生成唯一键

	uint32_t                  id;                           ///< 曲线编号
	uint32_t                  level;                        ///< 武将等级

	uint32_t                  exp;                          ///< 需要经验
	uint32_t                  exp_limit;                    ///< 经验上限
};
#pragma pack()

struct LevelDemad : public Entry
{
	uint32_t                  id()                          { return t_id; }
	uint32_t                  level()                       { return t_level; }

	uint32_t                  exp()                         { return t_exp; }
	uint32_t                  exp_limit()                   { return t_exp_limit; }

private:
	uint32_t                  t_id;                         ///< 曲线编号
	uint32_t                  t_level;                      ///< 武将等级

	uint32_t                  t_exp;                        ///< 需要经验
	uint32_t                  t_exp_limit;                  ///< 经验上限

public:
	void reset();
	void fill( const LevelDemadBase& base );
	const char* getClassName() const { return "w武将升级消耗表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< LevelDemad, LevelDemadBase > LevelDemadManager;

inline void LevelDemad::reset()
{
	this->t_id = 0;
	this->t_level = 0;
	this->t_exp = 0;
	this->t_exp_limit = 0;
};

inline void LevelDemad::fill( const LevelDemadBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_level = base.level;
	this->t_exp = base.exp;
	this->t_exp_limit = base.exp_limit;
};

inline const std::string LevelDemad::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "LevelDemad.tbls64";
#else
	return "LevelDemad.tbls32";
#endif
};
