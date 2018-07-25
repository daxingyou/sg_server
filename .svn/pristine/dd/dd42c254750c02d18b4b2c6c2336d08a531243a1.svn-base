#pragma once
#include "common/DataManager.h"
#include "common/Entry.h"
#include "common/macros.hpp"

/// @brief    表格结构:SkillAIBase
/// @remark   记录大小:56 X32
///           记录大小:56 X64
///           字段数量:14
///           数据文件:j技能策略.xlsx 技能类型选择
///           排序主键:作用类型编号
#pragma pack(1)

enum skill_ai_type
{
	skill_ai_start_rate = 0,               ///< 起始
	skill_ai_friend_hp_75_rate,            ///< 任意存活友军血量低于75
	skill_ai_friend_hp_50_rate,            ///< 任意存活友军血量低于50
	skill_ai_friend_hp_25_rate,            ///< 任意存活友军血量低于25
	skill_ai_friend_hp_75_add_rate,        ///< 任意存活友军血量低于75(叠加)
	skill_ai_friend_hp_50_add_rate,        ///< 任意存活友军血量低于50(叠加)
	skill_ai_friend_hp_25_add_rate,        ///< 任意存活友军血量低于25(叠加)
	skill_ai_friend_dead_rate,             ///< 友军死亡时
	skill_ai_friend_debuff_rate,           ///< 友军有减益时
	skill_ai_friend_debuff_add_rate,       ///< 友军有减益时(叠加)
	skill_ai_enemy_gain_rate,              ///< 敌军有增益时
	skill_ai_enemy_gain_add_rate,          ///< 敌军有增益时(叠加)
	skill_ai_enemy_all_immuno_rate,        ///< 敌军全体持有免疫

	skill_ai_type_count
};

struct SkillAIBase
{
#ifdef _PLATFORM_X64
	static const int          FormatCheckCode = 0x00005129; ///< I|iiiiiiiiiiiii
#else
	static const int          FormatCheckCode = 0x00005129; ///< I|iiiiiiiiiiiii
#endif

	//------------------------
	uint32_t getUniqueID() const { return id; }; // TODO: 请用 作用类型编号 生成唯一键

	uint32_t                  id;                           ///< 作用类型编号

	int32_t                   start_rate;                   ///< 起始
	int32_t                   friend_hp_75_rate;            ///< 任意存活友军血量低于75
	int32_t                   friend_hp_50_rate;            ///< 任意存活友军血量低于50
	int32_t                   friend_hp_25_rate;            ///< 任意存活友军血量低于25
	int32_t                   friend_hp_75_add_rate;        ///< 任意存活友军血量低于75(叠加)
	int32_t                   friend_hp_50_add_rate;        ///< 任意存活友军血量低于50(叠加)
	int32_t                   friend_hp_25_add_rate;        ///< 任意存活友军血量低于25(叠加)
	int32_t                   friend_dead_rate;             ///< 友军死亡时
	int32_t                   friend_dec_rate;              ///< 友军有减益时
	int32_t                   friend_dec_add_rate;          ///< 友军有减益时(叠加)
	int32_t                   enemy_inc_rate;               ///< 敌军有增益时
	int32_t                   enemy_inc_add_rate;           ///< 敌军有增益时(叠加)
	int32_t                   enemy_all_immuno_rate;        ///< 敌军全体持有免疫
};
#pragma pack()

struct SkillAI : public Entry
{
	uint32_t                  id()                          { return t_id; }

	
	int32_t	get_ai_rate(uint32_t i) { return i >= skill_ai_type_count ? -1 : t_all_skill_ai_type[i]; }

private:
	uint32_t                  t_id;                         ///< 作用类型编号


	std::vector<int32_t>	  t_all_skill_ai_type;			///全部AI类型
public:
	void reset();
	void fill( const SkillAIBase& base );
	const char* getClassName() const { return "j技能策略"; }

	static const std::string getBinaryName();
};

typedef DataTableManager< SkillAI, SkillAIBase > SkillAIManager;

inline void SkillAI::reset()
{
	this->t_id = 0;
};

inline void SkillAI::fill( const SkillAIBase& base )
{
	this->reset();
	this->m_id = base.getUniqueID();

	this->t_id = base.id;
	t_all_skill_ai_type.push_back(base.start_rate);
	t_all_skill_ai_type.push_back(base.friend_hp_75_rate);
	t_all_skill_ai_type.push_back(base.friend_hp_50_rate);
	t_all_skill_ai_type.push_back(base.friend_hp_25_rate);
	t_all_skill_ai_type.push_back(base.friend_hp_75_add_rate);
	t_all_skill_ai_type.push_back(base.friend_hp_50_add_rate);
	t_all_skill_ai_type.push_back(base.friend_hp_25_add_rate);
	t_all_skill_ai_type.push_back(base.friend_dead_rate);
	t_all_skill_ai_type.push_back(base.friend_dec_rate);
	t_all_skill_ai_type.push_back(base.friend_dec_add_rate);
	t_all_skill_ai_type.push_back(base.enemy_inc_rate);
	t_all_skill_ai_type.push_back(base.enemy_inc_add_rate);
	t_all_skill_ai_type.push_back(base.enemy_all_immuno_rate);
};

inline const std::string SkillAI::getBinaryName()
{
#ifdef _PLATFORM_X64
	return "SkillAI.tbls64";
#else
	return "SkillAI.tbls32";
#endif
};
