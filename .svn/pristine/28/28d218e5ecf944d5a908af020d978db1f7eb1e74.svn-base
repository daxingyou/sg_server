#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:OfflineArenaScoreBase
/// @remark   记录大小:8 X32
///           记录大小:8 X64
///           字段数量:2
///           数据文件:l离线竞技场.xlsx 每日积分
///           排序主键:需要积分
#pragma pack(1)
struct OfflineArenaScoreBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#else
	static const int          FormatCheckCode = 0x00005109; ///< I|I
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 需要积分 生成唯一键

	uint32_t                  id;                           ///< 需要积分

	uint32_t                  drop;                         ///< 实际奖励
};
#pragma pack()

struct OfflineArenaScore : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  drop()                        { return t_drop; }

private:
	uint32_t                  t_id;                         ///< 需要积分

	uint32_t                  t_drop;                       ///< 实际奖励

public:
	void reset();
	void fill( const OfflineArenaScoreBase& base );
	const char* getClassName() const { return "l离线竞技场"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< OfflineArenaScore, OfflineArenaScoreBase > OfflineArenaScoreManager;

inline void OfflineArenaScore::reset()
{
	this->t_id = 0;
	this->t_drop = 0;
};

inline void OfflineArenaScore::fill( const OfflineArenaScoreBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_drop = base.drop;
};

inline const std::string OfflineArenaScore::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "OfflineArenaScore.tbls64";
#else
	return "OfflineArenaScore.tbls32";
#endif
};
