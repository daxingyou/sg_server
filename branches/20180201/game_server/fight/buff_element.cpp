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
	
	return true;
}

uint32_t buff_element_t::element_notify_type(uint32_t type)
{
	return buff_notify_attr_change;
}

uint32_t buff_element_t::add_buff_by_small_type(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

		if (target == 1)
		{
			if (owner->m_buff_mgr.get_count_by_small_type( small_type ) > 0)
				owner->m_buff_mgr.add_buff(bufferid, buff->get_attacker(), buff->get_skill());
			else
				return buff_notify_none;
		}
		else
		{
			if (attacker->m_buff_mgr.get_count_by_small_type(small_type ) > 0)
				attacker->m_buff_mgr.add_buff(bufferid, buff->get_attacker(), buff->get_skill());
			else
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

uint32_t buff_element_t::add_buff_by_speed(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

uint32_t buff_element_t::add_buff_by_hp_ratio(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

uint32_t buff_element_t::speed_up_when_behurt(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

uint32_t buff_element_t::skill_ext_damage(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

		

		// 触发一次BUFF
		owner->skill_ext_damage(target, damage, buff->get_skill());

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::extreme_hit_back(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

uint32_t buff_element_t::athanasia(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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
		owner->do_team_script_event(event_on_athanasia);
		
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::change_model(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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
			owner->change_model(change_type, change_phase, change_level);
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

uint32_t buff_element_t::change_buff_round(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

uint32_t buff_element_t::add_buff_by_debuf_ratio(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

uint32_t buff_element_t::activation_skill(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

uint32_t buff_element_t::dead_when_relive(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

uint32_t buff_element_t::sub_hp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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
		int ret = calc_value(owner, attacker, type, value);

		owner->sub_hp(ret, attacker, buff->get_skill());
		owner->add_combat_act(proto::common::combat_act_type_buff_damage, ret, owner, attacker, owner->get_hp(), buff->get_id(), buff->get_tid());
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::in_control_speed(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

uint32_t buff_element_t::out_control_speed(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

uint32_t buff_element_t::other_action_count(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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
	switch (buff->get_step())
	{
	case buff_step_load:
		{
			
			owner->set_attr(attrtype_enum::other_action_addition_hurt, ratio);

			// 判断是否加载就生效
			if (buff->get_trigger_type() != (int)event_on_none)
				break;
		}
	case buff_step_trigger:
		{
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

uint32_t buff_element_t::add_buff_behurt(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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
			if (owner->do_effect_hit_result(attacker, ret) == false)
				return buff_notify_none;
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
		if (random_util_t::selectByTenTh(add_buffer_to_team_ratio))
		{
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
					p_hero->m_buff_mgr.add_buff(bufferid, buff->get_attacker(), buff->get_skill());
					//p_hero->add_combat_act(proto::common::combat_act_type_attr_inc, std::abs(value), p_hero, owner, type, p_hero->get_attr(type));
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

uint32_t buff_element_t::add_buff_after_damage(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

uint32_t buff_element_t::add_buff_to_target(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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
		int target_type = element.value3;
		int max_count = element.value4;
		// 附加概率类型(1-增益BUFF，2-减益BUFF)
		int add_ratio_type = element.value5;
		int add_ratio_value = element.value6;

		int add_ratio = 0;
		if (add_ratio_type == 1)
			add_ratio = owner->m_buff_mgr.get_profit_buff_count() * add_ratio_value;
		else if (add_ratio_type == 2)
			add_ratio = owner->m_buff_mgr.get_debuff_count() * add_ratio_value;
		if (!random_util_t::selectByTenTh(ratio + add_ratio))
			return buff_notify_none;

		fight_hero_ptr target = owner->get_cur_target();
		if (NULL == target)
		{
			//log_error("target is null");
			//return buff_notify_none;
			target = owner;
		}

		std::vector<fight_hero_ptr> out;
		owner->get_target_by_buff_select_target(target, target_type, max_count, out);
		for (auto hero : out)
		{
			if (NULL == hero)
			{
				continue;
			}

			hero->m_buff_mgr.add_buff(bufferid, buff->get_attacker(), buff->get_skill());
		}

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::addbuff_by_round_count(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

		// 是否可以重复触发
		if (repeate == 0)
		{
			if ((int)owner->get_count_data().round_count != round_count)
			{
				return buff_notify_none;
			}
		}
		else
		{
			if (( owner->get_count_data().round_count % round_count ) != 0 )
			{
				return buff_notify_none;
			}
		}
		

		if (!random_util_t::selectByTenTh(ratio))
			return buff_notify_none;

		owner->m_buff_mgr.add_buff(addbuff, buff->get_attacker(), buff->get_skill());
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::add_rand_buff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

uint32_t buff_element_t::addbuff_by_debuff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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
			if (owner->m_buff_mgr.get_buff_by_small_type(add_ratio_buff) != NULL)
				ret += add_ratio;
		}

		if (!random_util_t::selectByTenTh(ret))
			return buff_notify_none;

		if (bufferid != 0)
		{
			if (owner->m_buff_mgr.get_buff_by_small_type(bufferid) != NULL )
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

uint32_t buff_element_t::strategy_bomb(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

		int calcvalue = calc_value(owner, attacker, type, value);

		uint32_t result = calcvalue * buff->get_layer();
		if (owner->get_attr(attrtype_enum::debuff_damage_strengthen))
			result *= 1 + ( owner->get_attr(attrtype_enum::debuff_damage_strengthen) * 0.0001 );
		owner->sub_hp(result, attacker);
		FIGHT_LOG("%lu buff[%d] result:%d layer:%d", owner->get_combat()->get_fight_uid(), buff->get_id(), result, buff->get_layer());

		owner->add_combat_act(proto::common::combat_act_type_buff_damage, result, owner, attacker, owner->get_attr(attrtype_enum::hp), buff->get_id(), buff->get_tid());

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::bomb(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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
	}
	case buff_step_detonate:
	{	
		fight_hero_ptr attacker = owner->get_combat()->find_hero(buff->get_attacker());
		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}
		int value = element.value2;
		int type = element.value1;

		int calcvalue = calc_value(owner, attacker, type, value);

		uint32_t result = calcvalue * buff->get_layer();
		if (owner->get_attr(attrtype_enum::debuff_damage_strengthen))
			result *= 2;
		owner->sub_hp(result, attacker);
		FIGHT_LOG("%lu buff[%d] result:%d layer:%d", owner->get_combat()->get_fight_uid(), buff->get_id(), result, buff->get_layer());

		owner->add_combat_act(proto::common::combat_act_type_buff_dot, result, owner, attacker, owner->get_attr(attrtype_enum::hp), buff->get_id(), buff->get_tid());
		
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::detonate_bomb(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

		owner->m_buff_mgr.detonate_buff(value);
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}


uint32_t buff_element_t::union_attack(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

		fight_hero_ptr target = attacker->get_first_target();

		if (NULL == target)
		{
			log_error("target is null");
			return buff_notify_none;
		}
		
		SkillEffectTable* skill_effect_conf = attacker->get_skill_effect(attacker->get_default_skill());
		if (NULL == skill_effect_conf)
		{
			log_error("skill_effect_conf is null");
			return buff_notify_none;
		}
		owner->trigger_skill_by_buff(skill_effect_conf, target, proto::common::combat_act_type_pincer_attack);
		owner->dec_all_skill_cd(owner,1);
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::profit_buff_to_debuff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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
		if (count == 0)
			return buff_notify_none;

		int buffid = element.value3;

		//buff_ptr buff = owner->m_buff_mgr.get_profit_buff_count();
		if (owner->m_buff_mgr.get_profit_buff_count() == 0)
		{
			return buff_notify_none;
		}

		//uint32_t layer = buff->get_layer();
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

uint32_t buff_element_t::add_random_buff_layer(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

		if (count == 0)
			return buff_notify_none;
		int buffid = element.value3;

		owner->m_buff_mgr.add_buff_layer(buffid, buff->get_attacker(),count);

		return count > 0 ? buff_notify_valid : buff_notify_none;
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::add_random_buff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

		int buffid = element.value3;

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


uint32_t buff_element_t::switch_buff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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
		int layer = owner->m_buff_mgr.get_buff_layer_by_tid(buff_id);
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

uint32_t buff_element_t::calc_value(fight_hero_ptr owner, fight_hero_ptr attacker, int type, int value )
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
	case calc_attr_mode_value:
		return value;
	case calc_attr_mode_attacker_attack:
		{
			int attack = attacker->get_attack();
			return attack * (value * 0.0001);
		}
		break;
	case calc_attr_mode_attacker_hp:
		return attacker->get_max_hp() * (value * 0.0001);
	case calc_attr_mode_attacker_defend:
		return attacker->get_attr(attrtype_enum::def_physical) * (value * 0.0001);
	case calc_attr_mode_attacker_speed:
		return attacker->get_speed() * (value * 0.0001);
	case calc_attr_mode_self_hp:
		return owner->get_max_hp() * (value * 0.0001);
	case calc_attr_mode_attacker_damage:
		return attacker->m_calc_result.damage * (value * 0.0001);
	case calc_attr_mode_self_cur_hp:
		return attacker->get_hp() * (value * 0.0001);
	case calc_attr_mode_attacker_round_damage:
		return attacker->get_count_data().one_round_damage * (value * 0.0001);
	default:
		return value;
	}
	return value;
}

uint32_t buff_element_t::relive(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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
		if (!owner->is_dead())
			return buff_notify_none;
		// 复活时进度条
		int speed = element.value2;
		//if (speed == 0)
		//	speed = -1;
		// 复活血量计算方式(1-固定数值，2-攻击方攻击百分比，3-攻击方生命百分比，4-攻击方防御百分比，5-攻击方速度百分比，6-自身生命百分比)
		int hp_type = element.value3;
		// 复活血量参数
		int hp_param = element.value4;

		
		int relive_hp = calc_value( owner, attacker, hp_type, hp_param );

		owner->on_relive( attacker, buff->get_id(), buff->get_tid(), buff->get_skill(), relive_hp, speed);
		//int relive_skill_cd = element.value5;
		//FIGHT_LOG("[%lu]buff_step_load camp[%u] pos[%u] hero_tid[%u] buff[%u] type[%u] value[%d] layer[%u]",
		//	owner->get_fight_uid(), owner->get_camp(), owner->get_pos(), owner->get_tid(), buff->get_id(), type, value, buff->get_layer());
		//if (relive_skill_cd > 0)
		{
		//	attacker->add_skill_cd( buff->get_skill(), relive_skill_cd );
		}
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::shield(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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
			// 计算方式(1-固定数值，2-攻击方攻击百分比，3-攻击方生命百分比，4-攻击方防御百分比，5-攻击方速度百分比，6-自身生命百分比)
			int type = element.value1;
			// 参数
			int param = element.value2;
			// 参数3 盾小类型ID
			int shield_small_type = element.value3;
			int value = calc_value(owner, attacker, type, param);

			owner->set_attr(attrtype_enum::shield_points, value);
			owner->get_count_data().shield_small_type = shield_small_type;
			owner->add_combat_act(proto::common::combat_act_type_attr_inc, std::abs(value), owner, attacker, attrtype_enum::shield_points, owner->get_attr(attrtype_enum::shield_points));
			//FIGHT_LOG("[%lu]buff_step_load camp[%u] pos[%u] hero_tid[%u] buff[%u] type[%u] value[%d] layer[%u]",
			//	owner->get_fight_uid(), owner->get_camp(), owner->get_pos(), owner->get_tid(), buff->get_id(), type, value, buff->get_layer());
			return buff_notify_valid;
		}
		break;
		case buff_step_unload:
			{
				owner->set_attr(attrtype_enum::shield_points, 0);
				owner->add_combat_act(proto::common::combat_act_type_attr_dec, 0, owner, attacker, attrtype_enum::shield_points, owner->get_attr(attrtype_enum::shield_points));
			}
			break;
	default:
		break;
	}

	return buff_notify_none;
}


uint32_t buff_element_t::change_cd(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

uint32_t buff_element_t::cleanup(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

uint32_t buff_element_t::switch_hp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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
				uint32_t result = owner_ratio * attacker->get_max_hp();
				if (result > attacker->get_hp())
				{
					attacker->add_hp(result - attacker->get_hp());
					FIGHT_LOG("%lu buff[%d] result:%d layer:%d", owner->get_combat()->get_fight_uid(), buff->get_id(), result, buff->get_layer());

					attacker->add_combat_act(proto::common::combat_act_type_buff_heal, result, owner, attacker, attacker->get_attr(attrtype_enum::hp), buff->get_id(), buff->get_tid());
				}
				else
				{
					attacker->sub_hp(attacker->get_hp() - result, attacker, buff->get_skill());
					FIGHT_LOG("%lu buff[%d] result:%d layer:%d", owner->get_combat()->get_fight_uid(), buff->get_id(), result, buff->get_layer());

					attacker->add_combat_act(proto::common::combat_act_type_damage, result, owner, attacker, attacker->get_attr(attrtype_enum::hp), buff->get_id(), buff->get_tid());
				}
			
			}
			{
				uint32_t result = attack_ratio * owner->get_max_hp();
				if (result > owner->get_hp())
				{
					owner->add_hp(result - owner->get_hp());
					FIGHT_LOG("%lu buff[%d] result:%d layer:%d", owner->get_combat()->get_fight_uid(), buff->get_id(), result, buff->get_layer());

					owner->add_combat_act(proto::common::combat_act_type_buff_heal, result, owner, attacker, owner->get_attr(attrtype_enum::hp), buff->get_id(), buff->get_tid());
				}
				else
				{
					owner->sub_hp(owner->get_hp() - result, attacker, buff->get_skill());
					FIGHT_LOG("%lu buff[%d] result:%d layer:%d", owner->get_combat()->get_fight_uid(), buff->get_id(), result, buff->get_layer());

					owner->add_combat_act(proto::common::combat_act_type_damage, result, owner, attacker, owner->get_attr(attrtype_enum::hp), buff->get_id(), buff->get_tid());
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

uint32_t buff_element_t::switch_speed(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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
			int32_t att_speed = element.value1 * max_att_speed / 10000;
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

uint32_t buff_element_t::balance_hp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

uint32_t buff_element_t::balance_speed(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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
		int32_t att_speed = element.value1 * max_att_speed / 10000;
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

uint32_t buff_element_t::element_test(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
{
	log_error("buff_element_t::element_test nothing to do!");
	return buff_notify_none;
}

uint32_t buff_element_t::change_attr_without_num(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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
		owner->add_combat_act(proto::common::combat_act_type_attr_inc, std::abs(value), owner, attacker, type, owner->get_attr(type));

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
		owner->add_combat_act(proto::common::combat_act_type_attr_dec, std::abs(value), owner, attacker, type, owner->get_attr(type));

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

void buff_element_t::inc_all_team_attr(fight_hero_ptr owner, int type, int value, int chg_type)
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
			p_hero->add_combat_act(proto::common::combat_act_type_attr_inc, std::abs(value), p_hero, owner, type, p_hero->get_attr(type));
		}
	}
}

void buff_element_t::dec_all_team_attr(fight_hero_ptr owner, int type, int value, int chg_type)
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
			p_hero->add_combat_act(proto::common::combat_act_type_attr_dec, std::abs(value), p_hero, owner, type, p_hero->get_attr(type));
		}
	}
}

void buff_element_t::inc_all_enemy_attr(fight_hero_ptr owner, int type, int value, int chg_type)
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
			p_hero->add_combat_act(proto::common::combat_act_type_attr_inc, std::abs(value), p_hero, owner, type, p_hero->get_attr(type));
		}
	}
}

void buff_element_t::dec_all_enemy_attr(fight_hero_ptr owner, int type, int value, int chg_type)
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
			p_hero->add_combat_act(proto::common::combat_act_type_attr_dec, std::abs(value), p_hero, owner, type, p_hero->get_attr(type));
		}
	}
}

uint32_t buff_element_t::change_attr_all_team(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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
		
		inc_all_team_attr(owner, type, value, chg_type);

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
		inc_all_team_attr(owner, type, value, chg_type);
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
		dec_all_team_attr(owner, type, value, chg_type);

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

uint32_t buff_element_t::change_attr_all_enemy(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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
		inc_all_enemy_attr(owner, type, value, chg_type);

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
		inc_all_enemy_attr(owner, type, value, chg_type);
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
		dec_all_enemy_attr(owner, type, value, chg_type);

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

uint32_t buff_element_t::change_temp_attr(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

uint32_t buff_element_t::change_attr_by_hp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

uint32_t buff_element_t::change_attr_by_target_hp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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
		if (owner->get_cur_target() == NULL)
		{
			return buff_notify_none;
		}
		uint32_t type = element.value1;
		int value = element.value2 * buff->get_layer();
		int chg_type = element.value3;

		float ratio = (float)element.value4 * 0.0001;
		int greater = element.value5;


		float hp_ratio = (float)owner->get_cur_target()->get_hp() / (float)owner->get_cur_target()->get_max_hp();
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

uint32_t buff_element_t::change_attr(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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
				owner->add_combat_act(proto::common::combat_act_type_attr_inc, std::abs(value), owner, attacker, type,owner->get_attr(type));

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
				owner->add_combat_act(proto::common::combat_act_type_attr_dec, std::abs(value), owner, attacker, type, owner->get_attr(type));

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
					owner->add_combat_act(proto::common::combat_act_type_attr_inc, std::abs(value), owner, attacker, type, owner->get_attr(type));
				else
					owner->add_combat_act(proto::common::combat_act_type_attr_dec, std::abs(value), owner, attacker, type, owner->get_attr(type));

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

uint32_t buff_element_t::pursue_and_attack(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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
		// 获取当前行动的人
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

		if (attacker->get_unique_id() == owner->get_unique_id())
			return buff_notify_none;

		int32_t default_skill = attacker->get_default_skill();
		if (attacker->get_fight_calc_result().skill != (uint32_t)default_skill)
			return buff_notify_none;

		if (attacker->get_fight_calc_result().phase != 0)
			return buff_notify_none;

		SkillEffectTable* skill_effect_conf = NULL;

		uint32_t skill = element.value1;
		uint32_t skill_level = element.value2;
		uint32_t buff_id = element.value3;

		if (skill == 0)
		{
			skill = owner->get_default_skill();
			skill_level = owner->get_skill_level(skill);
		}

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

		owner->trigger_skill_by_buff(skill_effect_conf, target, proto::common::combat_act_type_follow);
		if (buff_id != 0)
		{
			owner->m_buff_mgr.add_buff(buff_id, buff->get_attacker());
		}
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::taunt(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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
			owner->set_be_taunt_info(attacker, true, buff->get_small_type());
			return element_notify_type(attrtype_enum::taunt);
		}
		break;
	case buff_step_unload:
		{
			owner->set_be_taunt_info(attacker, false);
			return element_notify_type(attrtype_enum::taunt);
		}
		break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::forget(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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
			owner->add_combat_act(proto::common::combat_act_type_attr_inc, 1, owner, attacker, attrtype_enum::forget, 1);
			return element_notify_type(attrtype_enum::forget);
		}
		break;
	case buff_step_unload:
		{
			owner->m_buff_element_mgr.dec_element_value(attrtype_enum::forget, 1);
			owner->add_combat_act(proto::common::combat_act_type_attr_dec, 0, owner, attacker, attrtype_enum::forget, 0);
			return element_notify_type(attrtype_enum::forget);
		}
		break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::silent(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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
		owner->add_combat_act(proto::common::combat_act_type_attr_inc, 1, owner, attacker, attrtype_enum::silent, 1);
		return element_notify_type(attrtype_enum::silent);
	}
	break;
	case buff_step_unload:
	{
		owner->m_buff_element_mgr.dec_element_value(attrtype_enum::silent, 1);
		owner->add_combat_act(proto::common::combat_act_type_attr_dec, 1, owner, attacker, attrtype_enum::silent, 0);
		return element_notify_type(attrtype_enum::silent);
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::trigger_skill_by_hp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

		target = get_target_by_type(owner, attacker, skill_effect_conf, buff, target_type);

		//if (NULL == target)
		//{
		//	log_error("target is null");
		//	return buff_notify_none;
		//}

		++attacker->get_count_data().trigger_skill_by_hp;

		attacker->trigger_skill_by_buff(skill_effect_conf, target, proto::common::combat_act_type_trigger_skill);
		attacker->trigger_skill_count(attacker->trigger_skill_count() + 1);
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::trigger_skill_by_critical_blow(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

		target = get_target_by_type(owner, attacker, skill_effect_conf, buff, target_type);


		//if (NULL == target)
		//{
		//	log_error("target is null");
		//	return buff_notify_none;
		//}
		++attacker->get_count_data().trigger_skill_by_critical_blow;

		attacker->trigger_skill_by_buff(skill_effect_conf, target, proto::common::combat_act_type_trigger_skill);
		attacker->trigger_skill_count(attacker->trigger_skill_count() + 1);
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::trigger_skill_by_beattack_count(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

		if ((attacker->get_count_data().beattack_count) % trigger_mod != 0)
		{
			return buff_notify_none;
		}

		if (in_control != 0 && attacker->is_in_control())
		{
			return buff_notify_none;
		}

		target = get_target_by_type(owner, attacker, skill_effect_conf, buff, target_type);

		//if (NULL == target)
		//{
		//	log_error("target is null");
		//	return buff_notify_none;
		//}

		++attacker->get_count_data().trigger_skill_by_beattack_count;
		attacker->trigger_skill_by_buff(skill_effect_conf, target, proto::common::combat_act_type_trigger_skill);
		attacker->trigger_skill_count(attacker->trigger_skill_count() + 1);
		
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::trigger_skill_by_attack_count(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

		if ((attacker->get_count_data().attack_count) % trigger_mod != 0)
		{
			return buff_notify_none;
		}

		if (in_control != 0 && attacker->is_in_control())
		{
			return buff_notify_none;
		}

		target = get_target_by_type(owner, attacker, skill_effect_conf, buff, target_type);

		//if (NULL == target)
		//{
		//	log_error("target is null");
		//	return buff_notify_none;
		//}
		++attacker->get_count_data().trigger_skill_by_attack_count;
		attacker->trigger_skill_by_buff(skill_effect_conf, target, proto::common::combat_act_type_trigger_skill);
		attacker->trigger_skill_count(attacker->trigger_skill_count() + 1);
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
	// 目标类型自己
	if (target_type == 0)
	{
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
		owner->get_combat()->find_target_by_skill_conf(owner, skill_conf, targets, spurting_heroes);
		if (!targets.empty())
			target = targets[0];
	}
	// 目标类型最近出手的人
	else if (target_type == 3)
	{
		target = owner->get_combat()->get_att_fight_hero();
	}
	return target;
}

uint32_t buff_element_t::trigger_skill(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

		target = get_target_by_type( owner, attacker, skill_effect_conf, buff, target_type );
		

		if (max_count > 0)
		{
			if ((int)attacker->trigger_skill_count() >= (int)max_count)
			{
				return buff_notify_none;
			}
		}

		//if (NULL == target)
		//{
		//	log_error("target is null");
		//	return buff_notify_none;
		//}

		attacker->trigger_skill_by_buff(skill_effect_conf, target, proto::common::combat_act_type_trigger_skill);
		attacker->trigger_skill_count( attacker->trigger_skill_count() + 1 );
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::use_skill(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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



		if (NULL == attacker)
		{
			log_error("attacker is null");
			return buff_notify_none;
		}

		target = get_target_by_type(owner, attacker, skill_effect_conf, buff, target_type);

		//if (NULL == target)
		{
		//	log_error("target is null");
		//	return buff_notify_none;
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

uint32_t buff_element_t::add_sp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

uint32_t buff_element_t::steal_buff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

		int effect_type = element.value1;
		buff_ptr buff = owner->m_buff_mgr.get_random_buff_by_type(effect_type);
		if (NULL == buff)
		{
			log_error("buff is null [%d]", effect_type);
			return buff_notify_none;
		}

		if (buff->get_layer() > 0)
		{
			// 告诉客户端偷BUFF行为 做表现用
			owner->add_combat_act(proto::common::combat_act_type_steal_buff, buff->get_id(), owner, attacker);
			uint32_t tid = buff->get_tid();
			owner->m_buff_mgr.remove_buff(buff->get_id());
			attacker->m_buff_mgr.add_buff(tid, owner->get_uid(), 0, buff->get_layer());
			return buff_notify_valid;
		}
		
		return buff_notify_none;
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::damage(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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
		int value = element.value2;
		int type = element.value1;
		int not_dot = element.value3;

		int calcvalue = calc_value(owner, attacker, type, value);

		uint32_t result = calcvalue * buff->get_layer();
		if (owner->get_attr(attrtype_enum::debuff_damage_strengthen))
			result *= 2;
		owner->sub_hp(result, attacker);
		FIGHT_LOG("%lu buff[%d] result:%d layer:%d", owner->get_combat()->get_fight_uid(), buff->get_id(), result, buff->get_layer());
		
		if(not_dot)
			owner->add_combat_act(proto::common::combat_act_type_buff_damage, result, owner, attacker, owner->get_attr(attrtype_enum::hp), buff->get_id(),buff->get_tid());
		else
			owner->add_combat_act(proto::common::combat_act_type_buff_dot, result, owner, attacker, owner->get_attr(attrtype_enum::hp), buff->get_id(), buff->get_tid());
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::sub_max_hp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

		/*
		减少给敌人造成伤害量的X%的血量上限。
		该效果是减少血量上限的效果，
		且减少的血量上限无法在所进行的战斗中恢复。
		最多可1次性减少血量上限的X%，
		并可通过持续攻击最多可减少X%。
		（未满基础体力的X%时无法减少）
		*/
		int damage_ratio = element.value1;
		int once_max_sub_max_hp_ratio = element.value2;
		int all_max_sub_max_hp_ratio = element.value3;

		
		int cur_sub_max = -owner->m_buff_element_mgr.get_buff_attr(attrtype_enum::max_hp);
		if (cur_sub_max >= all_max_sub_max_hp_ratio)
			return buff_notify_none;


		int result = attacker->get_fight_calc_result().damage * ( damage_ratio * 0.0001 );
		if (result == 0)
			return buff_notify_none;

		int once_max = owner->get_max_hp() * (once_max_sub_max_hp_ratio * 0.0001);
		if (result > once_max)
			result = once_max;

		int all_max = owner->get_max_hp() * (all_max_sub_max_hp_ratio * 0.0001);

		int cur_all_max = owner->get_max_hp() * (cur_sub_max * 0.0001);

		if (result + cur_all_max >= all_max)
			result = all_max - cur_all_max;

		//uint32_t result = calcvalue * buff->get_layer();


		owner->m_buff_element_mgr.dec_element_value(attrtype_enum::max_hp, result, 0);
		owner->add_combat_act(proto::common::combat_act_type_attr_dec, std::abs(result), owner, attacker, attrtype_enum::max_hp, owner->get_attr(attrtype_enum::max_hp));

		FIGHT_LOG("[%lu]buff_step_load camp[%u] pos[%u] hero_tid[%u] buff[%u] type[%u] value[%d] layer[%u]",
			owner->get_fight_uid(), owner->get_camp(), owner->get_pos(), owner->get_tid(), buff->get_id(), attrtype_enum::max_hp, result, buff->get_layer());

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::add_max_hp(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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
		

		int calcvalue = calc_value(owner, attacker, type, value);


		uint32_t result = calcvalue * buff->get_layer();


		owner->m_buff_element_mgr.inc_element_value(attrtype_enum::max_hp, result, 0);
		owner->add_combat_act(proto::common::combat_act_type_attr_inc, std::abs(result), owner, attacker, attrtype_enum::max_hp, owner->get_attr(attrtype_enum::max_hp));

		FIGHT_LOG("[%lu]buff_step_load camp[%u] pos[%u] hero_tid[%u] buff[%u] type[%u] value[%d] layer[%u]",
			owner->get_fight_uid(), owner->get_camp(), owner->get_pos(), owner->get_tid(), buff->get_id(), type, value, buff->get_layer());


		owner->add_hp(result);
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

uint32_t buff_element_t::addhp_by_hpchange(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

uint32_t buff_element_t::heal(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

		int calcvalue = calc_value(owner, attacker, type, value);


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

uint32_t buff_element_t::heal_special(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

uint32_t buff_element_t::heal_all_team(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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
				result = p_hero->heal_hp(owner, add_hp);
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

uint32_t buff_element_t::remove_buff_by_small_type(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

uint32_t buff_element_t::immuno(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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
		owner->m_buff_element_mgr.inc_element_value(attrtype_enum::immuno, 1);
		owner->add_combat_act(proto::common::combat_act_type_attr_inc, 1, owner, attacker, attrtype_enum::immuno, 1);
		owner->add_immuno_buff_type(element.value1);
		return element_notify_type(attrtype_enum::taunt);
	}
	break;
	case buff_step_unload:
	{
		owner->m_buff_element_mgr.dec_element_value(attrtype_enum::immuno, 1);
		owner->add_combat_act(proto::common::combat_act_type_attr_dec, 1, owner, attacker, attrtype_enum::immuno, 0);
		owner->clear_immuno_buff_type();
		return element_notify_type(attrtype_enum::taunt);
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::story_play(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

uint32_t buff_element_t::remove_buff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

uint32_t buff_element_t::remove_buff_layer(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

uint32_t buff_element_t::heal_overflow(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

		int calcvalue = calc_value(owner, attacker, type, value);

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

uint32_t buff_element_t::addbuff_when_add_small_type(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

		int small_type = element.value1;
		int add_buffer_id = element.value2;

		BuffTable* conf = GET_CONF(BuffTable, owner->get_fight_calc_result().add_buff_id);
		if (NULL == conf)
			return buff_notify_none;

		if ((int)conf->buff_small_type() != small_type)
		{
			return buff_notify_none;
		}

		owner->m_buff_mgr.add_buff(add_buffer_id, buff->get_attacker());

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::addbuff_when_addbuff(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

		owner->m_buff_mgr.add_buff(add_buffer_id, buff->get_attacker());

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::addbuff_when_over_layer(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

				owner->m_buff_mgr.add_buff(add_buff_id, buff->get_attacker());
			}
			return buff_notify_valid;
		}
		break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::addbuff_when_exist(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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
			if (owner->m_buff_mgr.get_buff_by_small_type(buffer_id) == NULL)
				return buff_notify_none;
		}
		else
		{
			// 有增益追加BUFF
			if (owner->m_buff_mgr.get_profit_buff_count() == 0)
				return buff_notify_none;
		}
		owner->m_buff_mgr.add_buff(add_buffer_id, buff->get_attacker());

		return buff_notify_valid;
	}
	break;
	default:
		break;
	}

	return buff_notify_none;
}

uint32_t buff_element_t::all_team_damage(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

uint32_t buff_element_t::rand_damage(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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
	// 判断攻击方与受击方
	fight_hero_ptr attacker = owner->get_combat()->get_att_fight_hero();
	if (NULL == attacker)
	{
		log_error("attacker is null");
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
			int min_rand_num = element.value1;
			int max_rand_num = element.value2;
			int damage_add_rate = element.value3;
			int rand_num = random_util_t::randBetween(min_rand_num, max_rand_num);
			int change_attr_value = rand_num * damage_add_rate;
			buff->m_run_attr.param1 = change_attr_value;

			owner->m_buff_element_mgr.inc_element_value(attrtype_enum::self_attack_addition, change_attr_value, 1);
			owner->add_combat_act(proto::common::combat_act_type_attr_inc, change_attr_value, owner, attacker, attrtype_enum::self_attack_addition, owner->get_attr(attrtype_enum::self_attack_addition));
			owner->add_combat_act(proto::common::combat_act_type_rand_damage, rand_num, owner, attacker);
			return element_notify_type(attrtype_enum::self_attack_addition);
		}
		break;
	case buff_step_unload:
		{
			int change_attr_value = buff->m_run_attr.param1;
			owner->m_buff_element_mgr.dec_element_value(attrtype_enum::self_attack_addition, change_attr_value, 1);
			owner->add_combat_act(proto::common::combat_act_type_attr_dec, change_attr_value, owner, attacker, attrtype_enum::self_attack_addition, owner->get_attr(attrtype_enum::self_attack_addition));
			return element_notify_type(attrtype_enum::self_attack_addition);
		}
		break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::all_team_shield(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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
			int value = calc_value(owner, attacker, type, param);

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

uint32_t buff_element_t::all_team_damage_turn(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

uint32_t buff_element_t::death_guard_energy(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

uint32_t buff_element_t::death_guard(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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
		//owner->get_buff_manager().add_buff_layer(buff_id, 0, energy);
		//owner->get_count_data().death_guard_energy = energy;
		//owner->get_count_data().max_death_guard_energy = max_energy;
		// 判断是否加载就生效
		if (buff->get_trigger_type() != (int)event_on_none)
			break;
	case buff_step_trigger:
	{
		int buff_id = element.value1;
		
		//if (owner->get_count_data().max_death_guard_energy == 0)
		//	return buff_notify_none;
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
		if ((int)attacker->get_fight_calc_result().damage < (int)owner->get_hp())
			return buff_notify_none;

		// 判断是否还有层数
		if (owner->get_buff_manager().get_buff_layer_by_tid(buff_id, 0) == 0)
			return buff_notify_none;

		//--owner->get_count_data().death_guard_energy;
		owner->get_buff_manager().sub_buff_layer(buff_id, 0, 1);
		// 增加本次死亡守护效果
		target->m_buff_element_moment_mgr.inc_element_value(attrtype_enum::athanasia, 1, 0);

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

uint32_t buff_element_t::bear_hurt(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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

		target->set_bear_damage(ratio, owner);
		owner->add_combat_act(proto::common::combat_act_type_accept_damage, 0, owner, target, 0, buff->get_id());
		return buff_notify_valid;
	}
	break;
	default:
		break;
	}
	return buff_notify_none;
}

uint32_t buff_element_t::change_att_speed(fight_hero_ptr owner, buff_ptr buff, const buff_element_data& element, const int numchange)
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
			int32_t max_att_speed = owner->get_combat()->get_max_att_speed();
			int32_t att_speed = element.value1 * ( max_att_speed * 0.0001 ) * (1 + owner->get_attr(attrtype_enum::speed_effect_ratio) * 0.0001);
			int32_t on_event = element.value2;
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
					attacker->do_team_script_event(event_on_add_att_speed);
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
