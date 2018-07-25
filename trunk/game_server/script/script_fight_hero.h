#pragma once
#include "ScriptProxy.h"
#include "fight/fight_hero.hpp"

class script_fight_hero_t : public ScriptProxy<fight_hero_t> {//tolua_export

public:
	script_fight_hero_t(fight_hero_t* owner);
	// ��ʼ��
	void on_init();

	// �¼�
	void on_event(
		const int event, 
		script_fight_hero_t* attacker = NULL, 
		script_fight_hero_t* defender = NULL, 
		const int skill = 0, 
		const int damage = 0, 
		const int phase = 0 );

	// ����Ŀ��
	void find_target(unsigned int skill);
	// ����ǰ
	void before_attack(unsigned int skill);
	// ���ֺ�
	void after_attack(unsigned int skill);

	// ����佫����ǰ
	void other_before_attack(script_fight_hero_t* attacker, const char *targets, unsigned int skill);
	// ����佫������
	void other_after_attack(script_fight_hero_t* attacker, const char *targets, unsigned int skill);

	// ����һ��Ŀ��ǰ
	void before_attack_target(script_fight_hero_t* target, unsigned int skill);
	// ����һ��Ŀ���
	void after_attack_target(script_fight_hero_t* target, unsigned int skill);

	// ����佫����һ��Ŀ��ǰ
	void other_before_attack_target(script_fight_hero_t* attacker, script_fight_hero_t* target, unsigned int skill);
	// ����佫����һ��Ŀ���
	void other_after_attack_target(script_fight_hero_t* attacker, script_fight_hero_t* target, unsigned int skill);

	// ����佫�ܻ�ǰ
	void other_before_be_attack(script_fight_hero_t* attacker, script_fight_hero_t* target, unsigned int skill);
	// ����佫�ܻ���
	void other_after_be_attack(script_fight_hero_t* attacker, script_fight_hero_t* target, unsigned int skill);
	
	// ��һ��Ŀ�����һ���˺�ǰ
	void before_attack_phase(script_fight_hero_t* target, unsigned int skill);
	// ��һ��Ŀ�����һ���˺���
	void after_attack_phase(script_fight_hero_t* target, unsigned int skill, size_t phase);
	// �غϿ�ʼ
	void round_start(unsigned int round);
	// �غϽ���
	void round_end(unsigned int round);
	// Ѫ���仯
	void hp_change(int change);
	// ������ǰ
	void before_be_attack(script_fight_hero_t* attacker, unsigned int skill);
	// ��������
	void after_be_attack(script_fight_hero_t* attacker, unsigned int skill);
	// ��������ǰ
	void before_absorb(script_fight_hero_t* attacker, unsigned int skill, int damage);
	// ���˺�ǰ
	bool before_injured(script_fight_hero_t* attacker, unsigned int skill, int damage);
	// ����˺�ǰ
	bool before_damage(script_fight_hero_t* target, unsigned int skill, int damage);
	// ���������˺�ǰ
	bool other_before_injured(script_fight_hero_t* attacker, script_fight_hero_t* target, unsigned int skill, int damage);
	// ����ʱ
	void on_dead();
	// ����佫����ʱ
	void on_other_dead(script_fight_hero_t* hero, script_fight_hero_t* attacker);
	// ��ɱʱ
	void on_kill(script_fight_hero_t* target, unsigned int skill);
	// ��CD
	void on_zero_cd(unsigned int skill);
	// Ŀ�괥������ʱ
	bool target_trigger_counter(unsigned int skill, script_fight_hero_t* target, unsigned int counter_skill);
	// ��ʼʹ�ü��ܣ�����ű�������Ҫ����true�������߼����˺�����
	bool script_use_skill(unsigned int skill);
	// aiѡ��Ŀ��
	bool ai_select_target(unsigned int skill);
	// ��������
	void on_buff_immuno();
public:
	// ������ǹؼ��� ���ܸ� ���������������
	//tolua_begin
	~script_fight_hero_t(void);
	// ��ȡ����ID
	unsigned int c_get_script_id();
	// ��ȡtid
	unsigned int c_get_tid();
	// ��ȡuid
	const char* c_get_uid();
	// ��ȡ��Ӫ
	unsigned int c_get_camp();
	// ��ȡ�Ա�
	unsigned int c_get_sex();
	// �Ƿ�����
	bool c_is_dead();
	// ��ȡ����
	int c_get_attr(unsigned int type);
	// ��������
	void c_set_attr(unsigned int type, int value);
	// ��Ѫ
	void c_sub_hp(int value, script_fight_hero_t* attaker, unsigned int skill);
	// ��Ѫ
	void c_add_hp(int value);
	// ��Ŀ���buff
	void c_add_buff(script_fight_hero_t* target, unsigned int id, unsigned int skill, unsigned int layer = 1);
	// �Ƴ��Լ���buff
	void c_remove_buff(unsigned int tid, unsigned int attacker_id = 0);
	// ��ɢ�Լ���buff
	void c_dispel_buff(unsigned int type, unsigned int num, unsigned int attacker_id = 0);
	// �Ƿ�ӵ��buff
	bool c_has_buff(unsigned int tid);
// -----------------����----------------------------
// 	// �ֶ�����buff��������
// 	void c_add_buff_count(unsigned int id);
// 	// �ֶ�����buff��������
// 	void c_sub_buff_count(unsigned int id);	
// 	// ��ȡbuff��������
// 	unsigned int c_get_buff_count(unsigned int id);
//--------------------------------------------------

	// �ֶ�����buff����
	void c_sub_buff_layer(unsigned int tid, unsigned int attacker_id = 0);
	// ��ȡbuff����
	unsigned int c_get_buff_layer(unsigned int tid, unsigned int attacker_id = 0);
// 	// ��ȡbuff�ͷ���
// 	const char* c_get_buff_attacker(unsigned int id);
	// ����һ��Ŀ��
	void c_add_target(script_fight_hero_t* target);
	// ����uid��ȡfight_hero_t
	script_fight_hero_t* c_get_fight_hero(const char* uid);
	// ��ȡ���ез�uid�ַ���
	const char* c_get_all_enemy();
	// ��ȡ�����ѷ�uid�ַ���
	const char* c_get_all_teammate();
	// ����˺�
	void c_skill_damage(script_fight_hero_t* target, int damage, unsigned int skill);
	// ���ܶ����˺�
	void c_skill_ext_damage(script_fight_hero_t* target, int damage, unsigned int skill);
	// ��ȡ��ͨ����
	unsigned int c_get_default_skill();
	// ��ȡ��������
	unsigned int c_get_counter_skill();
	// �ͷż���
	void c_use_skill(unsigned int skill, unsigned int type = 1, const char* targets = NULL);
	// ��̯�˺�
	void c_share_damage_to(script_fight_hero_t* teammate, script_fight_hero_t* attacker, unsigned int skill, int damage);
	
	// �ű������ӿ�
	void c_set_var(const char* var, int value);
	int c_get_var(const char* var);

	// ͨ��������ȡ����ID
	int c_get_skill_by_index(const int index);
	// ��ȡ��������
	int c_get_skill_size();

	// ����ս����Ϊ
	//void c_add_combat_act(unsigned int type, unsigned int value, const char* target, unsigned int remain_value);
	void c_add_combat_act(unsigned int type, unsigned int value, script_fight_hero_t* target, script_fight_hero_t* attacker, unsigned int remain_value);
	void c_add_combat_skill_act(unsigned int type, unsigned int value, const char* attacker, const char* targets);

	// ����AI����
	void c_set_cur_select(const char* target);
	// ��ȡ��������ߵ�Ŀ��
	const char* c_get_max_atk_target(unsigned int skill);
	// ��ȡ��ƤĿ��
	const char* c_get_weak_target(unsigned int skill);
	// ���ȴ�Ƥ�����Ѫ������
	const char* c_get_weak_hp_target(unsigned int skill);

	// ��ȡ��������
//	const char* c_get_skill_data(unsigned int skill);
	const char* c_get_fight_calc_result();

private:

};

//tolua_end