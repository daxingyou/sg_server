#include "pk_manager.hpp"
#include "log.hpp"
#include "log/log_wrapper.hpp"
#include "role/role_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "item/item_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "tblh/Pk.tbls.h"
#include "fight/fight_manager.hpp"
#include "role/money_manager.hpp"
#include "sys_notice/sys_notice_manager.hpp"
#include "scene/scene_manager.hpp"

USING_NS_COMMON;

uint32_t pk_manager_t::do_pk(role_ptr p_role, role_ptr p_target, proto::common::role_change_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return errcode_enum::notice_unknown;
	}
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	if (NULL == p_target)
	{
		log_error("NULL == p_target");
		return errcode_enum::notice_rt_no_target;
	}
	if (!p_target->can_enter_fight())
	{
		return errcode_enum::notice_is_fighting;
	}
	if (p_role->get_scene_id() != p_target->get_scene_id())
	{
		log_error("p_target[lu] is too far", p_target->get_uid());
		return errcode_enum::pk_cue_5;
	}

	if (!p_role->is_nearby_object(p_target, get_pk_distance()))
	{
		log_error("p_target[%llu] is more than[%d] too far", p_target->get_uid(), get_pk_distance());
		return errcode_enum::notice_fight_target_too_farawary;
	}
	if (is_safe(p_target))
	{
		log_error("p_target[%llu] is safe", p_target->get_uid());
		return errcode_enum::pk_cue_1;
	}
	if (is_protected(p_role))
	{
		log_error("p_role[%llu] level[%d] is protected", p_role->get_uid(), p_role->get_level());
		return errcode_enum::pk_cue_4;
	}
	proto::common::fight_type type = proto::common::fight_type_pk;
	switch (p_role->get_pk_mode())
	{
	case proto::common::PK_PEACE:
		{
			uint32_t pk_count = p_role->get_pk_count();
			if (pk_count == 0)
			{
				log_error("p_role[%llu] pk_count is limit", p_role->get_uid());
				return errcode_enum::pk_cue_2;
			}
			uint32_t delta_level = 0;
			uint32_t max_delta_level = pk_manager_t::get_delta_level();
			if (p_role->get_level() > p_target->get_level())
			{
				delta_level = p_role->get_level() - p_target->get_level();
			}
			else
			{
				delta_level = p_target->get_level() - p_role->get_level();
			}
			if (delta_level > max_delta_level)
			{
				log_error("p_role[%llu] level[%d] p_target[%llu] level[%d] delta_level too large", p_role->get_uid(), p_role->get_level(), p_target->get_uid(), p_target->get_level());
				return errcode_enum::pk_cue_3;
			}
			if (is_protected(p_target))
			{
				log_error("p_target[%llu] level[%d] is protected", p_target->get_uid(), p_target->get_level());
				return errcode_enum::pk_cue_4;
			}
			if (p_target->get_pk_value() == 0)
			{
				uint32_t item_id = get_pk_item();
				if (item_manager_t::remove_item(p_role, item_id, 1, log_enum::source_type_pk_fight, 0, p_data,
					proto::common::package_type_main) != errcode_enum::error_ok)
				{
					log_error("p_role[%llu] pk item not enough", p_role->get_uid());
					return errcode_enum::pk_cue_6;
				}
			}
			p_role->set_pk_count(pk_count - 1);
			proto::common::personal_info *p_info = p_data->mutable_per_info();
			p_role->get_personal_info(p_info);
			p_role->record_calc_pk_value();
			p_target->record_calc_pk_value();
		}
		break;
	case proto::common::PK_FAMILY:
		{
			if (p_role->get_family_id() == p_target->get_family_id())
			{
				log_error("p_role[%llu] and  p_target[%llu] same family", p_role->get_uid(), p_target->get_uid());
				return errcode_enum::notice_unknown;
			}

			// 家族战副本处理
			if (p_role->is_in_family_war() && p_target->is_in_family_war())
			{
				type = proto::common::fight_type_family_pk;
			}
		}
		break;
	case proto::common::PK_COUNTRY:
		{
			return errcode_enum::pk_cue_9;
			if (p_role->get_country_id() == p_target->get_country_id())
			{
				log_error("p_role[%llu] and  p_target[%llu] same country", p_role->get_uid(), p_target->get_uid());
				return errcode_enum::notice_unknown;
			}
		}
		break;
	default:
		break;
	} 
	proto::common::fight_param fp;
	fp.set_type(type);
	fight_manager_t::fight_pvp(p_role, p_target, &fp);
	return errcode_enum::error_ok;
}

bool pk_manager_t::attacker_pk_result(uint64_t self_uid, uint64_t enemy_uid, bool is_win, proto::common::role_change_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return false;
	}
	role_ptr p_self = role_manager_t::find_role(self_uid);
	if (NULL == p_self)
	{
		log_error("NULL == p_self[%llu]", self_uid);
		return false;
	}
	role_ptr p_enemy = role_manager_t::find_role(enemy_uid);
	if (NULL == p_enemy)
	{
		log_error("NULL == p_enemy[%llu]", enemy_uid);
		return false;
	}
	switch (p_self->get_pk_mode())
	{
	case proto::common::PK_PEACE:
		{
			if (is_win)
			{
				PK_LOG("attacker_pk_result win self[%llu] enemy[%llu]", self_uid, enemy_uid);
				uint32_t target_pk_value = p_enemy->get_calc_pk_value();
				PK_LOG("enemy[%lu] pk_value[%d]", enemy_uid, target_pk_value);
				if (target_pk_value > 0)
				{
					//击败红名奖励
					uint32_t add_errantry = get_errantry(target_pk_value);
					p_self->add_errantry(add_errantry, log_enum::source_type_pk_attacker_win, 0);
					PK_LOG("self[%lu] add_errantry[%d]", self_uid, add_errantry);
					if (is_criminal(target_pk_value))
					{
						//击败通缉犯
						proto::server::ge_pk_beat_criminal_notify ntf;
						ntf.set_criminal_uid(enemy_uid);
						env::server->send_msg_to_center(op_cmd::ge_pk_beat_criminal_notify, self_uid, ntf);
					}
				}
				else
				{
					//击败白名惩罚
					uint32_t cur_pk_value = p_self->get_calc_pk_value();
					bool old_is_criminal = is_criminal(cur_pk_value);
					uint32_t add_pk_value = get_pk_value_add();
					uint32_t old_pk_value = p_self->get_pk_value();
					p_self->add_pk_value(add_pk_value, log_enum::source_type_pk_attacker_win, 0);
					PK_LOG("self[%lu] add_pk_value[%d] from[%d] to [%d]", self_uid, add_pk_value, old_pk_value, p_self->get_pk_value());
					if (is_criminal(cur_pk_value))
					{
						if (!old_is_criminal)
						{
							if (random_util_t::selectByTenTh(get_prison_rate()))
							{
								//成为通缉犯有概率关进大牢
								p_self->move_to_prison();
							}
							else
							{
								//没有关进大牢发送公告
								BROADCAST_SYS_NOTICE(sys_notice_enum::sys_notice_9, 0, 0, 0, p_self->get_name().c_str())
							}
						}
					}
				}
			}
			else
			{
				PK_LOG("attacker_pk_result lose self[%llu] enemy[%llu]", self_uid, enemy_uid);
				uint32_t self_pk_value = p_self->get_calc_pk_value();

				if (self_pk_value > 0)
				{
					//发起方红名失败惩罚
					if (random_util_t::selectByTenTh(get_attacker_lose_prison_rate(self_pk_value)))
					{
						p_self->move_to_prison();
					}
					if (is_criminal(self_pk_value))
					{
						//击败通缉犯
						proto::server::ge_pk_beat_criminal_notify ntf;
						ntf.set_criminal_uid(self_uid);
						env::server->send_msg_to_center(op_cmd::ge_pk_beat_criminal_notify, enemy_uid, ntf);
					}
				}
				//复活点
			}
		}
		break;
	case proto::common::PK_FAMILY:
		{
			// 目前只有家族pk模式会死亡
			const scene_ptr& p_scene = scene_manager_t::find_scene(p_self->get_scene_id());
			if (p_scene != NULL)
			{
				// 目前只有家族pk模式会死亡
				if (is_win)
				{
					p_scene->on_role_kill_other(p_self, p_enemy);
				}
				else
				{
					p_scene->on_other_kill_me(p_self, p_enemy);
				}
			}
		}
		break;
	case proto::common::PK_COUNTRY:
		{

		}
		break;
	default:
		break;
	}
	proto::common::personal_info *p_info = p_data->mutable_per_info();
	p_self->get_personal_info(p_info);
	p_data->set_exp(p_self->get_exp());

	return true;
}

bool pk_manager_t::defenser_pk_result(uint64_t self_uid, uint64_t enemy_uid, bool is_win, proto::common::role_change_data* p_data)
{
	if (NULL == p_data)
	{
		log_error("NULL == p_data");
		return false;
	}
	role_ptr p_self = role_manager_t::find_role(self_uid);
	if (NULL == p_self)
	{
		log_error("NULL == p_self[%llu]", self_uid);
		return false;
	}
	role_ptr p_enemy = role_manager_t::find_role(enemy_uid);
	if (NULL == p_enemy)
	{
		log_error("NULL == p_enemy[%llu]", enemy_uid);
		return false;
	}
	switch (p_self->get_pk_mode())
	{
	case proto::common::PK_PEACE:
		{
			if (is_win)
			{
				//胜利
				PK_LOG("defenser_pk_result win self[%llu] enemy[%llu]", self_uid, enemy_uid);
			}
			else
			{
				PK_LOG("defenser_pk_result lose self[%llu] enemy[%llu]", self_uid, enemy_uid);
				//失败
				uint32_t self_pk_value = p_self->get_calc_pk_value();
				uint32_t cur_exp = p_self->get_exp();
				uint32_t del_exp = get_exp_reduce(self_pk_value);
				del_exp = cur_exp * del_exp / 10000;
				if (cur_exp > del_exp)
				{
					cur_exp -= del_exp;
				}
				else
				{
					cur_exp = 0;
				}
				PK_LOG("self[%lu] pk_value[%d]", self_uid, self_pk_value);
				if (self_pk_value > 0)
				{
					//红名失败
					if (random_util_t::selectByTenTh(get_defenser_lose_prison_rate(self_pk_value)))
					{
						p_self->move_to_prison();
					}
					uint32_t old_exp = p_self->get_exp();
					p_self->set_exp(cur_exp, log_enum::source_type_pk_defenser_lose, 0, p_data);
					log_wrapper_t::send_role_exp_log(p_self->get_uid(), p_self->get_level(), old_exp, del_exp,
						p_self->get_level(), p_self->get_exp(), log_enum::source_type_pk_defenser_lose, 0);
					PK_LOG("self[%lu] reduce exp[%d] from[%d] to [%d]", self_uid, del_exp, old_exp, p_self->get_exp());
				}
				else
				{
					PK_LOG("self[%lu] target_count[%d]", self_uid, p_self->get_target_count());
					//白名失败
					if (p_self->get_target_count() > 0)
					{
						p_self->set_target_count(p_self->get_target_count() - 1);
						uint32_t old_exp = p_self->get_exp();
						p_self->set_exp(cur_exp, log_enum::source_type_pk_defenser_lose, 0, p_data);
						log_wrapper_t::send_role_exp_log(p_self->get_uid(), p_self->get_level(), old_exp, del_exp,
							p_self->get_level(), p_self->get_exp(), log_enum::source_type_pk_defenser_lose, 0);
						PK_LOG("self[%lu] reduce exp[%d] from[%d] to [%d]", self_uid, del_exp, old_exp, p_self->get_exp());
					}
					//复活点
				}
			}
		}
		break;
	case proto::common::PK_FAMILY:
		{
			// 目前只有家族pk模式会死亡
			const scene_ptr& p_scene = scene_manager_t::find_scene(p_self->get_scene_id());
			if (p_scene != NULL)
			{
				// 目前只有家族pk模式会死亡
				if (is_win)
				{
					p_scene->on_role_kill_other(p_self, p_enemy);
				}
				else
				{
					p_scene->on_other_kill_me(p_self, p_enemy);
				}
			}
		}
		break;
	case proto::common::PK_COUNTRY:
		{

		}
		break;
	default:
		break;
	}
	proto::common::personal_info *p_info = p_data->mutable_per_info();
	p_self->get_personal_info(p_info);

	return true;
}

uint32_t pk_manager_t::do_challenge(role_ptr p_role, uint64_t target_uid)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	if (!money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_YUANBAO, get_challenge_gold(), log_enum::source_type_role_challenge, 0))
	{
		log_error("role[%llu] gold not enough", p_role->get_uid());
		return errcode_enum::notice_gold_money_not_enough;
	}
	proto::server::ge_challenge_request req;
	req.set_target_uid(target_uid);
	env::server->send_msg_to_center(op_cmd::ge_challenge_request, p_role->get_uid(), req);
	return errcode_enum::error_ok;
}


uint32_t pk_manager_t::process_challenge(role_ptr p_role, uint32_t process)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return errcode_enum::notice_role_null;
	}
	if (0 == process)
	{
		//接受
		if (!money_manager_t::use_money(p_role, proto::common::MONEY_TYPE_YUANBAO, get_accept_challenge_gold(), log_enum::source_type_role_challenge, 0))
		{
			log_error("role[%llu] gold not enough", p_role->get_uid());
			return errcode_enum::notice_gold_money_not_enough;
		}
	}
	proto::server::ge_challenge_process_request req;
	req.set_process(process);
	env::server->send_msg_to_center(op_cmd::ge_challenge_process_request, p_role->get_uid(), req);
	return errcode_enum::error_ok;
}

void pk_manager_t::challenge_fight(uint64_t challenger_uid, uint64_t target_uid)
{
	bool challenger_exist = false;

	role_ptr p_challenger = role_manager_t::find_role(challenger_uid);
	if (NULL != p_challenger && p_challenger->is_in_king_city())
	{
		challenger_exist = true;
	}
	bool target_exist = false;
	role_ptr p_target = role_manager_t::find_role(target_uid);
	if (NULL != p_target && p_target->is_in_king_city())
	{
		target_exist = true;
	}
	
	proto::server::ge_challenge_fight_reply reply;
	if (challenger_exist)
	{
		if (target_exist)
		{
			//开打 打完再返回消息给center
			proto::common::fight_param fp;
			fp.set_type(proto::common::fight_type_challenge);
			fight_manager_t::fight_pvp(p_challenger, p_target, &fp);
			return;
		}
		else
		{
			//发起方胜利
			reply.set_result(proto::common::challenge_state_win_without_fight);
		}
	}
	else
	{
		if (target_exist)
		{
			//应战方胜利
			reply.set_result(proto::common::challenge_state_lose_without_fight);
		}
		else
		{
			//双方都没到场
			reply.set_result(proto::common::challenge_state_draw);
		}
	}
	env::server->send_msg_to_center(op_cmd::ge_challenge_fight_reply, challenger_uid, reply);
}

bool pk_manager_t::is_safe(role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return true;
	}

	return p_role->get_role_scene_info().is_in_safe_area();
}

bool pk_manager_t::is_criminal(role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return false;
	}
	return p_role->get_pk_value() >= get_criminal_value();
}


bool pk_manager_t::is_criminal(uint32_t val)
{
	return val >= get_criminal_value();
}


bool pk_manager_t::is_protected(role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return false;
	}
	auto protected_level = GET_CONF(Comprehensive, comprehensive_common::pk_protect_lv);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(protected_level);
	return p_role->get_level() <= val;
}

uint32_t pk_manager_t::get_pk_item()
{
	auto pk_item = GET_CONF(Comprehensive, comprehensive_common::pk_use_item_id);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(pk_item);
	return val;
}

uint32_t pk_manager_t::get_pk_item_num()
{
	auto pk_item = GET_CONF(Comprehensive, comprehensive_common::pk_use_item_num);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(pk_item);
	return val;
}

uint32_t pk_manager_t::get_criminal_value()
{
	auto criminal_value = GET_CONF(Comprehensive, comprehensive_common::pk_wanted_value);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(criminal_value);
	return val;
}

uint32_t pk_manager_t::get_max_pk_value()
{
	auto max_pk_value = GET_CONF(Comprehensive, comprehensive_common::pk_value_upper_limit);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(max_pk_value);
	return val;
}

uint32_t pk_manager_t::get_pk_value_add()
{
	auto add_pk_value = GET_CONF(Comprehensive, comprehensive_common::pk_kill_unredname_add_value);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(add_pk_value);
	return val;
}

uint32_t pk_manager_t::get_delta_level()
{
	auto delta_level = GET_CONF(Comprehensive, comprehensive_common::pk_valid_lv);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(delta_level);
	return val;
}

uint32_t pk_manager_t::get_pk_distance()
{
	auto pk_distance = GET_CONF(Comprehensive, comprehensive_common::pk_distance);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(pk_distance);
	return val;
}

uint32_t pk_manager_t::get_pk_value_reduce_time()
{
	auto pk_value_reduce_time = GET_CONF(Comprehensive, comprehensive_common::pk_remove_pk_value_speed);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(pk_value_reduce_time);
	return val;
}


uint32_t pk_manager_t::get_pk_value_reduce()
{
	//auto pk_value_reduce = GET_CONF(Comprehensive, comprehensive_common::treasure_task_id);
	//uint32_t val = GET_COMPREHENSIVE_VALUE_1(pk_value_reduce);
	//return val;
	return 50;
}

uint32_t pk_manager_t::get_pk_count()
{
	auto pk_count = GET_CONF(Comprehensive, comprehensive_common::pk_a_daily_times);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(pk_count);
	return val;
}

uint32_t pk_manager_t::get_target_count()
{
	auto target_count = GET_CONF(Comprehensive, comprehensive_common::pk_b_daily_times);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(target_count);
	return val;
}


uint32_t pk_manager_t::get_challenge_gold()
{
	auto challenge_gold = GET_CONF(Comprehensive, comprehensive_common::pk_battle_a_cost);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(challenge_gold);
	return val;
}


uint32_t pk_manager_t::get_accept_challenge_gold()
{
	auto accept_challenge_gold = GET_CONF(Comprehensive, comprehensive_common::pk_battle_b_cost);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(accept_challenge_gold);
	return val;
}

uint32_t pk_manager_t::get_prison_rate()
{
	auto prison_rate = GET_CONF(Comprehensive, comprehensive_common::pk_be_wanted_caught_odds);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(prison_rate);
	return val;
}

uint32_t pk_manager_t::get_challenge_point()
{
	auto challenge_point = GET_CONF(Comprehensive, comprehensive_common::pk_battle_fight_jump_id);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(challenge_point);
	return val;
}

uint32_t pk_manager_t::get_prison_id()
{
	auto prison_id = GET_CONF(Comprehensive, comprehensive_common::prison_id);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(prison_id);
	return val;
}

uint32_t pk_manager_t::get_prison_point()
{
	auto prison_point = GET_CONF(Comprehensive, comprehensive_common::prison_jump_id);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(prison_point);
	return val;
}

uint32_t pk_manager_t::get_errantry(uint32_t pk_value)
{
	uint32_t val = 0;
	std::map<uint32_t, Pk*> pk_map;
	GET_ALL_CONF(Pk, pk_map);
	for (auto it : pk_map)
	{
		Pk* p_conf = it.second;
		if (NULL == p_conf)
		{
			continue;
		}
		if (p_conf->type() != 1)
		{
			continue;
		}
		uint32_t min_value = p_conf->value_range().first;
		uint32_t max_value = p_conf->value_range().second;
		if (min_value > max_value)
		{
			continue;
		}
		if (pk_value >= min_value && pk_value <= max_value)
		{
			val = p_conf->param();
			break;
		}
	}
	return val;
}

uint32_t pk_manager_t::get_exp_reduce(uint32_t pk_value)
{
	uint32_t val = 0;
	std::map<uint32_t, Pk*> pk_map;
	GET_ALL_CONF(Pk, pk_map);
	for (auto it : pk_map)
	{
		Pk* p_conf = it.second;
		if (NULL == p_conf)
		{
			continue;
		}
		if (p_conf->type() != 2)
		{
			continue;
		}
		uint32_t min_value = p_conf->value_range().first;
		uint32_t max_value = p_conf->value_range().second;
		if (min_value > max_value)
		{
			continue;
		}
		if (pk_value >= min_value && pk_value <= max_value)
		{
			val = p_conf->param();
			break;
		}
	}
	return val;
}

uint32_t pk_manager_t::get_prison_out_point()
{
	auto prison_point = GET_CONF(Comprehensive, comprehensive_common::prison_release_jump_id);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(prison_point);
	return val;
}

uint32_t pk_manager_t::get_attacker_lose_prison_rate(uint32_t pk_value)
{
	uint32_t val = 0;
	std::map<uint32_t, Pk*> pk_map;
	GET_ALL_CONF(Pk, pk_map);
	for (auto it : pk_map)
	{
		Pk* p_conf = it.second;
		if (NULL == p_conf)
		{
			continue;
		}
		if (p_conf->type() != 3)
		{
			continue;
		}
		uint32_t min_value = p_conf->value_range().first;
		uint32_t max_value = p_conf->value_range().second;
		if (min_value > max_value)
		{
			continue;
		}
		if (pk_value >= min_value && pk_value <= max_value)
		{
			val = p_conf->param();
			break;
		}
	}
	return val;
}

uint32_t pk_manager_t::get_defenser_lose_prison_rate(uint32_t pk_value)
{
	uint32_t val = 0;
	std::map<uint32_t, Pk*> pk_map;
	GET_ALL_CONF(Pk, pk_map);
	for (auto it : pk_map)
	{
		Pk* p_conf = it.second;
		if (NULL == p_conf)
		{
			continue;
		}
		if (p_conf->type() != 4)
		{
			continue;
		}
		uint32_t min_value = p_conf->value_range().first;
		uint32_t max_value = p_conf->value_range().second;
		if (min_value > max_value)
		{
			continue;
		}
		if (pk_value >= min_value && pk_value <= max_value)
		{
			val = p_conf->param();
			break;
		}
	}
	return val;
}
