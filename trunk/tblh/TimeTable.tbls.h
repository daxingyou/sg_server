#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    ���ṹ:TimeTableBase
/// @remark   ��¼��С:16 X32
///           ��¼��С:20 X64
///           �ֶ�����:4
///           �����ļ�:sʱ���.xlsx Sheet1
///           ��������:id
#pragma pack(1)
struct TimeTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0510EAD9; ///< I|IVII
#else
	static const int          FormatCheckCode = 0x05110AD9; ///< I|IvII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� id ����Ψһ��

	uint32_t                  id;                           ///< id

	uint32_t                  time_type;                    ///< ʱ������
	const char*               start_time;                   ///< ��ʼʱ��
	uint32_t                  time_flag;                    ///< ��Ӧ����
	uint32_t                  time_duration;                ///< ����ʱ��(s)
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

	uint32_t                  t_time_type;                  ///< ʱ������
	std::string               t_start_time;                 ///< ��ʼʱ��
	uint32_t                  t_time_flag;                  ///< ��Ӧ����
	uint32_t                  t_time_duration;              ///< ����ʱ��(s)
	std::vector<uint32_t>     t_start_time_vec;
public:
	void reset();
	void fill(const TimeTableBase& base);
	const char* getClassName() const { return "sʱ���"; }

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
