#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:TaskShilianBase
/// @remark   记录大小:12 X32
///           记录大小:12 X64
///           字段数量:3
///           数据文件:r任务表.xlsx 试炼任务
///           排序主键:试炼任务星级
#pragma pack(1)
struct TaskShilianBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x000510D9; ///< I|II
#else
	static const int          FormatCheckCode = 0x000510D9; ///< I|II
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 试炼任务星级 生成唯一键

	uint32_t                  id;                           ///< 试炼任务星级

	uint32_t                  rate;                         ///< 出现概率
	uint32_t                  coeff;                        ///< 奖励倍率
};
#pragma pack()

struct TaskShilian : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  rate()                        { return t_rate; }
	uint32_t                  coeff()                       { return t_coeff; }

private:
	uint32_t                  t_id;                         ///< 试炼任务星级

	uint32_t                  t_rate;                       ///< 出现概率
	uint32_t                  t_coeff;                      ///< 奖励倍率

public:
	void reset();
	void fill( const TaskShilianBase& base );
	const char* getClassName() const { return "r任务表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< TaskShilian, TaskShilianBase > TaskShilianManager;

inline void TaskShilian::reset()
{
	this->t_id = 0;
	this->t_rate = 0;
	this->t_coeff = 0;
};

inline void TaskShilian::fill( const TaskShilianBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_rate = base.rate;
	this->t_coeff = base.coeff;
};

inline const std::string TaskShilian::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "TaskShilian.tbls64";
#else
	return "TaskShilian.tbls32";
#endif
};
