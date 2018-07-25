#pragma once
#include "ScriptProxy.h"
#include "fight/fight_hero.hpp"

class script_fight_hero_t : public ScriptProxy<fight_hero_t> {//tolua_export

public:
	script_fight_hero_t(fight_hero_t* owner);
	// 初始化
	void on_init();

	// 事件
	void on_event(
		const int event, 
		script_fight_hero_t* attacker = NULL, 
		script_fight_hero_t* defender = NULL, 
		const int skill = 0, 
		const int damage = 0, 
		const int phase = 0 );

	// 查找目标
	void find_target(unsigned int skill);
	// 出手前
	void before_attack(unsigned int skill);
	// 出手后
	void after_attack(unsigned int skill);

	// 别的武将攻击前
	void other_before_attack(script_fight_hero_t* attacker, const char *targets, unsigned int skill);
	// 别的武将攻击后
	void other_after_attack(script_fight_hero_t* attacker, const char *targets, unsigned int skill);

	// 攻击一个目标前
	void before_attack_target(script_fight_hero_t* target, unsigned int skill);
	// 攻击一个目标后
	void after_attack_target(script_fight_hero_t* target, unsigned int skill);

	// 别的武将攻击一个目标前
	void other_before_attack_target(script_fight_hero_t* attacker, script_fight_hero_t* target, unsigned int skill);
	// 别的武将攻击一个目标后
	void other_after_attack_target(script_fight_hero_t* attacker, script_fight_hero_t* target, unsigned int skill);

	// 别的武将受击前
	void other_before_be_attack(script_fight_hero_t* attacker, script_fight_hero_t* target, unsigned int skill);
	// 别的武将受击后
	void other_after_be_attack(script_fight_hero_t* attacker, script_fight_hero_t* target, unsigned int skill);
	
	// 对一个目标进行一段伤害前
	void before_attack_phase(script_fight_hero_t* target, unsigned int skill);
	// 对一个目标进行一段伤害后
	void after_attack_phase(script_fight_hero_t* target, unsigned int skill, size_t phase);
	// 回合开始
	void round_start(unsigned int round);
	// 回合结束
	void round_end(unsigned int round);
	// 血量变化
	void hp_change(int change);
	// 被攻击前
	void before_be_attack(script_fight_hero_t* attacker, unsigned int skill);
	// 被攻击后
	void after_be_attack(script_fight_hero_t* attacker, unsigned int skill);
	// 护盾吸收前
	void before_absorb(script_fight_hero_t* attacker, unsigned int skill, int damage);
	// 受伤害前
	bool before_injured(script_fight_hero_t* attacker, unsigned int skill, int damage);
	// 造成伤害前
	bool before_damage(script_fight_hero_t* target, unsigned int skill, int damage);
	// 其他人受伤害前
	bool other_before_injured(script_fight_hero_t* attacker, script_fight_hero_t* target, unsigned int skill, int damage);
	// 死亡时
	void on_dead();
	// 别的武将死亡时
	void on_other_dead(script_fight_hero_t* hero, script_fight_hero_t* attacker);
	// 击杀时
	void on_kill(script_fight_hero_t* target, unsigned int skill);
	// 免CD
	void on_zero_cd(unsigned int skill);
	// 目标触发反击时
	bool target_trigger_counter(unsigned int skill, script_fight_hero_t* target, unsigned int counter_skill);
	// 开始使用技能，如果脚本处理，需要返回true，不再走技能伤害流程
	bool script_use_skill(unsigned int skill);
	// ai选择目标
	bool ai_select_target(unsigned int skill);
	// 触发免疫
	void on_buff_immuno();
public:
	// 下面的是关键字 不能改 析构必须放在这里
	//tolua_begin
	~script_fight_hero_t(void);
	// 获取特性ID
	unsigned int c_get_script_id();
	// 获取tid
	unsigned int c_get_tid();
	// 获取uid
	const char* c_get_uid();
	// 获取阵营
	unsigned int c_get_camp();
	// 获取性别
	unsigned int c_get_sex();
	// 是否死亡
	bool c_is_dead();
	// 获取属性
	int c_get_attr(unsigned int type);
	// 设置属性
	void c_set_attr(unsigned int type, int value);
	// 扣血
	void c_sub_hp(int value, script_fight_hero_t* attaker, unsigned int skill);
	// 加血
	void c_add_hp(int value);
	// 给目标加buff
	void c_add_buff(script_fight_hero_t* target, unsigned int id, unsigned int skill, unsigned int layer = 1);
	// 移除自己的buff
	void c_remove_buff(unsigned int tid, unsigned int attacker_id = 0);
	// 驱散自己的buff
	void c_dispel_buff(unsigned int type, unsigned int num, unsigned int attacker_id = 0);
	// 是否拥有buff
	bool c_has_buff(unsigned int tid);
// -----------------废弃----------------------------
// 	// 手动增加buff触发次数
// 	void c_add_buff_count(unsigned int id);
// 	// 手动减少buff触发次数
// 	void c_sub_buff_count(unsigned int id);	
// 	// 获取buff触发次数
// 	unsigned int c_get_buff_count(unsigned int id);
//--------------------------------------------------

	// 手动减少buff层数
	void c_sub_buff_layer(unsigned int tid, unsigned int attacker_id = 0);
	// 获取buff层数
	unsigned int c_get_buff_layer(unsigned int tid, unsigned int attacker_id = 0);
// 	// 获取buff释放者
// 	const char* c_get_buff_attacker(unsigned int id);
	// 增加一个目标
	void c_add_target(script_fight_hero_t* target);
	// 根据uid获取fight_hero_t
	script_fight_hero_t* c_get_fight_hero(const char* uid);
	// 获取所有敌方uid字符串
	const char* c_get_all_enemy();
	// 获取所有友方uid字符串
	const char* c_get_all_teammate();
	// 造成伤害
	void c_skill_damage(script_fight_hero_t* target, int damage, unsigned int skill);
	// 技能额外伤害
	void c_skill_ext_damage(script_fight_hero_t* target, int damage, unsigned int skill);
	// 获取普通攻击
	unsigned int c_get_default_skill();
	// 获取反击技能
	unsigned int c_get_counter_skill();
	// 释放技能
	void c_use_skill(unsigned int skill, unsigned int type = 1, const char* targets = NULL);
	// 分摊伤害
	void c_share_damage_to(script_fight_hero_t* teammate, script_fight_hero_t* attacker, unsigned int skill, int damage);
	
	// 脚本变量接口
	void c_set_var(const char* var, int value);
	int c_get_var(const char* var);

	// 通过索引获取技能ID
	int c_get_skill_by_index(const int index);
	// 获取技能数量
	int c_get_skill_size();

	// 增加战斗行为
	//void c_add_combat_act(unsigned int type, unsigned int value, const char* target, unsigned int remain_value);
	void c_add_combat_act(unsigned int type, unsigned int value, script_fight_hero_t* target, script_fight_hero_t* attacker, unsigned int remain_value);
	void c_add_combat_skill_act(unsigned int type, unsigned int value, const char* attacker, const char* targets);

	// 设置AI部署
	void c_set_cur_select(const char* target);
	// 获取攻击力最高的目标
	const char* c_get_max_atk_target(unsigned int skill);
	// 获取脆皮目标
	const char* c_get_weak_target(unsigned int skill);
	// 优先脆皮，其次血量最少
	const char* c_get_weak_hp_target(unsigned int skill);

	// 获取技能数据
//	const char* c_get_skill_data(unsigned int skill);
	const char* c_get_fight_calc_result();

private:

};

//tolua_end