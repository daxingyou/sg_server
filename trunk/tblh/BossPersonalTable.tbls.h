#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:BossPersonalTableBase
/// @remark   记录大小:20 X32
///           记录大小:28 X64
///           字段数量:5
///           数据文件:s首领刷新表.xlsx boss个人排名奖励
///           排序主键:id
#pragma pack(1)
struct BossPersonalTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0510DEB6; ///< I|IIVV
#else
	static const int          FormatCheckCode = 0x0510E0D6; ///< I|IIvv
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 id 生成唯一键

	uint32_t                  id;                           ///< id

	uint32_t                  monster_tid;                  ///< boss的id
	uint32_t                  rank;                         ///< 个人伤害排名
	const char*               killed_reward;                ///< killed
	const char*               gone_reward;                  ///< gone
};
#pragma pack()

struct BossPersonalTable : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  monster_tid()                 { return t_monster_tid; }
	uint32_t                  rank()                        { return t_rank; }
	//const std::string&        killed_reward()               { return t_killed_reward; }
	//const std::string&        gone_reward()                 { return t_gone_reward; }
	const std::map<uint32_t, uint32_t>& killed_reward() { return t_killed_reward; }
	const std::map<uint32_t, uint32_t>& gone_reward() { return t_gone_reward; }

private:
	uint32_t                  t_id;                         ///< id

	uint32_t                  t_monster_tid;                ///< boss的id
	uint32_t                  t_rank;                       ///< 个人伤害排名
	//std::string               t_killed_reward;              ///< killed
	//std::string               t_gone_reward;                ///< gone
	std::map<uint32_t, uint32_t> t_killed_reward;
	std::map<uint32_t, uint32_t> t_gone_reward;

public:
	void reset();
	void fill( const BossPersonalTableBase& base );
	const char* getClassName() const { return "s首领刷新表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< BossPersonalTable, BossPersonalTableBase > BossPersonalTableManager;

inline void BossPersonalTable::reset()
{
	this->t_id = 0;
	this->t_monster_tid = 0;
	this->t_rank = 0;
	this->t_killed_reward.clear();
	this->t_gone_reward.clear();
};

extern void addBossPersonalReward(uint32_t, uint32_t);

inline void BossPersonalTable::fill( const BossPersonalTableBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_monster_tid = base.monster_tid;
	this->t_rank = base.rank;
	// if( base.killed_reward == NULL ) this->t_killed_reward.clear(); else this->t_killed_reward.assign( base.killed_reward );
	// if( base.gone_reward == NULL ) this->t_gone_reward.clear(); else this->t_gone_reward.assign( base.gone_reward );

	if (base.killed_reward != NULL) {
		common::Misc::Parse2SeperatorDataInfo(std::string(base.killed_reward), this->t_killed_reward, "$");
	}

	if (base.gone_reward != NULL) {
		common::Misc::Parse2SeperatorDataInfo(std::string(base.gone_reward), this->t_gone_reward, "$");
	}

	addBossPersonalReward(base.monster_tid, base.id);
};

inline const std::string BossPersonalTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "BossPersonalTable.tbls64";
#else
	return "BossPersonalTable.tbls32";
#endif
};
