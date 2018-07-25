#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:FightAgainstBase
/// @remark   记录大小:64 X32
///           记录大小:80 X64
///           字段数量:16
///           数据文件:g怪物表.xlsx 对阵表
///           排序主键:对阵ID
#pragma pack(1)
struct FightAgainstBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x07512CB9; ///< I|VVVIIIVIIIIIIII
#else
	static const int          FormatCheckCode = 0x0750EAD9; ///< I|vvvIIIvIIIIIIII
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 对阵ID 生成唯一键

	uint32_t                  id;                           ///< 对阵ID

	const char*               against_ids;                  ///< 对应ID
	const char*               against_ids_2;                ///< 对应ID二阶段
	const char*               against_ids_3;                ///< 对应ID三阶段
	uint32_t                  is_story;                     ///< 剧情使用
	uint32_t                  main_hero_tid;                ///< 主公武将ID
	uint32_t                  can_form;                     ///< 是否允许布阵
	const char*               friend_against_ids;           ///< 友方布阵
	uint32_t                  combat_position_id;           ///< 战斗站位ID
	uint32_t                  troop_combat_position_id;     ///< 组队战斗站位ID
	uint32_t                  enemy_combat_position_id;     ///< 战斗敌方站位ID
	uint32_t                  hero_exp;                     ///< 经验
	uint32_t                  role_exp;                     ///< 修为经验
	uint32_t                  drop;                         ///< 掉落编号
	uint32_t                  prepare_time;                 ///< 部署时间
	uint32_t                  max_rounds;                   ///< 战斗结束回合数
};
#pragma pack()

struct FightAgainst : public Entry
{
	uint32_t                  id()                          { return t_id; }

	const std::string&        against_ids()				  { return t_against_ids; }
	const std::string&        against_ids_2()               { return t_against_ids_2; }
	const std::string&        against_ids_3()               { return t_against_ids_3; }
	uint32_t                  is_story() { return t_is_story; }
	uint32_t                  main_hero_tid()               { return t_main_hero_tid; }
	uint32_t                  can_form()                    { return t_can_form; }
	const std::string&        friend_against_ids() { return t_friend_against_ids; }
	uint32_t                  combat_position_id()          { return t_combat_position_id; }
	uint32_t                  troop_combat_position_id()    { return t_troop_combat_position_id; }
	uint32_t                  enemy_combat_position_id()    { return t_enemy_combat_position_id; }
	uint32_t                  hero_exp()					{ return t_hero_exp; }
	uint32_t                  role_exp()					{ return t_role_exp; }
	uint32_t                  drop()						{ return t_drop; }
	uint32_t                  prepare_time()                { return t_prepare_time; }
	uint32_t                  max_rounds()                  { return t_max_rounds; }
		
	
	const std::map<uint32_t, uint32_t>& get_against_ids() { return m_against_ids; }
	const std::map<uint32_t, uint32_t>& get_friend_against_ids() { return m_friend_against_ids; }

private:
	uint32_t                  t_id;                         ///< 对阵ID

	std::string               t_against_ids;                ///< 对应ID
	std::string               t_against_ids_2;              ///< 对应ID二阶段
	std::string               t_against_ids_3;              ///< 对应ID三阶段
	uint32_t                  t_is_story;                   ///< 剧情使用
	uint32_t                  t_main_hero_tid;              ///< 主公武将ID
	uint32_t                  t_can_form;                   ///< 是否允许布阵
	std::string               t_friend_against_ids;         ///< 友方布阵
	uint32_t                  t_combat_position_id;         ///< 战斗站位ID
	uint32_t                  t_troop_combat_position_id;   ///< 组队战斗站位ID
	uint32_t                  t_enemy_combat_position_id;   ///< 战斗敌方站位ID	
	uint32_t                  t_hero_exp;                   ///< 经验
	uint32_t                  t_role_exp;                   ///< 修为经验
	uint32_t                  t_drop;                       ///< 掉落编号
	uint32_t                  t_prepare_time;               ///< 部署时间
	uint32_t                  t_max_rounds;                 ///< 战斗结束回合数
		
	
	std::map<uint32_t, uint32_t> m_against_ids;
	std::map<uint32_t, uint32_t> m_friend_against_ids;

public:
	void reset();
	void fill( const FightAgainstBase& base );
	const char* getClassName() const { return "g怪物表"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< FightAgainst, FightAgainstBase > FightAgainstManager;

inline void FightAgainst::reset()
{
	this->t_id = 0;
	this->t_against_ids.clear();
	this->t_against_ids_2.clear();
	this->t_against_ids_3.clear();
	this->t_is_story = 0;
	this->t_main_hero_tid = 0;
	this->t_can_form = 0;
	this->t_friend_against_ids.clear();
	this->t_combat_position_id = 0;
	this->t_troop_combat_position_id = 0;
	this->t_enemy_combat_position_id = 0;
	this->t_hero_exp = 0;
	this->t_role_exp = 0;
	this->t_drop = 0;
	this->t_prepare_time = 0;
	this->t_max_rounds = 0;
	
	m_against_ids.clear();
	m_friend_against_ids.clear();
};

inline void FightAgainst::fill( const FightAgainstBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	if (base.against_ids == NULL) this->t_against_ids.clear(); else this->t_against_ids.assign(base.against_ids);
	if (base.against_ids_2 == NULL) this->t_against_ids_2.clear(); else this->t_against_ids_2.assign(base.against_ids_2);
	if (base.against_ids_3 == NULL) this->t_against_ids_3.clear(); else this->t_against_ids_3.assign(base.against_ids_3);
	this->t_is_story = base.is_story;
	this->t_main_hero_tid = base.main_hero_tid;
	this->t_can_form = base.can_form;
	if (base.friend_against_ids == NULL) this->t_friend_against_ids.clear(); else this->t_friend_against_ids.assign(base.friend_against_ids);
	this->t_combat_position_id = base.combat_position_id;
	this->t_troop_combat_position_id = base.troop_combat_position_id;
	this->t_enemy_combat_position_id = base.enemy_combat_position_id;
	this->t_hero_exp = base.hero_exp;
	this->t_role_exp = base.role_exp;
	this->t_drop = base.drop;
	this->t_prepare_time = base.prepare_time;
	this->t_max_rounds = base.max_rounds;
		
	if (!t_against_ids.empty())
	{
		std::vector<std::string> vec;
		common::string_util_t::split<std::string>(t_against_ids, vec, "$");
		common::string_util_t::Parse_id_cnt_str_to_map(vec, m_against_ids);
	}
	if (!t_against_ids_2.empty())
	{
		std::vector<std::string> vec;
		common::string_util_t::split<std::string>(t_against_ids_2, vec, "$");
		common::string_util_t::Parse_id_cnt_str_to_map(vec, m_against_ids);
	}
	if (!t_against_ids_3.empty())
	{
		std::vector<std::string> vec;
		common::string_util_t::split<std::string>(t_against_ids_3, vec, "$");
		common::string_util_t::Parse_id_cnt_str_to_map(vec, m_against_ids);
	}

	if (!t_friend_against_ids.empty())
	{
		std::vector<std::string> vec;
		common::string_util_t::split<std::string>(t_friend_against_ids, vec, "$");
		common::string_util_t::Parse_id_cnt_str_to_map(vec, m_friend_against_ids);
}
};

inline const std::string FightAgainst::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "FightAgainst.tbls64";
#else
	return "FightAgainst.tbls32";
#endif
};
