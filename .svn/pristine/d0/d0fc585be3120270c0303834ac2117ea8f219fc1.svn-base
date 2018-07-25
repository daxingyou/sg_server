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
#include "rank/rank_manager.hpp"
#include "tblh/rank_common.hpp"

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

void king_war_manager_t::apply_king_war_request(global_user_ptr p_user, uint32_t country_id, proto::client::ec_king_war_sign_up_reply& reply)
{
	if (NULL == p_user) {
		log_error("p_user null error");
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	uint64_t family_id = p_user->get_family_id();
	if (0 == family_id) {
		log_error("user[%lu] family_id == 0", p_user->get_uid());
		reply.set_reply_code(errcode_enum::notice_no_family);
		return;
	}

	const family_ptr& p_family = family_manager_t::get_family(family_id);
	if (NULL == p_family) {
		log_error("user[%lu] p_family null error", p_user->get_uid());
		reply.set_reply_code(errcode_enum::family_err_code_invalid_family);
		return;
	}

	if (!family_manager_t::has_family_privilige(p_user->get_family_officer(), privilige_type_family_king_signup)) {
		log_error("user[%lu] has no privilige king_signup", p_user->get_uid());
		reply.set_reply_code(common::errcode_enum::family_err_code_no_right);
		return;
	}

	country_ptr p_country = country_mgr_t::get_country(country_id);
	if (NULL == p_country) {
		log_error("user[%lu] p_country null error", p_user->get_uid());
		reply.set_reply_code(errcode_enum::country_error_invalid_country);
		return;
	}

	if (family_id == p_country->get_country_info().m_king_family_id) {
		KINGWAR_LOG("user[%lu] is king family", p_user->get_uid());
		reply.set_reply_code(errcode_enum::notice_king_war_error17);
		return;
	}

	// 是否处于报名期
	const country_t::king_war_info& info = p_country->get_king_war_info();
	if (info.m_state != proto::common::king_war_state_sign_up) {
		log_error("user[%lu] country king war state not in king_war_state_sign_up", p_user->get_uid());
		reply.set_reply_code(errcode_enum::notice_king_war_error2);
		return;
	}

	if (p_family->get_country_id() != 0 && p_family->get_country_id() != country_id) {
		log_error("user[%lu] family country[%u] !=  king_war signup country[%u]", p_user->get_uid(), p_family->get_country_id(), country_id);
		reply.set_reply_code(common::errcode_enum::king_war_error_family_in_country);
		return;
	}

	if (p_family->get_king_war_apply_country() != 0 && p_family->get_king_war_apply_country() != country_id) {
		log_error("user[%lu] family is already signup country[%u]", p_user->get_uid(), p_family->get_king_war_apply_country());
		reply.set_reply_code(common::errcode_enum::king_war_error_sign_up_one);
		return;
	}

	country_t::king_war_apply_ptr p_king_war_apply = p_country->get_king_war_apply_info(family_id);
	// 没报名过要做验证
	if (p_king_war_apply == NULL) {
		// 等级
		auto conf = GET_CONF(Comprehensive, comprehensive_common::king_family_level);
		uint32_t family_lv_limit = GET_COMPREHENSIVE_VALUE_1(conf);
		if (p_family->get_level() < family_lv_limit) {
			KINGWAR_LOG("user[%lu] family lv[%u] < family_lv_limit[%u]", p_user->get_uid(), p_family->get_level(), family_lv_limit);
			reply.set_reply_code(errcode_enum::notice_king_war_error3);
			return;
		}

		// 家族排名
		uint32_t rank = rank_manager_t::get_rank_by_uid(common::rank_common::family_rank_0, family_id);
		conf = GET_CONF(Comprehensive, comprehensive_common::king_family_rank);
		uint32_t family_rank_limit = GET_COMPREHENSIVE_VALUE_1(conf);
		if (rank == 0 || rank > family_rank_limit) {
			KINGWAR_LOG("user[%lu] family rank[%u] < family_rank_limit[%u]", p_user->get_uid(), rank, family_rank_limit);
			reply.set_reply_code(errcode_enum::notice_king_war_error4);
			return;
		}
	}

	reply.set_reply_code(errcode_enum::error_ok);
	if (NULL != p_king_war_apply) {
		reply.set_bid_money(p_king_war_apply->m_bid_cost);
	} else {
		reply.set_bid_money(0);
	}
	reply.set_family_money(p_family->get_money());
}

void king_war_manager_t::bid_king_war_request(global_user_ptr p_user, uint32_t country_id, uint32_t money, proto::client::ec_king_war_offer_money_reply& reply)
{
	if (NULL == p_user) {
		log_error("p_user null error");
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	uint64_t family_id = p_user->get_family_id();
	if (0 == family_id) {
		log_error("user[%lu] family_id == 0", p_user->get_uid());
		reply.set_reply_code(errcode_enum::notice_no_family);
		return;
	}

	const family_ptr& p_family = family_manager_t::get_family(family_id);
	if (NULL == p_family) {
		log_error("user[%lu] p_family null error", p_user->get_uid());
		reply.set_reply_code(errcode_enum::family_err_code_invalid_family);
		return;
	}

	if (!family_manager_t::has_family_privilige(p_user->get_family_officer(), privilige_type_family_king_signup)) {
		log_error("user[%lu] has no privilige king_signup", p_user->get_uid());
		reply.set_reply_code(common::errcode_enum::family_err_code_no_right);
		return;
	}

	country_ptr p_country = country_mgr_t::get_country(country_id);
	if (NULL == p_country) {
		log_error("user[%lu] p_country null error", p_user->get_uid());
		reply.set_reply_code(errcode_enum::country_error_invalid_country);
		return;
	}

	if (family_id == p_country->get_country_info().m_king_family_id) {
		KINGWAR_LOG("user[%lu] is king family", p_user->get_uid());
		reply.set_reply_code(errcode_enum::notice_king_war_error17);
		return;
	}

	// 是否处于报名期
	const country_t::king_war_info& info = p_country->get_king_war_info();
	if (info.m_state != proto::common::king_war_state_sign_up) {
		log_error("user[%lu] country king war state not in king_war_state_sign_up", p_user->get_uid());
		reply.set_reply_code(errcode_enum::notice_king_war_error2);
		return;
	}

	if (p_family->get_country_id() != 0 && p_family->get_country_id() != country_id) {
		log_error("user[%lu] family country[%u] !=  king_war signup country[%u]", p_user->get_uid(), p_family->get_country_id(), country_id);
		reply.set_reply_code(common::errcode_enum::king_war_error_family_in_country);
		return;
	}

	if (p_family->get_king_war_apply_country() != 0 && p_family->get_king_war_apply_country() != country_id) {
		log_error("user[%lu] family is already signup country[%u]", p_user->get_uid(), p_family->get_king_war_apply_country());
		reply.set_reply_code(common::errcode_enum::king_war_error_sign_up_one);
		return;
	}

	country_t::king_war_apply_ptr p_king_war_apply = p_country->get_king_war_apply_info(family_id);
	// 没报名过要做验证
	if (p_king_war_apply == NULL) {
		// 等级
		auto conf = GET_CONF(Comprehensive, comprehensive_common::king_family_level);
		uint32_t family_lv_limit = GET_COMPREHENSIVE_VALUE_1(conf);
		if (p_family->get_level() < family_lv_limit) {
			KINGWAR_LOG("user[%lu] family lv[%u] < family_lv_limit[%u]", p_user->get_uid(), p_family->get_level(), family_lv_limit);
			reply.set_reply_code(errcode_enum::notice_king_war_error3);
			return;
		}

		// 家族排名(先去掉)
		uint32_t rank = rank_manager_t::get_rank_by_uid(common::rank_common::family_rank_0, family_id);
		conf = GET_CONF(Comprehensive, comprehensive_common::king_family_rank);
		uint32_t family_rank_limit = GET_COMPREHENSIVE_VALUE_1(conf);
		if (rank == 0 || rank > family_rank_limit) {
			KINGWAR_LOG("user[%lu] family rank[%u] < family_rank_limit[%u]", p_user->get_uid(), rank, family_rank_limit);
			reply.set_reply_code(errcode_enum::notice_king_war_error4);
			return;
		}
	}

	if (0 == money) {
		log_error("p_user[%lu] bid money == 0", p_user->get_uid());
		reply.set_reply_code(errcode_enum::notice_king_war_error8);
		return;
	}

	// 验证家族资金
	if (p_family->get_money() < money) {
		log_error("user[%lu] family money[%u] < bid_money[%u]", p_user->get_uid(), p_family->get_money(), money);
		reply.set_reply_code(errcode_enum::notice_king_war_error10);
		return;
	}

	// 资金
	if (p_king_war_apply == NULL) {
		auto conf = GET_CONF(Comprehensive, comprehensive_common::king_signup_family_fund1);
		uint32_t family_money_limit = GET_COMPREHENSIVE_VALUE_1(conf);
		if (money < family_money_limit) {
			log_error("user[%lu] first bid[%u] must bigger than limit[%u]", p_user->get_uid(), money, family_money_limit);
			reply.set_reply_code(errcode_enum::notice_king_war_error7);
			return;
		}
	} else {
		auto conf = GET_CONF(Comprehensive, comprehensive_common::king_signup_family_fund2);
		uint32_t family_money_limit = GET_COMPREHENSIVE_VALUE_1(conf);
		if (money < family_money_limit) {
			log_error("user[%lu] append add bid[%u] must bigger than limit[%u]", p_user->get_uid(), money, family_money_limit);
			reply.set_reply_code(errcode_enum::notice_king_war_error6);
			return;
		}
	}

	// 扣钱
	p_family->cost_money(money, proto::server::family_money_change_king_war_cost, p_user->get_uid());

	if (p_king_war_apply == NULL) {
		p_king_war_apply = p_country->add_king_war_family(family_id, money);

		if (p_king_war_apply == NULL) {
			log_error("p_king_war_apply null error");
			reply.set_reply_code(errcode_enum::notice_unknown);
			return;
		}

		p_family->set_king_war_apply_country(country_id);
		p_country->get_king_war_sign_up(family_id, reply.mutable_country());
	} else {
		// 增加资金
		p_king_war_apply->m_bid_cost += money;
		country_data_mgr_t::update_king_war_apply_cost(family_id, p_king_war_apply);
	}

	reply.set_reply_code(errcode_enum::error_ok);
	reply.set_bid_money(p_king_war_apply->m_bid_cost);
	reply.set_family_money(p_family->get_money());

	// 写个日志
	KINGWAR_LOG("user[%lu] family[%s] now all bid money[%u]", p_user->get_uid(), p_family->get_name().c_str(), p_king_war_apply->m_bid_cost);
}

void king_war_manager_t::get_king_war_apply_list(global_user_ptr p_user, uint32_t country_id, proto::client::ec_king_war_sign_up_list_reply& reply)
{
	if (NULL == p_user) {
		log_error("p_user null error");
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	country_ptr p_country = country_mgr_t::get_country(country_id);
	if (NULL == p_country) {
		log_error("user[%lu] p_country null error", p_user->get_uid());
		reply.set_reply_code(errcode_enum::country_error_invalid_country);
		return;
	}

	// 是否处于报名期
	const country_t::king_war_info& info = p_country->get_king_war_info();
	if (info.m_state != proto::common::king_war_state_sign_up) {
		log_error("user[%lu] country king war state not in king_war_state_sign_up", p_user->get_uid());
		reply.set_reply_code(errcode_enum::notice_king_war_error2);
		return;
	}

	const country_t::king_war_apply_map& apply_map = p_country->get_king_war_apply_list();
	for (country_t::king_war_apply_map::const_iterator citr = apply_map.begin();
		citr != apply_map.end(); ++citr) {
		const country_t::king_war_apply_ptr& p_king_war_apply = citr->second;
		if (p_king_war_apply != NULL) {
			if (p_king_war_apply->m_bid_state == EM_BID_STATE_BIDING && p_king_war_apply->m_bid_cost > 0) {
				const family_ptr& p_king_war_family = family_manager_t::get_family(p_king_war_apply->m_family_id);
				if (NULL == p_king_war_family) {
					log_error("king_war apply family[%lu] can't find", p_king_war_apply->m_family_id);
					continue;
				}

				proto::common::king_war_sign_up_data* data = reply.add_datas();
				if (data != NULL) {
					data->set_family_name(p_king_war_family->get_name());
					data->set_time(p_king_war_apply->m_time);

					uint64_t general_id = p_king_war_family->get_general_id();
					const global_user_ptr& p_general = global_user_manager_t::get_global_user_by_uid(general_id);
					if (p_general != NULL) {
						data->set_patriarch_name(p_general->get_name());
					} else {
						data->set_patriarch_name(global_user_data_mgr_t::get_name(general_id));
					}
				}
			}
		}
	}

	reply.set_country_id(country_id);
	reply.set_reply_code(errcode_enum::error_ok);
}

void king_war_manager_t::sudo_king_war_apply(const global_user_ptr& p_user, proto::client::ec_king_war_undo_sign_up_reply& reply)
{
	if (NULL == p_user) {
		log_error("p_user null error");
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	uint64_t family_id = p_user->get_family_id();
	if (0 == family_id) {
		log_error("user[%lu] family_id == 0", p_user->get_uid());
		reply.set_reply_code(errcode_enum::notice_no_family);
		return;
	}

	const family_ptr& p_family = family_manager_t::get_family(family_id);
	if (NULL == p_family) {
		log_error("user[%lu] p_family null error", p_user->get_uid());
		reply.set_reply_code(errcode_enum::family_err_code_invalid_family);
		return;
	}

	if (!family_manager_t::has_family_privilige(p_user->get_family_officer(), privilige_type_family_king_signup)) {
		log_error("user[%lu] has no privilige king_signup", p_user->get_uid());
		reply.set_reply_code(errcode_enum::family_err_code_no_right);
		return;
	}

	uint32_t apply_country = p_family->get_king_war_apply_country();
	if (apply_country == 0) {
		log_error("user[%lu] family not apply king war", p_user->get_uid());
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	country_ptr p_country = country_mgr_t::get_country(apply_country);
	if (NULL == p_country) {
		log_error("user[%lu] p_country null error", p_user->get_uid());
		reply.set_reply_code(errcode_enum::country_error_invalid_country);
		return;
	}

	// 是否处于报名期
	const country_t::king_war_info& info = p_country->get_king_war_info();
	if (info.m_state != proto::common::king_war_state_sign_up) {
		log_error("user[%lu] country king war state not in king_war_state_sign_up", p_user->get_uid());
		reply.set_reply_code(errcode_enum::king_war_error_sign_up_done);
		return;
	}

	auto conf = GET_CONF(Comprehensive, comprehensive_common::king_signup_family_fund3);
	uint32_t family_money_limit = GET_COMPREHENSIVE_VALUE_1(conf);
	if (p_family->get_money() < family_money_limit) {
		log_error("user[%lu] sudo appp money limit[%u]", p_user->get_uid(), family_money_limit);
		reply.set_reply_code(errcode_enum::king_war_error_sudo_money_limit);
		return;
	}

	p_family->cost_money(family_money_limit, proto::server::family_money_change_king_war_cost, p_user->get_uid());

	p_country->del_king_war_apply_family(p_family);

	p_country->get_king_war_sign_up(family_id, reply.mutable_country());
	reply.set_reply_code(errcode_enum::error_ok);
}

void king_war_manager_t::update_king_war_notice(const global_user_ptr& p_user, uint32_t country_id, const std::string& notice, proto::client::ec_king_war_notice_reply& reply)
{
	if (NULL == p_user) {
		log_error("p_user null error");
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	uint64_t family_id = p_user->get_family_id();
	if (0 == family_id) {
		log_error("user[%lu] family_id == 0", p_user->get_uid());
		reply.set_reply_code(errcode_enum::notice_no_family);
		return;
	}

	const family_ptr& p_family = family_manager_t::get_family(family_id);
	if (NULL == p_family) {
		log_error("user[%lu] p_family null error", p_user->get_uid());
		reply.set_reply_code(errcode_enum::family_err_code_invalid_family);
		return;
	}

	if (!family_manager_t::has_family_privilige(p_user->get_family_officer(), privilige_type_family_king_edit)) {
		log_error("user[%lu] has no privilige king_edit", p_user->get_uid());
		reply.set_reply_code(errcode_enum::family_err_code_no_right);
		return;
	}

	country_ptr p_country = country_mgr_t::get_country(country_id);
	if (NULL == p_country) {
		log_error("user[%lu] p_country null error", p_user->get_uid());
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	// 是否处于报名期
	const country_t::king_war_info& info = p_country->get_king_war_info();
	if (info.m_state < proto::common::king_war_state_pre) {
		log_error("user[%lu] country king war state[%u] error", p_user->get_uid(), info.m_state);
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	uint32_t kingwar_type = p_country->get_king_war_family_type(family_id);
	if (kingwar_type == country_t::king_war_type_error) {
		log_error("user[%lu] family not country[%u] king war family", p_user->get_uid(), country_id);
		reply.set_reply_code(errcode_enum::king_war_error_not_king_war_family);
		return;
	}

	p_country->update_king_war_notice(p_user, notice);

	reply.set_reply_code(common::errcode_enum::error_ok);
	reply.set_country_id(country_id);
	p_country->peek_king_war_fight_by_type(kingwar_type, p_user->get_uid(), reply.mutable_info());
}

void king_war_manager_t::update_king_war_like(const global_user_ptr& p_user, uint32_t country_id, uint64_t family_id, uint32_t like, proto::client::ec_king_war_like_reply& reply)
{
	if (NULL == p_user) {
		log_error("p_user null error");
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	country_ptr p_country = country_mgr_t::get_country(country_id);
	if (NULL == p_country) {
		log_error("user[%lu] p_country null error", p_user->get_uid());
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	// 是否处于报名期
	const country_t::king_war_info& info = p_country->get_king_war_info();
	if (info.m_state < proto::common::king_war_state_pre) {
		log_error("user[%lu] country king war state[%u] error", p_user->get_uid(), info.m_state);
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	uint32_t kingwar_type = p_country->get_king_war_family_type(family_id);
	if (kingwar_type == country_t::king_war_type_error) {
		log_error("user[%lu] family not country[%u] king war family", p_user->get_uid(), country_id);
		reply.set_reply_code(errcode_enum::king_war_error_not_king_war_family);
		return;
	}

	if (p_country->is_do_like(kingwar_type, p_user->get_uid())) {
		log_error("user[%lu] country[%u] family[%lu] is alread do like or dilike", p_user->get_uid(), country_id, family_id);
		reply.set_reply_code(errcode_enum::king_war_error_already_like);
		return;
	}

	p_country->do_king_war_like(kingwar_type, p_user->get_uid(), like);

	reply.set_reply_code(common::errcode_enum::error_ok);
	reply.set_country_id(country_id);
	p_country->peek_king_war_fight_by_type(kingwar_type, p_user->get_uid(), reply.mutable_info());
}

void king_war_manager_t::get_king_war_scene_num(uint32_t country_id, proto::client::ec_king_war_scene_num_reply& reply)
{
	country_ptr p_country = country_mgr_t::get_country(country_id);
	if (NULL == p_country) {
		log_error("country[%u] null error", country_id);
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	// 是否处于报名期
	const country_t::king_war_info& info = p_country->get_king_war_info();
	if (info.m_state < proto::common::king_war_state_prepare) {
		log_error("country[%u] king war state[%u] error", country_id, info.m_state);
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	reply.set_reply_code(common::errcode_enum::error_ok);
	reply.set_att_family_num(info.m_attack_num);
	reply.set_def_family_num(info.m_defensive_num);
}

void king_war_manager_t::get_king_war_help_family_list(const global_user_ptr& p_user, uint32_t page, proto::client::ec_king_war_help_family_list_reply& reply)
{
	if (NULL == p_user) {
		log_error("p_user null error");
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	uint64_t family_id = p_user->get_family_id();
	if (0 == family_id) {
		log_error("user[%lu] family_id == 0", p_user->get_uid());
		reply.set_reply_code(errcode_enum::notice_no_family);
		return;
	}

	const family_ptr& p_family = family_manager_t::get_family(family_id);
	if (NULL == p_family) {
		log_error("user[%lu] p_family null error", p_user->get_uid());
		reply.set_reply_code(errcode_enum::family_err_code_invalid_family);
		return;
	}

	if (p_family->get_general_id() != p_user->get_uid()) {
		log_error("user[%lu] has no privilige get_king_war_help_family_list", p_user->get_uid());
		reply.set_reply_code(errcode_enum::family_err_code_no_right);
		return;
	}

	if (p_family->get_king_war_fight_country() == 0) {
		log_error("user[%lu] family not king war family", p_user->get_uid());
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	country_ptr p_country = country_mgr_t::get_country(p_family->get_king_war_fight_country());
	if (NULL == p_country) {
		log_error("user[%lu] p_country null error", p_user->get_uid());
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	// 是否处于预告期
	const country_t::king_war_info& info = p_country->get_king_war_info();
	if (info.m_state != proto::common::king_war_state_pre) {
		log_error("user[%lu] country king war state[%u] error", p_user->get_uid(), info.m_state);
		reply.set_reply_code(errcode_enum::king_war_error_no_state_pre);
		return;
	}

	uint32_t kingwar_type = p_country->get_king_war_family_type(family_id);
	if (kingwar_type == country_t::king_war_type_error) {
		log_error("user[%lu] family not country[%u] king war family", p_user->get_uid(), p_country->get_country_id());
		reply.set_reply_code(errcode_enum::king_war_error_not_king_war_family);
		return;
	}

	uint64_t help_family = p_country->get_king_war_help_family(kingwar_type);
	if (help_family != 0) {
		log_error("user[%lu] country[%u] king war has help family", p_user->get_uid(), p_country->get_country_id());
		reply.set_reply_code(errcode_enum::king_war_error_already_have_helper);
		return;
	}

	family_manager_t::get_help_family_list(page, p_country->get_king_war_invited_list(kingwar_type), p_country->get_king_war_refused_list(kingwar_type), reply);
}

void king_war_manager_t::get_king_war_help_family_fuzzy(const global_user_ptr& p_user, const std::string& name, proto::client::ec_king_war_help_family_fuzzy_reply& reply)
{
	if (NULL == p_user) {
		log_error("p_user null error");
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	uint64_t family_id = p_user->get_family_id();
	if (0 == family_id) {
		log_error("user[%lu] family_id == 0", p_user->get_uid());
		reply.set_reply_code(errcode_enum::notice_no_family);
		return;
	}

	const family_ptr& p_family = family_manager_t::get_family(family_id);
	if (NULL == p_family) {
		log_error("user[%lu] p_family null error", p_user->get_uid());
		reply.set_reply_code(errcode_enum::family_err_code_invalid_family);
		return;
	}

	if (p_family->get_general_id() != p_user->get_uid()) {
		log_error("user[%lu] has no privilige get_king_war_help_family_list", p_user->get_uid());
		reply.set_reply_code(errcode_enum::family_err_code_no_right);
		return;
	}

	if (p_family->get_king_war_fight_country() == 0) {
		log_error("user[%lu] family not king war family", p_user->get_uid());
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	country_ptr p_country = country_mgr_t::get_country(p_family->get_king_war_fight_country());
	if (NULL == p_country) {
		log_error("user[%lu] p_country null error", p_user->get_uid());
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	// 是否处于预告期
	const country_t::king_war_info& info = p_country->get_king_war_info();
	if (info.m_state != proto::common::king_war_state_pre) {
		log_error("user[%lu] country king war state[%u] error", p_user->get_uid(), info.m_state);
		reply.set_reply_code(errcode_enum::king_war_error_no_state_pre);
		return;
	}

	uint32_t kingwar_type = p_country->get_king_war_family_type(family_id);
	if (kingwar_type == country_t::king_war_type_error) {
		log_error("user[%lu] family not country[%u] king war family", p_user->get_uid(), p_country->get_country_id());
		reply.set_reply_code(errcode_enum::king_war_error_not_king_war_family);
		return;
	}

	uint64_t help_family = p_country->get_king_war_help_family(kingwar_type);
	if (help_family != 0) {
		log_error("user[%lu] country[%u] king war has help family", p_user->get_uid(), p_country->get_country_id());
		reply.set_reply_code(errcode_enum::king_war_error_already_have_helper);
		return;
	}

	family_manager_t::get_help_family_fuzzy_list(name, p_country->get_king_war_invited_list(kingwar_type), p_country->get_king_war_refused_list(kingwar_type), reply);
}

uint32_t king_war_manager_t::invite_king_war_help_family(const global_user_ptr& p_user, uint64_t invite_family)
{
	if (NULL == p_user) {
		log_error("p_user null error");
		return errcode_enum::notice_unknown;
	}

	uint64_t family_id = p_user->get_family_id();
	if (0 == family_id) {
		log_error("user[%lu] family_id == 0", p_user->get_uid());
		return errcode_enum::notice_no_family;
	}

	const family_ptr& p_family = family_manager_t::get_family(family_id);
	if (NULL == p_family) {
		log_error("user[%lu] p_family null error", p_user->get_uid());
		return errcode_enum::family_err_code_invalid_family;
	}

	if (p_family->get_general_id() != p_user->get_uid()) {
		log_error("user[%lu] has no privilige get_king_war_help_family_list", p_user->get_uid());
		return errcode_enum::family_err_code_no_right;
	}

	if (p_family->get_king_war_fight_country() == 0) {
		log_error("user[%lu] family not king war family", p_user->get_uid());
		return errcode_enum::notice_unknown;
	}

	country_ptr p_country = country_mgr_t::get_country(p_family->get_king_war_fight_country());
	if (NULL == p_country) {
		log_error("user[%lu] p_country null error", p_user->get_uid());
		return errcode_enum::notice_unknown;
	}

	// 是否处于预告期
	const country_t::king_war_info& info = p_country->get_king_war_info();
	if (info.m_state != proto::common::king_war_state_pre) {
		log_error("user[%lu] country king war state[%u] error", p_user->get_uid(), info.m_state);
		return errcode_enum::king_war_error_no_state_pre;
	}

	uint32_t kingwar_type = p_country->get_king_war_family_type(family_id);
	if (kingwar_type == country_t::king_war_type_error) {
		log_error("user[%lu] family not country[%u] king war family", p_user->get_uid(), p_country->get_country_id());
		return errcode_enum::king_war_error_not_king_war_family;
	}

	uint64_t help_family = p_country->get_king_war_help_family(kingwar_type);
	if (help_family != 0) {
		log_error("user[%lu] country[%u] king war has help family", p_user->get_uid(), p_country->get_country_id());
		return errcode_enum::king_war_error_already_have_helper;
	}

	const family_ptr& p_invite_family = family_manager_t::get_family(invite_family);
	if (NULL == p_invite_family) {
		log_error("user[%lu] invite_family[%lu] null error", p_user->get_uid(), invite_family);
		return errcode_enum::family_err_code_invalid_family;
	}

	if (p_invite_family->get_king_war_fight_country() != 0) {
		log_error("user[%lu] invite_family[%lu] is already king war family", p_user->get_uid(), invite_family);
		return errcode_enum::king_war_error_already_fight_family;
	}

	if (p_invite_family->is_king_family()) {
		log_error("user[%lu] invite_family[%lu] is king family", p_user->get_uid(), invite_family);
		return errcode_enum::king_war_error_invited_king_family;
	}

	auto conf = GET_CONF(Comprehensive, comprehensive_common::king_signup_help_cond1);
	uint32_t limit_lv = GET_COMPREHENSIVE_VALUE_1(conf);
	if (p_invite_family->get_level() < limit_lv) {
		log_error("user[%lu] invite_family[%lu] level[%u] < %u", p_user->get_uid(), invite_family, p_invite_family->get_level(), limit_lv);
		return errcode_enum::king_war_error_invited_level_limit;
	}

	p_country->add_king_war_help_invited_list(kingwar_type, invite_family);

	uint64_t invite_family_general_id = p_invite_family->get_general_id();
	global_user_ptr p_invite_family_general = global_user_manager_t::get_global_user_by_uid(invite_family_general_id);
	if (p_invite_family_general) {
		proto::client::ec_king_war_invite_help_notify ntf;
		proto::common::king_war_helf_info* p_info = ntf.add_infos();
		if (p_info) {
			p_info->set_family_uid(common::string_util_t::uint64_to_string(p_family->get_family_id()));
			p_info->set_family_name(p_family->get_name());
			p_info->set_country(p_country->get_country_id());
			p_info->set_fight_time(info.m_start_time);
		}
		p_invite_family_general->send_msg_to_client(op_cmd::ec_king_war_invite_help_notify, ntf);
	}

	return errcode_enum::error_ok;
}

uint32_t king_war_manager_t::king_war_process_invited(const global_user_ptr& p_user, uint64_t invited_family_id, uint32_t process)
{
	if (NULL == p_user) {
		log_error("p_user null error");
		return errcode_enum::notice_unknown;
	}

	const family_ptr& p_family = family_manager_t::get_family(p_user->get_family_id());
	if (NULL == p_family) {
		log_error("user[%lu] family_id null error", p_user->get_uid());
		return errcode_enum::family_err_code_invalid_family;
	}

	if (p_family->get_general_id() != p_user->get_uid()) {
		log_error("user[%lu] has no privilige process_invited", p_user->get_uid());
		return errcode_enum::family_err_code_no_right;
	}

	if (0 == invited_family_id) {
		log_error("user[%lu] invited_family_id == 0", p_user->get_uid());
		return errcode_enum::notice_unknown;
	}

	const family_ptr& p_invited_family = family_manager_t::get_family(invited_family_id);
	if (NULL == p_invited_family) {
		log_error("user[%lu] p_invited_family null error", p_user->get_uid());
		return errcode_enum::family_err_code_invalid_family;
	}

	if (p_invited_family->get_king_war_fight_country() == 0) {
		log_error("user[%lu] p_invited_family not king war family", p_user->get_uid());
		return errcode_enum::notice_unknown;
	}

	country_ptr p_country = country_mgr_t::get_country(p_invited_family->get_king_war_fight_country());
	if (NULL == p_country) {
		log_error("user[%lu] p_country null error", p_user->get_uid());
		return errcode_enum::notice_unknown;
	}

	// 是否处于预告期
	const country_t::king_war_info& info = p_country->get_king_war_info();
	if (info.m_state != proto::common::king_war_state_pre) {
		log_error("user[%lu] country king war state[%u] error", p_user->get_uid(), info.m_state);
		return errcode_enum::king_war_error_no_state_pre;
	}

	uint32_t kingwar_type = p_country->get_king_war_family_type(invited_family_id);
	if (kingwar_type == country_t::king_war_type_error) {
		log_error("user[%lu] invited_family not country[%u] king war family", p_user->get_uid(), p_country->get_country_id());
		return errcode_enum::king_war_error_not_king_war_family;
	}

	uint64_t help_family = p_country->get_king_war_help_family(kingwar_type);
	if (help_family != 0) {
		if (process != 0) {
			log_error("user[%lu] country[%u] king war has help family", p_user->get_uid(), p_country->get_country_id());
			return errcode_enum::king_war_error_already_have_helper;
		} else {
			if (!p_country->is_king_war_fight_invite(kingwar_type, p_family->get_family_id())) {
				log_error("user[%lu] family[%lu] not invited family", p_user->get_uid(), p_family->get_family_id());
				return errcode_enum::king_war_error_already_have_helper;
			}

			p_country->add_king_war_help_refused_list(kingwar_type, p_family->get_family_id());
		}
	} else {
		if (!p_country->is_king_war_fight_invite(kingwar_type, p_family->get_family_id())) {
			log_error("user[%lu] family[%lu] not invited family", p_user->get_uid(), p_family->get_family_id());
			return errcode_enum::king_war_error_already_have_helper;
		}

		// 先清除
		p_country->del_king_war_fight_invite(kingwar_type, p_family->get_family_id());

		if (process != 0) { // 同意助战
			if (p_family->get_king_war_fight_country() != 0) {
				log_error("user[%lu] family is already king war fight family", p_user->get_uid());
				return errcode_enum::king_war_error_already_fight_family;
			}

			if (p_family->is_king_family()) {
				log_error("user[%lu] invite_family[%lu] is king family", p_user->get_uid(), p_family->get_family_id());
				return errcode_enum::king_war_error_invited_king_family;
			}

			p_country->agree_king_war_fight_invite(kingwar_type, p_invited_family, p_family);
		} else { // 拒绝助战
			p_country->add_king_war_help_refused_list(kingwar_type, p_family->get_family_id());
		}
	}

	return common::errcode_enum::error_ok;
}

void king_war_manager_t::king_war_cancel_invited(const global_user_ptr& p_user, proto::client::ec_cencel_king_war_invite_reply& reply)
{
	if (NULL == p_user) {
		log_error("p_user null error");
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	uint64_t family_id = p_user->get_family_id();
	if (0 == family_id) {
		log_error("user[%lu] family_id == 0", p_user->get_uid());
		reply.set_reply_code(errcode_enum::notice_no_family);
		return;
	}

	const family_ptr& p_family = family_manager_t::get_family(family_id);
	if (NULL == p_family) {
		log_error("user[%lu] p_family null error", p_user->get_uid());
		reply.set_reply_code(errcode_enum::family_err_code_invalid_family);
		return;
	}

	if (p_family->get_general_id() != p_user->get_uid()) {
		log_error("user[%lu] has no privilige get_king_war_help_family_list", p_user->get_uid());
		reply.set_reply_code(errcode_enum::family_err_code_no_right);
		return;
	}

	if (p_family->get_king_war_fight_country() == 0) {
		log_error("user[%lu] family not king war family", p_user->get_uid());
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	country_ptr p_country = country_mgr_t::get_country(p_family->get_king_war_fight_country());
	if (NULL == p_country) {
		log_error("user[%lu] p_country null error", p_user->get_uid());
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	// 是否处于预告期
	const country_t::king_war_info& info = p_country->get_king_war_info();
	if (info.m_state != proto::common::king_war_state_pre) {
		log_error("user[%lu] country king war state[%u] error", p_user->get_uid(), info.m_state);
		reply.set_reply_code(errcode_enum::king_war_error_no_state_pre);
		return;
	}

	uint32_t kingwar_type = p_country->get_king_war_family_type(family_id);
	if (kingwar_type == country_t::king_war_type_error) {
		log_error("user[%lu] family not country[%u] king war family", p_user->get_uid(), p_country->get_country_id());
		reply.set_reply_code(errcode_enum::king_war_error_not_king_war_family);
		return;
	}

	uint64_t help_family = p_country->get_king_war_help_family(kingwar_type);
	if (help_family == 0) {
		log_error("user[%lu] country[%u] king war not have help family", p_user->get_uid(), p_country->get_country_id());
		reply.set_reply_code(errcode_enum::king_war_error_help_cancle);
		return;
	}

	p_country->cancel_king_war_fight_invite(kingwar_type, p_family);

	reply.set_country_id(p_country->get_country_id());
	p_country->peek_king_war_fight_by_type(kingwar_type, p_user->get_uid(), reply.mutable_info());
	reply.set_reply_code(errcode_enum::error_ok);
}

void king_war_manager_t::king_war_cancel_help(const global_user_ptr& p_user, proto::client::ec_cencel_king_war_help_reply& reply)
{
	if (NULL == p_user) {
		log_error("p_user null error");
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	uint64_t family_id = p_user->get_family_id();
	if (0 == family_id) {
		log_error("user[%lu] family_id == 0", p_user->get_uid());
		reply.set_reply_code(errcode_enum::notice_no_family);
		return;
	}

	const family_ptr& p_family = family_manager_t::get_family(family_id);
	if (NULL == p_family) {
		log_error("user[%lu] p_family null error", p_user->get_uid());
		reply.set_reply_code(errcode_enum::family_err_code_invalid_family);
		return;
	}

	if (p_family->get_general_id() != p_user->get_uid()) {
		log_error("user[%lu] has no privilige get_king_war_help_family_list", p_user->get_uid());
		reply.set_reply_code(errcode_enum::family_err_code_no_right);
		return;
	}

	if (p_family->get_king_war_fight_country() == 0) {
		log_error("user[%lu] family not king war family", p_user->get_uid());
		reply.set_reply_code(errcode_enum::king_war_error_help_cancle);
		return;
	}

	country_ptr p_country = country_mgr_t::get_country(p_family->get_king_war_fight_country());
	if (NULL == p_country) {
		log_error("user[%lu] p_country null error", p_user->get_uid());
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	// 是否处于预告期
	const country_t::king_war_info& info = p_country->get_king_war_info();
	if (info.m_state != proto::common::king_war_state_pre) {
		log_error("user[%lu] country king war state[%u] error", p_user->get_uid(), info.m_state);
		reply.set_reply_code(errcode_enum::king_war_error_no_state_pre);
		return;
	}

	uint32_t kingwar_type = p_country->get_king_war_help_family_type(family_id);
	if (kingwar_type == country_t::king_war_type_error) {
		log_error("user[%lu] family not country[%u] king war family", p_user->get_uid(), p_country->get_country_id());
		reply.set_reply_code(errcode_enum::king_war_error_not_king_war_family);
		return;
	}

	p_country->cancel_king_war_fight_help(kingwar_type, p_family);

	reply.set_country_id(p_country->get_country_id());
	p_country->peek_king_war_fight_by_type(kingwar_type, p_user->get_uid(), reply.mutable_info());
	reply.set_reply_code(errcode_enum::error_ok);
}
