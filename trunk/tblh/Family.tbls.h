#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:FamilyBase
/// @remark   记录大小:28 X32
///           记录大小:28 X64
///           字段数量:7
///           数据文件:j家族表.xlsx family_build
///           排序主键:ID
#pragma pack(1)
struct FamilyBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x02FFF8E9; ///< i|iiiiii
#else
	static const int          FormatCheckCode = 0x02FFF8E9; ///< i|iiiiii
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 ID 生成唯一键

	int32_t                   id;                           ///< id

	int32_t                   type;                         ///< 建筑类型
	int32_t                   level;                        ///< 建筑等级
	int32_t                   require;                      ///< 升级资金要求
	int32_t                   cost;                         ///< 升级资金消耗
	int32_t                   cd_time;                      ///< 升级冷却时间/秒
	int32_t                   cd_clean_cost;                ///< 清除冷却资金消耗
};
#pragma pack()

struct Family : public Entry
{
	int32_t                   id()                          { return t_id; }

	int32_t                   type()                        { return t_type; }
	int32_t                   level()                       { return t_level; }
	int32_t                   require()                     { return t_require; }
	int32_t                   cost()                        { return t_cost; }
	int32_t                   cd_time()                     { return t_cd_time; }
	int32_t                   cd_clean_cost()               { return t_cd_clean_cost; }

private:
	int32_t                   t_id;                         ///< id

	int32_t                   t_type;                       ///< 建筑类型
	int32_t                   t_level;                      ///< 建筑等级
	int32_t                   t_require;                    ///< 升级资金要求
	int32_t                   t_cost;                       ///< 升级资金消耗
	int32_t                   t_cd_time;                    ///< 升级冷却时间/秒
	int32_t                   t_cd_clean_cost;              ///< 清除冷却资金消耗

public:
	void reset();
	void fill( const FamilyBase& base );
	const char* getClassName() const { return "j家族表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Family, FamilyBase > FamilyManager;

inline void Family::reset()
{
	this->t_id = 0;
	this->t_type = 0;
	this->t_level = 0;
	this->t_require = 0;
	this->t_cost = 0;
	this->t_cd_time = 0;
	this->t_cd_clean_cost = 0;
};

inline void Family::fill( const FamilyBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_type = base.type;
	this->t_level = base.level;
	this->t_require = base.require;
	this->t_cost = base.cost;
	this->t_cd_time = base.cd_time;
	this->t_cd_clean_cost = base.cd_clean_cost;
};

inline const std::string Family::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Family.tbls64";
#else
	return "Family.tbls32";
#endif
};
