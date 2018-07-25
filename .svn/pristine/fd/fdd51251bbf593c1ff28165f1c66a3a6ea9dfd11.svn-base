#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:RestraintTableBase
/// @remark   记录大小:20 X32
///           记录大小:20 X64
///           字段数量:5
///           数据文件:x系别克制表.xlsx 系别对应编号
///           排序主键:攻击方系别,防御方系别
#pragma pack(1)
struct RestraintTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x04E10DD9; ///< II|III
#else
	static const int          FormatCheckCode = 0x04E10DD9; ///< II|III
#endif

	//------------------------
	uint32_t getUniqueID() const { return hash_util_t::hash_to_uint32(att_terry, be_att_terry); }; // TODO: 请用 克制系别,被克制系别 生成唯一键

	uint32_t                  att_terry;                    ///< 克制系别
	uint32_t                  be_att_terry;                 ///< 被克制系别

	uint32_t                  stage;                        ///< 对应阶数
	uint32_t                  restraint;                    ///< 克制系数
	uint32_t                  relation;                     ///< 克制关系
};
#pragma pack()

struct RestraintTable : public Entry
{
	uint32_t                  att_terry()                   { return t_att_terry; }
	uint32_t                  be_att_terry()                { return t_be_att_terry; }

	uint32_t                  stage()                       { return t_stage; }
	uint32_t                  restraint()                   { return t_restraint; }
	uint32_t                  relation()                    { return t_relation; }

private:
	uint32_t                  t_att_terry;                  ///< 克制系别
	uint32_t                  t_be_att_terry;               ///< 被克制系别

	uint32_t                  t_stage;                      ///< 对应阶数
	uint32_t                  t_restraint;                  ///< 克制系数
	uint32_t                  t_relation;                   ///< 克制关系

public:
	void reset();
	void fill( const RestraintTableBase& base );
	const char* getClassName() const { return "x系别克制表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< RestraintTable, RestraintTableBase > RestraintTableManager;

inline void RestraintTable::reset()
{
	this->t_att_terry = 0;
	this->t_be_att_terry = 0;
	this->t_stage = 0;
	this->t_restraint = 0;
	this->t_relation = 0;
};

inline void RestraintTable::fill( const RestraintTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_att_terry = base.att_terry;
	this->t_be_att_terry = base.be_att_terry;
	this->t_stage = base.stage;
	this->t_restraint = base.restraint;
	this->t_relation = base.relation;
};

inline const std::string RestraintTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "RestraintTable.tbls64";
#else
	return "RestraintTable.tbls32";
#endif
};
