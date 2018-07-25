#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:MonsterBase
/// @remark   记录大小:56 X32
///           记录大小:64 X64
///           字段数量:14
///           数据文件:g怪物表.xlsx 明雷基础表
///           排序主键:怪物ID
#pragma pack(1)
struct MonsterBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x03732109; ///< I|IVIVIIIIIIIII
#else
	static const int          FormatCheckCode = 0x0D6D2119; ///< I|IvIvIIIIIIIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 怪物ID 生成唯一键

	uint32_t                  id;                           ///< 怪物ID

	uint32_t                  type;                         ///< 怪物类型
	const char*               name;                         ///< 显示名
	uint32_t                  country;                      ///< 阵营
	const char*               fight_ids;                    ///< 怪物对阵表
	uint32_t                  alert_radius;                 ///< 警戒半径
	uint32_t                  alert_time;                   ///< 警戒时间
	uint32_t                  flag;                         ///< 标记
	uint32_t                  level;                        ///< 怪物等级
	uint32_t                  min_level;                    ///< 等级下限
	uint32_t                  max_level;                    ///< 等级上限
	uint32_t                  team;                         ///< 组队类型
	uint32_t                  arena_score;                  ///< 竞技场积分
	uint32_t                  arena_level;                  ///< 竞技场段位
};
#pragma pack()

struct Monster : public Entry
{
	uint32_t                  id()                          { return t_id; }

	uint32_t                  type()                        { return t_type; }
	const std::string&        name()                        { return t_name; }
	uint32_t                  country()                     { return t_country; }
	const std::string&        fight_ids()                   { return t_fight_ids; }
	uint32_t                  alert_radius()                { return t_alert_radius; }
	uint32_t                  alert_time()                  { return t_alert_time; }
	uint32_t                  flag()                        { return t_flag; }
	uint32_t                  level()                       { return t_level; }
	uint32_t                  min_level()                   { return t_min_level; }
	uint32_t                  max_level()                   { return t_max_level; }
	uint32_t                  team()                        { return t_team; }
	uint32_t                  arena_score()                 { return t_arena_score; }
	uint32_t                  arena_level()                 { return t_arena_level; }

	const std::vector<uint32_t>& get_fight_ids() { return m_fight_ids; }
	uint32_t get_fight_ids(uint32_t pos) { return pos >= m_fight_ids.size() ? 0 : m_fight_ids[pos]; }
	// 在所有fight中随机一个
	uint32_t get_fight_id();

private:
	uint32_t                  t_id;                         ///< 怪物ID

	uint32_t                  t_type;                       ///< 怪物类型
	std::string               t_name;                       ///< 显示名
	uint32_t                  t_country;                    ///< 阵营
	std::string               t_fight_ids;                  ///< 怪物对阵表
	uint32_t                  t_alert_radius;               ///< 警戒半径
	uint32_t                  t_alert_time;                 ///< 警戒时间
	uint32_t                  t_flag;                       ///< 标记
	uint32_t                  t_level;                      ///< 怪物等级
	uint32_t                  t_min_level;                  ///< 等级下限
	uint32_t                  t_max_level;                  ///< 等级上限
	uint32_t                  t_team;                       ///< 组队类型
	uint32_t                  t_arena_score;                ///< 竞技场积分
	uint32_t                  t_arena_level;                ///< 竞技场段位
	std::vector<uint32_t> m_fight_ids;

public:
	void reset();
	void fill( const MonsterBase& base );
	const char* getClassName() const { return "g怪物表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< Monster, MonsterBase > MonsterManager;

inline void Monster::reset()
{
	this->t_id = 0;
	this->t_type = 0;
	this->t_name.clear();
	this->t_country = 0;
	this->t_fight_ids.clear();
	this->t_alert_radius = 0;
	this->t_alert_time = 0;
	this->t_flag = 0;
	this->t_level = 0;
	this->t_min_level = 0;
	this->t_max_level = 0;
	this->t_team = 0;
	this->t_arena_score = 0;
	this->t_arena_level = 0;
	m_fight_ids.clear();
};

inline void Monster::fill( const MonsterBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_type = base.type;
	if( base.name == NULL ) this->t_name.clear(); else this->t_name.assign( base.name );
	this->t_country = base.country;
	if( base.fight_ids == NULL ) this->t_fight_ids.clear(); else this->t_fight_ids.assign( base.fight_ids );
	this->t_alert_radius = base.alert_radius;
	this->t_alert_time = base.alert_time;
	this->t_flag = base.flag;
	this->t_level = base.level;
	this->t_min_level = base.min_level;
	this->t_max_level = base.max_level;
	this->t_team = base.team;
	this->t_arena_score = base.arena_score;
	this->t_arena_level = base.arena_level;
	if (!t_fight_ids.empty())
	{
		common::string_util_t::ParseIds(t_fight_ids, m_fight_ids, '$');
	}
};

inline uint32_t Monster::get_fight_id()
{
	if (!m_fight_ids.empty())
	{
		int32_t index = random_util_t::randBetween(0, m_fight_ids.size() - 1);
        if (index < 0)
        {
            return 0;
        }
		return m_fight_ids.at(index);
	}

	return 0;
};

inline const std::string Monster::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "Monster.tbls64";
#else
	return "Monster.tbls32";
#endif
};
