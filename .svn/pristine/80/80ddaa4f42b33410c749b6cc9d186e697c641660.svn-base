#include "buff_element.hpp"
#include "buff.hpp"
#include "fight_hero.hpp"
#include "common/config_mgr.h"
#include <algorithm> 

buff_element_t::buff_element_t()
{
}

buff_element_t::~buff_element_t()
{
}

bool buff_element_t::regist(std::map<uint32_t, elenment_func>& container)
{
	container.insert(std::make_pair(buff_element_none, buff_element_t::element_test));
	container.insert(std::make_pair(buff_element_change_attr, buff_element_t::change_attr));
	container.insert(std::make_pair(buff_element_change_attr_by_hp, buff_element_t::change_attr_by_hp));
	container.insert(std::make_pair(buff_element_change_temp_attr, buff_element_t::change_temp_attr));
	container.insert(std::make_pair(buff_element_change_attr_by_target_hp, buff_element_t::change_attr_by_target_hp));
	container.insert(std::make_pair(buff_element_trigger_skill, buff_element_t::trigger_skill));

	container.insert(std::make_pair(buff_element_trigger_skill_by_hp, buff_element_t::trigger_skill_by_hp));
	container.insert(std::make_pair(buff_element_trigger_skill_by_critical_blow, buff_element_t::trigger_skill_by_critical_blow));
	container.insert(std::make_pair(buff_element_trigger_skill_by_beattack_count, buff_element_t::trigger_skill_by_beattack_count));
	container.insert(std::make_pair(buff_element_trigger_skill_by_attack_count, buff_element_t::trigger_skill_by_attack_count));

	container.insert(std::make_pair(buff_element_add_sp, buff_element_t::add_sp));
	container.insert(std::make_pair(buff_element_damage, buff_element_t::damage));
	container.insert(std::make_pair(buff_element_steal_buff, buff_element_t::steal_buff));
	container.insert(std::make_pair(buff_element_heal, buff_element_t::heal));
	container.insert(std::make_pair(buff_element_heal_special, buff_element_t::heal_special));
	container.insert(std::make_pair(buff_element_addhp_by_hpchange, buff_element_t::addhp_by_hpchange));
	container.insert(std::make_pair(buff_element_add_max_hp, buff_element_t::add_max_hp));
	container.insert(std::make_pair(buff_element_sub_max_hp, buff_element_t::sub_max_hp));
	container.insert(std::make_pair(buff_element_other_action_count, buff_element_t::other_action_count));
	container.insert(std::make_pair(buff_element_in_control_speed, buff_element_t::in_control_speed));
	container.insert(std::make_pair(buff_element_out_control_speed, buff_element_t::out_control_speed));
	container.insert(std::make_pair(buff_element_att_speed, buff_element_t::change_att_speed));
	container.insert(std::make_pair(buff_element_bear_hurt, buff_element_t::bear_hurt));
	container.insert(std::make_pair(buff_element_death_guard, buff_element_t::death_guard));
	container.insert(std::make_pair(buff_element_death_guard_energy, buff_element_t::death_guard_energy));
	container.insert(std::make_pair(buff_element_all_team_damage, buff_element_t::all_team_damage));

	//container.insert(std::make_pair(buff_element_heal_by_hp, buff_element_t::heal_by_hp));
	container.insert(std::make_pair(buff_element_change_attr_without_num, buff_element_t::change_attr_without_num));
	container.insert(std::make_pair(buff_element_relive, buff_element_t::relive));
	container.insert(std::make_pair(buff_element_change_cd, buff_element_t::change_cd));

	container.insert(std::make_pair(buff_element_cleanup, buff_element_t::cleanup));
	container.insert(std::make_pair(buff_element_switch_hp, buff_element_t::switch_hp));
	container.insert(std::make_pair(buff_element_switch_speed, buff_element_t::switch_speed));
	container.insert(std::make_pair(buff_element_union_attack, buff_element_t::union_attack));
	container.insert(std::make_pair(buff_element_switch_buff, buff_element_t::switch_buff));
	container.insert(std::make_pair(buff_element_change_attr_all_team, buff_element_t::change_attr_all_team));
	container.insert(std::make_pair(buff_element_change_attr_all_enmey, buff_element_t::change_attr_all_enemy));
	container.insert(std::make_pair(buff_element_change_buff_round, buff_element_t::change_buff_round));

	container.insert(std::make_pair(buff_element_balance_hp, buff_element_t::balance_hp));
	container.insert(std::make_pair(buff_element_balance_speed, buff_element_t::balance_speed));

	container.insert(std::make_pair(buff_element_shield, buff_element_t::shield));
	container.insert(std::make_pair(buff_element_bomb, buff_element_t::bomb));
	container.insert(std::make_pair(buff_element_strategy_bomb, buff_element_t::strategy_bomb));
	container.insert(std::make_pair(buff_element_detonate_bomb, buff_element_t::detonate_bomb));
	container.insert(std::make_pair(buff_element_addbuff_by_debuff, buff_element_t::addbuff_by_debuff));
	container.insert(std::make_pair(buff_element_add_rand_buff, buff_element_t::add_rand_buff));
	container.insert(std::make_pair(buff_element_addbuff_by_round_count, buff_element_t::addbuff_by_round_count));
	container.insert(std::make_pair(buff_element_profit_buff_to_debuff, buff_element_t::profit_buff_to_debuff));
	container.insert(std::make_pair(buff_element_add_random_buff, buff_element_t::add_random_buff));
	container.insert(std::make_pair(buff_element_add_random_buff_layer, buff_element_t::add_random_buff_layer));
	container.insert(std::make_pair(buff_element_remove_buff_layer, buff_element_t::remove_buff_layer));
	container.insert(std::make_pair(buff_element_remove_buff, buff_element_t::remove_buff));
	container.insert(std::make_pair(buff_element_remove_buff_by_small_type, buff_element_t::remove_buff_by_small_type));
	
	container.insert(std::make_pair(buff_element_heal_overflow, buff_element_t::heal_overflow));
	container.insert(std::make_pair(buff_element_addbuff_when_exist, buff_element_t::addbuff_when_exist));
	container.insert(std::make_pair(buff_element_addbuff_when_addbuff, buff_element_t::addbuff_when_addbuff));
	container.insert(std::make_pair(buff_element_addbuff_when_beadd_small_type, buff_element_t::addbuff_when_beadd_small_type));
	container.insert(std::make_pair(buff_element_addbuff_when_add_small_type, buff_element_t::addbuff_when_add_small_type));
	container.insert(std::make_pair(buff_element_addbuff_when_over_layer, buff_element_t::addbuff_when_over_layer));

	container.insert(std::make_pair(buff_element_add_buff_to_target, buff_element_t::add_buff_to_target));
	container.insert(std::make_pair(buff_element_pursue_and_attack, buff_element_t::pursue_and_attack));
	container.insert(std::make_pair(buff_element_taunt, buff_element_t::taunt));
	container.insert(std::make_pair(buff_element_forget, buff_element_t::forget));
	container.insert(std::make_pair(buff_element_silent, buff_element_t::silent));
	container.insert(std::make_pair(buff_element_add_buff_after_damage, buff_element_t::add_buff_after_damage));
	container.insert(std::make_pair(buff_element_add_buff_behurt, buff_element_t::add_buff_behurt));
	container.insert(std::make_pair(buff_element_sub_hp, buff_element_t::sub_hp));
	container.insert(std::make_pair(buff_element_dead_when_relive, buff_element_t::dead_when_relive));
	container.insert(std::make_pair(buff_element_add_buff_by_debuf_ratio, buff_element_t::add_buff_by_debuf_ratio));
	container.insert(std::make_pair(buff_element_change_model, buff_element_t::change_model));
	container.insert(std::make_pair(buff_element_athanasia, buff_element_t::athanasia));
	container.insert(std::make_pair(buff_element_extreme_hit_back, buff_element_t::extreme_hit_back));
	container.insert(std::make_pair(buff_element_skill_ext_damage, buff_element_t::skill_ext_damage));
	container.insert(std::make_pair(buff_element_speed_up_when_behurt, buff_element_t::speed_up_when_behurt));
	container.insert(std::make_pair(buff_element_activation_skill, buff_element_t::activation_skill));
	container.insert(std::make_pair(buff_element_add_buff_by_speed, buff_element_t::add_buff_by_speed));
	container.insert(std::make_pair(buff_element_add_buff_by_small_type, buff_element_t::add_buff_by_small_type));
	container.insert(std::make_pair(buff_element_add_buff_by_hp_ratio, buff_element_t::add_buff_by_hp_ratio));
	container.insert(std::make_pair(buff_element_immuno, buff_element_t::immuno));
	container.insert(std::make_pair(buff_element_trigger_story_play, buff_element_t::story_play));
	container.insert(std::make_pair(buff_element_rand_damage, buff_element_t::rand_damage));
	container.insert(std::make_pair(buff_element_heal_all_team, buff_element_t::heal_all_team));
	container.insert(std::make_pair(buff_element_all_team_shield, buff_element_t::all_team_shield));
	container.insert(std::make_pair(buff_element_all_team_damage_turn, buff_element_t::all_team_damage_turn));
	container.insert(std::make_pair(buff_element_soul_relive, buff_element_t::soul_relive));
	container.insert(std::make_pair(buff_element_summon, buff_element_t::summon));
	container.insert(std::make_pair(buff_element_continue_heal, buff_element_t::continue_heal));
	container.insert(std::make_pair(buff_element_beheaded, buff_element_t::be_buff_beheaded));
	container.insert(std::make_pair(buff_element_suck_and_turn, buff_element_t::suck_attr_and_turn));
	
	container.insert(std::make_pair(buff_add_by_other_buff_type, buff_element_t::be_buff_add_by_other_type));
	container.insert(std::make_pair(buff_add_by_attribute, buff_element_t::be_buff_add_by_attribute));
	container.insert(std::make_pair(buff_element_turn_other_buff, buff_element_t::turn_other_buff));
	container.insert(std::make_pair(buff_add_by_hero_alive, buff_element_t::be_buff_add_by_hero_alive));
	container.insert(std::make_pair(buff_element_alomst_die, buff_element_t::alomst_die));
	container.insert(std::make_pair(buff_element_cd_attack_back, buff_element_t::cd_attack_back));
	container.insert(std::make_pair(buff_element_shared_hurt, buff_element_t::shared_hurts));
	container.insert(std::make_pair(buff_element_attr_add_buff_id, buff_element_t::attr_add_buff_id));
	container.insert(std::make_pair(buff_element_attr_add_coeff, buff_element_t::attr_add_coeff));
	container.insert(std::make_pair(buff_element_add_by_spped, buff_element_t::add_by_spped));

	container.insert(std::make_pair(buff_element_suck_blood, buff_element_t::suck_blood));
	container.insert(std::make_pair(buff_element_attack_by_buff, buff_element_t::attack_by_buff));
	container.insert(std::make_pair(buff_element_special_buff_change_att, buff_element_t::special_buff_change_att));
	container.insert(std::make_pair(buff_element_buff_layer_change_att, buff_element_t::buff_layer_change_att));

	container.insert(std::make_pair(buff_element_xiangyu_skill2, buff_element_t::xiangyu_skill2));
	container.insert(std::make_pair(buff_element_xiangyu_skill3, buff_element_t::xiangyu_skill3));

	container.insert(std::make_pair(buff_element_move_after_think, buff_element_t::move_after_think));
	container.insert(std::make_pair(buff_element_move_with_think, buff_element_t::move_with_think));
	container.insert(std::make_pair(buff_element_lsm_gift, buff_element_t::lsm_gift));
	container.insert(std::make_pair(buff_element_add_buff_by_buff_layer, buff_element_t::add_buff_by_buff_layer));
	container.insert(std::make_pair(buff_element_clear_buff_add_attr, buff_element_t::clear_buff_add_attr));
	container.insert(std::make_pair(buff_element_miss_by_beattack_num, buff_element_t::miss_by_beattack_num));
	container.insert(std::make_pair(buff_element_att_speed_by_hp_sub, buff_element_t::add_att_speed_by_hp_sub));
	container.insert(std::make_pair(buff_element_suck_and_bomb, buff_element_t::suck_and_bomb));
	container.insert(std::make_pair(buff_element_record_hurt, buff_element_t::record_hurt));
	container.insert(std::make_pair(buff_element_add_buff_of_hurt_count, buff_element_t::add_buff_of_hurt_count));
	container.insert(std::make_pair(buff_element_limit_max_speed, buff_element_t::limit_max_speed));
	
	return true;
}

uint32_t buff_element_t::element_notify_type(uint32_t type)
{
	return buff_notify_attr_change;
}

uint32_t buff_element_t::add_buff_by_small_type(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{

	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}
		int bufferid = element.value1;
		int small_type = element.value2;
		int target = element.value3;
		int buff_target = element.value4;
		int buff_count = element.value5;

		if (target == 1)
		{
			if (owner->m_buff_mgr.get_count_by_small_type( small_type ) <= 0)
				return buff_notify_none;
		}
		else
		{
			if (attacker->m_buff_mgr.get_count_by_small_type(small_type ) <= 0)
				return buff_notify_none;
		}

		std::vector<fight_hero_ptr> out;
		owner->get_target_by_buff_select_target(owner, buff_target, buff_count, out);
		for (auto p_hero : out) 
		{
			if (NULL != p_hero) 
			{
				p_hero->m_buff_mgr.add_buff(bufferid, target == 1 ? attacker->get_uid() : owner->get_uid(), buff->get_skill(), buff->get_layer());
				break;
			}
		}

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::add_buff_by_speed(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}
		int bufferid = element.value1;
		int target = element.value2;
		int type = element.value3;

		if (type == 1)
		{
			if (owner->get_att_speed() < attacker->get_att_speed() )
				return buff_notify_none;
		}
		else
		{
			if (owner->get_att_speed() >= attacker->get_att_speed())
				return buff_notify_none;
		}


		if (target == 1)
		{
			owner->m_buff_mgr.add_buff(bufferid, buff->get_attacker(), buff->get_skill());
		}
		else
		{
			attacker->m_buff_mgr.add_buff(bufferid, buff->get_attacker(), buff->get_skill());
		}

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::add_buff_by_hp_ratio(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}
		int ratio = element.value1;
		int bufferid = element.value2;
		int target = element.value3;
		int type = element.value4;

		uint32_t max_hp = owner->get_max_hp();
		if (max_hp == 0)
		{
			log_error("add_buff_by_hp_ratio max_hp = 0");
			return buff_notify_none;
		}

		float hp_ratio = (float)owner->get_hp() / (float)max_hp;
		if (type == 1)
		{
			if (hp_ratio < (ratio * 0.0001))
				return buff_notify_none;
		}
		else
		{
			if (hp_ratio >= (ratio * 0.0001))
				return buff_notify_none;
		}


		if (target == 1)
		{
			owner->m_buff_mgr.add_buff(bufferid, buff->get_attacker(), buff->get_skill());
		}
		else
		{
			attacker->m_buff_mgr.add_buff(bufferid, buff->get_attacker(), buff->get_skill());
		}

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::speed_up_when_behurt(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_cur_attacker();
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}

		if (owner->is_dead())
		{
			return buff_notify_none;
		}

		int hurt_ratio = element.value1;
		int speed_ratio = element.value2;

		int damage = attacker->get_fight_calc_result().damage;
		int need_damage = owner->get_max_hp() * (hurt_ratio * 0.0001);
		if (need_damage == 0)
			return buff_notify_none;

		int count = damage / need_damage;
		if (count == 0)
			return buff_notify_none;

		// 触发一次拉条
		owner->skill_change_att_speed( count * speed_ratio, attacker, true );

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::skill_ext_damage(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr target = owner->get_cur_target();
		if (NULL == target)
		{
			log_error("target is null");
			return buff_notify_none;
		}

		int hp_ratio = element.value1;
		int attack_ratio = element.value2;

		int damage = target->get_max_hp() * (hp_ratio * 0.0001);
		int attack = owner->get_attack() * (attack_ratio * 0.0001);
		if (damage > attack)
			damage = attack;

		target->on_be_attack(damage, owner, buff->get_skill());
		owner->add_combat_act(proto::common::combat_act_type_ext, damage, target, owner, target->get_attr(attrtype_enum::hp));

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::extreme_hit_back(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_cur_attacker();
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}

		if (owner->get_max_hp() == 0) {
			log_error("hero[%u]max_hp is 0", owner->get_tid());
		}

		int add_buffer = element.value1;
		float ratio = element.value2 * 0.0001;
		float cur_ratio = (float)owner->get_hp() / (float)owner->get_max_hp();
		float next_ratio = (float)(owner->get_hp() - attacker->get_fight_calc_result().damage) / (float)owner->get_max_hp();

		if (cur_ratio < ratio)
			return buff_notify_none;

		if (next_ratio > ratio)
			return buff_notify_none;

		if (next_ratio <= 0)
			return buff_notify_none;

		// 触发一次BUFF
		owner->m_buff_mgr.add_buff(add_buffer, buff->get_attacker(), buff->get_skill());

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::athanasia(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		owner->athanasia_count(0);
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_cur_attacker();
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}
		// 判断不死次数
		if (owner->athanasia_count() > 0)
		{
			return buff_notify_none;
		}
		
		int add_buffer = element.value1;
		
		// 判断本次伤害是否大于当前HP
		if ((int)attacker->get_fight_calc_result().damage < (int)owner->get_hp())
			return buff_notify_none;
		
		// 触发一次BUFF
		owner->m_buff_mgr.add_buff(add_buffer, buff->get_attacker(), buff->get_skill());
		owner->athanasia_count(owner->athanasia_count() + 1);
		owner->do_team_script_event(event_on_athanasia, owner);
		
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::change_model(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
		{
			fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
			if (NULL == attacker)
			{
				log_error("attacker is null");
				return buff_notify_none;
			}
			int change_type = element.value1;
			int change_phase = element.value2;
			int change_level = element.value3;
			int resource_id = element.value4;
			owner->change_model(change_type, change_phase, change_level, resource_id);
			return buff_notify_valid;
		}
		break;
	case buff_step_unload:
		{
			owner->restore_model();
			return buff_notify_valid;
		}
		break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::change_buff_round(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}
		int bufferid = element.value1;
		// 正数是加回合、负数是减回合
		int value = element.value2;

		// 减益BUFF
		if (bufferid == 0)
			owner->m_buff_mgr.change_debuff_round(value);
		// 增益BUFF
		else if (bufferid == -1)
			owner->m_buff_mgr.change_profit_buff_round(value);
		// 具体BUFFERID
		else
			owner->m_buff_mgr.change_buff_round( bufferid, value );

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::add_buff_by_debuf_ratio(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}
		int base_ratio = element.value1;
		int ratio = element.value2;
		int bufferid = element.value3;
		int target = element.value4;

		// 计算概率
		int value = base_ratio + owner->m_buff_mgr.get_debuff_count() * ratio;

		if (!random_util_t::selectByTenTh(base_ratio))
			return buff_notify_none;

		if (target == 1)
			owner->m_buff_mgr.add_buff(bufferid, buff->get_attacker(), buff->get_skill());
		else
			attacker->m_buff_mgr.add_buff(bufferid, buff->get_attacker(), buff->get_skill());

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::activation_skill(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}
		int skill = element.value1;
		int level = element.value2;

		owner->activation_skill(skill, level);
		owner->add_combat_act(proto::common::combat_act_type_activation_skill, skill, owner, attacker, level, buff->get_id(), buff->get_tid());
		return buff_notify_valid;
	}
	break;
	case buff_step_unload:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}
		int skill = element.value1;
		int level = element.value2;
		owner->activation_skill(0, 0);
		owner->add_combat_act(proto::common::combat_act_type_unactivation_skill, skill, owner, attacker, level, buff->get_id(), buff->get_tid());
		return buff_notify_none;
	}
	
		break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::dead_when_relive(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}
		if (owner->get_relive_count() == 0)
			return buff_notify_none;

		
		int hp = owner->get_hp();
		owner->sub_hp(hp, attacker,buff->get_skill());
		owner->add_combat_act(proto::common::combat_act_type_buff_damage, hp, owner, attacker, owner->get_hp(), buff->get_id(), buff->get_tid());
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::sub_hp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}
		
		int type = element.value1;
		int value = element.value2;
		// 伤害限制
		int limit_ratio = element.value3;
		int32_t ret = calc_value(owner, attacker, type, value, limit_ratio);
		//log_info("fate damage is %d", ret);

		int32_t absorb = 0;
		fight_hero_t::do_target_shield_points(owner, ret, absorb );
		owner->sub_hp(ret, attacker, buff->get_skill());
		owner->add_combat_act(proto::common::combat_act_type_buff_damage, ret, owner, attacker, owner->get_hp(), buff->get_id(), buff->get_tid(), absorb );
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::in_control_speed(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	
	switch (buff->get_step())
	{
	case buff_step_load:
	{
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	}
	case buff_step_trigger:
	{
		if (!owner->is_in_control())
			return buff_notify_none;
		int add = element.value1;
		
		owner->get_count_data().in_control_speed = add;
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::out_control_speed(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}


	switch (buff->get_step())
	{
	case buff_step_load:
	{
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	}
	case buff_step_trigger:
	{
		if (owner->is_in_control())
			return buff_notify_none;


		owner->get_count_data().in_control_speed = 0;
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::other_action_count(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	int ratio = element.value1;

	switch (buff->get_step()) {
	case buff_step_load: {
		owner->set_attr(attrtype_enum::other_action_addition_hurt, ratio);

		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	}
	case buff_step_trigger: {
		if (owner->get_combat()->get_att_fight_hero() == NULL)
			return buff_notify_none;

		if (owner->get_combat()->get_att_fight_hero()->get_uid() == owner->get_uid())
			return buff_notify_none;

		++owner->get_count_data().other_action_count;

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::add_buff_behurt(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_cur_attacker();
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}
		int ratio = element.value1;
		int bufferid = element.value2;
		int target = element.value3;
		// 是否受效果命中影响
		int effect_hit = element.value4;
		// 讽刺目标减少概率
		int taunt_dec_ratio = element.value5;
		// 给队友添加BUFF的概率
		int add_buffer_to_team_ratio = element.value6;

		int ret = ratio;
		if (owner->check_target_taunt(attacker))
			ret = ratio - taunt_dec_ratio;
		if (effect_hit)
		{
			/*
			if (owner->do_effect_hit_result(attacker, ret) == false)
				return buff_notify_none;
			*/
		}
		else
		{
			if (!random_util_t::selectByTenTh(ret))
				return buff_notify_none;
		}

		// 自己
		if (target == 1)
		{
			owner->m_buff_mgr.add_buff(bufferid, buff->get_attacker(), buff->get_skill());
		}
		// 当前出手玩家
		else if (target == 2 )
		{
			if(owner->get_combat()->get_att_fight_hero() != NULL)
			owner->get_combat()->get_att_fight_hero()->m_buff_mgr.add_buff(bufferid, buff->get_attacker(), buff->get_skill());
		}
		// 攻击者
		else
		{
			attacker->m_buff_mgr.add_buff(bufferid, buff->get_attacker(), buff->get_skill());
		}

		fight_hero_map& all_heros = owner->get_combat()->get_all_heros_ptr();
		fight_hero_ptr p_hero = fight_hero_ptr();
		for (auto& heropair : all_heros)
		{
			p_hero = heropair.second;
			if (NULL == p_hero)
			{
				log_error("inc_all_team_attr p_hero NULL");
				continue;
			}

			if (!random_util_t::selectByTenTh(add_buffer_to_team_ratio)) {
				continue;
			}

			if (owner->get_camp() == p_hero->get_camp())
			{
				p_hero->m_buff_mgr.add_buff(bufferid, buff->get_attacker(), buff->get_skill());
				//p_hero->add_combat_act(proto::common::combat_act_type_attr_inc, std::abs(value), p_hero, owner, type, p_hero->get_attr(type));
			}
		}

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::add_buff_after_damage(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}
		int ratio = element.value1;
		int bufferid = element.value2;
		int target = element.value3;
		int count = element.value4;
		if (count == 0)
			count = 99999999;

		uint32_t max_hp = owner->get_max_hp();
		if (max_hp == 0)
		{
			log_error("add_buff_by_hp_ratio max_hp = 0");
			return buff_notify_none;
		}

		float hp_ratio = (float)owner->get_fight_calc_result().damage / (float)max_hp;
		if (hp_ratio < (ratio * 0.0001))
			return buff_notify_none;
		
		if (target == 1)
		{
			if ((int)owner->add_buff_after_damage_count() >= count)
				return buff_notify_none;
			owner->m_buff_mgr.add_buff(bufferid, buff->get_attacker(), buff->get_skill());
			owner->add_buff_after_damage_count(owner->add_buff_after_damage_count() + 1);
		}
		else
		{
			if ((int)attacker->add_buff_after_damage_count() >= count)
				return buff_notify_none;
			attacker->m_buff_mgr.add_buff(bufferid, buff->get_attacker(), buff->get_skill());
			attacker->add_buff_after_damage_count(attacker->add_buff_after_damage_count() + 1);
		}

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::add_buff_to_target(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}
		
		fight_hero_ptr target = owner->get_cur_target();
		if (NULL == target)
		{
			//log_error("add buff[%u] target is null", buff->get_tid());
			target = owner;
			//return buff_notify_none;
		}

		int ratio = element.value1;
		int bufferid = element.value2;
		int target_type = element.value3;
		int max_count = element.value4;
		// 附加概率类型(1-增益BUFF，2-减益BUFF)
		int add_ratio_type = element.value5;
		int add_ratio_value = element.value6;
		int reserve_target_type = element.value7; //备用类型
		int reserve_max_count = element.value8; //备用最大数量

		bool use_add_ratio = false;
		if (add_ratio_type == 1)
		{
			use_add_ratio = (target->m_buff_mgr.get_profit_buff_count() > 0);
		}
		else if (add_ratio_type == 2)
		{
			use_add_ratio = (target->m_buff_mgr.get_debuff_count() > 0);
		}

		int total_ratio = ratio + (use_add_ratio ? add_ratio_value : 0);
		if (!random_util_t::selectByTenTh(total_ratio))
			return buff_notify_none;

		std::vector<fight_hero_ptr> out;
		owner->get_target_by_buff_select_target(target, target_type, max_count, out);
		if(out.empty())
			owner->get_target_by_buff_select_target(target, reserve_target_type, reserve_max_count, out);

		buff_notify_type ret = buff_notify_none;
		for (auto hero : out)
		{
			if (NULL == hero) 
			{
				continue;
			}

			//每个目标要单独算概率
			bool use_add_ratio = false;
			if (add_ratio_type == 1)
			{
				use_add_ratio = (hero->m_buff_mgr.get_profit_buff_count() > 0);
			}
			else if (add_ratio_type == 2)
			{
				use_add_ratio = (hero->m_buff_mgr.get_debuff_count() > 0);
			}

			int total_ratio = ratio + (use_add_ratio ? add_ratio_value : 0);
			if (!random_util_t::selectByTenTh(total_ratio))
			{
				continue;
			}

			//添加buff
			hero->m_buff_mgr.add_buff(bufferid, buff->get_attacker(), buff->get_skill());
			ret = buff_notify_valid;
		}

		return ret;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::addbuff_by_round_count(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}
		int ratio = element.value1;
		int addbuff = element.value2;
		int round_count = element.value3;
		int repeate = element.value4;

		int32_t cur_round_count = (int)owner->get_count_data().round_count;

		// 是否可以重复触发
		if (repeate == 0) 
		{
			if (cur_round_count <= round_count)
			{
				//技能触发前向客户端发送触发计数，如果本次触发技能，则触发前需要发送达到触发次数的计数
				//uint32_t send_count = ((cur_round_count % round_count == 0) ? round_count : (cur_round_count % round_count));
				//owner->add_combat_act(proto::common::combat_act_type_count_of_trigger_skill, send_count, owner, attacker, buff->get_id(), buff->get_tid(), buff->get_skill());
				//log_info("buff_of_round_count count is %d", send_count);
			}
			if (cur_round_count != round_count)
			{
				return buff_notify_none;
			}
		}
		else 
		{
			if (round_count == 0) 
			{
				log_error("buff_tid[%u] round_count is 0 ", buff->get_tid() );
				return buff_notify_none;
			}

			//技能触发前向客户端发送触发计数，如果本次触发技能，则触发前需要发送达到触发次数的计数
			//uint32_t send_count = ((cur_round_count % round_count == 0) ? round_count : (cur_round_count % round_count));
			//owner->add_combat_act(proto::common::combat_act_type_count_of_trigger_skill, send_count, owner, attacker, buff->get_id(), buff->get_tid(), buff->get_skill());
			//log_info("buff_of_round_count count is %d", send_count);

			if ( cur_round_count % round_count != 0 ) 
			{
				return buff_notify_none;
			}
		}

		if (!random_util_t::selectByTenTh(ratio))
			return buff_notify_none;

		owner->m_buff_mgr.add_buff(addbuff, buff->get_attacker(), buff->get_skill());

		//技能触发后需要再次向客户端发送计数，触发后计数为0
		//uint32_t send_count = cur_round_count % round_count;
		//owner->add_combat_act(proto::common::combat_act_type_count_of_trigger_skill, send_count, owner, attacker, buff->get_id(), buff->get_tid(), buff->get_skill());
		//log_info("buff_of_round_count count is %d", send_count);

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::add_rand_buff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}
		std::vector<int> buffs;
		int count = element.value1;
		
		if (element.value2 > 0)
			buffs.push_back(element.value2);
		if (element.value3 > 0)
			buffs.push_back(element.value3);
		if (element.value4 > 0)
			buffs.push_back(element.value4);
		if (element.value5 > 0)
			buffs.push_back(element.value5);
		if (element.value6 > 0)
			buffs.push_back(element.value6);
		if (element.value7 > 0)
			buffs.push_back(element.value7);
		if (element.value8 > 0)
			buffs.push_back(element.value8);
		if (element.value9 > 0)
			buffs.push_back(element.value9);

		if (buffs.empty())
			return buff_notify_none;

		if ((int)buffs.size() < count)
			count = buffs.size();

		if (count == 0)
			return buff_notify_none;

		std::random_shuffle(buffs.begin(), buffs.end());

		for (int i = 0; i < (int)count; ++i)
		{
			owner->m_buff_mgr.add_buff(buffs[i], buff->get_attacker(), buff->get_skill());
		}
		
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::addbuff_by_debuff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}
		int ratio = element.value1;
		int bufferid = element.value2;
		int addbuff = element.value3;
		// 添加概率用的BUFFID
		int add_ratio_buff = element.value4;
		// 添加的概率
		int add_ratio = element.value5;

		int ret = ratio;
		if (add_ratio_buff != 0)
		{
			if (owner->m_buff_mgr.get_buff_by_tid(add_ratio_buff) != NULL)
				ret += add_ratio;
		}

		if (!random_util_t::selectByTenTh(ret))
			return buff_notify_none;

		if (bufferid != 0)
		{
			if (owner->m_buff_mgr.get_buff_by_tid(bufferid) != NULL )
			{
				owner->m_buff_mgr.add_buff(addbuff, buff->get_attacker(), buff->get_skill());
			}
		}
		else
		{
			if (owner->m_buff_mgr.get_debuff_count() > 0)
			{
				owner->m_buff_mgr.add_buff(addbuff, buff->get_attacker(), buff->get_skill());
			}
		}
		
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::strategy_bomb(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_trigger:
	{
		// BUFF剩余一回合的时候引爆
		if (buff->get_round() != 1)
			return buff_notify_none;
	case buff_step_detonate:

		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}
		int value = element.value2;
		int type = element.value1;
		// 伤害限制
		int limit_ratio = element.value3;
		int calcvalue = calc_value(owner, attacker, type, value, limit_ratio);

		int32_t result = calcvalue * buff->get_layer();
		if (owner->get_attr(attrtype_enum::debuff_damage_strengthen))
			result *= 1 + ( owner->get_attr(attrtype_enum::debuff_damage_strengthen) * 0.0001 );

		int32_t absorb = 0;
		fight_hero_t::do_target_shield_points(owner, result, absorb);
		owner->sub_hp(result, attacker);
		FIGHT_LOG("%lu buff[%d] result:%d layer:%d", owner->get_combat()->get_fight_uid(), buff->get_id(), result, buff->get_layer());

		owner->add_combat_act(proto::common::combat_act_type_buff_damage, result, owner, attacker, owner->get_attr(attrtype_enum::hp), buff->get_id(), buff->get_tid(), absorb);

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::bomb(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_trigger:
	{
		// BUFF剩余一回合的时候引爆
		if (buff->get_round() != 0 )
			return buff_notify_none;
	}
	case buff_step_detonate:
	{	
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}

		int type = element.value1;
		int value = element.value2;
		// 伤害限制
		int limit_ratio = element.value3;
		int calcvalue = calc_value(owner, attacker, type, value, limit_ratio);

		int32_t result = calcvalue * buff->get_layer();
		if (owner->get_attr(attrtype_enum::debuff_damage_strengthen))
			result *= 2;

		int32_t absorb = 0;
		fight_hero_t::do_target_shield_points(owner, result, absorb);
		owner->sub_hp(result, attacker);
		FIGHT_LOG("%lu buff[%d] result:%d layer:%d", owner->get_combat()->get_fight_uid(), buff->get_id(), result, buff->get_layer());

		owner->add_combat_act(proto::common::combat_act_type_bomb, result, owner, attacker, owner->get_attr(attrtype_enum::hp), buff->get_id(), buff->get_tid(), absorb);

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::detonate_bomb(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		// 需要引爆的元素
		int value = element.value1;
		owner->m_buff_mgr.detonate_buff(value, p_event_owner, event_num);
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}


uint32_t buff_element_t::union_attack(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		uint32_t limit_num = element.value1;
		uint32_t in_control = element.value2;

		fight_hero_ptr attacker = owner->get_combat()->get_attack_hero();
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}
		
		if (owner->get_count_data().union_attack >= limit_num) {
			return buff_notify_none;
		}

		fight_hero_ptr target = attacker->get_cur_target();
		if (NULL == target)
		{
			log_error("target is null");
			return buff_notify_none;
		}

		if (in_control != 0 && owner->is_in_control()) {
			return buff_notify_none;
		}

		SkillEffectTable* skill_effect_conf = owner->get_skill_effect(owner->get_default_skill());
		if (NULL == skill_effect_conf)
		{
			log_error("skill_effect_conf is null");
			return buff_notify_none;
		}
		
		owner->get_count_data().union_attack += 1;
		owner->trigger_skill_by_buff(skill_effect_conf, target, proto::common::combat_act_type_pincer_attack, NULL);
		owner->dec_all_skill_cd(owner,1);

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::profit_buff_to_debuff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}

		int min_value = element.value1;
		int max_value = element.value2;

		int count = random_util_t::randBetween(min_value, max_value);
		if (count <= 0)
			return buff_notify_none;

		int buffid = element.value3;

		if (owner->m_buff_mgr.get_profit_buff_count() == 0)
		{
			return buff_notify_none;
		}

		count = owner->m_buff_mgr.remove_profit_buff_by_count(count);
		for (int i = 0; i < count; ++i)
		{
			owner->m_buff_mgr.add_buff(buffid, buff->get_attacker(), buff->get_skill(), 1);
		}

		return count > 0 ? buff_notify_valid : buff_notify_none;
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::add_random_buff_layer(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}

		int min_value = element.value1;
		int max_value = element.value2;
		int buffid = element.value3;

		int count = random_util_t::randBetween(min_value, max_value);
		if (count <= 0)
			return buff_notify_none;

		owner->m_buff_mgr.add_buff(buffid, buff->get_attacker(), buff->get_skill(), count);

		return count > 0 ? buff_notify_valid : buff_notify_none;
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::add_random_buff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}

		int min_value = element.value1;
		int max_value = element.value2;
		int buffid = element.value3;

		int count = random_util_t::randBetween(min_value, max_value);

		for (int i = 0; i < count; ++i)
		{
			owner->m_buff_mgr.add_buff(buffid, buff->get_attacker(), buff->get_skill(), 1);
		}

		return count > 0 ? buff_notify_valid : buff_notify_none;
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}


uint32_t buff_element_t::switch_buff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}

		int buffid = element.value1;
		int switch_id = element.value2;
		buff_ptr buff = owner->m_buff_mgr.get_buff_by_small_type(buffid);
		if (NULL == buff)
		{
			return buff_notify_none;
		}

		uint32_t layer = buff->get_layer();
		owner->m_buff_mgr.remove_buff_by_small_type( buffid );
		owner->m_buff_mgr.add_buff( switch_id, buff->get_attacker(), buff->get_skill(), layer);

		/*if (buff->get_layer() > 0)
		{
			// 告诉客户端偷BUFF行为 做表现用
			owner->add_combat_act(proto::common::combat_act_type_steal_buff, buff->get_id(), owner, attacker, 0);
			owner->m_buff_mgr.remove_buff(buff->get_id());
			//attacker->add_combat_act(proto::common::combat_act_type_steal_buff, 1, attacker, 0);
			attacker->m_buff_mgr.add_buff(buff->get_id(), owner->get_uid(), 0, buff->get_layer());
		}*/

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::calc_heal_special_value(fight_hero_ptr owner, fight_hero_ptr attacker, int type, int buff_id, int value, int param)
{
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return value;
	}
	if (NULL == attacker)
	{
		log_error("buff_element_t attacker NULL");
		return value;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return value;
	}
	if (NULL == attacker->get_combat())
	{
		log_error("attacker get_combat is null");
		return value;
	}

	switch (type)
	{
	//类型1公式：自身血量上限 * （10% + buff层数 * Y%)
	case 1:
	{
		int layer = attacker->m_buff_mgr.get_buff_layer_by_tid(buff_id);
		int ret = owner->get_max_hp() * (value * 0.0001 + layer * (param * 0.0001));
		return ret;
	}
	break;
	// 类型2公式：（攻击者血量上限 + 目标血量上限) * X%
	case 2:
	{
		int ret = (owner->get_max_hp() + attacker->get_max_hp()) * (value * 0.0001);
		return ret;
	}
	break;
	// 类型3公式：自身血量上限 * Y%
	case 3:
	{
		int ret = owner->get_max_hp() * (value * 0.0001);
		return ret;
	}
	break;
	// 类型4公式：攻击者血量上限 * （10% + 增益buff的数量 * Y%)
	case 4:
	{
		int layer = owner->m_buff_mgr.get_profit_buff_count();
		int ret = attacker->get_max_hp() * (value * 0.0001 + layer * (param * 0.0001));
		return ret;
	}
	break;
	// 类型5公式：攻击者攻击力 * （攻击百分比X% + 弱化效果数量 * Y%)
	case 5:
	{
		int attack = attacker->get_attack();
		int layer = owner->m_buff_mgr.get_debuff_count();
		int ret = attack * (value * 0.0001 + layer * (param * 0.0001));
		return ret;
	}
	break;
	// 类型6公式：攻击者攻击力 * （攻击百分比X% + 驱散效果数量 * Y%)
	case 6:
	{
		int attack = attacker->get_attack();
		int layer = owner->m_buff_mgr.get_dispel_buff_count();
		int ret = attack * (value * 0.0001 + layer * (param * 0.0001));
		return ret;
	}
	break;
	// 类型7公式：攻击者攻击力 * （攻击生命比X% + 驱散效果数量 * Y%)
	case 7:
	{
		int attack = attacker->get_max_hp();
		int layer = attacker->m_buff_mgr.get_dispel_buff_count();
		int ret = attack * (value * 0.0001 + layer * (param * 0.0001));
		return ret;
	}
	break;
	default:
		return value;
	}
	return value;
}

uint32_t buff_element_t::calc_value(
	fight_hero_ptr owner, 
	fight_hero_ptr attacker, 
	int type, 
	int value,
	int limit_ratio )
{
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return value;
	}
	if (NULL == attacker)
	{
		log_error("buff_element_t attacker NULL");
		return value;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return value;
	}
	if (NULL == attacker->get_combat())
	{
		log_error("attacker get_combat is null");
		return value;
	}

	// 根据攻击力来计算伤害限制
	int limit = (int)( limit_ratio * attacker->get_attack() * 0.0001 );

	int ret = 0;

	switch (type)
	{
	case calc_attr_mode_value:
		ret = value;
		break;
	case calc_attr_mode_attacker_attack:
		{
			int attack = attacker->get_attack();
			ret = attack * (value * 0.0001);
		}
		break;
	case calc_attr_mode_attacker_hp:
		ret = attacker->get_max_hp() * (value * 0.0001);
		break;
	case calc_attr_mode_attacker_defend:
		ret = attacker->get_attr(attrtype_enum::def_physical) * (value * 0.0001);
		break;
	case calc_attr_mode_attacker_speed:
		ret = attacker->get_speed() * (value * 0.0001);
		break;
	case calc_attr_mode_self_hp:
		ret = owner->get_max_hp() * (value * 0.0001);
		break;
	case calc_attr_mode_attacker_damage:
		ret = attacker->m_calc_result.damage * (value * 0.0001);
		break;
	case calc_attr_mode_self_cur_hp:
		ret = attacker->get_hp() * (value * 0.0001);
		break;
	case calc_attr_mode_attacker_round_damage:
		ret = attacker->get_count_data().one_round_damage * (value * 0.0001);
		break;
	case calc_attr_mode_lucky_point:
		ret = attacker->get_attr(attrtype_enum::luck_points) * (value * 0.0001);
		break;
	case calc_attr_mode_heal_to_shield:
		ret = attacker->get_count_data().be_heal_hp * (value * 0.0001);
		break;
	case calc_attr_mode_self_max_hp_without_buff:
		ret = owner->get_max_hp_without_buff() * (value * 0.0001);
		break;
	default:
		ret = value;
		break;
	}

	if (limit == 0)
		return ret;

	if (ret > limit)
		ret = limit;

	return ret;
}


uint32_t buff_element_t::summon(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	if (buff->get_conf() == NULL)
	{
		log_error("buff_element_t conf NULL");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}
		// 召唤物ID
		uint32_t summonid = element.value1;
		// 召唤者HP比例， 如果为0 则不使用
		uint32_t master_hp_ratio = element.value2;
		// 召唤者攻击比例， 如果为0 则不使用
		uint32_t master_attack_ratio = element.value3;
		// 召唤者防御比例， 如果为0 则不使用
		uint32_t master_defend_ratio = element.value4;
		// 场上最大数量
		uint32_t max_count = element.value5;
		// 是否随着召唤者死亡
		uint32_t death_with_master = element.value6;
		if (!attacker->summon(
			summonid,
			master_hp_ratio,
			master_attack_ratio,
			master_defend_ratio,
			max_count,
			death_with_master,
			buff->get_id(),
			buff->get_tid(),
			buff->get_skill()))
		{
			return buff_notify_none;
		}


		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::soul_relive(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
    if (NULL == buff)
    {
        log_error("buff_element_t buff NULL");
        return buff_notify_none;
    }
    if (NULL == owner)
    {
        log_error("buff_element_t owner NULL");
        return buff_notify_none;
    }
    if (NULL == owner->get_combat())
    {
        log_error("owner get_combat is null");
        return buff_notify_none;
    }

    if (buff->get_conf() == NULL)
    {
        log_error("buff_element_t conf NULL");
        return buff_notify_none;
    }

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}

		// 最大复活次数
		int max_soul_relive_count = element.value1;
		if (max_soul_relive_count <= 0)
			max_soul_relive_count = 99999999;

		// 如果当前复活次数超过最大次数，则不生效
		if ((int)owner->get_soul_relive_count() >= max_soul_relive_count)
			return buff_notify_none;

		// 如果没有死亡则不能复活
		if (!owner->can_relive())
			return buff_notify_none;
		// 复活时进度条
		int speed = element.value2;
		//if (speed == 0)
		//	speed = -1;
		// 复活血量计算方式(1-固定数值，2-攻击方攻击百分比，3-攻击方生命百分比，4-攻击方防御百分比，5-攻击方速度百分比，6-自身生命百分比)
		int hp_type = element.value3;
		// 复活血量参数
		int hp_param = element.value4;
		// 伤害限制
		int limit_ratio = element.value5;
		// 复活回合数
		int relive_round = element.value6;
		if (relive_round == 0)
			relive_round = 1;
		// 复活时 是否自动释放技能
		int control_by_system = element.value7;

		int relive_hp = calc_value(owner, attacker, hp_type, hp_param, limit_ratio);

		owner->on_soul_relive(
			attacker, 
			buff->get_id(), 
			buff->get_tid(), 
			buff->get_skill(), 
			relive_hp, 
			speed, 
			relive_round,
			control_by_system);
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::relive(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	BuffTable *conf = buff->get_conf();
	if (NULL == conf)
	{
		log_error("buff_element_t conf NULL");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}

		// 最大复活次数
		int max_relive_count = element.value1;
		if (max_relive_count <= 0)
			max_relive_count = 99999999;
		
		// 如果当前复活次数超过最大次数，则不生效
		if ((int)owner->get_relive_count() >= max_relive_count)
			return buff_notify_none;

		// 如果没有死亡则不能复活
		if (!owner->can_relive())
			return buff_notify_none;

		// 复活时进度条
		int speed = element.value2;
		//if (speed == 0)
		//	speed = -1;
		// 复活血量计算方式(1-固定数值，2-攻击方攻击百分比，3-攻击方生命百分比，4-攻击方防御百分比，5-攻击方速度百分比，6-自身生命百分比)
		int hp_type = element.value3;
		// 复活血量参数
		int hp_param = element.value4;
		// 伤害限制
		int limit_ratio = element.value6;
		
		int relive_hp = calc_value( owner, attacker, hp_type, hp_param, limit_ratio);

		owner->on_relive( attacker, buff->get_id(), buff->get_tid(), buff->get_skill(), relive_hp, speed);

		uint32_t relive_skill_cd = element.value5;
		if (relive_skill_cd > 0)
		{
			attacker->add_skill_cd_ex( buff->get_skill(), relive_skill_cd );
		}
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::shield(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	BuffTable *conf = buff->get_conf();
	if (NULL == conf)
	{
		log_error("buff_element_t conf NULL");
		return buff_notify_none;
	}
	fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
	if (NULL == attacker)
	{
		log_error("buff_element_t attacker is null");
		return buff_notify_none;
	}

	bool is_zero_value = false;
	switch (buff->get_step())
	{
	case buff_step_load:
		{
			// 计算方式(1-固定数值，2-攻击方攻击百分比，3-攻击方生命百分比，4-攻击方防御百分比，5-攻击方速度百分比，6-自身生命百分比)
			int type = element.value1;
			// 参数
			int param = element.value2;
			// 参数3 盾小类型ID
			int shield_small_type = element.value3;
			// 伤害限制
			int limit_ratio = element.value4;
			//盾的id
			int shield_id = element.value5;

			owner->get_count_data().shield_small_type = shield_small_type;

			int value = calc_value(owner, attacker, type, param, limit_ratio);
			if (value == 0) {
				is_zero_value = true;
				break;
			}
			owner->set_attr(shield_id, value);
			owner->add_combat_act(proto::common::combat_act_type_attr_inc, std::abs(value), owner, attacker, attrtype_enum::shield_points, owner->get_attr(attrtype_enum::shield_points), buff->get_tid());
			//FIGHT_LOG("[%lu]buff_step_load camp[%u] pos[%u] hero_tid[%u] buff[%u] type[%u] value[%d] layer[%u]",
			//	owner->get_fight_uid(), owner->get_camp(), owner->get_pos(), owner->get_tid(), buff->get_id(), type, value, buff->get_layer());
			return buff_notify_valid;
		}
		break;
		case buff_step_unload:
			{
				int shield_id = element.value5;
				owner->set_attr(shield_id, 0);
				owner->add_combat_act(proto::common::combat_act_type_attr_dec, 0, owner, attacker, attrtype_enum::shield_points, owner->get_attr(attrtype_enum::shield_points), buff->get_tid());
			}
			break;
	default:
		break;
	}

	//如果护盾值是0 自己移除
	if (is_zero_value == true) {
		owner->m_buff_mgr.remove_buff_by_small_type(owner->get_count_data().shield_small_type);
        FIGHT_LOG("shiled value is null remove buff");
	}

	return buff_notify_none;
}


uint32_t buff_element_t::change_cd(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	BuffTable *conf = buff->get_conf();
	if (NULL == conf)
	{
		log_error("buff_element_t conf NULL");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		combat_ptr p_combat = owner->get_combat();
		if( NULL == p_combat )
			return buff_notify_none;
		
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (attacker == NULL)
		{
			log_error("attacker is NULL");
			return buff_notify_none;
		}
		int cd = element.value1;
		int rand = element.value2;
		if (rand != 0)
		{
			if (cd > 0)
				owner->inc_random_skill_cd(attacker, cd);
			else
				owner->dec_random_skill_cd(attacker, std::abs<int>(cd));
		}
		else
		{
			if (cd > 0)
				owner->inc_all_skill_cd(attacker, cd);
			else
				owner->dec_all_skill_cd(attacker, std::abs<int>(cd));
		}
		
		p_combat->set_hero_skill_cd(owner);

		//FIGHT_LOG("[%lu]buff_step_load camp[%u] pos[%u] hero_tid[%u] buff[%u] type[%u] value[%d] layer[%u]",
		//	owner->get_fight_uid(), owner->get_camp(), owner->get_pos(), owner->get_tid(), buff->get_id(), type, value, buff->get_layer());
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::cleanup(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	BuffTable *conf = buff->get_conf();
	if (NULL == conf)
	{
		log_error("buff_element_t conf NULL");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (attacker == NULL)
			attacker = owner;
		int num = element.value1;
		buff_dispel_info_vec list;
		if (element.value2 != 0)
			list.push_back(element.value2);
		if (element.value3 != 0)
			list.push_back(element.value3);

		owner->do_dispel_skill_buff(owner, attacker, buff->get_skill(), 10000, num, list);

		//FIGHT_LOG("[%lu]buff_step_load camp[%u] pos[%u] hero_tid[%u] buff[%u] type[%u] value[%d] layer[%u]",
		//	owner->get_fight_uid(), owner->get_camp(), owner->get_pos(), owner->get_tid(), buff->get_id(), type, value, buff->get_layer());
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::switch_hp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if(NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
		{
			fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
			if (NULL == attacker)
			{
				log_error("attacker is null");
				return buff_notify_none;
			}
			if (attacker->get_max_hp() == 0)
			{
				log_error("attacker is max hp is zero");
				return buff_notify_none;
			}
			if (owner->get_max_hp() == 0)
			{
				log_error("owner is max hp is zero");
				return buff_notify_none;
			}
			//int value = element.value1;
			float attack_ratio = (float)attacker->get_hp() / (float)attacker->get_max_hp();
			float owner_ratio = (float)owner->get_hp() / (float)owner->get_max_hp();

			// do attacker
			{
				int32_t result = owner_ratio * attacker->get_max_hp();
				if ((uint32_t)result > attacker->get_hp())
				{
					attacker->add_hp(result - attacker->get_hp());
					FIGHT_LOG("%lu buff[%d] result:%d layer:%d", owner->get_combat()->get_fight_uid(), buff->get_id(), result, buff->get_layer());

					attacker->add_combat_act(proto::common::combat_act_type_buff_heal, result, owner, attacker, attacker->get_attr(attrtype_enum::hp), buff->get_id(), buff->get_tid());
				}
				else
				{
					result = SAFE_SUB(attacker->get_hp(),(uint32_t)result);
					int32_t absorb = 0;
					fight_hero_t::do_target_shield_points(attacker, result, absorb);
					attacker->sub_hp(result, attacker, buff->get_skill());		//这里注释一下，只能这么补丁写了没法统一
					//attacker->sub_hp(attacker->get_hp() - result, attacker, buff->get_skill());
					FIGHT_LOG("%lu buff[%d] result:%d layer:%d", owner->get_combat()->get_fight_uid(), buff->get_id(), result, buff->get_layer());
					attacker->add_combat_act(proto::common::combat_act_type_damage, result, owner, attacker, attacker->get_attr(attrtype_enum::hp), buff->get_id(), buff->get_tid(), absorb);
				}
			
			}
			{
				int32_t result = attack_ratio * owner->get_max_hp();
				if ((uint32_t)result > owner->get_hp())
				{
					owner->add_hp((uint32_t)result - owner->get_hp());
					FIGHT_LOG("%lu buff[%d] result:%d layer:%d", owner->get_combat()->get_fight_uid(), buff->get_id(), result, buff->get_layer());

					owner->add_combat_act(proto::common::combat_act_type_buff_heal, result, owner, attacker, owner->get_attr(attrtype_enum::hp), buff->get_id(), buff->get_tid());
				}
				else
				{
					result = SAFE_SUB(attacker->get_hp(),  (uint32_t)result);
					int32_t absorb = 0;
					fight_hero_t::do_target_shield_points(owner, result, absorb);
					owner->sub_hp(result, attacker, buff->get_skill());
					FIGHT_LOG("%lu buff[%d] result:%d layer:%d", owner->get_combat()->get_fight_uid(), buff->get_id(), result, buff->get_layer());

					owner->add_combat_act(proto::common::combat_act_type_damage, result, owner, attacker, owner->get_attr(attrtype_enum::hp), buff->get_id(), buff->get_tid(), absorb);
				}
			}
			return buff_notify_valid;
		}
		break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::switch_speed(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
		{
			int32_t max_att_speed = owner->get_combat()->get_max_att_speed();
			int32_t att_speed = element.value1 * max_att_speed / 10000.0f;
			int32_t att_more_speed = 0;
			if (att_speed == 0)
			{
				log_error("buff_element_t change_att_speed att_speed = 0 element.value1[%d] max_att_speed[%d]", element.value1, max_att_speed);
				return buff_notify_none;
			}
			if (att_speed > 0)
			{
				int32_t dec_att_more_speed = owner->get_att_speed() + att_speed - max_att_speed;
				if (owner->get_att_speed() >= max_att_speed)
				{
					att_more_speed = att_speed + owner->get_att_more_speed();
				}
				else if (dec_att_more_speed > 0)
				{
					att_more_speed = dec_att_more_speed + owner->get_att_more_speed();
				}
				if (att_more_speed > 0)
					owner->set_att_more_speed(att_more_speed);
			}
			else
			{
				owner->set_att_more_speed(0);
			}
			owner->set_att_speed(owner->get_att_speed() + att_speed);

			FIGHT_LOG("[%lu]buff_step_load camp[%u] hero_pos[%u] hero_tid[%u] buff[%u]",
				owner->get_fight_uid(), owner->get_camp(), owner->get_pos(), owner->get_tid(), buff->get_id());
			return buff_notify_valid;
		}
		break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::balance_hp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}
		if (attacker->get_max_hp() == 0)
		{
			log_error("attacker is max hp is zero");
			return buff_notify_none;
		}
		if (owner->get_max_hp() == 0)
		{
			log_error("owner is max hp is zero");
			return buff_notify_none;
		}
		//int value = element.value1;
		float attack_ratio = (float)attacker->get_hp() / (float)attacker->get_max_hp();
		float owner_ratio = (float)owner->get_hp() / (float)owner->get_max_hp();
		if (attack_ratio > owner_ratio)
		{
			uint32_t result = ( attack_ratio - owner_ratio ) * owner->get_max_hp();
			result = owner->heal_hp(attacker,result);
			FIGHT_LOG("%lu buff[%d] result:%d layer:%d", owner->get_combat()->get_fight_uid(), buff->get_id(), result, buff->get_layer());

			owner->add_combat_act(proto::common::combat_act_type_buff_heal, result, owner, attacker, owner->get_attr(attrtype_enum::hp), buff->get_id(), buff->get_tid());
		}
		else
		{
			uint32_t result = ( owner_ratio - attack_ratio ) * attacker->get_max_hp();
			result = attacker->heal_hp(attacker,result);
			FIGHT_LOG("%lu buff[%d] result:%d layer:%d", attacker->get_combat()->get_fight_uid(), buff->get_id(), result, buff->get_layer());

			attacker->add_combat_act(proto::common::combat_act_type_buff_heal, result, owner, attacker, attacker->get_attr(attrtype_enum::hp), buff->get_id(), buff->get_tid());
		}
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::balance_speed(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		int32_t max_att_speed = owner->get_combat()->get_max_att_speed();
		int32_t att_speed = element.value1 * max_att_speed / 10000.0f;
		int32_t att_more_speed = 0;
		if (att_speed == 0)
		{
			log_error("buff_element_t change_att_speed att_speed = 0 element.value1[%d] max_att_speed[%d]", element.value1, max_att_speed);
			return buff_notify_none;
		}
		if (att_speed > 0)
		{
			int32_t dec_att_more_speed = owner->get_att_speed() + att_speed - max_att_speed;
			if (owner->get_att_speed() >= max_att_speed)
			{
				att_more_speed = att_speed + owner->get_att_more_speed();
			}
			else if (dec_att_more_speed > 0)
			{
				att_more_speed = dec_att_more_speed + owner->get_att_more_speed();
			}
			if (att_more_speed > 0)
				owner->set_att_more_speed(att_more_speed);
		}
		else
		{
			owner->set_att_more_speed(0);
		}
		owner->set_att_speed(owner->get_att_speed() + att_speed);

		FIGHT_LOG("[%lu]buff_step_load camp[%u] hero_pos[%u] hero_tid[%u] buff[%u]",
			owner->get_fight_uid(), owner->get_camp(), owner->get_pos(), owner->get_tid(), buff->get_id());
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::element_test(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	log_error("buff_element_t::element_test nothing to do!");
	return buff_notify_none;
}

uint32_t buff_element_t::change_attr_without_num(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	BuffTable *conf = buff->get_conf();
	if (NULL == conf)
	{
		log_error("buff_element_t conf NULL");
		return buff_notify_none;
	}
	fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
	if (NULL == attacker)
	{
		log_error("buff_element_t attacker is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
	{
		uint32_t type = element.value1;
		int value = element.value2;
		int chg_type = element.value3;
		owner->m_buff_element_mgr.inc_element_value(type, value, chg_type);
		owner->add_combat_act(proto::common::combat_act_type_attr_inc, std::abs(value), owner, attacker, type, owner->get_attr(type), buff->get_tid());

		FIGHT_LOG("[%lu]buff_step_load camp[%u] pos[%u] hero_tid[%u] buff[%u] type[%u] value[%d] layer[%u]",
			owner->get_fight_uid(), owner->get_camp(), owner->get_pos(), owner->get_tid(), buff->get_id(), type, value, buff->get_layer());
		return element_notify_type(type);
	}
	break;
	case buff_step_unload:
	{
		uint32_t type = element.value1;
		int value = element.value2;
		int chg_type = element.value3;
		owner->m_buff_element_mgr.dec_element_value(type, value, chg_type);
		owner->add_combat_act(proto::common::combat_act_type_attr_dec, std::abs(value), owner, attacker, type, owner->get_attr(type), buff->get_tid());

		FIGHT_LOG("[%lu]buff_step_unload camp[%u] pos[%u] hero_tid[%u] buff[%u] type[%u] value[%d] layer[%u]",
			owner->get_fight_uid(), owner->get_camp(), owner->get_pos(), owner->get_tid(), buff->get_id(), type, value, buff->get_layer());
		return element_notify_type(type);
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

void buff_element_t::inc_all_team_attr(fight_hero_ptr owner, int type, int value, int chg_type, uint32_t buff_tid )
{
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return;
	}

	fight_hero_map& all_heros = owner->get_combat()->get_all_heros_ptr();
	fight_hero_ptr p_hero = fight_hero_ptr();
	for (auto& heropair : all_heros)
	{
		p_hero = heropair.second;
		if (NULL == p_hero)
		{
			log_error("inc_all_team_attr p_hero NULL");
			continue;
		}
		if (owner->get_camp() == p_hero->get_camp())
		{
			p_hero->m_buff_element_mgr.inc_element_value(type, value, chg_type);
			p_hero->add_combat_act(proto::common::combat_act_type_attr_inc, std::abs(value), p_hero, owner, type, p_hero->get_attr(type), buff_tid);
		}
	}
}

void buff_element_t::dec_all_team_attr(fight_hero_ptr owner, int type, int value, int chg_type, uint32_t buff_tid )
{
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return;
	}

	fight_hero_map& all_heros = owner->get_combat()->get_all_heros_ptr();
	fight_hero_ptr p_hero = fight_hero_ptr();
	for (auto& heropair : all_heros)
	{
		p_hero = heropair.second;
		if (NULL == p_hero)
		{
			log_error("dec_all_team_attr p_hero NULL");
			continue;
		}
		if (owner->get_camp() == p_hero->get_camp())
		{
			p_hero->m_buff_element_mgr.dec_element_value(type, value, chg_type);
			p_hero->add_combat_act(proto::common::combat_act_type_attr_dec, std::abs(value), p_hero, owner, type, p_hero->get_attr(type), buff_tid );
		}
	}
}

void buff_element_t::inc_all_enemy_attr(fight_hero_ptr owner, int type, int value, int chg_type, uint32_t buff_tid)
{
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return;
	}

	fight_hero_map& all_heros = owner->get_combat()->get_all_heros_ptr();
	fight_hero_ptr p_hero = fight_hero_ptr();
	for (auto& heropair : all_heros)
	{
		p_hero = heropair.second;
		if (NULL == p_hero)
		{
			log_error("inc_all_enemy_attr p_hero NULL");
			continue;
		}
		if (owner->get_camp() != p_hero->get_camp())
		{
			p_hero->m_buff_element_mgr.inc_element_value(type, value, chg_type);
			p_hero->add_combat_act(proto::common::combat_act_type_attr_inc, std::abs(value), p_hero, owner, type, p_hero->get_attr(type), buff_tid);
		}
	}
}

void buff_element_t::dec_all_enemy_attr(fight_hero_ptr owner, int type, int value, int chg_type, uint32_t buff_tid)
{
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return;
	}

	fight_hero_map& all_heros = owner->get_combat()->get_all_heros_ptr();
	fight_hero_ptr p_hero = fight_hero_ptr();
	for (auto& heropair : all_heros)
	{
		p_hero = heropair.second;
		if (NULL == p_hero)
		{
			log_error("dec_all_enemy_attr p_hero NULL");
			continue;
		}
		if (owner->get_camp() != p_hero->get_camp())
		{
			p_hero->m_buff_element_mgr.dec_element_value(type, value, chg_type);
			p_hero->add_combat_act(proto::common::combat_act_type_attr_dec, std::abs(value), p_hero, owner, type, p_hero->get_attr(type), buff_tid);
		}
	}
}

uint32_t buff_element_t::change_attr_all_team(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	BuffTable *conf = buff->get_conf();
	if (NULL == conf)
	{
		log_error("buff_element_t conf NULL");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
	{
		uint32_t type = element.value1;
		int value = element.value2 * buff->get_layer();
		int chg_type = element.value3;
		
		inc_all_team_attr(owner, type, value, chg_type, buff->get_tid());

		FIGHT_LOG("[%lu]buff_step_load camp[%u] pos[%u] hero_tid[%u] buff[%u] type[%u] value[%d] layer[%u]",
			owner->get_fight_uid(), owner->get_camp(), owner->get_pos(), owner->get_tid(), buff->get_id(), type, value, buff->get_layer());
		return element_notify_type(type);
	}
	break;
	case buff_step_layer_change:
	{
		uint32_t type = element.value1;
		int value = element.value2 * numchange;
		int chg_type = element.value3;
		inc_all_team_attr(owner, type, value, chg_type, buff->get_tid());
		FIGHT_LOG("[%lu]buff_step_layer_change camp[%u] pos[%u] hero_tid[%u] buff[%u] type[%u] value[%d] layer[%u] change[%d]",
			owner->get_fight_uid(), owner->get_camp(), owner->get_pos(), owner->get_tid(), buff->get_id(), type, value, buff->get_layer(), numchange);
		return element_notify_type(type);
	}
	break;
	case buff_step_unload:
	{
		uint32_t type = element.value1;
		int value = element.value2 * buff->get_layer();
		int chg_type = element.value3;
		dec_all_team_attr(owner, type, value, chg_type, buff->get_tid());

		FIGHT_LOG("[%lu]buff_step_unload camp[%u] pos[%u] hero_tid[%u] buff[%u] type[%u] value[%d] layer[%u]",
			owner->get_fight_uid(), owner->get_camp(), owner->get_pos(), owner->get_tid(), buff->get_id(), type, value, buff->get_layer());
		return element_notify_type(type);
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::change_attr_all_enemy(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	BuffTable *conf = buff->get_conf();
	if (NULL == conf)
	{
		log_error("buff_element_t conf NULL");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
	{
		uint32_t type = element.value1;
		int value = element.value2 * buff->get_layer();
		int chg_type = element.value3;
		inc_all_enemy_attr(owner, type, value, chg_type, buff->get_tid());

		FIGHT_LOG("[%lu]buff_step_load camp[%u] pos[%u] hero_tid[%u] buff[%u] type[%u] value[%d] layer[%u]",
			owner->get_fight_uid(), owner->get_camp(), owner->get_pos(), owner->get_tid(), buff->get_id(), type, value, buff->get_layer());
		return element_notify_type(type);
	}
	break;
	case buff_step_layer_change:
	{
		uint32_t type = element.value1;
		int value = element.value2 * numchange;
		int chg_type = element.value3;
		inc_all_enemy_attr(owner, type, value, chg_type, buff->get_tid());
		FIGHT_LOG("[%lu]buff_step_layer_change camp[%u] pos[%u] hero_tid[%u] buff[%u] type[%u] value[%d] layer[%u] change[%d]",
			owner->get_fight_uid(), owner->get_camp(), owner->get_pos(), owner->get_tid(), buff->get_id(), type, value, buff->get_layer(), numchange);
		return element_notify_type(type);
	}
	break;
	case buff_step_unload:
	{
		uint32_t type = element.value1;
		int value = element.value2 * buff->get_layer();
		int chg_type = element.value3;
		dec_all_enemy_attr(owner, type, value, chg_type, buff->get_tid());

		FIGHT_LOG("[%lu]buff_step_unload camp[%u] pos[%u] hero_tid[%u] buff[%u] type[%u] value[%d] layer[%u]",
			owner->get_fight_uid(), owner->get_camp(), owner->get_pos(), owner->get_tid(), buff->get_id(), type, value, buff->get_layer());
		return element_notify_type(type);
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::change_temp_attr(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}



	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{

		uint32_t type = element.value1;
		int value = element.value2 * buff->get_layer();
		int chg_type = element.value3;


		owner->m_buff_element_moment_mgr.inc_element_value(type, value, chg_type);
		//owner->add_combat_act(proto::common::combat_act_type_attr_inc, std::abs(value), owner, attacker, type, owner->get_attr(type));

		FIGHT_LOG("[%lu]buff_step_load camp[%u] pos[%u] hero_tid[%u] buff[%u] type[%u] value[%d] layer[%u]",
			owner->get_fight_uid(), owner->get_camp(), owner->get_pos(), owner->get_tid(), buff->get_id(), type, value, buff->get_layer());
		return element_notify_type(type);
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::change_attr_by_hp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}



	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{

		uint32_t type = element.value1;
		int value = element.value2 * buff->get_layer();
		int chg_type = element.value3;

		float ratio = (float)element.value4 * 0.0001;
		int greater = element.value5;

		if (owner->get_max_hp() == 0) {
			log_error("hero[%u] max_hp is 0", owner->get_tid());
			return buff_notify_none;
		}

		float hp_ratio = (float)owner->get_hp() / (float)owner->get_max_hp();
		if (greater == 0)
		{
			if (hp_ratio < ratio)
				return buff_notify_none;
		}
		else
		{
			if (hp_ratio >= ratio)
				return buff_notify_none;
		}

		owner->m_buff_element_moment_mgr.inc_element_value(type, value, chg_type);
		//owner->add_combat_act(proto::common::combat_act_type_attr_inc, std::abs(value), owner, attacker, type, owner->get_attr(type));

		FIGHT_LOG("[%lu]buff_step_load camp[%u] pos[%u] hero_tid[%u] buff[%u] type[%u] value[%d] layer[%u]",
			owner->get_fight_uid(), owner->get_camp(), owner->get_pos(), owner->get_tid(), buff->get_id(), type, value, buff->get_layer());
		return element_notify_type(type);
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::change_attr_by_target_hp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}


	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr p_target = owner->get_cur_target();
		if (NULL == p_target ) {
			log_error("target is NULL ");
			return buff_notify_none;
		}

		uint32_t type = element.value1;
		int value = element.value2 * buff->get_layer();
		int chg_type = element.value3;

		float ratio = (float)element.value4 * 0.0001;
		int greater = element.value5;

		if (p_target->get_max_hp() == 0) {
			log_error("hero [%u] max_hp is 0", p_target->get_tid());
			return buff_notify_none;
		}

		float hp_ratio = (float)p_target->get_hp() / (float)p_target->get_max_hp();
		if (greater == 0)
		{
			if (hp_ratio < ratio)
				return buff_notify_none;
		}
		else
		{
			if (hp_ratio >= ratio)
				return buff_notify_none;
		}

		owner->m_buff_element_moment_mgr.inc_element_value(type, value, chg_type);
		//owner->add_combat_act(proto::common::combat_act_type_attr_inc, std::abs(value), owner, attacker, type, owner->get_attr(type));

		FIGHT_LOG("[%lu]buff_step_load camp[%u] pos[%u] hero_tid[%u] buff[%u] type[%u] value[%d] layer[%u]",
			owner->get_fight_uid(), owner->get_camp(), owner->get_pos(), owner->get_tid(), buff->get_id(), type, value, buff->get_layer());
		return element_notify_type(type);
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::change_attr(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	BuffTable *conf = buff->get_conf();
	if (NULL == conf)
	{
		log_error("buff_element_t conf NULL");
		return buff_notify_none;
	}
	fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
	if (NULL == attacker)
	{
		log_error("buff_element_t attacker is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
		case buff_step_load:
			{
				uint32_t type = element.value1;
				int value = element.value2 * buff->get_layer();
				int chg_type = element.value3;
				owner->m_buff_element_mgr.inc_element_value(type, value, chg_type);
				owner->add_combat_act(proto::common::combat_act_type_attr_inc, std::abs(value), owner, attacker, type,owner->get_attr(type), buff->get_tid());

				FIGHT_LOG("[%lu]buff_step_load camp[%u] pos[%u] hero_tid[%u] buff[%u] type[%u] value[%d] layer[%u]", 
					owner->get_fight_uid(), owner->get_camp(), owner->get_pos(), owner->get_tid(), buff->get_id(), type, value, buff->get_layer());
				return element_notify_type(type);
			}
			break;
		case buff_step_unload:
			{
				uint32_t type = element.value1;
				int value = element.value2 * buff->get_layer();
				int chg_type = element.value3;
				owner->m_buff_element_mgr.dec_element_value(type, value, chg_type);
				owner->add_combat_act(proto::common::combat_act_type_attr_dec, std::abs(value), owner, attacker, type, owner->get_attr(type), buff->get_tid());

				FIGHT_LOG("[%lu]buff_step_unload camp[%u] pos[%u] hero_tid[%u] buff[%u] type[%u] value[%d] layer[%u]",
					owner->get_fight_uid(), owner->get_camp(), owner->get_pos(), owner->get_tid(), buff->get_id(), type, value, buff->get_layer());
				return element_notify_type(type);
			}
			break;
		case buff_step_layer_change:
			{
				uint32_t type = element.value1;
				int value = element.value2 * numchange;
				int chg_type = element.value3;
				owner->m_buff_element_mgr.inc_element_value(type, value, chg_type);
				if(value > 0)
					owner->add_combat_act(proto::common::combat_act_type_attr_inc, std::abs(value), owner, attacker, type, owner->get_attr(type), buff->get_tid());
				else
					owner->add_combat_act(proto::common::combat_act_type_attr_dec, std::abs(value), owner, attacker, type, owner->get_attr(type), buff->get_tid());

				FIGHT_LOG("[%lu]buff_step_layer_change camp[%u] pos[%u] hero_tid[%u] buff[%u] type[%u] value[%d] layer[%u] change[%d]",
					owner->get_fight_uid(), owner->get_camp(), owner->get_pos(), owner->get_tid(), buff->get_id(), type, value, buff->get_layer(), numchange);
				return element_notify_type(type);
			}
			break;
		default:
			break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::pursue_and_attack(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		if (owner->is_dead()) 
		{
			return buff_notify_none;
		}

		//多段攻击自己，自己只触发一次追击
		if (owner->get_count_data().pursue_and_attack_count >= 1)
		{
			return buff_notify_none;
		}

		SkillEffectTable* skill_effect_conf = NULL;
		uint32_t skill = element.value1;
		uint32_t skill_level = element.value2;
		uint32_t target_type = element.value3;
		uint32_t target_num  = element.value4;

		if (skill == 0)
		{
			skill_effect_conf = owner->get_skill_effect(owner->get_default_skill());
		}
		else
		{
			skill_effect_conf = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(skill, skill_level));
		}
		
		if (NULL == skill_effect_conf)
		{
			log_error("skill_effect_conf is null %u", skill);
			return buff_notify_none;
		}

		if (owner->is_dead() || owner->is_in_control()) {
			return buff_notify_none;
		}

		std::vector<fight_hero_ptr> out;
		owner->get_target_by_buff_select_target(owner, target_type, target_num, out);
		for (auto p_hero : out) {
			if (NULL != p_hero) {
				if (event_num == event_on_death) {
					owner->trigger_skill_by_buff(skill_effect_conf, p_hero, proto::common::combat_act_type_death_skill);
				}
				else {
					owner->trigger_skill_by_buff(skill_effect_conf, p_hero, proto::common::combat_act_type_follow);
				}
				//添加一轮的追击次数
				++owner->get_count_data().pursue_and_attack_count;
			}
		}
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::taunt(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	BuffTable *conf = buff->get_conf();
	if (NULL == conf)
	{
		log_error("buff_element_t conf NULL");
		return buff_notify_none;
	}
	fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
	if (NULL == attacker)
	{
		log_error("buff_element_t attacker is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		{
			owner->set_be_taunt_info(attacker, true, buff->get_small_type(), buff->get_tid());
			owner->set_taunt_me_hero(buff->get_attacker());
			return element_notify_type(attrtype_enum::taunt);
		}
		break;
	case buff_step_unload:
		{
			owner->set_be_taunt_info(attacker, false, buff->get_small_type(), buff->get_tid());
			owner->clear_taunt_me_hero();
			return element_notify_type(attrtype_enum::taunt);
		}
		break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::forget(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	BuffTable *conf = buff->get_conf();
	if (NULL == conf)
	{
		log_error("buff_element_t conf NULL");
		return buff_notify_none;
	}
	fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
	if (NULL == attacker)
	{
		log_error("buff_element_t attacker is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		{
			owner->m_buff_element_mgr.inc_element_value(attrtype_enum::forget, 1);
			owner->add_combat_act(proto::common::combat_act_type_attr_inc, 1, owner, attacker, attrtype_enum::forget, 1, buff->get_tid());
			owner->on_forget();
			return element_notify_type(attrtype_enum::forget);
		}
		break;
	case buff_step_unload:
		{
			owner->m_buff_element_mgr.dec_element_value(attrtype_enum::forget, 1);
			owner->add_combat_act(proto::common::combat_act_type_attr_dec, 0, owner, attacker, attrtype_enum::forget, 0, buff->get_tid());
			owner->on_de_forget();
			return element_notify_type(attrtype_enum::forget);
		}
		break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::silent(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	BuffTable *conf = buff->get_conf();
	if (NULL == conf)
	{
		log_error("buff_element_t conf NULL");
		return buff_notify_none;
	}
	fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
	if (NULL == attacker)
	{
		log_error("buff_element_t attacker is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
	{
		owner->m_buff_element_mgr.inc_element_value(attrtype_enum::silent, 1);
		owner->add_combat_act(proto::common::combat_act_type_attr_inc, 1, owner, attacker, attrtype_enum::silent, 1, buff->get_tid());
		return element_notify_type(attrtype_enum::silent);
	}
	break;
	case buff_step_unload:
	{
		owner->m_buff_element_mgr.dec_element_value(attrtype_enum::silent, 1);
		owner->add_combat_act(proto::common::combat_act_type_attr_dec, 1, owner, attacker, attrtype_enum::silent, 0, buff->get_tid());
		return element_notify_type(attrtype_enum::silent);
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::trigger_skill_by_hp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		uint32_t skill = element.value1;
		uint32_t attacker_type = element.value2;
		uint32_t target_type = element.value3;
		uint32_t skill_level = element.value4;
		uint32_t max_count = element.value5;
		if (max_count == 0)
			max_count = 1;
		float hp_ratio = element.value6 * 0.0001;
		uint32_t in_control = element.value7;

		fight_hero_ptr attacker = NULL;
		fight_hero_ptr target = NULL;
		if (attacker_type == 0)
		{
			attacker = owner;
		}
		else
		{
			attacker = owner->get_combat()->find_hero(buff->get_attacker());
		}

		if (NULL == attacker) {
			log_error(" attacker is NULL");
			return buff_notify_none;
		}

		if (skill == 0)
		{
			skill = attacker->get_default_skill();
			skill_level = attacker->get_skill_level(skill);
		}

		SkillEffectTable* skill_effect_conf = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(skill, skill_level));
		if (NULL == skill_effect_conf)
		{
			log_error("skill_effect_conf is null %u", skill);
			return buff_notify_none;
		}

		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}

		// 对自己释放的技能不发生效果
		if (owner->get_cur_attacker() == owner)
		{
			return buff_notify_none;
		}


		// 血量百分比
		float ratio = (float)attacker->get_hp() / (float)owner->get_max_hp();
		if (ratio > hp_ratio)
		{
			return buff_notify_none;
		}

		// 触发次数
		if (attacker->get_count_data().trigger_skill_by_hp >= max_count)
		{
			return buff_notify_none;
		}

		if (in_control != 0 && attacker->is_in_control())
		{
			return buff_notify_none;
		}

		++attacker->get_count_data().trigger_skill_by_hp;
		attacker->trigger_skill_count(attacker->trigger_skill_count() + 1);

		target = get_target_by_type(owner, attacker, skill_effect_conf, buff, target_type);
		if (NULL == target) {
			log_error("target is null");
			return buff_notify_none;
		}
		attacker->trigger_skill_by_buff(skill_effect_conf, target, proto::common::combat_act_type_trigger_skill);
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::trigger_skill_by_critical_blow(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		uint32_t skill = element.value1;
		uint32_t attacker_type = element.value2;
		uint32_t target_type = element.value3;
		uint32_t skill_level = element.value4;
		uint32_t max_count = element.value5;
		if (max_count == 0)
			max_count = 1;

		// 是否受控制影响
		uint32_t in_control = element.value6;

		fight_hero_ptr attacker = NULL;
		fight_hero_ptr target = NULL;
		if (attacker_type == 0)
		{
			attacker = owner;
		}
		else
		{
			attacker = owner->get_combat()->find_hero(buff->get_attacker());
		}

		if (NULL == attacker) {
			log_error(" attacker is NULL");
			return buff_notify_none;
		}

		if (skill == 0)
		{
			skill = attacker->get_default_skill();
			skill_level = attacker->get_skill_level(skill);
		}

		SkillEffectTable* skill_effect_conf = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(skill, skill_level));
		if (NULL == skill_effect_conf)
		{
			log_error("skill_effect_conf is null %u", skill);
			return buff_notify_none;
		}

		if (attacker->get_count_data().trigger_skill_by_critical_blow >= max_count)
		{
			return buff_notify_none;
		}

		if (in_control != 0 && attacker->is_in_control())
		{
			return buff_notify_none;
		}

		++attacker->get_count_data().trigger_skill_by_critical_blow;
		attacker->trigger_skill_count(attacker->trigger_skill_count() + 1);

		target = get_target_by_type(owner, attacker, skill_effect_conf, buff, target_type);
		if (NULL == target) {
			log_error("target is null");
			return buff_notify_none;
		}
		attacker->trigger_skill_by_buff(skill_effect_conf, target, proto::common::combat_act_type_trigger_skill);
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::trigger_skill_by_beattack_count(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		uint32_t skill = element.value1;
		uint32_t attacker_type = element.value2;
		uint32_t target_type = element.value3;
		uint32_t skill_level = element.value4;
		uint32_t max_count = element.value5;
		if (max_count == 0)
			max_count = 1;
		// 触发模板数
		uint32_t trigger_mod = element.value6;
		// 是否受控制影响
		uint32_t in_control = element.value7;

		fight_hero_ptr attacker = NULL;
		fight_hero_ptr target = NULL;
		if (attacker_type == 0)
		{
			attacker = owner;
		}
		else
		{
			attacker = owner->get_combat()->find_hero(buff->get_attacker());
		}

		if (NULL == attacker) {
			log_error(" attacker is NULL");
			return buff_notify_none;
		}

		//本次被攻击已经触发了技能，便不再计数且不再触发技能
		if (attacker->get_count_data().is_triggered_of_be_attack_count)
		{
			return buff_notify_none;
		}

		if (skill == 0)
		{
			skill = attacker->get_default_skill();
			skill_level = attacker->get_skill_level(skill);
		}

		SkillEffectTable* skill_effect_conf = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(skill, skill_level));
		if (NULL == skill_effect_conf)
		{
			log_error("skill_effect_conf is null %u", skill);
			return buff_notify_none;
		}

		// 对自己释放的技能不发生效果
		if (owner->get_cur_attacker() == owner)
		{
			return buff_notify_none;
		}

		if (attacker->get_count_data().trigger_skill_by_beattack_count >= max_count)
		{
			return buff_notify_none;
		}

		//技能触发前向客户端发送触发计数，如果本次触发技能，则触发前需要发送达到触发次数的计数
		//uint32_t send_count = ((attacker->get_count_data().beattack_count % trigger_mod == 0) ? trigger_mod : (attacker->get_count_data().beattack_count % trigger_mod));
		//attacker->add_combat_act(proto::common::combat_act_type_count_of_trigger_skill, send_count, owner, attacker, buff->get_id(), buff->get_tid(), buff->get_skill());
		//log_info("count is %u, owner uid: %lu, attacker uid: %lu, skill id: %u", send_count, owner->get_uid(), attacker->get_uid(), buff->get_skill());
		
		if ((attacker->get_count_data().beattack_count) % trigger_mod != 0)
		{
			return buff_notify_none;
		}

		if (in_control != 0 && attacker->is_in_control())
		{
			return buff_notify_none;
		}

		++attacker->get_count_data().trigger_skill_by_beattack_count;
		attacker->trigger_skill_count(attacker->trigger_skill_count() + 1);

		target = get_target_by_type(owner, attacker, skill_effect_conf, buff, target_type);
		if (NULL == target) {
			log_error("target is null");
			return buff_notify_none;
		}

		attacker->trigger_skill_by_buff(skill_effect_conf, target, proto::common::combat_act_type_trigger_skill);
		attacker->get_count_data().is_triggered_of_be_attack_count = true;

		//技能触发后需要再次向客户端发送计数，触发后计数为0
		//send_count = attacker->get_count_data().beattack_count % trigger_mod;
		//owner->add_combat_act(proto::common::combat_act_type_count_of_trigger_skill, send_count, owner, attacker, buff->get_id(), buff->get_tid(), buff->get_skill());

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::trigger_skill_by_attack_count(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		uint32_t skill = element.value1;
		uint32_t attacker_type = element.value2;
		uint32_t target_type = element.value3;
		uint32_t skill_level = element.value4;
		uint32_t max_count = element.value5;
		if (max_count == 0)
			max_count = 1;
		// 触发模板数
		uint32_t trigger_mod = element.value6;
		// 是否受控制影响
		uint32_t in_control = element.value7;

		fight_hero_ptr attacker = NULL;
		fight_hero_ptr target = NULL;
		if (attacker_type == 0)
		{
			attacker = owner;
		}
		else
		{
			attacker = owner->get_combat()->find_hero(buff->get_attacker());
		}

		if (NULL == attacker) {
			log_error(" attacker is NULL");
			return buff_notify_none;
		}

		if (skill == 0)
		{
			skill = attacker->get_default_skill();
			skill_level = attacker->get_skill_level(skill);
		}

		SkillEffectTable* skill_effect_conf = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(skill, skill_level));
		if (NULL == skill_effect_conf)
		{
			log_error("skill_effect_conf is null %u", skill);
			return buff_notify_none;
		}

		if (attacker->get_count_data().trigger_skill_by_attack_count >= max_count)
		{
			return buff_notify_none;
		}

		//技能触发前向客户端发送触发计数，如果本次触发技能，则触发前需要发送达到触发次数的计数
		uint32_t send_count = ((attacker->get_count_data().attack_count % trigger_mod == 0) ? trigger_mod : (attacker->get_count_data().attack_count % trigger_mod));
		owner->add_combat_act(proto::common::combat_act_type_count_of_trigger_skill, send_count, owner, attacker, buff->get_id(), buff->get_tid(), buff->get_skill());
		//log_info("trigger_skill_by_attack_count count is %d, skill id: %u", send_count, buff->get_skill());

		if (attacker->get_count_data().attack_count % trigger_mod != 0)
		{
			return buff_notify_none;
		}

		if (in_control != 0 && attacker->is_in_control())
		{
			return buff_notify_none;
		}

		++attacker->get_count_data().trigger_skill_by_attack_count;
		attacker->trigger_skill_count(attacker->trigger_skill_count() + 1);
		target = get_target_by_type(owner, attacker, skill_effect_conf, buff, target_type);
		if (NULL == target)
		{
			log_error("target is null");
			return buff_notify_none;
		}

		attacker->trigger_skill_by_buff(skill_effect_conf, target, proto::common::combat_act_type_trigger_skill);

		//技能触发后需要再次向客户端发送计数，触发后计数为0
		send_count = attacker->get_count_data().attack_count % trigger_mod;
		owner->add_combat_act(proto::common::combat_act_type_count_of_trigger_skill, send_count, owner, attacker, buff->get_id(), buff->get_tid(), buff->get_skill());
		//log_info("trigger_skill_by_attack_count count is %d, skill id: %u", send_count, buff->get_skill());

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

fight_hero_ptr buff_element_t::get_target_by_type(fight_hero_ptr owner, fight_hero_ptr attacker, SkillEffectTable* skill_conf, buff_ptr buff, uint32_t target_type)
{
	fight_hero_ptr target = NULL;

	if (  NULL == owner ||  NULL == owner->get_combat() || NULL == buff || NULL == skill_conf  ) {
		return target;
	}

	// 目标类型自己
	if (target_type == 0) {
		target = owner;
	}
	// 目标类型BUFF释放者
	else if (target_type == 1)
	{
		target = owner->get_combat()->find_hero(buff->get_attacker());
	}
	// 目标类型空
	else if (target_type == 2)
	{
		target = NULL;
		std::vector<fight_hero_ptr> targets;
		std::vector<fight_hero_ptr> spurting_heroes;
		owner->get_combat()->find_target_by_skill_conf(owner, skill_conf, targets, spurting_heroes, NULL, proto::common::combat_act_type::combat_act_type_NULL);
		if (!targets.empty())
			target = targets[0];
	}
	// 目标类型最近出手的人
	else if (target_type == 3)
	{
		target = owner->get_combat()->get_att_fight_hero();
	}
	// 敌方血量百分比最少的人
	else if (target_type == 4)
	{
		target = owner->get_combat()->select_min_hp_target(owner->get_enemy_camp());
	}
	return target;
}

uint32_t buff_element_t::trigger_skill(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		uint32_t skill = element.value1;
		uint32_t attacker_type = element.value2;
		uint32_t target_type = element.value3;
		uint32_t skill_level = element.value4;
		uint32_t max_count = element.value5;
		if (max_count == 0)
			max_count = 10;
		uint32_t in_control = element.value6;
		uint32_t immedia = element.value7;

		fight_hero_ptr attacker = NULL;
		fight_hero_ptr target = NULL;
		if (attacker_type == 0)
		{
			attacker = owner;
		}
		else
		{
			attacker = owner->get_combat()->find_hero(buff->get_attacker());
		}

		if (NULL == attacker) {
			log_error(" attacker is NULL");
			return buff_notify_none;
		}

		if (in_control != 0 && attacker->is_in_control())
			return buff_notify_none;

		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}

		if (skill == 0)
		{
			skill = attacker->get_default_skill();
			skill_level = attacker->get_skill_level(skill);
		}

		SkillEffectTable* skill_effect_conf = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(skill, skill_level));
		if (NULL == skill_effect_conf)
		{
			log_error("skill_effect_conf is null %u", skill);
			return buff_notify_none;
		}

		if (max_count > 0)
		{
			if ((int)attacker->trigger_skill_count() >= (int)max_count)
			{
				return buff_notify_none;
			}
		}

		++attacker->get_count_data().trigger_skill_by_attack_count;
		attacker->trigger_skill_count( attacker->trigger_skill_count() + 1 );
		target = get_target_by_type( owner, attacker, skill_effect_conf, buff, target_type );
		if (NULL == target) {
			return buff_notify_none;
		}

		if (immedia == 0) {
			attacker->trigger_skill_by_buff(skill_effect_conf, target, proto::common::combat_act_type_trigger_skill);
		}else{
			owner->do_attack(skill_effect_conf, target, proto::common::combat_act_type_trigger_skill, NULL, true);
		}
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::use_skill(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		uint32_t skill = element.value1;
		uint32_t attacker_type = element.value2;
		uint32_t target_type = element.value3;
		uint32_t skill_level = element.value4;

		fight_hero_ptr attacker = NULL;
		fight_hero_ptr target = NULL;
		if (attacker_type == 0)
		{
			attacker = owner;
		}
		else
		{
			attacker = owner->get_combat()->find_hero(buff->get_attacker());
		}
		
		if (NULL == attacker) {
			log_error(" attacker is NULL");
			return buff_notify_none;
		}

		if (skill == 0)
		{
			skill = attacker->get_default_skill();
			skill_level = attacker->get_skill_level(skill);
		}

		SkillEffectTable* skill_effect_conf = GET_CONF(SkillEffectTable, hash_util_t::hash_to_uint32_move_7(skill, skill_level));;
		if (NULL == skill_effect_conf)
		{
			log_error("skill_effect_conf is null %u", skill);
			return buff_notify_none;
		}

		target = get_target_by_type(owner, attacker, skill_effect_conf, buff, target_type);
		if (NULL == target) {
			log_error("target is null");
			return buff_notify_none;
		}

		attacker->trigger_skill_by_buff(skill_effect_conf, target, proto::common::combat_act_type_trigger_skill);
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::add_sp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		int value = element.value1;
		if (value > 0)
		{
			owner->add_sp(value);
		}
		else
		{
			owner->sub_sp(-value);
		}

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}


//转移buff->owner身上的buff 到指定的 目标身上
uint32_t buff_element_t::steal_buff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}

		fight_hero_ptr target = attacker->get_cur_target();
		if (NULL == target) {
			log_error("target is null");
			return buff_notify_none;
		}

		uint32_t target_type = element.value1;	//目标类型
		uint32_t target_num = element.value2;	//目标数量
		int32_t effect_type = element.value3;	//效果类型(0表示增益 ，-1减益， 其他就是小类型)
		uint32_t min_rand_num = element.value4;	//最小数量
		uint32_t max_rand_num = element.value5;	//最大数量

		int rand_num = random_util_t::randBetween(min_rand_num, max_rand_num);
		if (rand_num < 0) {
			return buff_notify_none;
		}

		for (int i = 0; i < rand_num; ++i) {
			buff_ptr buff = owner->m_buff_mgr.get_random_buff_by_smalltype(effect_type);
			if (NULL == buff) {//没有buff了
				return buff_notify_none;
			}

			uint32_t tid = buff->get_tid();
			uint32_t id = buff->get_id();
			owner->m_buff_mgr.remove_buff(id);

			std::vector<fight_hero_ptr> out;
			owner->get_target_by_buff_select_target(target, target_type, target_num, out);
			for (auto p_hero : out) {
				if (NULL != p_hero) {
					p_hero->add_combat_act(proto::common::combat_act_type_steal_buff, buff->get_id(), p_hero, owner);
					p_hero->m_buff_mgr.add_buff(tid, owner->get_uid(), 0, buff->get_layer(), buff->get_round() );
				}
			}
		}
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::damage(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	case buff_step_detonate:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}

		int type = element.value1;
		int value = element.value2;
		int not_dot = element.value3;
		// 伤害限制
		int limit_ratio = element.value4;
		int calcvalue = calc_value(owner, attacker, type, value, limit_ratio);

		int32_t result = calcvalue * buff->get_layer();
		if (owner->get_attr(attrtype_enum::debuff_damage_strengthen))
			result *= 2;
		
		int32_t absorb = 0;
		fight_hero_t::do_target_shield_points(owner, result, absorb);
		owner->sub_hp(result, attacker);
		FIGHT_LOG("%lu buff[%d] result:%d layer:%d", owner->get_combat()->get_fight_uid(), buff->get_id(), result, buff->get_layer());
		
		if(not_dot)
			owner->add_combat_act(proto::common::combat_act_type_buff_damage, result, owner, attacker, owner->get_attr(attrtype_enum::hp), buff->get_id(),buff->get_tid(), absorb);
		else {
			owner->add_combat_act(proto::common::combat_act_type_buff_dot, result, owner, attacker, owner->get_attr(attrtype_enum::hp), buff->get_id(), buff->get_tid(), absorb);
		}
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::sub_max_hp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker) {
			log_error("attacker is null");
			return buff_notify_none;
		}

		if (owner->is_dead())
			return buff_notify_none;

		if (owner->is_boss())//boss不受该元素的影响
		{
			owner->add_combat_act(proto::common::combat_act_type_immuno, 1, owner, attacker);
			return buff_notify_none;
		}

		int type = element.value1;		//计算类型
		int value = element.value2;		//计算值
		int limit_ratio = element.value3; //伤害限制

		int32_t result = calc_value(owner, attacker, type, value, limit_ratio) * -1;	//添加一个负值

		owner->m_buff_element_mgr.inc_element_value(attrtype_enum::max_hp + EXTRA_ATTR_PLUS, result, 0);
		owner->add_combat_act(proto::common::combat_act_type_attr_inc, std::abs(result), owner, attacker, attrtype_enum::max_hp + EXTRA_ATTR_PLUS, owner->get_attr(attrtype_enum::max_hp), buff->get_tid());

		uint32_t n_sub_hp = owner->get_hp() > owner->get_max_hp() ? owner->get_hp() - owner->get_max_hp() : 0;
		if (n_sub_hp > 0) {
			owner->sub_hp(n_sub_hp, owner, buff->get_skill());
			owner->add_combat_act(proto::common::combat_act_type_attr_dec, std::abs(n_sub_hp), owner, attacker, attrtype_enum::hp, owner->get_attr(attrtype_enum::hp), buff->get_tid());
		}
		buff->add_trigger_attr(attrtype_enum::max_hp + EXTRA_ATTR_PLUS, result);
		return buff_notify_valid;
	}
	break;
	case buff_step_unload: {
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker) {
			log_error("attacker is null");
			return buff_notify_none;
		}

		if (owner->is_dead())
			return buff_notify_none;

		if (owner->is_boss())//boss不受该元素的影响
		{
			return buff_notify_none;
		}

		uint32_t old_max_hp = owner->get_max_hp();
		int32_t value = buff->get_trigger_attr(attrtype_enum::max_hp + EXTRA_ATTR_PLUS) * -1;
		owner->m_buff_element_mgr.dec_element_value(attrtype_enum::max_hp + EXTRA_ATTR_PLUS, value, 0);
		owner->add_combat_act(proto::common::combat_act_type_attr_dec, std::abs(value), owner, attacker, attrtype_enum::max_hp + EXTRA_ATTR_PLUS, owner->get_attr(attrtype_enum::max_hp), buff->get_tid());

		//满血才奶一口
		if (owner->get_hp() == old_max_hp) {
			owner->add_hp(abs(value));
			owner->add_combat_act(proto::common::combat_act_type_buff_heal, abs(value), owner, attacker, owner->get_attr(attrtype_enum::hp), buff->get_id(), buff->get_tid());
		}
		buff->clear_trigger_attr();
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::add_max_hp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker) {
			log_error("attacker is null");
			return buff_notify_none;
		}

		if (owner->is_dead())
			return buff_notify_none;

		if (owner->is_boss())//boss不受该元素的影响
		{
			owner->add_combat_act(proto::common::combat_act_type_immuno, 1, owner, attacker);
			return buff_notify_none;
		}

		int type = element.value1;		//计算类型
		int value = element.value2;		//计算值
		int limit_ratio = element.value3; //伤害限制

		uint32_t result = calc_value(owner, attacker, type, value, limit_ratio);

		uint32_t old_max_hp = owner->get_attr(attrtype_enum::max_hp);

		owner->m_buff_element_mgr.inc_element_value(attrtype_enum::max_hp + EXTRA_ATTR_PLUS, result, 0);
		owner->add_combat_act(proto::common::combat_act_type_attr_inc, std::abs(result), owner, attacker, attrtype_enum::max_hp + EXTRA_ATTR_PLUS, owner->get_attr(attrtype_enum::max_hp), buff->get_tid());

		//满血才奶一口
		if (owner->get_hp() == old_max_hp) {
			owner->add_hp(result);
			owner->add_combat_act(proto::common::combat_act_type_buff_heal, result, owner, attacker, owner->get_attr(attrtype_enum::hp), buff->get_id(), buff->get_tid());
		}
		buff->add_trigger_attr(attrtype_enum::max_hp + EXTRA_ATTR_PLUS, result);
		return buff_notify_valid;
	}
	break;
	case buff_step_unload:	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker) {
			log_error("attacker is null");
			return buff_notify_none;
		}

		if (owner->is_dead())
			return buff_notify_none;

		if (owner->is_boss())//boss不受该元素的影响
		{
			return buff_notify_none;
		}

		int32_t value = buff->get_trigger_attr(attrtype_enum::max_hp + EXTRA_ATTR_PLUS);
		owner->m_buff_element_mgr.dec_element_value(attrtype_enum::max_hp + EXTRA_ATTR_PLUS, value, 0);
		owner->add_combat_act(proto::common::combat_act_type_attr_dec, std::abs(value), owner, attacker, attrtype_enum::max_hp + EXTRA_ATTR_PLUS, owner->get_attr(attrtype_enum::max_hp), buff->get_tid());

		uint32_t n_sub_hp = owner->get_hp() > owner->get_max_hp() ? owner->get_hp() - owner->get_max_hp() : 0;
		if ( n_sub_hp > 0 ) {
			owner->sub_hp( n_sub_hp, owner, buff->get_skill() );
			owner->add_combat_act(proto::common::combat_act_type_attr_dec, std::abs(n_sub_hp), owner, attacker, attrtype_enum::hp, owner->get_attr(attrtype_enum::hp), buff->get_tid());
		}
		buff->clear_trigger_attr();
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::addhp_by_hpchange(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}
		if (owner->is_dead())
			return buff_notify_none;

		int type = element.value1;
		int buff_id = element.value2;
		int value = element.value3;
		int param = element.value4;
		float ratio = element.value5 * 0.0001;
		int greater = element.value6;

		if( owner->get_max_hp() == 0 ){
			log_error("hero %u max_hp is 0", owner->get_tid());
			return buff_notify_none;
		}

		float hp_ratio = (float)owner->get_hp() / (float)owner->get_max_hp();
		if (greater == 0)
		{
			if (hp_ratio >= ratio)
				return buff_notify_none;
		}
		else
		{
			if (hp_ratio < ratio)
				return buff_notify_none;
		}

		int calcvalue = calc_heal_special_value(owner, attacker, type, buff_id, value, param);

		uint32_t result = calcvalue * buff->get_layer();
		result = owner->heal_hp(attacker,result);
		FIGHT_LOG("%lu buff[%d] result:%d layer:%d", owner->get_combat()->get_fight_uid(), buff->get_id(), result, buff->get_layer());

		owner->add_combat_act(proto::common::combat_act_type_buff_heal, result, owner, attacker, owner->get_attr(attrtype_enum::hp), buff->get_id(), buff->get_tid());
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::heal(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}
		if (owner->is_dead())
			return buff_notify_none;
		int value = element.value2;
		int type = element.value1;
		// 伤害限制
		int limit_ratio = element.value3;
		int calcvalue = calc_value(owner, attacker, type, value, limit_ratio);


		uint32_t result = calcvalue * buff->get_layer();
		result = owner->heal_hp(attacker, result);
		FIGHT_LOG("%lu buff[%d] result:%d layer:%d", owner->get_combat()->get_fight_uid(), buff->get_id(), result, buff->get_layer());

		// 			if (buff->get_conf()->rounds() > 1)
		// 				owner->add_combat_act(proto::common::combat_act_type_continue_heal, result, owner, attacker, owner->get_attr(attrtype_enum::hp), buff->get_id(), buff->get_tid());
		// 			else
		owner->add_combat_act(proto::common::combat_act_type_buff_heal, result, owner, attacker, owner->get_attr(attrtype_enum::hp), buff->get_id(), buff->get_tid());

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::heal_special(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}
		if (owner->is_dead())
			return buff_notify_none;
		
		int type = element.value1;
		int buff_id = element.value2;
		int value = element.value3;
		int param = element.value4;

		int calcvalue = calc_heal_special_value(owner, attacker, type, buff_id, value, param);
		// 治疗加成与治疗降低

		uint32_t result = calcvalue * buff->get_layer();
		result = owner->heal_hp(attacker,result);
		FIGHT_LOG("%lu buff[%d] result:%d layer:%d", owner->get_combat()->get_fight_uid(), buff->get_id(), result, buff->get_layer());

		owner->add_combat_act(proto::common::combat_act_type_buff_heal, result, owner, attacker, owner->get_attr(attrtype_enum::hp), buff->get_id(), buff->get_tid());
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::heal_all_team(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
		{
			if (owner->is_dead())
				return buff_notify_none;

			uint32_t hp_max_percent = 0;
			uint32_t hp_percent = 0;
			uint32_t no_add_hero_pos = 0;
			std::vector<fight_hero_ptr> alive_teammembers;
			owner->get_combat()->get_camp_alive_heros(alive_teammembers, owner->get_camp());
			fight_hero_ptr p_hero = fight_hero_ptr();
			for (uint32_t i = 0; i < alive_teammembers.size(); ++i)
			{
				p_hero = alive_teammembers[i];
				if (NULL == p_hero)
				{
					log_error("heal_all_team p_hero NULL");
					continue;
				}
				if (p_hero->get_attr(attrtype_enum::max_hp) == 0)
				{
					log_error("max_hp = 0 hero_tid[%d]", p_hero->get_tid());
					continue;
				}
				hp_percent = p_hero->get_hp() * 100 / p_hero->get_attr(attrtype_enum::max_hp);
				if (hp_percent > hp_max_percent)
				{
					hp_max_percent = hp_percent;
					no_add_hero_pos = i;
				}
			}

			int32_t add_hp = 0;
			uint32_t result = 0;
			for (uint32_t i = 0; i < alive_teammembers.size(); ++i)
			{
				p_hero = alive_teammembers[i];
				if (NULL == p_hero)
				{
					log_error("heal_all_team p_hero NULL");
					continue;
				}
				if(no_add_hero_pos == i)
					continue;
				add_hp = p_hero->get_attr(attrtype_enum::max_hp) * hp_max_percent / 100 - p_hero->get_hp();
				result = p_hero->heal_hp_regardless_of_attr(add_hp);//该元素治疗队友不考虑释放者和队友的治疗增减益效果（包括禁疗）
				FIGHT_LOG("%lu buff[%d] result:%d layer:%d", owner->get_combat()->get_fight_uid(), buff->get_id(), result, buff->get_layer());
				p_hero->add_combat_act(proto::common::combat_act_type_buff_heal, result, p_hero, owner, p_hero->get_hp(), buff->get_id(), buff->get_tid());
			}
			return buff_notify_valid;
		}
		break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::remove_buff_by_small_type(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}

		int buffer_id = element.value1;

		owner->m_buff_mgr.remove_buff_by_small_type(buffer_id);

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::immuno(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	BuffTable *conf = buff->get_conf();
	if (NULL == conf)
	{
		log_error("buff_element_t conf NULL");
		return buff_notify_none;
	}
	fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
	if (NULL == attacker)
	{
		log_error("buff_element_t attacker is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
	{
		uint32_t type  = element.value2;
		owner->m_buff_element_mgr.inc_element_value(attrtype_enum::immuno, 1);
		owner->add_combat_act(proto::common::combat_act_type_attr_inc, 1, owner, attacker, attrtype_enum::immuno, 1, buff->get_tid());
		if( type == 0 )
			owner->add_immuno_big_buff_type(element.value1);
		else 
			owner->add_immuno_small_buff_type(element.value1);

		return buff_notify_valid;
	}
	break;
	case buff_step_unload:
	{
		owner->m_buff_element_mgr.dec_element_value(attrtype_enum::immuno, 1);
		owner->add_combat_act(proto::common::combat_act_type_attr_dec, 1, owner, attacker, attrtype_enum::immuno, 0, buff->get_tid());
		owner->clear_immuno_buff_type();
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::story_play(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	BuffTable *conf = buff->get_conf();
	if (NULL == conf)
	{
		log_error("buff_element_t conf NULL");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_trigger:
		{
			owner->add_combat_act(proto::common::combat_act_type_story_play, element.value1, owner, owner);
			return buff_notify_none;
		}
		break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::remove_buff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}

		int buffer_id = element.value1;

		owner->m_buff_mgr.remove_buff_by_tid(buffer_id);

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::remove_buff_layer(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}

		int buffer_id = element.value1;
		int layer = element.value2;

		owner->m_buff_mgr.sub_buff_layer_or_remove(buffer_id, 0, layer);

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}

	return buff_notify_none;

}

void buff_element_t::heal_all_team_hp(fight_hero_ptr owner, fight_hero_ptr attacker, buff_ptr buff, int value)
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return;
	}
	if (NULL == attacker)
	{
		log_error("attacker is null");
		return;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return;
	}
	if (NULL == attacker->get_combat())
	{
		log_error("attacker get_combat is null");
		return;
	}

	fight_hero_map& all_heros = owner->get_combat()->get_all_heros_ptr();
	for (auto& heropair : all_heros)
	{
		if (NULL == heropair.second) 
			continue;

		if (owner->get_camp() != heropair.second->get_camp())
			continue;
		if( owner->get_unique_id() == heropair.second->get_unique_id())
			continue;
		if(heropair.second->is_dead())
			continue;
		
		uint32_t result = heropair.second->heal_hp(owner,value);
		heropair.second->add_combat_act(proto::common::combat_act_type_buff_heal, result, heropair.second, attacker, heropair.second->get_attr(attrtype_enum::hp), buff->get_id(), buff->get_tid() );
	}
}

uint32_t buff_element_t::heal_overflow(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}
		if (owner->is_dead())
			return buff_notify_none;
		int type = element.value1;
		int value = element.value2;
		
		int ratio = element.value3;

		int hp_ratio = element.value4;
		// 伤害限制
		int limit_ratio = element.value5;
		int calcvalue = calc_value(owner, attacker, type, value, limit_ratio);

		int result = calcvalue * buff->get_layer();
		int cur_hp = owner->get_hp();
		int max_hp = owner->get_max_hp();

		
		result = owner->heal_hp(attacker,result);
		// 判断溢出值
		FIGHT_LOG("%lu buff[%d] result:%d layer:%d", owner->get_combat()->get_fight_uid(), buff->get_id(), result, buff->get_layer());
		
		owner->add_combat_act(proto::common::combat_act_type_buff_heal, result, owner, attacker, owner->get_attr(attrtype_enum::hp), buff->get_id(), buff->get_tid());

		int max_hp_ratio = max_hp * ( hp_ratio * 0.0001 );


		int need = max_hp_ratio - cur_hp;
		if (need >= result)
			return buff_notify_valid;
		uint32_t overflow = ( result - need ) * ( ratio * 0.0001 );
		if (overflow == 0)
			return buff_notify_valid;
		heal_all_team_hp(owner, attacker, buff, overflow);

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::addbuff_when_beadd_small_type(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = p_event_owner;
		if (NULL == attacker) {
			log_error("attacker is null");
			return buff_notify_none;
		}

		uint32_t small_type = element.value1;		//buff小类型
		uint32_t add_buffer_id = element.value2;	//追加buff_id
		uint32_t first_target = element.value3;
		uint32_t second_target = element.value4;
		uint32_t target_num = element.value5;//有时候这个是模型ID)

		BuffTable* conf = GET_CONF(BuffTable, owner->get_fight_calc_result().add_buff_id);
		if (NULL == conf)
			return buff_notify_none;

		if ((uint32_t)conf->buff_small_type() != small_type) {
			return buff_notify_none;
		}

		fight_hero_ptr  target = owner;
		if (first_target == 1) {
			target = attacker;
		}
		target->m_buff_mgr.add_buff(add_buffer_id, buff->get_attacker(), buff->get_skill());

		//给备选目标加
		if (second_target != 0) {
			std::vector<fight_hero_ptr> out;
			owner->get_target_by_buff_select_target(owner, second_target, target_num, out, target_num);
			for (auto p_hero : out) {
				if (NULL == p_hero) {
					continue;
				}
				if (p_hero->get_uid() == target->get_uid()) {
					continue;
				}
				p_hero->m_buff_mgr.add_buff(add_buffer_id, buff->get_attacker(), buff->get_skill());
			}
		}
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::addbuff_when_addbuff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}

		int buffer_id = element.value1;
		int add_buffer_id = element.value2;

		if ((int)owner->get_fight_calc_result().add_buff_id != buffer_id)
		{
			return buff_notify_none;
		}

		owner->m_buff_mgr.add_buff(add_buffer_id, buff->get_attacker(), buff->get_skill());

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::addbuff_when_over_layer(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_layer_change:
		{
			uint32_t convert_layer = element.value1;
			uint32_t remove_layer = element.value2;
			uint32_t add_buff_id = element.value3;
			if (buff->get_layer() >= convert_layer)
			{
				if (remove_layer >= convert_layer)
				{
					owner->m_buff_mgr.remove_buff(buff, buff->get_attacker());
				}
				else
				{
					owner->m_buff_mgr.sub_buff_layer(buff, remove_layer);
				}

				owner->m_buff_mgr.add_buff(add_buff_id, buff->get_attacker(), buff->get_skill() );
			}
			return buff_notify_valid;
		}
		break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::addbuff_when_exist(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}

		int buffer_id = element.value1;
		int add_buffer_id = element.value2;

		if (buffer_id != 0)
		{
			// 判断BUFFID是否存在
			if (owner->m_buff_mgr.get_buff_by_tid(buffer_id) == NULL)
				return buff_notify_none;
		}
		else
		{
			// 有增益追加BUFF
			if (owner->m_buff_mgr.get_profit_buff_count() == 0)
				return buff_notify_none;
		}
		owner->m_buff_mgr.add_buff(add_buffer_id, buff->get_attacker(), buff->get_skill() );

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::all_team_damage(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		// 判断攻击方与受击方
		fight_hero_ptr attacker = owner->get_combat()->get_att_fight_hero();
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}
		fight_hero_ptr target = attacker->get_cur_target();
		if (NULL == target)
		{
			log_error("target is null");
			return buff_notify_none;
		}

		// 判断是否队友
		if (target->get_camp() != owner->get_camp())
			return buff_notify_none;
		// 判断是否敌方攻击
		if (owner->get_camp() == attacker->get_camp())
			return buff_notify_none;

		int damage = attacker->get_fight_calc_result().damage;

		owner->get_count_data().all_team_damage += damage;
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::rand_damage(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	fight_hero_ptr attacker = owner;//buff所有者即攻击者

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
		{
			int min_rand_num = element.value1;//骰子下线
			int max_rand_num = element.value2;//骰子上限
			int damage_add_rate = element.value3;//伤害增加万分比
			uint32_t target_buff_id = element.value4;
			uint32_t base_rate = element.value5;//每点骰子增加的概率
			uint32_t buff_small_type = element.value6;
			uint32_t min_add_num = element.value7;//骰子下限增加数
			uint32_t max_add_num = element.value8;//骰子上限增加数
			
			fight_hero_ptr p_target = owner->get_cur_target();
			if (NULL == p_target)
			{
				log_error("target is NULL");
				return buff_notify_none;
			}

			//目标有配置的小类型buff则增加骰子上下限
			if (p_target->m_buff_mgr.get_buff_layer_by_smalltype(buff_small_type) > 0) 
			{
				min_rand_num += min_add_num;
				max_rand_num += max_add_num;
			}

			if (min_rand_num > max_rand_num)
			{
				log_error("min_rand_num is larger than max_rand_num");
				return buff_notify_none;
			}

			//增加伤害
			int rand_num = random_util_t::randBetween(min_rand_num, max_rand_num);
			int change_attr_value = rand_num * damage_add_rate;
			buff->m_run_attr.param1 = change_attr_value;

			owner->m_buff_element_mgr.inc_element_value(attrtype_enum::self_attack_addition, change_attr_value, 1);
			owner->add_combat_act(proto::common::combat_act_type_attr_inc, change_attr_value, owner, attacker, attrtype_enum::self_attack_addition, owner->get_attr(attrtype_enum::self_attack_addition), buff->get_tid());
			owner->add_combat_act(proto::common::combat_act_type_rand_damage, rand_num, owner, attacker);

			//增加概率
			uint32_t rate = rand_num * base_rate;
			if ( target_buff_id > 0 && random_util_t::selectByTenTh(rate) ){
				p_target->m_buff_mgr.add_buff( target_buff_id, owner->get_uid(), buff->get_skill() );
			}
			return element_notify_type(attrtype_enum::self_attack_addition);
		}
		break;
	case buff_step_unload:
		{
			int change_attr_value = buff->m_run_attr.param1;
			owner->m_buff_element_mgr.dec_element_value(attrtype_enum::self_attack_addition, change_attr_value, 1);
			owner->add_combat_act(proto::common::combat_act_type_attr_dec, change_attr_value, owner, attacker, attrtype_enum::self_attack_addition, owner->get_attr(attrtype_enum::self_attack_addition), buff->get_tid());
			return element_notify_type(attrtype_enum::self_attack_addition);
		}
		break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::all_team_shield(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	BuffTable *conf = buff->get_conf();
	if (NULL == conf)
	{
		log_error("buff_element_t conf NULL");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		{
			fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
			if (NULL == attacker)
			{
				log_error("attacker is null");
				return buff_notify_none;
			}
			// 计算方式(1-固定数值，2-攻击方攻击百分比，3-攻击方生命百分比，4-攻击方防御百分比，5-攻击方速度百分比，6-自身生命百分比)
			int type = element.value1;
			// 参数
			int param = element.value2;
			// 参数3 盾小类型ID
			int shield_small_type = element.value3;
			// 伤害限制
			int limit_ratio = element.value4;
			int value = calc_value(owner, attacker, type, param, limit_ratio);

			uint64_t target_obj_uid = owner->get_master();
			fight_obj_ptr p_target_obj = owner->get_combat()->find_obj(target_obj_uid);
			if (NULL == p_target_obj)
			{
				log_error("hero[%lu][%d] obj[%lu] not find", owner->get_uid(), owner->get_tid(), target_obj_uid);
				return buff_notify_none;
			}
			p_target_obj->set_team_shield(value);
			p_target_obj->set_team_shield_buff_type(shield_small_type);
			p_target_obj->set_team_shield_hero_uid(owner->get_uid());
			return buff_notify_valid;
		}
		break;
	case buff_step_unload:
		{
			uint64_t target_obj_uid = owner->get_master();
			fight_obj_ptr p_target_obj = owner->get_combat()->find_obj(target_obj_uid);
			if (NULL == p_target_obj)
			{
				log_error("hero[%lu][%d] obj[%lu] not find", owner->get_uid(), owner->get_tid(), target_obj_uid);
				return buff_notify_none;
			}
			p_target_obj->set_team_shield(0);
			p_target_obj->set_team_shield_buff_type(0);
			p_target_obj->set_team_shield_hero_uid(0);
		}
		break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::all_team_damage_turn(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	BuffTable *conf = buff->get_conf();
	if (NULL == conf)
	{
		log_error("buff_element_t conf NULL");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}
		
		uint64_t target_obj_uid = owner->get_master();
		fight_obj_ptr p_target_obj = owner->get_combat()->find_obj(target_obj_uid);
		if (NULL == p_target_obj)
		{
			log_error("hero[%lu][%d] obj[%lu] not find", owner->get_uid(), owner->get_tid(), target_obj_uid);
			return buff_notify_none;
		}
		p_target_obj->set_team_damage_turn_hero_uid(owner->get_uid());
		return buff_notify_valid;
	}
	break;
	case buff_step_unload:
	{
		uint64_t target_obj_uid = owner->get_master();
		fight_obj_ptr p_target_obj = owner->get_combat()->find_obj(target_obj_uid);
		if (NULL == p_target_obj)
		{
			log_error("hero[%lu][%d] obj[%lu] not find", owner->get_uid(), owner->get_tid(), target_obj_uid);
			return buff_notify_none;
		}
		p_target_obj->set_team_damage_turn_hero_uid(0);
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::death_guard_energy(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		// 没有开启则跳过
		//if (owner->get_count_data().max_death_guard_energy == 0)
		//	return buff_notify_none;
		int min_energy = element.value1;
		int max_energy = element.value2;
		int buff_id = element.value3;
		int count = random_util_t::randBetween(min_energy, max_energy);
		if (count <= 0)
			return buff_notify_none;
	
		owner->get_buff_manager().add_buff_layer(buff_id,0,count);

		/*int add = count;
		if (add + owner->get_count_data().death_guard_energy > owner->get_count_data().max_death_guard_energy)
			add = owner->get_count_data().max_death_guard_energy - owner->get_count_data().death_guard_energy;
		if (add <= 0)
			return buff_notify_none;*/

		//owner->get_count_data().death_guard_energy += add;
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::death_guard(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	// 触发概率
	//int energy = element.value1;
	//int max_energy = element.value2;
	

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		int small_buff_type = element.value1;
		int add_buff_id = element.value2;
		
		// 判断攻击方与受击方
		fight_hero_ptr attacker = owner->get_combat()->get_att_fight_hero();
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}
		fight_hero_ptr target = attacker->get_cur_target();
		if (NULL == target)
		{
			log_error("target is null");
			return buff_notify_none;
		}

		// 判断是否队友
		if (target->get_camp() != owner->get_camp())
			return buff_notify_none;

		// 判断是否敌方攻击
		if (owner->get_camp() == attacker->get_camp())
			return buff_notify_none;

		int damage = attacker->get_fight_calc_result().damage;

		// 判断本次伤害是否大于当前HP
		if ((int)attacker->get_fight_calc_result().damage < (int)target->get_hp())
		{
			return buff_notify_none;
		}
		
		buff_ptr p_buff = owner->get_buff_manager().get_buff_by_small_type( small_buff_type, 0);
		if (NULL == p_buff) {
			return buff_notify_none;
		}
		
		if (p_buff->get_layer() == 0) {
			return buff_notify_none;
		}

		p_buff->sub_layer();
		owner->add_combat_buff_act(proto::common::combat_act_type_buff_update, buff->get_id(), buff->get_tid(), buff->get_layer(), buff->get_round(), buff->get_attacker());

		// 增加本次死亡守护效果
		target->m_buff_element_moment_mgr.inc_element_value(attrtype_enum::athanasia, 1, 0);
		target->m_buff_mgr.add_buff(add_buff_id, buff->get_attacker(), buff->get_skill());
		
		owner->add_combat_act(proto::common::combat_act_type_death_guard, 1, target, owner, 0,0, buff->get_tid() );
		return buff_notify_valid;
	}
	break;
	case buff_step_unload:
	{
		//owner->get_count_data().death_guard_energy = 0;
		//owner->get_count_data().max_death_guard_energy = 0;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::bear_hurt(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_combat()->get_att_fight_hero();
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}
		fight_hero_ptr target = attacker->get_cur_target();
		if (NULL == target)
		{
			log_error("target is null");
			return buff_notify_none;
		}
		// 处于被控制中则不触发
		if (owner->is_in_control())
			return buff_notify_none;

		// 触发概率
		int ratio = element.value1;
		// 触发比例
		int damage_ratio = element.value2;

		if (target->get_uid() == owner->get_uid())
			return buff_notify_none;
		if (target->get_camp() != owner->get_camp())
			return buff_notify_none;
		if (owner->get_camp() == attacker->get_camp())
			return buff_notify_none;
		// 判断是否单体攻击
		if (attacker->get_cur_skill() == NULL)
			return buff_notify_none;
		if (attacker->is_single_attack(attacker->get_cur_skill()) == false)
			return buff_notify_none;
		//int damage = attacker->get_fight_calc_result().damage;
		//int bear_damage = damage * (damage_ratio * 0.0001);
		//int remain_damage = damage - bear_damage;
		
		if (!random_util_t::selectByTenTh(ratio))
		{
			return buff_notify_none;
		}

		target->set_bear_damage(damage_ratio, owner);
		owner->add_combat_act(proto::common::combat_act_type_accept_damage, 0, owner, target, 0, buff->get_id());
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::change_att_speed(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
		{
			fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
			if (NULL == attacker)
			{
				log_error("attacker is null");
				return buff_notify_none;
			}

			int32_t add_speed_per = owner->get_attr_percent(attrtype_enum::speed_effect_ratio) - owner->get_attr_percent(attrtype_enum::speed_effect_reduce_ratio);
			int32_t max_att_speed = owner->get_combat()->get_max_att_speed();

			int32_t att_speed = element.value1 * ( max_att_speed * 0.0001 ) * (1 + add_speed_per * 0.0001);
			int32_t on_event = element.value2;
			int32_t get_new_round = element.value3;

			//获得新回合
			if (get_new_round > 0) {
				owner->get_count_data().is_get_new_round = true;
				return buff_notify_valid;
			}

			int32_t att_more_speed = 0;

			proto::common::combat_act_type act_type = proto::common::combat_act_type_att_speed_inc;
			if (att_speed == 0)
			{
				log_error("buff_element_t change_att_speed att_speed = 0 element.value1[%d] max_att_speed[%d]", element.value1, max_att_speed);
				return buff_notify_none;
			}
			if (att_speed > 0)
			{
				int32_t dec_att_more_speed = owner->get_att_speed() + att_speed - max_att_speed;
				if (owner->get_att_speed() >= max_att_speed)
				{
					att_more_speed = owner->get_att_more_speed() + att_speed ;
				}
				else if (dec_att_more_speed > 0)
				{
					att_more_speed = owner->get_att_more_speed() + dec_att_more_speed;
				}
				if (att_more_speed > 0)
				{
					owner->set_att_more_speed(att_more_speed);
				}
				act_type = proto::common::combat_act_type_att_speed_inc;
				// 根据参数控制是否触发事件 0触发 1不触发
				if (on_event == 0)
				{
					// 触发施法者拉条事件 只有增加时才触发 减值不触发
					attacker->do_team_script_event(event_on_add_att_speed, attacker);
				}
			}
			else
			{
				owner->set_att_more_speed(0);
				act_type = proto::common::combat_act_type_att_speed_dec;
			}
			owner->set_att_speed(owner->get_att_speed() + att_speed);
			owner->add_combat_act(act_type, std::abs(att_speed), owner, attacker, owner->get_att_speed(), buff->get_id());

			FIGHT_LOG("[%lu]buff_step_load camp[%u] hero_pos[%u] hero_tid[%u] buff[%u]",
				owner->get_fight_uid(), owner->get_camp(), owner->get_pos(), owner->get_tid(), buff->get_id());
			return buff_notify_valid;
		}
		break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::continue_heal(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}
		if (owner->is_dead())
			return buff_notify_none;
		int value = element.value2;
		int type = element.value1;
		// 伤害限制
		int limit_ratio = element.value3;
		int calcvalue = calc_value(owner, attacker, type, value, limit_ratio);


		uint32_t result = calcvalue * buff->get_layer();
		result = owner->heal_hp(attacker, result);
		FIGHT_LOG("%lu buff[%d] result:%d layer:%d", owner->get_combat()->get_fight_uid(), buff->get_id(), result, buff->get_layer());

		owner->add_combat_act(proto::common::combat_act_type_continue_heal, result, owner, attacker, owner->get_attr(attrtype_enum::hp), buff->get_id(), buff->get_tid());

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::be_buff_add_by_hero_alive(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	uint32_t plugin_left = element.value1;
	uint32_t plugin_right = element.value2;
	uint32_t camp = element.value3;
	uint32_t buff_id = element.value4;
	uint32_t buff_target = element.value5;
	uint32_t target_num = element.value6;

	bool valid = false;

	fight_hero_map& all_heros = owner->get_combat()->get_all_heros_ptr();
	fight_hero_ptr p_hero = fight_hero_ptr();

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		for (auto& heropair : all_heros)
		{
			p_hero = heropair.second;
			if (NULL == p_hero)
			{
				log_error("inc_all_team_attr p_hero NULL");
				continue;
			}
			if (owner->get_camp() == p_hero->get_camp() && camp == 0 && p_hero->get_plugin() >= plugin_left && p_hero->get_plugin() < plugin_right)
			{
				std::vector<fight_hero_ptr> out;
				owner->get_target_by_buff_select_target(owner, buff_target, target_num, out);
				for (auto hero : out)
				{
					if (NULL == hero) {
						log_error("find p_hero is NULL , target_type : %u", buff_target);
						continue;
					}

					hero->m_buff_mgr.add_buff(buff_id, buff->get_attacker(), buff->get_skill());
					valid = true;
				}
				break;
			}
			else if (owner->get_camp() != p_hero->get_camp() && camp == 1 && p_hero->get_plugin() >= plugin_left && p_hero->get_plugin() < plugin_right)
			{
				std::vector<fight_hero_ptr> out;
				owner->get_target_by_buff_select_target(owner, buff_target, target_num, out);
				for (auto hero : out)
				{
					if (NULL == hero) {
						log_error("find p_hero is NULL , target_type : %u", buff_target);
						continue;
					}
					hero->m_buff_mgr.add_buff(buff_id, buff->get_attacker(), buff->get_skill());
					valid = true;
				}
				break;
			}
		}
	}
	break;
	}

	return valid ? buff_notify_valid : buff_notify_none;
}


uint32_t buff_element_t::be_buff_add_by_attribute(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		uint32_t attr_target = element.value1;
		uint32_t attr_type = element.value2;
		uint32_t parm1 = element.value3;
		uint32_t parm2 = element.value4;
		uint32_t buff_id = element.value5;
		uint32_t target_type = element.value6;
		uint32_t target_num = element.value7;

		uint32_t ratio = 0;

		fight_hero_ptr target = owner->get_combat()->find_hero(buff->get_attacker());
		if (attr_target == 1)//根据buff施放者属性计算
		{
			if (NULL == target)
			{
				target = owner;
			}
			ratio = (target->get_attr(attr_type) * (parm1 * 0.0001f) + parm2);
		}
		else if (attr_target == 2)//根据buff被施放者属性计算
		{
			ratio = (owner->get_attr(attr_type) * (parm1 * 0.0001f) + parm2);//owner为buff被施放者
		}

		bool valid = false;
		if (random_util_t::selectByTenTh(ratio))
		{
			std::vector<fight_hero_ptr> out;
			owner->get_target_by_buff_select_target(target, target_type, target_num, out);
			for (auto hero : out)
			{
				if (NULL == hero) {
					log_error("find p_hero is NULL , target_type : %u", target_type);
					continue;
				}
				hero->m_buff_mgr.add_buff(buff_id, buff->get_attacker(), buff->get_skill());
				valid = true;
			}

		}
		return valid ? buff_notify_valid : buff_notify_none;
	}
	default:
		return buff_notify_none;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::be_buff_add_by_other_type(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step()) 
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		uint32_t small_type = element.value1;

		uint32_t add_pencent_own = element.value2;
		uint32_t target_type_own = element.value3;
		uint32_t target_number_own = element.value4;
		uint32_t buff_id_own = element.value5;

		uint32_t add_pencent_miss = element.value6;
		uint32_t target_type_miss = element.value7;
		uint32_t target_number_miss = element.value8;
		uint32_t buff_id_miss = element.value9;
		uint32_t is_used_by_dead = element.value10;

		if ( is_used_by_dead == 0 && (owner->is_dead() || owner->is_in_control()) ) {
			return buff_notify_none;
		}

		bool valid = false;
		if (owner->m_buff_mgr.get_buff_by_small_type(small_type, 0) != NULL)
		{
			if (random_util_t::selectByTenTh(add_pencent_own))
			{
				fight_hero_ptr target = owner->get_cur_target();
				if (NULL == target)
				{
					target = owner;
				}

				std::vector<fight_hero_ptr> out;
				owner->get_target_by_buff_select_target(target, target_type_own, target_number_own, out);
				for (auto hero : out)
				{
					if (NULL == hero)
						continue;

					hero->m_buff_mgr.add_buff(buff_id_own, buff->get_attacker(), buff->get_skill());
					log_error("add_buff_to[%u] hero[%lu]", buff_id_own, hero->get_uid());
					valid = true;
				}
			}
		}
		else
		{
			if (random_util_t::selectByTenTh(add_pencent_miss))
			{
				fight_hero_ptr target = owner->get_cur_target();
				if (NULL == target)
					target = owner;

				std::vector<fight_hero_ptr> out;
				owner->get_target_by_buff_select_target(target, target_type_miss, target_number_miss, out);
				for (auto hero : out)
				{
					if (NULL == hero)
						continue;

					hero->m_buff_mgr.add_buff(buff_id_miss, buff->get_attacker(), buff->get_skill());
					log_error("add_buff_to[%u] hero[%lu]", buff_id_own, hero->get_uid());
					valid = true;
				}
			}
		}
		return valid ? buff_notify_valid : buff_notify_none;
	}
	default:	
		return buff_notify_none;
	}
	return buff_notify_none;
}


uint32_t buff_element_t::be_buff_beheaded(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		if (owner->is_dead())
		{
			break;
		}

		int32_t percent = element.value1;//参数万分比
		int32_t attack_percent_of_boss = element.value2;//如果为boss不触发斩杀，伤害数值为攻击力的万分比

		fight_hero_ptr attacker = owner->get_cur_attacker();
		if (!attacker)
		{
			log_error("get attacker error, NULL");
			return buff_notify_none;
		}

		int hp_percent = owner->get_attr(attrtype_enum::hp_present_ratio);
		if (hp_percent >= percent)//血量大于等于斩杀血量比例
		{
			return buff_notify_none;
		}

		if (owner->is_boss())//boss不会被击杀，添加斩杀伤害属性
		{
			//添加攻击者打boss斩杀额外伤害属性
			attacker->m_buff_element_mgr.inc_element_value(attrtype_enum::execute_boss_extra_damage, attack_percent_of_boss, 1);
		}
		else//非boss直接扣血
		{
			uint32_t result_hp = owner->get_max_hp() * 100;	//防止有护盾斩不死
			owner->sub_hp(result_hp, attacker);
			owner->add_combat_act(proto::common::combat_act_type_beheaded, result_hp, owner, attacker, owner->get_attr(attrtype_enum::hp), buff->get_id(), buff->get_tid());
		}
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

//吸收属性并且转换
uint32_t buff_element_t::suck_attr_and_turn(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		if (owner->is_dead())
			break;

		uint32_t target_type = element.value1;
		uint32_t max_count = element.value2;
		uint32_t turn_attr_id = element.value3;//增加的属性id
		uint32_t target_attr_id = element.value4;//吸取的属性id
		uint32_t percent = element.value5;
		uint32_t is_clear = element.value6;
		uint32_t max_per = element.value7;//吸取上限，不配置则吸取没有上限

		fight_hero_ptr attacker = owner;
		if (NULL == attacker) {
			log_error("attacker is null");
			return buff_notify_none;
		}

		fight_hero_ptr target = owner->get_cur_target();
		if (NULL == target)
		{
// 			log_error("target is null");
// 			return buff_notify_none;
			target = owner;//刘彻技能触发时机为部署前，此时是没有攻击目标的，其实应该取刘彻自己的友军全体，故这里取自己做target是对的。
		}

		uint32_t value = 0;
		std::vector<fight_hero_ptr> out;
		owner->get_target_by_buff_select_target(target, target_type, max_count, out);
		for (auto p_hero : out)
		{
			if (NULL == p_hero)
				continue;
				
			int32_t tmp_value = p_hero->get_attr(target_attr_id) * (percent / 10000.0f);
			if (is_clear == 2) 
			{
				//暂时不做减少，目前没有更好的方
				p_hero->sub_attr(target_attr_id, std::min<int32_t>(tmp_value, p_hero->get_attr(target_attr_id)));
				if (target_attr_id == attrtype_enum::shield_points)//吸收的属性为护盾时，删除护盾buff
				{
					//这里调用护盾处理逻辑，传入伤害0，气运盾被吸收后为0，会走清除气运盾buff的逻辑
					int32_t damage_temp = 0, absorb_temp = 0;
					p_hero->deal_with_shield(p_hero, damage_temp, absorb_temp, attrtype_enum::shield_points, buff_small_type_shield);
					log_info("shield is absorbed, left %d", p_hero->get_attr(target_attr_id));
				}
			}
			p_hero->add_combat_act(proto::common::combat_act_type_suck_attr, tmp_value, p_hero, attacker, p_hero->get_attr(target_attr_id));
			value += tmp_value;
		}
		
		if (max_per > 0)//如果该字段没有配置，则表明没有上限
		{
			uint32_t max_suck_value = (owner->get_attr_without_buff(turn_attr_id) * (max_per * 0.0001));
			uint32_t cur_suck_value = owner->get_suck_value(turn_attr_id);
			
			//吸满了
			if (cur_suck_value >= max_suck_value) {
				return buff_notify_none;
			}

			if (cur_suck_value + value > max_suck_value) {
				value = max_suck_value - cur_suck_value;
			}
		}
		
		//log_warn("fate value is %d", value);
		owner->add_suck_value(turn_attr_id, value);

		owner->m_buff_element_mgr.inc_element_value( turn_attr_id, value );
		owner->add_combat_act(proto::common::combat_act_type_turn_attr, value, attacker, attacker , owner->get_attr(turn_attr_id));
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}


uint32_t buff_element_t::attr_add_buff_id(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		if (owner->is_dead())
			break;

		uint32_t cond = element.value1;			//0不比较， 1大于，2小于
		uint32_t attr_id = element.value2;		//属性id
		uint32_t percent = element.value3;		//万分比
		uint32_t target_type = element.value4;	//目标类型
		uint32_t max_count = element.value5;	//最大数量
		uint32_t buff_id = element.value6;		//buff_id

		fight_hero_ptr p_target = owner->get_cur_target();
		if (NULL == p_target) {
			log_error("p_target is null");
			return buff_notify_none;
		}

		if (percent == 0) {
			log_error("buff:[%u] percent is null", percent);
			return buff_notify_none;
		}

		uint32_t own_value = owner ->get_attr(attr_id);
		uint32_t target_value = p_target->get_attr(attr_id);
		switch (cond) {
		case 1: {//大于
			if ( target_value > (own_value * (percent / 10000.0f)) ) {
				return buff_notify_none;
			}
		}
		break;
		case 2: {//小于
			if ( target_value < (own_value * (percent / 10000.0f)) ) {
				return buff_notify_none;
			}
		}
		break;
		}

		std::vector<fight_hero_ptr> out;
		owner->get_target_by_buff_select_target(p_target, target_type, max_count, out);
		for (auto hero : out)
		{
			if (NULL == hero) 
				continue;

			hero->m_buff_mgr.add_buff(buff_id, buff->get_attacker(), buff->get_skill());
		}
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::add_by_spped(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		uint32_t condition = element.value1;
		uint32_t percent = element.value2;
		uint32_t buff_id = element.value3;
		uint32_t buff_target = element.value4;
		uint32_t buff_num = element.value5;
		
		uint32_t max_att_speed = owner->get_combat()->get_max_att_speed();
		if (max_att_speed == 0) {
			log_error("max_att_speed is 0");
			return buff_notify_none;
		}

		std::vector<fight_hero_ptr> out;
		if (condition == 0)
		{
			if (percent > (owner->get_order_att_speed() * 10000.0f) / max_att_speed) {
				owner->get_target_by_buff_select_target(owner, buff_target, buff_num, out);
			}
		}
		else
		{
			if (percent < (owner->get_order_att_speed() * 10000.0f) / max_att_speed) {
				owner->get_target_by_buff_select_target(owner, buff_target, buff_num, out);
			}
		}

		for (auto hero : out)
		{
			if (NULL == hero)
				continue;

			hero->m_buff_mgr.add_buff(buff_id, buff->get_attacker(), buff->get_skill());
		}

		return buff_notify_valid;
	}
	break;
	case buff_step_unload:
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::attr_add_coeff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		if (owner->is_dead())
			break;

		uint32_t target_type = element.value1;//选取目标，1 buff持有者，2 buff持有者攻击目标
		uint32_t attr_id = element.value2;
		uint32_t percent = element.value3;
		uint32_t count_type = element.value4;//参与计算类型
		uint32_t calc_type = element.value5;//计算类型

		//target_type 1 自己 2：目标
		int32_t attr_value_selected = 0;
		switch (target_type) 
		{
			case 1: 
			{
				attr_value_selected = owner->get_attr(attr_id);
				break;
			}
			case 2: 
			{
				fight_hero_ptr target = owner->get_cur_target();
				if (NULL == target) 
				{
					log_error("target is null");
					return buff_notify_none;
				}
				attr_value_selected = target->get_attr(attr_id);
				break;
			}
			default:
			{
				log_error("target_type value error");
				return buff_notify_none;
			}
		}

		int value = 0;
		switch (calc_type)
		{
			case 0:
			{
				value = attr_value_selected * (percent / 10000.0f);
				break;
			}
			case 1:
			{
				fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
				if (!attacker)
				{
					log_error("buff attacker is null");
					return buff_notify_none;
				}
				if (attr_value_selected == 0)
				{
					log_error("attr_value_selected is 0");
					return buff_notify_none;
				}
				value = attacker->get_attr(attr_id) / attr_value_selected * (percent / 10000.0f);
				break;
			}
			default:
			{
				log_error("calc_type value error");
				return buff_notify_none;
			}
		}

		//count_type 0:技能系数 1攻击力, 2附加攻击
		switch (count_type) 
		{
			case 0:	
			{
				owner->m_buff_element_mgr.inc_element_value(attrtype_enum::extra_skill_ratio, value);
				break;
			}
			case 1:
			{
				owner->m_buff_element_mgr.inc_element_value(attrtype_enum::atk, value);
				break;
			}
			case 2:	
			{
				owner->m_buff_element_mgr.inc_element_value(attrtype_enum::additive_damage, value);
				break;
			}
			default:
			{
				log_error("count_type value error");
				return buff_notify_none;
			}
		}
		buff->add_trigger_attr(attr_id, value);
		return buff_notify_valid;
	}
	break;
	case buff_step_unload: 
	{
		uint32_t attr_id = element.value2;
		uint32_t count_type = element.value4;

		int32_t value = buff->get_trigger_attr(attr_id);
		//count_type 0:技能系数 1攻击力, 2附加攻击
		switch (count_type) 
		{
			case 0:
			{
				owner->m_buff_element_mgr.dec_element_value(attrtype_enum::extra_skill_ratio, value);
				break;
			}
			case 1:
			{
				owner->m_buff_element_mgr.dec_element_value(attrtype_enum::atk, value);
				break;
			}
			case 2:
			{
				owner->m_buff_element_mgr.dec_element_value(attrtype_enum::additive_damage, value);
				break;
			}
			default:
			{
				log_error("count_type value error");
				return buff_notify_none;
			}
		}
		break;
	}
	default:
		break;
	}
	return buff_notify_none;
}

//转换buff，
//根据指定buff 转另一个buff
uint32_t buff_element_t::turn_other_buff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		int32_t check_buff_id = element.value1;
		uint32_t target_type = element.value2;
		uint32_t target_num = element.value3;
		uint32_t target_buff_id = element.value4;
		uint32_t floor_limit = element.value5;
		uint32_t layer_or_count = element.value6;

		uint32_t count = 0;

		if (layer_or_count == 0)
		{
			if (check_buff_id > 0)
				//layer = owner->get_buff_manager().get_buff_layer_by_tid(check_buff_id, 0);
				count = owner->get_buff_manager().get_buff_layer_by_tid(check_buff_id, 0);
			else
				count = owner->m_buff_mgr.get_all_count_by_effect_type(check_buff_id);
		}
		else
		{
			if (check_buff_id > 0)
				count = owner->get_buff_manager().get_buff_count_by_tid(check_buff_id, 0);
			//else
			//	count = owner->m_buff_mgr.get_all_count_by_effect_type(check_buff_id);
		}
		
		
		if (count == 0)
			return buff_notify_none;

		if(count < floor_limit)
			return buff_notify_none;

		//特殊写法,91
		if (target_num == 91) {
			target_num = owner->get_buff_manager().get_buff_layer_by_tid(check_buff_id, 0);
		}

		std::vector<fight_hero_ptr> out;
		owner->get_target_by_buff_select_target(owner, target_type, target_num, out);
		for (auto hero : out)
		{
			if (NULL == hero)
				continue;

			hero->m_buff_mgr.add_buff(target_buff_id, owner->get_uid(), buff->get_skill(), count);
		}
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::alomst_die(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		owner->athanasia_count(0);
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		uint32_t target_type = element.value1;
		uint32_t add_buff_id = element.value2;
		uint32_t hp_rate = element.value3;
		uint32_t sub_attr_id = element.value4;
		uint32_t sub_rate = element.value5;

		const fight_hero_ptr target = p_event_owner;
		if (NULL == target)
		{
			log_error("target is null");
			return buff_notify_none;
		}

		if( NULL == p_event_owner ){
			return buff_notify_none;
		}

		const fight_hero_ptr attacker = p_event_owner->get_cur_attacker();
		if (NULL == attacker) {
			log_error("attacker is null");
			return buff_notify_none;
		}

		//1是自己， 其他是队友
		if (target_type == 1) {
			if ( target->get_uid() != owner->get_uid() )
				return buff_notify_none;
		}
		else {
			// 判断是否队友
			if (target->get_camp() != owner->get_camp())
				return buff_notify_none;
		}
		
		if( target->get_attr(attrtype_enum::invincibility) )
			return buff_notify_none;

		//容错有些直接打死的英雄处理问题(不触发)
		if (target->is_dead() || target->get_hp() == 0) {
			return buff_notify_none;
		}

		// 判断本次伤害是否大于当前HP
		int damage = attacker->get_fight_calc_result().damage;
		if ( damage < (int)target->get_hp())
			return buff_notify_none;

		// 判断owner血量是否足够
		if ( hp_rate > 0 && (owner->get_max_hp() * hp_rate/10000.0f) > owner->get_hp()  )
			return buff_notify_none;

		//扣除指定的数值
		if (sub_attr_id > 0) {
			uint32_t tmp_id = sub_attr_id;
			uint32_t max_value = 0;
			if (sub_attr_id == common::attrtype_enum::hp) {
				tmp_id = common::attrtype_enum::max_hp;
				max_value = target->get_max_hp();
			}

			uint32_t value = owner->get_attr(tmp_id) * (sub_rate / 10000.0f);
			owner->sub_attr(sub_attr_id, value);
			owner->add_combat_act(proto::common::combat_act_type_protect_almost_dead, value, owner, target, owner->get_attr( sub_attr_id ), damage, buff->get_tid());
			
			//防止属性溢出
			if ( max_value != 0 && value > max_value) {
				value = max_value;
			}
			target->get_count_data().athanasia_heal_hp = value;
		}

		// 增加本次死亡守护效果
		target->m_buff_element_moment_mgr.inc_element_value(attrtype_enum::athanasia, 1, 0);

		// 触发一次BUFF
		if (add_buff_id > 0) {
			target->m_buff_mgr.add_buff(add_buff_id, buff->get_attacker(), buff->get_skill());
		}

		owner->add_combat_act(proto::common::combat_act_type_death_guard, 1, target, owner, 0,0, buff->get_tid() );

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}


uint32_t buff_element_t::cd_attack_back(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		owner->athanasia_count(0);
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		uint32_t skill_id = element.value1;
		uint32_t is_cd = element.value2;
		uint32_t buff_id = element.value3;
	
		fight_hero_ptr attacker = owner->get_combat()->get_att_fight_hero();
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}
		
		uint32_t  skill_cd = owner->get_skill_cd(skill_id);
		do 
		{
			if (is_cd == 0 && skill_cd == 0  ){
				break;
			}
			else if (is_cd > 0 && skill_cd > 0) {
				break;
			}else{
				return buff_notify_none;
			}
		} while (0);

		owner->m_buff_mgr.add_buff(buff_id, owner->get_uid(), buff->get_skill());
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::shared_hurts(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		uint32_t shared_hurt_per = element.value1;
		uint32_t reduce_shared_hurt_per = element.value2;
			
		owner->m_buff_element_mgr.inc_element_value(attrtype_enum::shared_hurt_per, shared_hurt_per);
		owner->m_buff_element_mgr.inc_element_value(attrtype_enum::reduce_shared_hurt_per, reduce_shared_hurt_per);
		owner->m_buff_element_mgr.inc_element_value(attrtype_enum::shared_buff_id, buff->get_id());

		return buff_notify_valid;
	}
	break;
	default:
	{
		owner->m_buff_element_mgr.dec_element_value(attrtype_enum::shared_hurt_per, 0);
		owner->m_buff_element_mgr.dec_element_value(attrtype_enum::reduce_shared_hurt_per, 0);
		owner->m_buff_element_mgr.dec_element_value(attrtype_enum::shared_buff_id, 0);
	}
		break;
	}
	return buff_notify_none;
}


uint32_t buff_element_t::suck_blood(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		uint32_t target_type = element.value1;
		uint32_t target_attr_id = element.value2;
		uint32_t suck_per       = element.value3;
		uint32_t max_per = element.value4;
			
		fight_hero_ptr target = owner->get_cur_target();
		if( NULL == target ){
			log_error("target is NULL");
			return buff_notify_none;
		}

		int32_t suck_value = 0;
		if( target_type == 0 ){
			//自己属性	
			suck_value = owner->get_attr(target_attr_id) * (suck_per / 10000.0f);
		}
		else {
			//目标属性
			suck_value = target->get_attr(target_attr_id) * (suck_per / 10000.0f);
		}

		if (suck_value == 0) {
			return buff_notify_none;
		}
		
		uint32_t max_value = (max_per/10000.0f) * owner->get_attr(attrtype_enum::atk);
		if (max_value > 0 && (uint32_t)suck_value > max_value) {
			suck_value = max_value;
		}
		
		int32_t absorb = 0;
		fight_hero_t::do_target_shield_points(target, suck_value, absorb);
		target->sub_hp(suck_value, owner, buff->get_skill());
		owner->add_hp(suck_value);
		owner->add_combat_act(proto::common::combat_act_type_buff_damage, suck_value, target, owner, target->get_hp(), buff->get_id(), buff->get_tid(), absorb);
		owner->add_combat_act(proto::common::combat_act_type_leech, suck_value, owner, target, owner->get_hp(), buff->get_id(), buff->get_tid(), absorb);
		
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::attack_by_buff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		uint32_t skill_id = element.value1;
		uint32_t rate = element.value2;
		if (!random_util_t::selectByTenTh(rate)) {
			return buff_notify_none;
		}
			
		fight_hero_ptr buff_attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == buff_attacker) {
			return buff_notify_none;
		}

		fight_hero_ptr attacker = owner->get_cur_attacker();
		if (NULL == attacker) {
			return buff_notify_none;
		}

		if (buff_attacker->is_dead() || buff_attacker->is_in_control()) {
			return buff_notify_none;
		}

		// 如果是自己buff不触发连击
		if( attacker == buff_attacker ){
			return buff_notify_none;
		}
		SkillEffectTable* skill_effect_conf = attacker->get_skill_effect(skill_id);
		if (NULL == skill_effect_conf) {
			log_error("skill_effect_conf[%u] is null", skill_id);
			return buff_notify_none;
		}

		if (buff_attacker->get_count_data().buff_combo > 1 ) {
			return buff_notify_none;
		}
		++buff_attacker->get_count_data().buff_combo;

		buff_attacker->trigger_skill_by_buff(skill_effect_conf, owner, proto::common::combat_act_type_trigger_skill);
		return buff_notify_valid;
	}
	break;
	default: 
		break;
	}
	return buff_notify_none;
}


uint32_t buff_element_t::special_buff_change_att(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	BuffTable *conf = buff->get_conf();
	if (NULL == conf)
	{
		log_error("buff_element_t conf NULL");
		return buff_notify_none;
	}
	fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
	if (NULL == attacker)
	{
		log_error("buff_element_t attacker is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
	{
		//97:属性id:属性值：是否百分比形式加入:技能id
		uint32_t attr_id = element.value1;
		int value = element.value2;
		uint32_t chg_type = element.value3;
	
		if (chg_type == 0) {
			owner->m_buff_element_mgr.set_element_value(attr_id, value);
		}
		else {
			owner->m_buff_element_mgr.set_element_value_percent(attr_id, value);
		}
		owner->add_combat_act(proto::common::combat_act_type_attr_inc, std::abs(value), owner, attacker, attr_id, owner->get_attr(attr_id), buff->get_tid());

		FIGHT_LOG("[%lu]buff_step_load camp[%u] pos[%u] hero_tid[%u] buff[%u] type[%u] value[%d] layer[%u]",
			owner->get_fight_uid(), owner->get_camp(), owner->get_pos(), owner->get_tid(), buff->get_id(), attr_id, value, buff->get_layer());

		log_error("[%lu]buff_step_load camp[%u] pos[%u] hero_tid[%u] buff[%u] type[%u] value[%d] layer[%u]",
			owner->get_fight_uid(), owner->get_camp(), owner->get_pos(), owner->get_tid(), buff->get_id(), attr_id, value, buff->get_layer());
		return element_notify_type(attr_id);
	}
	break;
	case buff_step_unload:
	{
		uint32_t attr_id = element.value1;
		int value = element.value2;
		uint32_t chg_type = element.value3;

		//如果是unload直接把这个数值设置为0
		value = 0;	
		if (chg_type == 0) {
			owner->m_buff_element_mgr.set_element_value(attr_id, value);
		}
		else {
			owner->m_buff_element_mgr.set_element_value_percent(attr_id, value);
		}
		owner->add_combat_act(proto::common::combat_act_type_attr_dec, std::abs(value), owner, attacker, attr_id, owner->get_attr(attr_id), buff->get_tid());

		FIGHT_LOG("[%lu]buff_step_unload camp[%u] pos[%u] hero_tid[%u] buff[%u] type[%u] value[%d] layer[%u]",
			owner->get_fight_uid(), owner->get_camp(), owner->get_pos(), owner->get_tid(), buff->get_id(), attr_id, value, buff->get_layer());

		log_error("[%lu]buff_step_unload camp[%u] pos[%u] hero_tid[%u] buff[%u] type[%u] value[%d] layer[%u]",
			owner->get_fight_uid(), owner->get_camp(), owner->get_pos(), owner->get_tid(), buff->get_id(), attr_id, value, buff->get_layer());
		return element_notify_type(attr_id);
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::buff_layer_change_att(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num ) 
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	BuffTable *conf = buff->get_conf();
	if (NULL == conf)
	{
		log_error("buff_element_t conf NULL");
		return buff_notify_none;
	}
	fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
	if (NULL == attacker)
	{
		log_error("buff_element_t attacker is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
	{
		//98:buff类型:计算属性id:配置系数:计算类型:目标属性id:转化属性的受益者:目标数量
		int buff_type = element.value1;//-1减益，0增益，大于0为buff小类型
		uint32_t src_attr_id = element.value2;//计算属性ID
		uint32_t ratio = element.value3;//配置系数
		uint32_t calc_type = element.value4;//计算类型
		uint32_t dest_attr_id = element.value5;//目标属性ID
		uint32_t dest_attr_hero_type = element.value6;//转化属性受益者
		uint32_t target_num = element.value7;//目标数量
		uint32_t src_attr_hero_type = element.value8;//计算目标，0 自己，1 当前攻击目标
		uint32_t buff_layer_hero_type = element.value9;//buff层数计算目标，0 自己，1 当前攻击目标


		fight_hero_ptr p_target = owner->get_cur_target();
		fight_hero_ptr p_src_attr_hero = owner;
		//设置属性计算目标
		if (src_attr_hero_type == 1) {
			if (p_target == NULL) {
				log_error("p_target is NULL");
				return buff_notify_none;
			}
			p_src_attr_hero = p_target;
		}

		//设置层数计算目标
		fight_hero_ptr p_buff_layer_hero = owner;	
		if (buff_layer_hero_type == 1) {
			if (NULL == p_target) {
				log_error("p_target is NULL");
				return buff_notify_none;
			}
			p_buff_layer_hero = p_target;
		}

		uint32_t layer = 0;
		if(buff_type == -1)//减益
		{
			layer = p_buff_layer_hero->m_buff_mgr.get_buff_layer_by_effect_type(buff_effect_type_debuff);
		}
		else if(buff_type == 0)//增益
		{
			layer = p_buff_layer_hero->m_buff_mgr.get_buff_layer_by_effect_type(buff_effect_type_gain);
		}
		else if(buff_type > 0)//buff小类型
		{
			layer = p_buff_layer_hero->m_buff_mgr.get_buff_layer_by_smalltype(buff_type);
		}

		uint32_t value = 0;
		switch (calc_type) 
		{
		case 1:	//普通计算
			value = p_src_attr_hero->get_attr(src_attr_id) * layer * (ratio / 10000.0f);
			break;
		case 2: //层数的平方
			value = p_src_attr_hero->get_attr(src_attr_id) * layer * layer * (ratio / 10000.0f);
			break;
		case 3: //层数的平方+1 诸葛亮
			value = p_src_attr_hero->get_attr(src_attr_id) * layer * (layer+1) * (ratio / 10000.0f);
			break;
		default:
			return buff_notify_none;
		}

		buff->add_trigger_attr(dest_attr_id, value);

		//选择属性获益目标
		std::vector<fight_hero_ptr> out;
		owner->get_target_by_buff_select_target(owner, dest_attr_hero_type, target_num, out);
		for (auto p_hero : out)
		{
			if (NULL == p_hero) {
				log_error("find p_hero is NULL , target_type : %u", dest_attr_hero_type);
				continue;
			}
			p_hero->m_buff_element_mgr.inc_element_value(dest_attr_id, value,0);
			p_hero->add_combat_act(proto::common::combat_act_type_attr_inc, std::abs(value), owner, owner, dest_attr_id, owner->get_attr(dest_attr_id), buff->get_tid());
		}
		return buff_notify_valid;
	}
	break;
	case buff_step_unload:
	{
		uint32_t tag_att_id = element.value5;
		uint32_t target_type = element.value6;
		uint32_t target_num = element.value7;

		uint32_t value = buff->get_trigger_attr(tag_att_id);
		
		std::vector<fight_hero_ptr> out;
		owner->get_target_by_buff_select_target(owner, target_type, target_num, out);
		for (auto p_hero : out)
		{
			if (NULL == p_hero) {
				log_error("find p_hero is NULL , target_type : %u", target_type);
				continue;
			}
			p_hero->m_buff_element_mgr.dec_element_value(tag_att_id, value);
			p_hero->add_combat_act(proto::common::combat_act_type_attr_dec, std::abs(value), owner, owner, tag_att_id, owner->get_attr(tag_att_id), buff->get_tid());
		}
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::xiangyu_skill2(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	BuffTable *conf = buff->get_conf();
	if (NULL == conf)
	{
		log_error("buff_element_t conf NULL");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		uint32_t limit_rate = element.value1;
		uint32_t buff_id = element.value2;
		//uint32_t buff_round = element.value3;

		combat_ptr p_combat = owner->get_combat();
		if (NULL == p_combat) {
			log_warn("combat is NULL");
			return buff_notify_none;
		}

		fight_hero_ptr p_attack = p_combat->find_hero( buff->get_attacker() );
		if (NULL == p_attack) {
			log_warn("p_attack is NULL");
			return buff_notify_none;
		}

		if (limit_rate >= (uint32_t)owner->get_attr(attrtype_enum::hp_present_ratio)) {
			return buff_notify_none;
		}
			
		int32_t turn_value = owner->get_hp() * (limit_rate / 10000.0f);
		/*项羽盾 不走护盾计算
		int32_t absorb = 0;
		fight_hero_t::do_target_shield_points(owner, turn_value, absorb);
		*/
		owner->sub_hp( turn_value, owner, buff->get_skill() );
		owner->add_combat_act(proto::common::combat_act_type_buff_damage, turn_value, owner, p_attack, owner->get_hp(), buff->get_id(), buff->get_tid());
		
		owner->set_attr( attrtype_enum::xiangyu_shield_points , turn_value);
		owner->add_combat_act(proto::common::combat_act_type_attr_inc, std::abs(turn_value), owner, owner, attrtype_enum::xiangyu_shield_points, owner->get_attr(attrtype_enum::xiangyu_shield_points), buff->get_tid());
		owner->m_buff_mgr.add_buff(buff_id, owner->get_uid() , buff->get_skill(), 1);

		return buff_notify_valid;
	}
	break;
	case buff_step_unload:
	{
		//owner->m_buff_element_mgr.clear_element_value( uint32_t(attrtype_enum::xiangyu_shiled_points) );

		//owner->add_combat_act(proto::common::combat_act_type_attr_dec, std::abs(0), owner, owner, attrtype_enum::xiangyu_shiled_points, owner->get_attr(attrtype_enum::xiangyu_shiled_points), buff->get_tid());

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::xiangyu_skill3(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num) {
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	BuffTable *conf = buff->get_conf();
	if (NULL == conf)
	{
		log_error("buff_element_t conf NULL");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		uint32_t turn_percent = element.value1;
		uint32_t plugin_id = element.value2;

		const fight_hero_ptr attacker = owner->get_cur_attacker();
		if (NULL == attacker) {
			log_error("attacker is null");
			return buff_notify_none;
		}
		
		combat_ptr p_combat = owner->get_combat();
		if (NULL == p_combat) {
			return buff_notify_none;
		}
		
		// 本次伤害 
		int damage = attacker->get_fight_calc_result().damage;

		//无敌状态下不用转
		if ( damage == 0 || owner->get_attr(attrtype_enum::invincibility))
			return buff_notify_none;


		int32_t share_damage = damage * (turn_percent / 10000.0f);

		std::vector<fight_hero_ptr> out;
		std::vector< fight_hero_ptr > normal_team_list;
		std::vector< fight_hero_ptr > special_team_list;
	
		p_combat->get_camp_heros( out, owner->get_camp(), owner->get_pos() );
		std::random_shuffle(out.begin(), out.end());

		fight_hero_ptr  p_hero = NULL;
		for (auto iter : out) {
			p_hero = iter;
			if ( p_hero == NULL || p_hero->is_dead() || p_hero->get_attr(attrtype_enum::soul_condition) > 0 ) {
				continue;
			}

			if ( (p_hero->get_plugin() / 100) == plugin_id ){
				special_team_list.push_back( p_hero );
			}
			else {
				normal_team_list.push_back(p_hero);
			}
		}

		//优先虞姬列表
		fight_hero_ptr p_target = NULL;
		if (special_team_list.size() > 0) {
			p_target = special_team_list[0];
		}else if (normal_team_list.size() > 0) {
			p_target = normal_team_list[0];
		}

		//没有目标或者无法承担
		if (NULL == p_target || (uint32_t)share_damage > p_target->get_hp()) {
			//owner->m_buff_mgr.add_buff(buff_id, owner->get_uid(), buff->get_skill());
			owner->do_team_script_event(event_on_transfer_damage_fail, owner);
			return buff_notify_valid;
		}
		
		owner->add_transfer_hero(p_target->get_uid(), share_damage);
		
		int32_t absorb = 0;
		fight_hero_t::do_target_shield_points(p_target, share_damage, absorb);
		p_target->sub_hp(share_damage, attacker,buff->get_skill());
		p_target->add_combat_act(proto::common::combat_act_type_buff_damage, share_damage, p_target, owner, p_target->get_hp(), buff->get_id(), buff->get_tid(), absorb);
		return buff_notify_valid;
	}
	break;
	case buff_step_unload:
	{
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::move_after_think(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num) {
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	BuffTable *conf = buff->get_conf();
	if (NULL == conf)
	{
		log_error("buff_element_t conf NULL");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		uint32_t base_rate = element.value1;
		uint32_t level_rate = element.value2;
		base_rate += (buff->get_skill_level() * level_rate);
		if (!random_util_t::selectByTenTh(base_rate)) {
			return buff_notify_none;
		}

		if( event_num != event_on_lsm_run_buff )
			owner->do_team_script_event(event_on_move_after_think, owner);
	}
	case buff_step_detonate: {
		uint32_t turn_buff_id = element.value3;
		uint32_t target_buff_id = element.value4;
	
		uint32_t layer = owner->m_buff_mgr.get_buff_layer_by_tid(turn_buff_id);
		if (layer == 0) {
			return buff_notify_none;
		}
		
		owner->m_buff_mgr.sub_buff_layer_or_remove(turn_buff_id, owner->get_uid());
		owner->m_buff_mgr.add_buff( target_buff_id, buff->get_attacker(), buff->get_skill() );
		return buff_notify_valid;
	}
	break;
	case buff_step_unload:
	{
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}


uint32_t buff_element_t::move_with_think(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num) {
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	BuffTable *conf = buff->get_conf();
	if (NULL == conf)
	{
		log_error("buff_element_t conf NULL");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		uint32_t rate = element.value1;
		if (!random_util_t::selectByTenTh(rate)) {
			return buff_notify_none;
		}

		if( NULL == p_event_owner ){
			return buff_notify_none;
		}

		//几率给队友触发		
		p_event_owner->do_team_script_event(event_on_lsm_run_buff, p_event_owner);

		return buff_notify_valid;
	}
	break;
	case buff_step_unload:
	{
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::lsm_gift(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num) {
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	BuffTable *conf = buff->get_conf();
	if (NULL == conf)
	{
		log_error("buff_element_t conf NULL");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load: // 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		uint32_t turn_buff_id = element.value1;
		uint32_t target_buff_id = element.value2;
		uint32_t first_small_buff_type = element.value3;
		uint32_t buff_id = element.value4;

		uint32_t layer = owner->m_buff_mgr.get_buff_layer_by_tid(turn_buff_id);
		if (layer == 0) {
			return buff_notify_none;
		}

		combat_ptr p_combat = owner->get_combat();
		if (NULL == p_combat) {
			return buff_notify_none;
		}

		owner->m_buff_mgr.sub_buff_layer_or_remove(turn_buff_id, owner->get_uid());
		owner->m_buff_mgr.add_buff(target_buff_id, buff->get_attacker(), buff->get_skill());

		std::vector<fight_hero_ptr> out;
		p_combat->get_camp_heros(out, owner->get_camp(), owner->get_pos());
		std::random_shuffle(out.begin(), out.end());

		fight_hero_ptr  p_hero = NULL;
		for (auto iter : out) {
			p_hero = iter;
			if (p_hero == NULL || p_hero->is_dead() || p_hero->get_attr(attrtype_enum::soul_condition) > 0) {
				continue;
			}
		
			if (p_hero->m_buff_mgr.get_buff_layer_by_smalltype(first_small_buff_type) > 0) {
				p_hero->do_team_script_event(event_on_lsm_run_buff, p_hero);
				break;
			}
		}
		
		if ( NULL == p_hero ) {
			return buff_notify_none;
		}

		p_hero->m_buff_mgr.add_buff( buff_id, owner->get_uid(), buff->get_skill() );
		return buff_notify_valid;
	}
	break;
	case buff_step_unload:
	{
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}


uint32_t buff_element_t::add_buff_by_buff_layer(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num) {
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	BuffTable *conf = buff->get_conf();
	if (NULL == conf)
	{
		log_error("buff_element_t conf NULL");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load: // 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		uint32_t buff_id = element.value1;
		uint32_t check_layer = element.value2;
		uint32_t add_buff_id = element.value3;
		uint32_t target_type = element.value4;
		uint32_t target_num = element.value5;

		uint32_t layer = owner->m_buff_mgr.get_buff_layer( buff_id );
		if ( check_layer > layer ) {
			return buff_notify_none;
		}
			
		std::vector<fight_hero_ptr> out;
		owner->get_target_by_buff_select_target(owner, target_type, target_num, out);
		for (auto p_hero : out) {
			if (NULL == p_hero) 
				continue;

			p_hero->m_buff_mgr.add_buff(add_buff_id, owner->get_uid(), buff->get_skill());
		}
		return buff_notify_valid;
	}
	break;
	case buff_step_unload:
	{
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}


uint32_t buff_element_t::clear_buff_add_attr(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num) 
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}

	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}

	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	BuffTable *conf = buff->get_conf();
	if (NULL == conf)
	{
		log_error("buff_element_t conf NULL");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
	case buff_step_trigger:
	{
		return buff_notify_valid;
	}
	break;
	case buff_step_unload:
	{
		uint32_t buff_attr_id = element.value1;

		//项羽盾比较特殊
		if (buff_attr_id == attrtype_enum::xiangyu_shield_points) {
			owner->set_attr(buff_attr_id, 0);
		}
		else {
			owner->m_buff_element_mgr.clear_element_value( buff_attr_id );
		}
		owner->add_combat_act(proto::common::combat_act_type_attr_dec, std::abs(0), owner, owner, buff_attr_id , owner->get_attr(buff_attr_id), buff->get_tid());

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::miss_by_beattack_num(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num)
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}

	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}

	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	BuffTable *conf = buff->get_conf();
	if (NULL == conf)
	{
		log_error("buff_element_t conf NULL");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
	case buff_step_trigger:
	{
		//根据受攻击次数增加闪避
		uint32_t cd		     = element.value1;
		uint32_t miss_num    = element.value2;
		uint32_t attack_type = element.value2;
	
		if (buff->get_cd() != 0 ) {
			return buff_notify_none;
		}
		
		uint32_t left_num = 0;
		switch (attack_type) {
		case 0:
			left_num = owner->get_count_data().equip_miss_all_atk_count;
			break;
		case 1:
			left_num = owner->get_count_data().equip_miss_single_atk_count;
			break;
		case 2:
			left_num = owner->get_count_data().equip_miss_mutilp_atk_count;
			break;
		}

		//只有闪避后才会计数cd
		if (left_num == miss_num) {
			return buff_notify_none;
		}

		buff->set_cd(cd);
		
		switch (attack_type) {
		case 0:
			owner->get_count_data().equip_miss_all_atk_count = miss_num;
			break;
		case 1:
			owner->get_count_data().equip_miss_single_atk_count = miss_num;
			break;
		case 2:
			owner->get_count_data().equip_miss_mutilp_atk_count = miss_num;
			break;
		}
		return buff_notify_valid;
	}
	break;
	case buff_step_unload:
	{
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::add_att_speed_by_hp_sub(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num)
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		uint32_t speed_inc_by_hp_lose_1_percent = element.value1;//血量每少1%增加的行动条
		bool is_infuenced_by_change_speed = (bool)element.value2;//是否受拉条影响

		int max_hp = owner->get_attr(attrtype_enum::max_hp);
		if (max_hp == 0)
		{
			log_error("add_att_speed_by_hp_sub max_hp equals 0");
			return  buff_notify_none;
		}
		fight_hero_ptr attacker = owner->get_cur_attacker();
		if (!attacker)
		{
			log_error("add_att_speed_by_hp_sub attacker null!");
			return  buff_notify_none;
		}
		float hp_lose = attacker->get_fight_calc_result().damage / (float)max_hp;
		int att_speed_add = owner->get_combat()->get_max_att_speed() * hp_lose * 100 * speed_inc_by_hp_lose_1_percent * 0.0001f;
		owner->set_att_speed(owner->get_att_speed() + att_speed_add);

		//是否受拉条检测和影响
		if (is_infuenced_by_change_speed)
		{
			owner->do_team_script_event(event_on_add_att_speed, owner);
		}

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}


uint32_t buff_element_t::suck_and_bomb(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num) 
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		if ( NULL == p_event_owner )  {
			return buff_notify_none;
		}

		//收集伤害
		uint32_t record_rate = element.value1;

		uint32_t damage = p_event_owner->get_count_data().round_sub_hp * record_rate * 0.0001f;

		buff->add_trigger_attr(attrtype_enum::damage_absorb_and_boom, damage);

		//死亡的时候炸
		if (event_num == event_on_death) {
			uint32_t total_damange = buff->get_trigger_attr(attrtype_enum::damage_absorb_and_boom);
			std::vector<fight_hero_ptr> out;
			combat_ptr p_combat = p_event_owner->get_combat();
			if (!p_combat)
			{
				log_error("event owner get combat ptr null");
				return buff_notify_none;
			}
			p_combat->get_camp_heros(out, p_event_owner->get_enemy_camp());
			uint32_t enemy_size = out.size();
			if (enemy_size == 0)
			{
				log_error("enemy_size is 0");
				return buff_notify_none;
			}
			int32_t share_damage = (total_damange / (float)enemy_size);

			for (auto p_hero : out) {
				if (NULL == p_hero || p_hero->is_dead() ) {
					continue;		
				}

				int32_t absorb = 0;
				fight_hero_t::do_target_shield_points(p_hero, share_damage, absorb);
				p_hero->sub_hp(share_damage, p_event_owner, buff->get_skill());
				p_hero->add_combat_act(proto::common::combat_act_type_buff_damage, share_damage, p_hero, p_event_owner, p_hero->get_hp(), buff->get_id(), buff->get_tid(), absorb);
			}
		}
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::record_hurt(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num)
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		//记录受到的伤害
		owner->add_round_enemy_hurt();

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::add_buff_of_hurt_count(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num)
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	//该buff只能添加给技能释放目标，否则owner错误
	switch (buff->get_step())
	{
	case buff_step_load:
	{
		uint32_t buff_id = element.value1;//添加的buff ID
		owner->m_buff_mgr.add_buff(buff_id, buff->get_attacker(), buff->get_skill());//该buff加给技能攻击目标
	}
	break;
	case buff_step_trigger:
	{
		uint32_t buff_id = element.value1;//添加的buff ID
		//uint32_t disappear_type = element.value2;//buff移除类型：1 buff释放者被攻击次数，暂时只有一种，以后再扩展
		uint32_t remove_count = element.value3;//受多少次攻击清除该buff

		buff_ptr p_buff = owner->m_buff_mgr.get_buff_by_tid(buff_id, p_event_owner->get_uid());//p_event_owner为buff释放者
		if(!p_buff)
		{
			return buff_notify_none;
		}

		//计数
		p_buff->add_trigger_attr(attrtype_enum::buff_attacker_be_hurt_count, 1);

		//删除
		uint32_t be_hurt_count = p_buff->get_trigger_attr(attrtype_enum::buff_attacker_be_hurt_count);
		if (be_hurt_count >= remove_count)//达到目标计数则删除该buff
		{
			owner->m_buff_mgr.remove_buff_by_tid(buff_id, buff->get_attacker());
		}

		//向客户端发送剩余计数
		fight_hero_ptr p_attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (p_attacker)
		{
			//客户端要求：参数3 buff拥有者，参数4 buff释放者
			owner->add_combat_act(proto::common::combat_act_type_count_left_of_buff, SAFE_SUB(remove_count, be_hurt_count), owner, p_event_owner, p_buff->get_id(), p_buff->get_tid());
		}
		else
		{
			log_error("get attacker null");
		}
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}


uint32_t buff_element_t::limit_max_speed(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num)
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("buff_element_t owner NULL");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		uint32_t type = element.value1;
		uint32_t value = element.value2;

		int32_t speed = value;
		if (type == 1) {
			fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
			if (NULL == attacker)
				return buff_notify_none;
			speed = attacker->get_attr_without_buff(attrtype_enum::speed) - 1;
		}
		else {
			speed = value;
		}

		//大于speed 第二速度要设置为对应的值,小于的话直接用原始值即可
		if (owner->get_attr_without_buff(attrtype_enum::speed) > speed) {
			owner->set_attr(attrtype_enum::speed_second, speed);
			log_error("load set speed_scond :%u", speed);
		}
		else {
			owner->set_attr( attrtype_enum::speed_second, owner->get_attr_without_buff(attrtype_enum::speed) );
			log_error("load set speed_scond :%u", owner->get_attr_without_buff(attrtype_enum::speed));
		}
		return buff_notify_valid;
	}
	break;
	case buff_step_unload: {
		log_error("unload set speed_scond 0");
		owner->set_attr(attrtype_enum::speed_second, 0);
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::addbuff_when_add_small_type(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange, const fight_hero_ptr p_event_owner, uint32_t event_num )
{
	if (NULL == buff)
	{
		log_error("buff_element_t buff NULL");
		return buff_notify_none;
	}
	if (NULL == owner)
	{
		log_error("owner is null");
		return buff_notify_none;
	}
	if (NULL == owner->get_combat())
	{
		log_error("owner get_combat is null");
		return buff_notify_none;
	}

	switch (buff->get_step())
	{
	case buff_step_load:
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		fight_hero_ptr attacker = p_event_owner;
		if (NULL == attacker) {
			log_error("attacker is null");
			return buff_notify_none;
		}

		uint32_t small_type = element.value1;		//buff小类型
		uint32_t add_buffer_id = element.value2;	//追加buff_id
		uint32_t first_target = element.value3;
		uint32_t second_target = element.value4;
		uint32_t target_num = element.value5;

		BuffTable* conf = GET_CONF(BuffTable, owner->get_fight_calc_result().add_buff_id);
		if (NULL == conf)
			return buff_notify_none;

		if ((uint32_t)conf->buff_small_type() != small_type) {
			return buff_notify_none;
		}

		fight_hero_ptr  target = owner;
		if (first_target == 1) {
			target = attacker;
		}
		target->m_buff_mgr.add_buff(add_buffer_id, buff->get_attacker(), buff->get_skill());

		//给备选目标加
		if (second_target != 0) {
			std::vector<fight_hero_ptr> out;
			owner->get_target_by_buff_select_target(owner, second_target, target_num, out);
			for (auto p_hero : out) {
				if (NULL == p_hero) {
					continue;
				}
				if (p_hero->get_uid() == target->get_uid()) {
					continue;
				}
				p_hero->m_buff_mgr.add_buff(add_buffer_id, buff->get_attacker(), buff->get_skill());
			}
		}
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

