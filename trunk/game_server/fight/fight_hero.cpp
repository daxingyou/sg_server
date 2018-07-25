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
	data.is_use_public_cd = is_use_public_cd;		//ʹ�ù���CD
	m_skill_manager.add_skill(turn_level, data, pos);
	//log_error("add_skill turn_level:[%u],skill_id[%u],pos[%u]", turn_level, skill, pos);
}

//��������
void fight_hero_t::fill_turn_skill(const change_model_skills_vec &skill_list) {
	//i��ʾ����׶�,j��ʾλ��
	for (size_t i = 0; i < skill_list.size(); i++) {
		const skills_vec &skills = skill_list[i];
		for (size_t j = 0; j < skills.size(); j++) {
			skill_info_t *p_skill_info = m_skill_manager.get_skill_info_by_pos(j);
			if (p_skill_info) {//����ļ���
				add_skill(skills[j].skill_id, p_skill_info->get_skill_level(i), 0, i+1, j, skills[j].is_use_public_cd);
			}
			else {//��������
				  //TODO	
			}
		}
	}
}

//��ӱ�����
void fight_hero_t::add_turn_skill() {
	if ( !is_monster() ) {//Ӣ��
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

	if (m_cur_form.attacker().empty()) // û�в���
	{
		skill_id = get_default_skill();
	}
	else
	{
		skill_id = m_cur_form.skill();
	}

	// ��Ĭ ���� ����
	if (get_attr(attrtype_enum::silent) > 0 || get_attr(attrtype_enum::taunt) > 0 ||
		get_attr(attrtype_enum::confused) > 0)
	{
		skill_id = get_default_skill();
		//can_counter = false;
	}

	// ����
	if (get_attr(attrtype_enum::confused) > 0)
	{
		select = get_combat()->select_confused_target(shared_from_this());
	}
	// ����
	else if (get_attr(attrtype_enum::taunt) > 0)
	{
		//targets.push_back(get_special_target());
		select = get_special_target();
	}
	// ������ѡ��Ŀ��
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

	//��⼼������
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
	// ������趨�Զ�ս��ģʽ Ĭ��ʹ���չ�
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

		//��ͨ����
		if (p_conf->type() == proto::common::skill_type_atk || p_conf->type() == proto::common::skill_type_sp_skill) {
			skill_ai_list.push_back(p_conf);
		}

		//�����
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
	if (!is_auto_fight()) // �Զ�ս��
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
	uint32_t enemy_flag_count = 0;				// �б�ǵĵ�������
	uint32_t enemy_count = 0;					// ��������
	fight_hero_ptr p_fight_hero = NULL;
	std::map<uint32_t, uint32_t> skill_ai_type_list;
	std::vector<uint32_t> skill_ai_rate_list;

	// ��������ֵ�ȫ���¼�
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

	// ����ѡ�����¼���Ȩ��
	SkillEffectTable *p_skill_conf = NULL;
	uint32_t find_skill = 0;							// �ҵ��ļ���
	int32_t skill_ai_rate = 0;							// ����AIȨ��
	int32_t max_skill_ai_rate = 0;						// ���ļ���AIȨ��
	int32_t all_skill_ai_rate = 0;						// ȫ������AIȨ��
	uint32_t skill_ai_id = 0;							// ����AI id
	const int32_t skill_ai_rate_max_value = 100;		// ����aiȨ�صķ�ֵ
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
		// ����ü���AI��Ȩ�س�����ֵ ��ֱ��ȡ�ü��� ����roll
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
		// roll����һ��
		if ((uint32_t)random_value <= compare_value)
		{
			// ��ȫУ�� ��������Խ�� skill_ai_list��˳���skill_ai_rate_list��˳����һ�µ�
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

	// ����
	if (is_in_control()) {
		log_error("in control can not attack");
		return false;
	}

	if (is_dead()) {
		log_error("dead can not attack");
		return false;
	}

	//�ж�׷�����
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
		// ����,ִ�з�������
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
		target->get_count_data().round_phase_num = 0;		//��������������������,��Ȼ���¶�η��������쳣
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
	end_phase += get_count_data().crit_combo_phase;															//�����ӵĶ���
	end_phase += m_buff_mgr.get_buff_layer_by_tid( get_attr(common::attrtype_enum::phase_buffer_id) );		//���buff����
	end_phase += get_count_data().phase_rate_add;

	if (get_attr(common::attrtype_enum::phase_rate) > 0)
	{
		end_phase = std::min<size_t>(get_attr(common::attrtype_enum::phase_max), end_phase);//���ܱ���Ķ���+�������ӵĶ������ܳ������õ�������
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
	//��̬�ж��Ƿ񱩻�׷��
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
	
	// ��������������һ���������ж�
	do_counter(counters, can_counter);

	if (trigger_combo)
	{
		FIGHT_LOG("[%lu]start combo: camp[%u] pos[%u] tid[%u]", get_fight_uid(), get_camp(), get_pos(), get_tid());
		// ����,ִ�б�����ͬ�ļ���
		//�޸���������������
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
	
	// ����Ƿ�Ҫ���ž�ͷ ���action	
	uint32_t camera_rate = skill_conf->camera_rate();
	if (get_combat() != NULL)
	{
		// ֻ��pve���ž�ͷ
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
	bool is_refind /*= false*/)//�Ƿ񲥷ż��ܶ�����false����true����
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
	if (targets.size() == 1)//���弼��
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
	else if (targets.size() > 1)//Ⱥ������
	{
		do_attack_targets(targets, skill_conf, trigger_combo, trigger_by_buff, counters, type);
	}

	for ( auto target : targets) {
		if (target == NULL) {
			continue;
		}
		do_after_attack_target(target, skill_conf, trigger_by_buff);
	}


	// ���ж����˺�ת��Ϊֱ��
	if (get_attr(attrtype_enum::all_team_damage_to_injury) > 0) {
		get_count_data().all_team_damage = 0;
	}

	phase = init_phase;

	//�����˺�
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

// ����һ��Ŀ��
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
	// ��ȡϵ����������
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
	// ��ȡϵ����������
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
	// ��ȡϵ����������
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

	// �Ƿ�������
	int crit = get_attr(attrtype_enum::crit_rate); // �Լ�������30

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
		//crit_multiple = ceil(min(max((0 + ����Ч��(�Լ�������31) - ������(Ŀ�������33)) * 0.0001,1.2),10));
		crit_multiple = std::min<int>(std::max<int>((0 + get_attr(attrtype_enum::crit_damage) - target->get_attr(attrtype_enum::crit_damage_reduce)), 12000), 100000);
		//heal_crit_multiple = ceil(min(max((0 + ����Ч��(�Լ�������31)) * 0.0001,1.2),10))��
		heal_crit_multiple = std::min<int>(std::max<int>((15000 + get_attr(attrtype_enum::crit_damage)), 12000), 100000);

		// Ѫ��������Ч���ӳ�
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

	//��������Ч����ֱ�/10000
	m_calc_result.heal = all;

	if (m_calc_result.crit)
	{
		m_calc_result.heal *= (heal_crit_multiple * 0.0001);
	}

	// ִ�м�Ѫ����
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

// �����˺�����
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
	// �����˺�Ѫ������
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
	// ����ֱ�� 
	int hp_value = (int)((float)get_attr(attrtype_enum::hp) * (float)( skill_conf->hp_hurt_rate(phase) * 0.0001 ));

	// ����ֱ��
	int defend_value = (int)((float)get_attr(attrtype_enum::def_physical) * (float)( skill_conf->defend_hurt_rate(phase) * 0.0001 ));

	// �ٶ�ֱ��
	int speed_value = (int)((float)get_speed() * (float)( skill_conf->speed_hurt_rate(phase) * 0.0001 ));
	ret += hp_value;
	ret += defend_value;
	ret += speed_value;

	// ����BUFFID
	if (get_attr(attrtype_enum::damnation_buff_id) > 0)
	{
		int layer = target->m_buff_mgr.get_buff_layer_by_tid(get_attr(attrtype_enum::damnation_buff_id));
		if (layer > 0)
		{
			ret += target->get_max_hp() * ( get_attr(attrtype_enum::damnation_hurt_ratio) * 0.0001 ) * layer;
		}
	}

	// ����ID
	if (get_attr(attrtype_enum::thunder_cloud_buff_id) > 0)
	{
		int layer = m_buff_mgr.get_buff_layer_by_tid(get_attr(attrtype_enum::thunder_cloud_buff_id));
		if (layer > 0)
		{
			ret += get_max_hp() * layer * layer * ( get_attr(attrtype_enum::thunder_clond_hurt_ratio) * 0.0001 );
		}
	}

	// �Ʋ�ID
	if (get_attr(attrtype_enum::stratagem_buff_id) > 0)
	{
		int layer = target->m_buff_mgr.get_buff_layer_by_tid(get_attr(attrtype_enum::stratagem_buff_id));
		if (layer > 0)
		{
			ret += get_attack() * (get_attr(attrtype_enum::stratagem_buff_ratio) * 0.0001) * layer;
		}
	}

	// �غ��������˺�ת��Ϊֱ��
	if (get_attr(attrtype_enum::one_round_all_damage_to_injury) > 0)
	{
		int value = get_count_data().one_round_injured * (get_attr(attrtype_enum::one_round_all_damage_to_injury) * 0.0001);
		if (m_target_count > 0)
			value /= m_target_count;
		
		ret += value;
	}

	// ���ж����˺�ת��Ϊֱ��
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

// ���㹥����
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

	//	Ѫ����ʧ�ٷֱ�ת������
	int ret = get_attack();
	if (get_attr(attrtype_enum::lose_blood_volume_to_attack) > 0)
	{
		int sub = get_max_hp() - get_hp();
		sub = std::max<int>(sub, 0);
		float ratio = (float)sub / (float)max_hp * 100;
		ret *= (1 + ratio * ( get_attr(attrtype_enum::lose_blood_volume_to_attack) * 0.0001 ));
	}
	// ׷�ӹ�����ֱ�
	if (get_attr(attrtype_enum::self_attack_addition) > 0)
	{
		ret *= (1 + ( get_attr(attrtype_enum::self_attack_addition) * 0.0001 ));
	}

	// Ѫ���˺��ӳɽ���(����)
	if (get_attr(attrtype_enum::blood_volume_to_attack_limit_weapon) > 0)
	{
		float ratio = (float)target->get_hp() / (float)target_max_hp * 100;
		float need_ratio = get_attr(attrtype_enum::blood_volume_to_attack_ratio_weapon) * 0.0001;
		if (ratio > need_ratio)
		{
			ret *= (1 + get_attr(attrtype_enum::blood_volume_to_attack_limit_weapon) * 0.0001);
		}
	}

	// Ѫ���˺��ӳɽ���(����)
	if (get_attr(attrtype_enum::blood_volume_to_attack_ratio_skill) > 0)
	{
		float ratio = (float)target->get_hp() / (float)target_max_hp;
		float need_ratio = get_attr(attrtype_enum::blood_volume_to_attack_limit_skill) * 0.0001;
		if (ratio > need_ratio)
		{
			ret *= (1 + get_attr(attrtype_enum::blood_volume_to_attack_ratio_skill) * 0.0001);
		}
	}

	// ���������ӳ�
	if (get_attr(attrtype_enum::counter_attack_addition) > 0)
	{
		if (type == proto::common::combat_act_type_counter)
		{
			ret *= (1 + get_attr(attrtype_enum::counter_attack_addition) * 0.0001);
		}
	}

	return ret;
}

// ���������
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
	// Ѫ���ٷֱ�ת������
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
	// �Ƿ����ӷ���
	if (random_util_t::selectByTenTh(get_attr(attrtype_enum::def_ignore_rate)))
	{
		remove_def = target->get_attr(attrtype_enum::def_physical) * (get_attr(attrtype_enum::def_ignore_value) * 0.0001);
	}
	return std::max<int>(ret - remove_def, 0 );
}

// �����˺����Ӱٷֱ�
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

	int32_t pvp_add_param = 0;		// pvp������ֱ�
	int32_t pvp_dec_param = 0;		// pvp������ֱ�
	
	get_pvp_param(pvp_add_param, pvp_dec_param, target);


	float ret = 1;
	ret += SAFE_SUB( get_attr(attrtype_enum::damage_bonus), target->get_attr(attrtype_enum::get_damage_reduce) ) * 0.0001;
	
	ret += (pvp_add_param + pvp_dec_param) * 0.0001;


	// �жϳ������
	if (get_attr(attrtype_enum::taunt) > 0)
	{
		//ret = SAFE_SUB( ret , ( target->get_attr(attrtype_enum::taunt_damage_reduce) * 0.0001) );
		//�޸�Ϊֻ��Զ�Ӧ�������
		uint32_t taunt_damage_reduce = 3500;
		if (target->get_uid() == get_taunt_me_hero()) {
			ret = SAFE_SUB(ret, (ret * (taunt_damage_reduce) / 10000.0f));
		}
	}

	// �Ƿ��幥��
	if (is_single_attack(skill_conf))
	{
		// Ŀ�굥�幥���ӳ�
		ret += target->get_attr(attrtype_enum::single_attack_behurt_ratio) * 0.0001;
	}
	else
	{
		// Ŀ��Ⱥ�幥���ӳ�
		ret += target->get_attr(attrtype_enum::range_attack_behurt_ratio) * 0.0001;
	}

	// ���ڴ��ڿ���״̬�Ķ����˺������ڻþ�״̬���佫�����ܵ��˺����������ж����˺�
	if (target->is_in_control() && !target->is_in_mirage())
	{
		ret += get_attr(attrtype_enum::in_control_attack_addition) * 0.0001;
	}

	//նɱ���ܶ�boss�����˺�
	ret += m_buff_element_mgr.get_buff_attr_percent(attrtype_enum::execute_boss_extra_damage) * 0.0001;
	
	// ����״̬׷�ӹ�����ֱ�
	if (get_attr(attrtype_enum::target_debuff_attack_addition) > 0)
	{
		ret += target->m_buff_mgr.get_debuff_count() * ( get_attr(attrtype_enum::target_debuff_attack_addition) * 0.0001 );
	}
	// ָ��״̬׷�ӹ�����ֱ�
	if (get_attr(attrtype_enum::target_status_attack_id) > 0 )
	{
		int buffer_id = get_attr(attrtype_enum::target_status_attack_id);
		if( target->m_buff_mgr.get_count_by_small_type(buffer_id) > 0 )
			ret += ( get_attr(attrtype_enum::target_status_attack_addition) * 0.0001 );
	}

	// �����ж�׷���˺�
	ret += (get_attr(attrtype_enum::other_action_addition_hurt) * 0.0001) * get_count_data().other_action_count;

	// ����Ŀ�����Եļ���
	//ret = SAFE_SUB( ret , target->get_hero_element_hurt_reduce(shared_from_this())  * 0.0001 );

	//ϵ������˺�(�Լ�����������-Ŀ��ļ������ԣ�
	int element_hurt_ratio = get_hero_element_hurt_increase(target) - target->get_hero_element_hurt_reduce(shared_from_this());

	return ret * std::max<int>(terry_param + element_hurt_ratio , 0) * 0.0001;
}

// �����˺�
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

	// ������ʵ�˺�
	uint32_t final_real_damage = std::max<int>((get_attr(attrtype_enum::true_damage) - target->get_attr(attrtype_enum::true_def)), 0);

	int attack = calc_attack(phase, skill_conf, target, type, crit_multiple);
	int defend = calc_defend(phase, skill_conf, target, type, crit_multiple);

	int total_fight_constant = get_attr(common::attrtype_enum::fight_constant) + combat_t::get_init_fight_constant();
	
	float skill_ratio = (((get_attr(attrtype_enum::extra_skill_ratio) + skill_conf->hurt_rate(phase))) * 0.0001);

	//2018.5.8�� ����˵ս������������0
	float fight_constant_ratio = total_fight_constant /(float)(total_fight_constant + defend *(1 - std::min<int>( (get_attr(attrtype_enum::def_cut_per) * 0.0001), 1)));

	//������
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
	if (NULL == target) //����Ҫ�ж�damage�Ƿ�Ϊ0���������ն��ѻ��ܵ�ʱ�����damageΪ0
	{
		return;
	}

	int32_t target_shield_point = target->get_attr(type);
	// ���Ŀ��Ļ���ֵ
	if (damage > target_shield_point) {
		absorb += target_shield_point;
		target->sub_attr(type, absorb);
		damage -= target_shield_point;
	} else {
	// ���û���ֵΪ�۳��˺����ֵ
		absorb += damage;
		target->sub_attr(type, absorb);
		damage = 0;
	}

	// �ж��Ƿ������
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

	//�����, 502,5022,5021 buff_С����
	deal_with_shield( target, damage, absorb,  attrtype_enum::xiangyu_shield_points, buff_small_type_xyd );

	//���˶�
	deal_with_shield( target, damage, absorb, attrtype_enum::luck_shield_points, buff_small_type_luckshield );

	//��ͨ��
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

		// ������
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

	// ���ж����˺�ת��Ϊֱ��
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
		int atk = get_attr(attrtype_enum::atk); // ��ȡ10
		int atk_range = get_attr(attrtype_enum::atk_range); // ��ȡ11
		int max_atk = std::max<int>(atk * (atk_range * 0.0001), 1); // ������*��������/10000
		int real_atk = random_util_t::randBetween(atk, max_atk);
		if (real_atk == -1) {
			log_error("dispel_buff rand error min[%d] > max[%d]", atk, max_atk);
		}
		attack = real_atk;
	}
	return attack;
}

// ���㷴��
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

	// ����
	int reflect_skill_value = 0; // ���ܻ�������ֵ, �ȿ���,�Ժ������

	// �����������޷����˺�
	if (target->get_attr(attrtype_enum::reflect) > 0)
	{
		reflect_skill_value += target->get_max_hp() * (target->get_attr(attrtype_enum::reflect) * 0.0001);
	}
	// ���ݹ��������˺�
	if (target->get_attr(attrtype_enum::reflect_by_attack) > 0)
	{
		reflect_skill_value += target->get_attack() * (target->get_attr(attrtype_enum::reflect_by_attack) * 0.0001);
	}

	// �����˺����㷴��
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

	//�ͻ���˵Ҫ�ŵ�����֪ͨ
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
	
	// ��Ѫ��Ѫ����
	add_hp(m_calc_result.leech);
	//add_combat_act(proto::common::combat_act_type_leech, m_calc_result.leech, get_uid(), get_attr(attrtype_enum::hp));
	add_combat_act(proto::common::combat_act_type_leech, m_calc_result.leech, shared_from_this(), shared_from_this(), get_attr(attrtype_enum::hp));
	FIGHT_LOG("[%lu]attacker leech [%d] [%d]", get_fight_uid(), m_calc_result.leech, get_attr(attrtype_enum::hp));
	
}

int fight_hero_t::calc_leech(SkillEffectTable* skill_conf, fight_hero_ptr target)
{
	// ��Ѫ����
	int leech_skill_value = 0; // ���ܻ�����Ѫֵ, �ȿ���,�Ժ������

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
		// ִ�з��˿��Լ���Ѫ����
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

	// �����˺�
	if (type == proto::common::combat_act_type_spurting)
	{
		m_calc_result.damage = m_calc_result.damage * (get_attr(attrtype_enum::spurting) * 0.0001);
	}

	// ��Ѫ����
	m_calc_result.leech = calc_leech(skill_conf, target);

	// ����
	m_calc_result.reflect = calc_reflect(phase, skill_conf, target, type, crit_multiple);

	m_calc_result.damage = std::max<int>(  m_calc_result.damage, ceil( get_attr(attrtype_enum::atk)* 0.01));

	// ����ǰ
	target->get_script_proxy()->before_absorb(get_script_proxy(), skill_conf->id(), m_calc_result.damage);

	//���ܼ��㶪�뵽���տ�Ѫ��
	do_target_shield_points(target, m_calc_result.damage, m_calc_result.absorb);

	// ִ���˺���Ѫ����
	do_skill_buff(NONE_PHASE, event_on_hit_target, skill_event_target_type_all, skill_conf, target);
	do_team_script_event(event_on_hit_target, shared_from_this());	
	
	target->on_be_attack(m_calc_result.damage, shared_from_this(), skill_conf->id());

	do_attack_result_notify(type, target, skill_conf );

	// �ȸ��ͻ��˷��걾���˺���� �ٴ��������¼�
	if (m_calc_result.crit)
	{
		do_skill_buff(NONE_PHASE, event_on_critical_strike, skill_event_target_type_all, skill_conf, target);
		do_team_script_event(event_on_critical_strike, shared_from_this());
		target->do_team_script_event(event_on_becritical_strike, target);
	}

	FIGHT_LOG("[%lu]target damage [%d] [%d]", get_fight_uid(), m_calc_result.damage, target->get_attr(attrtype_enum::hp));


	do_leech(skill_conf, target);

	do_reflect(skill_conf, target);

	// �жϷ���
	int target_counter_rate = target->get_attr(attrtype_enum::counter_rate); // Ŀ�������73
	counter |= random_util_t::selectByTenTh(target_counter_rate);

	// �ж�����
	combo |= check_combo(skill_conf,target);
	return counter;
}

bool fight_hero_t::check_combo(SkillEffectTable* skill_conf, fight_hero_ptr target)
{
	if( NULL == target ){
		log_error("target is null");
		return false;
	}

	// �ж�������һ�μ��ܹ���������Ⱥ������Σ�ֻ��һ����������
	if (!get_count_data().is_calc_combo)
	{
		get_count_data().is_calc_combo = true;

		int combo_rate = get_attr(attrtype_enum::combo_rate); // �Լ�������71
		if (random_util_t::selectByTenTh(combo_rate))
		{
			return true;
		}
	}

	// ����BUFF
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

	// �������ɱ��ι�����
	int real_atk = 0;
	int atk = get_attr(attrtype_enum::atk); // ��ȡ10
	int atk_range = get_attr(attrtype_enum::atk_range); // ��ȡ11
	int max_atk = std::max<int>(atk * ( atk_range * 0.0001 ), 1); // ������*��������/10000

	real_atk = random_util_t::randBetween(atk, max_atk);
	if (-1 == real_atk) {
		log_error("dispel_buff rand error min[%d] > max[%d]", atk, max_atk);
		return false;
	}

	// Ҫ��ʵ�ʹ������õ��佫���� ranged_atk 24
	set_attr(attrtype_enum::ranged_atk, real_atk);
	FIGHT_LOG("[%lu]real_atk [%u][%d]", get_fight_uid(), (uint32_t)attrtype_enum::ranged_atk, (int)real_atk);

	int crit_multiple = 1;
	int heal_crit_multiple = 1;
	// ������ز���
	do_crit(target, crit_multiple, heal_crit_multiple);		

	if (skill_conf->release_ai() == skill_release_ai_heal) // ����������
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
	//�ջؾɵı���Ч��
	recovery_special_buff();
	clear_passive_buff();
	clear_all_specail_buff();

	m_change_model_phase = change_phase;
	m_skill_manager.set_turn_level(change_phase);

	//���ݾɴ���
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

	//֪ͨ���ܱ仯
	add_skill_cd_notify();

	// �����µı���Ч��
	if (NULL != get_combat()) {
		get_combat()->set_public_buff_on(get_uid());
		//get_combat()->distribute_special_buff();
	}

	activation_passive_buff();
}

//��ԭ����
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

	int anti_dogge = get_attr(attrtype_enum::anti_dodge);		// ��������
	int target_dodge = target->get_attr(attrtype_enum::dodge); // ��ȡ Ŀ�������ֵ70
	int result = SAFE_SUB(target_dodge, anti_dogge);

	if (result < 0) {
		result = 0;
	}

	if (random_util_t::selectByTenTh(result)) {
		m_calc_result.dodge = true;
		FIGHT_LOG("[%lu]target dodge", get_fight_uid());
		// ���ܳɹ�
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
	// ���ߵ��幥��
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
		// ���߳ɹ�
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
	
	//��ȡ�ܹ��ܴ�Ķ���,�������Զ�̬���Ӷ��׷��
	size_t init_phase = phase;
	size_t min_phase = init_phase;
	//size_t max_phase = get_total_end_phase( skill_conf );//do_phase_hurt����ܻ��޸�phase_add_rate,���Բ����ñ���ֱ����Ϊѭ�������
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

	// �ж϶Է��Ƿ�����,�������ݲ���������
	//todo
	if (!target->is_dead() && type != proto::common::combat_act_type_counter) {
		int max_counter_count = target->get_attr(attrtype_enum::counter_max); // Ŀ�������74
		if (counter && target->counter_count() < (uint32_t)max_counter_count) {
			target->counter_count(target->counter_count() + 1);
			// ����,ִ�з�������
			trigger_counter = true;
			FIGHT_LOG("[%lu]trigger counter", get_fight_uid());
		}
	}

	//�߼�����combo����ʼ����false
	// �ж��Լ��Ƿ�����,�������ݲ���������
	// todo
// 	if (!is_dead())
// 	{
// 		int max_combo_count = get_max_combo_count( target ); // �Լ�������72
// 		if (combo && combo_count() < (uint32_t)max_combo_count) // ��������
// 		{
// 			combo_count(combo_count() + 1);
// 			// ����,ִ�б�����ͬ�ļ���
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

	//һ��ʼ�������combo_max ��Ϊ���������������žͱ��ˣ�Ŀǰ�������ʵ��佫����ȡ���ֵ��Ϊ������
	if (get_attr(attrtype_enum::combo_rate) > 0) 
	{
		max_count = (int)get_attr(attrtype_enum::combo_max);
	}

	//����������Ӣ��һ������������ԣ�
	if (max_count > (int)get_attr(attrtype_enum::combo_max))
		max_count = (int)get_attr(attrtype_enum::combo_max);
	
	return max_count;
}

int fight_hero_t::get_max_combo_count_by_self()
{
	int max_count = 0;
	// ����BUFF
	if (get_attr(attrtype_enum::combo_buffer_id) > 0 ) {
		max_count = m_buff_mgr.get_buff_layer_by_tid(get_attr(attrtype_enum::combo_buffer_id));
	}
	return max_count;
}

int fight_hero_t::get_max_comba_count_by_enemy()
{
	int max_count = 0;
	// ����BUFF
	if (get_attr(attrtype_enum::be_combo_buffer_id) > 0) {
		max_count = m_buff_mgr.get_buff_layer_by_tid(get_attr(attrtype_enum::be_combo_buffer_id));
	}
	return max_count;
}


bool fight_hero_t::do_after_attack_target(fight_hero_ptr target, SkillEffectTable* skill_conf, bool trigger_by_buff)
{
	// һ��Ŀ�����
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

	// �ж��Լ��Ƿ�����
	// todo
	if (is_dead())
	{
		FIGHT_LOG("[%lu]attacker dead", get_fight_uid());
		on_dead(target, skill_conf);
	}

	// �ж϶Է��Ƿ�����
	//todo
	if (target->is_dead())
	{
		target->on_dead(shared_from_this(), skill_conf);
	}
	// ��̯�˺��ᵼ�¶���Ҳ���� ����Ҫ�ж϶���
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
* param1 ���12��13 ��ʾģ��idȥ����2λ��ֵ
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

	// ��ɢBUFF
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

	// ���BUFF
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

	//����ϵ�����
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
	//�鿴ϵ����ƹ�ϵ
	if ( hit_fix > 0 && random_util_t::selectByTenTh( hit_fix )) {
		return false;
	}

	//����Ч���ֿ�	
	auto skill_effect_max_conf = GET_CONF(Comprehensive, comprehensive_common::skil_effect_max);
	int skill_effect_max = GET_COMPREHENSIVE_VALUE_1(skill_effect_max_conf);
	auto skill_effect_min_conf = GET_CONF(Comprehensive, comprehensive_common::skil_effect_min);
	int skill_effect_min = GET_COMPREHENSIVE_VALUE_1(skill_effect_min_conf);

	int effect_hit_rate = attacker->get_attr(attrtype_enum::effect_hit_rate);				//Ч������
	int effect_dodge_rate = target->get_attr(attrtype_enum::effect_dodge_rate);	//Ч���ֿ�

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

		//���㼼�ܸ���
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
				// ��һ�μ���δ����
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
			// δ���� Ʈ��
			if (p_buff_conf->buff_formula() == buff_formula_type_debuff)
			{
				p_hero->add_combat_act(proto::common::combat_act_type_buff_oppose, 1, p_hero, p_attacker);
				// ����Ч���ֿ�����Ч���ֿ���Ϊ0
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
	//������ʷ����
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
// ��������״̬���������״̬���ǿ��Ը����(������ܸ�������>0 ���ܸ���)
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

//����ȡ���ٶ������佫
bool fight_hero_t::hero_max_speed_sort(const fight_hero_ptr& h1, const fight_hero_ptr& h2) {
	if (NULL == h1 || NULL == h2) {
		return false;
	}

	//�����ٶ�����
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
	//�ٶ�������
	if (h1->get_att_speed() != h2->get_att_speed()) 
	{
		return h1->get_att_speed() < h2->get_att_speed();
	}
	
	//����������
	if (h1->get_full_speed() != h2->get_full_speed()) {
		return h1->get_full_speed() < h2->get_full_speed();
	}

	//�����ٶ�����
	if (h1->get_speed() != h2->get_speed()) {
		return h1->get_speed() < h2->get_speed();
	}
	
	//tid����
	if (h1->get_tid() != h2->get_tid())
	{
		return h1->get_tid() > h2->get_tid();
	}
	// ������� �Ƚ����ID С����ǰ ����ں�
	if (h1->get_type() == h2->get_type() && h1->is_player())
	{
		return h1->get_master() > h2->get_master();
	}
	// �����ǰ �����ں�
	if (h1->is_player())
	{
		return false;
	}
	if (h1->get_master() == h2->get_master())
		return h1->get_uid() > h2->get_uid();
	else
		return true;
}

// �Ƿ����
bool fight_hero_t::is_monster()
{
	return m_hero_data.base_attr().type() == hero_type_summon || m_hero_data.base_attr().type() == hero_type_monster;
}
// �Ƿ����
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
	// ������� �Ƚ����ID С����ǰ ����ں�
	if (h1->get_type() == h2->get_type() && h1->is_player())
	{
		return h1->get_master() > h2->get_master();
	}
	return false;
	// �����ǰ �����ں�
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
	//������������
	switch (type) {
	case attrtype_enum::hp_present_ratio: {	//��ǰѪ����ֱ�
		if (get_max_hp() == 0) {
			return 0;
		}
		return (get_hp() / (get_max_hp() * 1.0f)) * 10000;
	}
	case attrtype_enum::hp_loss_ratio: {	//��ʧѪ����ֱ�
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
	m_buff_element_mgr.clear_element_value_percent(attrtype_enum::execute_boss_extra_damage);//���նɱ���ܶ�boss�����˺�buff����
}

// ����HP
uint32_t fight_hero_t::heal_hp(fight_hero_ptr attacker, int value)
{
	// ���Ƽӳ������ƽ���
	//�����ߵ��������
	int bonus = attacker->get_attr(attrtype_enum::heal_bonus);
	bonus -= attacker->get_attr(attrtype_enum::heal_reduce);
	//�������ߵ��������
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
	// �ж��Ƿ��޵�
	if (get_attr(attrtype_enum::invincibility) > 0)
		return 0;
	// �ж��Ƿ��в������ԣ�����е��۳���ֵ��С��ǰֵ����1��Ѫ
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
	
	//���������ֵ��������Ӧ������ֵ
	m_buff_element_mgr.dec_element_value(attrtype_enum::luck_points, value);//��������value������������ֵ�Ĵ���
	//log_info("point after sub hp is %d", get_attr(attrtype_enum::luck_points));

	// �ж��Ƿ��в������ԣ�����е��۳���ֵ��С��ǰֵ����1��Ѫ
	value = get_real_sub_hp_value(value);
	sub_attr(attrtype_enum::hp, value);
	//��¼��Ѫֵ
	get_count_data().round_sub_hp = value;
	get_count_data().round_sub_hp_attacker_uid = attacker->get_uid();
	
	//����б���תѪ�����Ѫ
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
		uint32_t alive_num = 1;		// 1����ָ�Լ�
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

	//�ͻ���˵Ҫ�ŵ�����֪ͨ
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

	int final_damage = do_share_damage( damage, attacker, skill, is_buff);			//ƽ����̯�˺�
	final_damage = do_share_hurt(final_damage, attacker, skill);					//ָ����̯
	final_damage = do_target_bear_damage(final_damage, attacker, skill, is_buff);	//�е��˺�����

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
		// ��������������¼� �����¼�����Ӧ����
		if (event != event_on_death)
			return false;
	}
	++get_count_data().event_trigger_count;
	// ��ֹ�¼�������ô�����ѭ��
	if (get_count_data().event_trigger_count > 50)
	{
		log_error("on_event dead cycle count > 50");
		return false;
	}
	return get_buff_manager().update_buff(event, p_event_owner);
	//return true;
}

// �¼�
void fight_hero_t::do_team_script_event( FIGHT_EVENT event, const fight_hero_ptr p_event_owner)
{
	get_count_data().event_trigger_count = 0;
	fight_hero_ptr p_hero = fight_hero_ptr();

	combat_ptr p_combat = get_combat();
	if (NULL == p_combat) {
		log_error("combat is null");
		return;
	}

	//ִ�й⻷buff��
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
	// ���ֻغ���
	++m_count_data.round_count;
	++m_round;
	m_buff_mgr.on_round_start();

	set_attack_step(proto::common::combat_act_step_round_start);
	on_event(event_on_round_begin, shared_from_this() );

}

void fight_hero_t::soul_condition_on_round_end()
{
	// ���״̬����������
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

	// ��������ڱ�����״̬���CD
	if (!is_in_control()) {
		sub_skill_cd();
	}

	//����buffcd
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

	//���ÿ�غϵ���ʱ����
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

	//һ�μ��ܹ�����ش�������
	get_count_data().resetByOneSkillAttack();

	// ����ǰ��buff����
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

	// ���ֺ��buff����
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

	// �غϽ���ǰ���ͻ��˷��͵�ǰ�����佫���غ���ɵ����˺�
	send_total_damage(skill_conf, get_cur_target(), shared_from_this());
	
	//������ɺ��Ӧ���������б�.����֮��ļ���Ҳ����do_attack��Ȼ�󴥷���κ󣬻ᵼ��m_attack_targets ���󡣡�
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
	return;		// wys 2017.12.07 ��ʱû����
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

	// �������buff
	m_buff_mgr.on_dead();

	// ���չ⻷BUFF
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
	// ��������ٻ��� �����Ƿ���Ҫ���ٻ������
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
	
	//��������Ҳ��һ�ι���غ�
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
	// �ж���������Ƿ񱻶�����
	add_combat_act(proto::common::combat_act_type_special_skill, skill_id, shared_from_this(), shared_from_this(), 0, buff_id, buff_tid);
}

// �޸�ϵͳ����
void fight_hero_t::change_control_by_system(uint32_t control_by_system)
{
	m_control_by_system = control_by_system;
}
// ��ȡϵͳ����
uint32_t fight_hero_t::get_control_by_system()
{
	// ������ٻ��� ������ϵͳ���Ƶ�
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

	// �ַ��⻷BUFF
	get_combat()->set_public_buff_on(get_uid());
	//get_combat()->distribute_special_buff();

	//m_dead_flag = false;
	// ����HP
	if (relive_hp <= 0)
		relive_hp = 1;
	add_hp(relive_hp);

	// �����ٶ���
	if (relive_speed >= 0)
	{
		set_att_speed(relive_speed);
		set_att_more_speed(relive_speed);
	}
	
	// �������״̬�غ���
	//m_buff_element_mgr.inc_element_value(attrtype_enum::soul_condition, relive_round);
	//�������غ���
	set_attr(attrtype_enum::soul_condition, relive_round);
	add_combat_act(
		proto::common::combat_act_type_attr_inc, 
		relive_round, 
		shared_from_this(),
		attacker,
		attrtype_enum::soul_condition, 
		relive_round,
		bufftid);
	// ������м���CD
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

	//�Ƴ�����ʱ��Ҫ�Ƴ���buff
	m_buff_mgr.on_revive();

	// �ַ��⻷BUFF
	get_combat()->set_public_buff_on(get_uid());
	//get_combat()->distribute_special_buff();

	m_dead_flag = false;
	// ����HP
	add_hp( relive_hp );
	
	// �����ٶ���
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

// ���ٻ�����
bool fight_hero_t::summon(
	// �ٻ���ID
	uint32_t summonid,
	// �ٻ���HP������ ���Ϊ0 ��ʹ��
	uint32_t master_hp_ratio,
	// �ٻ��߹��������� ���Ϊ0 ��ʹ��
	uint32_t master_attack_ratio,
	// �ٻ��߷��������� ���Ϊ0 ��ʹ��
	uint32_t master_defend_ratio,
	// �����������
	uint32_t max_count,
	// �Ƿ������ٻ�������
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

	// �ж��Ƿ�����ٻ���λ��
	uint32_t pos = 0;
	if (p_combat->get_summon_pos(get_camp(), pos) == false)
	{
		log_error("summon get_summon_pos max!!!");
		return false;
	}

	// ����ٻ�����
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

	// �޸�����
	single->mutable_base_attr()->set_type(hero_type_summon);
	// �ٻ���HP������ ���Ϊ0 ��ʹ��
	if (master_hp_ratio > 0)
	{
		int32_t max_hp = (int32_t)( get_max_hp() * (float)(master_hp_ratio * 0.0001) );
		update_fight_attr(single, (uint32_t)attrtype_enum::max_hp, max_hp);
	}
	// �ٻ��߹��������� ���Ϊ0 ��ʹ��
	if (master_attack_ratio > 0)
	{
		int32_t attack = (int32_t)(get_attr(attrtype_enum::atk) * (float)(master_attack_ratio * 0.0001));
		update_fight_attr(single, (uint32_t)attrtype_enum::atk, attack);
	}
	// �ٻ��߷��������� ���Ϊ0 ��ʹ��
	if (master_defend_ratio > 0)
	{
		int32_t defend = (int32_t)(get_attr(attrtype_enum::def_physical) * (float)(master_defend_ratio * 0.0001));
		update_fight_attr(single, (uint32_t)attrtype_enum::def_physical, defend);
	}

	
	// ������Ӫ
	ntf.set_camp( get_camp() );
	// ����MASTER
	ntf.set_master_uid(m_hero_data.base_attr().fight_hero_uid());


	// �ٻ�
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

// �����Ӽ���CD 
void fight_hero_t::inc_random_skill_cd(fight_hero_ptr attacker, uint32_t cd)
{
	m_skill_manager.be_inc_random_skill_cd(attacker, cd);
}
// ������ټ���CD
void fight_hero_t::dec_random_skill_cd(fight_hero_ptr attacker, uint32_t cd)
{
	m_skill_manager.be_dec_random_skill_cd(attacker, cd);
}

// ������м���CD 
void fight_hero_t::inc_all_skill_cd(fight_hero_ptr attacker,uint32_t cd)
{
	m_skill_manager.be_inc_all_skill_cd(attacker, cd);
}

// �������м���CD
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
	
	// û���˺�ϵ�����ܴ������˺��¼�
	if(!is_buff_skill( skill ))
		do_team_script_event(event_on_behurt_without_sub_hp, shared_from_this());

	damage = do_transfer_damage(damage, attacker, skill, false);		//ת���˺�

	sub_hp(damage, attacker, skill);

	// û���˺�ϵ�����ܴ������˺��¼�
	if (!is_buff_skill(skill))
		do_team_script_event(event_on_behurt, shared_from_this());

	// �������ѹ��� �����ⳳ��
	if (get_attr(attrtype_enum::sleep) == 0)
		return;

	uint32_t no_wakeup_rate = attacker->get_attr(attrtype_enum::attack_no_wakeup_rate);
	if (no_wakeup_rate > 0)
	{
		if (random_util_t::selectByTenTh(no_wakeup_rate))
			return;
	}
	
	do_team_script_event(event_on_attack_wake_up, shared_from_this());
// 	// �޸Ļ�˯״̬ �����Ϊ
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

//ͬ���˺�Ѫ��
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
	// ��¼���buff�Ĳ���
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
	// ��¼���buff�Ĳ���
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
	// attacker�ڴ���Ϊ��û�� Ϊ�˼��ݲ��� д�Լ�����
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

			if (target != attacker)//�����ߺ�Ŀ����ͬΪ�Լ����Լ���Ѫ���������˺�
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
	
	//���������طţ�Ҫ����ɵ�m_process����
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
	if (get_attr(attrtype_enum::stun) > 0			//��ѣ
		|| get_attr(attrtype_enum::frozen) > 0		//����
		|| get_attr(attrtype_enum::sleep) > 0		//˯��
		|| get_attr(attrtype_enum::fairyland) > 0)	//�þ�
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

// �ַ��⻷BUFF
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

		// ������ӹ⻷BUFF
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

			//������߹⻷buff����
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

// ���չ⻷BUFF
void fight_hero_t::recovery_special_buff()
{
	combat_ptr p_combat = get_combat();
	if (NULL == p_combat) {
		return;
	}
	p_combat->set_public_buff_off(get_uid());
	return;
}

// �����BUFF
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

		// ������ӱ���BUFF
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

		// ��������BUFF
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
	// ��������
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
		uint32_t skill_id = skill_info.get_skill_id(0);			//ֻ����ͨ����CD
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

		//������˺�
		int share_hurts = damage * (get_attr(attrtype_enum::shared_hurt_per) / 10000.0f);

		//������ʵ���˺�
		final_damage = (final_damage > share_hurts) ? final_damage - share_hurts : 0;
		
		//�����˺��ڼ���
		share_hurts = share_hurts - ( share_hurts * (get_attr(attrtype_enum::reduce_shared_hurt_per) / 10000.0f) );

		share_damage_to( p_share_hero, shared_from_this(), skill, share_hurts );
	}
	return final_damage;
}

//�����жϹ⻷���ܼӵ����Ժ��Ƴ��������Ƿ���Ч
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

//������ʱ
uint32_t fight_hero_t::on_forget() {
	log_error("user[%lu] on_forget", get_uid());
	combat_ptr p_combat = get_combat();
	if (NULL == p_combat) {
		return 0;
	}
	p_combat->set_public_buff_off(get_uid());
	return 0;
}

//������ʱ
uint32_t fight_hero_t::on_de_forget() {
	combat_ptr p_combat = get_combat();
	if (NULL == p_combat) {
		return 0;
	}
	p_combat->set_public_buff_on(get_uid());
	return 0;
}	

//�ͷŹ⻷����ʱ
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

	//�������
	std::set<fight_hero_ptr> tmp_counters;
	for (size_t i = 0; i < get_total_end_phase( skill_conf ); ++i) {
		for (auto target : targets) {
			if (!target) {
				continue;	
			}

			m_cur_target = target;
			target->set_cur_attacker(shared_from_this());

			if (i == 0)//ֻ�ڵ�һ��ִ�е��߼�
			{
				do_skill_buff(NONE_PHASE, event_on_attack_begin, skill_event_target_type_target, skill_conf, target);

				target->do_team_script_event(event_on_behurt_begin, target);
				FIGHT_LOG("[%lu]attack target: camp[%u] pos[%u] tid[%u]", get_fight_uid(), target->get_camp(), target->get_pos(), target->get_tid());

				do_skill_buff(NONE_PHASE, event_on_attack_one_target_begin, skill_event_target_type_all, skill_conf, target);
			}

			//��սǰ�ȸ�������ִ��һ���ܵ��˺�ǰ��buff֮��Ķ���
			do_team_script_event(event_on_attack_one_target_begin, shared_from_this());

			bool trigger_counter = false;
			do_phase_hurt(target, skill_conf, trigger_counter, trigger_combo, i, trigger_by_buff);
			//������ʱ����
			clear_phase_fight_info();
			target->clear_phase_fight_info();
			if (trigger_counter) {
				tmp_counters.insert(target);
			}
			//log_error("attacker[%lu] atk target [%lu] in phase[%lu]", get_uid(), target->get_uid(), i);
		}
	}

	//���÷����б�
	for (auto target : tmp_counters) {
		counters.push_back(target);
	}

	//���ó��ִ���
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
	
	uint32_t damage_phase = get_skill_phase(skill_conf);//���ܱ���Ķ���
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

	// ÿ���˺�ǰ��buff����
	do_skill_buff(phase, event_on_attack_phase_begin, skill_event_target_type_all, skill_conf, target);
	do_team_script_event(event_on_attack_phase_begin, shared_from_this());		
	target->do_team_script_event(event_on_behurt_phase_begin, target);
		
	if (do_dodge(skill_conf,target)) { // Step(2) �жϱ����Ƿ�����
		// Nothing TODO
	}
	else if (do_immuno(skill_conf,target)) { // Step(3) �жϱ����Ƿ�����
		// Nothing TODO
	}
	else {// �����˺�����
		counter |= do_skill_hurt(phase , skill_conf, target, type, combo);
	}
		
	// һ���˺�����
	set_attack_step(proto::common::combat_act_step_after_attack_phase);

	// ÿ���˺����buff����
	do_skill_buff(phase, event_on_attack_phase_end, skill_event_target_type_all, skill_conf, target);

	// �����¼�
	do_team_script_event(event_on_attack_phase_end, shared_from_this());
	target->do_team_script_event(event_on_behurt_phase_end, target);

	// �ж��Լ��Ƿ�����
	if (is_dead()) {
		FIGHT_LOG("[%lu]attacker dead", get_fight_uid());
	}

	// �����α�����,���Ӷ�˵Ĵ���
	uint32_t crit_add_phase_rate = get_attr(attrtype_enum::crit_add_phase_rate);
	if (m_calc_result.crit && get_count_data().round_phase_num < damage_phase) {
		if (random_util_t::selectByTenTh(crit_add_phase_rate)) {
			++get_count_data().crit_combo_phase;
		}
	}

	get_count_data().round_phase_num += 1;

	// �ж϶Է��Ƿ�����
	if ( target->is_dead() || target->is_relive_cur_round() ) {
		FIGHT_LOG("[%lu]target dead", get_fight_uid());
		if (skill_conf->refind_target()) {
			return false;
		}
	}

	//ÿ�ι����꣬Ҫ�����δ����Ƿ����ӣ��Դ���εģ�Ҫÿ���ö����ż���һ��(����³������2��)
	uint32_t phase_rate = get_attr(attrtype_enum::phase_rate);
	if (  ( cur_phase % damage_phase == 0 ) && random_util_t::selectByTenTh(phase_rate)){
		get_count_data().phase_rate_add += get_skill_phase(skill_conf);//��ι���һ��Ҫ�Ӷ���
	}

	if (get_cur_target() != NULL)
		get_cur_target()->set_bear_damage(0, NULL);

	// �ж϶Է��Ƿ�����,�������ݲ���������
	if (!target->is_dead() && type != proto::common::combat_act_type_counter) {
		int max_counter_count = target->get_attr(attrtype_enum::counter_max); // Ŀ�������74
		if (counter && target->counter_count() < (uint32_t)max_counter_count) {
			// ����,ִ�з�������
			target->counter_count(target->counter_count() + 1);
			trigger_counter = true;
			FIGHT_LOG("[%lu]trigger counter", get_fight_uid());
		}
	}

	// �ж��Լ��Ƿ�����,�����򲻴�������
	if (!is_dead()) 
	{
		int max_combo_count = get_max_combo_count( target );
		//ֻ��trigger_comboΪfalseʱ������������������Ȼһ�ι���������Ⱥ���Ͷ�Σ����ܻ����Ӷ���������������ι������ӹ����������Ͳ��ٽ������߼�
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
	
	//��Ӫ��ͬ���Թ���
	if (p_attacker->get_camp() == get_camp()) {
		return true;
	}
	
	//��ǰ�غϸ����
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

	if (p_hero->get_camp() != get_camp())//ֻ�е��˵Ŀ�Ѫ�ż�¼
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
		else if (it->second == max_hurt)//������ȡһ��
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
	else//���غ�û�ж��Լ�����˺��Ĵ����佫�������һ���з�����佫
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
	if (m_target_count > 1 || skill_phase > 1)//���嵥���˺������Ϳͻ�����ʾ
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
		else if (pair.second == max_hurt)//������ȡһ��
		{
			if (random_util_t::randBetween(0, 1) == 1)
			{
				uid_of_max_hurt = pair.first;
			}
		}
	}
	
	return uid_of_max_hurt;//���Ϊ0��������غ�û�ж��Լ�����˺����佫
}
