#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:TaskEventBase
/// @remark   记录大小:24 X32
///           记录大小:28 X64
///           字段数量:6
///           数据文件:r任务表.xlsx 特殊事件
///           排序主键:事件ID
#pragma pack(1)
struct TaskEventBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x010DEA89; ///< I|IIVII
#else
	static const int          FormatCheckCode = 0x010E0A89; ///< I|IIvII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 事件ID 生成唯一键

	uint32_t                  id;                           ///< 事件ID

	uint32_t                  buff_id;                      ///< 增益ID
	uint32_t                  hero_id;                      ///< 武将ID
	const char*               money;                        ///< 购买价格
	uint32_t                  time;                         ///< 时间限制
	uint32_t                  rate;                         ///< 出现概率
};
#pragma pack()

struct TaskEvent : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  buff_id()                     { return t_buff_id; }
	uint32_t                  hero_id()                     { return t_hero_id; }
	const std::pair<uint32_t,uint32_t>&        money()                       { return t_money; }
	uint32_t                  time()                        { return t_time; }
	uint32_t                  rate()                        { return t_rate; }

private:
	uint32_t                  t_id;                         ///< 事件ID

	uint32_t                  t_buff_id;                    ///< 增益ID
	uint32_t                  t_hero_id;                    ///< 武将ID
	std::pair<uint32_t, uint32_t>               t_money;                      ///< 购买价格
	uint32_t                  t_time;                       ///< 时间限制
	uint32_t                  t_rate;                       ///< 出现概率

public:
	void reset();
	void fill( const TaskEventBase& base );
	const char* getClassName() const { return "r任务表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< TaskEvent, TaskEventBase > TaskEventManager;

inline void TaskEvent::reset()
{
	this->t_id = 0;
	this->t_buff_id = 0;
	this->t_hero_id = 0;
	//this->t_money.clear();
	this->t_time = 0;
	this->t_rate = 0;
};

inline void TaskEvent::fill( const TaskEventBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_buff_id = base.buff_id;
	this->t_hero_id = base.hero_id;
	//if( base.money == NULL ) this->t_money.clear(); else this->t_money.assign( base.money );
	Dawn::parseTuple(this->t_money, base.money, ":");
	this->t_time = base.time;
	this->t_rate = base.rate;
};

inline const std::string TaskEvent::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "TaskEvent.tbls64";
#else
	return "TaskEvent.tbls32";
#endif
};
