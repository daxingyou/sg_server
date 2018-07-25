#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/utility.hpp"
USING_NS_COMMON;

/// @brief    表格结构:TaskCircleBase
/// @remark   记录大小:24 X32
///           记录大小:32 X64
///           字段数量:6
///           数据文件:r任务环表.xlsx Sheet1
///           排序主键:环任务ID
#pragma pack(1)
struct TaskCircleBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0EBABDA9; ///< I|IVVIVfI
#else
	static const int          FormatCheckCode = 0x00DB5DB9; ///< I|IvvIvfI
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 环任务ID 生成唯一键

	uint32_t                  id;                           ///< 环任务ID

	uint32_t                  type;                         ///< 任务类型
	const char*               level_range;                  ///< 等级筛选
	const char*               circle_range;                 ///< 环数区间
	uint32_t                  task_id_style;                ///< 任务ID方式
	const char*               task_id_range;                ///< 任务ID区间
	float                     exp_coeff;                    ///< 环数经验奖励系数
	uint32_t                  award;                        ///< 分段奖励
};
#pragma pack()

struct TaskCircle : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  type()                        { return t_type; }
	const std::vector<uint32_t>&        level_range() { return t_level_range; }
	const std::vector<uint32_t>&        circle_range()                { return t_circle_range; }
	uint32_t                  task_id_style()               { return t_task_id_style; }
	const std::vector<uint32_t>&        task_id_range()               { return t_task_id_range; }
	float                     exp_coeff()                   { return t_exp_coeff; }
	uint32_t                  award() { return t_award; }

private:
	uint32_t                  t_id;                         ///< 环任务ID

	uint32_t                  t_type;                       ///< 任务类型
	std::vector<uint32_t>     t_level_range;				///< 等级筛选
	std::vector<uint32_t>     t_circle_range;               ///< 环数区间
	uint32_t                  t_task_id_style;              ///< 任务ID方式
	std::vector<uint32_t>     t_task_id_range;              ///< 任务ID区间
	float                     t_exp_coeff;                  ///< 环数经验奖励系数
	uint32_t                  t_award;                      ///< 分段奖励

public:
	void reset();
	void fill( const TaskCircleBase& base );
	const char* getClassName() const { return "r任务环表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< TaskCircle, TaskCircleBase > TaskCircleManager;

inline void TaskCircle::reset()
{
	this->t_id = 0;
	this->t_type = 0;
	this->t_level_range.clear();
	this->t_circle_range.clear();
	this->t_task_id_style = 0;
	this->t_task_id_range.clear();
	this->t_exp_coeff = 0;
	this->t_award = 0;
};

inline void TaskCircle::fill( const TaskCircleBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_type = base.type;
	//if (base.level_range == NULL) this->t_level_range.clear(); else this->t_level_range.assign(base.level_range);
	if (base.level_range != NULL)
	{
		string_util_t::split<uint32_t>(base.level_range, t_level_range, "~");
	}
	//if( base.circle_range == NULL ) this->t_circle_range.clear(); else this->t_circle_range.assign( base.circle_range );
	if (base.circle_range != NULL)
	{
		string_util_t::split<uint32_t>(base.circle_range, t_circle_range, "$");
	}
	this->t_task_id_style = base.task_id_style;
	//if( base.task_id_range == NULL ) this->t_task_id_range.clear(); else this->t_task_id_range.assign( base.task_id_range );
	if (base.task_id_range != NULL)
	{
		string_util_t::split<uint32_t>(base.task_id_range, t_task_id_range, "$");
	}
	this->t_exp_coeff = base.exp_coeff;
	this->t_award = base.award;
};

inline const std::string TaskCircle::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "TaskCircle.tbls64";
#else
	return "TaskCircle.tbls32";
#endif
};
