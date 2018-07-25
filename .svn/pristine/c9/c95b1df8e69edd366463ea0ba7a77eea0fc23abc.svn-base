#include "fight_hero.hpp"
#include "fight_manager.hpp"
#include "common/config_mgr.h"

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
//	m_round_data.clear();
	m_attack_targets.clear();
	m_immuno_big_buff_types.clear();
	m_immuno_small_buff_types.clear();
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
	m_skill_manager.init(shared_from_this() );

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

	uint32_t skill_size = hs->skill().skills_size();
		
	for (uint32_t i = 0; i < skill_size; ++i)
	{
		const proto::common::skill_single& skill_single = hs->skill().skills(i);
		if(skill_single.has_skill_cd())
			add_skill(skill_single.skill_id(), skill_single.skill_level(), skill_single.skill_cd(), 0, i );
		else
			add_skill(skill_single.skill_id(), skill_single.skill_level(), 0, 0, i);
	}

	add_turn_skill();

	m_att_speed = get_speed();
	m_order_att_speed = get_speed();
	m_round = 1;
	
	FIGHT_LOG("[%lu]fight_hero_t init: camp[%u] pos[%u] tid[%u] [%u][%d]", get_fight_uid(), get_camp(), get_pos(), get_tid(), attrtype_enum::hp, get_attr(attrtype_enum::hp));

	return true;
}

void fight_hero_t::remove_skill(const int skill)
{
}

int fight_hero_t::get_skill_level(const int skill)
{
	return m_skill_manager.get_skill_level(skill);
}

void fight_hero_t::add_skill(const int skill, const int level, const int init_cd, uint32_t turn_level, uint32_t pos, uint32_t is_use_public_cd)
{
	SkillEffectTable *p_skill_conf = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(skill, level));
	if (NULL == p_skill_conf)
	{
		log_error("fight_hero_t p_skill_conf NULL skill_id:%d skill_level:%d", skill, level);
		return;
	}

	skill_data_t data;
	data.skill_id = skill;
	data.skill_level = level;
	data.skill_cd = init_cd;
	data.type = (proto::common::skill_type)p_skill_conf->type();
	data.is_use_public_cd = is_use_public_cd;		//使用公共CD
	m_skill_manager.add_skill(turn_level, data, pos);
	//log_error("add_skill turn_level:[%u],skill_id[%u],pos[%u]", turn_level, skill, pos);
}

//填充变身技能
void fight_hero_t::fill_turn_skill(const change_model_skills_vec &skill_list) {
	//i表示变身阶段,j表示位置
	for (size_t i = 0; i < skill_list.size(); i++) {
		const skills_vec &skills = skill_list[i];
		for (size_t j = 0; j < skills.size(); j++) {
			skill_info_t *p_skill_info = m_skill_manager.get_skill_info_by_pos(j);
			if (p_skill_info) {//变身改技能
				add_skill(skills[j].skill_id, p_skill_info->get_skill_level(i), 0, i+1, j, skills[j].is_use_public_cd);
			}
			else {//新增技能
				  //TODO	
			}
		}
	}
}

//添加变身技能
void fight_hero_t::add_turn_skill() {
	if ( !is_monster() ) {//英雄
		Hero* hero_conf = GET_CONF(Hero, get_tid());
		if (hero_conf == NULL) {
			log_error("Hero [%u] not exists", get_tid());
			return;
		}

		const change_model_skills_vec &skill_list = hero_conf->get_change_model_skills();
		fill_turn_skill(skill_list);
	}
	else {
		MonsterHero* monster_conf = GET_CONF(MonsterHero, get_tid());
		if (NULL == monster_conf) {
			log_error("monster_conf [%u] is null", get_tid() );
			return;
		}
		const change_model_skills_vec &skill_list = monster_conf->get_change_model_skills();
		fill_turn_skill(skill_list);
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
	proto::common::combat_form_single temp;
	m_cur_form.Swap(&temp);
}

void fight_hero_t::reset_wave_change_data()
{
	set_first_att_order_pos(-1);
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

	clear_att_speed();

	add_combat_act(proto::common::combat_act_type_syn_hp, recover_hp, shared_from_this(), shared_from_this(), get_hp());

	std::vector<skill_info_t> &skill_info_list = m_skill_manager.get_cur_skill_list();
	for( auto skill_info : skill_info_list) {
		const skill_data_t *p_skill_data = skill_info.get_skill_data_by_turn_level(m_change_model_phase);
		if (NULL == p_skill_data) {
			continue;
		}
		add_combat_act( proto::common::combat_act_type_syn_cd, p_skill_data->skill_id, shared_from_this(), shared_from_this(), skill_info.get_skill_cd(p_skill_data->skill_id) );
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
	}
	else
	{
		m_activation_skill = 0;
	}
}

void fight_hero_t::add_immuno_big_buff_type(uint32_t type) {
	m_immuno_big_buff_types.insert(type);
}

void fight_hero_t::add_immuno_small_buff_type(uint32_t type) {
	m_immuno_small_buff_types.insert(type);
}

bool fight_hero_t::is_immuno_big_buff_type(uint32_t type) {
	return m_immuno_big_buff_types.find(type) != m_immuno_big_buff_types.end();
}

bool fight_hero_t::is_immuno_small_buff_type(uint32_t type) {
	return m_immuno_small_buff_types.find(type) != m_immuno_small_buff_types.end();
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

void fight_hero_t::set_combo_count(uint32_t value)
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
		//can_counter = false;
	}

	// 混乱
	if (get_attr(attrtype_enum::confused) > 0)
	{
		select = get_combat()->select_confused_target(shared_from_this());
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

bool fight_hero_t::check_skill_exist(int32_t skill_id)
{
	skill_info_t *p_info = m_skill_manager.get_skill_info(skill_id);
	if (NULL == p_info) {
		log_error("hero:%u get_skill[%u] null",get_tid(), skill_id);
		return false;
	}
		
	if( p_info->get_skill_id(m_change_model_phase) != (uint32_t)skill_id ){
		log_error("hero:%u get_skill[%u] not cur turn_level[%u]null", get_tid(), skill_id, m_change_model_phase );
		return false;
	}
	
	if (p_info->get_skill_cd(skill_id) > 0) {
		log_error("hero:%u get_skill[%u] cd > 0", get_tid(), skill_id);
		return false;
	}

	//检测技能消耗
	SkillEffectTable *p_skill_conf = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(skill_id, p_info->get_skill_level(m_change_model_phase)));
	if (NULL == p_skill_conf) {
		log_error("hero:%u get_skill[%u] null",get_tid(), skill_id);
		return false;
	}

	if (check_skill_consum(p_skill_conf) == false) {
		return false;
	}
	
	return true;
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

uint32_t fight_hero_t::get_best_skill() {
	// 如果有设定自动战斗模式 默认使用普攻
	if (get_auto_fight_mode() == 1) {
		return get_default_skill();
	}
	
	std::vector<SkillEffectTable*> skill_ai_list;
	const std::vector<skill_info_t> &skill_list = m_skill_manager.get_cur_skill_list();
	for (auto skill_info : skill_list) {
		const skill_data_t *p_skill_data = skill_info.get_skill_data_by_turn_level(m_change_model_phase);
		if (NULL == p_skill_data) {
			continue;
		}

		uint32_t skill_id = p_skill_data->skill_id;
		uint32_t skill_level = p_skill_data->skill_level;

		if (!check_skill(skill_id)) {
			continue;
		}

		SkillEffectTable *p_conf = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(skill_id, skill_level));
		if (NULL == p_conf) {
			continue;
		}

		if (check_skill_consum(p_conf) == false) {
			continue;
		}

		//普通技能
		if (p_conf->type() == proto::common::skill_type_atk || p_conf->type() == proto::common::skill_type_sp_skill) {
			skill_ai_list.push_back(p_conf);
		}

		//激活技能
		if (p_conf->type() == proto::common::skill_type_activate && get_activation_skill() == skill_id) {
			skill_ai_list.push_back(p_conf);
		}
	}

	if (skill_ai_list.empty()) {
		return get_default_skill();
	}
	
	uint32_t find_skill = find_skill_ai(skill_ai_list);
		
	return find_skill;
}

bool fight_hero_t::do_auto_fight()
{
	if (!is_auto_fight()) // 自动战斗
		return false;
	
	clear_cur_action();
	
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
			continue;
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
			break;
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

	int32_t random_value = random_util_t::randBetween(1, all_skill_ai_rate);
	if (random_value  == -1 ) {
		log_error("dispel_buff rand error min[%d] > max[%d]", 1, all_skill_ai_rate);
		return 0;
	}

	uint32_t compare_value = 0;
	for (uint32_t i = 0; i < skill_ai_rate_list.size(); ++i)
	{
		compare_value += skill_ai_rate_list[i];
		// roll到了一个
		if ((uint32_t)random_value <= compare_value)
		{
			// 安全校验 避免数组越界 skill_ai_list的顺序跟skill_ai_rate_list的顺序是一致的
			if (i < skill_ai_list.size())
			{
				return skill_ai_list[i]->id();
			}
			else
			{
				log_error("find_skill_ai find_pos[%d] >= skill_ai_list_size[%d]", i, (int32_t)skill_ai_list.size());
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

	// 被控
	if (is_in_control()) {
		log_error("in control can not attack");
		return false;
	}

	if (is_dead()) {
		log_error("dead can not attack");
		return false;
	}

	//判断追击情况
	if (trigger_by_buff) {
		if (NULL != select && select->can_be_follow_attack(shared_from_this()) == false) {
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
		do_team_script_event(event_on_attack_back, target);	
		target->get_count_data().round_phase_num = 0;		//出手完后必须清楚这个次数,不然导致多次反击播放异常
	}
}

size_t fight_hero_t::get_skill_phase(SkillEffectTable* skill_conf)
{
	if ( NULL == skill_conf  ) {
		log_error("skill_conf is NULL");
		return 0;
	}

	size_t end_phase = skill_conf->hurt_rate().size();
	return end_phase;
}

size_t fight_hero_t::get_total_end_phase(SkillEffectTable* skill_conf)
{
	if ( NULL == skill_conf)
	{
		log_error("skill_conf is NULL");
		return 0;
	}

	size_t end_phase = get_skill_phase(skill_conf);
	end_phase += get_count_data().crit_combo_phase;															//暴击加的段数
	end_phase += m_buff_mgr.get_buff_layer_by_tid( get_attr(common::attrtype_enum::phase_buffer_id) );		//多段buff层数
	end_phase += get_count_data().phase_rate_add;

	if (get_attr(common::attrtype_enum::phase_rate) > 0)
	{
		end_phase = std::min<size_t>(get_attr(common::attrtype_enum::phase_max), end_phase);//技能本身的段数+额外增加的段数不能超过配置的最大段数
	}

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

	if (NULL == targets || targets->size() == 0)
	{
		find_target_and_attack(skill_conf, counters, trigger_combo, phase, trigger_by_buff, select, type);
	}
	else
	{
		do_attack_after_find_target(skill_conf, counters, trigger_combo, phase, trigger_by_buff, *targets, spurting_targets, type);
	}

	get_count_data().round_do_attack_num += 1;
	//动态判断是否暴击追打
	while ( ( phase < get_total_end_phase(skill_conf) ) && skill_conf->refind_target())
	{
		get_count_data().round_do_attack_num += 1;
		FIGHT_LOG("[%lu]refind target: camp[%u] pos[%u] tid[%u] phase[%u]", get_fight_uid(), get_camp(), get_pos(), get_tid(), (uint32_t)phase);
		phase += 1;
		if (find_target_and_attack(skill_conf, counters, trigger_combo, phase, trigger_by_buff, select, type, true) == false)
		{
			break;
		}
	}

	// todo
	after_attack(skill_conf, trigger_by_buff);
	
	// 反击和连击走下一次完整的行动
	do_counter(counters, can_counter);

	if (trigger_combo)
	{
		FIGHT_LOG("[%lu]start combo: camp[%u] pos[%u] tid[%u]", get_fight_uid(), get_camp(), get_pos(), get_tid());
		// 连击,执行本次相同的技能
		//修复连击攻击别的玩家
		//do_attack(skill_conf, NULL, proto::common::combat_act_type_combo);
		do_attack(skill_conf, get_cur_target(), proto::common::combat_act_type_combo);
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
	find_target(skill_effect_conf, targets, spurting_targets, type, select);
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
	if (NULL == skill_conf) {
		return false;
	}

	std::vector<uint64_t> act_targets;
	for (auto target : targets)
	{
		if (NULL == target)
		{
			continue;
		}

		act_targets.push_back(target->get_uid());
	}

	if ( (skill_conf->special_sign() & skill_union_atk_type) !=  0 )
		add_combat_skill_act( proto::common::combat_act_type_pincer_attack, skill_conf->id(), get_uid(), act_targets );
	else
		add_combat_skill_act(type, skill_conf->id(), get_uid(), act_targets);

	//do_skill_consum(skill_conf);
	
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
	bool is_refind /*= false*/)//是否播放技能动画，false播，true不播
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

	m_target_count = targets.size();

	size_t init_phase = 0;
	if (targets.size() == 1)//单体技能
	{
		fight_hero_ptr target = targets[0];
		if (target)
		{
			bool trigger_counter = false;
			init_phase = phase;
			if (do_attack_target(target, skill_conf, trigger_counter, trigger_combo, init_phase, trigger_by_buff, type))
			{
				if (trigger_counter)
				{
					counters.push_back(target);
				}
			}
		}
		else
		{
			log_error("single target is null");
		}
	}
	else if (targets.size() > 1)//群攻技能
	{
		do_attack_targets(targets, skill_conf, trigger_combo, trigger_by_buff, counters, type);
	}

	for ( auto target : targets) {
		if (target == NULL) {
			continue;
		}
		do_after_attack_target(target, skill_conf, trigger_by_buff);
	}


	// 所有队友伤害转换为直伤
	if (get_attr(attrtype_enum::all_team_damage_to_injury) > 0) {
		get_count_data().all_team_damage = 0;
	}

	phase = init_phase;

	//溅射伤害
	init_phase = 0;
	for (auto target : spurting_targets)
	{
		if (NULL == target) {
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

int32_t fight_hero_t::get_hero_element_hurt_increase(fight_hero_ptr target)
{
	if (NULL == target)
	{
		return 0;
	}

	uint32_t type = 0;
	// 获取系别免疫属性
	switch (target->get_terry())
	{
	case proto::common::hero_terry_type_water:
		type = attrtype_enum::water_hurt_increase;
		break;
	case proto::common::hero_terry_type_fire:
		type = attrtype_enum::fire_hurt_increase;
		break;
	case proto::common::hero_terry_type_wind:
		type = attrtype_enum::wind_hurt_increase;
		break;
	case proto::common::hero_terry_type_yin:
		type = attrtype_enum::yin_hurt_increase;
		break;
	case proto::common::hero_terry_type_yang:
		type = attrtype_enum::yang_hurt_increase;
		break;
	default:
		break;
	}
	return get_attr(type);
}

int32_t fight_hero_t::get_hero_element_hurt_reduce(fight_hero_ptr attacker)
{
	if (NULL == attacker)
		return 0;

	uint32_t type = 0;
	// 获取系别免疫属性
	switch (attacker->get_terry())
	{
	case proto::common::hero_terry_type_water:
		type = attrtype_enum::water_hurt_reduce;
		break;
	case proto::common::hero_terry_type_fire:
		type = attrtype_enum::fire_hurt_reduce;
		break;
	case proto::common::hero_terry_type_wind:
		type = attrtype_enum::wind_hurt_reduce;
		break;
	case proto::common::hero_terry_type_yin:
		type = attrtype_enum::yin_hurt_reduce;
		break;
	case proto::common::hero_terry_type_yang:
		type = attrtype_enum::yang_hurt_reduce;
		break;
	default:
		break;
	}
	
	return get_attr(type);
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
	if ( NULL == target ) {
		log_error("target is null");
		return;
	}

	// 是否发生暴击
	int crit = get_attr(attrtype_enum::crit_rate); // 自己的属性30

	uint32_t terry = get_terry();
	if (get_attr(attrtype_enum::temp_terry) > 0)
	{
		terry = get_attr(attrtype_enum::temp_terry);
	}
	RestraintTable *p_terry_conf = GET_CONF(RestraintTable, hash_util_t::hash_to_uint32(terry, target->get_terry()));
	int crit_fix = 0;
	if (p_terry_conf != NULL)
		crit_fix = p_terry_conf->crit_fix();

	if ((crit + crit_fix) > 0 && random_util_t::selectByTenTh(crit + crit_fix))
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

		do_team_script_event(event_on_crit_before, shared_from_this());
		target->do_team_script_event( event_on_be_crit_before, shared_from_this() );
	}
}

bool fight_hero_t::do_skill_heal(size_t phase, SkillEffectTable* skill_conf, fight_hero_ptr target, int heal_crit_multiple)
{

	if ( NULL == skill_conf ){
		log_error("skill_conf is null");
		return false;
	}

	if (NULL == target) {
		log_error("target is null");
		return false;
	}

	float skill_heal = get_attr(attrtype_enum::skill_heal) * 0.0001;
	int all = (int)(get_attack() * (((float)skill_conf->hurt_rate(phase) * 0.0001) + get_attr(attrtype_enum::additive_damage)) * (skill_heal));

	//技能治疗效果万分比/10000
	m_calc_result.heal = all;

	if (m_calc_result.crit)
	{
		m_calc_result.heal *= (heal_crit_multiple * 0.0001);
	}

	// 执行加血操作
	//todo
	m_calc_result.heal = target->heal_hp(shared_from_this(), m_calc_result.heal);
	if (m_calc_result.crit)
	{
		add_combat_act(proto::common::combat_act_type_heal_crit, m_calc_result.heal, target, shared_from_this(),
			target->get_attr(attrtype_enum::hp));
	}
	else
	{
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
	if (NULL == target) {
		log_error("target is NULL");
		return 0;
	}

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
	if (NULL == skill_conf) {
		log_error("skill_conf is NULL");
		return 0;
	}

	if (NULL == target){
		log_error("target is NULL");
		return 0;
	}
	
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
	if (get_attr(attrtype_enum::blood_volume_to_attack_ratio_skill) > 0)
	{
		float ratio = (float)target->get_hp() / (float)target_max_hp;
		float need_ratio = get_attr(attrtype_enum::blood_volume_to_attack_limit_skill) * 0.0001;
		if (ratio > need_ratio)
		{
			ret *= (1 + get_attr(attrtype_enum::blood_volume_to_attack_ratio_skill) * 0.0001);
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
	if (NULL == target) {
		log_error(" target is NULL ");
		return 0;
	}
	
	int ret = target->get_attr(attrtype_enum::def_physical);
	// 血量百分比转换防御
	if (target->get_attr(attrtype_enum::lose_blood_volume_to_defend) > 0)
	{
		if (get_max_hp() == 0) {
			return 0;
		}

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
	if (NULL == target) {
		log_error("target is NULL");
		return 0;
	}

	if (NULL == skill_conf) {
		log_error("skill_conf is NULL");
		return 0;
	}

	uint32_t terry = get_terry();
	if (get_attr(attrtype_enum::temp_terry) > 0 )
	{
		terry = get_attr(attrtype_enum::temp_terry);
	}

	int terry_param = 1;
	RestraintTable *p_terry_conf = GET_CONF(RestraintTable, hash_util_t::hash_to_uint32(terry, target->get_terry()));
	if (p_terry_conf != NULL)
		terry_param = p_terry_conf->restraint();

	int32_t pvp_add_param = 0;		// pvp增伤万分比
	int32_t pvp_dec_param = 0;		// pvp减伤万分比
	
	get_pvp_param(pvp_add_param, pvp_dec_param, target);


	float ret = 1;
	ret += SAFE_SUB( get_attr(attrtype_enum::damage_bonus), target->get_attr(attrtype_enum::get_damage_reduce) ) * 0.0001;
	
	ret += (pvp_add_param + pvp_dec_param) * 0.0001;


	// 判断嘲讽减伤
	if (get_attr(attrtype_enum::taunt) > 0)
	{
		//ret = SAFE_SUB( ret , ( target->get_attr(attrtype_enum::taunt_damage_reduce) * 0.0001) );
		//修改为只针对对应嘲讽的人
		uint32_t taunt_damage_reduce = 3500;
		if (target->get_uid() == get_taunt_me_hero()) {
			ret = SAFE_SUB(ret, (ret * (taunt_damage_reduce) / 10000.0f));
		}
	}

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

	// 对于处于控制状态的额外伤害，处于幻境状态的武将不会受到伤害，更不会有额外伤害
	if (target->is_in_control() && !target->is_in_mirage())
	{
		ret += get_attr(attrtype_enum::in_control_attack_addition) * 0.0001;
	}

	//斩杀技能对boss额外伤害
	ret += m_buff_element_mgr.get_buff_attr_percent(attrtype_enum::execute_boss_extra_damage) * 0.0001;
	
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

	// 计算目标属性的减免
	//ret = SAFE_SUB( ret , target->get_hero_element_hurt_reduce(shared_from_this())  * 0.0001 );

	//系别额外伤害(自己的增伤属性-目标的减伤属性）
	int element_hurt_ratio = get_hero_element_hurt_increase(target) - target->get_hero_element_hurt_reduce(shared_from_this());

	return ret * std::max<int>(terry_param + element_hurt_ratio , 0) * 0.0001;
}

// 计算伤害
int fight_hero_t::calc_injury(
	size_t phase,
	SkillEffectTable* skill_conf,
	fight_hero_ptr target,
	proto::common::combat_act_type type,
	int crit_multiple)
{
	if (NULL == target) {
		log_error("target is NULL");
		return 0;
	}

	uint32_t damage = 0;

	// 计算真实伤害
	uint32_t final_real_damage = std::max<int>((get_attr(attrtype_enum::true_damage) - target->get_attr(attrtype_enum::true_def)), 0);

	int attack = calc_attack(phase, skill_conf, target, type, crit_multiple);
	int defend = calc_defend(phase, skill_conf, target, type, crit_multiple);

	int total_fight_constant = get_attr(common::attrtype_enum::fight_constant) + combat_t::get_init_fight_constant();
	
	float skill_ratio = (((get_attr(attrtype_enum::extra_skill_ratio) + skill_conf->hurt_rate(phase))) * 0.0001);

	//2018.5.8日 李瑞说战斗常数不会配0
	float fight_constant_ratio = total_fight_constant /(float)(total_fight_constant + defend *(1 - std::min<int>( (get_attr(attrtype_enum::def_cut_per) * 0.0001), 1)));

	//免伤率
	float hurt_reduce = target->get_attr(attrtype_enum::damage_reduce) * 0.0001f;

	damage = std::max<int>( (((attack * skill_ratio)+get_attr(attrtype_enum::additive_damage)) * fight_constant_ratio * (SAFE_SUB(1 , hurt_reduce)) ), ceil( attack * 0.01 ) ) ;
		
	damage += final_real_damage;

	if (m_calc_result.crit)
	{
		damage *= (crit_multiple * 0.0001);
	}

	float percent = calc_injury_addition_percent(phase, skill_conf, target, type, crit_multiple);

	damage = std::max<int>(damage * percent, final_real_damage);
	return damage;
}

void fight_hero_t::deal_with_shield(fight_hero_ptr target, int32_t &damage, int32_t &absorb ,attrtype_enum::attr_type type, uint32_t buff_small_type) {
	if (NULL == target) //不需要判断damage是否为0，刘彻吸收队友护盾的时候传入的damage为0
	{
		return;
	}

	int32_t target_shield_point = target->get_attr(type);
	// 清空目标的护盾值
	if (damage > target_shield_point) {
		absorb += target_shield_point;
		target->sub_attr(type, absorb);
		damage -= target_shield_point;
	} else {
	// 设置护盾值为扣除伤害后的值
		absorb += damage;
		target->sub_attr(type, absorb);
		damage = 0;
	}

	// 判断是否盾破了
	if (target->get_attr(type) <= 0) {
		target->m_buff_mgr.remove_buff_by_small_type( buff_small_type );
	}
}


void fight_hero_t::do_target_shield_points(fight_hero_ptr target, int32_t &damage, int32_t &absorb )
{
	if (NULL == target) {
		log_error("target is NULL");
		return;
	}

	uint64_t target_obj_uid = target->get_master();
	fight_obj_ptr p_target_obj = target->get_combat()->find_obj(target_obj_uid);
	if (NULL == p_target_obj)
	{
		log_error("do_target_shield_points hero[%lu][%d] obj[%lu] not find", target->get_uid(), target->get_tid(), target_obj_uid);
		return;
	}

	//项羽盾, 502,5022,5021 buff_小类型
	deal_with_shield( target, damage, absorb,  attrtype_enum::xiangyu_shield_points, buff_small_type_xyd );

	//气运盾
	deal_with_shield( target, damage, absorb, attrtype_enum::luck_shield_points, buff_small_type_luckshield );

	//普通盾
	deal_with_shield( target, damage, absorb, attrtype_enum::shield_points, buff_small_type_shield );

	if (damage == 0) {
		return;
	}

	if (p_target_obj->get_team_shield() > 0 )
	{
		if ( damage > p_target_obj->get_team_shield())
		{
			absorb += p_target_obj->get_team_shield();
			p_target_obj->set_team_shield(0);
			damage -= p_target_obj->get_team_shield();
		}
		else
		{
			absorb += damage;
			p_target_obj->set_team_shield( p_target_obj->get_team_shield() - damage );
			damage = 0;
		}

		// 盾破了
		if (p_target_obj->get_team_shield() == 0)
		{
			fight_hero_ptr p_buff_owner = target->get_combat()->find_hero(p_target_obj->get_team_shield_hero_uid());
			if (NULL == p_buff_owner)
			{
				log_error("do_target_shield_points p_buff_owner NULL uid[%lu]", p_target_obj->get_team_shield_hero_uid());
				return;
			}
			p_buff_owner->m_buff_mgr.remove_buff_by_small_type(p_target_obj->get_team_shield_buff_type());
		}
	}

	// 所有队友伤害转换为直伤
	if ( damage > 0 && target->get_attr(attrtype_enum::all_team_damage_turn) > 0)
	{
		int32_t value = damage * (target->get_attr(attrtype_enum::all_team_damage_turn) * 0.0001);
		fight_hero_ptr p_owner = target->get_combat()->find_hero(p_target_obj->get_team_damage_turn_hero_uid());
		if (NULL == p_owner)
		{
			log_error("do_target_shield_points p_owner NULL uid[%lu]", p_target_obj->get_team_shield_hero_uid());
			return;
		}
		p_owner->get_count_data().all_team_damage_turn += value;
		damage = SAFE_SUB(damage, value);
	}

	//add_combat_act(proto::common::combat_act_type_absorb, m_calc_result.absorb, target, shared_from_this());
	FIGHT_LOG("[%lu]target absorb [%d] [%d]", target->get_fight_uid(), absorb, target->get_attr(attrtype_enum::shield_points));	
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
		if (real_atk == -1) {
			log_error("dispel_buff rand error min[%d] > max[%d]", atk, max_atk);
		}
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
	if (NULL == target) {
		log_error("target is NULL");
		return 0;
	}

	if (NULL == skill_conf) {
		log_error("skill_conf is NULL");
		return 0;
	}

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

void fight_hero_t::do_attack_result_notify(proto::common::combat_act_type type, fight_hero_ptr target , SkillEffectTable *p_skill_conf )
{
	if (m_calc_result.damage == 0 && m_calc_result.absorb == 0){
		log_error("hero[%u] do attack result is[0]", get_tid());
		return;
	}
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

	//客户端说要放到后面通知
	fight_hero_ptr p_bear_damage_hero = target->get_bear_damage_target();
	if (p_bear_damage_hero) {
		target->add_combat_act(proto::common::combat_act_type_share_damage, target->m_bear_damage, p_bear_damage_hero, shared_from_this(), p_bear_damage_hero->get_attr(attrtype_enum::hp));
		target->m_bear_damage = 0;
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
	if (NULL == target) {
		log_error("target is NULL");
		return false;
	}

	if (NULL == skill_conf) {
		log_error("skill_conf is NULL");
		return false;
	}

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

	m_calc_result.damage = std::max<int>(  m_calc_result.damage, ceil( get_attr(attrtype_enum::atk)* 0.01));

	// 吸收前
	target->get_script_proxy()->before_absorb(get_script_proxy(), skill_conf->id(), m_calc_result.damage);

	//护盾计算丢入到最终扣血中
	do_target_shield_points(target, m_calc_result.damage, m_calc_result.absorb);

	// 执行伤害扣血操作
	do_skill_buff(NONE_PHASE, event_on_hit_target, skill_event_target_type_all, skill_conf, target);
	do_team_script_event(event_on_hit_target, shared_from_this());	
	
	target->on_be_attack(m_calc_result.damage, shared_from_this(), skill_conf->id());

	do_attack_result_notify(type, target, skill_conf );

	// 先给客户端发完本次伤害结果 再触发暴击事件
	if (m_calc_result.crit)
	{
		do_skill_buff(NONE_PHASE, event_on_critical_strike, skill_event_target_type_all, skill_conf, target);
		do_team_script_event(event_on_critical_strike, shared_from_this());
		target->do_team_script_event(event_on_becritical_strike, target);
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
	if( NULL == target ){
		log_error("target is null");
		return false;
	}

	// 判断连击，一次技能攻击（包括群攻、多段）只算一次连击概率
	if (!get_count_data().is_calc_combo)
	{
		get_count_data().is_calc_combo = true;

		int combo_rate = get_attr(attrtype_enum::combo_rate); // 自己的属性71
		if (random_util_t::selectByTenTh(combo_rate))
		{
			return true;
		}
	}

	// 连击BUFF
	if (get_attr(attrtype_enum::combo_buffer_id) > 0 || target->get_attr(attrtype_enum::be_combo_buffer_id) > 0)
	{
		uint32_t layer = m_buff_mgr.get_buff_layer_by_tid(get_attr(attrtype_enum::combo_buffer_id)) + target->m_buff_mgr.get_buff_layer_by_tid(get_attr(attrtype_enum::be_combo_buffer_id));
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
	if (NULL == skill_conf) {
		log_error("skill_conf is null");
		return false;
	}

	if (skill_conf->is_ignore_damage(phase) == true) {
		log_error("hero[%u] do skill hurt phase[%lu] is ignore", get_tid(), phase);
		return false;
	}

	// 计算生成本次攻击力
	int real_atk = 0;
	int atk = get_attr(attrtype_enum::atk); // 获取10
	int atk_range = get_attr(attrtype_enum::atk_range); // 获取11
	int max_atk = std::max<int>(atk * ( atk_range * 0.0001 ), 1); // 攻击力*攻击浮动/10000

	real_atk = random_util_t::randBetween(atk, max_atk);
	if (-1 == real_atk) {
		log_error("dispel_buff rand error min[%d] > max[%d]", atk, max_atk);
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

void fight_hero_t::change_model(uint32_t change_type, uint32_t change_phase, uint32_t change_skill_level, uint32_t resource_id)
{
	//收回旧的被动效果
	recovery_special_buff();
	clear_passive_buff();
	clear_all_specail_buff();

	m_change_model_phase = change_phase;
	m_skill_manager.set_turn_level(change_phase);

	//兼容旧代码
	const std::vector<skill_info_t> &skill_list = m_skill_manager.get_cur_skill_list();
	for (auto skill_info : skill_list) {
		uint32_t skill_id = skill_info.get_skill_id(change_phase);
		add_combat_act(proto::common::combat_act_type_add_skill, skill_id, shared_from_this(),
											shared_from_this(),skill_info.get_skill_level(skill_id), skill_info.get_skill_cd(skill_id),
											resource_id);
		log_error("sync changemod turn_level[%u] skill[%u], level:[%u], cd:[%u]", change_phase, skill_id,
			skill_info.get_skill_level(skill_id), skill_info.get_skill_cd(skill_id));
	}

	add_combat_act(proto::common::combat_act_type_change_model, m_change_model_phase, shared_from_this(), 
						  shared_from_this(), change_type, change_skill_level, resource_id);

	//通知技能变化
	add_skill_cd_notify();

	// 增加新的被动效果
	if (NULL != get_combat()) {
		get_combat()->set_public_buff_on(get_uid());
		//get_combat()->distribute_special_buff();
	}

	activation_passive_buff();
}

//还原变身
void fight_hero_t::restore_model()
{
	m_change_model_phase = 0;
	m_skill_manager.set_turn_level(0);
	add_combat_act(proto::common::combat_act_type_change_model, 0, shared_from_this(), shared_from_this());
}

bool fight_hero_t::is_single_attack(SkillEffectTable* skill_conf)
{
	if( NULL == skill_conf ){
		log_error("skill_conf is NULL");
		return false;
	}

	if ( skill_conf->target_type() == skill_attack_target_type_select 
		 || skill_conf->attack_target() == skill_attack_target_type_default 
		 || skill_conf->attack_target() == skill_attack_target_type_min_hp) {
		return true;
	}
	return false;
}

bool fight_hero_t::is_mutilp_attack(SkillEffectTable* skill_conf) {
	if( NULL == skill_conf ){
		log_error("skill_conf is NULL");
		return false;
	}
	if (skill_conf->target_type() == skill_attack_target_type_random
		|| skill_conf->target_type() == skill_attack_target_type_all ) {
		return true;
	}
	return false;
}

bool fight_hero_t::do_dodge(SkillEffectTable* skill_conf,fight_hero_ptr target)
{
	if (NULL == target || NULL == skill_conf ) {
		return false;
	}
		
	if ( do_equip_dodge( skill_conf , target ) == true ) {
		add_combat_act(proto::common::combat_act_type_dodge, 1, target, shared_from_this());
		return true;
	}

	int anti_dogge = get_attr(attrtype_enum::anti_dodge);		// 抗闪避率
	int target_dodge = target->get_attr(attrtype_enum::dodge); // 获取 目标的闪避值70
	int result = SAFE_SUB(target_dodge, anti_dogge);

	if (result < 0) {
		result = 0;
	}

	if (random_util_t::selectByTenTh(result)) {
		m_calc_result.dodge = true;
		FIGHT_LOG("[%lu]target dodge", get_fight_uid());
		// 闪避成功
		//add_combat_act(proto::common::combat_act_type_dodge, 0, target->get_uid());
		add_combat_act(proto::common::combat_act_type_dodge, 1, target, shared_from_this());
		return true;
	}

	uint32_t dodge_single_target_damage = target->get_attr(attrtype_enum::dodge_single_target_damage);
	if (dodge_single_target_damage > 0) {
		if ( is_single_attack( skill_conf ) && random_util_t::selectByTenTh(dodge_single_target_damage) ) {
				m_calc_result.dodge = true;
				FIGHT_LOG("[%lu]target dodge", get_fight_uid());
				add_combat_act(proto::common::combat_act_type_dodge, 1, target, shared_from_this());
				return true;
		}
	}
	return false;
}

bool fight_hero_t::do_equip_dodge(SkillEffectTable* skill_conf, fight_hero_ptr target) {
	if (NULL == skill_conf) {
		log_error("skill conf is NULL");
		return false;
	}

	if (NULL == target) {
		log_error("target is null");
		return false;
	}

	if (target->get_count_data().equip_miss_all_atk_count > 0) {
		target->get_count_data().equip_miss_all_atk_count -= 1;
		return true;
	}

	if (target->get_count_data().equip_miss_single_atk_count > 0 && is_single_attack(skill_conf)) {
		target->get_count_data().equip_miss_single_atk_count -= 1;
		return true;
	}

	if (target->get_count_data().equip_miss_mutilp_atk_count > 0 && is_mutilp_attack(skill_conf)) {
		target->get_count_data().equip_miss_mutilp_atk_count -= 1;
		return true;
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
	if (NULL == target) {
		log_error("target is NULL");
		return false;
	}

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
	if (NULL == target) {
		return false;
	}

	if (check_do_attack_target(target, skill_conf, phase, trigger_by_buff, type) == false)
		return false;
	
	m_cur_target = target;
	target->set_cur_attacker(shared_from_this());

	FIGHT_LOG("[%lu]attack target: camp[%u] pos[%u] tid[%u]", get_fight_uid(), target->get_camp(), target->get_pos(), target->get_tid());
	
	set_attack_step(proto::common::combat_act_step_before_attack_target);

	do_skill_buff(NONE_PHASE, event_on_attack_begin, skill_event_target_type_target, skill_conf, target);
	do_skill_buff(NONE_PHASE, event_on_attack_one_target_begin, skill_event_target_type_all, skill_conf, target);
	do_team_script_event(event_on_attack_begin, shared_from_this());
	target->do_team_script_event(event_on_behurt_begin, target);

	//bool combo = false;
	bool counter = false;
	
	//获取总共能打的段数,这样可以动态增加多次追打
	size_t init_phase = phase;
	size_t min_phase = init_phase;
	//size_t max_phase = get_total_end_phase( skill_conf );//do_phase_hurt里可能会修改phase_add_rate,所以不能用变量直接作为循环最大数
	for (size_t i = init_phase; i < get_total_end_phase(skill_conf); ++i, ++min_phase) {
		phase = min_phase;
		bool ret = do_phase_hurt(target, skill_conf, trigger_counter, trigger_combo, min_phase, trigger_by_buff, type);
		clear_phase_fight_info();
		target->clear_phase_fight_info();
		if (false == ret) {
			break;
		}
	}

	size_t skill_phase = get_skill_phase(skill_conf);
	add_combat_act(proto::common::combat_act_type_syn_atk_count, skill_phase, target, shared_from_this(), get_count_data().round_phase_num );

	if (get_cur_target() != NULL)
		get_cur_target()->set_bear_damage(0, NULL);

	// 判断对方是否死亡,死亡则暂不触发反击
	//todo
	if (!target->is_dead() && type != proto::common::combat_act_type_counter) {
		int max_counter_count = target->get_attr(attrtype_enum::counter_max); // 目标的属性74
		if (counter && target->counter_count() < (uint32_t)max_counter_count) {
			target->counter_count(target->counter_count() + 1);
			// 反击,执行反击技能
			trigger_counter = true;
			FIGHT_LOG("[%lu]trigger counter", get_fight_uid());
		}
	}

	//逻辑错误，combo变量始终是false
	// 判断自己是否死亡,死亡则暂不触发连击
	// todo
// 	if (!is_dead())
// 	{
// 		int max_combo_count = get_max_combo_count( target ); // 自己的属性72
// 		if (combo && combo_count() < (uint32_t)max_combo_count) // 触发连击
// 		{
// 			combo_count(combo_count() + 1);
// 			// 连击,执行本次相同的技能
// 			trigger_combo = true;
// 			FIGHT_LOG("[%lu]trigger combo", get_fight_uid());
// 		}
// 	}

	return true;
}


int fight_hero_t::get_max_combo_count(fight_hero_ptr target) {
	if (NULL == target) {
		return 0;
	}

	int max_count = 0;

	if (get_attr(attrtype_enum::combo_buffer_id) == 0 && target->get_attr(attrtype_enum::be_combo_buffer_id) == 0 && get_attr(attrtype_enum::combo_rate) == 0)
		return max_count;

	max_count += get_max_combo_count_by_self();

	max_count += target->get_max_comba_count_by_enemy();

	//一开始的设计是combo_max 作为连击数，用着用着就变了，目前有连击率的武将，都取这个值作为连击数
	if (get_attr(attrtype_enum::combo_rate) > 0) 
	{
		max_count = (int)get_attr(attrtype_enum::combo_max);
	}

	//触发连击的英雄一定会有这个属性？
	if (max_count > (int)get_attr(attrtype_enum::combo_max))
		max_count = (int)get_attr(attrtype_enum::combo_max);
	
	return max_count;
}

int fight_hero_t::get_max_combo_count_by_self()
{
	int max_count = 0;
	// 连击BUFF
	if (get_attr(attrtype_enum::combo_buffer_id) > 0 ) {
		max_count = m_buff_mgr.get_buff_layer_by_tid(get_attr(attrtype_enum::combo_buffer_id));
	}
	return max_count;
}

int fight_hero_t::get_max_comba_count_by_enemy()
{
	int max_count = 0;
	// 连击BUFF
	if (get_attr(attrtype_enum::be_combo_buffer_id) > 0) {
		max_count = m_buff_mgr.get_buff_layer_by_tid(get_attr(attrtype_enum::be_combo_buffer_id));
	}
	return max_count;
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

	combat_ptr p_combat = get_combat();
	if (NULL == p_combat) {
		log_error("p_combat is NULL");
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
	if (target->is_dead())
	{
		target->on_dead(shared_from_this(), skill_conf);
	}
	// 分摊伤害会导致队友也死亡 所以要判断队友
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
			if (hero->is_dead())
			{
				hero->on_dead(shared_from_this(), skill_conf);
			}
		}
	}
	
	if (!is_buff_skill(skill_conf->id()))
		target->do_team_script_event(event_on_behurt_end, target);

	m_attack_targets[target->get_uid()]=target;

	return true;
}

/*
* param1 针对12，13 表示模型id去处后2位的值
*/

void fight_hero_t::get_target_by_buff_select_target(fight_hero_ptr select, int type, int rand_count, std::vector<fight_hero_ptr>& out, uint32_t param1)
{
	if (NULL == get_combat()) {
		return;
	}

	switch( type )
	{
		case skill_buff_target_type_target:
		{
			if( select != NULL )
				out.push_back(select);
			//return get_cur_target();
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
		case skill_buff_target_type_min_hp_except_mine:
		{
			fight_hero_ptr target = get_combat()->select_min_hp_target(get_camp(), shared_from_this()-> get_uid());
			if (target != NULL)
				out.push_back(target);
		}
		break;
		case skill_buff_target_type_enmey_max_hp:
		{
			fight_hero_ptr select = get_combat()->select_max_hp_target(get_enemy_camp());
			if ((select != NULL))
				out.push_back(select);
		}
		break;
		case skill_buff_target_type_attack_me:
		{
			fight_hero_ptr attacker = get_cur_attacker();
			if (attacker != NULL)
				out.push_back(attacker);
		}
		break;
		case skill_buff_target_type_friend_dead_rand:
		{
			std::vector<fight_hero_ptr> friends;
			fight_hero_ptr p_fight_hero = fight_hero_ptr();
			get_combat()->get_camp_heros(friends, get_camp(), get_pos());
			std::random_shuffle(friends.begin(), friends.end());
			for (uint32_t j = 0; j < friends.size(); ++j)
			{
				p_fight_hero = friends[j];
				if (NULL == p_fight_hero) {
					continue;
				}
				if (p_fight_hero->is_dead())
					out.push_back(p_fight_hero);

				if ((int)out.size() >= rand_count)
					break;
			}
		}
		break;
		case skill_buff_target_type_friend_hero_id:
		{
			std::vector<fight_hero_ptr> heros;
			fight_hero_ptr p_fight_hero = fight_hero_ptr();
			if (get_combat()->get_camp_heros(heros, get_camp()))
			{
				for (uint32_t j = 0; j < heros.size(); ++j)
				{
					p_fight_hero = heros[j];
					if (NULL == p_fight_hero) {
						log_error("p_fight_hero NULL pos[%d]", j);
						continue;
					}
					if( (p_fight_hero->get_resource_id() / 100) == param1 )
						out.push_back(p_fight_hero);
				}
			}
		}
		break;
		case skill_buff_target_type_enemy_hero_id:
		{
			std::vector<fight_hero_ptr> heros;
			fight_hero_ptr p_fight_hero = fight_hero_ptr();
			if (get_combat()->get_camp_heros(heros, get_enemy_camp()))
			{
				for (uint32_t j = 0; j < heros.size(); ++j)
				{
					p_fight_hero = heros[j];
					if (NULL == p_fight_hero)
					{
						log_error("p_fight_hero NULL pos[%d]", j);
						continue;
					}
					if( (p_fight_hero->get_resource_id() / 100) == param1 )
						out.push_back(p_fight_hero);
				}
			}
		}
		break;
		case skill_buff_target_type_cur_action_target:
		{
			combat_ptr p_combat = get_combat();
			if (NULL == p_combat) {
				return;
			}

			if (NULL == p_combat->get_attack_hero()) {
				return;
			}
			out.push_back(get_combat()->get_attack_hero());
		}
		break;
		case skill_buff_target_type_hurt_me_most:
		{
			fight_hero_ptr target = get_hurt_most_alive_enemy();
			if (target)
			{
				out.push_back(target);
			}
			else
			{
				log_error("find buff target type hurt me most null");
				return;
			}
		}
		break;
		case skill_buff_target_type_select_by_client:
		{
			fight_hero_ptr p_selected = get_form_selected_hero();
			if (!p_selected)
			{
				return;
			}
			if (p_selected->is_dead())
			{
				return;
			}
			out.push_back(p_selected);
		}
		break;
		default:
		{
			log_error("type error");
			break;
		}
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

bool fight_hero_t::do_effect_hit_result(fight_hero_ptr attacker, fight_hero_ptr target)
{
	if ( NULL == target || NULL == attacker ) {
		return false;
	}

	//计算系别克制
	uint32_t atk_terry = attacker->get_terry();
	if ( attacker->get_attr(attrtype_enum::temp_terry) > 0) {
		atk_terry = attacker->get_attr(attrtype_enum::temp_terry);
	}

	uint32_t def_terry = target->get_terry();
	int hit_fix = 0;
	RestraintTable *p_terry_conf = GET_CONF( RestraintTable, hash_util_t::hash_to_uint32( atk_terry, def_terry) );
	if (p_terry_conf) {
		hit_fix = p_terry_conf->hit_fix();	
	}

	//log_error("atk_terry[%u], def_terry[%u], hit_fix[%u]", atk_terry, def_terry, hit_fix);
	//查看系别克制关系
	if ( hit_fix > 0 && random_util_t::selectByTenTh( hit_fix )) {
		return false;
	}

	//计算效果抵抗	
	auto skill_effect_max_conf = GET_CONF(Comprehensive, comprehensive_common::skil_effect_max);
	int skill_effect_max = GET_COMPREHENSIVE_VALUE_1(skill_effect_max_conf);
	auto skill_effect_min_conf = GET_CONF(Comprehensive, comprehensive_common::skil_effect_min);
	int skill_effect_min = GET_COMPREHENSIVE_VALUE_1(skill_effect_min_conf);

	int effect_hit_rate = attacker->get_attr(attrtype_enum::effect_hit_rate);				//效果命中
	int effect_dodge_rate = target->get_attr(attrtype_enum::effect_dodge_rate);	//效果抵抗

	int32_t base_hit = std::min<int>(effect_dodge_rate, 10000) -std::min<int>(effect_hit_rate, 10000);
	uint32_t result = std::min<int>(std::max<int>(base_hit, skill_effect_max), skill_effect_min);


	//log_error("atk_hit_rate[%d], def_dodge_rate[%d],  effect_min[%u], effect_max[%u], result[%u] ", effect_hit_rate, effect_dodge_rate, skill_effect_min, skill_effect_max, result);
	if ( random_util_t::selectByTenTh( result ) ) {
		target->add_combat_act( proto::common::combat_act_type_buff_oppose, 1, target, attacker );
		target->do_team_script_event(event_on_resistance, target);
		return false;
	}

	return true;
}


void fight_hero_t::do_add_skill_buff(fight_hero_ptr p_target, fight_hero_ptr p_attacker, uint32_t skill_id,
	bool b_use_rate, uint32_t buff_add_rate, const buff_info_vec& buffs)
{
	if (NULL == p_target)
	{
		log_error("fight_hero_t p_hero NULL");
		return;
	}
	if (NULL == p_attacker)
	{
		log_error("fight_hero_t p_attacker NULL");
		return;
	}

	for (uint32_t i = 0; i < buffs.size(); ++i) {
		BuffTable *p_buff_conf = GET_CONF(BuffTable, buffs[i].first);
		if (NULL == p_buff_conf) {
			log_error("fight_hero_t p_buff_conf NULL id[%d]", buffs[i].first);
			continue;
		}

		//计算技能概率
		if ( random_util_t::selectByTenTh(buff_add_rate) == false ) {
			continue;
		}

		p_target->m_buff_mgr.add_buff(buffs[i].first, p_attacker->get_uid(), skill_id, buffs[i].second);
	}
}


/*
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

	uint32_t terry = get_terry();
	if (get_attr(attrtype_enum::temp_terry) > 0)
	{
		terry = get_attr(attrtype_enum::temp_terry);
	}
	RestraintTable *p_terry_conf = GET_CONF(RestraintTable, hash_util_t::hash_to_uint32(terry, p_hero->get_terry()));
	int hit_fix = 0;
	if (p_terry_conf != NULL)
		hit_fix = p_terry_conf->hit_fix();

	int effect_hit_rate = get_attr(attrtype_enum::effect_hit_rate) + hit_fix;
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
				add_rate = std::max<int>(std::min<int>(buff_add_rate * ((10000.f + effect_hit_rate) / (10000.f)), skill_effect_max), skill_effect_min);
				// 第一次计算未命中
				if (!random_util_t::selectByTenTh(add_rate))
				{
					continue;
				}
				add_rate = std::max<int>(std::min<int>(buff_add_rate * ((10000.f + effect_hit_rate) / (10000.f + effect_dodge_rate)), skill_effect_max), skill_effect_min);
			}
			else
			{
				add_rate = std::max<int>(std::min<int>(buff_add_rate * ((10000.f + effect_hit_rate) / 10000.f), skill_effect_max), skill_effect_min);
			}
		}
		else
		{
			add_rate = buff_add_rate;
		}

		if (add_rate == 0 || !random_util_t::selectByTenTh(add_rate))
		{
			// 未命中 飘字
			if (p_buff_conf->buff_formula() == buff_formula_type_debuff)
			{
				p_hero->add_combat_act(proto::common::combat_act_type_buff_oppose, 1, p_hero, p_attacker);
				// 计算效果抵抗并且效果抵抗不为0
				if (b_use_rate)
				{
					p_hero->do_team_script_event(event_on_resistance, p_hero);
				}
			}
			continue;
		}

		p_hero->m_buff_mgr.add_buff(buffs[i].first, p_attacker->get_uid(), skill_id, buffs[i].second);
		FIGHT_LOG("[%lu] add_buff hero[%lu:%d] buff[%d:%d]", get_fight_uid(), p_hero->get_uid(), p_hero->get_tid(), buffs[i].first, buffs[i].second);
	}
	return true;
}
*/
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
	const std::vector< skill_info_t > &skill_list = m_skill_manager.get_cur_skill_list();
	for (auto skill_info : skill_list) {
		const skill_data_t *p_skill_data = skill_info.get_skill_data_by_turn_level(m_change_model_phase);
		if (NULL == p_skill_data) {
			continue;
		}

		if(p_skill_data->type == proto::common::skill_type_atk)
			return p_skill_data->skill_id;
	}
	return -1;
}

int32_t fight_hero_t::get_counter_skill()
{
	return get_default_skill();
}

SkillEffectTable* fight_hero_t::get_skill_effect(uint32_t skill_id)
{
	//兼容历史代码
	uint32_t skill_level = m_skill_manager.get_skill_level(skill_id);
	if (skill_level == 0) {	
		skill_level = 1;
	}
	return GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(skill_id, skill_level));
}

bool fight_hero_t::find_target(SkillEffectTable* skill_conf, std::vector<fight_hero_ptr>& targets, std::vector<fight_hero_ptr>& spurting_heroes, proto::common::combat_act_type type, fight_hero_ptr select)
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

	if (targets.empty())
	{
		if (!get_combat()->find_target(shared_from_this(), skill_conf, targets, spurting_heroes, type, select))
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

bool fight_hero_t::can_relive()
{
// 处于死亡状态，或者灵魂状态都是可以复活的(如果不能复活属性>0 不能复活)
	if ( !is_dead() || get_attr(attrtype_enum::no_revive_condition) > 0) {
		return false;
	}
	return true;
}

bool fight_hero_t::is_dead()
{
	if (m_dead_flag)
		return true;

	if (get_attr(attrtype_enum::hp) <= 0 && get_attr(attrtype_enum::soul_condition) == 0)
		return true;

	return false;
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

	if (get_control_by_system() != 0)
		return true;

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
	if (get_attr(attrtype_enum::speed_second) > 0) {
		return get_attr(attrtype_enum::speed_second);
	}
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
	if (att_speed > max_att_speed) {
		final_att_speed = max_att_speed;
		m_full_speed = att_speed - max_att_speed;
	}
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


uint32_t fight_hero_t::get_resource_id() {
	return m_hero_data.base_attr().resource_id();
}

bool fight_hero_t::is_summon()
{
	return m_hero_data.base_attr().type() == hero_type_summon;
}

uint32_t fight_hero_t::get_script_id()
{
	if (is_monster())
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

//用于取出速度最快的武将
bool fight_hero_t::hero_max_speed_sort(const fight_hero_ptr& h1, const fight_hero_ptr& h2) {
	if (NULL == h1 || NULL == h2) {
		return false;
	}

	//基础速度排序
	if (h1->get_attr(attrtype_enum::speed) != h2->get_attr(attrtype_enum::speed)) {
		return h1->get_attr(attrtype_enum::speed) < h2->get_attr(attrtype_enum::speed);
	}

	return true;
}

bool fight_hero_t::hero_speed_sort(const fight_hero_ptr& h1, const fight_hero_ptr& h2)
{
	if (NULL == h1 || NULL == h2)
	{
		return false;
	}

	/*
	if (h1->get_order_att_speed() != h2->get_order_att_speed())
	{
		return h1->get_order_att_speed() < h2->get_order_att_speed();
	}
	*/
	//速度条排序
	if (h1->get_att_speed() != h2->get_att_speed()) 
	{
		return h1->get_att_speed() < h2->get_att_speed();
	}
	
	//拉的条排序
	if (h1->get_full_speed() != h2->get_full_speed()) {
		return h1->get_full_speed() < h2->get_full_speed();
	}

	//基础速度排序
	if (h1->get_speed() != h2->get_speed()) {
		return h1->get_speed() < h2->get_speed();
	}
	
	//tid排序
	if (h1->get_tid() != h2->get_tid())
	{
		return h1->get_tid() > h2->get_tid();
	}
	// 都是玩家 比较玩家ID 小的在前 大的在后
	if (h1->get_type() == h2->get_type() && h1->is_player())
	{
		return h1->get_master() > h2->get_master();
	}
	// 玩家在前 怪物在后
	if (h1->is_player())
	{
		return false;
	}
	if (h1->get_master() == h2->get_master())
		return h1->get_uid() > h2->get_uid();
	else
		return true;
}

// 是否怪物
bool fight_hero_t::is_monster()
{
	return m_hero_data.base_attr().type() == hero_type_summon || m_hero_data.base_attr().type() == hero_type_monster;
}
// 是否玩家
bool fight_hero_t::is_player()
{
	return m_hero_data.base_attr().type() == hero_type_role || m_hero_data.base_attr().type() == hero_type_hero;
	
}

bool fight_hero_t::hero_att_more_speed_sort(const fight_hero_ptr& h1, const fight_hero_ptr& h2)
{
	if (NULL == h1 || NULL == h2)
	{
		return false;
	}

	//bool b_result = false;
	if (h1->get_att_more_speed() != h2->get_att_more_speed())
	{
		return h1->get_att_more_speed() < h2->get_att_more_speed();
	}
	if (h1->get_order_att_speed() != h2->get_order_att_speed())
	{
		return h1->get_order_att_speed() < h2->get_order_att_speed();
	}
	if (h1->get_speed() != h2->get_speed())
	{
		return h1->get_speed() < h2->get_speed();
	}
	if (h1->get_tid() != h2->get_tid())
	{
		return h1->get_tid() > h2->get_tid();
	}
	// 都是玩家 比较玩家ID 小的在前 大的在后
	if (h1->get_type() == h2->get_type() && h1->is_player())
	{
		return h1->get_master() > h2->get_master();
	}
	return false;
	// 玩家在前 怪物在后
}

uint32_t fight_hero_t::get_atk_mode()
{
	if (is_monster())
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
	if (is_monster())
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
	if (is_monster())
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
	if (is_monster())
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
	//特殊类型属性
	switch (type) {
	case attrtype_enum::hp_present_ratio: {	//当前血量万分比
		if (get_max_hp() == 0) {
			return 0;
		}
		return (get_hp() / (get_max_hp() * 1.0f)) * 10000;
	}
	case attrtype_enum::hp_loss_ratio: {	//损失血量万分比
		if (get_max_hp() == 0) {
			return 0;
		}
		return SAFE_SUB(1, (get_hp() / (get_max_hp() * 1.0f))) * 10000;
	}
	}
	
	int32_t base_value = get_attr_without_buff(type);
	int32_t value = m_buff_element_mgr.get_buff_attr(type) + m_buff_element_moment_mgr.get_buff_attr(type);
	int32_t extra_value = m_buff_element_mgr.get_buff_attr(type + EXTRA_ATTR_PLUS) + m_buff_element_moment_mgr.get_buff_attr(type + EXTRA_ATTR_PLUS);
	int32_t percent = m_buff_element_mgr.get_buff_attr_percent(type) + m_buff_element_moment_mgr.get_buff_attr_percent( type );
	int32_t result = base_value * ((percent + 10000.f) / 10000.f) + value + extra_value;
	return result;
}

int fight_hero_t::get_attr_percent(uint32_t type) {
	int32_t percent = m_buff_element_mgr.get_buff_attr_percent(type) + m_buff_element_moment_mgr.get_buff_attr_percent(type);

	return percent;
}


int fight_hero_t::get_max_attr(uint32_t attr_id)
{
	if (attr_id == attrtype_enum::hp) {
		return get_max_hp();
	}
	return get_attr(attr_id);
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


void fight_hero_t::clear_round_attr()
{
	m_buff_element_mgr.clear_element_value_percent(attrtype_enum::execute_boss_extra_damage);//清除斩杀技能对boss额外伤害buff属性
}

// 治疗HP
uint32_t fight_hero_t::heal_hp(fight_hero_ptr attacker, int value)
{
	// 治疗加成与治疗降低
	//治疗者的增益减益
	int bonus = attacker->get_attr(attrtype_enum::heal_bonus);
	bonus -= attacker->get_attr(attrtype_enum::heal_reduce);
	//受治疗者的增益减益
	bonus += get_attr(attrtype_enum::get_heal_bonus);
	bonus -= get_attr(attrtype_enum::get_heal_reduce);

	value *= (float)(10000 + bonus) * 0.0001;
	if (value < 0)
		return 0;

	get_count_data().be_heal_hp += value;
	do_team_script_event(event_on_heal, shared_from_this());
	add_hp(value);
	return value;
}

uint32_t fight_hero_t::heal_hp_regardless_of_attr(int value)
{
	if (value <= 0)
		return 0;

	get_count_data().be_heal_hp += value;
	do_team_script_event(event_on_heal, shared_from_this());
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
			do_team_script_event( event_on_event_almost_dead, shared_from_this() );
			value = get_attr(attrtype_enum::hp) - 1;
			return value;
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
	uint32_t percent = (cur_hp / (max_hp * 1.0f)) * 10000 ;
	return percent;
}

void fight_hero_t::sub_hp(int value, fight_hero_ptr attacker, uint32_t skill, bool is_use_event )
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
	
	//如果有气运值，减少相应的气运值
	m_buff_element_mgr.dec_element_value(attrtype_enum::luck_points, value);//函数内有value大于现有属性值的处理
	//log_info("point after sub hp is %d", get_attr(attrtype_enum::luck_points));

	// 判断是否有不死属性，如果有当扣除的值大小当前值则保留1滴血
	value = get_real_sub_hp_value(value);
	sub_attr(attrtype_enum::hp, value);
	//记录扣血值
	get_count_data().round_sub_hp = value;
	get_count_data().round_sub_hp_attacker_uid = attacker->get_uid();
	
	//如果有别人转血，则加血
	value = get_count_data().athanasia_heal_hp;
	if ( (value + get_hp()) >= get_max_hp() ) {
		value = get_max_hp() > get_hp() ? get_max_hp() - get_hp() : 0;
	}

	set_attr(attrtype_enum::hp, get_hp() + value );
	get_count_data().athanasia_heal_hp = 0;
	
	if (is_use_event) {
		do_team_script_event( event_on_sub_hp, shared_from_this() );
	}
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
	fight_hero_ptr p_bear_damage_hero = get_bear_damage_target();
	if ( p_bear_damage_hero  == NULL)
		return final_damage;

	if (p_bear_damage_hero->is_dead())
		return final_damage;

	m_bear_damage = damage * (m_bear_damage_ratio * 0.0001);

	//客户端说要放到后面通知
	//attacker->add_combat_act(proto::common::combat_act_type_share_damage, bear_damage, p_bear_damage_hero, attacker, p_bear_damage_hero->get_attr(attrtype_enum::hp));
	p_bear_damage_hero->deal_be_attack(m_bear_damage, attacker, skill);

	final_damage = SAFE_SUB(damage , (int)m_bear_damage);
	return final_damage;
}

int fight_hero_t::do_transfer_damage(int damage, fight_hero_ptr attacker, uint32_t skill, bool is_buff) {
	int final_damage = damage;
	if (m_transfer_hero_list.empty()) {
		return final_damage;
	}

	combat_ptr p_combat = get_combat();
	if (NULL == p_combat) {
		return final_damage;
	}

	int32_t transfer_damage = 0;
	for (auto iter : m_transfer_hero_list) {
		fight_hero_ptr p_hero = p_combat->find_hero(iter.first);
		if (NULL == p_hero || p_hero->is_dead()) {
			continue;
		}
		transfer_damage += iter.second;
	}
	m_transfer_hero_list.clear();

	final_damage = SAFE_SUB( final_damage, transfer_damage);

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

	int final_damage = do_share_damage( damage, attacker, skill, is_buff);			//平均分摊伤害
	final_damage = do_share_hurt(final_damage, attacker, skill);					//指定分摊
	final_damage = do_target_bear_damage(final_damage, attacker, skill, is_buff);	//承担伤害比例

	deal_be_attack(final_damage, attacker, skill);
}

bool fight_hero_t::on_round_start_imp()
{
 	clear_cur_action();

	return on_event(event_on_round_start, shared_from_this());
	//get_script_proxy()->on_event(event_on_round_begin, get_script_proxy());
}

void fight_hero_t::on_round_end_imp()
{

	//get_script_proxy()->on_event(event_on_round_end, get_script_proxy());
}

bool fight_hero_t::on_event(FIGHT_EVENT event, const fight_hero_ptr p_event_owner)
{
	if (is_dead())
	{
		// 死亡后除了死亡事件 其他事件都不应触发
		if (event != event_on_death)
			return false;
	}
	++get_count_data().event_trigger_count;
	// 防止事件互相调用触发死循环
	if (get_count_data().event_trigger_count > 50)
	{
		log_error("on_event dead cycle count > 50");
		return false;
	}
	return get_buff_manager().update_buff(event, p_event_owner);
	//return true;
}

// 事件
void fight_hero_t::do_team_script_event( FIGHT_EVENT event, const fight_hero_ptr p_event_owner)
{
	get_count_data().event_trigger_count = 0;
	fight_hero_ptr p_hero = fight_hero_ptr();

	combat_ptr p_combat = get_combat();
	if (NULL == p_combat) {
		log_error("combat is null");
		return;
	}

	//执行光环buff先
	if (p_event_owner != NULL) {
		p_combat->do_public_buff(event, shared_from_this(), p_event_owner, get_camp());
	}

	const auto &event_map = p_combat->hero_event_map();
	auto iter_f = event_map.find(event);
	if (iter_f == event_map.end())
		return;

	for (auto iter : iter_f->second) {
		if (iter.second == 0)
			continue;

		fight_hero_ptr p_hero = p_combat->find_hero( iter.first );
		if (NULL == p_hero)
			continue;

		if (get_uid() == p_hero->get_uid())
		{
			if (!p_hero->on_event(event, p_event_owner))
				continue;
		}
		else if (get_camp() == p_hero->get_camp())
		{
			p_hero->get_count_data().event_trigger_count = 0;
			if (!p_hero->on_event((FIGHT_EVENT)(event + MAGIC_TEAM_EVENT), p_event_owner))
				continue;
		}
		else
		{
			p_hero->get_count_data().event_trigger_count = 0;
			if (p_hero->on_event((FIGHT_EVENT)(event + MAGIC_ENEMY_EVENT), p_event_owner))
				continue;
		}
        FIGHT_LOG("hero[%lu] do_event[%u]", p_hero->get_uid(), event);
	}
}

void fight_hero_t::clear_att_speed()
{
	set_att_speed(0);
	set_att_more_speed(0);
	set_att_order_speed(0);
}

void fight_hero_t::on_round_start()
{
	clear_att_speed();
	// 出手回合数
	++m_count_data.round_count;
	++m_round;
	m_buff_mgr.on_round_start();

	set_attack_step(proto::common::combat_act_step_round_start);
	on_event(event_on_round_begin, shared_from_this() );

}

void fight_hero_t::soul_condition_on_round_end()
{
	// 灵魂状态结束则死亡
	int soul_state = get_attr(attrtype_enum::soul_condition);
	if (soul_state > 0)
	{
		--soul_state;
		set_attr(attrtype_enum::soul_condition, soul_state);
		if (soul_state == 0)
		{
			add_combat_act(proto::common::combat_act_type_attr_dec, 1, shared_from_this(), shared_from_this(), attrtype_enum::soul_condition, get_attr(attrtype_enum::soul_condition));
			to_die();
		}
	}
}

void fight_hero_t::on_round_end()
{
	//if (NULL == m_script_proxy)
	{
	//	return;
	}

	// 如果不存于被控制状态则减CD
	if (!is_in_control()) {
		sub_skill_cd();
	}

	//减少buffcd
	m_buff_mgr.sub_buff_cd();

	set_attack_step(proto::common::combat_act_step_round_end);
	//m_script_proxy->round_end(m_round);
	on_event(event_on_round_end, shared_from_this());
	do_team_script_event( event_on_self_round_end, shared_from_this() );

	m_count_data.resetByRound();
	m_round_hurt_data.clear_hurt_info();
	m_cur_attacker.reset();

	m_cur_target.reset();
	m_attack_targets.clear();
	soul_condition_on_round_end();

	//清除每回合的临时属性
	clear_round_attr();
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

	//一次技能攻击相关次数重置
	get_count_data().resetByOneSkillAttack();

	// 出手前的buff处理
	do_skill_buff(NONE_PHASE, event_on_attack_begin, skill_event_target_type_except_target, skill_conf, NULL);

	if ( false == trigger_by_buff) {
		add_skill_cd(skill_conf->id());
	}
}

fight_hero_ptr fight_hero_t::get_first_target()
{
	if (m_attack_targets.empty())
		return NULL;

	auto iter_f = m_attack_targets.begin();

	return iter_f->second.lock();
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
	
	if (skill_conf->type() == proto::common::skill_type_sp_skill)
	{
		do_team_script_event(event_on_use_sp_skill, shared_from_this());
	}

	do_team_script_event(event_on_attack_end, shared_from_this());
	
	if (!trigger_by_buff)
	{
		//m_buff_mgr.after_attack();
		get_combat()->add_attack_count(get_master());
	}

	// 出手后的buff处理
	do_skill_buff(NONE_PHASE, event_on_attack_end, skill_event_target_type_except_target, skill_conf, NULL);
	for (auto iter : m_attack_targets)
	{
		auto target = iter.second;
		do_skill_buff(NONE_PHASE, event_on_attack_end, skill_event_target_type_target, skill_conf, target.lock());

		m_cur_target = target;
		do_skill_buff(NONE_PHASE, event_on_attack_one_target_end, skill_event_target_type_target, skill_conf, target.lock());
		do_skill_buff(NONE_PHASE, event_on_attack_one_target_end, skill_event_target_type_except_target, skill_conf, NULL);
		do_team_script_event(event_on_attack_one_target_end, shared_from_this());
	}

	// 回合结束前给客户端发送当前出手武将本回合造成的总伤害
	send_total_damage(skill_conf, get_cur_target(), shared_from_this());
	
	//攻击完成后就应该清楚这个列表.反击之类的技能也会走do_attack，然后触发多次后，会导致m_attack_targets 增大。。
	m_attack_targets.clear();
	//m_script_proxy->after_attack(skill_conf->id());

	FIGHT_LOG("[%lu]do_attack end:camp[%u] pos[%u] tid[%u] skill[%u]>>>>>", get_fight_uid(), get_camp(), get_pos(), get_tid(), skill_conf->id());
}

// void fight_hero_t::after_script_use_skill(SkillEffectTable* skill_conf)
// {
// 	after_attack(skill_conf, false);
// }

bool fight_hero_t::check_skill_consum(SkillEffectTable* skill_conf) {
	if (NULL == skill_conf) {
		return false;
	}

	if (skill_conf->consum_value() == 0) {
		return true;
	}

	uint32_t value = 0;
	switch (skill_conf->consum_type()) {
		case skill_consum_type_hp: 
			value = get_hp();
		break;
		case skill_consum_type_sp: 
			value = get_hp();
		break;
		default:
			return false;
	}

	if (skill_conf->consum_value() >= value ) {
		return false;
	}

	return true;
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

	if (m_dead_flag)
		return;

	m_dead_flag = true;

	clear_taunt_info();

	FIGHT_LOG("[%lu]*****on_dead: camp[%u] pos[%u] tid[%u]", get_fight_uid(), get_camp(), get_pos(), get_tid());

	// 死亡清除buff
	m_buff_mgr.on_dead();

	// 回收光环BUFF
	recovery_special_buff();

	get_combat()->on_fight_hero_dead(shared_from_this());

	if (NULL != skill_conf && NULL != attaker)
	{
		attaker->do_skill_buff(
			NONE_PHASE,
			event_on_killer,
			skill_event_target_type_except_target,
			skill_conf,
			NULL);

		attaker->do_skill_buff(NONE_PHASE, event_on_killer, skill_event_target_type_target, skill_conf, attaker->get_cur_target());
	}
	if( NULL != attaker )
		attaker->do_team_script_event(event_on_killer, attaker);

	do_team_script_event(event_on_death, shared_from_this());

	FIGHT_LOG("[%lu]hero[%d] dead", get_fight_uid(), get_tid());
	// 如果不是召唤物 看看是否需要把召唤物清除
	if (!is_summon())
	{
		combat_ptr p_combat = get_combat();
		if (NULL == p_combat)
		{
			log_error("p_combat NULL");
			return;
		}
		p_combat->on_master_die(get_camp(), get_uid());
	}
	
	//怪物死亡也算一次怪物回合
	if (is_monster())
		fight_manager_t::on_monster_round_end( get_combat() );

	clear_suck_value();
}

uint32_t fight_hero_t::get_relive_count()
{
	return get_count_data().relive_count;
}

uint32_t fight_hero_t::get_soul_relive_count()
{
	return get_count_data().soul_relive_count;
}

void fight_hero_t::send_trigger_passive_to_user(uint32_t skill_id, uint32_t buff_id, uint32_t buff_tid)
{
	SkillEffectTable* skill_conf = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(skill_id, 1));
	if (NULL == skill_conf)
		return;
	// 判断这个技能是否被动技能
	add_combat_act(proto::common::combat_act_type_special_skill, skill_id, shared_from_this(), shared_from_this(), 0, buff_id, buff_tid);
}

// 修改系统控制
void fight_hero_t::change_control_by_system(uint32_t control_by_system)
{
	m_control_by_system = control_by_system;
}
// 获取系统控制
uint32_t fight_hero_t::get_control_by_system()
{
	// 如果是召唤物 都是由系统控制的
	if (is_summon())
		return 1;
	return m_control_by_system;
}

void fight_hero_t::on_soul_relive(
	fight_hero_ptr attacker,
	uint32_t buffid,
	uint32_t bufftid,
	uint32_t skill,
	int relive_hp,
	int relive_speed,
	int relive_round,
	int control_by_system)
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

	if (!can_relive())
	{
		return;
	}
	m_dead_flag = false;

	change_control_by_system(control_by_system);

	// 分发光环BUFF
	get_combat()->set_public_buff_on(get_uid());
	//get_combat()->distribute_special_buff();

	//m_dead_flag = false;
	// 设置HP
	if (relive_hp <= 0)
		relive_hp = 1;
	add_hp(relive_hp);

	// 设置速度条
	if (relive_speed >= 0)
	{
		set_att_speed(relive_speed);
		set_att_more_speed(relive_speed);
	}
	
	// 增加灵魂状态回合数
	//m_buff_element_mgr.inc_element_value(attrtype_enum::soul_condition, relive_round);
	//增加灵魂回合数
	set_attr(attrtype_enum::soul_condition, relive_round);
	add_combat_act(
		proto::common::combat_act_type_attr_inc, 
		relive_round, 
		shared_from_this(),
		attacker,
		attrtype_enum::soul_condition, 
		relive_round,
		bufftid);
	// 清空所有技能CD
	dec_all_skill_cd(attacker,999);
	++get_count_data().soul_relive_count;
}

void fight_hero_t::on_relive(
	fight_hero_ptr attacker, 
	uint32_t buffid, 
	uint32_t bufftid, 
	uint32_t skill, 
	int relive_hp, 
	int relive_speed )
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

	//移除复活时需要移除的buff
	m_buff_mgr.on_revive();

	// 分发光环BUFF
	get_combat()->set_public_buff_on(get_uid());
	//get_combat()->distribute_special_buff();

	m_dead_flag = false;
	// 设置HP
	add_hp( relive_hp );
	
	// 设置速度条
	if (relive_speed > 0)
	{
		set_att_speed(relive_speed);
		set_att_more_speed(relive_speed);
	}

	add_combat_act(proto::common::combat_act_type_relive, relive_hp, shared_from_this(), attacker, get_hp(), buffid, bufftid);
	do_team_script_event(event_on_relive, shared_from_this());
	++get_count_data().relive_count;
	get_count_data().is_relive_cur_round = true;
}

void fight_hero_t::to_die()
{
	if (!is_dead())
	{
		if (get_attr(attrtype_enum::hp) > 0 )
			set_attr(attrtype_enum::hp, 0);
		if (get_attr(attrtype_enum::soul_condition) != 0)
			set_attr(attrtype_enum::soul_condition,0);
	}
	on_dead(NULL, NULL);
}

// 
void fight_hero_t::change_death_with_master(uint32_t value)
{
	m_death_with_master = value;
}
// 
uint32_t fight_hero_t::get_death_with_master()
{
	return m_death_with_master;
}

// 被召唤出来
bool fight_hero_t::summon(
	// 召唤物ID
	uint32_t summonid,
	// 召唤者HP比例， 如果为0 则不使用
	uint32_t master_hp_ratio,
	// 召唤者攻击比例， 如果为0 则不使用
	uint32_t master_attack_ratio,
	// 召唤者防御比例， 如果为0 则不使用
	uint32_t master_defend_ratio,
	// 场上最大数量
	uint32_t max_count,
	// 是否随着召唤者死亡
	uint32_t death_with_master,
	uint32_t buffid,
	uint32_t bufftid,
	uint32_t skill)
{
	MonsterHero* conf = GET_CONF(MonsterHero, summonid);
	if (conf == NULL)
	{
		log_error("summon conf NULL id[%d]", summonid);
		return false;
	}
	combat_ptr p_combat = get_combat();
	if (NULL == p_combat)
	{
		log_error("p_combat NULL");
		return false;
	}

	// 判断是否存在召唤的位置
	uint32_t pos = 0;
	if (p_combat->get_summon_pos(get_camp(), pos) == false)
	{
		log_error("summon get_summon_pos max!!!");
		return false;
	}

	// 填充召唤数据
	proto::client::gc_combat_summon_ntf ntf;
	proto::common::hero_single* single = ntf.mutable_hero_list()->add_hero_list();
	if (single == NULL)
	{
		log_error("summon memory out!!!");
		return false;
	}
	if (!fight_manager_t::fill_hero_single(
		get_camp(),
		fight_manager_t::get_max_camp_hero_count(),
		conf,
		conf->level(),
		single,
		get_uid(),
		pos,
		NULL))
	{
		log_error("summon fill_hero_single error!!! id[%d]", summonid);
		return false;
	}

	// 修改类型
	single->mutable_base_attr()->set_type(hero_type_summon);
	// 召唤者HP比例， 如果为0 则不使用
	if (master_hp_ratio > 0)
	{
		int32_t max_hp = (int32_t)( get_max_hp() * (float)(master_hp_ratio * 0.0001) );
		update_fight_attr(single, (uint32_t)attrtype_enum::max_hp, max_hp);
	}
	// 召唤者攻击比例， 如果为0 则不使用
	if (master_attack_ratio > 0)
	{
		int32_t attack = (int32_t)(get_attr(attrtype_enum::atk) * (float)(master_attack_ratio * 0.0001));
		update_fight_attr(single, (uint32_t)attrtype_enum::atk, attack);
	}
	// 召唤者防御比例， 如果为0 则不使用
	if (master_defend_ratio > 0)
	{
		int32_t defend = (int32_t)(get_attr(attrtype_enum::def_physical) * (float)(master_defend_ratio * 0.0001));
		update_fight_attr(single, (uint32_t)attrtype_enum::def_physical, defend);
	}

	
	// 设置阵营
	ntf.set_camp( get_camp() );
	// 设置MASTER
	ntf.set_master_uid(m_hero_data.base_attr().fight_hero_uid());


	// 召唤
	fight_hero_ptr ret = p_combat->summon_hero(ntf.mutable_hero_list());
	if (NULL == ret)
		return false;
	ret->change_death_with_master(death_with_master);
	p_combat->send_msg_to_all(op_cmd::gc_combat_summon_ntf, ntf);
	return true;
}

void fight_hero_t::update_fight_attr(proto::common::hero_single* single, uint32_t type, int32_t value)
{
	if (NULL == single)
		return;
	proto::common::attr_single* data;
	for (int i = 0; i < single->fight_attr().attr().attrs_size(); ++i)
	{
		if (single->fight_attr().attr().attrs(i).type() != type)
			continue;
		data = single->mutable_fight_attr()->mutable_attr()->mutable_attrs(i);
		break;
	}
	if (data == NULL)
		data = single->mutable_fight_attr()->mutable_attr()->mutable_attrs()->Add();
	data->set_type(type);
	data->set_value(value);
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
	m_skill_manager.be_inc_random_skill_cd(attacker, cd);
}
// 随机减少技能CD
void fight_hero_t::dec_random_skill_cd(fight_hero_ptr attacker, uint32_t cd)
{
	m_skill_manager.be_dec_random_skill_cd(attacker, cd);
}

// 添加所有技能CD 
void fight_hero_t::inc_all_skill_cd(fight_hero_ptr attacker,uint32_t cd)
{
	m_skill_manager.be_inc_all_skill_cd(attacker, cd);
}

// 减少所有技能CD
void fight_hero_t::dec_all_skill_cd(fight_hero_ptr attacker,uint32_t cd)
{
	m_skill_manager.be_dec_all_skill_cd(attacker, cd);
}

uint32_t fight_hero_t::get_skill_cd(uint32_t skill_id)
{
	return m_skill_manager.get_skill_cd(skill_id);
}


void fight_hero_t::sub_skill_cd(uint32_t skill_id, uint32_t cd) {
	m_skill_manager.sub_skill_cd(skill_id, cd);
	return;
}


void fight_hero_t::set_skill_cd(uint32_t skill_id, uint32_t cd) {
	m_skill_manager.set_skill_cd(skill_id, cd);
}

void fight_hero_t::sub_skill_cd(uint32_t skill_cd)
{
	m_skill_manager.sub_all_skill_cd(skill_cd);
}

void fight_hero_t::add_skill_cd(uint32_t skill_id)
{
	m_skill_manager.add_skill_cd(skill_id);
}

void fight_hero_t::add_skill_cd_ex(uint32_t skill_id, uint32_t cd){
	m_skill_manager.add_skill_cd_ex(skill_id, cd);
}


bool fight_hero_t::is_buff_skill(uint32_t skill)
{
	SkillEffectTable* p_skill_conf = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(skill, 1));
	if (NULL == p_skill_conf)
		return false;
	if (p_skill_conf->hurt_rate().empty())
		return true;
	if (p_skill_conf->hurt_rate().size() > 1)
		return false;
	return p_skill_conf->hurt_rate()[0] == 0;
}


void fight_hero_t::deal_be_attack(int32_t damage, fight_hero_ptr attacker, uint32_t skill)
{
	if (NULL == attacker) {
		log_error("attacker is NULL");
		return;
	}

	++get_count_data().beattack_count;
	
	// 没有伤害系数则不能触发被伤害事件
	if(!is_buff_skill( skill ))
		do_team_script_event(event_on_behurt_without_sub_hp, shared_from_this());

	damage = do_transfer_damage(damage, attacker, skill, false);		//转移伤害

	sub_hp(damage, attacker, skill);

	// 没有伤害系数则不能触发被伤害事件
	if (!is_buff_skill(skill))
		do_team_script_event(event_on_behurt, shared_from_this());

	// 攻击吵醒规则 概率免吵醒
	if (get_attr(attrtype_enum::sleep) == 0)
		return;

	uint32_t no_wakeup_rate = attacker->get_attr(attrtype_enum::attack_no_wakeup_rate);
	if (no_wakeup_rate > 0)
	{
		if (random_util_t::selectByTenTh(no_wakeup_rate))
			return;
	}
	
	do_team_script_event(event_on_attack_wake_up, shared_from_this());
// 	// 修改昏睡状态 添加行为
 	set_attr(attrtype_enum::sleep, 0);
 	add_combat_act(proto::common::combat_act_type_attr_dec, 1, shared_from_this(), attacker, attrtype_enum::sleep, get_attr(attrtype_enum::sleep));
}

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
	}
}

//同步伤害血量
void fight_hero_t::sync_hp(uint32_t hurt)
{
	if (is_dead())
		return;

	set_attr(attrtype_enum::hp, hurt);
}

void fight_hero_t::set_be_taunt_info(fight_hero_ptr target, bool is_add, uint32_t taunt_buff_type, uint32_t buff_tid)
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
		add_combat_act(proto::common::combat_act_type_attr_inc, 1, shared_from_this(), target, attrtype_enum::taunt, 1, buff_tid);
		target->set_taunt_buff_info(taunt_buff_type);
		target->add_taunt_target(shared_from_this());
	}
	else
	{
		m_buff_element_mgr.dec_element_value(attrtype_enum::taunt, 1);
		add_combat_act(proto::common::combat_act_type_attr_dec, 1, shared_from_this(), target, attrtype_enum::taunt, 0, buff_tid);
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

void fight_hero_t::on_remove_buff(uint32_t id, uint32_t tid, uint32_t layer, uint32_t round, uint32_t event, uint64_t attacker_id, const bool dispel)
{
	// 记录添加buff的参数
	m_calc_result.remove_buff_id = tid;

	if (dispel)
		add_combat_buff_act(proto::common::combat_act_type_dispel_buff, id, tid, layer, round, attacker_id);
	else
		add_combat_buff_act(proto::common::combat_act_type_buff_remove, id, tid, layer, round, attacker_id);
	do_team_script_event(event_on_remove_buff, shared_from_this());
	FIGHT_LOG("[%lu]==buff remove: camp[%u] pos[%u] tid[%u] [%u]", get_fight_uid(), get_camp(), get_pos(), get_tid(), id);
}

void fight_hero_t::on_add_buff(uint32_t id, uint32_t tid, uint32_t layer, uint32_t round, uint32_t event, uint64_t attacker_id)
{
	// 记录添加buff的参数
	m_calc_result.add_buff_id = tid;

//	add_combat_buff_act(proto::common::combat_act_type_buff, id, tid, layer, round, attacker_id);
	do_team_script_event(event_on_add_buff, shared_from_this());
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

void fight_hero_t::add_combat_act(
	proto::common::combat_act_type type, 
	uint32_t value, 
	fight_hero_ptr target,
	fight_hero_ptr attacker, 
	uint32_t remain_value, 
	uint32_t remain_value2, 
	uint32_t param,
	uint32_t param1)
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
		log_warn("fight_hero type:%d value:%d attacker_tid:%d target_tid:%d", type, value, attacker->get_tid(), target->get_tid());
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
	ca->set_step( get_combat()->get_round_state() );
	ca->set_remain_value(remain_value);
	ca->set_remain_value2(remain_value2);
	ca->set_param(param);
	ca->set_param1(param1);

	FIGHT_LOG("combat_act fight_uid[%lu] self[%u] attacker[%u] target[%u] type[%d] value[%d] step[%d] remain_value[%d] remain_value2[%d] param[%d]",
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
	case proto::common::combat_act_type_bomb:
		{
//			old_value = target->get_stat_data().get_accept_damage_count();
			target->get_stat_data().add_accept_damage(value);
			target->get_count_data().one_round_injured += value;
			target->get_count_data().once_injured += value;
// 			log_warn("count data accept_damage hero_tid:%d  master_id:%lu  new_value:%d  one_round_injured:%u", target->get_tid(), 
// 						target->get_master(), target->get_stat_data().get_accept_damage_count(), target->get_count_data().one_round_injured);

//			old_value = attacker->get_stat_data().get_damage_count();
			attacker->get_stat_data().add_damage(value);
			attacker->get_count_data().one_round_damage += value;

			//log_info("attacker uid: %lu, target uid: %lu", attacker->get_uid(), target->get_uid());

			if (target != attacker)//攻击者和目标相同为自己给自己扣血，不算做伤害
			{
				attacker->get_count_data().one_attack_damage += value;
			}
//			log_warn("count data damage hero_tid:%d  master_id:%lu  new_value:%d  one_round_damage:%u", attacker->get_tid(),
//				attacker->get_master(), attacker->get_stat_data().get_damage_count(), attacker->get_count_data().one_round_damage);

			combat_ptr p_combat = get_combat();
			if (NULL == p_combat) {
				log_error("combat is null");
				return ;
			}
			p_combat->add_beattack_hero( target );
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
	
	//如果不保存回放，要清除旧的m_process数据
	if ( get_combat()->is_replay() == false ) {
		get_combat()->clear_last_combat_log();
	}
}

bool fight_hero_t::pass_combat_act_type(proto::common::combat_act_type type)
{
	if (type == proto::common::combat_act_type_syn_cd || 
		type == proto::common::combat_act_type_damage ||
		type == proto::common::combat_act_type_beheaded ||
		type == proto::common::combat_act_type_spurting || 
		type == proto::common::combat_act_type_crit||
		type == proto::common::combat_act_type_bomb||
		type == proto::common::combat_act_type_change_model||
		type == proto::common::combat_act_type_attr_dec||
		type == proto::common::combat_act_type_buff_damage
		)
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
	ca->set_step(get_combat()->get_round_state());
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
	ca->set_step(get_combat()->get_round_state());
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
	if (get_attr(attrtype_enum::stun) > 0			//晕眩
		|| get_attr(attrtype_enum::frozen) > 0		//冰冻
		|| get_attr(attrtype_enum::sleep) > 0		//睡眠
		|| get_attr(attrtype_enum::fairyland) > 0)	//幻境
	{
		return true;
	}
	return false;
}

bool fight_hero_t::is_in_mirage()
{
	return get_attr(attrtype_enum::fairyland) > 0;
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
	fight_hero_ptr p_fight_hero = fight_hero_ptr();

	for( auto iter : m_attack_targets ) {
		auto target = iter.second;
		p_fight_hero = target.lock();
		if( NULL == p_fight_hero ){
			log_error("fight p_fight_hero NULL [%lu]", iter.first );
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
	peek_attr_list(hero_data);

	peek_fight_skill_data(hero_data->mutable_skill_data());
}

void fight_hero_t::peek_attr_list(proto::common::fight_recovery_hero_single_data *hero_data)
{
	if (!hero_data)
	{
		log_error("hera data null");
		return;
	}
	std::map<uint32_t, int32_t> attr_temp;
	for (attr_map::iterator it = m_cur_attr.begin(); it != m_cur_attr.end(); ++it)
	{
		attr_temp[it->first] = get_attr(it->first);
	}

	std::set<uint32_t> buff_element_ids;
	m_buff_element_mgr.get_all_element_attr_id(buff_element_ids);

	for (std::set<uint32_t>::iterator it = buff_element_ids.begin(); it != buff_element_ids.end(); ++it)
	{
		attr_temp[*it] = get_attr(*it);
	}

	for (attr_map::iterator it = attr_temp.begin(); it != attr_temp.end(); ++it)
	{	
		proto::common::fight_hero_attr_single_data* single_attr_data = hero_data->mutable_attrs()->add_attr_list();
		single_attr_data->set_id(it->first);
		single_attr_data->set_value(it->second);
		//log_warn("id: %u, value: %d", single_attr_data->id(), single_attr_data->value());
	}
}

void fight_hero_t::peek_fight_skill_data(proto::common::fight_hero_skill_data *skill_data)
{
	if (skill_data == NULL)
	{
		log_error("fight_hero_t skill_data NULL");
		return;
	}

	proto::common::skill_single *skill_single = NULL;

	const std::vector<skill_info_t> &skill_list = m_skill_manager.get_cur_skill_list();
	for (auto skill_info : skill_list) {
		const skill_data_t *p_skill_data = skill_info.get_skill_data_by_turn_level(m_change_model_phase);
		if (NULL == p_skill_data) {
			continue;
		}

		skill_single = skill_data->add_skill_list();
		skill_single->set_skill_id(p_skill_data->skill_id);
		skill_single->set_skill_cd(skill_info.get_skill_cd(p_skill_data->skill_id));
		skill_single->set_skill_level(p_skill_data->skill_level );
		skill_single->set_hero_uid( get_str_uid() );
		//log_error("sync hero:[%u] skill[%u],cd:[%u], turn_level:[%u]", get_tid(), p_skill_data->skill_id, skill_info.get_skill_cd(p_skill_data->skill_id), m_change_model_phase);
	}
}

// 分发光环BUFF
void fight_hero_t::distribute_special_buff()
{
	const std::vector< skill_info_t > &skill_list = m_skill_manager.get_cur_skill_list();
	for (auto skill_info : skill_list) {
		const skill_data_t *p_skill_data = skill_info.get_skill_data_by_turn_level(m_change_model_phase);
		if (NULL == p_skill_data) {
			continue;
		}

		SkillEffectTable *p_skill_conf = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(p_skill_data->skill_id, p_skill_data->skill_level));
		if (NULL == p_skill_conf)
			continue;

		// 遍历添加光环BUFF
		const add_buff_t& buffs = p_skill_conf->get_gx_buff();
		buff_info_vec::const_iterator it;
		for (it = buffs.fifth.begin(); it != buffs.fifth.end(); ++it)
		{
			BuffTable *p_buff_conf = GET_CONF(BuffTable, it->first );
			if (NULL == p_buff_conf) {
				log_error("buff[%u] config is NULL", it->first);
				continue;
			}

			add_special_buff_to_all(it->first, buffs.third, p_skill_data->skill_id);

			//添加免疫光环buff类型
			if ((p_skill_conf->special_sign() & skill_special_not_effect_self) != 0) {
				add_immuno_buff(p_buff_conf->buff_small_type());
			}
		}
	}
}

void fight_hero_t::add_special_buff_to_all(int buff, int target_type, int skill_id) {
	combat_ptr p_combat = get_combat();
	if (NULL == p_combat) {
		log_error("p_combat is NULL");
		return;
	}
	
	uint32_t level = get_skill_level(skill_id);
	buff_ptr p_buff = buff_manager_t::make_new_buff( p_combat,  buff, get_uid(), skill_id,  level );
	if (NULL == p_buff) {
		log_error("make new buff[%u] is NULL", buff);
		return;
	}

	p_combat->add_public_buff(get_uid(), p_buff, target_type, get_camp() );
}

// 回收光环BUFF
void fight_hero_t::recovery_special_buff()
{
	combat_ptr p_combat = get_combat();
	if (NULL == p_combat) {
		return;
	}
	p_combat->set_public_buff_off(get_uid());
	return;
}

// 激活被动BUFF
void fight_hero_t::activation_passive_buff()
{
	const std::vector< skill_info_t > &skill_list = m_skill_manager.get_cur_skill_list();
	for (auto skill_info : skill_list) {
		const skill_data_t *p_skill_data = skill_info.get_skill_data_by_turn_level(m_change_model_phase);
		if (NULL == p_skill_data) {
			continue;
		}

		SkillEffectTable *p_skill_conf = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(p_skill_data->skill_id, p_skill_data->skill_level));
		if (NULL == p_skill_conf){
			log_error("activation_passive_buff get skill conf error, skill id: %d, skill level: %d", p_skill_data->skill_id, p_skill_data->skill_level);
			continue; 
		}

		// 遍历添加被动BUFF
		const add_buff_t& buffs = p_skill_conf->get_bd_buff();
		buff_info_vec::const_iterator it;
		for (it = buffs.fifth.begin(); it != buffs.fifth.end(); ++it)
		{
			m_buff_mgr.add_special_buff(it->first, get_uid(), p_skill_data->skill_id);
		}
	}

}

void fight_hero_t::clear_passive_buff()
{
	const std::vector< skill_info_t > &skill_list = m_skill_manager.get_cur_skill_list();
	for (auto skill_info : skill_list ) {
		const skill_data_t *p_skill_data = skill_info.get_skill_data_by_turn_level(m_change_model_phase);
		if (NULL == p_skill_data) {
			continue;
		}

		SkillEffectTable *p_skill_conf = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(p_skill_data->skill_id, p_skill_data->skill_level));
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
	
	const std::vector<skill_info_t> &skill_list = m_skill_manager.get_cur_skill_list();
	for (auto skill_info : skill_list )
	{
		uint32_t skill_id = skill_info.get_skill_id(0);			//只存普通技能CD
		uint32_t skill_cd = skill_info.get_skill_cd(skill_id);
		p_skill_data = hero_data->add_skill_list();
		p_skill_data->set_skill_id(skill_id);
		p_skill_data->set_cd(skill_cd);
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

int fight_hero_t::do_share_hurt(int damage, fight_hero_ptr attacker, uint32_t skill )
{
	int final_damage = damage;
	if (get_attr(attrtype_enum::shared_hurt_per) > 0)
	{
		buff_ptr p_buff = get_buff_manager().get_buff_by_id(get_attr(attrtype_enum::shared_buff_id));
		if (NULL == p_buff)
			return final_damage;

		uint64_t hero_id = p_buff->get_attacker();
		fight_hero_ptr p_share_hero = get_combat()->find_hero(hero_id);
		if( NULL == p_share_hero )
			return final_damage;

		//减免的伤害
		int share_hurts = damage * (get_attr(attrtype_enum::shared_hurt_per) / 10000.0f);

		//减免后的实际伤害
		final_damage = (final_damage > share_hurts) ? final_damage - share_hurts : 0;
		
		//减免伤害在减免
		share_hurts = share_hurts - ( share_hurts * (get_attr(attrtype_enum::reduce_shared_hurt_per) / 10000.0f) );

		share_damage_to( p_share_hero, shared_from_this(), skill, share_hurts );
	}
	return final_damage;
}

//用来判断光环技能加的属性和移除的属性是否生效
uint32_t fight_hero_t::add_special_buff_skill_id(uint32_t skill_id) {
	m_special_buff_skill_id[skill_id] += 1;
	//log_error("hero[%lu] add speciall skill_id[%u], num[%u]", get_uid(), skill_id, m_special_buff_skill_id[skill_id] );
	return 0;
}

uint32_t fight_hero_t::del_special_buff_skill_id(uint32_t skill_id) {
	if (m_special_buff_skill_id[skill_id] >= 1) {
		m_special_buff_skill_id[skill_id] -= 1;
	}
	log_error("hero[%lu] del speciall skill_id[%u], num[%u]", get_uid(), skill_id, m_special_buff_skill_id[skill_id] );
	return 0;
}

uint32_t fight_hero_t::get_special_buff_skill_size(uint32_t skill_id) {
	return	m_special_buff_skill_id[skill_id];
}

//被遗忘时
uint32_t fight_hero_t::on_forget() {
	log_error("user[%lu] on_forget", get_uid());
	combat_ptr p_combat = get_combat();
	if (NULL == p_combat) {
		return 0;
	}
	p_combat->set_public_buff_off(get_uid());
	return 0;
}

//解遗忘时
uint32_t fight_hero_t::on_de_forget() {
	combat_ptr p_combat = get_combat();
	if (NULL == p_combat) {
		return 0;
	}
	p_combat->set_public_buff_on(get_uid());
	return 0;
}	

//释放光环技能时
uint32_t fight_hero_t::on_release_special_buff(uint64_t hero_uid, uint32_t buff_id) {
	std::set<uint32_t> &buff_set = m_be_special_buff_hero_list[hero_uid];
	buff_set.insert(buff_id);
	//log_error("user[%lu] be add speciall buff[%u]", hero_uid, buff_id);
	return 0;
}



bool fight_hero_t::do_attack_targets(
	const std::vector<fight_hero_ptr>& targets,
	SkillEffectTable* skill_conf,
	bool& trigger_combo,
	bool trigger_by_buff,
	std::vector<fight_hero_ptr>& counters,
	proto::common::combat_act_type type)
{
	set_attack_step(proto::common::combat_act_step_before_attack_target);

	do_team_script_event(event_on_attack_begin, shared_from_this());

	//计算段数
	std::set<fight_hero_ptr> tmp_counters;
	for (size_t i = 0; i < get_total_end_phase( skill_conf ); ++i) {
		for (auto target : targets) {
			if (!target) {
				continue;	
			}

			m_cur_target = target;
			target->set_cur_attacker(shared_from_this());

			if (i == 0)//只在第一段执行的逻辑
			{
				do_skill_buff(NONE_PHASE, event_on_attack_begin, skill_event_target_type_target, skill_conf, target);

				target->do_team_script_event(event_on_behurt_begin, target);
				FIGHT_LOG("[%lu]attack target: camp[%u] pos[%u] tid[%u]", get_fight_uid(), target->get_camp(), target->get_pos(), target->get_tid());

				do_skill_buff(NONE_PHASE, event_on_attack_one_target_begin, skill_event_target_type_all, skill_conf, target);
			}

			//开战前先给所有人执行一次受到伤害前的buff之类的东西
			do_team_script_event(event_on_attack_one_target_begin, shared_from_this());

			bool trigger_counter = false;
			do_phase_hurt(target, skill_conf, trigger_counter, trigger_combo, i, trigger_by_buff);
			//清理临时属性
			clear_phase_fight_info();
			target->clear_phase_fight_info();
			if (trigger_counter) {
				tmp_counters.insert(target);
			}
			//log_error("attacker[%lu] atk target [%lu] in phase[%lu]", get_uid(), target->get_uid(), i);
		}
	}

	//设置反击列表
	for (auto target : tmp_counters) {
		counters.push_back(target);
	}

	//设置出手次数
	uint32_t skill_phase = get_skill_phase(skill_conf);
	for (auto target : targets) {
		add_combat_act(proto::common::combat_act_type_syn_atk_count, skill_phase, target, shared_from_this(), get_count_data().round_phase_num );
	}

	return true;
}


bool fight_hero_t::do_phase_hurt(
	fight_hero_ptr target, 
	SkillEffectTable* skill_conf,
	bool& trigger_counter,
	bool& trigger_combo,
	size_t phase,
	bool trigger_by_buff,
	proto::common::combat_act_type type)
{
	if (NULL == target) {
		return false;
	}

	if (check_do_attack_target(target, skill_conf, phase, trigger_by_buff, type) == false)
		return false;
	
	m_cur_target = target;
	target->set_cur_attacker(shared_from_this());

	FIGHT_LOG("[%lu]attack target: camp[%u] pos[%u] tid[%u]", get_fight_uid(), target->get_camp(), target->get_pos(), target->get_tid());
	bool combo = false;
	bool counter = false;
	
	uint32_t damage_phase = get_skill_phase(skill_conf);//技能本身的段数
	uint32_t cur_phase = phase;
	if (phase >= damage_phase) {
		phase = 0;
	}

	m_calc_result.reset();

	m_calc_result.phase = phase;
	m_calc_result.act_type = type;
	m_calc_result.skill = skill_conf->id();
	FIGHT_LOG("[%lu]target phase [%u]", get_fight_uid(), (uint32_t)phase);

	set_attack_step(proto::common::combat_act_step_before_attack_phase);

	// 每段伤害前的buff处理
	do_skill_buff(phase, event_on_attack_phase_begin, skill_event_target_type_all, skill_conf, target);
	do_team_script_event(event_on_attack_phase_begin, shared_from_this());		
	target->do_team_script_event(event_on_behurt_phase_begin, target);
		
	if (do_dodge(skill_conf,target)) { // Step(2) 判断本次是否闪避
		// Nothing TODO
	}
	else if (do_immuno(skill_conf,target)) { // Step(3) 判断本次是否免疫
		// Nothing TODO
	}
	else {// 技能伤害计算
		counter |= do_skill_hurt(phase , skill_conf, target, type, combo);
	}
		
	// 一段伤害结束
	set_attack_step(proto::common::combat_act_step_after_attack_phase);

	// 每段伤害后的buff处理
	do_skill_buff(phase, event_on_attack_phase_end, skill_event_target_type_all, skill_conf, target);

	// 触发事件
	do_team_script_event(event_on_attack_phase_end, shared_from_this());
	target->do_team_script_event(event_on_behurt_phase_end, target);

	// 判断自己是否死亡
	if (is_dead()) {
		FIGHT_LOG("[%lu]attacker dead", get_fight_uid());
	}

	// 计算多段暴击率,增加多端的次数
	uint32_t crit_add_phase_rate = get_attr(attrtype_enum::crit_add_phase_rate);
	if (m_calc_result.crit && get_count_data().round_phase_num < damage_phase) {
		if (random_util_t::selectByTenTh(crit_add_phase_rate)) {
			++get_count_data().crit_combo_phase;
		}
	}

	get_count_data().round_phase_num += 1;

	// 判断对方是否死亡
	if ( target->is_dead() || target->is_relive_cur_round() ) {
		FIGHT_LOG("[%lu]target dead", get_fight_uid());
		if (skill_conf->refind_target()) {
			return false;
		}
	}

	//每次攻击完，要计算多段次数是否增加，自带多段的，要每配置段数才计算一次(比如鲁班天生2段)
	uint32_t phase_rate = get_attr(attrtype_enum::phase_rate);
	if (  ( cur_phase % damage_phase == 0 ) && random_util_t::selectByTenTh(phase_rate)){
		get_count_data().phase_rate_add += get_skill_phase(skill_conf);//多段攻击一次要加段数
	}

	if (get_cur_target() != NULL)
		get_cur_target()->set_bear_damage(0, NULL);

	// 判断对方是否死亡,死亡则暂不触发反击
	if (!target->is_dead() && type != proto::common::combat_act_type_counter) {
		int max_counter_count = target->get_attr(attrtype_enum::counter_max); // 目标的属性74
		if (counter && target->counter_count() < (uint32_t)max_counter_count) {
			// 反击,执行反击技能
			target->counter_count(target->counter_count() + 1);
			trigger_counter = true;
			FIGHT_LOG("[%lu]trigger counter", get_fight_uid());
		}
	}

	// 判断自己是否死亡,死亡则不触发连击
	if (!is_dead()) 
	{
		int max_combo_count = get_max_combo_count( target );
		//只有trigger_combo为false时才增加连击次数，不然一次攻击（包括群攻和多段）可能会增加多次连击次数，本次攻击增加过连击次数就不再进增加逻辑
		if (!trigger_combo && combo && combo_count() < (uint32_t)max_combo_count) 
		{
			trigger_combo = true;
			set_combo_count(combo_count() + 1);

			FIGHT_LOG("[%lu]trigger combo", get_fight_uid());
		}
	}
	return true;
}

bool fight_hero_t::can_be_follow_attack( fight_hero_ptr p_attacker ){
	if (NULL == p_attacker) {
		return false;
	}
	
	//阵营相同可以攻击
	if (p_attacker->get_camp() == get_camp()) {
		return true;
	}
	
	//当前回合复活过
	if (get_count_data().is_relive_cur_round == true ) {
		return false;
	}

	return true;
}

void fight_hero_t::add_round_enemy_hurt()
{
	if (m_count_data.round_sub_hp == 0 || m_count_data.round_sub_hp_attacker_uid == 0)
	{
		return;
	}

	fight_hero_ptr p_hero = get_combat()->find_hero(m_count_data.round_sub_hp_attacker_uid);
	if (!p_hero)
	{
		log_error("find hero null");
		return;
	}

	if (p_hero->get_camp() != get_camp())//只有敌人的扣血才记录
	{
		m_round_hurt_data.add_hurt(m_count_data.round_sub_hp_attacker_uid, m_count_data.round_sub_hp);
	}
}

fight_hero_ptr fight_hero_t::get_hurt_most_alive_enemy(fight_hero_ptr attacker, SkillEffectTable* skill_conf)
{
	if (!attacker || !skill_conf)
	{
		log_error("attacker or skill_conf null");
		return NULL;
	}
	combat_ptr p_combat = get_combat();
	if (!p_combat)
	{
		log_error("get combat null");
		return NULL;
	}

	uint64_t max_hurt_uid = 0;
	uint32_t max_hurt = 0;
	fight_hero_ptr p_fight_hero = fight_hero_ptr();
	for (std::map<uint64_t, uint32_t>::iterator it = m_round_hurt_data.hurt_info.begin(); it != m_round_hurt_data.hurt_info.end(); ++it)
	{
		p_fight_hero = p_combat->find_hero(it->first);
		if (NULL == p_fight_hero)
		{
			log_error("p_fight_hero NULL");
			continue;
		}
		if (attacker && skill_conf)
		{
			if (!p_combat->check_skill_target_type(skill_conf, attacker, p_fight_hero))
			{
				continue;
			}
		}
		if (p_fight_hero->is_dead())
		{
			continue;
		}
		if (it->second > max_hurt)
		{
			max_hurt_uid = it->first;
			max_hurt = it->second;
		}
		else if (it->second == max_hurt)//相等随机取一个
		{
			if (random_util_t::randBetween(0, 1) == 1)
			{
				max_hurt_uid = it->first;
			}
		}
	}

	if (max_hurt_uid != 0)
	{
		return p_combat->find_hero(max_hurt_uid);
	}
	else//本回合没有对自己造成伤害的存活的武将，则随机一个敌方存活武将
	{
		fight_hero_ptr found = p_combat->get_random_alive_hero_by_camp(get_enemy_camp());
		if (!found)
		{
			log_error("get random alive hero null");
			return NULL;
		}
		return found;
	}
}

fight_hero_ptr fight_hero_t::get_form_selected_hero()
{
	uint64_t target_id = string_util_t::string_to_uint64(m_cur_form.target());
	combat_ptr p_combat = get_combat();
	if (!p_combat)
	{
		log_error("get combat ptr null");
		return NULL;
	}
	fight_hero_ptr target = p_combat->find_hero(target_id);
	if (NULL == target)
	{
		log_error("can not find selected hero[%lu] by client", target_id);
		return NULL;
	}
	return target;
}

void fight_hero_t::send_total_damage(SkillEffectTable* skill_conf, fight_hero_ptr target, fight_hero_ptr attacker)
{
	if (!target)
	{
		log_error("target is null");
	}
	if (!attacker)
	{
		log_error("attacker is null");
	}
	if (!skill_conf)
	{
		log_error("skill conf is null");
	}

	uint32_t skill_phase = get_skill_phase(skill_conf);
	if (m_target_count > 1 || skill_phase > 1)//单体单段伤害不发送客户端显示
	{
		add_combat_act(proto::common::combat_act_type_one_att_total_hurt, get_count_data().one_attack_damage, target, attacker);
		//log_info("**********************   %u   **********************", get_count_data().one_attack_damage);
	}
}

void fight_hero_t::clear_phase_fight_info() {
	m_buff_element_moment_mgr.clear_all_element();
	m_buff_mgr.clear_phase_run_info();

	if (get_combat()) {
		get_combat()->clear_phase_run_info(get_uid());
	}
}


void fight_hero_t::clear_all_specail_buff() {
	m_buff_mgr.remove_all_special_buff();
}

void fight_hero_t::add_skill_cd_notify() {
	combat_ptr p_combat = get_combat();
	if (p_combat) {
		p_combat->set_hero_skill_cd(shared_from_this());
	}
}

void round_hurt_data::clear_hurt_info()
{
	hurt_info.clear();
}

void round_hurt_data::add_hurt(uint64_t hero_uid, uint32_t hurt)
{
	hurt_info[hero_uid] += hurt;
}

uint64_t round_hurt_data::get_uid_of_hurt_most() const
{
	uint64_t uid_of_max_hurt = 0;
	uint32_t max_hurt = 0;
	for (auto& pair : hurt_info)
	{
		if (pair.second > max_hurt)
		{
			uid_of_max_hurt = pair.first;
		}
		else if (pair.second == max_hurt)//相等随机取一个
		{
			if (random_util_t::randBetween(0, 1) == 1)
			{
				uid_of_max_hurt = pair.first;
			}
		}
	}
	
	return uid_of_max_hurt;//如果为0则表明本回合没有对自己造成伤害的武将
}
