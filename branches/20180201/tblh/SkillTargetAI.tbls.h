#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:SkillTargetAIBase
/// @remark   记录大小:64 X32
///           记录大小:64 X64
///           字段数量:16
///           数据文件:j技能策略.xlsx 技能目标选择
///           排序主键:作用类型编号
#pragma pack(1)

enum target_ai_type
{
	target_ai_all_att_target_rate = 0,      ///< 集火目标单位
	target_ai_enemy_hp_20_rate,             ///< 血量低于20敌军
	target_ai_enemy_def_break_rate,         ///< 附带破甲效果敌军
	target_ai_enemy_restrain_rate,          ///< 属性克敌军
	target_ai_enemy_no_restrain_rate,       ///< 属性无关敌军
	target_ai_enemy_be_restrain_rate,       ///< 属性被克敌军
	target_ai_enemy_shield_rate,            ///< 附带护盾敌军
	target_ai_enemy_invincibility_rate,     ///< 附带无敌敌军
	target_ai_enemy_big_type1_rate,         ///< 附带大类型1的敌军
	target_ai_enemy_big_type2_rate,         ///< 附带大类型2的敌军
	target_ai_enemy_big_type3_rate,         ///< 附带大类型3的友军
	target_ai_enemy_big_type4_rate,         ///< 附带大类型4的友军
	target_ai_friend_min_hp_rate,           ///< 血量百分比最低的友军
	target_ai_friend_dead_rate,             ///< 友军死亡单位
	target_ai_friend_min_att_speed_rate,    ///< 行动条最低的友军

	target_ai_count
};

struct SkillTargetAIBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00512FF9; ///< I|iiiiiiiiiiiiiii
#else
	static const int          FormatCheckCode = 0x00512FF9; ///< I|iiiiiiiiiiiiiii
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 作用类型编号 生成唯一键

	uint32_t                  id;                           ///< 作用类型编号

	int32_t                   all_att_target_rate;          ///< 集火目标单位
	int32_t                   enemy_hp_20_rate;             ///< 血量低于20敌军
	int32_t                   enemy_pojia_rate;             ///< 附带破甲效果敌军
	int32_t                   enemy_restrain_rate;          ///< 属性克敌军
	int32_t                   enemy_no_restrain_rate;       ///< 属性无关敌军
	int32_t                   enemy_be_restrain_rate;       ///< 属性被克敌军
	int32_t                   enemy_shield_rate;            ///< 附带护盾敌军
	int32_t                   enemy_invincibility_rate;     ///< 附带无敌敌军
	int32_t                   enemy_big_type1_rate;         ///< 附带大类型1的敌军
	int32_t                   enemy_big_type2_rate;         ///< 附带大类型2的敌军
	int32_t                   enemy_big_type3_rate;         ///< 附带大类型3的友军
	int32_t                   enemy_big_type4_rate;         ///< 附带大类型4的友军
	int32_t                   friend_min_hp_rate;           ///< 血量百分比最低的友军
	int32_t                   friend_dead_rate;             ///< 友军死亡单位
	int32_t                   friend_min_att_speed_rate;    ///< 行动条最低的友军
};
#pragma pack()

struct SkillTargetAI : public Entry
{
	uint32_t                  id()                          { return t_id; }
	

	int32_t	get_target_ai_rate(uint32_t i) { return i >= target_ai_count ? -1 : t_all_target_ai_type[i]; }
private:
	uint32_t                  t_id;                         ///< 作用类型编号
	
	std::vector<int32_t>	  t_all_target_ai_type;			///全部AI类型
public:
	void reset();
	void fill( const SkillTargetAIBase& base );
	const char* getClassName() const { return "j技能策略"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< SkillTargetAI, SkillTargetAIBase > SkillTargetAIManager;

inline void SkillTargetAI::reset()
{
	this->t_id = 0;
};

inline void SkillTargetAI::fill( const SkillTargetAIBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	t_all_target_ai_type.push_back(base.all_att_target_rate);
	t_all_target_ai_type.push_back(base.enemy_hp_20_rate);
	t_all_target_ai_type.push_back(base.enemy_pojia_rate);
	t_all_target_ai_type.push_back(base.enemy_restrain_rate);
	t_all_target_ai_type.push_back(base.enemy_no_restrain_rate);
	t_all_target_ai_type.push_back(base.enemy_be_restrain_rate);
	t_all_target_ai_type.push_back(base.enemy_shield_rate);
	t_all_target_ai_type.push_back(base.enemy_invincibility_rate);
	t_all_target_ai_type.push_back(base.enemy_big_type1_rate);
	t_all_target_ai_type.push_back(base.enemy_big_type2_rate);
	t_all_target_ai_type.push_back(base.enemy_big_type3_rate);
	t_all_target_ai_type.push_back(base.enemy_big_type4_rate);
	t_all_target_ai_type.push_back(base.friend_min_hp_rate);
	t_all_target_ai_type.push_back(base.friend_dead_rate);
	t_all_target_ai_type.push_back(base.friend_min_att_speed_rate);
};

inline const std::string SkillTargetAI::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "SkillTargetAI.tbls64";
#else
	return "SkillTargetAI.tbls32";
#endif
};
