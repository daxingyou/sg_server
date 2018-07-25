#ifndef __BUFF_ELEMENT_HPP__
#define __BUFF_ELEMENT_HPP__

#include "common/macros.hpp"
#include "tblh/BuffTable.tbls.h"
#include "combat.hpp"

typedef uint32_t(*elenment_func) (fight_hero_ptr entry, buff_ptr buff, const buff_element_data& element, const int numchange);

enum buff_element_index
{
	buff_element_none						= 0,
	// 改变属性
	buff_element_change_attr				= 1,
	// 触发技能
	buff_element_trigger_skill				= 2,
	// 增加怒气值
	buff_element_add_sp						= 3,
	//buff_element_use_skill = 4,
	// DOTA伤害
	buff_element_damage						= 4,
	// 偷取BUFF
	buff_element_steal_buff					= 5,
	// 治疗
	buff_element_heal						= 6,
	// 拉条
	buff_element_att_speed					= 7,
	// 溢出性治疗
	buff_element_heal_overflow				= 8,
	// 改变属性(不受层数影响)
	buff_element_change_attr_without_num	= 9,
	// 复活效果
	buff_element_relive						= 10,
	// 修改技能CD 
	buff_element_change_cd					= 11,
	// 驱散
	buff_element_cleanup					= 12,
	// 平衡血量
	buff_element_balance_hp					= 13,
	// 交换血量
	buff_element_switch_hp					= 14,
	// 交换速度条
	buff_element_switch_speed				= 15,
	// 平衡速度条
	buff_element_balance_speed				= 16,
	// 夹击效果
	buff_element_union_attack				= 17,
	// buffer转换
	buff_element_switch_buff				= 18,
	// 改变友军属性
	buff_element_change_attr_all_team		= 19,
	// 改变敌军属性
	buff_element_change_attr_all_enmey		= 20,
	// 护盾计算
	buff_element_shield						= 21,
	// 炸弹效果
	buff_element_bomb						= 22,
	// 引爆炸弹
	buff_element_detonate_bomb				= 23,
	// 添加额外BUFF
	buff_element_addbuff_by_debuff			= 24,
	// 增益转DEBUFF
	buff_element_profit_buff_to_debuff		= 25,
	// 移除BUFF层数
	buff_element_remove_buff_layer			= 26,
	// 存在指定BUFF时追加BUFF
	buff_element_addbuff_when_exist			= 27,
	// 添加指定BUFF是追加BUFF
	buff_element_addbuff_when_addbuff		= 28,
	// 层数溢出替换效果
	buff_element_addbuff_when_over_layer	= 29,
	// 移除指定BUFF
	buff_element_remove_buff				= 30,
	// 给目标添加指定BUFF
	buff_element_add_buff_to_target			= 31,
	// 追击
	buff_element_pursue_and_attack			= 32,
	// 嘲讽
	buff_element_taunt						= 33,
	// 忘却
	buff_element_forget						= 34,
	// 特殊加血效果
	buff_element_heal_special				= 35,
	// 添加随机数量BUFF
	buff_element_add_random_buff			= 36,
	// 伤害后添加BUFF
	buff_element_add_buff_after_damage		= 37,
	// 扣除血量
	buff_element_sub_hp						= 38,
	// 复活后死亡
	buff_element_dead_when_relive			= 39,
	// 激活技能
	buff_element_activation_skill			= 40,
	// 根据DEBUFF数量计算概率添加BUFF
	buff_element_add_buff_by_debuf_ratio	= 41,
	// 改变BUFF回合数
	buff_element_change_buff_round			= 42,
	// 沉默
	buff_element_silent						= 43,
	// 变身
	buff_element_change_model				= 44,
	// 不死
	buff_element_athanasia					= 45,
	// 计策炸弹
	buff_element_strategy_bomb				= 46,
	// 根据血量添加BUFF
	buff_element_add_buff_by_hp_ratio		= 47,
	// 根据速度添加BUFF
	buff_element_add_buff_by_speed			= 48,
	// 随机添加BUFF层数
	buff_element_add_random_buff_layer		= 49,
	// 按照BUFF小类型添加元素
	buff_element_add_buff_by_small_type		= 50,
	// 移除指定BUFF
	buff_element_remove_buff_by_small_type	= 51,
	// 添加指定BUFF是追加BUFF
	buff_element_addbuff_when_add_small_type = 52,
	// 免疫
	buff_element_immuno						= 53,
	// 根据血量触发技能
	buff_element_trigger_skill_by_hp		= 54,
	// 致命一击触发技能
	buff_element_trigger_skill_by_critical_blow	= 55,
	// 受攻击次数触发技能
	buff_element_trigger_skill_by_beattack_count= 56,
	// 攻击次数触发技能
	buff_element_trigger_skill_by_attack_count	= 57,
	// 剧情
	buff_element_trigger_story_play				= 58,
	// 根据出手回合数添加BUFF
	buff_element_addbuff_by_round_count			= 59,
	// 增加最大血量
	buff_element_add_max_hp						= 60,
	// 根据血量改变属性
	buff_element_change_attr_by_hp				= 61,
	// 根据目标血量改变属性
	buff_element_change_attr_by_target_hp		= 62,
	// 根据血量加血
	buff_element_addhp_by_hpchange				= 63,
	// 临时改变属性(本段攻击内生效)
	buff_element_change_temp_attr				= 64,
	// 绝地反击(战斗中每次血量小于X%时，所有技能CD-X（或清0？）并立即获得1次行动机会)
	buff_element_extreme_hit_back				= 65,
	// 伤害后添加BUFF
	buff_element_add_buff_behurt				= 66,
	// 额外伤害
	buff_element_skill_ext_damage				= 67,
	// 受伤害时拉条
	buff_element_speed_up_when_behurt			= 68,
	// 减少最大血量
	buff_element_sub_max_hp						= 69,
	// 更新其他人行动次数
	buff_element_other_action_count				= 70,
	// 控制添加速度
	buff_element_in_control_speed				= 71,
	// 移除控制添加速度
	buff_element_out_control_speed				= 72,
	// 承担伤害
	buff_element_bear_hurt						= 73,
	// 七星灯
	buff_element_death_guard					= 74,
	// 七星灯能量
	buff_element_death_guard_energy				= 75,
	// 随机添加BUFF
	buff_element_add_rand_buff					= 76,
	// 伤害累计
	buff_element_all_team_damage				= 77,
	// 随机伤害
	buff_element_rand_damage					= 78,
	// 治疗全队
	buff_element_heal_all_team					= 79,
	// 全队护盾
	buff_element_all_team_shield				= 80,
	// 全队护盾
	buff_element_all_team_damage_turn			= 81,
};


// 计算方式(1-固定数值，2-攻击方攻击百分比，3-攻击方生命百分比，4-攻击方防御百分比，5-攻击方速度百分比，6-自身生命百分比,7-攻击方战斗结果,8-自身当前生命百分比)
enum calc_attr_mode
{
	calc_attr_mode_value			= 1,
	calc_attr_mode_attacker_attack	= 2,
	calc_attr_mode_attacker_hp		= 3,
	calc_attr_mode_attacker_defend	= 4,
	calc_attr_mode_attacker_speed	= 5,
	calc_attr_mode_self_hp			= 6,
	calc_attr_mode_attacker_damage	= 7,
	calc_attr_mode_self_cur_hp		= 8,
	calc_attr_mode_attacker_round_damage	= 9,
};

class buff_element_t
{
public:
	buff_element_t();
	~buff_element_t();

	static bool regist(std::map<uint32_t, elenment_func>& container);

	static uint32_t element_notify_type(uint32_t type);

	static uint32_t element_test(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t change_attr(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t change_temp_attr(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t change_attr_by_hp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t change_attr_by_target_hp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t trigger_skill(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);

	static uint32_t trigger_skill_by_hp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t trigger_skill_by_critical_blow(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t trigger_skill_by_beattack_count(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t trigger_skill_by_attack_count(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);

	static uint32_t use_skill(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t add_sp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t steal_buff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t damage(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t heal(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t heal_special(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t heal_all_team(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t addhp_by_hpchange(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t add_max_hp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t sub_max_hp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t change_att_speed(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);


	static uint32_t change_attr_without_num(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t relive(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t change_cd(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	
	static uint32_t cleanup(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t balance_hp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t balance_speed(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t switch_hp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t switch_speed(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	
	static uint32_t union_attack(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t switch_buff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t change_attr_all_team(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t change_attr_all_enemy(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t shield(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t bomb(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t detonate_bomb(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t addbuff_by_debuff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t addbuff_by_round_count(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t profit_buff_to_debuff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);

	static uint32_t remove_buff_layer(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t heal_overflow(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t addbuff_when_exist(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t add_rand_buff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t addbuff_when_addbuff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t addbuff_when_over_layer(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);

	static uint32_t remove_buff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);

	static uint32_t add_buff_to_target(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);

	static uint32_t pursue_and_attack(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t taunt(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t forget(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t silent(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);

	static uint32_t add_random_buff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t add_buff_after_damage(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t add_buff_behurt(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t sub_hp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t dead_when_relive(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t activation_skill(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t add_buff_by_debuf_ratio(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t change_buff_round(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);

	static uint32_t change_model(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t athanasia(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);

	static uint32_t strategy_bomb(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t add_buff_by_hp_ratio(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t add_buff_by_speed(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);

	static uint32_t add_random_buff_layer(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t add_buff_by_small_type(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t remove_buff_by_small_type(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t addbuff_when_add_small_type(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);

	static uint32_t immuno(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t story_play(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t extreme_hit_back(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t skill_ext_damage(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t speed_up_when_behurt(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t other_action_count(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t in_control_speed(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t out_control_speed(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t bear_hurt(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t death_guard_energy(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t death_guard(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t all_team_damage(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t rand_damage(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t all_team_shield(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
	static uint32_t all_team_damage_turn(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange);
public:
	static void inc_all_team_attr(fight_hero_ptr owner, int type, int value, int chg_type);
	static void dec_all_team_attr(fight_hero_ptr owner, int type, int value, int chg_type);

	static void inc_all_enemy_attr(fight_hero_ptr owner, int type, int value, int chg_type);
	static void dec_all_enemy_attr(fight_hero_ptr owner, int type, int value, int chg_type);

	static void heal_all_team_hp(fight_hero_ptr owner, fight_hero_ptr attacker, buff_ptr buff, int value);

	static uint32_t calc_value(fight_hero_ptr owner, fight_hero_ptr attacker, int type, int value);

	static uint32_t calc_heal_special_value(fight_hero_ptr owner, fight_hero_ptr attacker, int type, int buff_id, int value, int param);

	static fight_hero_ptr get_target_by_type(fight_hero_ptr owner, fight_hero_ptr attacker, SkillEffectTable* skill_conf, buff_ptr buff, uint32_t target_type);
};

#endif//__BUFF_ELEMENT_HPP__