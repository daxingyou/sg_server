#include "king_war_manager.hpp"

#include "log.hpp"
#include "common/time_manager.hpp"
#include "config_mgr.h"
#include "country/country_mgr.hpp"
#include "tblh/errcode_enum.hpp"
#include "redis/country_data_mgr.hpp"
#include "user/global_user_manager.hpp"
#include "redis/global_user_data_mgr.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "family/family_manager.hpp"

bool king_war_manager_t::init()
{
	// 设置王城战的一些时间回调
	auto conf = GET_CONF(Comprehensive, comprehensive_common::king_signup_time);
	uint32_t sign_up_time = GET_COMPREHENSIVE_VALUE_1(conf);
	time_manager_t::register_func(sign_up_time, king_war_manager_t::sign_up_callback);

	conf = GET_CONF(Comprehensive, comprehensive_common::king_ready_time);
	uint32_t prepare_time = GET_COMPREHENSIVE_VALUE_1(conf);
	time_manager_t::register_func(prepare_time, king_war_manager_t::prepare_callback);

	conf = GET_CONF(Comprehensive, comprehensive_common::king_battle_time);
	uint32_t battle_time = GET_COMPREHENSIVE_VALUE_1(conf);
	time_manager_t::register_func(battle_time, king_war_manager_t::start_callback);

	return true;
}

void king_war_manager_t::sign_up_callback(uint32_t id, bool is_in_time)
{
	if (is_in_time)
	{
		std::pair<uint32_t, uint32_t> sign_up_time = time_manager_t::get_refresh_time(id);

		auto conf = GET_CONF(Comprehensive, comprehensive_common::king_battle_time);
		uint32_t battle_time_id = GET_COMPREHENSIVE_VALUE_1(conf);
		std::pair<uint32_t, uint32_t> battle_time = time_manager_t::get_refresh_time(battle_time_id);

		country_mgr_t::start_king_war_sign_up(battle_time.first, sign_up_time.second);
	}
	else
	{
		country_mgr_t::end_king_war_sign_up();
	}
}

void king_war_manager_t::prepare_callback(uint32_t id, bool is_in_time)
{
	if (is_in_time)
	{
		std::pair<uint32_t, uint32_t> prepare_time = time_manager_t::get_refresh_time(id);

		country_mgr_t::start_king_war_prepare(prepare_time.second);
	}
	else
	{
		country_mgr_t::end_king_war_prepare();
	}
}

void king_war_manager_t::start_callback(uint32_t id, bool is_in_time)
{
	if (is_in_time)
	{
		std::pair<uint32_t, uint32_t> fight_time = time_manager_t::get_refresh_time(id);

		country_mgr_t::start_king_war_fight(fight_time.second);
	}
	else
	{
		country_mgr_t::end_king_war_fight();
	}
}

void king_war_manager_t::apply_king_war_request(global_user_ptr p_user, proto::client::ec_king_war_sign_up_reply& reply)
{
	if (NULL == p_user)
	{
		log_error("p_user null error");
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	uint32_t country_id = p_user->get_country_id();
	if (0 == country_id)
	{
		log_error("user[%lu] country_id == 0", p_user->get_uid());
		reply.set_reply_code(errcode_enum::country_error_not_in_country);
		return;
	}

	country_ptr p_country = country_mgr_t::get_country(country_id);
	if (NULL == p_country)
	{
		log_error("user[%lu] p_country null error", p_user->get_uid());
		reply.set_reply_code(errcode_enum::country_error_invalid_country);
		return;
	}

	uint64_t family_id = p_user->get_family_id();
	if (0 == family_id)
	{
		log_error("user[%lu] family_id == 0", p_user->get_uid());
		reply.set_reply_code(errcode_enum::notice_no_family);
		return;
	}

	if (family_id == p_country->get_country_info().m_king_family_id)
	{
		KINGWAR_LOG("user[%lu] is king family", p_user->get_uid());
		reply.set_reply_code(errcode_enum::notice_king_war_error17);
		return;
	}

	const family_ptr& p_family = family_manager_t::get_family(family_id);
	if (NULL == p_family)
	{
		log_error("user[%lu] p_family null error", p_user->get_uid());
		reply.set_reply_code(errcode_enum::family_err_code_invalid_family);
		return;
	}

	country_t::king_war_apply_ptr p_king_war_apply = p_country->get_king_war_apply_info(family_id);
	// 没报名过要做验证
	if (p_king_war_apply == NULL)
	{
		//职位
		family_officer_type family_officer = (family_officer_type)p_user->get_family_officer();
		if (family_officer != family_officer_type::family_officer_type_general && family_officer != family_officer_type::family_officer_type_vice_general)
		{
			log_error("user[%lu] have no sign up qualifications", p_user->get_uid());
			reply.set_reply_code(errcode_enum::notice_king_war_error1);
			return;
		}

		// 是否处于报名期
		const country_t::king_war_info& info = p_country->get_king_war_info();
		if (info.m_state != proto::common::king_war_state_sign_up)
		{
			log_error("user[%lu] country king war state not in king_war_state_sign_up", p_user->get_uid());
			reply.set_reply_code(errcode_enum::notice_king_war_error2);
			return;
		}

		// 等级
		auto conf = GET_CONF(Comprehensive, comprehensive_common::king_family_level);
		uint32_t family_lv_limit = GET_COMPREHENSIVE_VALUE_1(conf);
		if (p_family->get_level() < family_lv_limit)
		{
			KINGWAR_LOG("user[%lu] family lv[%u] < family_lv_limit[%u]", p_user->get_uid(), p_family->get_level(), family_lv_limit);
			reply.set_reply_code(errcode_enum::notice_king_war_error3);
			return;
		}

		// 家族排名目前没有

		// 繁荣度
		conf = GET_CONF(Comprehensive, comprehensive_common::king_family_prosperity);
		uint32_t family_prosperity_limit = GET_COMPREHENSIVE_VALUE_1(conf);
		if (p_family->get_prosperity() < family_prosperity_limit)
		{
			KINGWAR_LOG("user[%lu] family prosperity[%u] < family_prosperity_limit[%u]", p_user->get_uid(), p_family->get_prosperity(), family_prosperity_limit);
			reply.set_reply_code(errcode_enum::notice_king_war_error5);
			return;
		}

		// 资金
		conf = GET_CONF(Comprehensive, comprehensive_common::king_family_fund);
		uint32_t family_money_limit = GET_COMPREHENSIVE_VALUE_1(conf);
		if (p_family->get_money() < family_money_limit)
		{
			KINGWAR_LOG("user[%lu] family money[%u] < family_money_limit[%u]", p_user->get_uid(), p_family->get_money(), family_money_limit);
			reply.set_reply_code(errcode_enum::notice_king_war_error6);
			return;
		}

		// 加入到报名列表
		p_king_war_apply = p_country->add_king_war_family(family_id);
	}

	if (NULL == p_king_war_apply)
	{
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	reply.set_reply_code(errcode_enum::error_ok);
	reply.set_bid_money(p_king_war_apply->m_bid_cost);
	reply.set_family_money(p_family->get_money());
}

void king_war_manager_t::bid_king_war_request(global_user_ptr p_user, uint32_t money, proto::client::ec_king_war_offer_money_reply& reply)
{
	if (NULL == p_user)
	{
		log_error("p_user null error");
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	if (0 == money)
	{
		log_error("p_user[%lu] bid money == 0", p_user->get_uid());
		reply.set_reply_code(errcode_enum::notice_king_war_error8);
		return;
	}

	uint32_t country_id = p_user->get_country_id();
	if (0 == country_id)
	{
		log_error("user[%lu] country_id == 0", p_user->get_uid());
		reply.set_reply_code(errcode_enum::country_error_not_in_country);
		return;
	}

	country_ptr p_country = country_mgr_t::get_country(country_id);
	if (NULL == p_country)
	{
		log_error("user[%lu] p_country null error", p_user->get_uid());
		reply.set_reply_code(errcode_enum::country_error_invalid_country);
		return;
	}

	// 是否处于报名期
	const country_t::king_war_info& info = p_country->get_king_war_info();
	if (info.m_state != proto::common::king_war_state_sign_up)
	{
		log_error("user[%lu] country king war state not in king_war_state_sign_up", p_user->get_uid());
		reply.set_reply_code(errcode_enum::notice_king_war_error2);
		return;
	}

	uint64_t family_id = p_user->get_family_id();
	if (0 == family_id)
	{
		log_error("user[%lu] family_id == 0", p_user->get_uid());
		reply.set_reply_code(errcode_enum::notice_no_family);
		return;
	}

	const family_ptr& p_family = family_manager_t::get_family(family_id);
	if (NULL == p_family)
	{
		log_error("user[%lu] p_family null error", p_user->get_uid());
		reply.set_reply_code(errcode_enum::family_err_code_invalid_family);
		return;
	}

	//职位
	family_officer_type family_officer = (family_officer_type)p_user->get_family_officer();
	if (family_officer != family_officer_type::family_officer_type_general && family_officer != family_officer_type::family_officer_type_vice_general)
	{
		log_error("user[%lu] have no sign up qualifications", p_user->get_uid());
		reply.set_reply_code(errcode_enum::notice_king_war_error1);
		return;
	}

	// 验证家族资金
	if (p_family->get_money() < money)
	{
		log_error("user[%lu] family money[%u] < bid_money[%u]", p_user->get_uid(), p_family->get_money(), money);
		reply.set_reply_code(errcode_enum::notice_king_war_error10);
		return;
	}

	// 是否报名过
	country_t::king_war_apply_ptr p_king_war_apply = p_country->get_king_war_apply_info(family_id);
	if (NULL == p_king_war_apply)
	{
		log_error("user[%lu] family has not in apply list", p_user->get_uid());
		reply.set_reply_code(errcode_enum::notice_king_war_error9);
		return;
	}

	// 资金
	auto conf = GET_CONF(Comprehensive, comprehensive_common::king_signup_family_fund);
	uint32_t family_money_limit = GET_COMPREHENSIVE_VALUE_1(conf);
	if (p_king_war_apply->m_bid_cost == 0 && money < family_money_limit)
	{
		log_error("user[%lu] first bid[%u] must bigger than limit[%u]", p_user->get_uid(), money, family_money_limit);
		reply.set_reply_code(errcode_enum::notice_king_war_error7);
		return;
	}

	// 扣钱
	p_family->cost_money(money, proto::server::family_money_change_king_war_cost, p_user->get_uid());

	// 增加资金
	p_king_war_apply->m_bid_cost += money;
	country_data_mgr_t::update_king_war_apply_cost(family_id, p_king_war_apply);

	// 写个日志
	KINGWAR_LOG("user[%lu] family[%s] now all bid money[%u]", p_user->get_uid(), p_family->get_name().c_str(), p_king_war_apply->m_bid_cost);

	reply.set_reply_code(errcode_enum::error_ok);
	reply.set_bid_money(p_king_war_apply->m_bid_cost);
	reply.set_family_money(p_family->get_money());
}

void king_war_manager_t::get_king_war_apply_list(global_user_ptr p_user, proto::client::ec_king_war_sign_up_list_reply& reply)
{
	if (NULL == p_user)
	{
		log_error("p_user null error");
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	uint32_t country_id = p_user->get_country_id();
	if (0 == country_id)
	{
		log_error("user[%lu] country_id == 0", p_user->get_uid());
		reply.set_reply_code(errcode_enum::country_error_not_in_country);
		return;
	}

	country_ptr p_country = country_mgr_t::get_country(country_id);
	if (NULL == p_country)
	{
		log_error("user[%lu] p_country null error", p_user->get_uid());
		reply.set_reply_code(errcode_enum::country_error_invalid_country);
		return;
	}

	// 是否处于报名期
	const country_t::king_war_info& info = p_country->get_king_war_info();
	if (info.m_state != proto::common::king_war_state_sign_up)
	{
		log_error("user[%lu] country king war state not in king_war_state_sign_up", p_user->get_uid());
		reply.set_reply_code(errcode_enum::notice_king_war_error2);
		return;
	}

	//uint64_t family_id = p_user->get_family_id();
	//if (0 == family_id)
	//{
	//	log_error("user[%lu] family_id == 0", p_user->get_uid());
	//	reply.set_reply_code(errcode_enum::notice_no_family);
	//	return;
	//}

	//auto p_family = family_mgr_t::FindObject(family_id);
	//if (NULL == p_family)
	//{
	//	log_error("user[%lu] p_family null error", p_user->get_uid());
	//	reply.set_reply_code(errcode_enum::family_err_code_invalid_family);
	//	return;
	//}

	const country_t::king_war_apply_map& apply_map = p_country->get_king_war_apply_list();
	for (country_t::king_war_apply_map::const_iterator citr = apply_map.begin();
		citr != apply_map.end(); ++citr)
	{
		const country_t::king_war_apply_ptr& p_king_war_apply = citr->second;
		if (p_king_war_apply != NULL)
		{
			if (p_king_war_apply->m_bid_state == EM_BID_STATE_BIDING && p_king_war_apply->m_bid_cost > 0)
			{
				const family_ptr& p_king_war_family = family_manager_t::get_family(p_king_war_apply->m_family_id);
				if (NULL == p_king_war_family)
				{
					log_error("family[%lu] can't find", p_king_war_apply->m_family_id);
					continue;
				}

				proto::common::king_war_sign_up_data* data = reply.add_datas();
				if (data != NULL)
				{
					data->set_family_name(p_king_war_family->get_name());
					data->set_time(p_king_war_apply->m_time);

					uint64_t general_id = p_king_war_family->get_general_id();
					const global_user_ptr& p_general = global_user_manager_t::get_global_user_by_uid(general_id);
					if (p_general != NULL)
					{
						data->set_patriarch_name(p_general->get_name());
					}
					else
					{
						data->set_patriarch_name(global_user_data_mgr_t::get_name(general_id));
					}
				}
			}
		}
	}

	reply.set_reply_code(errcode_enum::error_ok);
}
