#include "log_wrapper.hpp"
#include "log_unify_save.hpp"
#include "common/common_struct.hpp"
#include "protos_fwd.hpp"

USING_NS_COMMON;

void log_wrapper_t::send_role_login_log(uint64_t role_uid,
                                        uint32_t gate_id,
                                        uint32_t game_id)
{
	if (!env::xml->log_sql.role)
	{
		return;
	}
    proto::log::go_role_login_log_notify msg;
    msg.set_role_uid(role_uid);
    msg.set_gate_id(gate_id);
    msg.set_game_id(game_id);
    msg.set_log_time(common::time_util_t::now_time());
    write_new_log(op_cmd::go_role_login_log_notify, msg);
}

void log_wrapper_t::send_role_logout_log(uint64_t role_uid,
                                         uint32_t online_time,
                                         uint32_t gate_id,
                                         uint32_t game_id,
                                         uint32_t kick_reason)
{
	if (!env::xml->log_sql.role)
	{
		return;
	}
    uint32_t now_time = common::time_util_t::now_time();
    proto::log::go_role_logout_log_notify msg;
    msg.set_role_uid(role_uid);
    msg.set_online_time(online_time);
    msg.set_gate_id(gate_id);
    msg.set_game_id(game_id);
    msg.set_kick_reason(kick_reason);
    msg.set_log_time(now_time);
    write_new_log(op_cmd::go_role_logout_log_notify, msg);
}

void log_wrapper_t::send_item_log(uint64_t role_uid, uint32_t role_level, uint64_t item_uid, uint32_t item_tid,
								  uint32_t old_num, uint32_t up_num, uint32_t new_num, uint32_t op_type,
								  uint32_t source_type, uint32_t source_param)
{
	if (!env::xml->log_sql.item)
	{
		return;
	}
    uint32_t now_time = common::time_util_t::now_time();
    proto::log::go_item_log_notify msg;
	msg.set_role_uid(role_uid);
	msg.set_role_level(role_level);
    msg.set_item_uid(item_uid);
    msg.set_item_tid(item_tid);
    msg.set_old_num(old_num);
    msg.set_up_num(up_num);
    msg.set_new_num(new_num);
    msg.set_op_type(op_type);
	msg.set_source_type(source_type);
	msg.set_source_param(source_param);
    msg.set_log_time(now_time);
    write_new_log(op_cmd::go_item_log_notify, msg);
}

void log_wrapper_t::send_money_log(uint64_t role_uid, uint32_t role_level, uint32_t old_num, uint32_t up_num, 
								   uint32_t new_num, uint32_t money_type, uint32_t op_type,
								   uint32_t source_type, uint32_t source_param)
{
	if (!env::xml->log_sql.money)
	{
		return;
	}
	proto::log::go_money_log_notify msg;
	msg.set_role_uid(role_uid);
	msg.set_role_level(role_level);
	msg.set_old_num(old_num);
	msg.set_up_num(up_num);
	msg.set_new_num(new_num);
	msg.set_op_type(op_type);
	msg.set_money_type(money_type);
	msg.set_source_type(source_type);
	msg.set_source_param(source_param);

	uint32_t now_time = common::time_util_t::now_time();
	msg.set_log_time(now_time);
	write_new_log(op_cmd::go_money_log_notify, msg);
}


void log_wrapper_t::send_pk_log(uint64_t role_uid, uint32_t role_level, uint32_t old_num, uint32_t up_num, uint32_t new_num, uint32_t value_type, uint32_t op_type, uint32_t source_type, uint32_t source_param)
{
	if (!env::xml->log_sql.pk)
	{
		return;
	}
	proto::log::go_pk_log_notify msg;
	msg.set_role_uid(role_uid);
	msg.set_role_level(role_level);
	msg.set_old_num(old_num);
	msg.set_up_num(up_num);
	msg.set_new_num(new_num);
	msg.set_op_type(op_type);
	msg.set_value_type(value_type);
	msg.set_source_type(source_type);
	msg.set_source_param(source_param);

	uint32_t now_time = common::time_util_t::now_time();
	msg.set_log_time(now_time);
	write_new_log(op_cmd::go_pk_log_notify, msg);
}


void log_wrapper_t::send_user_shop_log(uint64_t role_uid, uint32_t goods_tid, uint32_t goods_num, uint32_t cost_type, uint32_t cost_num, uint32_t item_id, uint32_t item_num, uint32_t source_type, uint32_t source_param)
{
	if (!env::xml->log_sql.shop)
	{
		return;
	}
	proto::log::go_shop_log_notify msg;
	msg.set_role_uid(role_uid);
	msg.set_goods_tid(goods_tid);
	msg.set_goods_num(goods_num);
	msg.set_cost_type(cost_type);
	msg.set_cost_value(cost_num);
	msg.set_item_id(item_id);
	msg.set_item_num(item_num);
	msg.set_source_type(source_type);
	msg.set_source_param(source_param);

	uint32_t now_time = common::time_util_t::now_time();
	msg.set_log_time(now_time);
	write_new_log(op_cmd::go_shop_log_notify, msg);
}


void log_wrapper_t::send_random_shop_log(uint64_t role_uid, uint32_t shop_tid, uint32_t goods_tid, uint32_t cost_type, uint32_t cost_value, uint32_t cost_item, uint32_t cost_item_value, uint32_t old_num, uint32_t new_num, uint32_t source_type)
{
	if (!env::xml->log_sql.shop)
	{
		return;
	}
	proto::log::go_random_shop_log_notify msg;
	msg.set_role_uid(role_uid);
	msg.set_shop_tid(shop_tid);
	msg.set_goods_tid(goods_tid);
	msg.set_cost_type(cost_type);
	msg.set_cost_value(cost_value);
	msg.set_cost_item(cost_item);
	msg.set_cost_item_value(cost_item_value);
	msg.set_old_num(old_num);
	msg.set_new_num(new_num);
	msg.set_source_type(source_type);

	uint32_t now_time = common::time_util_t::now_time();
	msg.set_log_time(now_time);
	write_new_log(op_cmd::go_random_shop_log_notify, msg);
}

void log_wrapper_t::send_equip_log(uint64_t role_uid, uint32_t role_level, item_ptr p_equip, uint32_t op_type, uint32_t source_type, uint32_t source_param)
{
	if (!env::xml->log_sql.equip)
	{
		return;
	}
	if (NULL == p_equip)
	{
		log_error("NULL == p_equip");
		return;
	}
	proto::log::go_equip_log_notify msg;
	p_equip->fill_equip_log(msg);
	msg.set_role_uid(role_uid);
	msg.set_role_level(role_level);
	msg.set_op_type(op_type);
	msg.set_source_type(source_type);
	msg.set_source_param(source_param);
	uint32_t now_time = common::time_util_t::now_time();
	msg.set_log_time(now_time);
	write_new_log(op_cmd::go_equip_log_notify, msg);
}

void log_wrapper_t::send_chart_success_log(uint64_t role_uid, uint32_t role_level, const std::string& pay_channel,
										   uint32_t yuanbao, uint32_t free_yuanbao, uint32_t new_yuanbao, uint32_t cash, \
										   std::string &chart_order, std::string &game_chart_order, 
										   proto::common::mobile_info& mobile)
{

}

void log_wrapper_t::send_hero_log(uint64_t role_uid, uint32_t role_level, uint64_t hero_uid, uint32_t hero_tid, uint32_t op_type,
								  uint32_t source_type, uint32_t old_num, uint32_t up_num, uint32_t new_num, uint32_t source_param)
{
	if (!env::xml->log_sql.hero)
	{
		return;
	}
	proto::log::go_hero_log_notify msg;
	msg.set_role_uid(role_uid);
	msg.set_role_level(role_level);
	msg.set_hero_uid(hero_uid);
	msg.set_hero_tid(hero_tid);
	msg.set_op_type(op_type);
	msg.set_source_type(source_type);
	msg.set_old_num(old_num);
	msg.set_up_num(up_num);
	msg.set_new_num(new_num);
	msg.set_source_param(source_param);
	uint32_t now_time = common::time_util_t::now_time();
	msg.set_log_time(now_time);
	write_new_log(op_cmd::go_hero_log_notify, msg);
}

void log_wrapper_t::send_task_log(uint64_t role_uid, uint32_t task_id, uint32_t task_type, uint32_t task_state, const std::string& progress, 
									uint32_t accept_time, uint32_t op_type)
{
	if (!env::xml->log_sql.task)
	{
		return;
	}
	proto::log::go_task_log_notify msg;
	msg.set_role_uid(role_uid);
	msg.set_task_id(task_id);
	msg.set_task_type(task_type);
	msg.set_task_state(task_state);
	msg.set_task_progress(progress);
	msg.set_accept_time(accept_time);
	msg.set_op_type(op_type);
	msg.set_log_time(common::time_util_t::now_time());
	
	write_new_log(op_cmd::go_task_log_notify, msg);
}

void log_wrapper_t::send_prestige_log(uint64_t role_uid, uint32_t role_level, uint32_t old_prestige, uint32_t old_prestige_total, uint32_t old_prestige_level, 
										uint32_t up_prestige, uint32_t new_prestige, uint32_t new_prestige_total, uint32_t new_prestige_level, 
										uint32_t op_type, uint32_t source_type, uint32_t source_param)
{
	if (!env::xml->log_sql.money)
	{
		return;
	}
	uint32_t now_time = common::time_util_t::now_time();
	proto::log::go_prestige_log_notify msg;
	msg.set_role_uid(role_uid);
	msg.set_role_level(role_level);
	msg.set_old_prestige(old_prestige);
	msg.set_old_prestige_total(old_prestige_total);
	msg.set_old_prestige_level(old_prestige_level);
	msg.set_up_prestige(up_prestige);
	msg.set_new_prestige(new_prestige);
	msg.set_new_prestige_total(new_prestige_total);
	msg.set_new_prestige_level(new_prestige_level);
	msg.set_op_type(op_type);
	msg.set_source_type(source_type);
	msg.set_source_param(source_param);
	msg.set_log_time(now_time);
	write_new_log(op_cmd::go_prestige_log_notify, msg);
}

void log_wrapper_t::send_achieve_log(uint64_t role_uid, uint32_t achieve_tid, uint32_t old_level, 
									 uint32_t up_level,uint32_t new_level, uint32_t old_count, 
									uint32_t new_count, uint32_t old_achieve_point,
									uint32_t add_achieve_point, uint32_t new_achieve_point, 
									uint32_t old_achieve_level, uint32_t up_achieve_level, 
									uint32_t new_achieve_level, uint32_t old_reward_get_level,
									uint32_t new_reward_get_level,uint32_t source_type, 
									uint32_t source_param,const std::string& source_src)
{
	if (!env::xml->log_sql.achieve)
	{
		return;
	}
	uint32_t now_time = common::time_util_t::now_time();
	proto::log::go_achieve_log_notify msg;
	msg.set_role_uid(role_uid);
	msg.set_achieve_id(achieve_tid);
	msg.set_old_level(old_level);
	msg.set_up_level(up_level);
	msg.set_new_level(new_level);
	msg.set_old_count(old_count);
    msg.set_up_count(0); //取消不用
    msg.set_new_count(new_count);
	msg.set_old_achieve_point(old_achieve_point);
	msg.set_add_achieve_point(add_achieve_point);
	msg.set_new_achieve_point(new_achieve_point);
	msg.set_old_achieve_level(old_achieve_level);
	msg.set_up_achieve_level(up_achieve_level);
	msg.set_new_achieve_level(new_achieve_level);
	msg.set_old_reward_get_level(old_reward_get_level);
	msg.set_new_reward_get_level(new_reward_get_level);
	msg.set_source_type(source_type);
	msg.set_source_param(source_param);
	msg.set_sourec_type_str(source_src);
	msg.set_log_time(now_time);
	write_new_log(op_cmd::go_achieve_log_notify, msg);
}

void log_wrapper_t::send_lifeforce_log(uint64_t role_uid, uint32_t old_lifestate_level, uint32_t old_lifestar_level,
										uint32_t up_lifestate, uint32_t up_lifestar, uint32_t new_lifestate_level ,
										uint32_t new_lifestar_level, uint32_t source_type, uint32_t source_param)
{
	if (!env::xml->log_sql.lifeforce)
	{
		return;
	}
	uint32_t now_time = common::time_util_t::now_time();
	proto::log::go_lifeforce_log_notify msg;
	msg.set_role_uid(role_uid);
	msg.set_old_lifestate_level(old_lifestate_level);
	msg.set_old_lifestar_level(old_lifestar_level);
	msg.set_up_lifestate(up_lifestate);
	msg.set_up_lifestar(up_lifestar);
	msg.set_new_lifestate_level(new_lifestate_level);
	msg.set_new_lifestar_level(new_lifestar_level);
	msg.set_source_type(source_type);
	msg.set_source_param(source_param);
	msg.set_log_time(now_time);
	write_new_log(op_cmd::go_lifeforce_log_notify, msg);
}

void log_wrapper_t::send_dungeon_log(uint64_t role_uid, 
	uint32_t dungeon_id, uint32_t create_type, uint64_t create_id, 
	uint32_t source_type, uint32_t source_param)
{
	if (!env::xml->log_sql.dungeon)
	{
		return;
	}
	proto::log::go_dungeon_log_notify msg;
	msg.set_role_uid(role_uid);
	msg.set_dungeon_id(dungeon_id);
	msg.set_create_type(create_type);
	msg.set_create_id(create_id);
	msg.set_source_type(source_type);
	msg.set_source_param(source_param);

	uint32_t now_time = common::time_util_t::now_time();
	msg.set_log_time(now_time);
	write_new_log(op_cmd::go_dungeon_log_notify, msg);
}

void log_wrapper_t::send_role_buff_log(uint64_t role_uid, uint32_t buff_id, uint32_t op_type, uint32_t speed, uint32_t source_type, uint32_t source_param)
{
	if (!env::xml->log_sql.role_buff)
	{
		return;
	}
	proto::log::go_role_buff_log_notify msg;
	msg.set_role_uid(role_uid);
	msg.set_buff_id(buff_id);
	msg.set_op_type(op_type);
	msg.set_speed(speed);
	msg.set_source_type(source_type);
	msg.set_source_param(source_param);

	uint32_t now_time = common::time_util_t::now_time();
	msg.set_log_time(now_time);
	write_new_log(op_cmd::go_role_buff_log_notify, msg);
}

void log_wrapper_t::send_new_guide_log(uint64_t role_uid, uint32_t new_guide_id, uint32_t source_type, uint32_t source_param)
{
	if (!env::xml->log_sql.new_guide)
	{
		return;
	}
	proto::log::go_role_new_guide_log_notify msg;
	msg.set_role_uid(role_uid);
	msg.set_new_guide_id(new_guide_id);
	msg.set_source_type(source_type);
	msg.set_source_param(source_param);

	uint32_t now_time = common::time_util_t::now_time();
	msg.set_log_time(now_time);
	write_new_log(op_cmd::go_role_new_guide_log_notify, msg);
}

void log_wrapper_t::send_trade_log(uint64_t role_uid, uint32_t item_tid,uint32_t price,uint32_t num,uint32_t tax,
	uint32_t total_num, uint32_t total_cost, uint32_t gain,uint32_t trade_id,uint32_t op_type,uint32_t source_type)
{
	if (!env::xml->log_sql.trade)
	{
		return;
	}
	proto::log::go_role_trade_log_notify msg;
	msg.set_role_uid(role_uid);
	msg.set_item_tid(item_tid);	
	msg.set_price(price);
	msg.set_num(num);
	msg.set_tax(tax);
	msg.set_total_num(total_num);
	msg.set_total_cost(total_cost);
	msg.set_gain(gain);
	msg.set_trade_id(trade_id);
	msg.set_op_type(op_type);
	msg.set_source_type(source_type);

	uint32_t now_time = common::time_util_t::now_time();
	msg.set_log_time(now_time);
	write_new_log(op_cmd::go_role_trade_log_notify, msg);

}

void log_wrapper_t::send_recharge_log(uint64_t role_uid, const std::string& order_id,uint32_t recharge_tid,
									uint32_t recharge_rmb, uint32_t channel_id)
{
	if (!env::xml->log_sql.money)
	{
		return;
	}
	proto::log::go_role_recharge_log_notify msg;
	msg.set_role_uid(role_uid);
	msg.set_order_id(order_id);
	msg.set_recharge_tid(recharge_tid);
	msg.set_recharge_rmb(recharge_rmb);
	msg.set_channel_id(channel_id);

	uint32_t now_time = common::time_util_t::now_time();
	msg.set_log_time(now_time);
	write_new_log(op_cmd::go_role_recharge_log_notify, msg);
}


void log_wrapper_t::send_rmb_log(uint64_t role_uid, uint32_t role_level, uint32_t old_num, uint32_t up_num, uint32_t new_num, uint32_t op_type, uint32_t source_type, uint32_t source_param)
{
	if (!env::xml->log_sql.money)
	{
		return;
	}
	proto::log::go_rmb_log_notify msg;
	msg.set_role_uid(role_uid);
	msg.set_role_level(role_level);
	msg.set_old_num(old_num);
	msg.set_up_num(up_num);
	msg.set_new_num(new_num);
	msg.set_op_type(op_type);
	msg.set_source_type(source_type);
	msg.set_source_param(source_param);

	uint32_t now_time = common::time_util_t::now_time();
	msg.set_log_time(now_time);
	write_new_log(op_cmd::go_rmb_log_notify, msg);
}

void log_wrapper_t::send_expedition_log(uint64_t role_uid, uint32_t role_level, uint32_t source_type, uint32_t source_param)
{
	if (!env::xml->log_sql.expedition)
	{
		return;
	}
	proto::log::go_expedition_log_notify msg;
	msg.set_role_uid(role_uid);
	msg.set_role_level(role_level);
	msg.set_source_type(source_type);
	msg.set_source_param(source_param);

	uint32_t now_time = common::time_util_t::now_time();
	msg.set_log_time(now_time);
	write_new_log(op_cmd::go_expedition_log_notify, msg);
}

void log_wrapper_t::send_role_exp_log(uint64_t role_uid, uint32_t old_level, uint32_t old_exp, uint32_t up_exp, uint32_t new_level, uint32_t new_exp, uint32_t source_type, uint32_t source_param)
{
	if (!env::xml->log_sql.role)
	{
		return;
	}
	proto::log::go_exp_log_notify msg;
	msg.set_role_uid(role_uid);
	msg.set_old_level(old_level);
	msg.set_old_exp(old_exp);
	msg.set_up_exp(up_exp);
	msg.set_new_level(new_level);
	msg.set_new_exp(new_exp);
	msg.set_source_type(source_type);
	msg.set_source_param(source_param);

	uint32_t now_time = common::time_util_t::now_time();
	msg.set_log_time(now_time);
	write_new_log(op_cmd::go_exp_log_notify, msg);
}

void log_wrapper_t::send_role_combat_log(uint64_t role_uid, uint32_t role_level, uint32_t fight_type, uint32_t combat_type,
	const std::vector<uint64_t>& team_member, const std::vector<uint64_t>& targets, uint64_t fight_uid, uint32_t is_win,
	uint32_t start_time, uint32_t finish_time)
{
	if (!env::xml->log_sql.fight)
	{
		return;
	}
	proto::log::go_combat_log_notify msg;
	msg.set_role_uid(role_uid);
	msg.set_role_level(role_level);
	msg.set_fight_type(fight_type);
	msg.set_combat_type(combat_type);
	for (uint32_t i = 0; i < team_member.size(); ++i)
	{
		msg.add_team_members(team_member[i]);
	}
	for (uint32_t i = 0; i < targets.size(); ++i)
	{
		msg.add_targets(targets[i]);
	}
	msg.set_fight_uid(fight_uid);
	msg.set_start_time(start_time);
	msg.set_fight_time(finish_time-start_time);
	msg.set_finish_time(finish_time);
	msg.set_is_win(is_win);

	uint32_t now_time = common::time_util_t::now_time();
	msg.set_log_time(now_time);
	write_new_log(op_cmd::go_combat_log_notify, msg);
}

void log_wrapper_t::send_hero_exp_log(uint64_t role_uid, uint32_t role_level, uint64_t hero_uid, 
	uint32_t hero_tid, uint32_t old_level, uint32_t old_exp, uint32_t up_exp, uint32_t new_level, 
	uint32_t new_exp, uint32_t source_type, uint32_t source_param)
{
	if (!env::xml->log_sql.hero)
	{
		return;
	}
	proto::log::go_hero_exp_log_notify msg;
	msg.set_role_uid(role_uid);
	msg.set_role_level(role_level);
	msg.set_hero_uid(hero_uid);
	msg.set_hero_tid(hero_tid);
	msg.set_old_level(old_level);
	msg.set_old_exp(old_exp);
	msg.set_up_exp(up_exp);
	msg.set_new_level(new_level);
	msg.set_new_exp(new_exp);
	msg.set_source_type(source_type);
	msg.set_source_param(source_param);

	uint32_t now_time = common::time_util_t::now_time();
	msg.set_log_time(now_time);
	write_new_log(op_cmd::go_hero_exp_log_notify, msg);
}

void log_wrapper_t::send_patrol_hire_log(uint64_t role_uid, uint32_t begin_time, uint32_t end_time, 
	uint32_t count, uint32_t total_exp, uint32_t use_multi_point, uint32_t map_id, uint32_t source_type)
{
	if (!env::xml->log_sql.patrol_hire)
	{
		return;
	}
	proto::log::go_patrol_hire_log_notify msg;
	msg.set_role_uid(role_uid);
	msg.set_begin_time(begin_time);
	msg.set_end_time(end_time);
	msg.set_count(count);
	msg.set_total_exp(total_exp);
	msg.set_use_multi_point(use_multi_point);
	msg.set_map_id(map_id);
	msg.set_source_type(source_type);

	uint32_t now_time = common::time_util_t::now_time();
	msg.set_log_time(now_time);
	write_new_log(op_cmd::go_patrol_hire_log_notify, msg);
}

