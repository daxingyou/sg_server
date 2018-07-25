#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    ���ṹ:TimeRankBase
/// @remark   ��¼��С:8 X32
///           ��¼��С:12 X64
///           �ֶ�����:2
///           �����ļ�:y��Ӫ���.xlsx ���˳��
///           ��������:����ʱ��ID
#pragma pack(1)
struct TimeRankBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005116; ///< I|V
#else
	static const int          FormatCheckCode = 0x00005136; ///< I|v
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: ���� ����ʱ��ID ����Ψһ��

	uint32_t                  id;                           ///< ����ʱ��ID

	const char*               rank;                         ///< ���а�ID
};
#pragma pack()

struct TimeRank : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::vector<uint32_t>&        rank()                        { return t_rank; }

private:
	uint32_t                  t_id;                         ///< ����ʱ��ID

	std::vector<uint32_t>               t_rank;                       ///< ���а�ID

public:
	void reset();
	void fill( const TimeRankBase& base );
	const char* getClassName() const { return "y��Ӫ���"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< TimeRank, TimeRankBase > TimeRankManager;

inline void TimeRank::reset()
{
	this->t_id = 0;
	this->t_rank.clear();
};

inline void TimeRank::fill( const TimeRankBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	//if( base.rank == NULL ) this->t_rank.clear(); else this->t_rank.assign( base.rank );
	Dawn::parseContainer<uint32_t>(this->t_rank, base.rank, "$");
};

inline const std::string TimeRank::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "TimeRank.tbls64";
#else
	return "TimeRank.tbls32";
#endif
};
