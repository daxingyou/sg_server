#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:levelBase
/// @remark   记录大小:8 X32
///           记录大小:8 X64
///           字段数量:2
///           数据文件:z主公经验表.xlsx Sheet1
///           排序主键:等级
#pragma pack(1)
struct levelBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#else
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#endif

	//------------------------
	uint32_t getUniqueID() const { return level; }; // TODO: 请用 等级 生成唯一键

	uint32_t                  level;                        ///< 等级

	uint32_t                  level_up_exp;                 ///< 主公所需经验
};
#pragma pack()

struct level : public Entry
{
	uint32_t                  level()                       { return t_level; }

	uint32_t                  level_up_exp()                { return t_level_up_exp; }

private:
	uint32_t                  t_level;                      ///< 等级

	uint32_t                  t_level_up_exp;               ///< 主公所需经验

public:
	void reset();
	void fill( const levelBase& base );
	const char* getClassName() const { return "z主公经验表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< level, levelBase > levelManager;

inline void level::reset()
{
	this->t_level = 0;
	this->t_level_up_exp = 0;
};

inline void level::fill( const levelBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_level = base.level;
	this->t_level_up_exp = base.level_up_exp;
};

inline const std::string level::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "level.tbls64";
#else
	return "level.tbls32";
#endif
};
