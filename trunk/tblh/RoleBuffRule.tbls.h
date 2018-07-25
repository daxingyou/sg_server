#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:RoleBuffRuleBase
/// @remark   记录大小:16 X32
///           记录大小:16 X64
///           字段数量:4
///           数据文件:b大地图增益表.xlsx 增益管理
///           排序主键:增益类型
#pragma pack(1)
struct RoleBuffRuleBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#else
	static const int          FormatCheckCode = 0x00510DD9; ///< I|III
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 增益类型 生成唯一键

	uint32_t                  id;                           ///< 增益类型

	uint32_t                  mutex;                        ///< 互斥
	uint32_t                  replace;                      ///< 替换
	uint32_t                  together;                     ///< 共存
};
#pragma pack()

struct RoleBuffRule : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  mutex()                       { return t_mutex; }
	uint32_t                  replace()                     { return t_replace; }
	uint32_t                  together()                    { return t_together; }

private:
	uint32_t                  t_id;                         ///< 增益类型

	uint32_t                  t_mutex;                      ///< 互斥
	uint32_t                  t_replace;                    ///< 替换
	uint32_t                  t_together;                   ///< 共存

public:
	void reset();
	void fill( const RoleBuffRuleBase& base );
	const char* getClassName() const { return "b大地图增益表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< RoleBuffRule, RoleBuffRuleBase > RoleBuffRuleManager;

inline void RoleBuffRule::reset()
{
	this->t_id = 0;
	this->t_mutex = 0;
	this->t_replace = 0;
	this->t_together = 0;
};

inline void RoleBuffRule::fill( const RoleBuffRuleBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_mutex = base.mutex;
	this->t_replace = base.replace;
	this->t_together = base.together;
};

inline const std::string RoleBuffRule::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "RoleBuffRule.tbls64";
#else
	return "RoleBuffRule.tbls32";
#endif
};
