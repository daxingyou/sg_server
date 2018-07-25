#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:FightTypeTableBase
/// @remark   记录大小:40 X32
///           记录大小:40 X64
///           字段数量:10
///           数据文件:d对战类型配置.xlsx 对战类型配置
///           排序主键:对战类型编号
#pragma pack(1)
struct FightTypeTableBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x0D8CD009; ///< I|IIIIIIIII
#else
	static const int          FormatCheckCode = 0x0D8CD009; ///< I|IIIIIIIII
#endif

															//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 对战类型编号 生成唯一键

	uint32_t                  id;                           ///< 对战类型编号

	uint32_t                  ignore_lineup;                ///< 跳过布阵
	uint32_t                  max_visit;                    ///< 观战人数上限
	uint32_t                  team_2_exp_addition;          ///< 2人组队加成经验万分比
	uint32_t                  team_3_exp_addition;          ///< 3人组队加成经验万分比
	uint32_t                  team_2_coin_addition;         ///< 2人组队加成货币万分比
	uint32_t                  team_3_coin_addition;         ///< 3人组队加成货币万分比
	uint32_t                  auto_fight_inherit;           ///< 自动战斗继承
	uint32_t                  fight_with_team;              ///< 是否允许组队战斗
	uint32_t                  is_replay;                    ///< 保存回放
};
#pragma pack()

struct FightTypeTable : public Entry
{
	uint32_t                  id() { return t_id; }

	uint32_t                  ignore_lineup() { return t_ignore_lineup; }
	uint32_t                  max_visit() { return t_max_visit; }
	uint32_t                  team_2_exp_addition() { return t_team_2_exp_addition; }
	uint32_t                  team_3_exp_addition() { return t_team_3_exp_addition; }
	uint32_t                  team_2_coin_addition() { return t_team_2_coin_addition; }
	uint32_t                  team_3_coin_addition() { return t_team_3_coin_addition; }
	uint32_t                  auto_fight_inherit() { return t_auto_fight_inherit; }
	uint32_t                  fight_with_team()             { return t_fight_with_team; }
	uint32_t                  is_replay()                   { return t_is_replay; }

private:
	uint32_t                  t_id;                         ///< 对战类型编号

	uint32_t                  t_ignore_lineup;              ///< 跳过布阵
	uint32_t                  t_max_visit;                  ///< 观战人数上限
	uint32_t                  t_team_2_exp_addition;        ///< 2人组队加成经验万分比
	uint32_t                  t_team_3_exp_addition;        ///< 3人组队加成经验万分比
	uint32_t                  t_team_2_coin_addition;       ///< 2人组队加成货币万分比
	uint32_t                  t_team_3_coin_addition;       ///< 3人组队加成货币万分比
	uint32_t                  t_auto_fight_inherit;         ///< 自动战斗继承
	uint32_t                  t_fight_with_team;            ///< 是否允许组队战斗
	uint32_t                  t_is_replay;                  ///< 保存回放

public:
	void reset();
	void fill(const FightTypeTableBase& base);
	const char* getClassName() const { return "d对战类型配置"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< FightTypeTable, FightTypeTableBase > FightTypeTableManager;

inline void FightTypeTable::reset()
{
	this->t_id = 0;
	this->t_ignore_lineup = 0;
	this->t_max_visit = 0;
	this->t_team_2_exp_addition = 0;
	this->t_team_3_exp_addition = 0;
	this->t_team_2_coin_addition = 0;
	this->t_team_3_coin_addition = 0;
	this->t_auto_fight_inherit = 0;
	this->t_fight_with_team = 0;
	this->t_is_replay = 0;
};

inline void FightTypeTable::fill(const FightTypeTableBase& base)
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	this->t_ignore_lineup = base.ignore_lineup;
	this->t_max_visit = base.max_visit;
	this->t_team_2_exp_addition = base.team_2_exp_addition;
	this->t_team_3_exp_addition = base.team_3_exp_addition;
	this->t_team_2_coin_addition = base.team_2_coin_addition;
	this->t_team_3_coin_addition = base.team_3_coin_addition;
	this->t_auto_fight_inherit = base.auto_fight_inherit;
	this->t_fight_with_team = base.fight_with_team;
	this->t_is_replay = base.is_replay;
};

inline const std::string FightTypeTable::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "FightTypeTable.tbls64";
#else
	return "FightTypeTable.tbls32";
#endif
};
