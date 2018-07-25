#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:OfflineRobotInfoBase
/// @remark   记录大小:32 X32
///           记录大小:36 X64
///           字段数量:8
///           数据文件:l离线竞技场.xlsx 机器人
///           排序主键:分段,排名
#pragma pack(1)
struct OfflineRobotInfoBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x01CFB1E9; ///< II|Viiiii
#else
	static const int          FormatCheckCode = 0x03CFB1E9; ///< II|viiiii
#endif

	//------------------------
	uint32_t getUniqueID() const { return hash_util_t::hash_to_uint32(sublevel, rank); }; // TODO: 请用 分段,排名 生成唯一键

	uint32_t                  rank;                         ///< 排名
	uint32_t                  sublevel;                     ///< 分段

	const char*               name;                         ///< 名字
	int32_t                   plugin;                       ///< 模型
	int32_t                   lv;                           ///< 等级
	int32_t                   against_ids;                  ///< 阵容
	int32_t                   fight_value;                  ///< 战斗力
	int32_t                   headpic;                      ///< 头像
};
#pragma pack()

struct OfflineRobotInfo : public Entry
{
	uint32_t                  rank()                        { return t_rank; }
	uint32_t                  sublevel()                    { return t_sublevel; }

	const std::string&        name()                        { return t_name; }
	int32_t                   plugin()                      { return t_plugin; }
	int32_t                   lv()                          { return t_lv; }
	int32_t                   against_ids()                 { return t_against_ids; }
	int32_t                   fight_value()                 { return t_fight_value; }
	int32_t                   headpic()                     { return t_headpic; }

private:
	uint32_t                  t_rank;                       ///< 排名
	uint32_t                  t_sublevel;                   ///< 分段

	std::string               t_name;                       ///< 名字
	int32_t                   t_plugin;                     ///< 模型
	int32_t                   t_lv;                         ///< 等级
	int32_t                   t_against_ids;                ///< 阵容
	int32_t                   t_fight_value;                ///< 战斗力
	int32_t                   t_headpic;                    ///< 头像

public:
	void reset();
	void fill( const OfflineRobotInfoBase& base );
	const char* getClassName() const { return "l离线竞技场"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< OfflineRobotInfo, OfflineRobotInfoBase > OfflineRobotInfoManager;

inline void OfflineRobotInfo::reset()
{
	this->t_rank = 0;
	this->t_sublevel = 0;
	this->t_name.clear();
	this->t_plugin = 0;
	this->t_lv = 0;
	this->t_against_ids = 0;
	this->t_fight_value = 0;
	this->t_headpic = 0;
};

inline void OfflineRobotInfo::fill( const OfflineRobotInfoBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_rank = base.rank;
	this->t_sublevel = base.sublevel;
	if( base.name == NULL ) this->t_name.clear(); else this->t_name.assign( base.name );
	this->t_plugin = base.plugin;
	this->t_lv = base.lv;
	this->t_against_ids = base.against_ids;
	this->t_fight_value = base.fight_value;
	this->t_headpic = base.headpic;
};

inline const std::string OfflineRobotInfo::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "OfflineRobotInfo.tbls64";
#else
	return "OfflineRobotInfo.tbls32";
#endif
};
