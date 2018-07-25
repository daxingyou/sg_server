#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:SkillScoreBase
/// @remark   记录大小:12 X32
///           记录大小:12 X64
///           字段数量:3
///           数据文件:z战力转换表.xlsx 技能转战力
///           排序主键:技能ID,技能等级
#pragma pack(1)
struct SkillScoreBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0004E109; ///< II|I
#else
	static const int          FormatCheckCode = 0x0004E109; ///< II|I
#endif

	//------------------------
	uint32_t getUniqueID() const { return hash_util_t::hash_to_uint32(id, level); }; // TODO: 请用 技能ID,技能等级 生成唯一键

	uint32_t                  id;                           ///< 技能ID
	uint32_t                  level;                        ///< 技能等级

	uint32_t                  score;                        ///< 提升战力
};
#pragma pack()

struct SkillScore : public Entry
{
	uint32_t                  id()                          { return t_id; }
	uint32_t                  level()                       { return t_level; }

	uint32_t                  score()                       { return t_score; }

private:
	uint32_t                  t_id;                         ///< 技能ID
	uint32_t                  t_level;                      ///< 技能等级

	uint32_t                  t_score;                      ///< 提升战力

public:
	void reset();
	void fill( const SkillScoreBase& base );
	const char* getClassName() const { return "z战力转换表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< SkillScore, SkillScoreBase > SkillScoreManager;

inline void SkillScore::reset()
{
	this->t_id = 0;
	this->t_level = 0;
	this->t_score = 0;
};

inline void SkillScore::fill( const SkillScoreBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_level = base.level;
	this->t_score = base.score;
};

inline const std::string SkillScore::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "SkillScore.tbls64";
#else
	return "SkillScore.tbls32";
#endif
};
