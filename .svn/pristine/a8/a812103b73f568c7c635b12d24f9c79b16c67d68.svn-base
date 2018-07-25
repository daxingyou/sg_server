#include "fight_hero.hpp"
#include "common/config_mgr.h"
#include "tblh/MonsterHero.tbls.h"
#include "tblh/attr_enum.hpp"
#include "tblh/HeroStar.tbls.h"
#include "tblh/HeroGrade.tbls.h"
#include "tblh/Hero.tbls.h"
#include "script/script_fight_hero.h"
#include "tblh/Comprehensive.tbls.h"
#include "tblh/RestraintTable.tbls.h"
#include "config/config_manager.hpp"
#include "tblh/SkillAI.tbls.h"
#include "tblh/SkillTargetAI.tbls.h"

static const size_t NONE_PHASE = 99999;

fight_hero_t::fight_hero_t()
	: m_script_proxy(new script_fight_hero_t(this))
{
}

fight_hero_t::~fight_hero_t()
{
	close();
}

void fight_hero_t::close()
{
	if (NULL != m_script_proxy)
	{
		m_script_proxy->dispose();
		//delete(m_script_proxy);
		m_script_proxy = NULL;
	}
	m_combat.reset();
	m_buff_mgr.close();
	m_cur_attr.clear();
	m_skills.clear();
	m_skill_cd.clear();
//	m_round_data.clear();
	m_attack_targets.clear();
	m_immuno_buff_types.clear();
}

bool fight_hero_t::init(combat_ptr combat, proto::common::hero_single* hs)
{
	if (NULL == hs)
	{
		log_error("hs is null");
		return false;
	}

	if (NULL == combat)
	{
		log_error("combat is null");
		return false;
	}
	
	m_combat = combat;
	m_buff_mgr.set_owner(shared_from_this());
	m_hero_data.CopyFrom(*hs);

	for (const proto::common::attr_single& attr : hs->fight_attr().attr().attrs())
	{
		set_attr(attr.type(), attr.value());
		//FIGHT_LOG("[%lu]fight_hero_t init: camp[%u] pos[%u] tid[%u] [%u][%d]", get_fight_uid(), get_camp(), get_pos(), get_tid(), attr.type(), attr.value());
	}

	if (m_hero_data.fight_attr().start().pos() == get_pos())
	{
		set_attr(attrtype_enum::hp, m_hero_data.fight_attr().start().hp());
	}
	else
	{
		set_attr(attrtype_enum::hp, get_attr(attrtype_enum::hp));
	}

	if (get_hp() == 0)
	{
		return false;
	}

	uint32_t skill_size = hs->skill().skills_size();

	
	for (uint32_t i = 0; i < skill_size; ++i)
	{
		const proto::common::skill_single& skill_single = hs->skill().skills(i);
		if(skill_single.has_skill_cd())
			add_skill(skill_single.skill_id(), skill_single.skill_level(), skill_single.skill_cd());
		else
			add_skill(skill_single.skill_id(), skill_single.skill_level(), 0);
	}

	m_att_speed = get_speed();
	m_order_att_speed = get_speed();
	m_round = 1;
	
	FIGHT_LOG("[%lu]fight_hero_t init: camp[%u] pos[%u] tid[%u] [%u][%d]", get_fight_uid(), get_camp(), get_pos(), get_tid(), attrtype_enum::hp, get_attr(attrtype_enum::hp));

	return true;
}

void fight_hero_t::remove_skill(const int skill)
{
	if (skill == 0)
		return;
	m_skills.erase(skill);
}

int fight_hero_t::get_skill_level(const int skill)
{
	std::map<uint32_t, skill_data>::const_iterator it;
	it = m_skills.find(skill);
	if (it != m_skills.end())
		return it->second.skill_level;
	return 0;
}

void fight_hero_t::add_skill(const int skill, const int level, const int init_cd, bool is_change_model)
{
	SkillEffectTable *p_skill_conf = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(skill, level));
	if (NULL == p_skill_conf)
	{
		log_error("fight_hero_t p_skill_conf NULL skill_id:%d skill_level:%d", skill, level);
		return;
	}

	skill_data data;
	data.skill_id = skill;
	data.skill_level = level;
	data.type = (proto::common::skill_type)p_skill_conf->type();
	m_skills.insert(std::make_pair(data.skill_id, data));

	if (init_cd > 0)
	{
		m_skill_cd.insert(std::make_pair(skill, init_cd));
	}
}

void fight_hero_t::on_init()
{
	distribute_special_buff();
	activation_passive_buff();

	get_script_proxy()->on_init();
}

void fight_hero_t::set_cur_action(const proto::common::combat_form_single& action)
{
	m_cur_form.CopyFrom(action);
}

void fight_hero_t::clear_cur_action()
{
	m_cur_form.Clear();
}

void fight_hero_t::reset_wave_change_data()
{
	if (is_dead())
		return;

	auto recover_hp_conf = GET_CONF(Comprehensive, comprehensive_common::hp_recover_every_stage);
	auto recover_cd_conf = GET_CONF(Comprehensive, comprehensive_common::skill_cd_recover_every_stage);
	uint32_t recover_hp_percent = GET_COMPREHENSIVE_VALUE_1(recover_hp_conf);
	uint32_t recover_cd = GET_COMPREHENSIVE_VALUE_1(recover_cd_conf);

	m_buff_mgr.remove_all_buff();

	uint32_t max_hp = get_attr(attrtype_enum::max_hp);
	uint32_t recover_hp = max_hp * recover_hp_percent / 10000;
	add_hp(recover_hp);
	sub_skill_cd(recover_cd);
	m_count_data.reset();

	set_att_speed(get_speed());
	set_att_order_speed(get_speed());
	set_att_more_speed(0);

	add_combat_act(proto::common::combat_act_type_syn_hp, recover_hp, shared_from_this(), shared_from_this(), get_hp());
	std::map<uint32_t, uint32_t>::iterator iter = m_skill_cd.begin();
	for (; iter != m_skill_cd.end(); ++iter)
	{
		add_combat_act(proto::common::combat_act_type_syn_cd, iter->first, shared_from_this(), shared_from_this(), iter->second);
	}
}

uint32_t fight_hero_t::trigger_skill_count() const
{
	return get_count_data().trigger_skill_count;
}

void fight_hero_t::trigger_skill_count(uint32_t value)
{
	get_count_data().trigger_skill_count = value;
}

void fight_hero_t::activation_skill(uint32_t skill, uint32_t level)
{
	if (skill > 0)
	{
		m_activation_skill = skill;
		m_activation_skill_level = level;
		add_skill(skill, level, 0);
	}
	else
	{
		remove_skill(m_activation_skill);
		m_activation_skill = 0;
		m_activation_skill_level = 0;
	}
	
}

void fight_hero_t::add_immuno_buff_type(uint32_t type)
{
	m_immuno_buff_types.push_back(type);
}


uint32_t fight_hero_t::counter_count() const
{
	return get_count_data().counter_count;
}

void fight_hero_t::counter_count(uint32_t value)
{
	get_count_data().counter_count = value;
}

uint32_t fight_hero_t::combo_count() const
{
	return get_count_data().combo_count;
}

void fight_hero_t::combo_count(uint32_t value)
{
	get_count_data().combo_count = value;
}

bool fight_hero_t::do_attack()
{
	if (get_combat() == NULL)
	{
		log_error("do_attack m_combat is null");
		return false;
	}

	if (NULL == get_script_proxy())
	{
		log_error("script_proxy is null");
		return false;
	}

	do_auto_fight();

	bool can_counter = true;
	uint32_t skill_id = 0;
	fight_hero_ptr select = NULL;
	std::vector<fight_hero_ptr> targets;

	if (m_cur_form.attacker().empty()) // 没有部署
	{
		skill_id = get_default_skill();
	}
	else
	{
		skill_id = m_cur_form.skill();
	}

	// 沉默 嘲讽 混乱
	if (get_attr(attrtype_enum::silent) > 0 || get_attr(attrtype_enum::taunt) > 0 ||
		get_attr(attrtype_enum::confused) > 0)
	{
		skill_id = get_default_skill();
	}
	// 混乱
	if (get_attr(attrtype_enum::confused) > 0)
	{
		select = get_combat()->select_confused_target(shared_from_this());
		can_counter = false;
	}
	// 嘲讽
	else if (get_attr(attrtype_enum::taunt) > 0)
	{
		//targets.push_back(get_special_target());
		select = get_special_target();
	}
	// 正常的选择目标
	else
	{
		uint64_t target_id = string_util_t::string_to_uint64(m_cur_form.target());
		select = get_combat()->find_hero(target_id);
	}

	SkillEffectTable* skill_effect_conf = get_skill_effect(skill_id);
	if (NULL == skill_effect_conf)
	{
		log_error("do_attack skill_id invalid %u", skill_id);
		return false;
	}
	
	return do_attack(skill_effect_conf, select, proto::common::combat_act_type_skill, &targets, false, can_counter);
}

bool fight_hero_t::check_skill(int32_t skill_id)
{
	if (-1 == skill_id || 0 == skill_id)
		return false;

	uint32_t cd = get_skill_cd(skill_id);
	if (cd > 0)
	{
		return false;
	}
	return true;
}

uint32_t fight_hero_t::get_best_skill()
{
	std::vector<SkillEffectTable*> skill_ai_list;

	// 如果有设定自动战斗模式 默认使用普攻
	if (get_auto_fight_mode() == 1)
	{
		return get_default_skill();
	}
	
	SkillEffectTable *p_conf = NULL;
	std::map<uint32_t, skill_data>::iterator iter = m_skills.begin();
	for (; iter != m_skills.end(); ++iter)
	{
		if(!check_skill(iter->first))
			continue;
		p_conf = get_skill_effect(iter->first);
		if (p_conf != NULL && p_conf->type() != proto::common::skill_type_passive)
		{
			skill_ai_list.push_back(p_conf);
		}
	}

	uint32_t find_skill = find_skill_ai(skill_ai_list);
		
	return find_skill;
}

bool fight_hero_t::do_auto_fight()
{
	if (!is_auto_fight()) // 自动战斗
		return false;
	
	m_cur_form.Clear();

	
	uint32_t skill_id = get_best_skill();
	if (skill_id > 0)
	{
		m_cur_form.set_attacker(get_str_uid());
		m_cur_form.set_skill(skill_id);
	}
	return true;
}

uint32_t fight_hero_t::find_skill_ai(std::vector<SkillEffectTable*>& skill_ai_list)
{
	if (NULL == get_combat())
	{
		log_error("find_skill_ai get_combat() NULL");
		return 0;
	}

	SkillAI *p_conf = NULL;
	uint32_t ai_id = 0;							// AI id
	uint32_t enemy_flag_count = 0;				// 有标记的敌人数量
	uint32_t enemy_count = 0;					// 敌人总数
	fight_hero_ptr p_fight_hero = NULL;
	std::map<uint32_t, uint32_t> skill_ai_type_list;
	std::vector<uint32_t> skill_ai_rate_list;

	// 计算出出现的全部事件
	const fight_hero_map& all_heros = get_combat()->get_all_heros();
	for (const auto& heropair : all_heros)
	{
		p_fight_hero = heropair.second;
		if (NULL == p_fight_hero)
		{
			log_error("fight_hero p_fight_hero NULL");
			continue;
		}
		if (get_camp() == heropair.second->get_camp())
		{
			if (!p_fight_hero->is_dead())
			{
				uint32_t percent = p_fight_hero->get_hp_percent();
				if (percent <= 2500)
				{
					skill_ai_type_list[skill_ai_friend_hp_25_rate] = 1;
					skill_ai_type_list[skill_ai_friend_hp_25_add_rate] += 1;
				}
				else if (percent <= 5000)
				{
					skill_ai_type_list[skill_ai_friend_hp_50_rate] = 1;
					skill_ai_type_list[skill_ai_friend_hp_50_add_rate] += 1;
				}
				else if (percent <= 7500)
				{
					skill_ai_type_list[skill_ai_friend_hp_75_rate] = 1;
					skill_ai_type_list[skill_ai_friend_hp_75_add_rate] += 1;
				}
			}
			if (p_fight_hero->is_dead())
				skill_ai_type_list[skill_ai_friend_dead_rate] = 1;
			if (p_fight_hero->m_buff_mgr.get_debuff_count() > 0)
			{
				skill_ai_type_list[skill_ai_friend_debuff_rate] = 1;
				skill_ai_type_list[skill_ai_friend_debuff_add_rate] += 1;
			}
		}
		else
		{
			if (p_fight_hero->m_buff_mgr.get_profit_buff_count() > 0)
			{
				skill_ai_type_list[skill_ai_enemy_gain_rate] = 1;
				skill_ai_type_list[skill_ai_enemy_gain_add_rate] += 1;
			}
			if (p_fight_hero->get_attr(attrtype_enum::immuno) > 0)
				++enemy_flag_count;
			++enemy_count;
		}
	}
	if (enemy_flag_count == enemy_count)
		skill_ai_type_list[skill_ai_enemy_all_immuno_rate] = 1;

	// 根据选出的事件加权重
	SkillEffectTable *p_skill_conf = NULL;
	uint32_t find_skill = 0;							// 找到的技能
	int32_t skill_ai_rate = 0;							// 技能AI权重
	int32_t max_skill_ai_rate = 0;						// 最大的技能AI权重
	int32_t all_skill_ai_rate = 0;						// 全部技能AI权重
	uint32_t skill_ai_id = 0;							// 技能AI id
	const int32_t skill_ai_rate_max_value = 100;		// 技能ai权重的阀值
	std::map<uint32_t, uint32_t>::iterator iter;
	for (uint32_t i = 0; i < skill_ai_list.size(); ++i)
	{
		p_skill_conf = skill_ai_list[i];
		if (NULL == p_skill_conf)
		{
			log_error("find_skill_ai p_skill_conf NULL pos[%d]", i);
		}
		skill_ai_id = p_skill_conf->ai_id();
		p_conf = GET_CONF(SkillAI, skill_ai_id);
		if (NULL == p_conf)
		{	
			log_error("find_skill_ai p_conf NULL id[%d]", ai_id);
			continue;
		}
		skill_ai_rate = p_conf->get_ai_rate(skill_ai_start_rate);
		for (iter = skill_ai_type_list.begin(); iter != skill_ai_type_list.end(); ++iter)
		{
			skill_ai_rate += p_conf->get_ai_rate(iter->first) * iter->second;
		}
		// 如果该技能AI的权重超过阀值 就直接取该技能 不再roll
		if (skill_ai_rate >= skill_ai_rate_max_value && skill_ai_rate > max_skill_ai_rate)
		{
			max_skill_ai_rate = skill_ai_rate;
			find_skill = p_skill_conf->id();
		}
		else
		{
			skill_ai_rate_list.push_back(skill_ai_rate);
			all_skill_ai_rate += skill_ai_rate;
		}

	}
	if (find_skill != 0)
	{
		return find_skill;
	}

	uint32_t random_value = random_util_t::randBetween(1, all_skill_ai_rate);
	uint32_t compare_value = 0;
	for (uint32_t i = 0; i < skill_ai_rate_list.size(); ++i)
	{
		compare_value += skill_ai_rate_list[i];
		// roll到了一个
		if (random_value <= compare_value)
		{
			// 安全校验 避免数组越界 skill_ai_list的顺序跟skill_ai_rate_list的顺序是一致的
			if (i < skill_ai_list.size())
			{
				return skill_ai_list[i]->id();
			}
			else
			{
				log_error("find_skill_ai find_pos[%d] >= skill_ai_list_size[%d]", i, skill_ai_list.size());
				return 0;
			}
		}
	}
	log_error("find_skill_ai not find compare ai roll[%d] all_ai_rate[%d]", random_value, all_skill_ai_rate);
	return 0;
}

uint32_t fight_hero_t::find_target_ai(uint32_t skill_id)
{
	return 0;
}

bool fight_hero_t::check_do_attack(
	SkillEffectTable* skill_conf,
	fight_hero_ptr select,
	proto::common::combat_act_type type,
	const std::vector<fight_hero_ptr>* targets,
	bool trigger_by_buff)
{
	if (NULL == get_script_proxy())
	{
		log_error("script_proxy is null");
		return false;
	}

	if (NULL == skill_conf)
	{
		log_error("skill_conf is null");
		return false;
	}

	if (!trigger_by_buff)
	{
		if (is_dead())
		{
			log_error("dead can not attack");
			return false;
		}

		if (is_in_control()) // 被控
		{
			return false;
		}
	}
	return true;
}

void fight_hero_t::do_counter(std::vector<fight_hero_ptr>& counters, bool can_counter)
{
	if (!can_counter)
	{
		return;
	}

	for (auto target : counters)
	{
		if (target == NULL)
		{
			continue;
		}

		FIGHT_LOG("[%lu]start counter: camp[%u] pos[%u] tid[%u]", get_fight_uid(), target->get_camp(), target->get_pos(), target->get_tid());
		// 反击,执行反击技能
		SkillEffectTable* counter_skill_effect_conf = target->get_skill_effect(target->get_counter_skill());
		if (NULL == counter_skill_effect_conf)
			continue;
		
		fight_hero_ptr counter_target = shared_from_this();
		if (is_dead())
		{
			counter_target = NULL;
		}

		target->do_attack(counter_skill_effect_conf, counter_target, proto::common::combat_act_type_counter);
	}
}

size_t fight_hero_t::get_end_phase(SkillEffectTable* skill_conf)
{
	size_t end_phase = skill_conf->hurt_rate().size();
	return end_phase;
}

bool fight_hero_t::do_attack(
	SkillEffectTable* skill_conf,
	fight_hero_ptr select,
	proto::common::combat_act_type type,
	const std::vector<fight_hero_ptr>* targets,
	bool trigger_by_buff,
	bool can_counter)
{
	if (check_do_attack(skill_conf, select, type, targets, trigger_by_buff) == false)
		return false;

	before_attack(skill_conf, trigger_by_buff);

// 	if (get_script_proxy()->script_use_skill(skill_conf->id()))
// 	{
// 		after_script_use_skill(skill_conf);
// 		return false;
// 	}

	bool trigger_combo = false;
	std::vector<fight_hero_ptr> counters;
	std::vector<fight_hero_ptr> spurting_targets;
	size_t phase = 0;

	if (NULL == targets || (targets != NULL && targets->size() == 0))
	{
		find_target_and_attack(skill_conf, counters, trigger_combo, phase, trigger_by_buff, select, type);
	}
	else
	{
		do_attack_after_find_target(skill_conf, counters, trigger_combo, phase, trigger_by_buff, *targets, spurting_targets, type);
	}

	size_t end_phase = get_end_phase(skill_conf);
	while (phase < end_phase && skill_conf->refind_target())
	{
		FIGHT_LOG("[%lu]refind target: camp[%u] pos[%u] tid[%u] phase[%u]", get_fight_uid(), get_camp(), get_pos(), get_tid(), phase);
		phase += 1;
		find_target_and_attack(skill_conf, counters, trigger_combo, phase, trigger_by_buff, select, type, true);
	}

	// todo
	after_attack(skill_conf, trigger_by_buff);
	
	// 反击和连击走下一次完整的行动
	do_counter(counters, can_counter);

	if (trigger_combo)
	{
		FIGHT_LOG("[%lu]start combo: camp[%u] pos[%u] tid[%u]", get_fight_uid(), get_camp(), get_pos(), get_tid());
		// 连击,执行本次相同的技能
		do_attack(skill_conf, NULL, proto::common::combat_act_type_combo);
	}

	return true;
}

bool fight_hero_t::find_target_and_attack(
	SkillEffectTable* skill_effect_conf, 
	std::vector<fight_hero_ptr>& counters,
	bool& trigger_combo,
	size_t& phase,
	bool trigger_by_buff,
	fight_hero_ptr select /*= NULL*/, 
	proto::common::combat_act_type type,
	bool is_refind /*= false*/)
{
	if (NULL == skill_effect_conf)
	{
		log_error("skill_effect_conf is null");
		return false;
	}
	std::vector<fight_hero_ptr> targets;
	std::vector<fight_hero_ptr> spurting_targets;
	find_target(skill_effect_conf, targets, spurting_targets, select);
	if (targets.empty())
	{
		log_error("targets is empty");
		return false;
	}

	return do_attack_after_find_target(skill_effect_conf, counters, trigger_combo, phase, trigger_by_buff, targets, spurting_targets, type, is_refind);
}

bool fight_hero_t::trigger_skill_by_buff(SkillEffectTable* skill_conf, fight_hero_ptr select, 
										 proto::common::combat_act_type type, bool can_counter)
{
	if (get_combat() == NULL)
	{
		log_error("get_combat() NULL");
		return false;
	}
	round_use_skill_info skill_info;
	skill_info.skill_conf = skill_conf;
	skill_info.select = select;
	skill_info.type = type;
	skill_info.trigger_by_buff = true;
	skill_info.can_counter = can_counter;
	skill_info.attacker = shared_from_this();

	get_combat()->add_round_use_skill_list(skill_info);

	return true;
}

bool fight_hero_t::do_skill_attack_target(SkillEffectTable* skill_conf, const std::vector<fight_hero_ptr>& targets, proto::common::combat_act_type type )
{
	std::vector<uint64_t> act_targets;
	for (auto target : targets)
	{
		if (NULL == target)
		{
			continue;
		}

		act_targets.push_back(target->get_uid());
	}
	add_combat_skill_act(type, skill_conf->id(), get_uid(), act_targets);

	do_skill_consum(skill_conf);

	// 检查是否要播放镜头 添加action	
	uint32_t camera_rate = skill_conf->camera_rate();
	if (get_combat() != NULL)
	{
		// 只有pve播放镜头
		if (get_combat()->is_pve() && get_combat()->get_fight_type() != proto::common::fight_type_dark_monster)
		{
			fight_obj_ptr p_owner = get_combat()->find_obj(get_master());
			if (p_owner != NULL)
			{
				if (p_owner->get_type() == proto::common::SCENEOBJECT_USER)
				{
					if (camera_rate > 0)
					{
						auto conf1 = GET_CONF(Comprehensive, comprehensive_common::skill_camera_rate_for_newbird);
						auto conf2 = GET_CONF(Comprehensive, comprehensive_common::skill_camera_recover_level);
						auto conf3 = GET_CONF(Comprehensive, comprehensive_common::skill_camera_rate_for_lowlevel);
						auto conf4 = GET_CONF(Comprehensive, comprehensive_common::skill_camera_recover_level_lowlevel);
						uint32_t camera_rate_for_newbird = GET_COMPREHENSIVE_VALUE_1(conf1);
						uint32_t level_limit = GET_COMPREHENSIVE_VALUE_1(conf2);
						uint32_t camera_rate_for_lowlevel = GET_COMPREHENSIVE_VALUE_1(conf3);
						uint32_t low_level_limit = GET_COMPREHENSIVE_VALUE_1(conf4);
						if (p_owner->get_level() <= low_level_limit)
							camera_rate = camera_rate_for_lowlevel;
						else if (p_owner->get_level() <= level_limit)
							camera_rate = camera_rate_for_newbird;
					}
				}
				if (camera_rate > 0 && random_util_t::selectByTenTh(camera_rate))
				{
					add_combat_skill_act(proto::common::combat_act_type_camera, skill_conf->id(), get_uid(), act_targets);
				}
			}
		}
	}
	return true;
}

bool fight_hero_t::do_attack_after_find_target(
	SkillEffectTable* skill_conf, 
	std::vector<fight_hero_ptr>& counters,
	bool& trigger_combo, 
	size_t& phase, 
	bool trigger_by_buff,
	const std::vector<fight_hero_ptr>& targets,
	const std::vector<fight_hero_ptr>& spurting_targets,
	proto::common::combat_act_type type,
	bool is_refind /*= false*/)
{
	if (NULL == skill_conf)
	{
		log_error("skill_conf is null");
		return false;
	}
	m_cur_skill_conf = skill_conf;
	++get_count_data().attack_count;

	if (!is_refind)
	{
		do_skill_attack_target(skill_conf, targets, type);
	}

	size_t init_phase = 0;
	m_target_count = targets.size();
	for (auto target : targets)
	{
		if (NULL == target)
		{
			continue;
		}

		bool trigger_counter = false;
		init_phase = phase;
		if (do_attack_target(target, skill_conf, trigger_counter, trigger_combo, init_phase, trigger_by_buff, type))
		{
			do_after_attack_target(target, skill_conf, trigger_by_buff);

			if (trigger_counter)
			{
				counters.push_back(target);
			}
		}
	}

	// 所有队友伤害转换为直伤
	if (get_attr(attrtype_enum::all_team_damage_to_injury) > 0)
	{
		get_count_data().all_team_damage = 0;
	}

	phase = init_phase;

	init_phase = 0;
	for (auto target : spurting_targets)
	{
		if (NULL == target)
		{
			continue;
		}

		bool trigger_counter_spurting = false;
		bool trigger_combo_spurting = false;
		init_phase = phase;
		if (do_attack_target(target, skill_conf, trigger_counter_spurting, trigger_combo_spurting, init_phase, trigger_by_buff, proto::common::combat_act_type_spurting))
		{
			do_after_attack_target(target, skill_conf, trigger_by_buff);
		}
	}
	
	return true;
}

// 攻击一个目标
bool fight_hero_t::check_do_attack_target(
	fight_hero_ptr target,
	SkillEffectTable* skill_conf,
	size_t& phase,
	bool trigger_by_buff,
	proto::common::combat_act_type type)
{
	if (NULL == target)
	{
		log_error("do_attack target is null");
		return false;
	}

	if (NULL == skill_conf)
	{
		log_error("do_attack skill_conf is null");
		return false;
	}

	if (NULL == get_script_proxy())
	{
		log_error("script_proxy is null");
		return false;
	}

	if (NULL == target->get_script_proxy())
	{
		log_error("target script_proxy is null");
		return false;
	}

	combat_ptr p_combat = get_combat();
	if (NULL == p_combat)
	{
		log_error("p_combat NULL");
		return false;
	}
	return true;
}

void fight_hero_t::get_pvp_param(int32_t& pvp_add_param, int32_t& pvp_dec_param, fight_hero_ptr target)
{
	if (get_combat()->is_arena_pvp())
	{
		pvp_add_param = get_attr(attrtype_enum::pvp_arena_damage);
		pvp_dec_param = target->get_attr(attrtype_enum::pvp_arena_def);
	}
	else if (get_combat()->is_un_arena_pvp())
	{
		pvp_add_param = get_attr(attrtype_enum::pvp_no_arena_damage);
		pvp_dec_param = target->get_attr(attrtype_enum::pvp_no_arena_def);
	}
}

uint32_t fight_hero_t::get_hero_immuno_attr()
{
	// 获取系别免疫属性
	switch (get_terry())
	{
	case proto::common::hero_terry_type_water:
		return attrtype_enum::water_immune;
		break;
	case proto::common::hero_terry_type_fire:
		return attrtype_enum::fire_immune;
		break;
	case proto::common::hero_terry_type_wind:
		return attrtype_enum::wind_immune;
		break;
	case proto::common::hero_terry_type_yin:
		return attrtype_enum::yin_immune;
		break;
	case proto::common::hero_terry_type_yang:
		return attrtype_enum::yang_immune;
		break;
	default:
		break;
	}
	return 0;
}

uint32_t fight_hero_t::get_hero_attr_relation_type(fight_hero_ptr p_target)
{
	if (NULL == p_target)
	{
		log_error("get_hero_attr_relation_type p_target NULL");
		return 0;
	}
	RestraintTable *p_terry_conf = GET_CONF(RestraintTable, hash_util_t::hash_to_uint32(get_terry(), p_target->get_terry()));
	uint32_t terry_relation = 0;
	if (p_terry_conf != NULL)
		terry_relation = p_terry_conf->relation();
	return terry_relation;
}

void fight_hero_t::do_crit(fight_hero_ptr target, int & crit_multiple, int & heal_crit_multiple)
{
	// 是否发生暴击
	int crit = get_attr(attrtype_enum::crit_rate); // 自己的属性30

	if (random_util_t::selectByTenTh(crit))
	{
		//crit_multiple = ceil(min(max((0 + 暴伤效果(自己的属性31) - 抗暴伤(目标的属性33)) * 0.0001,1.2),10));
		crit_multiple = std::min<int>(std::max<int>((0 + get_attr(attrtype_enum::crit_damage) - target->get_attr(attrtype_enum::crit_damage_reduce)), 12000), 100000);
		//heal_crit_multiple = ceil(min(max((0 + 暴伤效果(自己的属性31)) * 0.0001,1.2),10))；
		heal_crit_multiple = std::min<int>(std::max<int>((15000 + get_attr(attrtype_enum::crit_damage)), 12000), 100000);

		// 血量至暴击效果加成
		int addition = get_attr(attrtype_enum::lose_blood_volume_to_crit_effect);
		if (addition > 0)
		{
			int sub = get_max_hp() - get_hp();
			sub = std::max<int>(sub, 0);
			float ratio = (float)sub / (float)get_max_hp() * 100;
			crit_multiple += ratio * addition;
			heal_crit_multiple += ratio * addition;
		}
		m_calc_result.crit = true;
	}
}

bool fight_hero_t::do_skill_heal(size_t phase, SkillEffectTable* skill_conf, fight_hero_ptr target, int heal_crit_multiple)
{
	//total_heal = 己方攻击*(技能治疗效果万分比/10000)
	//m_calc_result.heal = std::max<int>((get_attr(attrtype_enum::ranged_atk) * (get_attr(attrtype_enum::skill_heal) * 0.0001)), 1);

	//m_calc_result.heal = std::max<int>(m_calc_result.heal * (skill_conf->hurt_rate(phase) * 0.0001) + skill_conf->hurt(), 1);

	// 攻击直伤
	int attack_value = (int)((float)get_attack() * ( (float)skill_conf->hurt_rate(phase) * 0.0001) );
	// 生命直伤 
	int hp_value = ( int )( ( float )get_attr(attrtype_enum::hp) * ( ( float )skill_conf->hp_hurt_rate(phase) * 0.0001 ) );

	// 防御直伤
	int defend_value = (int)((float)get_attr(attrtype_enum::def_physical) * ( (float)skill_conf->defend_hurt_rate(phase) * 0.0001) );

	// 速度直伤
	int speed_value = (int)((float)get_speed() * ( (float)skill_conf->speed_hurt_rate(phase) * 0.0001) );

	int all = attack_value + hp_value + defend_value + speed_value;

	//技能治疗效果万分比/10000
	float skill_heal = get_attr(attrtype_enum::skill_heal) * 0.0001;

	m_calc_result.heal = std::max<int>(all * skill_heal + skill_conf->hurt(), 1);

	if (m_calc_result.crit)
	{
		m_calc_result.heal *= (heal_crit_multiple * 0.0001);
	}



	// 执行加血操作
	//todo
	m_calc_result.heal = target->heal_hp(shared_from_this(), m_calc_result.heal);
	if (m_calc_result.crit)
	{
		// 						add_combat_act(proto::common::combat_act_type_heal_crit, m_calc_result.heal, target->get_uid(), 
		// 									   target->get_attr(attrtype_enum::hp));
		add_combat_act(proto::common::combat_act_type_heal_crit, m_calc_result.heal, target, shared_from_this(),
			target->get_attr(attrtype_enum::hp));
	}
	else
	{
		//add_combat_act(proto::common::combat_act_type_heal, m_calc_result.heal, target->get_uid(), target->get_attr(attrtype_enum::hp));
		add_combat_act(proto::common::combat_act_type_heal, m_calc_result.heal, target, shared_from_this(), target->get_attr(attrtype_enum::hp));
	}

	FIGHT_LOG("[%lu]target heal [%d] [%d]", get_fight_uid(), m_calc_result.heal, target->get_attr(attrtype_enum::hp));
	return true;
}

// 计算伤害限制
uint32_t fight_hero_t::calc_limit_injury(
	size_t phase,
	SkillEffectTable* skill_conf,
	fight_hero_ptr target,
	proto::common::combat_act_type type,
	int crit_multiple
	)
{
	uint32_t damage = m_calc_result.damage;
	// 单次伤害血量上限
	if (target->get_attr(attrtype_enum::once_attack_lose_max_hp_ratio) > 0)
	{
		float ratio = target->get_attr(attrtype_enum::once_attack_lose_max_hp_ratio) * 0.0001;
		int max_sub_hp = target->get_max_hp() * ratio;
		if ((int)damage > max_sub_hp)
			damage = max_sub_hp;
	}
	return damage;
}

uint32_t fight_hero_t::calc_direct_injury(
	size_t phase,
	SkillEffectTable* skill_conf,
	fight_hero_ptr target,
	proto::common::combat_act_type type,
	int crit_multiple)
{
	uint32_t ret = 0;
	// 生命直伤 
	int hp_value = (int)((float)get_attr(attrtype_enum::hp) * (float)( skill_conf->hp_hurt_rate(phase) * 0.0001 ));

	// 防御直伤
	int defend_value = (int)((float)get_attr(attrtype_enum::def_physical) * (float)( skill_conf->defend_hurt_rate(phase) * 0.0001 ));

	// 速度直伤
	int speed_value = (int)((float)get_speed() * (float)( skill_conf->speed_hurt_rate(phase) * 0.0001 ));
	ret += hp_value;
	ret += defend_value;
	ret += speed_value;

	// 诅咒BUFFID
	if (get_attr(attrtype_enum::damnation_buff_id) > 0)
	{
		int layer = target->m_buff_mgr.get_buff_layer_by_tid(get_attr(attrtype_enum::damnation_buff_id));
		if (layer > 0)
		{
			ret += target->get_max_hp() * ( get_attr(attrtype_enum::damnation_hurt_ratio) * 0.0001 ) * layer;
		}
	}

	// 雷云ID
	if (get_attr(attrtype_enum::thunder_cloud_buff_id) > 0)
	{
		int layer = m_buff_mgr.get_buff_layer_by_tid(get_attr(attrtype_enum::thunder_cloud_buff_id));
		if (layer > 0)
		{
			ret += get_max_hp() * layer * layer * ( get_attr(attrtype_enum::thunder_clond_hurt_ratio) * 0.0001 );
		}
	}

	// 计策ID
	if (get_attr(attrtype_enum::stratagem_buff_id) > 0)
	{
		int layer = target->m_buff_mgr.get_buff_layer_by_tid(get_attr(attrtype_enum::stratagem_buff_id));
		if (layer > 0)
		{
			ret += get_attack() * (get_attr(attrtype_enum::stratagem_buff_ratio) * 0.0001) * layer;
		}
	}

	// 回合内所受伤害转换为直伤
	if (get_attr(attrtype_enum::one_round_all_damage_to_injury) > 0)
	{
		int value = get_count_data().one_round_injured * (get_attr(attrtype_enum::one_round_all_damage_to_injury) * 0.0001);
		if (m_target_count > 0)
			value /= m_target_count;
		
		ret += value;
	}

	// 所有队友伤害转换为直伤
	if (get_attr(attrtype_enum::all_team_damage_to_injury) > 0)
	{
		int value = get_count_data().all_team_damage * (get_attr(attrtype_enum::all_team_damage_to_injury) * 0.0001);
		if (m_target_count > 0)
			value /= m_target_count;

		ret += value;
		//get_count_data().all_team_damage = 0;
	}

	return ret;
}

// 计算攻击力
int fight_hero_t::calc_attack(
	size_t phase,
	SkillEffectTable* skill_conf,
	fight_hero_ptr target,
	proto::common::combat_act_type type,
	int crit_multiple)
{
	if (NULL == target)
	{
		log_error("calc_attack target NULL");
		return 0;
	}
	uint32_t max_hp = get_max_hp();
	if (max_hp == 0)
	{
		log_error("calc_attack max_hp = 0 hero[%lu][%d]", get_uid(), get_tid());
		return 0;
	}
	uint32_t target_max_hp = target->get_max_hp();
	if (target_max_hp == 0)
	{
		log_error("calc_attack target_max_hp = 0 hero[%lu][%d]", target->get_uid(), target->get_tid());
		return 0;
	}

	//	血量损失百分比转化攻击
	int ret = get_attack();
	if (get_attr(attrtype_enum::lose_blood_volume_to_attack) > 0)
	{
		int sub = get_max_hp() - get_hp();
		sub = std::max<int>(sub, 0);
		float ratio = (float)sub / (float)max_hp * 100;
		ret *= (1 + ratio * ( get_attr(attrtype_enum::lose_blood_volume_to_attack) * 0.0001 ));
	}
	// 追加攻击万分比
	if (get_attr(attrtype_enum::self_attack_addition) > 0)
	{
		ret *= (1 + ( get_attr(attrtype_enum::self_attack_addition) * 0.0001 ));
	}

	// 血量伤害加成界限(武器)
	if (get_attr(attrtype_enum::blood_volume_to_attack_limit_weapon) > 0)
	{
		float ratio = (float)target->get_hp() / (float)target_max_hp * 100;
		float need_ratio = get_attr(attrtype_enum::blood_volume_to_attack_ratio_weapon) * 0.0001;
		if (ratio > need_ratio)
		{
			ret *= (1 + get_attr(attrtype_enum::blood_volume_to_attack_limit_weapon) * 0.0001);
		}
	}

	// 血量伤害加成界限(技能)
	if (get_attr(attrtype_enum::blood_volume_to_attack_limit_skill) > 0)
	{
		float ratio = (float)target->get_hp() / (float)target_max_hp * 100;
		float need_ratio = get_attr(attrtype_enum::blood_volume_to_attack_ratio_skill) * 0.0001;
		if (ratio > need_ratio)
		{
			ret *= (1 + get_attr(attrtype_enum::blood_volume_to_attack_limit_skill) * 0.0001);
		}
	}

	// 反击攻击加成
	if (get_attr(attrtype_enum::counter_attack_addition) > 0)
	{
		if (type == proto::common::combat_act_type_counter)
		{
			ret *= (1 + get_attr(attrtype_enum::counter_attack_addition) * 0.0001);
		}
	}

	return ret;
}

// 计算防御力
int fight_hero_t::calc_defend(
	size_t phase,
	SkillEffectTable* skill_conf,
	fight_hero_ptr target,
	proto::common::combat_act_type type,
	int crit_multiple)
{
	
	int ret = target->get_attr(attrtype_enum::def_physical);
	// 血量百分比转换防御
	if (target->get_attr(attrtype_enum::lose_blood_volume_to_defend) > 0)
	{
		int sub = get_max_hp() - get_hp();
		sub = std::max<int>(sub, 0);
		float ratio = (float)sub / (float)get_max_hp() * 100;
		ret *= (1 + ratio * (get_attr(attrtype_enum::lose_blood_volume_to_defend) * 0.0001));
	}
	int remove_def = 0;
	// 是否无视防御
	if (random_util_t::selectByTenTh(get_attr(attrtype_enum::def_ignore_rate)))
	{
		remove_def = target->get_attr(attrtype_enum::def_physical) * (get_attr(attrtype_enum::def_ignore_value) * 0.0001);
	}
	return std::max<int>(ret - remove_def, 0 );
}

// 计算伤害附加百分比
float fight_hero_t::calc_injury_addition_percent(
	size_t phase,
	SkillEffectTable* skill_conf,
	fight_hero_ptr target,
	proto::common::combat_act_type type,
	int crit_multiple)
{
	RestraintTable *p_terry_conf = GET_CONF(RestraintTable, hash_util_t::hash_to_uint32(get_terry(), target->get_terry()));
	int terry_param = 1;
	if (p_terry_conf != NULL)
		terry_param = p_terry_conf->restraint();
	int32_t pvp_add_param = 0;		// pvp增伤万分比
	int32_t pvp_dec_param = 0;		// pvp减伤万分比
	
	get_pvp_param(pvp_add_param, pvp_dec_param, target);


	float ret = 1;
	ret += ( get_attr(attrtype_enum::damage_bonus) - target->get_attr(attrtype_enum::get_damage_reduce) ) * 0.0001;
	
	ret += (pvp_add_param + pvp_dec_param) * 0.0001;
	// 是否单体攻击
	if (is_single_attack(skill_conf))
	{
		// 目标单体攻击加成
		ret += target->get_attr(attrtype_enum::single_attack_behurt_ratio) * 0.0001;
	}
	else
	{
		// 目标群体攻击加成
		ret += target->get_attr(attrtype_enum::range_attack_behurt_ratio) * 0.0001;
	}

	// 对于处于控制状态的额外伤害
	if (target->is_in_control())
	{
		ret += get_attr(attrtype_enum::in_control_attack_addition) * 0.0001;
	}
	
	// 负面状态追加攻击万分比
	if (get_attr(attrtype_enum::target_debuff_attack_addition) > 0)
	{
		ret += target->m_buff_mgr.get_debuff_count() * ( get_attr(attrtype_enum::target_debuff_attack_addition) * 0.0001 );
	}
	// 指定状态追加攻击万分比
	if (get_attr(attrtype_enum::target_status_attack_id) > 0 )
	{
		int buffer_id = get_attr(attrtype_enum::target_status_attack_id);
		if( target->m_buff_mgr.get_count_by_small_type(buffer_id) > 0 )
			ret += ( get_attr(attrtype_enum::target_status_attack_addition) * 0.0001 );
	}

	// 别人行动追加伤害
	ret += (get_attr(attrtype_enum::other_action_addition_hurt) * 0.0001) * get_count_data().other_action_count;
	
	return ret * terry_param * 0.0001;
}

// 计算伤害
int fight_hero_t::calc_injury(
	size_t phase,
	SkillEffectTable* skill_conf,
	fight_hero_ptr target,
	proto::common::combat_act_type type,
	int crit_multiple)
{
	uint32_t damage = 0;

	// 计算真实伤害
	uint32_t final_real_damage = std::max<int>((get_attr(attrtype_enum::true_damage) - target->get_attr(attrtype_enum::true_def)), 0);

	int attack = calc_attack(phase, skill_conf, target, type, crit_multiple);
	int defend = calc_defend(phase, skill_conf, target, type, crit_multiple);
	//total_damage = max?〖（（己方攻击-敌人防御）〗，（己方攻击）  *  0.1）
	damage = std::max<int>(attack - defend, attack * 0.1);

	
	damage += final_real_damage;
	damage = std::max<int>(damage * (skill_conf->hurt_rate(phase) * 0.0001) + skill_conf->hurt(), 1);


	if (m_calc_result.crit)
	{
		damage *= (crit_multiple * 0.0001);
	}

	float percent = calc_injury_addition_percent(phase, skill_conf, target, type, crit_multiple);

	damage = std::max<int>(damage * percent, final_real_damage);
	return damage;
}

void fight_hero_t::do_target_shield_points(fight_hero_ptr target)
{
	uint64_t target_obj_uid = target->get_master();
	fight_obj_ptr p_target_obj = get_combat()->find_obj(target_obj_uid);
	if (NULL == p_target_obj)
	{
		log_error("do_target_shield_points hero[%lu][%d] obj[%lu] not find", get_uid(), get_tid(), target_obj_uid);
		return;
	}
	int target_shield_points = target->get_attr(attrtype_enum::shield_points); // 获取目标的属性62

	if (target_shield_points > 0)
	{
		if (m_calc_result.damage > target_shield_points)
		{
			// 清空目标的护盾值
			m_calc_result.absorb = target_shield_points;
			target->sub_attr(attrtype_enum::shield_points, m_calc_result.absorb);
			m_calc_result.damage -= target_shield_points;
		}
		else
		{
			// 设置护盾值为扣除伤害后的值
			// todo
			m_calc_result.absorb = m_calc_result.damage;
			target->sub_attr(attrtype_enum::shield_points, m_calc_result.absorb);
			m_calc_result.damage = 0;
		}
		// 判断是否盾破了
		if (target->get_attr(attrtype_enum::shield_points) == 0)
		{
			target->m_buff_mgr.remove_buff_by_small_type(target->get_count_data().shield_small_type);
		}
	}

	if (p_target_obj->get_team_shield() > 0 && m_calc_result.damage > 0)
	{
		if (m_calc_result.damage > p_target_obj->get_team_shield())
		{
			m_calc_result.absorb += p_target_obj->get_team_shield();
			p_target_obj->set_team_shield(0);
			m_calc_result.damage -= p_target_obj->get_team_shield();
		}
		else
		{
			m_calc_result.absorb += m_calc_result.damage;
			p_target_obj->set_team_shield(p_target_obj->get_team_shield() - m_calc_result.damage);
			m_calc_result.damage = 0;
		}
		// 盾破了
		if (p_target_obj->get_team_shield() == 0)
		{
			fight_hero_ptr p_buff_owner = get_combat()->find_hero(p_target_obj->get_team_shield_hero_uid());
			if (NULL == p_buff_owner)
			{
				log_error("do_target_shield_points p_buff_owner NULL uid[%lu]", p_target_obj->get_team_shield_hero_uid());
				return;
			}
			p_buff_owner->m_buff_mgr.remove_buff_by_small_type(p_target_obj->get_team_shield_buff_type());
		}
	}

	// 所有队友伤害转换为直伤
	if (m_calc_result.damage > 0 && get_attr(attrtype_enum::all_team_damage_turn) > 0)
	{
		int value = m_calc_result.damage * (get_attr(attrtype_enum::all_team_damage_turn) * 0.0001);
		fight_hero_ptr p_owner = get_combat()->find_hero(p_target_obj->get_team_damage_turn_hero_uid());
		if (NULL == p_owner)
		{
			log_error("do_target_shield_points p_owner NULL uid[%lu]", p_target_obj->get_team_shield_hero_uid());
			return;
		}
		p_owner->get_count_data().all_team_damage_turn += value;
		m_calc_result.damage -= value;
	}

	//add_combat_act(proto::common::combat_act_type_absorb, m_calc_result.absorb, target, shared_from_this());
	FIGHT_LOG("[%lu]target absorb [%d] [%d]", get_fight_uid(), m_calc_result.absorb, target->get_attr(attrtype_enum::shield_points));	
}

uint32_t fight_hero_t::get_attack()
{
	int attack = get_attr(attrtype_enum::ranged_atk);
	if (attack == 0)
	{
		int atk = get_attr(attrtype_enum::atk); // 获取10
		int atk_range = get_attr(attrtype_enum::atk_range); // 获取11
		int max_atk = std::max<int>(atk * (atk_range * 0.0001), 1); // 攻击力*攻击浮动/10000
		int real_atk = random_util_t::randBetween(atk, max_atk);
		attack = real_atk;
	}
	return attack;
}

// 计算反伤
int fight_hero_t::calc_reflect(
	size_t phase,
	SkillEffectTable* skill_conf,
	fight_hero_ptr target,
	proto::common::combat_act_type type,
	int crit_multiple)
{
	// 反伤
	int reflect_skill_value = 0; // 技能基础反伤值, 先空着,以后加配置

	// 根据生命上限反射伤害
	if (target->get_attr(attrtype_enum::reflect) > 0)
	{
		reflect_skill_value += target->get_max_hp() * (target->get_attr(attrtype_enum::reflect) * 0.0001);
	}
	// 根据攻击反射伤害
	if (target->get_attr(attrtype_enum::reflect_by_attack) > 0)
	{
		reflect_skill_value += target->get_attack() * (target->get_attr(attrtype_enum::reflect_by_attack) * 0.0001);
	}

	// 根据伤害计算反射
	if (target->get_attr(attrtype_enum::reflect_by_damage) > 0)
	{
		int reflect = m_calc_result.damage * (target->get_attr(attrtype_enum::reflect_by_damage) * 0.0001);
		if (target->get_attr(attrtype_enum::reflect_limit) > 0)
		{
			if (reflect > (int)target->get_hp())
				reflect = target->get_hp();
		}

		reflect_skill_value += reflect;
	}

	return reflect_skill_value;
}

void fight_hero_t::do_attack_result_notify(proto::common::combat_act_type type, fight_hero_ptr target)
{
	if (m_calc_result.damage == 0 && m_calc_result.absorb == 0)
		return;
	if (type == proto::common::combat_act_type_spurting)
	{
		add_combat_act(proto::common::combat_act_type_spurting, m_calc_result.damage, target, shared_from_this(), target->get_attr(attrtype_enum::hp), m_calc_result.absorb);
	}
	else if (m_calc_result.beheaded)
	{
		add_combat_act(proto::common::combat_act_type_beheaded, m_calc_result.damage, target, shared_from_this(), target->get_attr(attrtype_enum::hp), m_calc_result.absorb);
	}
	else if (m_calc_result.crit)
	{
		add_combat_act(proto::common::combat_act_type_crit, m_calc_result.damage, target, shared_from_this(), target->get_attr(attrtype_enum::hp), m_calc_result.absorb);
	}
	else
	{
		add_combat_act(proto::common::combat_act_type_damage, m_calc_result.damage, target, shared_from_this(), target->get_attr(attrtype_enum::hp), m_calc_result.absorb);
	}
}

void fight_hero_t::do_leech(SkillEffectTable* skill_conf, fight_hero_ptr target)
{
	if (m_calc_result.leech == 0)
		return;
	
	// 吸血加血操作
	add_hp(m_calc_result.leech);
	//add_combat_act(proto::common::combat_act_type_leech, m_calc_result.leech, get_uid(), get_attr(attrtype_enum::hp));
	add_combat_act(proto::common::combat_act_type_leech, m_calc_result.leech, shared_from_this(), shared_from_this(), get_attr(attrtype_enum::hp));
	FIGHT_LOG("[%lu]attacker leech [%d] [%d]", get_fight_uid(), m_calc_result.leech, get_attr(attrtype_enum::hp));
	
}

int fight_hero_t::calc_leech(SkillEffectTable* skill_conf, fight_hero_ptr target)
{
	// 吸血计算
	int leech_skill_value = 0; // 技能基础吸血值, 先空着,以后加配置

	if (get_attr(attrtype_enum::leech) > 0)
	{
		return std::max<int>((leech_skill_value + m_calc_result.damage * (get_attr(attrtype_enum::leech) * 0.0001) * ((10000 + get_attr(attrtype_enum::heal_bonus)) * 0.0001)), 1);
	}
	return leech_skill_value;
}

void fight_hero_t::do_reflect(SkillEffectTable* skill_conf, fight_hero_ptr target)
{
	if (m_calc_result.reflect > 0)
	{
		// 执行反伤扣自己的血操作
		on_be_attack(m_calc_result.reflect, target, 0);
		//add_combat_act(proto::common::combat_act_type_reflect, m_calc_result.reflect, get_uid(), get_attr(attrtype_enum::hp));
		add_combat_act(proto::common::combat_act_type_reflect, m_calc_result.reflect, shared_from_this(), target, get_attr(attrtype_enum::hp));
		FIGHT_LOG("[%lu]target reflect [%d] [%d]", get_fight_uid(), m_calc_result.reflect, get_attr(attrtype_enum::hp));
	}
}

bool fight_hero_t::do_skill_hurt_imp(
	size_t phase,
	SkillEffectTable* skill_conf,
	fight_hero_ptr target,
	proto::common::combat_act_type type,
	int crit_multiple,
	bool & combo)
{
	bool counter = false;

	m_calc_result.damage = calc_injury(phase, skill_conf, target, type, crit_multiple);

	m_calc_result.damage += calc_direct_injury(phase, skill_conf, target, type, crit_multiple);

	m_calc_result.damage = calc_limit_injury(phase, skill_conf, target, type, crit_multiple);

	// 溅射伤害
	if (type == proto::common::combat_act_type_spurting)
	{
		m_calc_result.damage = m_calc_result.damage * (get_attr(attrtype_enum::spurting) * 0.0001);
	}

	// 吸血计算
	m_calc_result.leech = calc_leech(skill_conf, target);
	

	// 反伤
	m_calc_result.reflect = calc_reflect(phase, skill_conf, target, type, crit_multiple);

	// 吸收前
	target->get_script_proxy()->before_absorb(get_script_proxy(), skill_conf->id(), m_calc_result.damage);

	do_target_shield_points(target);

	// 执行伤害扣血操作
	// 通知目标的脚本处理
	do_skill_buff(NONE_PHASE, event_on_hit_target, skill_event_target_type_all, skill_conf, target);
	do_team_script_event(event_on_hit_target);	
	
	target->on_be_attack(m_calc_result.damage, shared_from_this(), skill_conf->id());
	do_attack_result_notify(type, target);

	// 先给客户端发完本次伤害结果 再触发暴击事件
	if (m_calc_result.crit)
	{
		do_skill_buff(NONE_PHASE, event_on_critical_strike, skill_event_target_type_all, skill_conf, target);
		do_team_script_event(event_on_critical_strike);
		target->do_team_script_event(event_on_becritical_strike);
	}

	FIGHT_LOG("[%lu]target damage [%d] [%d]", get_fight_uid(), m_calc_result.damage, target->get_attr(attrtype_enum::hp));


	do_leech(skill_conf, target);

	do_reflect(skill_conf, target);

	// 判断反击
	int target_counter_rate = target->get_attr(attrtype_enum::counter_rate); // 目标的属性73
	counter |= random_util_t::selectByTenTh(target_counter_rate);

	// 判断连击
	combo |= check_combo(skill_conf,target);
	return counter;
}

bool fight_hero_t::check_combo(SkillEffectTable* skill_conf, fight_hero_ptr target)
{
	// 判断连击
	int combo_rate = get_attr(attrtype_enum::combo_rate); // 自己的属性71
	if (random_util_t::selectByTenTh(combo_rate))
		return true;
	// 连击BUFF
	if (get_attr(attrtype_enum::combo_buffer_id) > 0)
	{
		uint32_t layer = m_buff_mgr.get_buff_layer_by_tid(get_attr(attrtype_enum::combo_buffer_id));
		if (get_count_data().combo_count < layer)
			return true;
	}
	return false;
}

bool fight_hero_t::do_skill_hurt(
	size_t phase,
	SkillEffectTable* skill_conf,
	fight_hero_ptr target,
	proto::common::combat_act_type type,
	bool & combo)
{
	if (skill_conf->is_ignore_damage(phase) == true)
		return false;
	// 计算生成本次攻击力
	int real_atk = 0;
	int atk = get_attr(attrtype_enum::atk); // 获取10
	int atk_range = get_attr(attrtype_enum::atk_range); // 获取11
	int max_atk = std::max<int>(atk * ( atk_range * 0.0001 ), 1); // 攻击力*攻击浮动/10000

	real_atk = random_util_t::randBetween(atk, max_atk);
	if (-1 == real_atk)
	{
		log_error("min_count[%d] max_count[%d] error!", atk, max_atk);
		return false;
	}

	// 要把实际攻击设置到武将身上 ranged_atk 24

	set_attr(attrtype_enum::ranged_atk, real_atk);
	FIGHT_LOG("[%lu]real_atk [%u][%d]", get_fight_uid(), (uint32_t)attrtype_enum::ranged_atk, (int)real_atk);

	int crit_multiple = 1;
	int heal_crit_multiple = 1;
	// 暴击相关操作
	do_crit(target, crit_multiple, heal_crit_multiple);		

	if (skill_conf->release_ai() == skill_release_ai_heal) // 计算治疗量
	{
		return do_skill_heal(phase, skill_conf, target, heal_crit_multiple);
	}
	else
	{
		return do_skill_hurt_imp(phase, skill_conf, target, type, crit_multiple, combo);
	}
}

uint32_t fight_hero_t::athanasia_count() const
{
	return get_count_data().athanasia_count;
}

void fight_hero_t::athanasia_count(uint32_t value)
{
	get_count_data().athanasia_count = value;
}

void fight_hero_t::change_model(uint32_t change_type, uint32_t change_phase, uint32_t change_skill_level)
{
	// 回收被动效果
//	recovery_special_buff();
//	clear_passive_buff();
	m_change_model_phase = change_phase;

	skills_vec skill_list;
	std::vector<uint32_t> new_skill_level;

	if (get_type() == proto::common::SCENEOBJECT_USER)
	{
		Hero* hero_conf = GET_CONF(Hero, get_tid());
		if (hero_conf == NULL)
		{
	 		log_error("Hero [%u] not exists", get_tid());
	 		return;
		}
		skill_list = hero_conf->get_change_model_phase_skills(m_change_model_phase);
	}
	else
	{
		MonsterHero* pre_hero_conf = GET_CONF(MonsterHero, get_tid());
		if (pre_hero_conf == NULL)
		{
			log_error("Hero [%u] not exists", get_tid());
			return;
		}

		skill_list = pre_hero_conf->get_change_model_phase_skills(m_change_model_phase);
	}

	if (skill_list.size() == 0)
	{
		log_error("change_model skill_list_size = 0 change_model_phase[%d]", m_change_model_phase);
		return;
	}

	// change_type = 1:通过参数指定等级 0:新技能对应技能的等级
	SkillEffectTable *p_skill_conf = NULL;
	for (uint32_t i = 0; i < skill_list.size(); ++i)
	{
		if (change_type == 1)
		{
			new_skill_level.push_back(change_skill_level);
		}
		else
		{
			p_skill_conf = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(skill_list[i], 1));
			if (NULL == p_skill_conf)
			{
				log_error("not find skill[%d]", skill_list[i]);
				return;
			}
			uint32_t skill_level = get_skill_level(p_skill_conf->change_mod_pre_skill());

			new_skill_level.push_back(skill_level);
		}
	}

	std::map<uint32_t, skill_data>::iterator iter = m_skills.begin();
	for (; iter != m_skills.end();)
	{
		m_change_model_old_skills.insert(std::make_pair(iter->first, iter->second));
		if (iter->second.type == proto::common::skill_type_equip_skill)
		{
			++iter;
			continue;
		}
		else
		{
			iter = m_skills.erase(iter);
		}
	}

	for (uint32_t i = 0; i < skill_list.size(); ++i)
	{
		add_skill(skill_list[i], new_skill_level[i], 0, true);
	}

	add_combat_act(proto::common::combat_act_type_change_model, m_change_model_phase, shared_from_this(), 
						  shared_from_this(), change_type, change_skill_level);
	//m_hero_data.mutable_base_attr()->set_id(hero_tid);

	// 被动效果
	//distribute_special_buff();
	//activation_passive_buff();
}

void fight_hero_t::restore_model()
{
	m_skills.clear();
	m_change_model_phase = 0;
	std::map<uint32_t, skill_data>::iterator iter = m_change_model_old_skills.begin();
	for (; iter != m_change_model_old_skills.end(); ++iter)
	{
		m_skills.insert(std::make_pair(iter->first, iter->second));
	}
	add_combat_act(proto::common::combat_act_type_change_model, 0, shared_from_this(), shared_from_this());
}

bool fight_hero_t::is_single_attack(SkillEffectTable* skill_conf)
{
	if (skill_conf->target_type() == skill_attack_target_type_select)
		return true;
	return false;
}

bool fight_hero_t::do_dodge(SkillEffectTable* skill_conf,fight_hero_ptr target)
{
	int anti_dogge = get_attr(attrtype_enum::anti_dodge);		// 抗闪避率
	int target_dodge = target->get_attr(attrtype_enum::dodge); // 获取 目标的闪避值70
	int result = target_dodge - target_dodge;
	if (result < 0)
		result = 0;
	if (random_util_t::selectByTenTh(result))
	{
		m_calc_result.dodge = true;
		FIGHT_LOG("[%lu]target dodge", get_fight_uid());
		// 闪避成功
		//add_combat_act(proto::common::combat_act_type_dodge, 0, target->get_uid());
		add_combat_act(proto::common::combat_act_type_dodge, 1, target, shared_from_this());
		return true;
	}
	if (get_attr(attrtype_enum::dodge_single_target_damage) > 0)
	{
		if (skill_conf->attack_target() == skill_target_alive_enemy
			|| skill_conf->attack_target() == skill_target_enemy)
		{
			// 闪避成功
			if (random_util_t::selectByTenTh(get_attr(attrtype_enum::dodge_single_target_damage)))
			{
				m_calc_result.dodge = true;
				FIGHT_LOG("[%lu]target dodge", get_fight_uid());
				// 闪避成功
				//add_combat_act(proto::common::combat_act_type_dodge, 0, target->get_uid());
				add_combat_act(proto::common::combat_act_type_dodge, 1, target, shared_from_this());
				return true;
			}
		}
		
	}
	return false;
}

uint32_t fight_hero_t::add_buff_after_damage_count() const
{
	return get_count_data().add_buff_after_damage_count;
}

void fight_hero_t::add_buff_after_damage_count(uint32_t value)
{
	get_count_data().add_buff_after_damage_count = value;
}

bool fight_hero_t::do_immuno(SkillEffectTable* skill_conf, fight_hero_ptr target)
{
	if (target->get_attr(attrtype_enum::invincibility))
		return true;
	// 免疫单体攻击
	if (target->get_attr(attrtype_enum::immuno_single_attack))
	{
		if (is_single_attack(skill_conf) == true)
			return true;
	}
	uint32_t hero_immuno_attr = get_hero_immuno_attr();
	int32_t target_immuno = target->get_attr(hero_immuno_attr);
	if (random_util_t::selectByTenTh(target_immuno))
	{
		m_calc_result.immuno = true;
		FIGHT_LOG("[%lu]target immuno", get_fight_uid());
		// 免疫成功
		add_combat_act(proto::common::combat_act_type_immuno, 1, target, shared_from_this());
		return true;
	}
	return false;
}

bool fight_hero_t::do_attack_target(
	fight_hero_ptr target, 
	SkillEffectTable* skill_conf,
	bool& trigger_counter,
	bool& trigger_combo,
	size_t& phase,
	bool trigger_by_buff,
	proto::common::combat_act_type type)
{
	if (check_do_attack_target(target, skill_conf, phase, trigger_by_buff, type) == false)
		return false;
	
	FIGHT_LOG("[%lu]attack target: camp[%u] pos[%u] tid[%u]", get_fight_uid(), target->get_camp(), target->get_pos(), target->get_tid());
	
	set_attack_step(proto::common::combat_act_step_before_attack_target);

	do_skill_buff(NONE_PHASE, event_on_attack_begin, skill_event_target_type_target, skill_conf, target);
	target->do_team_script_event(event_on_behurt_begin);

	m_cur_target = target;
	target->set_cur_attacker(shared_from_this());
	
	bool combo = false;
	bool counter = false;
	
	uint32_t damage_phase = skill_conf->hurt_rate().size();
	size_t init_phase = phase;

	for (size_t i = init_phase; i < damage_phase; ++i)
	{
		m_calc_result.reset();
		phase = i;
		m_calc_result.phase = i;
		m_calc_result.act_type = type;
		m_calc_result.skill = skill_conf->id();
		FIGHT_LOG("[%lu]target phase [%u]", get_fight_uid(), phase);

		set_attack_step(proto::common::combat_act_step_before_attack_phase);

		// 每段伤害前的buff处理
		do_skill_buff(i, event_on_attack_phase_begin, skill_event_target_type_all, skill_conf, target);
		do_team_script_event(event_on_attack_phase_begin);		
		target->do_team_script_event(event_on_behurt_phase_begin);

		
		
		// Step(2) 判断本次是否闪避
		if (do_dodge(skill_conf,target))
		{
			// Nothing TODO
		}
		// Step(3) 判断本次是否免疫
		else if (do_immuno(skill_conf,target))
		{
			// Nothing TODO
		}
		else
		{
			// 技能伤害计算
			counter |= do_skill_hurt(i, skill_conf, target, type, combo);
		}
		
		// 一段伤害结束
		set_attack_step(proto::common::combat_act_step_after_attack_phase);

		// 每段伤害后的buff处理
		do_skill_buff(i, event_on_attack_phase_end, skill_event_target_type_all, skill_conf, target);
		// 触发事件
		do_team_script_event(event_on_attack_phase_end);
		target->do_team_script_event(event_on_behurt_phase_end);

		m_buff_element_moment_mgr.clear_all_element();
		target->m_buff_element_moment_mgr.clear_all_element();
		// 判断自己是否死亡
		// todo
		if (is_dead())
		{
			FIGHT_LOG("[%lu]attacker dead", get_fight_uid());
		}

		// 判断对方是否死亡
		//todo
		if (target->is_dead())
		{
			FIGHT_LOG("[%lu]target dead", get_fight_uid());
			if (skill_conf->refind_target())
			{
				break;
			}
		}

		
	}

	if (get_cur_target() != NULL)
		get_cur_target()->set_bear_damage(0, NULL);

	if (type == proto::common::combat_act_type_counter)
	{
		// 反击不会产生反击和连击
		// 返回
		return true;
	}

	// 判断对方是否死亡,死亡则暂不触发反击
	//todo
	if (!target->is_dead())
	{
		int max_counter_count = target->get_attr(attrtype_enum::counter_max); // 目标的属性74
		if (counter && target->counter_count() < (uint32_t)max_counter_count)
		{
			target->counter_count(target->counter_count() + 1);
			// 反击,执行反击技能
			//todo
			trigger_counter = true;
			FIGHT_LOG("[%lu]trigger counter", get_fight_uid());
		}
	}

	// 判断自己是否死亡,死亡则暂不触发连击
	// todo
	if (!is_dead())
	{
		int max_combo_count = get_max_combo_count(); // 自己的属性72
		if (combo && target->combo_count() < (uint32_t)max_combo_count) // 触发连击
		{
			target->combo_count(target->combo_count() + 1);
			// 连击,执行本次相同的技能
			// todo
			trigger_combo = true;
			FIGHT_LOG("[%lu]trigger combo", get_fight_uid());
		}
	}

	return true;
}

int fight_hero_t::get_max_combo_count()
{
	int max_count = 0;
	// 连击BUFF
	if (get_attr(attrtype_enum::combo_buffer_id) > 0)
	{
		max_count = m_buff_mgr.get_buff_layer_by_tid(get_attr(attrtype_enum::combo_buffer_id));
	}
	if (max_count > (int)get_attr(attrtype_enum::combo_max))
		return max_count;
	return (int)get_attr(attrtype_enum::combo_max);
}

bool fight_hero_t::do_after_attack_target(fight_hero_ptr target, SkillEffectTable* skill_conf, bool trigger_by_buff)
{
	// 一个目标结束
	if (NULL == target)
	{
		log_error("do_after_attack target is null");
		return false;
	}

	if (NULL == skill_conf)
	{
		log_error("do_after_attack skill_conf is null");
		return false;
	}

	if (NULL == m_script_proxy)
	{
		log_error("m_script_proxy is null");
		return false;
	}

	if (NULL == target->get_script_proxy())
	{
		log_error("target m_script_proxy is null");
		return false;
	}
	
	set_attack_step(proto::common::combat_act_step_after_attack_target);

	// 判断自己是否死亡
	// todo
	if (is_dead())
	{
		FIGHT_LOG("[%lu]attacker dead", get_fight_uid());
		on_dead(target, skill_conf);
	}

	// 判断对方是否死亡
	//todo
	if (!target->has_dead() && target->is_dead())
	{
		FIGHT_LOG("[%lu]target[%d] dead", get_fight_uid(), target->get_tid());
		target->on_dead(shared_from_this(), skill_conf);
	}
	// 分摊伤害会导致队友也死亡 所以要判断队友
	combat_ptr p_combat = get_combat();
	if (p_combat != NULL)
	{
		std::vector<fight_hero_ptr> camp_heroes;
		p_combat->get_camp_heros(camp_heroes, target->get_camp(), target->get_pos());
		for (auto hero : camp_heroes)
		{
			if (NULL == hero)
			{
				continue;
			}
			if (!hero->has_dead() && hero->is_dead())
			{
				FIGHT_LOG("[%lu]hero[%d] dead", hero->get_fight_uid(), hero->get_tid());
				hero->on_dead(shared_from_this(), skill_conf);
			}
		}
	}

	
	//m_script_proxy->after_attack_target(target->get_script_proxy(), skill_conf->id());
	//target->get_script_proxy()->after_be_attack(get_script_proxyon_attack_end(), skill_conf->id());
	target->do_team_script_event(event_on_behurt_end);
// 	if (!trigger_by_buff)
// 	{
// 		target->m_buff_mgr.after_be_attack();
// 	}

	m_attack_targets.push_back(target);

	return true;
}

void fight_hero_t::get_target_by_buff_select_target(fight_hero_ptr select, int type, int rand_count, std::vector<fight_hero_ptr>& out)
{
	switch( type )
	{
		case skill_buff_target_type_target:
		{
			if( select != NULL )
				out.push_back(select);
		}
		break;
		case skill_buff_target_type_self:
		{
			out.push_back(shared_from_this());
		}
		break;
		case skill_buff_target_type_friend:
		{
			get_combat()->get_camp_heros(out, get_camp());
		}
		break;
		case skill_buff_target_type_enemy:
		{
			get_combat()->get_camp_heros(out, get_enemy_camp());
		}
		break;
		case skill_buff_target_type_friend_rand:
		{
			std::vector<fight_hero_ptr> friends;
			fight_hero_ptr p_fight_hero = fight_hero_ptr();
			get_combat()->get_camp_heros(friends, get_camp(), get_pos());
			std::random_shuffle(friends.begin(), friends.end());
			for (uint32_t j = 0; j < friends.size(); ++j)
			{
				p_fight_hero = friends[j];
				if (NULL == p_fight_hero)
				{
					log_error("p_fight_hero NULL pos[%d]", j);
					continue;
				}
				if(p_fight_hero->is_dead())
					continue;
				out.push_back(p_fight_hero);
				if ((int)out.size() >= rand_count)
				{
					break;
				}
			}
		}
		break;
		case skill_buff_target_type_enemy_rand:
		{
			std::vector<fight_hero_ptr> enemys;
			fight_hero_ptr p_fight_hero = fight_hero_ptr();
			get_combat()->get_camp_heros(enemys, get_enemy_camp());
			std::random_shuffle(enemys.begin(), enemys.end());
			for (uint32_t j = 0; j < enemys.size(); ++j)
			{
				p_fight_hero = enemys[j];
				if (NULL == p_fight_hero)
				{
					log_error("p_fight_hero NULL pos[%d]", j);
					continue;
				}
				if (p_fight_hero->is_dead())
					continue;
				out.push_back(p_fight_hero);
				if ((int)out.size() >= rand_count)
				{
					break;
				}
			}
		}
		break;
		case skill_buff_target_type_friend_min_hp:
		{
			fight_hero_ptr select = get_combat()->select_min_hp_target(get_camp());
			if( select != NULL )
				out.push_back(select);
		}
		break;
		case skill_buff_target_type_enemy_min_hp:
		{
			fight_hero_ptr select = get_combat()->select_min_hp_target(get_enemy_camp());
			if (select != NULL)
				out.push_back(select);
		}
		break;
		case skill_buff_target_type_attack_me:
		{
			fight_hero_ptr attacker = get_cur_attacker();
			if (attacker != NULL)
				out.push_back(attacker);
			else if (select != NULL)
				out.push_back(attacker);
		}
		break;
	}
}

bool fight_hero_t::do_skill_buff(
	size_t phase,
	FIGHT_EVENT when,
	skill_event_target_type to,
	SkillEffectTable* skill_conf,
	fight_hero_ptr target /*= NULL*/)
{
	if (NULL == get_combat())
	{
		log_error("get_combat is null");
		return false;
	}

	if (NULL == skill_conf)
	{
		log_error("skill_conf is null");
		return false;
	}

	// 驱散BUFF
	const skill_dispel_buff_vec& dispel_buffs = skill_conf->get_dispel_buffs();
	for (uint32_t i = 0; i < dispel_buffs.size(); ++i)
	{
		if (dispel_buffs[i].second != when)
			continue;
		if (to == skill_event_target_type_target && dispel_buffs[i].third != skill_buff_target_type_target)
			continue;
		if (to == skill_event_target_type_except_target && dispel_buffs[i].third == skill_buff_target_type_target)
			continue;

		std::vector<fight_hero_ptr> out;
		get_target_by_buff_select_target(target, dispel_buffs[i].third, dispel_buffs[i].fourth, out);
		for (auto hero : out)
		{
			if (NULL == hero)
			{
				continue;
			}

			do_dispel_skill_buff(hero, shared_from_this(), skill_conf->id(), dispel_buffs[i].first, dispel_buffs[i].fifth, dispel_buffs[i].sixth);
		}
	}

	// 添加BUFF
	const skill_add_buff_vec& add_buffs = skill_conf->get_add_buffs();
	for (uint32_t i = 0; i < add_buffs.size(); ++i)
	{
		if (add_buffs[i].second != when)
			continue;
		if(to == skill_event_target_type_target && add_buffs[i].third != skill_buff_target_type_target)
			continue;
		if(to == skill_event_target_type_except_target && add_buffs[i].third == skill_buff_target_type_target)
			continue;

		std::vector<fight_hero_ptr> out;
		get_target_by_buff_select_target(target, add_buffs[i].third, 10, out);
		for (auto hero : out)
		{
			if (NULL == hero)
			{
				continue;
			}

			do_add_skill_buff(hero, shared_from_this(), skill_conf->id(), add_buffs[i].fourth, add_buffs[i].first, add_buffs[i].fifth);
		}
	}

	return true;
}

bool fight_hero_t::check_target_taunt(fight_hero_ptr target)
{
	if (target->get_attr(attrtype_enum::taunt) == 0)
		return false;
	if (target->get_special_target() == NULL)
		return false;
	return target->get_special_target()->get_uid() == get_uid();
}

bool fight_hero_t::do_effect_hit_result(fight_hero_ptr target, uint32_t buff_add_rate)
{
	int effect_hit_rate = get_attr(attrtype_enum::effect_hit_rate);
	int effect_dodge_rate = target->get_attr(attrtype_enum::effect_dodge_rate);
	// 第一次计算判定是否命中
	uint32_t add_rate = buff_add_rate * ((10000.f + effect_hit_rate) / (10000.f /*+ effect_dodge_rate*/));
	if (!random_util_t::selectByTenTh(add_rate))
	{
		return false;
	}
	// 如果没有效果抵抗 只计算一次就行
	if (effect_dodge_rate == 0)
		return true;
	// 第二次计算判定是否抵抗
	add_rate = buff_add_rate * ((10000.f + effect_hit_rate) / (10000.f + effect_dodge_rate) );
	if (!random_util_t::selectByTenTh(add_rate))
	{
		// 未命中 飘字
		target->add_combat_act(proto::common::combat_act_type_buff_oppose, 1, target, shared_from_this());
		target->do_team_script_event(event_on_resistance);
		return false;
	}
	return true;
}

bool fight_hero_t::do_add_skill_buff(fight_hero_ptr p_hero, fight_hero_ptr p_attacker, uint32_t skill_id, 
									 bool b_use_rate, uint32_t buff_add_rate, const buff_info_vec& buffs)
{
	if (NULL == p_hero)
	{
		log_error("fight_hero_t p_hero NULL");
		return false;
	}
	if (NULL == p_attacker)
	{
		log_error("fight_hero_t p_attacker NULL");
		return false;
	}

	uint32_t add_rate = 0;
	auto skill_effect_max_conf = GET_CONF(Comprehensive, comprehensive_common::skil_effect_max);
	auto skill_effect_min_conf = GET_CONF(Comprehensive, comprehensive_common::skil_effect_min);
	int skill_effect_max = GET_COMPREHENSIVE_VALUE_1(skill_effect_max_conf);
	int skill_effect_min = GET_COMPREHENSIVE_VALUE_1(skill_effect_min_conf);

	int effect_hit_rate = get_attr(attrtype_enum::effect_hit_rate);
	int effect_dodge_rate = p_hero->get_attr(attrtype_enum::effect_dodge_rate);

	BuffTable *p_buff_conf = NULL;

	for (uint32_t i = 0; i < buffs.size(); ++i)
	{
		p_buff_conf = GET_CONF(BuffTable, buffs[i].first);
		if (NULL == p_buff_conf)
		{
			log_error("fight_hero_t p_buff_conf NULL id[%d]", buffs[i].first);
			continue;
		}

		if (b_use_rate)
		{
			if (p_buff_conf->buff_formula() == buff_formula_type_debuff)
			{
				add_rate = std::max<uint32_t>(std::min<uint32_t>(buff_add_rate * ((10000.f + effect_hit_rate) / (10000.f)), skill_effect_max), skill_effect_min);
				// 第一次计算未命中
				if (!random_util_t::selectByTenTh(add_rate))
				{
					continue;
				}
				add_rate = std::max<uint32_t>(std::min<uint32_t>(buff_add_rate * ((10000.f + effect_hit_rate) / (10000.f + effect_dodge_rate)), skill_effect_max), skill_effect_min);
			}
			else
			{
				add_rate = std::max<uint32_t>(std::min<uint32_t>(buff_add_rate * ((10000.f + effect_hit_rate) / 10000.f), skill_effect_max), skill_effect_min);
			}
		}
		else
		{
			add_rate = buff_add_rate;
		}

		if (!random_util_t::selectByTenTh(add_rate))
		{
			// 未命中 飘字
			if (p_buff_conf->buff_formula() == buff_formula_type_debuff)
			{
				p_hero->add_combat_act(proto::common::combat_act_type_buff_oppose, 1, p_hero, p_attacker);
				// 计算效果抵抗并且效果抵抗不为0
				if (b_use_rate)
				{
					p_hero->do_team_script_event(event_on_resistance);
				}
			}
			continue;
		}

		p_hero->m_buff_mgr.add_buff(buffs[i].first, p_attacker->get_uid(), skill_id, buffs[i].second);
		FIGHT_LOG("[%lu] add_buff hero[%lu:%d] buff[%d:%d]", get_fight_uid(), p_hero->get_uid(), p_hero->get_tid(), buffs[i].first, buffs[i].second);
	}
	return true;
}

bool fight_hero_t::do_dispel_skill_buff(fight_hero_ptr p_hero, fight_hero_ptr p_attacker, uint32_t skill_id, uint32_t buff_dispel_rate, uint32_t buff_dispel_num, const buff_dispel_info_vec& buffs)
{
	if (NULL == p_hero)
	{
		log_error("fight_hero_t p_hero NULL");
		return false;
	}
	if (NULL == p_attacker)
	{
		log_error("fight_hero_t p_attacker NULL");
		return false;
	}

	if (!random_util_t::selectByTenTh(buff_dispel_rate))
	{
		return false;
	}
		
	p_hero->m_buff_mgr.dispel_buff(buffs, buff_dispel_num, p_attacker);	

	return true;
}

int32_t fight_hero_t::get_default_skill()
{
	for (auto pair : m_skills)
	{
		skill_data &data = pair.second;
		if(data.type == proto::common::skill_type_atk)
			return data.skill_id;
	}
	return -1;
}

int32_t fight_hero_t::get_counter_skill()
{
	return get_default_skill();
}

SkillEffectTable* fight_hero_t::get_skill_effect(uint32_t skill_id)
{
	uint32_t skill_level = 1;
	std::map<uint32_t, skill_data>::iterator iter = m_skills.find(skill_id);
	if (iter != m_skills.end())
		skill_level = iter->second.skill_level;
	return GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(skill_id, skill_level));
}

bool fight_hero_t::find_target(SkillEffectTable* skill_conf, std::vector<fight_hero_ptr>& targets, std::vector<fight_hero_ptr>& spurting_heroes, fight_hero_ptr select)
{
	if (get_combat() == NULL)
	{
		log_error("find_target m_combat is null");
		return false;
	}

	if (NULL == skill_conf)
	{
		log_error("find_target skill_conf is null");
		return false;
	}

	if (NULL == m_script_proxy)
	{
		log_error("m_script_proxy is null");
		return false;
	}

// 	m_cur_targets.clear();
// 	m_script_proxy->find_target(skill_conf->id());
// 
// 	targets.assign(m_cur_targets.begin(), m_cur_targets.end());
	if (targets.empty())
	{
		if (!get_combat()->find_target(shared_from_this(), skill_conf, targets, spurting_heroes, select))
		{
			log_error("find_target targets fail");
			return false;
		}
	}

	if (targets.empty())
	{
		log_error("targets is empty");
		return false;
	}

	return true;
}

combat_ptr fight_hero_t::get_combat()
{
	return m_combat.lock();
}


uint64_t fight_hero_t::get_fight_uid()
{
	combat_ptr combat = get_combat();
	if (NULL != combat)
	{
		return combat->get_fight_uid();
	}

	return 0;
}

bool fight_hero_t::is_dead()
{
	return (get_attr(attrtype_enum::hp) <= 0 && get_attr(attrtype_enum::soul_condition) == 0);
}

bool fight_hero_t::has_dead()
{
	return m_dead_flag;
}

bool fight_hero_t::is_immuno()
{
	return get_attr(attrtype_enum::immuno) > 0;
}

bool fight_hero_t::is_auto_fight()
{
	if (NULL == get_combat())
	{
		log_error("get_combat is null");
		return false;
	}

	fight_obj_ptr obj = get_combat()->find_obj(get_master());
	if (NULL == obj)
	{
		log_error("obj is null");
		return false;
	}

	return obj->is_auto_fight();
}

void fight_hero_t::change_auto_fight(uint32_t auto_fight_mode)
{
	m_auto_fight_mode = auto_fight_mode;
}

uint32_t fight_hero_t::get_auto_fight_mode()
{
	return m_auto_fight_mode;
}

uint32_t fight_hero_t::get_speed()
{
	return get_attr(attrtype_enum::speed) + get_count_data().in_control_speed;
}

int32_t fight_hero_t::get_att_speed()
{
	return m_att_speed;
}

int32_t fight_hero_t::get_order_att_speed()
{
	return m_order_att_speed;
}

void fight_hero_t::set_att_speed(int32_t att_speed)
{
	combat_ptr p_combat = get_combat();
	if (NULL == p_combat)
	{
		log_error("fight_hero_t p_combat NULL");
		return;
	}
	if (att_speed < 0)
		set_att_order_speed(0);
	else
		set_att_order_speed(att_speed);

	int32_t max_att_speed = p_combat->get_max_att_speed();
	int32_t final_att_speed = att_speed;
	if (att_speed > max_att_speed)
		final_att_speed = max_att_speed;
	else if (att_speed < 0)
		final_att_speed = 0;
	m_att_speed = final_att_speed;
}

int32_t fight_hero_t::get_att_more_speed()
{
	return m_att_more_speed;
}

void fight_hero_t::set_att_more_speed(int32_t value)
{
	combat_ptr p_combat = get_combat();
	if (NULL == p_combat)
	{
		log_error("fight_hero_t p_combat NULL");
		return;
	}
	int32_t max_att_speed = p_combat->get_max_att_speed();
	int32_t final_att_speed = value;
	if (value > max_att_speed)
		final_att_speed = max_att_speed;
	else if (value < 0)
		final_att_speed = 0;
	m_att_more_speed = final_att_speed;
}

void fight_hero_t::set_att_order_speed(int32_t value)
{
	m_order_att_speed = value;
}

uint32_t fight_hero_t::get_pos()
{
	return m_hero_data.fight_attr().pos();
}

uint64_t fight_hero_t::get_master()
{
	return string_util_t::string_to_uint64(m_hero_data.fight_attr().master());
}

uint32_t fight_hero_t::get_camp()
{
	return m_hero_data.fight_attr().camp();
}

uint32_t fight_hero_t::get_enemy_camp()
{
	if (get_camp() == proto::common::combat_camp_1)
	{
		return proto::common::combat_camp_2;
	}
	else
	{
		return proto::common::combat_camp_1;
	}
}

uint64_t fight_hero_t::get_uid()
{
	return string_util_t::string_to_uint64(m_hero_data.base_attr().fight_hero_uid());
}

uint64_t fight_hero_t::get_unique_id()
{
	return string_util_t::string_to_uint64(m_hero_data.base_attr().unique_id());
}

const std::string& fight_hero_t::get_str_uid()
{
	return m_hero_data.base_attr().fight_hero_uid();
}

uint32_t fight_hero_t::get_tid()
{
	return m_hero_data.base_attr().id();
}

uint32_t fight_hero_t::get_wakeup()
{
	return m_hero_data.base_attr().wakeup();
}

uint32_t fight_hero_t::get_plugin()
{
	return m_hero_data.base_attr().current_plugin();
}

uint32_t fight_hero_t::get_script_id()
{
	if (m_hero_data.base_attr().type() == hero_type_monster)
	{
		MonsterHero* monster_conf = GET_CONF(MonsterHero, get_tid());
		if (NULL == monster_conf)
		{
			log_error("monster_conf is null");
			return 0;
		}

		return monster_conf->script_id();
	}
	else
	{
		Hero* hero_conf = GET_CONF(Hero, get_tid());
		if (hero_conf == NULL)
		{
			log_error("Hero [%u] not exists", get_tid());
			return 0;
		}

		if (hero_conf->script_id() == 0)
		{
			return get_tid();
		}
		else
		{
			return hero_conf->script_id();
		}
	}
}

bool fight_hero_t::hero_speed_sort(const fight_hero_ptr& h1, const fight_hero_ptr& h2)
{
	if (NULL == h1 || NULL == h2)
	{
		return false;
	}
	
	bool b_result = false;

	if (h1->get_order_att_speed() == h2->get_order_att_speed())
	{
		if (h1->get_tid() == h2->get_tid())
		{
			// 都是玩家 比较玩家ID 小的在前 大的在后
			if (h1->get_type() == h2->get_type() && h1->get_type() != hero_type_monster)
			{
				b_result = h1->get_master() < h2->get_master();
			}
			else
			{
				// 玩家在前 怪物在后
				if (h1->get_type() != hero_type_monster)
				{
					b_result = true;
				}
				else
				{
					if (h1->get_master() == h2->get_master())
					{
						b_result = h1->get_uid() < h2->get_uid();
					}
					else
						b_result = false;
				}
			}
		}
		else
		{
			b_result = h1->get_tid() < h2->get_tid();
		}
	}
	else
	{
		b_result = h1->get_order_att_speed() > h2->get_order_att_speed();
	}

	return !b_result;
}

bool fight_hero_t::hero_att_more_speed_sort(const fight_hero_ptr& h1, const fight_hero_ptr& h2)
{
	if (NULL == h1 || NULL == h2)
	{
		return false;
	}

	bool b_result = false;
	if (h1->get_att_more_speed() == h2->get_att_more_speed())
	{
		if (h1->get_order_att_speed() == h2->get_order_att_speed())
		{
			if (h1->get_speed() == h2->get_speed())
			{
				if (h1->get_tid() == h2->get_tid())
				{
					// 都是玩家 比较玩家ID 小的在前 大的在后
					if (h1->get_type() == h2->get_type() && h1->get_type() != hero_type_monster)
					{
						b_result = h1->get_master() < h2->get_master();
					}
					else
					{
						// 玩家在前 怪物在后
						if (h1->get_type() != hero_type_monster)
						{
							b_result = true;
						}
						else
						{
							b_result = false;
						}
					}
				}
				else
				{
					b_result = h1->get_tid() < h2->get_tid();
				}
			}
			else
			{
				b_result = h1->get_speed() > h2->get_speed();
			}
		}
		else
		{
			b_result = h1->get_order_att_speed() > h2->get_order_att_speed();
		}
	}
	else
	{
		b_result = h1->get_att_more_speed() > h2->get_att_more_speed();
	}

	return !b_result;
}

uint32_t fight_hero_t::get_atk_mode()
{
	if (m_hero_data.base_attr().type() == hero_type_monster)
	{
		MonsterHero* monster_conf = GET_CONF(MonsterHero, get_tid());
		if (NULL == monster_conf)
		{
			log_error("monster_conf is null");
			return 0;
		}

		return monster_conf->atk_mode();
	}
	else
	{
		Hero* hero_conf = GET_CONF(Hero, get_tid());
		if (hero_conf == NULL)
		{
			log_error("Hero [%u] not exists", get_tid());
			return 0;
		}

		return hero_conf->atk_mode();
	}
}

uint32_t fight_hero_t::get_terry()
{
	if (m_hero_data.base_attr().type() == hero_type_monster)
	{
		MonsterHero* monster_conf = GET_CONF(MonsterHero, get_tid());
		if (NULL == monster_conf)
		{
			log_error("monster_conf is null");
			return 0;
		}

		return monster_conf->terry();
	}
	else
	{
		Hero* hero_conf = GET_CONF(Hero, get_tid());
		if (hero_conf == NULL)
		{
			log_error("Hero [%u] not exists", get_tid());
			return 0;
		}

		return hero_conf->terry();
	}
}

uint32_t fight_hero_t::get_sex()
{
	if (m_hero_data.base_attr().type() == hero_type_monster)
	{
		MonsterHero* monster_conf = GET_CONF(MonsterHero, get_tid());
		if (NULL == monster_conf)
		{
			log_error("monster_conf is null");
			return 0;
		}

		return monster_conf->sex();
	}
	else
	{
		Hero* hero_conf = GET_CONF(Hero, get_tid());
		if (hero_conf == NULL)
		{
			log_error("Hero [%u] not exists", get_tid());
			return 0;
		}

		return hero_conf->sex();
	}
}

uint32_t fight_hero_t::get_profession()
{
	if (m_hero_data.base_attr().type() == hero_type_monster)
	{
		MonsterHero* monster_conf = GET_CONF(MonsterHero, get_tid());
		if (NULL == monster_conf)
		{
			log_error("monster_conf is null");
			return 0;
		}

		return monster_conf->profession();
	}
	else
	{
		Hero* hero_conf = GET_CONF(Hero, get_tid());
		if (hero_conf == NULL)
		{
			log_error("Hero [%u] not exists", get_tid());
			return 0;
		}

		return hero_conf->profession();
	}
}

uint32_t fight_hero_t::get_wave_num()
{
	uint32_t wave_num = m_hero_data.fight_attr().wave_num();
	return wave_num;
}

int fight_hero_t::get_attr_without_buff(uint32_t type)
{
	attr_map::const_iterator it = m_cur_attr.find(type);
	if (it == m_cur_attr.end())
	{
		return 0;
	}

	return it->second;
}

int fight_hero_t::get_attr(uint32_t type)
{
	int32_t base_value = get_attr_without_buff(type);
	int32_t value = m_buff_element_mgr.get_buff_attr(type) + m_buff_element_moment_mgr.get_buff_attr( type );
	int32_t percent = m_buff_element_mgr.get_buff_attr_percent(type) + m_buff_element_moment_mgr.get_buff_attr( type );
	int32_t result = base_value * ((percent + 10000.f) / 10000.f) + value;
	return result;
}

void fight_hero_t::set_attr(uint32_t type, int value)
{
	if (value == m_cur_attr[type])
	{
		return;
	}

	m_cur_attr[type] = value;
}

void fight_hero_t::sub_attr(uint32_t type, int value)
{
	int sub = value;
	if (sub > get_attr_without_buff(type))
	{
		sub = get_attr_without_buff(type);
	}

	set_attr(type, get_attr_without_buff(type) - sub);
}

// 治疗HP
uint32_t fight_hero_t::heal_hp(fight_hero_ptr attacker, int value)
{
	// 治疗加成与治疗降低
	int bonus = attacker->get_attr(attrtype_enum::heal_bonus);
	bonus -= attacker->get_attr(attrtype_enum::heal_reduce);
	if (attacker->get_unique_id() != get_unique_id())
	{
		bonus += get_attr(attrtype_enum::get_heal_bonus);
		bonus -= get_attr(attrtype_enum::get_heal_reduce);
	}

	value *= (float)(10000 + bonus) * 0.0001;
	if (value < 0)
		return 0;
	do_team_script_event(event_on_resistance);
	add_hp(value);
	return value;
}

void fight_hero_t::add_hp(int value)
{
	if (NULL == m_script_proxy)
	{
		log_error("m_script_proxy is null");
		return;
	}
	if (value == 0)
		return;

	int add = value;
	if (get_attr_without_buff(attrtype_enum::hp) + value > get_attr(attrtype_enum::max_hp))
	{
		add = get_attr(attrtype_enum::max_hp) - get_attr_without_buff(attrtype_enum::hp);
	}

	set_attr(attrtype_enum::hp, get_attr_without_buff(attrtype_enum::hp) + add);

//	m_script_proxy->hp_change(value);
}

int fight_hero_t::get_real_sub_hp_value( int value )
{
	// 判断是否无敌
	if (get_attr(attrtype_enum::invincibility) > 0)
		return 0;
	// 判断是否有不死属性，如果有当扣除的值大小当前值则保留1滴血
	if( get_attr(attrtype_enum::athanasia) > 0 )
	{
		if( get_attr(attrtype_enum::hp) <= value )
		{
			return get_attr(attrtype_enum::hp) - 1;
		}
	}
	return value;
}

uint32_t fight_hero_t::get_hp_percent()
{
	uint32_t cur_hp = get_hp();
	uint32_t max_hp = get_max_hp();
	if (max_hp == 0)
	{
		log_error("get_hp_percent max_hp = 0");
		return 0;
	}
	uint32_t percent = cur_hp * 10000 / max_hp;
	return percent;
}

void fight_hero_t::sub_hp(int value, fight_hero_ptr attacker, uint32_t skill)
{
	if (NULL == attacker)
	{
		log_error("attacker is null");
		return;
	}

	if (NULL == m_script_proxy)
	{
		log_error("m_script_proxy is null");
		return;
	}
	if (value == 0)
		return;
	
	// 判断是否有不死属性，如果有当扣除的值大小当前值则保留1滴血
	value = get_real_sub_hp_value(value);
	sub_attr(attrtype_enum::hp, value);
}

int fight_hero_t::do_share_damage(int damage, fight_hero_ptr attacker, uint32_t skill, bool is_buff)
{
	int final_damage = damage;
	if (get_attr(attrtype_enum::share_damage) == 1)
	{
		fight_hero_ptr p_hero = fight_hero_ptr();
		uint32_t alive_num = 1;		// 1就是指自己
		std::vector<fight_hero_ptr> teamates;
		get_combat()->get_camp_alive_heros(teamates, get_camp(), get_pos());
		alive_num += teamates.size();
		if (alive_num == 0)
		{
			log_error("on_be_attack alive_num = 0");
			return final_damage;
		}

		final_damage = damage / alive_num;

		for (uint32_t i = 0; i < teamates.size(); ++i)
		{
			p_hero = teamates[i];
			if (NULL == p_hero)
			{
				log_error("on_be_attack p_hero NULL pos[%d]", i);
				continue;
			}
			share_damage_to(p_hero, shared_from_this(), skill, final_damage);
		}
	}
	return final_damage;
}

int fight_hero_t::do_target_bear_damage(int damage, fight_hero_ptr attacker, uint32_t skill, bool is_buff)
{
	int final_damage = damage;
	if (get_bear_damage_target() == NULL)
		return final_damage;
	if (get_bear_damage_target()->is_dead())
		return final_damage;
	int bear_damage = damage * (m_bear_damage_ratio * 0.0001);

	//attacker->add_combat_act(proto::common::combat_act_type_share_damage, damage, teammate, attacker, teammate->get_attr(attrtype_enum::hp));
	get_bear_damage_target()->deal_be_attack(bear_damage, attacker, skill);
	add_combat_act(proto::common::combat_act_type_damage, bear_damage, get_bear_damage_target(), attacker, get_bear_damage_target()->get_hp());

	final_damage = damage - bear_damage;
	return final_damage;
	
			

	return final_damage;
}

void fight_hero_t::on_be_attack(int damage, fight_hero_ptr attacker, uint32_t skill, bool is_buff)
{
	if (NULL == attacker)
	{
		log_error("attacker NULL");
		return;
	}
	if (damage == 0)
		return;

	m_cur_attacker = attacker;

	int final_damage = do_share_damage( damage, attacker, skill, is_buff);
	final_damage = do_target_bear_damage(damage, attacker, skill, is_buff);

	deal_be_attack(final_damage, attacker, skill);
}

void fight_hero_t::on_round_start_imp()
{
 	clear_cur_action();
	//get_script_proxy()->on_event(event_on_round_begin, get_script_proxy());
}

void fight_hero_t::on_round_end_imp()
{
	//get_script_proxy()->on_event(event_on_round_end, get_script_proxy());
}

bool fight_hero_t::on_event(FIGHT_EVENT event)
{
	if (is_dead())
	{
		// 死亡后除了死亡事件 其他事件都不应触发
		if (event != event_on_death)
			return true;
	}
	++get_count_data().event_trigger_count;
	// 防止事件互相调用触发死循环
	if (get_count_data().event_trigger_count > 50)
	{
		log_error("on_event dead cycle count > 50");
		return false;
	}
	get_buff_manager().update_buff(event);
	return true;
}

// 事件
void fight_hero_t::do_team_script_event(
	FIGHT_EVENT event)
{
	get_count_data().event_trigger_count = 0;
	fight_hero_ptr p_hero = fight_hero_ptr();

	const fight_hero_map& all_heros = get_combat()->get_all_heros();
	for (const auto& heropair : all_heros)
	{
		p_hero = heropair.second;
		if (NULL == p_hero)
		{
			log_error("fight_hero p_hero NULL");
			continue;
		}
		if (get_uid() == p_hero->get_uid())
		{
			if ( !p_hero->on_event(event) )
				continue;
		}
		else if (get_camp() == heropair.second->get_camp())
		{
			p_hero->get_count_data().event_trigger_count = 0;
			if ( !p_hero->on_event((FIGHT_EVENT)(event + MAGIC_TEAM_EVENT) ) )
				continue;
		}
		else
		{
			p_hero->get_count_data().event_trigger_count = 0;
			if ( p_hero->on_event((FIGHT_EVENT)(event + MAGIC_ENEMY_EVENT) ) )
				continue;
		}
	}
}

void fight_hero_t::on_round_start()
{
	set_att_speed(0);
	set_att_more_speed(0);
	set_att_order_speed(0);

	// 出手回合数
	++m_count_data.round_count;
	++m_round;
	m_buff_mgr.on_round_start();

	set_attack_step(proto::common::combat_act_step_round_start);
	on_event(event_on_round_begin);
}

void fight_hero_t::on_round_end()
{
	//if (NULL == m_script_proxy)
	{
	//	return;
	}

	// 如果不存于被控制状态则减CD
	if (!is_in_control())
	{
		sub_skill_cd();
	}
	
	set_attack_step(proto::common::combat_act_step_round_end);
	//m_script_proxy->round_end(m_round);
	on_event(event_on_round_end);
	m_count_data.resetByRound();
//	m_buff_mgr.round_end();
//	++m_round;	// 为了处理某些BUFF的规则 挪到回合开始++
	m_cur_attacker.reset();
}

void fight_hero_t::before_attack(SkillEffectTable* skill_conf, bool trigger_by_buff)
{
	if (NULL == skill_conf)
	{
		log_error("skill conf is null");
		return;
	}

	if (NULL == m_script_proxy)
	{
		log_error("m_script_proxy is null");
		return;
	}

	FIGHT_LOG("[%lu]do_attack start:camp[%u] pos[%u] tid[%u] skill[%u]<<<<<", get_fight_uid(), get_camp(), get_pos(), get_tid(), skill_conf->id());

	set_attack_step(proto::common::combat_act_step_before_attack);
	//m_script_proxy->before_attack(skill_conf->id());
	do_team_script_event(event_on_attack_begin);
	//notify_other_before_attack(skill_conf->id());

// 	if (!trigger_by_buff)
// 	{
// 		m_buff_mgr.before_attack();
// 	}
	
	// 出手前的buff处理
	do_skill_buff(NONE_PHASE, event_on_attack_begin, skill_event_target_type_except_target, skill_conf, NULL);
	m_cur_target.reset();
	m_attack_targets.clear();
}

fight_hero_ptr fight_hero_t::get_first_target()
{
	if (m_attack_targets.empty())
		return NULL;
	return m_attack_targets[0].lock();
}

SkillEffectTable* fight_hero_t::get_cur_skill()
{
	return m_cur_skill_conf;
}

fight_hero_ptr fight_hero_t::get_cur_target()
{
	return m_cur_target.lock();
}

fight_hero_ptr fight_hero_t::get_special_target()
{
	return m_special_target.lock();
}

fight_hero_ptr fight_hero_t::get_bear_damage_target()
{
	return m_bear_damage_target.lock();
}

fight_hero_ptr fight_hero_t::get_cur_attacker()
{
	return m_cur_attacker.lock();
}

void fight_hero_t::set_cur_attacker(fight_hero_ptr attacker)
{
	m_cur_attacker = attacker;
}

void fight_hero_t::after_attack(SkillEffectTable* skill_conf, bool trigger_by_buff)
{
	if (NULL == skill_conf)
	{
		log_error("skill_conf is null");
		return;
	}

	if (NULL == m_script_proxy)
	{
		log_error("m_script_proxy is null");
		return;
	}

	if (NULL == get_combat())
	{
		log_error("get_combat is null");
		return;
	}
	
	set_attack_step(proto::common::combat_act_step_after_attack);
	
	if(skill_conf->type() == proto::common::skill_type_sp_skill)
		do_team_script_event(event_on_use_sp_skill);
	do_team_script_event(event_on_attack_end);
	

	if (!trigger_by_buff)
	{
		//m_buff_mgr.after_attack();
		add_skill_cd(skill_conf->id());
		get_combat()->add_attack_count(get_master());
	}

	// 出手后的buff处理
	do_skill_buff(NONE_PHASE, event_on_attack_end, skill_event_target_type_except_target, skill_conf, NULL);
	for (auto target : m_attack_targets)
	{
		do_skill_buff(NONE_PHASE, event_on_attack_end, skill_event_target_type_target, skill_conf, target.lock());
	}
	
	//m_script_proxy->after_attack(skill_conf->id());
	m_cur_target.reset();
	m_attack_targets.clear();

	FIGHT_LOG("[%lu]do_attack end:camp[%u] pos[%u] tid[%u] skill[%u]>>>>>", get_fight_uid(), get_camp(), get_pos(), get_tid(), skill_conf->id());
}

void fight_hero_t::after_script_use_skill(SkillEffectTable* skill_conf)
{
	after_attack(skill_conf, false);
}

void fight_hero_t::do_skill_consum(SkillEffectTable* skill_conf)
{
	return;		// wys 2017.12.07 暂时没用了
	if (NULL == skill_conf)
	{
		log_error("skill_conf is null");
		return;
	}

	if (skill_conf->consum_value() == 0)
	{
		return;
	}

	if (skill_conf->consum_type() == skill_consum_type_hp)
	{
		if (!is_dead())
		{
			sub_hp(skill_conf->consum_value(), shared_from_this(), skill_conf->id());
			//add_combat_act(proto::common::combat_act_type_hp_dec, skill_conf->consum_value(), get_uid(), get_attr(attrtype_enum::hp));
			//add_combat_act(proto::common::combat_act_type_hp_dec, skill_conf->consum_value(), shared_from_this(), shared_from_this(), get_attr(attrtype_enum::hp));
		}
	}
	else if (skill_conf->consum_type() == skill_consum_type_sp)
	{
		sub_sp(skill_conf->consum_value());
	}
}

void fight_hero_t::on_dead(fight_hero_ptr attaker, SkillEffectTable* skill_conf)
{
	if (NULL == attaker)
	{
		log_error("attaker is null");
		return;
	}

	if (NULL == skill_conf)
	{
		log_error("skill_conf is null");
		return;
	}

	if (NULL == attaker->get_script_proxy())
	{
		log_error("attaker script_proxy is null");
		return;
	}

	if (NULL == m_script_proxy)
	{
		log_error("m_script_proxy is null");
		return;
	}

	if (NULL == get_combat())
	{
		log_error("get_combat is null");
		return;
	}

// 	if (m_last_hp == 0)
// 	{
// 		return;
// 	}
	m_dead_flag = true;
	clear_taunt_info();

	FIGHT_LOG("[%lu]*****on_dead: camp[%u] pos[%u] tid[%u]", get_fight_uid(), get_camp(), get_pos(), get_tid());

	// 死亡清除buff
	m_buff_mgr.on_dead();
	// 回收光环BUFF
	recovery_special_buff();

	get_combat()->on_fight_hero_dead(shared_from_this());
	do_team_script_event(event_on_death);
	//m_script_proxy->on_dead();

	attaker->do_skill_buff(NONE_PHASE, event_on_killer, skill_event_target_type_except_target, skill_conf, NULL);
	attaker->do_team_script_event(event_on_killer);
	//attaker->get_script_proxy()->on_kill(get_script_proxy(), skill);
}

uint32_t fight_hero_t::get_relive_count()
{
	return get_count_data().relive_count;
}

void fight_hero_t::send_trigger_passive_to_user(uint32_t skill_id, uint32_t buff_id, uint32_t buff_tid)
{
	SkillEffectTable* skill_conf = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(skill_id, 1));
	if (NULL == skill_conf)
		return;
	// 判断这个技能是否被动技能
	add_combat_act(proto::common::combat_act_type_special_skill, skill_id, shared_from_this(), shared_from_this(), 0, buff_id, buff_tid);
}

void fight_hero_t::on_relive(fight_hero_ptr attacker, uint32_t buffid, uint32_t bufftid, uint32_t skill, int relive_hp, int relive_speed )
{
	if (NULL == attacker)
	{
		log_error("attacker is null");
		return;
	}

	if (NULL == get_combat())
	{
		log_error("get_combat is null");
		return;
	}

	// 分发光环BUFF
	distribute_special_buff();

	m_dead_flag = false;
	// 设置HP
	add_hp( relive_hp );
	
	// 设置速度条
	if( relive_speed > 0 )
		set_att_more_speed(relive_speed);


	add_combat_act(proto::common::combat_act_type_relive, relive_hp, shared_from_this(), attacker, get_hp(), buffid, bufftid);
	do_team_script_event(event_on_relive);
	++get_count_data().relive_count;
}

// 被召唤出来
void fight_hero_t::on_summon(fight_hero_ptr attacker, uint32_t skill)
{
	
}

uint32_t fight_hero_t::get_round()
{
// 	if (NULL == get_combat())
// 	{
// 		return 0;
// 	}
// 
// 	return get_combat()->get_round();
	return m_round;
}

// 随机添加技能CD 
void fight_hero_t::inc_random_skill_cd(fight_hero_ptr attacker, uint32_t cd)
{
	std::vector<uint32_t> skills;
	std::map<uint32_t, skill_data>::iterator it;

	for (it = m_skills.begin(); it != m_skills.end(); ++it)
	{
		SkillEffectTable* p_skill_conf = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(it->second.skill_id, it->second.skill_level));
		if (NULL == p_skill_conf)
			continue;
		if (p_skill_conf->cd() == 0)
			continue;
		// 判断是否被动技能
		if (p_skill_conf->type() == proto::common::skill_type_passive)
			continue;
		if (get_skill_cd(it->second.skill_id) >= p_skill_conf->cd())
			continue;

		skills.push_back(it->second.skill_id);
	}
	{
		if (skills.empty())
			return;
		int rand = random_util_t::randMin(0, skills.size());
		if (rand == -1)
		{
			log_error("rand error skill_size[%d]", skills.size());
			return;
		}
		SkillEffectTable* p_skill_conf = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(skills[rand], get_skill_level(skills[rand])));
		if (NULL == p_skill_conf)
			return;
		uint32_t cur = get_skill_cd(skills[rand]);
		uint32_t add = cur + cd;
		uint32_t ret = std::min(add, p_skill_conf->cd());
		if (ret == cur)
			return;
		m_skill_cd[skills[rand]] = ret;
		add_combat_act(proto::common::combat_act_type_syn_cd, skills[rand], shared_from_this(), attacker, ret);
	}
	
}
// 随机减少技能CD
void fight_hero_t::dec_random_skill_cd(fight_hero_ptr attacker, uint32_t cd)
{
	std::vector<uint32_t> skills;
	std::map<uint32_t, uint32_t>::iterator it;
	for (it = m_skill_cd.begin(); it != m_skill_cd.end(); ++it)
	{
		if (it->second == 0)
			continue;
		skills.push_back(it->first);
	}
	{
		if (skills.empty())
			return;
		int rand = random_util_t::randBetween(0, skills.size());
		it = m_skill_cd.find(skills[rand]);
		if (it != m_skill_cd.end())
			return;
		if (it->second > cd)
			it->second = it->second - cd;
		else
			it->second = 0;
		add_combat_act(proto::common::combat_act_type_syn_cd, it->first, shared_from_this(), attacker, it->second);
	}
}

// 添加所有技能CD 
void fight_hero_t::inc_all_skill_cd(fight_hero_ptr attacker,uint32_t cd)
{
	std::map<uint32_t, skill_data>::iterator it;
	SkillEffectTable* p_skill_conf = NULL;
	for (it = m_skills.begin(); it != m_skills.end(); ++it)
	{
		p_skill_conf = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(it->second.skill_id, it->second.skill_level));
		if( NULL == p_skill_conf )
			continue;
		if (p_skill_conf->cd() == 0)
			continue;
		uint32_t cur = get_skill_cd(it->second.skill_id);
		uint32_t add = cur + cd;
		uint32_t ret = std::min(add, p_skill_conf->cd());
		if( ret == cur )
			continue;
		m_skill_cd[it->second.skill_id] = ret;
		add_combat_act(proto::common::combat_act_type_syn_cd, it->second.skill_id, shared_from_this(), attacker, ret);
	}
}

// 减少所有技能CD
void fight_hero_t::dec_all_skill_cd(fight_hero_ptr attacker,uint32_t cd)
{
	std::map<uint32_t, uint32_t>::iterator it;
	for (it = m_skill_cd.begin(); it != m_skill_cd.end(); ++it)
	{
		if( it->second == 0 )
			continue;
		if (it->second > cd)
			it->second = it->second - cd;
		else
			it->second = 0;
		add_combat_act(proto::common::combat_act_type_syn_cd, it->first, shared_from_this(), attacker, it->second);
	}
}

uint32_t fight_hero_t::get_skill_cd(uint32_t skill)
{
	std::map<uint32_t, uint32_t>::const_iterator it = m_skill_cd.find(skill);
	if (it == m_skill_cd.end())
	{
		return 0;
	}

	return it->second;
}

void fight_hero_t::sub_skill_cd(uint32_t skill_cd)
{
	std::map<uint32_t, uint32_t>::iterator it = m_skill_cd.begin();
	
	for (; it != m_skill_cd.end(); ++it)
	{
		if (it->second > skill_cd)
		{
			it->second -= skill_cd;
		}
		else
			it->second = 0;
	}
}

void fight_hero_t::add_skill_cd(uint32_t skill)
{
	int level = get_skill_level(skill);
	if (level == 0)
		level = 1;
	// 判断是否需要免CD
	int exempt_cd = get_attr(attrtype_enum::exempt_cd);
	if (random_util_t::selectByTenTh(exempt_cd))
	{
		add_combat_act(proto::common::combat_act_type_syn_cd, skill, shared_from_this(), shared_from_this(), m_skill_cd[skill]);
		FIGHT_LOG("[%lu]exempt cd [%u] [%u] [%u]", get_fight_uid(), get_camp(), get_pos(), skill);
		return;
	}
	// 判断技能是否被动技能
	SkillEffectTable* skill_conf = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(skill, level));
	if (NULL == skill_conf)
		return;
	// 没有技能CD的不用加
	if (skill_conf->cd() == 0)
		return;

	int cd = skill_conf->cd();
	if (cd + m_skill_cd[skill] > skill_conf->cd())
	{
		cd = skill_conf->cd() - m_skill_cd[skill];
	}
	
	// 技能CD 的额外变化
	int changecd = get_attr(attrtype_enum::skill_cd_change);
	
	cd += changecd;
	if (cd < 0)
		return;

	m_skill_cd[skill] += cd;
}

// bool fight_hero_t::check_start_skill_cd(SkillEffectTable* conf)
// {
// 	if (NULL == conf)
// 		return false;
// 
// 	std::map<uint32_t, uint32_t>::iterator iter = m_start_skill_cd.find(conf->id());
// 	if (iter == m_start_skill_cd.end())
// 	{
// 		return true;
// 	}
// 	else
// 	{
// 		return false;
// 	}
// 	return true;
// }

void fight_hero_t::deal_be_attack(int32_t damage, fight_hero_ptr attacker, uint32_t skill)
{
	++get_count_data().beattack_count;
	do_team_script_event(event_on_behurt_without_sub_hp);
	sub_hp(damage, attacker, skill);
	do_team_script_event(event_on_behurt);

	// 攻击吵醒规则 概率免吵醒
	if (get_attr(attrtype_enum::sleep) == 0)
		return;

	uint32_t no_wakeup_rate = attacker->get_attr(attrtype_enum::attack_no_wakeup_rate);
	if (no_wakeup_rate > 0)
	{
		if (random_util_t::selectByTenTh(no_wakeup_rate))
			return;
	}
	
	do_team_script_event(event_on_attack_wake_up);
// 	// 修改昏睡状态 添加行为
// 	set_attr(attrtype_enum::sleep, 0);
// 	add_combat_act(proto::common::combat_act_type_attr_dec, 1, shared_from_this(), attacker, attrtype_enum::sleep, get_attr(attrtype_enum::sleep));
}

// void fight_hero_t::add_damage(int damage)
// {
// 	m_round_data[get_round()].damage += damage;
// }
// 
// void fight_hero_t::add_injured(int injured)
// {
// 	m_round_data[get_round()].injured += injured;
// }
// 
// uint32_t fight_hero_t::get_round_damage(uint32_t round)
// {
// 	round_data::const_iterator it = m_round_data.find(round);
// 	if (it == m_round_data.end())
// 	{
// 		return 0;
// 	}
// 
// 	return it->second.damage;
// }
// 
// uint32_t fight_hero_t::get_round_injured(uint32_t round)
// {
// 	round_data::const_iterator it = m_round_data.find(round);
// 	if (it == m_round_data.end())
// 	{
// 		return 0;
// 	}
// 
// 	return it->second.injured;
// }

bool fight_hero_t::update_fight(const proto::common::hero_state& state)
{
	m_hero_data.mutable_fight_attr()->mutable_update()->CopyFrom(state);
	return true;
}

void fight_hero_t::do_update_fight()
{
	if (m_hero_data.fight_attr().update().pos() == get_pos())
	{
		set_attr(attrtype_enum::hp, m_hero_data.fight_attr().update().hp());
		m_hero_data.mutable_fight_attr()->mutable_update()->Clear();
		//add_combat_act(proto::common::combat_act_type_hp_dec, 0, get_uid(), get_attr(attrtype_enum::hp));
		//add_combat_act(proto::common::combat_act_type_hp_dec, 0, shared_from_this(), shared_from_this(), get_attr(attrtype_enum::hp));
	}
}
// 
// void fight_hero_t::add_cur_target(fight_hero_ptr target)
// {
// 	if (NULL == target)
// 	{
// 		log_error("target is null");
// 		return;
// 	}
// 
// 	m_cur_targets.push_back(target);
// }

void fight_hero_t::set_be_taunt_info(fight_hero_ptr target, bool is_add, uint32_t taunt_buff_type)
{
	if (NULL == target)
	{
		log_error("set_be_taunt_info target NULL");
		return;
	}
	m_special_target = target;
	if (is_add)
	{
		m_buff_element_mgr.inc_element_value(attrtype_enum::taunt, 1);
		add_combat_act(proto::common::combat_act_type_attr_inc, 1, shared_from_this(), target, attrtype_enum::taunt, 1);
		target->set_taunt_buff_info(taunt_buff_type);
		target->add_taunt_target(shared_from_this());
	}
	else
	{
		m_buff_element_mgr.dec_element_value(attrtype_enum::taunt, 1);
		add_combat_act(proto::common::combat_act_type_attr_dec, 1, shared_from_this(), target, attrtype_enum::taunt, 0);
		target->remove_taunt_target(shared_from_this());
	}
}

void fight_hero_t::add_taunt_target(fight_hero_ptr p_target)
{
	if (NULL == p_target)
	{
		log_error("p_target NULL");
		return;
	}
	m_taunt_data.taunt_target_map.insert(std::make_pair(p_target->get_uid(), p_target));
}

void fight_hero_t::remove_taunt_target(fight_hero_ptr p_target)
{
	if (NULL == p_target)
	{
		log_error("p_target NULL");
		return;
	}
	m_taunt_data.taunt_target_map.erase(p_target->get_uid());
	if (m_taunt_data.taunt_target_map.size() == 0)
		m_taunt_data.taunt_buff_type = 0;
}

void fight_hero_t::clear_taunt_info()
{
	if (m_taunt_data.taunt_target_map.size() > 0)
	{
		std::map<uint32_t, fight_hero_wptr> copy(m_taunt_data.taunt_target_map);
		std::map<uint32_t, fight_hero_wptr>::iterator iter = copy.begin();
		for (; iter != copy.end(); ++iter)
		{
			fight_hero_ptr p_hero = iter->second.lock();
			if (NULL == p_hero)
			{
				log_error("clear_taunt_info p_hero NULL");
				continue;
			}
			p_hero->m_buff_mgr.remove_buff_by_small_type(m_taunt_data.taunt_buff_type);
		}
	}
	m_taunt_data.taunt_target_map.clear();
	m_taunt_data.taunt_buff_type = 0;
}

void fight_hero_t::set_cur_select(const std::string& select)
{
	m_cur_form.set_target(select);
}

void fight_hero_t::skill_damage(fight_hero_ptr target, uint32_t damage, uint32_t skill)
{
	if (NULL == target)
	{
		log_error("target is null");
		return;
	}

	target->on_be_attack(damage, shared_from_this(), skill);
	//add_combat_act(proto::common::combat_act_type_damage, damage, target->get_uid(), target->get_attr(attrtype_enum::hp));
	add_combat_act(proto::common::combat_act_type_damage, damage, target, shared_from_this(), target->get_attr(attrtype_enum::hp));
}

void fight_hero_t::skill_ext_damage(fight_hero_ptr target, uint32_t damage, uint32_t skill)
{
	if (NULL == target)
	{
		log_error("target is null");
		return;
	}

	target->on_be_attack(damage, shared_from_this(), skill);
	//add_combat_act(proto::common::combat_act_type_ext, damage, target->get_uid(), target->get_attr(attrtype_enum::hp));
	add_combat_act(proto::common::combat_act_type_ext, damage, target, shared_from_this(), target->get_attr(attrtype_enum::hp));
}

void fight_hero_t::share_damage_to(fight_hero_ptr teammate, fight_hero_ptr attacker, uint32_t skill, uint32_t damage)
{
	if (NULL == teammate)
	{
		log_error("teammate is null");
		return;
	}

	if (NULL == attacker)
	{
		log_error("attacker is null");
		return;
	}

	FIGHT_LOG("c_share_damage_to damage[%d] be_share_hero:%d attacker:%d accpet_hero:%d", damage, get_tid(), attacker->get_tid(), teammate->get_tid());


	attacker->add_combat_act(proto::common::combat_act_type_share_damage, damage, teammate, attacker, teammate->get_attr(attrtype_enum::hp));
	teammate->deal_be_attack(damage, attacker, skill);
	teammate->add_combat_act(proto::common::combat_act_type_be_share_damage, damage, teammate, attacker, teammate->get_attr(attrtype_enum::hp));
}

void fight_hero_t::on_remove_buff(uint32_t id, uint32_t tid, uint32_t layer, uint32_t round, uint64_t attacker_id)
{
	// 记录添加buff的参数
	m_calc_result.remove_buff_id = tid;

	add_combat_buff_act(proto::common::combat_act_type_buff_remove, id, tid, layer, round, attacker_id);
	do_team_script_event(event_on_remove_buff);
	FIGHT_LOG("[%lu]==buff remove: camp[%u] pos[%u] tid[%u] [%u]", get_fight_uid(), get_camp(), get_pos(), get_tid(), id);
}

void fight_hero_t::on_add_buff(uint32_t id, uint32_t tid, uint32_t layer, uint32_t round, uint64_t attacker_id)
{
	// 记录添加buff的参数
	m_calc_result.add_buff_id = tid;

//	add_combat_buff_act(proto::common::combat_act_type_buff, id, tid, layer, round, attacker_id);
	do_team_script_event(event_on_add_buff);
	FIGHT_LOG("[%lu]==buff add: camp[%u] pos[%u] tid[%u] [%u]", get_fight_uid(), get_camp(), get_pos(), get_tid(), id);
}

void fight_hero_t::on_buff_immuno()
{
	if (NULL == m_script_proxy)
	{
		log_error("m_script_proxy is null");
		return;
	}
	// attacker在此行为中没用 为了兼容参数 写自己就行
	add_combat_act(proto::common::combat_act_type_buff_immuno, 1, shared_from_this(), shared_from_this());
//	m_script_proxy->on_immuno();
}

void fight_hero_t::add_combat_act(proto::common::combat_act_type type, uint32_t value, fight_hero_ptr target,
									fight_hero_ptr attacker, uint32_t remain_value, uint32_t remain_value2, uint32_t param)
{
	if (NULL == get_cur_process())
	{
		log_error("get_cur_process() is null");
		return;
	}
	if (NULL == target)
	{
		log_error("traget NULL");
		return;
	}
	if (NULL == attacker)
	{
		log_error("attacker NULL");
		return;
	}
	if (!pass_combat_act_type(type) && value == 0)
	{
		log_warn("fight_hero type:%d value = 0 attacker_tid:%d target_tid:%d", type, value, attacker->get_tid(), target->get_tid());
		return;
	}

	proto::common::combat_act* ca = get_cur_process()->add_actions();
	if (NULL == ca)
	{
		log_error("ca is null");
		return;
	}

	ca->set_act_type(type);
	ca->set_act_value(value);
	ca->add_target(target->get_str_uid());
	ca->set_attacker(attacker->get_str_uid());
	ca->set_step(get_attack_step());
	ca->set_remain_value(remain_value);
	ca->set_remain_value2(remain_value2);
	ca->set_param(param);

	FIGHT_LOG("combat_act fight_uid[%lu] self[%lu] attacker[%lu] target[%lu] type[%d] value[%d] step[%d] remain_value[%d] remain_value2[%d] param[%d]",
		get_fight_uid(), get_tid(), attacker->get_tid(), target->get_tid(), type, value, get_attack_step(), remain_value, remain_value2, param);

//	uint32_t old_value = 0;

	switch (type)
	{
	case proto::common::combat_act_type_damage:
	case proto::common::combat_act_type_buff_damage:
	case proto::common::combat_act_type_share_damage:
	case proto::common::combat_act_type_absorb:
	case proto::common::combat_act_type_crit:
	case proto::common::combat_act_type_beheaded:
	case proto::common::combat_act_type_reflect:
	case proto::common::combat_act_type_ext:
	case proto::common::combat_act_type_spurting:
		{
//			old_value = target->get_stat_data().get_accept_damage_count();
			target->get_stat_data().add_accept_damage(value);
			target->get_count_data().one_round_injured += value;
// 			FIGHT_LOG("count data accept_damage hero_tid:%d master_id:%lu old_value:%d new_value:%d", target->get_tid(), 
// 						target->get_master(), old_value, target->get_stat_data().get_accept_damage_count());

//			old_value = attacker->get_stat_data().get_damage_count();
			attacker->get_stat_data().add_damage(value);
			attacker->get_count_data().one_round_damage += value;
// 			FIGHT_LOG("count data damage hero_tid:%d master_id:%lu old_value:%d new_value:%d", attacker->get_tid(),
// 				attacker->get_master(), old_value, attacker->get_stat_data().get_damage_count());
		}
		break;
	case proto::common::combat_act_type_buff_heal:
	case proto::common::combat_act_type_heal:
	case proto::common::combat_act_type_heal_crit:
	case proto::common::combat_act_type_recover:
	case proto::common::combat_act_type_leech:
	case proto::common::combat_act_type_kill_leech:
		{
//			old_value = attacker->get_stat_data().get_heal_count();
			attacker->get_stat_data().add_heal(value);
// 			FIGHT_LOG("count data heal hero_tid:%d master_id:%lu old_value:%d new_value:%d", attacker->get_tid(),
// 				attacker->get_master(), old_value, attacker->get_stat_data().get_heal_count());
		}
		break;
	default:
		break;
	}

}

bool fight_hero_t::pass_combat_act_type(proto::common::combat_act_type type)
{
	if (type == proto::common::combat_act_type_syn_cd || 
		type == proto::common::combat_act_type_damage ||
		type == proto::common::combat_act_type_beheaded ||
		type == proto::common::combat_act_type_spurting || 
		type == proto::common::combat_act_type_crit)
		return true;
	return false;
}

void fight_hero_t::add_combat_skill_act(proto::common::combat_act_type type, uint32_t value, uint64_t attacker, std::vector<uint64_t>& targets)
{
	if (NULL == get_cur_process())
	{
		log_error("get_cur_process() is null");
		return;
	}

	proto::common::combat_act* ca = get_cur_process()->add_actions();
	if (NULL == ca)
	{
		log_error("ca is null");
		return;
	}

	ca->set_act_type(type);
	ca->set_act_value(value);
	ca->set_attacker(string_util_t::uint64_to_string(attacker));
	for (auto target : targets)
	{
		ca->add_target(string_util_t::uint64_to_string(target));
	}
	ca->set_step(get_attack_step());
}

void fight_hero_t::add_combat_buff_act(proto::common::combat_act_type type, uint32_t id, uint32_t tid, uint32_t layer, uint32_t round, uint64_t attacker_id)
{
	if (NULL == get_cur_process())
	{
		log_error("get_cur_process() is null");
		return;
	}

	proto::common::combat_act* ca = get_cur_process()->add_actions();
	if (NULL == ca)
	{
		log_error("ca is null");
		return;
	}

	ca->set_act_type(type);
	ca->set_act_value(id);
	ca->add_target(get_str_uid());
	ca->set_attacker(string_util_t::uint64_to_string(attacker_id));
	ca->set_step(get_attack_step());
	ca->set_remain_value(layer);
	ca->set_remain_value2(round);
	ca->set_param(tid);
}

void fight_hero_t::set_script_var(const std::string& var, int value)
{
	m_script_var[var] = value;
}

int fight_hero_t::get_script_var(const std::string& var)
{
	std::map<std::string, int>::const_iterator it = m_script_var.find(var);
	if (it == m_script_var.end())
	{
		return 0;
	}

	return it->second;
}

proto::common::combat_action_data* fight_hero_t::get_cur_process()
{
	if (NULL == get_combat())
	{
		log_error("get_combat is null");
		return NULL;
	}

	return get_combat()->get_cur_process();
}

uint32_t fight_hero_t::get_sp()
{
	if (NULL == get_combat())
	{
		log_error("get_combat is null");
		return 0;
	}

	return get_combat()->get_sp(get_master());
}

void fight_hero_t::sub_sp(uint32_t value)
{
	if (NULL == get_combat())
	{
		log_error("get_combat is null");
		return;
	}

	get_combat()->sub_sp(get_master(), value);
}

void fight_hero_t::add_sp(uint32_t value)
{
	if (NULL == get_combat())
	{
		log_error("get_combat is null");
		return;
	}

	get_combat()->add_sp(get_master(), value);
}

void fight_hero_t::sub_disposition_sp(uint32_t value)
{
	if (NULL == get_combat())
	{
		log_error("get_combat is null");
		return;
	}

	get_combat()->sub_disposition_sp(get_master(), value);
}

uint32_t fight_hero_t::get_disposition_sp()
{
	if (NULL == get_combat())
	{
		log_error("get_combat is null");
		return 0;
	}

	return get_combat()->get_disposition_sp(get_master());
}

bool fight_hero_t::is_in_control()
{
	// 眩晕状态
	if (get_attr(attrtype_enum::stun) > 0 || get_attr(attrtype_enum::frozen) > 0 || get_attr(attrtype_enum::sleep) > 0)
	{
		return true;
	}
	return false;
}

proto::common::combat_act_step fight_hero_t::get_attack_step()
{
	if (NULL == get_combat())
	{
		log_error("get_combat is null");
		return proto::common::combat_act_step_none;
	}

	return get_combat()->get_attack_step();
}

void fight_hero_t::set_attack_step(proto::common::combat_act_step step)
{
	if (NULL == get_combat())
	{
		log_error("get_combat is null");
		return;
	}

	get_combat()->set_attack_step(step);
}

bool fight_hero_t::is_weak_profession()
{
	uint32_t profession = get_profession();
	return (profession == 4 || profession == 5 || profession == 6);
}

void fight_hero_t::pack_attack_targets(std::string &targets)
{
	targets.clear();
	uint32_t target_num = (uint32_t)m_attack_targets.size();
	fight_hero_ptr p_fight_hero = fight_hero_ptr();
	for (uint32_t i = 0; i < target_num; ++i)
	{
		p_fight_hero = m_attack_targets[i].lock();
		if (NULL == p_fight_hero)
		{
			log_error("fight p_fight_hero NULL pos[%d]", i);
			continue;
		}
		if (targets.empty())
			targets.append(p_fight_hero->get_str_uid());
		else
		{
			std::string temp = ";" + p_fight_hero->get_str_uid();
			targets.append(temp);
		}
	}
}

void fight_hero_t::peek_recovery_data(proto::common::fight_recovery_hero_single_data *hero_data)
{
	if (NULL == hero_data)
	{
		log_error("fight_hero_t peek_recovery_data hero_data NULL");
		return;
	}
	hero_data->set_id(get_str_uid());
	hero_data->set_cur_hp(get_attr(attrtype_enum::hp));
	hero_data->set_mark_id(get_mark_id());
	hero_data->set_speed(get_speed());
	m_buff_mgr.peek_all_buff_data(hero_data->mutable_buff_data());

	peek_fight_skill_data(hero_data->mutable_skill_data());
}

void fight_hero_t::peek_fight_skill_data(proto::common::fight_hero_skill_data *skill_data)
{
	if (skill_data == NULL)
	{
		log_error("fight_hero_t skill_data NULL");
		return;
	}

	proto::common::fight_hero_skill_single_data *skill_single = NULL;

	for (auto pair : m_skill_cd)
	{
		skill_single = skill_data->add_skill_list();
		skill_single->set_skill_id(pair.first);
		skill_single->set_skill_cd(pair.second);
	}
}



// 分发光环BUFF
void fight_hero_t::distribute_special_buff()
{
	SkillEffectTable* p_skill_conf = NULL;
	for (auto pair : m_skills)
	{
		skill_data &data = pair.second;
		p_skill_conf = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(data.skill_id, data.skill_level));
		if (NULL == p_skill_conf)
			continue;
		// 遍历添加光环BUFF
		const add_buff_t& buffs = p_skill_conf->get_gx_buff();
		buff_info_vec::const_iterator it;
		for (it = buffs.fifth.begin(); it != buffs.fifth.end(); ++it)
		{
			add_special_buff_to_all(it->first, buffs.third, data.skill_id);
		}

	}
}

void fight_hero_t::add_special_buff_to_all(int buff, int target_type, int skill_id)
{
	fight_hero_map& all_heros = get_combat()->get_all_heros_ptr();
	for (auto& heropair : all_heros)
	{
		// 加给敌方
		if (target_type == skill_buff_target_type_enemy)
		{
			if (get_camp() != heropair.second->get_camp())
			{
				heropair.second->m_buff_mgr.add_special_buff(buff, get_uid(), skill_id);
			}
		}
		// 加给友方
		else
		{
			if (get_camp() == heropair.second->get_camp())
			{
				heropair.second->m_buff_mgr.add_special_buff(buff, get_uid(), skill_id);
			}
		}
	}

}

void fight_hero_t::remove_special_buff_from_all(int buff, int target_type, int skill_id)
{
	fight_hero_map& all_heros = get_combat()->get_all_heros_ptr();
	for (auto& heropair : all_heros)
	{
		heropair.second->m_buff_mgr.remove_special_buff(buff, get_uid());
	}
}

// 回收光环BUFF
void fight_hero_t::recovery_special_buff()
{
	SkillEffectTable* p_skill_conf = NULL;
	for (auto pair : m_skills)
	{
		skill_data &data = pair.second;
		p_skill_conf = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(data.skill_id, data.skill_level));
		if (NULL == p_skill_conf)
			continue;
		// 遍历添加光环BUFF
		const add_buff_t& buffs = p_skill_conf->get_gx_buff();
		buff_info_vec::const_iterator it;
		for (it = buffs.fifth.begin(); it != buffs.fifth.end(); ++it)
		{
			remove_special_buff_from_all(it->first, buffs.third, data.skill_id);
		}
	}

}

// 激活被动BUFF
void fight_hero_t::activation_passive_buff()
{
	SkillEffectTable* p_skill_conf = NULL;
	for (auto pair : m_skills)
	{
		skill_data &data = pair.second;
		p_skill_conf = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(data.skill_id, data.skill_level));
		if (NULL == p_skill_conf)
			continue;
		// 遍历添加被动BUFF
		const add_buff_t& buffs = p_skill_conf->get_bd_buff();
		buff_info_vec::const_iterator it;
		for (it = buffs.fifth.begin(); it != buffs.fifth.end(); ++it)
		{
			m_buff_mgr.add_special_buff(it->first, get_uid(), data.skill_id);
		}
	}

}

void fight_hero_t::clear_passive_buff()
{
	SkillEffectTable* p_skill_conf = NULL;
	for (auto pair : m_skills)
	{
		skill_data &data = pair.second;
		p_skill_conf = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(data.skill_id, data.skill_level));
		if (NULL == p_skill_conf)
			continue;
		// 遍历被动BUFF
		const add_buff_t& buffs = p_skill_conf->get_bd_buff();
		buff_info_vec::const_iterator it;
		for (it = buffs.fifth.begin(); it != buffs.fifth.end(); ++it)
		{
			m_buff_mgr.remove_special_buff(it->first, get_uid());
		}
	}
}

void fight_hero_t::peek_save_data(proto::common::hero_save_data *hero_data)
{
	if (NULL == hero_data)
	{
		log_error("peek_save_data hero_data NULL");
		return;
	}
	// 主公不存
	if (get_type() == 0)
	{
		return;
	}
	uint32_t max_hp = get_attr(attrtype_enum::max_hp);
	if (max_hp == 0)
	{
		log_error("peek_save_data max_hp = 0 hero[%lu][%d]", get_uid(), get_tid());
		return;
	}
	hero_data->set_hero_uid(get_unique_id());
	uint32_t save_hp_percent = get_hp() * 100 / max_hp;
	if (save_hp_percent == 0)
	{
		if (get_hp() > 0)
			save_hp_percent = 1;
	}
	hero_data->set_hp(save_hp_percent);

	proto::common::skill_save_data *p_skill_data = NULL;
	
	std::map<uint32_t, uint32_t>::iterator iter;
	for (auto pair : m_skills)
	{
		skill_data &data = pair.second;
		p_skill_data = hero_data->add_skill_list();
		p_skill_data->set_skill_id(data.skill_id);
		p_skill_data->set_cd(0);

		iter = m_skill_cd.find(data.skill_id);
		if (iter != m_skill_cd.end())
		{
			p_skill_data->set_cd(iter->second);
		}
	}
}

void fight_hero_t::set_bear_damage(uint32_t ratio, fight_hero_ptr target)
{
	m_bear_damage_ratio = ratio;
	m_bear_damage_target = target;
}

void fight_hero_t::skill_change_att_speed(int32_t change_value, fight_hero_ptr attacker, bool is_percent, uint32_t param1)
{
	if (NULL == get_combat())
	{
		log_error("get_combat is null");
		return;
	}
	int32_t att_more_speed = 0;
	proto::common::combat_act_type act_type = proto::common::combat_act_type_att_speed_inc;

	int32_t max_att_speed = get_combat()->get_max_att_speed();
	int32_t att_speed = change_value;
	if (is_percent)
		att_speed = change_value * max_att_speed / 10000;

	if (att_speed > 0)
	{
		int32_t dec_att_more_speed = get_att_speed() + att_speed - max_att_speed;
		if (dec_att_more_speed <= 0)
		{
			att_more_speed = get_att_more_speed() + att_speed;
		}
		else if (dec_att_more_speed > 0)
		{
			att_more_speed = get_att_more_speed() + dec_att_more_speed;
		}
		if (att_more_speed > 0)
		{
			set_att_more_speed(att_more_speed);
		}
		act_type = proto::common::combat_act_type_att_speed_inc;
	}
	else
	{
		set_att_more_speed(0);
		act_type = proto::common::combat_act_type_att_speed_dec;
	}
	set_att_speed(get_att_speed() + att_speed);
	add_combat_act(act_type, std::abs(att_speed), shared_from_this(), attacker, get_att_speed(), param1);
}
