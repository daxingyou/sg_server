#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:KingWarRankBase
/// @remark   记录大小:16 X32
///           记录大小:20 X64
///           字段数量:4
///           数据文件:l立国战.xlsx 立国战功勋排名奖励
///           排序主键:id
#pragma pack(1)
struct KingWarRankBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00510DE6; ///< I|IIV
#else
	static const int          FormatCheckCode = 0x00510E06; ///< I|IIv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 id 生成唯一键

	uint32_t                  id;                           ///< id

	uint32_t                  level;                        ///< 功勋等级
	uint32_t                  rank;                         ///< 功勋排名
	const char*               reward;                       ///< 奖励
};
#pragma pack()

struct KingWarRank : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  level()                       { return t_level; }
	uint32_t                  rank()                        { return t_rank; }
	const std::map<uint32_t, uint32_t>& reward()			{ return t_reward; }

private:
	uint32_t                  t_id;                         ///< id

	uint32_t                  t_level;                      ///< 功勋等级
	uint32_t                  t_rank;                       ///< 功勋排名
	std::map<uint32_t, uint32_t> t_reward;					///< 奖励

public:
	void reset();
	void fill( const KingWarRankBase& base );
	const char* getClassName() const { return "l立国战"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< KingWarRank, KingWarRankBase > KingWarRankManager;

inline void KingWarRank::reset()
{
	this->t_id = 0;
	this->t_level = 0;
	this->t_rank = 0;
	this->t_reward.clear();
};

inline void KingWarRank::fill( const KingWarRankBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_level = base.level;
	this->t_rank = base.rank;
	
	if (base.reward != NULL) {
		common::Misc::Parse2SeperatorDataInfo(std::string(base.reward), this->t_reward, "$");
	}
};

inline const std::string KingWarRank::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "KingWarRank.tbls64";
#else
	return "KingWarRank.tbls32";
#endif
};
