#include "money_manager.hpp"
#include "log/log_wrapper.hpp"
#include "role/role_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "config_mgr.h"
#include "tblh/Prestige.tbls.h"
#include <time.h>
#include "tblh/Recharge.tbls.h"
#include "global_id.hpp"
#include "common/utility.hpp"
#include "achieve/achieve_common.hpp"
#include "rank/rank_manager.hpp"
#include "tblh/rank_common.hpp"
//#include "achieve/achieve_manager.hpp"
#include "sys_notice/sys_notice_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "rank/rank_manager.hpp"
USING_NS_COMMON;


uint32_t money_manager_t::get_money(role_ptr p_role, proto::common::MONEY_TYPE type)
{
	if (NULL == p_role) {
		log_error("NULL == p_role");
		return 0;
	}

	if (type == proto::common::MONEY_TYPE_NONE || type >= proto::common::MONEY_TYPE_END) {
		log_error("role[%lu] money_type invalid", p_role->get_uid());
		return 0;
	}

	switch (type) {
		case proto::common::MONEY_TYPE_VIGOUR: {
			return p_role->get_vigour();
			break;
		}
		default: {
			money_ptr p_money = p_role->get_money();
			if (NULL != p_money) {
				return p_money->get_money(type);
			}
		}
	}

	return 0;
}

bool money_manager_t::add_money(role_ptr p_role, proto::common::MONEY_TYPE type, uint32_t count, uint32_t source_type, uint32_t source_param, bool notice , proto::common::role_change_data* p_data,bool is_notify)
{
	if (NULL == p_role) {
		log_error("NULL == p_role");
		return false;
	}

	if (count == 0) {
		//log_warn("count = 0 role_uid[%lu] money_type[%d] source_type[%d]", p_role->get_uid(), type, source_type);
		return false;
	}

    if (type == proto::common::MONEY_TYPE_NONE || type >= proto::common::MONEY_TYPE_END) {
        log_error("role[%lu] money_type invalid", p_role->get_uid());
        return false;
    }

	if (type == proto::common::MONEY_TYPE_ENERGY)
	{
		uint32_t energy_limit = get_energy_limit();
		uint32_t cur_energy = get_money(p_role, type);
		if (cur_energy >= energy_limit)
		{
			MONEY_LOG("role[%lu] energy[%d] full limit[%d] ", p_role->get_uid(), cur_energy, energy_limit);
			return false;
		}
	}

	switch (type) {
		case proto::common::MONEY_TYPE_PRESTIGE: {
			add_prestige(p_role, count, source_type, source_param);
			break;
		}
		case proto::common::MONEY_TYPE_ACHIEVE_POINT: {
			add_achieve_point(p_role, count, source_type, source_param, p_data);
			break;
		}
		case proto::common::MONEY_TYPE_VIGOUR: {
			p_role->add_vigour(count, source_type, source_param);
			break;
		}
		case proto::common::MONEY_TYPE_FAMILY_FUNDS: {
			p_role->add_family_money(count, source_type, source_param);
			// 直接返回true
			return true;
		}
		case proto::common::MONEY_TYPE_COUNTRY_FUNDS: {
			p_role->add_country_money(count, source_type, source_param);
			// 直接返回true
			return true;
		}
		default: {
			money_ptr p_money = p_role->get_money();
			if (NULL == p_money) {
				log_error("role[%lu] p_money == NULL", p_role->get_uid());
				return false;
			}

			uint32_t old_money = p_money->get_money(type);
			uint32_t new_money = old_money + count;
			p_money->set_money(type, new_money);

			// 这里都不赋值了，统一在各自的外层赋值
			if (p_data) p_role->get_personal_info(p_data->mutable_per_info());
			if (type == proto::common::MONEY_TYPE_LIFE_STONE)
			{
				log_wrapper_t::send_rmb_log(p_role->get_uid(), p_role->get_level(), old_money, count, new_money, log_enum::opt_type_add, source_type, source_param);
			}
			else
			{
				log_wrapper_t::send_money_log(p_role->get_uid(), p_role->get_level(), old_money, count, new_money, type, log_enum::opt_type_add, source_type, source_param);
			}
			
			if (type == proto::common::MONEY_TYPE_COPPER)
			{
				if (is_notify)
				{
					//成就完成立即通知
					achieve_common_t::notify_progress_state(p_role->get_uid(), proto::common::Achieve_Event_Type::ACHIEVE_PACKAGE_COPPER_NUMBER, 0, new_money);
					achieve_common_t::notify_progress_state(p_role->get_uid(), proto::common::Achieve_Event_Type::ACHIEVE_TOTAL_COPPER, 0, count);
				}

			}
			MONEY_LOG("role[%lu] add money[%d:%d] from[%d] to [%d]", p_role->get_uid(), type, count, old_money, new_money);
		}
	}

	if (notice) sys_notice_manager_t::broadcast_system_msg_for_money(p_role->get_uid(), type, count);
	p_role->save_self();
	return true;
}

bool money_manager_t::use_money(role_ptr p_role, proto::common::MONEY_TYPE type, uint32_t count, uint32_t source_type, uint32_t source_param, bool notice /*= true*/, proto::common::role_change_data* p_data)
{
	if (NULL == p_role) {
		log_error("NULL == p_role");
		return false;
	}

    if (type == proto::common::MONEY_TYPE_NONE || type >= proto::common::MONEY_TYPE_END) {
        log_error("role[%lu] money_type invalid", p_role->get_uid());
        return false;
    }

	switch (type) {
		case proto::common::MONEY_TYPE_PRESTIGE: {
			use_prestige(p_role, count, source_type, source_param);
			break;
		}
		case proto::common::MONEY_TYPE_VIGOUR: {
			if (!p_role->user_vigour(count, source_type, source_param)) {
				return false;
			}
			break;
		}
		default: {
			money_ptr p_money = p_role->get_money();
			if (NULL == p_money) {
				log_error("role[%lu] p_money == NULL", p_role->get_uid());
				return false;
			}
			uint32_t old_money = p_money->get_money(type);
			if (old_money < count)
			{
				return false;
			}
			uint32_t new_money = old_money - count;
			p_money->set_money(type, new_money);

			// 这里都不赋值了，统一在各自的外层赋值
			if (p_data) p_role->get_personal_info(p_data->mutable_per_info());
			if (type == proto::common::MONEY_TYPE_LIFE_STONE)
			{
				log_wrapper_t::send_rmb_log(p_role->get_uid(), p_role->get_level(), old_money, count, new_money, log_enum::opt_type_del, source_type, source_param);
			}
			else
			{
				log_wrapper_t::send_money_log(p_role->get_uid(), p_role->get_level(), old_money, count, new_money, type, log_enum::opt_type_del, source_type, source_param);
			}
			if (type == proto::common::MONEY_TYPE_COPPER)
			{
				rank_manager_t::update_copper(p_role->get_uid(), count);
			}
			else if(type == proto::common::MONEY_TYPE_YUANBAO)
			{
				rank_manager_t::update_gold(p_role->get_uid(), count);
			}
			
			MONEY_LOG("role[%lu] use money[%d:%d] from[%d] to [%d]", p_role->get_uid(), type, count, old_money, new_money);
		}
	}

	p_role->save_self();
    return true;
}

bool money_manager_t::exchange_money(role_ptr p_role, proto::common::MONEY_TYPE source_type, uint32_t count, proto::common::MONEY_TYPE dest_type, proto::common::role_change_data* p_data)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return false;
	}
	money_ptr p_money = p_role->get_money();
	if (NULL == p_money)
	{
		log_error("role[%lu] p_money == NULL", p_role->get_uid());
		return false;
	}
	if (source_type != proto::common::MONEY_TYPE_YUANBAO &&
		source_type != proto::common::MONEY_TYPE_SILVER &&
		source_type != proto::common::MONEY_TYPE_LIFE_STONE)
	{
		log_error("role[%lu] money_type invalid source_type[%d] dest_type[%d]", p_role->get_uid(), source_type, dest_type);
		return false;
	}
	if (dest_type < proto::common::MONEY_TYPE_YUANBAO || dest_type > proto::common::MONEY_TYPE_COPPER)
	{
		log_error("role[%lu] money_type invalid source_type[%d] dest_type[%d] ", p_role->get_uid(), source_type, dest_type);
		return false;
	}

	auto conf1 = GET_CONF(Comprehensive, comprehensive_common::mingshi_equal_yuanbao);
	auto conf2 = GET_CONF(Comprehensive, comprehensive_common::mingshi_equal_yinding);
	auto conf3 = GET_CONF(Comprehensive, comprehensive_common::yuanbao_equal_tongbi);
	auto conf4 = GET_CONF(Comprehensive, comprehensive_common::yinding_equal_tongbi);
	uint32_t mingshi_equal_yuanbao  = GET_COMPREHENSIVE_VALUE_1(conf1);
	uint32_t mingshi_equal_yinding  = GET_COMPREHENSIVE_VALUE_1(conf2);
	uint32_t yuanbao_equal_tongbi   = GET_COMPREHENSIVE_VALUE_1(conf3);
	uint32_t yinding_equal_tongbi   = GET_COMPREHENSIVE_VALUE_1(conf4);

	uint32_t dest_count = 0;
	switch (source_type)
	{
	case proto::common::MONEY_TYPE_LIFE_STONE:
	{
		if (proto::common::MONEY_TYPE_SILVER == dest_type)
		{
			if (use_money(p_role, proto::common::MONEY_TYPE(source_type), count, log_enum::source_type_role_exchange, 0, true, p_data))
			{
				dest_count = count * mingshi_equal_yinding;
				add_money(p_role, proto::common::MONEY_TYPE(dest_type), dest_count, log_enum::source_type_role_exchange, 0, true, p_data);
			}
		}
		else if (proto::common::MONEY_TYPE_YUANBAO == dest_type)
		{
			if (use_money(p_role, proto::common::MONEY_TYPE(source_type), count, log_enum::source_type_role_exchange, 0, true, p_data))
			{
				dest_count = count * mingshi_equal_yuanbao;
				add_money(p_role, proto::common::MONEY_TYPE(dest_type), dest_count, log_enum::source_type_role_exchange, 0, true, p_data);
			}
		}
	}break;
	case proto::common::MONEY_TYPE_YUANBAO:
	{
		if (proto::common::MONEY_TYPE_COPPER == dest_type)
		{
			if (use_money(p_role, proto::common::MONEY_TYPE(source_type), count, log_enum::source_type_role_exchange, 0, true, p_data))
			{
				dest_count = count * yuanbao_equal_tongbi;
				add_money(p_role, proto::common::MONEY_TYPE(dest_type), dest_count, log_enum::source_type_role_exchange, 0, true, p_data);
			}
		}
	}break;
	case proto::common::MONEY_TYPE_SILVER:
	{
		if (proto::common::MONEY_TYPE_COPPER == dest_type)
		{
			if (use_money(p_role, proto::common::MONEY_TYPE(source_type), count, log_enum::source_type_role_exchange, 0, true, p_data))
			{
				dest_count = count * yinding_equal_tongbi;
				add_money(p_role, proto::common::MONEY_TYPE(dest_type), dest_count, log_enum::source_type_role_exchange, 0, true, p_data);
			}
		}
	}break;
	default:
		break;
	}
	return true;
}

void money_manager_t::add_luck_gift_gold(role_ptr p_role, uint32_t not_received_gold)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}
	money_ptr p_money = p_role->get_money();
	if (NULL == p_money)
	{
		log_error("role[%lu] p_money == NULL", p_role->get_uid());
		return;
	}
	uint32_t old_money = p_money->get_luck_gift_gold();
	uint32_t new_money = old_money + not_received_gold;
	p_money->set_luck_gift_gold(new_money);
	p_role->save_self();
}

void money_manager_t::subtract_luck_gift_gold(role_ptr p_role, uint32_t not_received_gold)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}
	money_ptr p_money = p_role->get_money();
	if (NULL == p_money)
	{
		log_error("role[%lu] p_money == NULL", p_role->get_uid());
		return;
	}
	uint32_t old_money = p_money->get_luck_gift_gold();
	uint32_t new_money = 0;
	if (old_money > not_received_gold)
	{
		new_money = old_money - not_received_gold;
	}
	p_money->set_luck_gift_gold(new_money);
	p_role->save_self();
}

void money_manager_t::add_prestige(role_ptr p_role, uint32_t count, uint32_t source_type, uint32_t source_param)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}
	money_ptr p_money = p_role->get_money();
	if (NULL == p_money)
	{
		log_error("role[%lu] p_money == NULL", p_role->get_uid());
		return;
	}
	uint32_t old_prestige = p_money->get_prestige();
	uint32_t old_prestige_total = p_money->get_prestige_total();
	uint32_t old_prestige_level = p_money->get_prestige_level();
	uint32_t prestige_today = p_money->get_prestige_today();
	auto pres_conf = GET_CONF(Prestige, old_prestige_level);
	if (NULL == pres_conf)
	{
		log_error("role[%lu] prestige_level invalid, FindCnf false", p_role->get_uid());
		return;
	}
	int prestige_limit = pres_conf->prestige_limit() - prestige_today;
	if (prestige_limit <= 0)
	{
		log_error("role[%lu] prestige_limit", p_role->get_uid());
		return;
	}
	if (source_type != log_enum::source_type_gm_add_prestige && count > (uint32_t)prestige_limit)
	{
		count = prestige_limit;
	}
	uint32_t new_prestige = old_prestige + count;
	uint32_t new_prestige_total = old_prestige_total + count;
	if (source_type != log_enum::source_type_gm_add_prestige)
	{
		prestige_today += count;
	}
	
	p_money->set_prestige(new_prestige);
	p_money->set_prestige_total(new_prestige_total);
	p_money->set_prestige_today(prestige_today);
	p_money->calc_prestige_level();
	
	uint32_t new_prestige_level = p_money->get_prestige_level();

	log_wrapper_t::send_prestige_log(p_role->get_uid(), p_role->get_level(), old_prestige, old_prestige_total, old_prestige_level, count,
		new_prestige, new_prestige_total, new_prestige_level, log_enum::opt_type_add, source_type, source_param);
	MONEY_LOG("role[%lu] add money[%d:%d] from[%d] to [%d]", p_role->get_uid(), proto::common::MONEY_TYPE_PRESTIGE, count, old_prestige, new_prestige);
}

bool money_manager_t::use_prestige(role_ptr p_role, uint32_t count, uint32_t source_type, uint32_t source_param)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return false;
	}
	money_ptr p_money = p_role->get_money();
	if (NULL == p_money)
	{
		log_error("role[%lu] p_money == NULL", p_role->get_uid());
		return false;
	}
	uint32_t old_prestige = p_money->get_prestige();
	uint32_t old_prestige_total = p_money->get_prestige_total();
	uint32_t old_prestige_level = p_money->get_prestige_level();
	if (old_prestige < count)
	{
		return false;
	}
	uint32_t new_prestige = old_prestige - count;
	p_money->set_prestige(new_prestige);

	log_wrapper_t::send_prestige_log(p_role->get_uid(), p_role->get_level(), old_prestige, old_prestige_total, old_prestige_level, count,
		new_prestige, old_prestige_total, old_prestige_level, log_enum::opt_type_del, source_type, source_param);
	MONEY_LOG("role[%lu] use money[%d:%d] from[%d] to [%d]", p_role->get_uid(), proto::common::MONEY_TYPE_PRESTIGE, count, old_prestige, new_prestige);
	return true;
}

uint32_t money_manager_t::get_prestige_level(role_ptr p_role)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return 0;
	}
	money_ptr p_money = p_role->get_money();
	if (NULL == p_money)
	{
		log_error("role[%lu] p_money == NULL", p_role->get_uid());
		return 0;
	}
	return p_money->get_prestige_level();
}


void money_manager_t::add_achieve_point(role_ptr p_role, uint32_t count, uint32_t source_type, uint32_t source_param, proto::common::role_change_data* p_data)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}
	money_ptr p_money = p_role->get_money();
	if (NULL == p_money)
	{
		log_error("role[%lu] p_money == NULL", p_role->get_uid());
		return;
	}
	ACHIEVE_LOG("role[%lu], begin add achieve point ", p_role->get_uid());
	
	uint32_t old_achieve_point = p_money->get_achieve_point();

	uint32_t new_achieve_point = old_achieve_point + count;

	p_money->set_achieve_point(new_achieve_point);

	ACHIEVE_LOG("role[%lu],because achieve[%d],add[%d] achieve point, old count/new count[%d/%d] ", p_role->get_uid(), source_param,count,old_achieve_point,new_achieve_point);

	achieve_manager_t::add_level(p_role->get_uid());
	if (NULL != p_data)
	{
		proto::common::personal_info* p_personal_data = p_data->mutable_per_info();
		if (NULL != p_personal_data)
		{
			p_personal_data->set_achieve_level(p_role->get_achieve_level());
			p_personal_data->set_achieve_recv_reward_level(p_role->get_achieve_recv_reward_level());
			p_personal_data->set_achieve_point(money_manager_t::get_money(p_role, proto::common::MONEY_TYPE_ACHIEVE_POINT));
		}
	}

	log_wrapper_t::send_money_log(p_role->get_uid(), p_role->get_level(), old_achieve_point, count, new_achieve_point, proto::common::MONEY_TYPE_ACHIEVE_POINT, log_enum::opt_type_add, source_type, source_param);

	//成就排行
	rank_manager_t::update_achievement(p_role->get_uid(), new_achieve_point, p_role->get_achieve_level());
	MONEY_LOG("role[%lu] add money[%d:%d] from[%d] to [%d]", p_role->get_uid(), proto::common::MONEY_TYPE_ACHIEVE_POINT, count, old_achieve_point, new_achieve_point);
}


void money_manager_t::add_honor(role_ptr p_role, uint32_t count, uint32_t source_type, uint32_t source_param, proto::common::role_change_data* p_data /*= NULL*/)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}
	money_ptr p_money = p_role->get_money();
	if (NULL == p_money)
	{
		log_error("role[%lu] p_money == NULL", p_role->get_uid());
		return;
	}
	uint32_t old_honor = p_money->get_money(proto::common::MONEY_TYPE_HONOR);
	uint32_t honor_week = p_money->get_honor_week();
	uint32_t honor_limit = p_money->get_honor_limit();

	if (honor_limit <= honor_week)
	{
		log_error("role[%lu] honor_week_limit[%d]", p_role->get_uid(), honor_limit);
		return;
	}
	if (count > honor_limit - honor_week)
	{
		count = honor_limit - honor_week;
	}
	uint32_t new_honor = old_honor + count;

	p_money->set_money(proto::common::MONEY_TYPE_HONOR, new_honor);
	if (source_type != log_enum::source_type_gm_add_money)
	{
		p_money->set_honor_week(honor_week + count);
	}

	log_wrapper_t::send_money_log(p_role->get_uid(), p_role->get_level(), old_honor, count, new_honor,
		proto::common::MONEY_TYPE_HONOR, log_enum::opt_type_add, source_type, source_param);

	MONEY_LOG("role[%lu] add money[%d:%d] from[%d] to [%d]", p_role->get_uid(), proto::common::MONEY_TYPE_HONOR, count, old_honor, new_honor);
}

void money_manager_t::add_friendship_point(role_ptr p_role, uint32_t amount, uint32_t source_type, uint32_t source_param, proto::common::role_change_data * p_data)
{
	if (NULL == p_role)
	{
		log_error("NULL == p_role");
		return;
	}
	money_ptr p_money = p_role->get_money();
	if (NULL == p_money)
	{
		log_error("role[%lu] p_money == NULL", p_role->get_uid());
		return;
	}
	uint32_t old_value = p_money->get_money(proto::common::MONEY_TYPE_FRIENDSHIP_POINT);

	uint32_t new_value = old_value + amount;

	p_money->set_money(proto::common::MONEY_TYPE_FRIENDSHIP_POINT, new_value);
	if (source_type != log_enum::source_type_gm_add_money)
	{
		//p_money->set_honor_week(honor_week + count);
	}

	log_wrapper_t::send_money_log(p_role->get_uid(), p_role->get_level(), old_value, amount, new_value,
		proto::common::MONEY_TYPE_FRIENDSHIP_POINT, log_enum::opt_type_add, source_type, source_param);

	MONEY_LOG("role[%lu] add money[%d:%d] from[%d] to [%d]", p_role->get_uid(), proto::common::MONEY_TYPE_FRIENDSHIP_POINT, amount, old_value, new_value);
}


uint32_t money_manager_t::get_energy_limit()
{
	auto energy_limit = GET_CONF(Comprehensive, comprehensive_common::vitality_max);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(energy_limit);
	return val;
}

uint32_t money_manager_t::role_buy_vigour(const role_ptr& p_role)
{
	if (p_role == NULL) {
		log_error("p_role null error");
		return common::errcode_enum::notice_unknown;
	}

	uint32_t curr_vigour = p_role->get_vigour();
	uint32_t max_vigour = p_role->get_vigour_max_limit();
	if (curr_vigour >= max_vigour) {
		ROLE_LOG("role[%lu] curr_vigour[%u] >= max[%u]", p_role->get_uid(), curr_vigour, max_vigour);
		return common::errcode_enum::notice_vigour_too_much;
	}

	uint32_t buy_times = p_role->get_vigour_buy_times();
	uint32_t max_times = p_role->get_max_buy_times();
	if (buy_times >= max_times) {
		ROLE_LOG("role[%lu] buy_times[%u] >= max_times[%u]", p_role->get_uid(), buy_times, max_times);
		return common::errcode_enum::notice_vigour_buy_times_much;
	}

	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::energy_buy_limit);
	if (NULL == p_conf || p_conf->parameter1().empty()) {
		log_error("role[%lu] energy_buy_limit error", p_role->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	std::vector<uint32_t> cost_vec;
	common::string_util_t::split<uint32_t>(p_conf->parameter1(), cost_vec, ",");
	if (cost_vec.empty()) {
		log_error("role[%lu] energy_buy_limit error", p_role->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	uint32_t cost = 0;
	if (buy_times >= cost_vec.size()) {
		cost = cost_vec[cost_vec.size() - 1];
	} else {
		cost = cost_vec[buy_times];
	}

	money_ptr p_money = p_role->get_money();
	if (p_money == NULL) {
		log_error("role[%lu] p_money null error", p_role->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	uint32_t money = p_money->get_money(proto::common::MONEY_TYPE_YUANBAO);
	if (money < cost)
	{
		SHOP_LOG("role[%lu] money_type[%u] money_value[%u] < cost[%u], when buy vigour",
			p_role->get_uid(), proto::common::MONEY_TYPE_YUANBAO, money, cost);

		return common::errcode_enum::notice_gold_money_not_enough;
	}

	if (!use_money(p_role, proto::common::MONEY_TYPE_YUANBAO, cost, log_enum::source_type_vigour_buy_add,
		0, true, NULL))
	{
		log_error("role[%lu] use_money money error, when buy vigour", p_role->get_uid());
		return common::errcode_enum::notice_unknown;
	}

	add_money(p_role, proto::common::MONEY_TYPE_VIGOUR, p_role->get_vigour_base_limit(), log_enum::source_type_vigour_buy_add, 0, true);
	p_role->add_vigour_buy_times(1);

	return common::errcode_enum::error_ok;
}

