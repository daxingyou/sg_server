#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    表格结构:ActivitiesBase
/// @remark   记录大小:40 X32
///           记录大小:44 X64
///           字段数量:10
///           数据文件:h活动表.xlsx activities
///           排序主键:活动id
#pragma pack(1)
struct ActivitiesBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x08CE7009; ///< I|IIIIIIVIII
#else
	static const int          FormatCheckCode = 0x08D07009; ///< I|IIIIIIvIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 活动id 生成唯一键

	uint32_t                  id;                           ///< 活动id

	uint32_t                  type;                         ///< 活动类型
	uint32_t                  numbers;                      ///< 活动人数
	uint32_t                  times;                        ///< 参与次数
	uint32_t                  reset_type;                   ///< 次数重置类型
	uint32_t                  level;                        ///< 活动限制等级
	uint32_t                  liveness;                     ///< 活跃值奖励
	const char*               time_id;                      ///< 活动时间
	uint32_t                  page;                         ///< 活动大厅分页
	uint32_t                  quick_cost;                   ///< 一键完成消耗
	uint32_t                  task_type;                    ///< 任务类型
};
#pragma pack()

struct Activities : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  type()                        { return t_type; }
	uint32_t                  numbers()                     { return t_numbers; }
	uint32_t                  times()                       { return t_times; }
	uint32_t                  reset_type()                  { return t_reset_type; }
	uint32_t                  level()                       { return t_level; }
	uint32_t                  liveness()                    { return t_liveness; }
	const std::vector<uint32_t>&        time_id()                     { return t_time_id; }
	uint32_t                  page()                        { return t_page; }
	uint32_t                  quick_cost()                  { return t_quick_cost; }
	uint32_t                  task_type()                   { return t_task_type; }

private:
	uint32_t                  t_id;                         ///< 活动id

	uint32_t                  t_type;                       ///< 活动类型
	uint32_t                  t_numbers;                    ///< 活动人数
	uint32_t                  t_times;                      ///< 参与次数
	uint32_t                  t_reset_type;                 ///< 次数重置类型
	uint32_t                  t_level;                      ///< 活动限制等级
	uint32_t                  t_liveness;                   ///< 活跃值奖励
	std::vector<uint32_t>               t_time_id;                    ///< 活动时间
	uint32_t                  t_page;                       ///< 活动大厅分页
	uint32_t                  t_quick_cost;                 ///< 一键完成消耗
	uint32_t                  t_task_type;                  ///< 任务类型

public:
	void reset();
	void fill( const ActivitiesBase& base );
	const char* getClassName() const { return "h活动表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Activities, ActivitiesBase > ActivitiesManager;

inline void Activities::reset()
{
	this->t_id = 0;
	this->t_type = 0;
	this->t_numbers = 0;
	this->t_times = 0;
	this->t_reset_type = 0;
	this->t_level = 0;
	this->t_liveness = 0;
	this->t_time_id.clear();
	this->t_page = 0;
	this->t_quick_cost = 0;
	this->t_task_type = 0;
};

inline void Activities::fill( const ActivitiesBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_type = base.type;
	this->t_numbers = base.numbers;
	this->t_times = base.times;
	this->t_reset_type = base.reset_type;
	this->t_level = base.level;
	this->t_liveness = base.liveness;
	//if( base.time_id == NULL ) this->t_time_id.clear(); else this->t_time_id.assign( base.time_id );
	Dawn::parseContainer<uint32_t>(this->t_time_id, base.time_id, "$");
	this->t_page = base.page;
	this->t_quick_cost = base.quick_cost;
	this->t_task_type = base.task_type;
};

inline const std::string Activities::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Activities.tbls64";
#else
	return "Activities.tbls32";
#endif
};
