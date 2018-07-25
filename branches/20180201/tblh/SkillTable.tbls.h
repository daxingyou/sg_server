#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:SkillTableBase
/// @remark   记录大小:4 X32
///           记录大小:4 X64
///           字段数量:1
///           数据文件:j技能.xlsx NewSkill
///           排序主键:技能ID
#pragma pack(1)
struct SkillTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0000050C; ///< I|
#else
	static const int          FormatCheckCode = 0x0000050C; ///< I|
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 技能ID 生成唯一键

	uint32_t                  id;                           ///< 技能ID

};
#pragma pack()

struct SkillTable : public Entry
{
	uint32_t                  id()                          { return t_id; }


private:
	uint32_t                  t_id;                         ///< 技能ID


public:
	void reset();
	void fill( const SkillTableBase& base );
	const char* getClassName() const { return "j技能"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< SkillTable, SkillTableBase > SkillTableManager;

inline void SkillTable::reset()
{
	this->t_id = 0;
};

inline void SkillTable::fill( const SkillTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
};

inline const std::string SkillTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "SkillTable.tbls64";
#else
	return "SkillTable.tbls32";
#endif
};
