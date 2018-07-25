#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:OfflineArenaClassBase
/// @remark   记录大小:28 X32
///           记录大小:32 X64
///           字段数量:7
///           数据文件:l离线竞技场.xlsx 分段
///           排序主键:分段
#pragma pack(1)
struct OfflineArenaClassBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00DDEFC9; ///< I|IIIVII
#else
	static const int          FormatCheckCode = 0x00DE0FC9; ///< I|IIIvII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 分段 生成唯一键

	uint32_t                  id;                           ///< 分段

	uint32_t                  level;                        ///< 进入等级
	uint32_t                  class_up_award;               ///< 进阶奖励
	uint32_t                  min_rank;                     ///< 最低排名
	const char*               name;                         ///< 奖励邮件中名称
	uint32_t                  win_award;                    ///< 战斗胜利奖励
	uint32_t                  lose_award;                   ///< 战斗失败奖励
};
#pragma pack()

struct OfflineArenaClass : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  level()                       { return t_level; }
	uint32_t                  class_up_award()              { return t_class_up_award; }
	uint32_t                  min_rank()                    { return t_min_rank; }
	const std::string&        name()                        { return t_name; }
	uint32_t                  win_award()                   { return t_win_award; }
	uint32_t                  lose_award()                  { return t_lose_award; }

private:
	uint32_t                  t_id;                         ///< 分段

	uint32_t                  t_level;                      ///< 进入等级
	uint32_t                  t_class_up_award;             ///< 进阶奖励
	uint32_t                  t_min_rank;                   ///< 最低排名
	std::string               t_name;                       ///< 奖励邮件中名称
	uint32_t                  t_win_award;                  ///< 战斗胜利奖励
	uint32_t                  t_lose_award;                 ///< 战斗失败奖励

public:
	void reset();
	void fill( const OfflineArenaClassBase& base );
	const char* getClassName() const { return "l离线竞技场"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< OfflineArenaClass, OfflineArenaClassBase > OfflineArenaClassManager;

inline void OfflineArenaClass::reset()
{
	this->t_id = 0;
	this->t_level = 0;
	this->t_class_up_award = 0;
	this->t_min_rank = 0;
	this->t_name.clear();
	this->t_win_award = 0;
	this->t_lose_award = 0;
};

inline void OfflineArenaClass::fill( const OfflineArenaClassBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_level = base.level;
	this->t_class_up_award = base.class_up_award;
	this->t_min_rank = base.min_rank;
	if( base.name == NULL ) this->t_name.clear(); else this->t_name.assign( base.name );
	this->t_win_award = base.win_award;
	this->t_lose_award = base.lose_award;
};

inline const std::string OfflineArenaClass::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "OfflineArenaClass.tbls64";
#else
	return "OfflineArenaClass.tbls32";
#endif
};
