#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:TimeTableBase
/// @remark   记录大小:16 X32
///           记录大小:20 X64
///           字段数量:4
///           数据文件:s时间表.xlsx Sheet1
///           排序主键:id
#pragma pack(1)
struct TimeTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0510EAD9; ///< I|IVII
#else
	static const int          FormatCheckCode = 0x05110AD9; ///< I|IvII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 id 生成唯一键

	uint32_t                  id;                           ///< id

	uint32_t                  time_type;                    ///< 时间类型
	const char*               start_time;                   ///< 开始时间
	uint32_t                  time_flag;                    ///< 对应类型
	uint32_t                  time_duration;                ///< 持续时间(s)
};
#pragma pack()

struct TimeTable : public Entry
{
	uint32_t                  id() { return t_id; }

	uint32_t                  time_type() { return t_time_type; }
	const std::string&        start_time() { return t_start_time; }
	uint32_t                  time_flag()                   { return t_time_flag; }
	uint32_t                  time_duration() { return t_time_duration; }
	const std::vector<uint32_t>& start_time_vec() const { return t_start_time_vec; }
private:
	uint32_t                  t_id;                         ///< id

	uint32_t                  t_time_type;                  ///< 时间类型
	std::string               t_start_time;                 ///< 开始时间
	uint32_t                  t_time_flag;                  ///< 对应类型
	uint32_t                  t_time_duration;              ///< 持续时间(s)
	std::vector<uint32_t>     t_start_time_vec;
public:
	void reset();
	void fill(const TimeTableBase& base);
	const char* getClassName() const { return "s时间表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< TimeTable, TimeTableBase > TimeTableManager;

inline void TimeTable::reset()
{
	this->t_id = 0;
	this->t_time_type = 0;
	this->t_start_time.clear();
	this->t_time_flag = 0;
	this->t_time_duration = 0;
	this->t_start_time_vec.clear();
};

inline void TimeTable::fill( const TimeTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_time_type = base.time_type;
	if( base.start_time == NULL ) this->t_start_time.clear(); else this->t_start_time.assign( base.start_time );
	this->t_time_flag = base.time_flag;
	this->t_time_duration = base.time_duration;
	common::string_util_t::ParseIds(this->start_time(), this->t_start_time_vec, ':');
};

inline const std::string TimeTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "TimeTable.tbls64";
#else
	return "TimeTable.tbls32";
#endif
};
