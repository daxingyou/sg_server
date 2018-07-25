#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"
#include "common/ContainerParse.h"

/// @brief    表格结构:OfflineArenaAwardBase
/// @remark   记录大小:20 X32
///           记录大小:24 X64
///           字段数量:5
///           数据文件:l离线竞技场.xlsx 奖励
///           排序主键:排序
#pragma pack(1)
struct OfflineArenaAwardBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0511AEB6; ///< I|VIVV
#else
	static const int          FormatCheckCode = 0x0513B0D6; ///< I|vIvv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 排序 生成唯一键

	uint32_t                  id;                           ///< 排序

	const char*               rank;                         ///< 排名段
	uint32_t                  sublevel;                     ///< 分段
	const char*               day_award;                    ///< 每日奖励
	const char*               up_award;                     ///< 每前进1名奖励
};
#pragma pack()

struct OfflineArenaAward : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::vector<uint32_t>&        rank()                        { return t_rank; }
	uint32_t                  sublevel()                    { return t_sublevel; }
	const std::map<uint32_t,uint32_t>                  day_award()                   { return t_day_award; }
	const std::map<uint32_t, uint32_t>                  up_award()                    { return t_up_award; }

private:
	uint32_t                  t_id;                         ///< 排序

	std::vector<uint32_t>               t_rank;                       ///< 排名段
	uint32_t                  t_sublevel;                   ///< 分段
	std::map<uint32_t, uint32_t>                  t_day_award;                  ///< 每日奖励
	std::map<uint32_t, uint32_t>                  t_up_award;                   ///< 每前进1名奖励

public:
	void reset();
	void fill( const OfflineArenaAwardBase& base );
	const char* getClassName() const { return "l离线竞技场"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< OfflineArenaAward, OfflineArenaAwardBase > OfflineArenaAwardManager;

inline void OfflineArenaAward::reset()
{
	this->t_id = 0;
	this->t_rank.clear();
	this->t_sublevel = 0;
	this->t_day_award.clear();
	this->t_up_award.clear();
};

inline void OfflineArenaAward::fill( const OfflineArenaAwardBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	//if( base.rank == NULL ) this->t_rank.clear(); else this->t_rank.assign( base.rank );
	Dawn::parseContainer<uint32_t>(this->t_rank, base.rank, "~");
	this->t_sublevel = base.sublevel;
	Dawn::parseMapContainer<uint32_t,uint32_t>(this->t_day_award, base.day_award, ":$");
	Dawn::parseMapContainer<uint32_t, uint32_t>(this->t_up_award, base.up_award, ":$");
};

inline const std::string OfflineArenaAward::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "OfflineArenaAward.tbls64";
#else
	return "OfflineArenaAward.tbls32";
#endif
};
