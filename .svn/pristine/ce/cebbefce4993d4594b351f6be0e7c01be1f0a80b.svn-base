#include "script_fight_hero.h"
#include "common/ScriptManager.hpp"
#include "common/config_mgr.h"

script_fight_hero_t::script_fight_hero_t(fight_hero_t* owner)
	: ScriptProxy<fight_hero_t>(owner, this, ScriptManager::getInstancePtr(), "script_fight_hero_t")
{
}

script_fight_hero_t::~script_fight_hero_t(void)
{
}

void script_fight_hero_t::on_init()
{
	this->callRTVoid("on_init");
}

// ÊÂ¼þ
void script_fight_hero_t::on_event(
	const int event,
	script_fight_hero_t* attacker,
	script_fight_hero_t* defender,
	const int skill,
	const int damage,
	const int phase)
{

	//this->callRTVoid("on_event", event, (script_fight_hero_t*)attacker, (script_fight_hero_t*)defender, skill, damage, phase);
}

void script_fight_hero_t::find_target(unsigned int skill)
{
	this->callRTVoid("find_target", skill);
}

void script_fight_hero_t::before_attack(unsigned int skill)
{
	this->callRTVoid("before_attack", skill);
}

void script_fight_hero_t::after_attack(unsigned int skill)
{
	this->callRTVoid("after_attack", skill);
}

void script_fight_hero_t::other_before_attack(script_fight_hero_t* attacker, const char *targets, unsigned int skill)
{
	this->callRTVoid("other_before_attack", attacker, targets, skill);
}

void script_fight_hero_t::other_after_attack(script_fight_hero_t* attacker, const char *targets, unsigned int skill)
{
	this->callRTVoid("other_after_attack", attacker, targets, skill);
}

void script_fight_hero_t::before_attack_target(script_fight_hero_t* target, unsigned int skill)
{
	this->callRTVoid("before_attack_target", target, skill);
}

void script_fight_hero_t::after_attack_target(script_fight_hero_t* target, unsigned int skill)
{
	this->callRTVoid("after_attack_target", target, skill);
}

void script_fight_hero_t::other_before_attack_target(script_fight_hero_t* attacker, script_fight_hero_t* target, unsigned int skill)
{
	this->callRTVoid("other_before_attack_target", attacker, target, skill);
}

void script_fight_hero_t::other_after_attack_target(script_fight_hero_t* attacker, script_fight_hero_t* target, unsigned int skill)
{
	this->callRTVoid("other_after_attack_target", attacker, target, skill);
}

void script_fight_hero_t::other_before_be_attack(script_fight_hero_t* attacker, script_fight_hero_t* target, unsigned int skill)
{
	this->callRTVoid("other_before_be_attack", attacker, target, skill);
}

void script_fight_hero_t::other_after_be_attack(script_fight_hero_t* attacker, script_fight_hero_t* target, unsigned int skill)
{
	this->callRTVoid("other_after_be_attack", attacker, target, skill);
}

void script_fight_hero_t::before_attack_phase(script_fight_hero_t* target, unsigned int skill)
{
	this->callRTVoid("before_attack_phase", target, skill);
}

void script_fight_hero_t::after_attack_phase(script_fight_hero_t* target, unsigned int skill, size_t phase)
{
	this->callRTVoid("after_attack_phase", target, skill, phase);
}

void script_fight_hero_t::round_start(unsigned int round)
{
	this->callRTVoid("round_start", round);
}

void script_fight_hero_t::round_end(unsigned int round)
{
	this->callRTVoid("round_end", round);
}

void script_fight_hero_t::hp_change(int change)
{
	this->callRTVoid("hp_change", change);
}

void script_fight_hero_t::before_be_attack(script_fight_hero_t* attacker, unsigned int skill)
{
	this->callRTVoid("before_be_attack", attacker, skill);
}

void script_fight_hero_t::after_be_attack(script_fight_hero_t* attacker, unsigned int skill)
{
	this->callRTVoid("after_be_attack", attacker, skill);
}

void script_fight_hero_t::before_absorb(script_fight_hero_t* attacker, unsigned int skill, int damage)
{
	this->callRTVoid("before_absorb", attacker, skill, damage);
}

bool script_fight_hero_t::before_injured(script_fight_hero_t* attacker, unsigned int skill, int damage)
{
	return this->callRTBool("before_injured", attacker, skill, damage);
}

bool script_fight_hero_t::before_damage(script_fight_hero_t* target, unsigned int skill, int damage)
{
	return this->callRTBool("before_damage", target, skill, damage);
}

bool script_fight_hero_t::other_before_injured(script_fight_hero_t* attacker, script_fight_hero_t* target, unsigned int skill, int damage)
{
	return this->callRTBool("other_before_injured", attacker, target, skill, damage);
}

void script_fight_hero_t::on_dead()
{
	this->callRTVoid("on_dead");
}

void script_fight_hero_t::on_other_dead(script_fight_hero_t* hero, script_fight_hero_t* attacker)
{
	this->callRTVoid("on_other_dead", hero, attacker);
}

void script_fight_hero_t::on_kill(script_fight_hero_t* target, unsigned int skill)
{
	this->callRTVoid("on_kill", target, skill);
}

void script_fight_hero_t::on_zero_cd(unsigned int skill)
{
	this->callRTVoid("on_zero_cd", skill);
}

bool script_fight_hero_t::target_trigger_counter(unsigned int skill, script_fight_hero_t* target, unsigned int counter_skill)
{
	return this->callRTBool("target_trigger_counter", skill, target, counter_skill);
}

bool script_fight_hero_t::script_use_skill(unsigned int skill)
{
	return this->callRTBool("script_use_skill", skill);
}

bool script_fight_hero_t::ai_select_target(unsigned int skill)
{
	return this->callRTBool("ai_select_target", skill);
}

void script_fight_hero_t::on_buff_immuno()
{
	this->callRTVoid("on_immuno");
}

unsigned int script_fight_hero_t::c_get_script_id()
{
	if (NULL == getOwner())
	{
		log_error("owner is null");
		return 0;
	}

	return getOwner()->get_script_id();
}

unsigned int script_fight_hero_t::c_get_tid()
{
	if (NULL == getOwner())
	{
		log_error("owner is null");
		return 0;
	}

	return getOwner()->get_tid();
}

const char* script_fight_hero_t::c_get_uid()
{
	static std::string s_get_uid;
	if (NULL == getOwner())
	{
		log_error("owner is null");
		return s_get_uid.c_str();
	}

	s_get_uid = getOwner()->get_str_uid();
	return s_get_uid.c_str();
}

unsigned int script_fight_hero_t::c_get_camp()
{
	if (NULL == getOwner())
	{
		log_error("owner is null");
		return 0;
	}

	return getOwner()->get_camp();
}

unsigned int script_fight_hero_t::c_get_sex()
{
	if (NULL == getOwner())
	{
		log_error("owner is null");
		return 0;
	}

	return getOwner()->get_sex();
}


bool script_fight_hero_t::c_is_dead()
{
	if (NULL == getOwner())
	{
		log_error("owner is null");
		return false;
	}

	return getOwner()->is_dead();
}

int script_fight_hero_t::c_get_attr(unsigned int type)
{
	if (NULL == getOwner())
	{
		log_error("owner is null");
		return 0;
	}

	return getOwner()->get_attr(type);
}

void script_fight_hero_t::c_set_attr(unsigned int type, int value)
{
	if (NULL == getOwner())
	{
		log_error("owner is null");
		return;
	}

	getOwner()->set_attr(type, value);
	FIGHT_LOG("[%lu]script set attr camp[%u] pos[%u] tid[%u] type[%u] value[%d]",
		getOwner()->get_fight_uid(), getOwner()->get_camp(), getOwner()->get_pos(), getOwner()->get_tid(), type, value);
}

void script_fight_hero_t::c_sub_hp(int value, script_fight_hero_t* attaker, unsigned int skill)
{
	if (NULL == attaker)
	{
		log_error("attaker is null");
		return;
	}

	if (NULL == getOwner())
	{
		log_error("owner is null");
		return;
	}

	if (NULL == attaker->getOwner())
	{
		log_error("attaker owner is null");
		return;
	}

	getOwner()->sub_hp(value, attaker->getOwner()->shared_from_this(), skill);
}

void script_fight_hero_t::c_add_hp(int value)
{
	if (NULL == getOwner())
	{
		log_error("owner is null");
		return;
	}

	getOwner()->add_hp(value);
}

void script_fight_hero_t::c_add_buff(script_fight_hero_t* target, unsigned int id, unsigned int skill, unsigned int layer)
{
	if (NULL == target)
	{
		log_error("target is null");
		return;
	}

	if (NULL == target->getOwner())
	{
		log_error("target owner is null");
		return;
	}

	if (NULL == getOwner())
	{
		log_error("owner is null");
		return;
	}

	target->getOwner()->m_buff_mgr.add_buff(id, getOwner()->get_uid(), skill, layer);
	FIGHT_LOG("[%lu]script add buff camp[%u] pos[%u] tid[%u] buff[%u] layer[%u]",
		target->getOwner()->get_fight_uid(), target->getOwner()->get_camp(), target->getOwner()->get_pos(), target->getOwner()->get_tid(), id, layer);
}

void script_fight_hero_t::c_remove_buff(unsigned int tid, unsigned int attacker_id)
{
	if (NULL == getOwner())
	{
		log_error("owner is null");
		return;
	}

	getOwner()->m_buff_mgr.remove_buff_by_tid(tid, attacker_id);
	FIGHT_LOG("[%lu]script remove buff camp[%u] pos[%u] tid[%u] buff[%u]",
		getOwner()->get_fight_uid(), getOwner()->get_camp(), getOwner()->get_pos(), getOwner()->get_tid(), tid);
}

void script_fight_hero_t::c_dispel_buff(unsigned int type, unsigned int num, unsigned int attacker_id)
{
	if (NULL == getOwner())
	{
		log_error("owner is null");
		return;
	}

	getOwner()->m_buff_mgr.dispel_buff(type, num, attacker_id);
	FIGHT_LOG("[%lu]script dispel buff camp[%u] pos[%u] tid[%u] buff_type[%d] num[%d]",
		getOwner()->get_fight_uid(), getOwner()->get_camp(), getOwner()->get_pos(), getOwner()->get_tid(), type, num);
}

bool script_fight_hero_t::c_has_buff(unsigned int tid)
{
	if (NULL == getOwner())
	{
		log_error("owner is null");
		return false;
	}

	return (getOwner()->m_buff_mgr.get_buff_by_tid(tid) != NULL);
}

void script_fight_hero_t::c_add_target(script_fight_hero_t* target)
{
	if (NULL == target)
	{
		log_error("target is null");
		return;
	}

	if (NULL == target->getOwner())
	{
		log_error("target owner is null");
		return;
	}

	if (NULL == getOwner())
	{
		log_error("owner is null");
		return;
	}

	//getOwner()->add_cur_target(target->getOwner()->shared_from_this());
}

script_fight_hero_t* script_fight_hero_t::c_get_fight_hero(const char* uid)
{
	if (NULL == uid)
	{
		log_error("uid is null");
		return NULL;
	}

	if (NULL == getOwner())
	{
		log_error("owner is null");
		return NULL;
	}

	if (NULL == getOwner()->get_combat())
	{
		log_error("combat is null");
		return NULL;
	}

	fight_hero_ptr hero = getOwner()->get_combat()->find_hero(string_util_t::string_to_uint64(std::string(uid)));
	if (NULL == hero)
	{
		log_error("hero is null %s", uid);
		return NULL;
	}

	return hero->get_script_proxy();
}

const char* script_fight_hero_t::c_get_all_enemy()
{
	if (NULL == getOwner())
	{
		log_error("owner is null");
		return NULL;
	}

	if (NULL == getOwner()->get_combat())
	{
		log_error("combat is null");
		return NULL;
	}

	static std::string s_all_enemy;
	s_all_enemy.clear();

	const pos_hero* heros = getOwner()->get_combat()->get_camp_pos_hero(getOwner()->get_enemy_camp());
	if (NULL == heros)
	{
		log_error("heros is null");
		return NULL;
	}

	for (auto pospair : *heros)
	{
		s_all_enemy += string_util_t::uint64_to_string(pospair.second);
		s_all_enemy += ";";
	}

	return s_all_enemy.c_str();
}

const char* script_fight_hero_t::c_get_all_teammate()
{
	if (NULL == getOwner())
	{
		log_error("owner is null");
		return NULL;
	}

	if (NULL == getOwner()->get_combat())
	{
		log_error("combat is null");
		return NULL;
	}

	static std::string s_all_teammate;
	s_all_teammate.clear();

	const pos_hero* heros = getOwner()->get_combat()->get_camp_pos_hero(getOwner()->get_camp());
	if (NULL == heros)
	{
		log_error("heros is null");
		return NULL;
	}

	for (auto pospair : *heros)
	{
		s_all_teammate += string_util_t::uint64_to_string(pospair.second);
		s_all_teammate += ";";
	}

	return s_all_teammate.c_str();
}

void script_fight_hero_t::c_skill_damage(script_fight_hero_t* target, int damage, unsigned int skill)
{
	
}

void script_fight_hero_t::c_skill_ext_damage(script_fight_hero_t* target, int damage, unsigned int skill)
{
}

unsigned int script_fight_hero_t::c_get_default_skill()
{
	if (NULL == getOwner())
	{
		log_error("owner is null");
		return 0;
	}

	return getOwner()->get_default_skill();
}

unsigned int script_fight_hero_t::c_get_counter_skill()
{
	if (NULL == getOwner())
	{
		log_error("owner is null");
		return 0;
	}

	return getOwner()->get_counter_skill();
}

void script_fight_hero_t::c_use_skill(unsigned int skill, unsigned int type, const char* targets)
{
	if (NULL == getOwner())
	{
		log_error("owner is null");
		return;
	}

	SkillEffectTable* skill_effect_conf = getOwner()->get_skill_effect(skill);
	if (NULL == skill_effect_conf)
	{
		log_error("skill_effect_conf is null %u", skill);
		return;
	}

	FIGHT_LOG("[%lu]script use skill camp[%u] pos[%u] tid[%u] skill[%u]",
		getOwner()->get_fight_uid(), getOwner()->get_camp(), getOwner()->get_pos(), getOwner()->get_tid(), skill);

	if (c_get_var("c_use_skill") > 0)
	{
		return;
	}

	c_set_var("c_use_skill", 1);


	if (NULL == targets)
	{
		getOwner()->do_attack(skill_effect_conf, NULL, (proto::common::combat_act_type)type);
	}
	else
	{
		std::string str_targets(targets);
		std::vector<std::string> vec_targets;
		string_util_t::split(str_targets, vec_targets, ";");
		std::vector<fight_hero_ptr> target_list;
		for (auto target : vec_targets)
		{
			script_fight_hero_t* hero = c_get_fight_hero(target.c_str());
			if (NULL == hero)
			{
				continue;
			}

			if (NULL == hero->getOwner())
			{
				continue;
			}

			target_list.push_back(hero->getOwner()->shared_from_this());
		}

		if (target_list.empty())
		{
			getOwner()->do_attack(skill_effect_conf, NULL, (proto::common::combat_act_type)type);
		}
		else
		{
			getOwner()->do_attack(skill_effect_conf, NULL, (proto::common::combat_act_type)type, &target_list);
		}
	}

	c_set_var("c_use_skill", 0);
}

void script_fight_hero_t::c_share_damage_to(script_fight_hero_t* teammate, script_fight_hero_t* attacker, unsigned int skill, int damage)
{

}


void script_fight_hero_t::c_sub_buff_layer(unsigned int tid, unsigned int attacker_id)
{
	if (NULL == getOwner())
	{
		log_error("owner is null");
		return;
	}

	getOwner()->m_buff_mgr.sub_buff_layer(tid, attacker_id);
}

unsigned int script_fight_hero_t::c_get_buff_layer(unsigned int tid, unsigned int attacker_id)
{
	if (NULL == getOwner())
	{
		log_error("owner is null");
		return 0;
	}

	return getOwner()->m_buff_mgr.get_buff_layer_by_tid(tid, attacker_id);
}

// const char* script_fight_hero_t::c_get_buff_attacker(unsigned int id)
// {
// 	static std::string s_get_buff_attacker;
// 	s_get_buff_attacker.clear();
// 
// 	if (NULL == getOwner())
// 	{
// 		log_error("owner is null");
// 		return s_get_buff_attacker.c_str();
// 	}
// 
// 	uint64_t attacker = getOwner()->m_buff_mgr.get_buff_attacker(id);
// 	s_get_buff_attacker += string_util_t::uint64_to_string(attacker);
// 	return s_get_buff_attacker.c_str();
// }

void script_fight_hero_t::c_set_var(const char* var, int value)
{
	if (NULL == var)
	{
		log_error("var is null");
		return;
	}

	if (NULL == getOwner())
	{
		log_error("owner is null");
		return;
	}

	std::string str_var(var);
	getOwner()->set_script_var(str_var, value);
}

int script_fight_hero_t::c_get_var(const char* var)
{
	if (NULL == var)
	{
		log_error("var is null");
		return 0;
	}

	if (NULL == getOwner())
	{
		log_error("owner is null");
		return 0;
	}

	std::string str_var(var);
	return getOwner()->get_script_var(str_var);
}

int script_fight_hero_t::c_get_skill_by_index(const int index)
{
	return 0;
}

int script_fight_hero_t::c_get_skill_size()
{
	return 0;
}

// void script_fight_hero_t::c_add_combat_act(unsigned int type, unsigned int value, const char* target, unsigned int remain_value)
// {
// 	if (NULL == target)
// 	{
// 		log_error("target is null");
// 		return;
// 	}
// 
// 	if (NULL == getOwner())
// 	{
// 		log_error("owner is null");
// 		return;
// 	}
// 
// 	getOwner()->add_combat_act((proto::common::combat_act_type)type, value, string_util_t::string_to_uint64(std::string(target)), remain_value);
// }

void script_fight_hero_t::c_add_combat_act(unsigned int type, unsigned int value, script_fight_hero_t* target, script_fight_hero_t* attacker, unsigned int remain_value)
{
	if (NULL == target)
	{
		log_error("target is null");
		return;
	}

	if (NULL == attacker)
	{
		log_error("attacker is null");
		return;
	}

	if (NULL == getOwner())
	{
		log_error("owner is null");
		return;
	}

	if (NULL == target->getOwner())
	{
		log_error("target owner is null");
		return;
	}

	if (NULL == attacker->getOwner())
	{
		log_error("attacker owner is null");
		return;
	}

	getOwner()->add_combat_act((proto::common::combat_act_type)type, value, target->getOwner()->shared_from_this(), attacker->getOwner()->shared_from_this(), remain_value);
}

void script_fight_hero_t::c_add_combat_skill_act(unsigned int type, unsigned int value, const char* attacker, const char* targets)
{
	if (NULL == attacker)
	{
		log_error("attacker is null");
		return;
	}

	if (NULL == targets)
	{
		log_error("targets is null");
		return;
	}

	if (NULL == getOwner())
	{
		log_error("owner is null");
		return;
	}
	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
	std::string str_targets(targets);
	std::vector<std::string> vec_targets;
	string_util_t::split(str_targets, vec_targets, ";");
	std::vector<uint64_t> target_list;
	for (auto target : vec_targets)
	{
		target_list.push_back(string_util_t::string_to_uint64(target));
	}

	getOwner()->add_combat_skill_act((proto::common::combat_act_type)type, value, string_util_t::string_to_uint64(std::string(attacker)), target_list);
}

void script_fight_hero_t::c_set_cur_select(const char* target)
{
	if (NULL == target)
	{
		log_error("target is null");
		return;
	}

	if (NULL == getOwner())
	{
		log_error("owner is null");
		return;
	}

	getOwner()->set_cur_select(std::string(target));
}

const char* script_fight_hero_t::c_get_max_atk_target(unsigned int skill)
{
	static std::string s_get_max_atk_target;
	s_get_max_atk_target.clear();

	if (NULL == getOwner())
	{
		log_error("owner is null");
		return s_get_max_atk_target.c_str();
	}

	if (NULL == getOwner()->get_combat())
	{
		log_error("combat is null");
		return s_get_max_atk_target.c_str();
	}

	SkillEffectTable* skill_conf = getOwner()->get_skill_effect(skill);
	if (NULL == skill_conf)
	{
		log_error("SkillEffectTable is null %u", skill);
		return s_get_max_atk_target.c_str();
	}

	fight_hero_ptr target = getOwner()->get_combat()->select_max_atk_target(getOwner()->shared_from_this(), skill_conf);
	if (NULL == target)
	{
		log_error("select max atk target fail %u", skill);
		return s_get_max_atk_target.c_str();
	}

	s_get_max_atk_target = target->get_str_uid();
	return s_get_max_atk_target.c_str();
}

const char* script_fight_hero_t::c_get_weak_target(unsigned int skill)
{
	static std::string s_get_weak_target;
	s_get_weak_target.clear();

	if (NULL == getOwner())
	{
		log_error("owner is null");
		return s_get_weak_target.c_str();
	}

	if (NULL == getOwner()->get_combat())
	{
		log_error("combat is null");
		return s_get_weak_target.c_str();
	}

	SkillEffectTable* skill_conf = getOwner()->get_skill_effect(skill);
	if (NULL == skill_conf)
	{
		log_error("skill_conf is null %u", skill);
		return s_get_weak_target.c_str();
	}

	fight_hero_ptr target = getOwner()->get_combat()->select_weak_target(getOwner()->shared_from_this(), skill_conf);
	if (NULL == target)
	{
		log_error("select weak target fail %u", skill);
		return s_get_weak_target.c_str();
	}

	s_get_weak_target = target->get_str_uid();
	return s_get_weak_target.c_str();
}

const char* script_fight_hero_t::c_get_weak_hp_target(unsigned int skill)
{
	static std::string s_get_weak_hp_target;
	s_get_weak_hp_target.clear();

	if (NULL == getOwner())
	{
		log_error("owner is null");
		return s_get_weak_hp_target.c_str();
	}

	if (NULL == getOwner()->get_combat())
	{
		log_error("combat is null");
		return s_get_weak_hp_target.c_str();
	}

	SkillEffectTable* skill_conf = getOwner()->get_skill_effect(skill);
	if (NULL == skill_conf)
	{
		log_error("skill_conf is null %u", skill);
		return s_get_weak_hp_target.c_str();
	}

	fight_hero_ptr target = getOwner()->get_combat()->select_weak_target(getOwner()->shared_from_this(), skill_conf);
	if (NULL == target)
	{
		target = getOwner()->get_combat()->select_min_hp_target(getOwner()->shared_from_this(), skill_conf);
	}

	if (NULL == target)
	{
		log_error("select weak hp target fail %u", skill);
		return s_get_weak_hp_target.c_str();
	}

	s_get_weak_hp_target = target->get_str_uid();
	return s_get_weak_hp_target.c_str();
}
// 
// const char* script_fight_hero_t::c_get_skill_data(unsigned int skill)
// {
// 	static std::string s_get_skill_data;
// 	s_get_skill_data.clear();
// 
// 	if (NULL == getOwner())
// 	{
// 		log_error("owner is null");
// 		return s_get_skill_data.c_str();
// 	}
// 
// 	proto::common::skill_single ss = getOwner()->get_skill_data(skill);
// 	
// 	s_get_skill_data += string_util_t::uint32_to_string(ss.skill_level());
// 	s_get_skill_data += ";";
// 
// 	return s_get_skill_data.c_str();
// }

const char* script_fight_hero_t::c_get_fight_calc_result()
{
	static std::string s_get_fight_calc_result;
	s_get_fight_calc_result.clear();

	if (NULL == getOwner())
	{
		log_error("owner is null");
		return s_get_fight_calc_result.c_str();
	}

	const fight_calc_result_t& result = getOwner()->get_fight_calc_result();
	s_get_fight_calc_result += string_util_t::int32_to_string((int32_t)(result.dodge));
	s_get_fight_calc_result += ";";
	s_get_fight_calc_result += string_util_t::int32_to_string((int32_t)(result.crit));
	s_get_fight_calc_result += ";";
	s_get_fight_calc_result += string_util_t::int32_to_string(result.damage);
	s_get_fight_calc_result += ";";
	s_get_fight_calc_result += string_util_t::int32_to_string(result.heal);
	s_get_fight_calc_result += ";";
	s_get_fight_calc_result += string_util_t::int32_to_string(result.block);
	s_get_fight_calc_result += ";";
	s_get_fight_calc_result += string_util_t::int32_to_string(result.absorb);
	s_get_fight_calc_result += ";";
	s_get_fight_calc_result += string_util_t::int32_to_string(result.leech);
	s_get_fight_calc_result += ";";
	s_get_fight_calc_result += string_util_t::int32_to_string(result.reflect);
	s_get_fight_calc_result += ";";
	s_get_fight_calc_result += string_util_t::int32_to_string(result.beheaded);
	s_get_fight_calc_result += ";";

	return s_get_fight_calc_result.c_str();
}
