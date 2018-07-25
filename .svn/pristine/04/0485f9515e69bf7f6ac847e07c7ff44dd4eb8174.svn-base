//技能管理器
#ifndef SKILL_MANAGER_H
#define SKILL_MANAGER_H

#include <vector>
#include "combat.hpp"
#include "skill_data.hpp"

class skill_manager_t {
public:
	void init(fight_hero_ptr p_hero) { m_owner = p_hero; }

	void add_skill( uint32_t  turn_level, skill_data_t &data, uint32_t pos);

	fight_hero_ptr get_self_ptr() { return m_owner.lock();  }
public:
	//获取当前技能列表
	std::vector<skill_info_t> &get_cur_skill_list();

	skill_info_t *get_skill_info_by_pos(uint32_t pos);

	skill_info_t *get_skill_info(uint32_t skill_id);

	skill_data_t *get_skill_data(uint32_t skill_id);
	// 获取默认技能
	int32_t get_default_skill();

	// 获取反击技能
	int32_t get_counter_skill();

	bool check_skill_release( uint32_t skill_id );

	void add_skill_cd_notify();

	void sync_skill_cd(fight_hero_ptr attacker, skill_info_t *p_skill_info);

	int32_t get_skill_pos(uint32_t skill_id);
public:
	uint32_t get_skill_level( uint32_t skill_id );

	uint32_t get_turn_level() { return m_turn_level;  }

	void set_turn_level(uint32_t turn_level) { m_turn_level = turn_level; }
public:
	void set_cur_skill_conf(SkillEffectTable *conf) { m_cur_skill_conf = conf; }
	const SkillEffectTable* get_cur_skill_conf() { return m_cur_skill_conf;  }
	static bool is_single_attack(SkillEffectTable *conf);
public:	
	//获取技能CD
	uint32_t get_skill_cd(uint32_t skill_id);
	//减少所有技能CD
	void sub_all_skill_cd(uint32_t cd);
	//减指定技能CD
	void sub_skill_cd(uint32_t skill_id, uint32_t cd);
	// 加技能cd
	void add_skill_cd(uint32_t skill_id);
	//加被动技能CD
	void add_passive_skill_cd(uint32_t skill_id, uint32_t old_turn_level);
	// 加技能CD
	void add_skill_cd_ex(uint32_t skill_id, uint32_t cd);
	//设置技能CD
	void set_skill_cd(uint32_t skill_id, uint32_t cd);

	// 被添加所有技能CD 
	void be_inc_all_skill_cd(fight_hero_ptr attacker, uint32_t cd);
	// 被减少所有技能CD
	void be_dec_all_skill_cd(fight_hero_ptr attacker, uint32_t cd);
	// 被随机添加技能CD 
	void be_inc_random_skill_cd(fight_hero_ptr attacker, uint32_t cd);
	// 被随机减少技能CD
	void be_dec_random_skill_cd(fight_hero_ptr attacker, uint32_t cd);
private:
	std::vector<skill_info_t> m_skill_list;	//技能列表
	std::map<uint32_t, uint32_t> m_skill_pos;		//技能对应位置	
	uint32_t m_turn_level = 0;						//变身等级  	//使用m_turn_level只是为了加速速度读取
	SkillEffectTable *m_cur_skill_conf;
	fight_hero_wptr m_owner;
};

#endif