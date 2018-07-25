#include "country_mgr.hpp"
#include "main/center_server_fwd.hpp"
#include "main/center_server.hpp"
#include "redis/country_data_mgr.hpp"
#include "config/config_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "redis/global_data_mgr.hpp"
#include "family/family_manager.hpp"
#include "tblh/sys_notice_enum.hpp"
#include "common/time_manager.hpp"
#include "tblh/time_type_enum.hpp"
#include "sys_notice/sys_notice_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "tblh/CountryOfficial.tbls.h"
#include "redis/global_user_data_mgr.hpp"
#include "tblh/mailcode_enum.hpp"
#include "tblh/MailTable.tbls.h"
#include "mail/mail_manager.hpp"
#include "rank/rank_manager.hpp"
#include "log/log_wrapper.hpp"

#define WRITE_COUNTRY_LOG_TO_DB(country, type, param1, param2, param3) \
    log_wrapper_t::send_country_log(country, type, param1, param2, param3);

country_mgr_t::country_map country_mgr_t::m_country_map;

// proto::server::global_data_country_data country_mgr_t::m_global_country_data;

proto::server::global_data_king_war_data country_mgr_t::m_global_king_war_data;

// boost::shared_ptr<boost::asio::deadline_timer> country_mgr_t::m_open_timer;

bool country_mgr_t::init()
{
	// 加载国家数据
	if (!country_data_mgr_t::load_all_datas(m_country_map)) {
		log_error("load country data error");
		return false;
	}

	// 加载所有家族
	if (!family_manager_t::init())
	{
		log_error("init family failed");
		return false;
	}
	log_info("init family success!");

// 	// 国家全局数据
// 	global_data_mgr_t::load_country_data(proto::server::global_data_type_country, m_global_country_data);
// 
// 	// 国家时期处理
// 	switch (m_global_country_data.period_type()) {
// 		case proto::common::country_period_none: {
// 			if (time_manager_t::is_timer_activating(time_type_enum::time_country_unlock)) { // 还未到达开服N天
// 				time_manager_t::register_func(time_type_enum::time_country_unlock, country_mgr_t::open_establish_callback);
// 			} else {
// 				family_ptr p_family = family_manager_t::is_can_create_country();
// 				if (NULL != p_family) open_country_system(p_family->get_name(), false);
// 			}
// 			break;
// 		}
// 		case proto::common::country_period_establish: {
// 			uint32_t curr_time = common::time_util_t::now_time();
// 			if (curr_time >= m_global_country_data.period_time()) { // 时间已经到了
// 				m_global_country_data.set_period_type(proto::common::country_period_hegemony);
// 				// 保存
// 				global_data_mgr_t::save_country_data(proto::server::global_data_type_country, m_global_country_data);
// 
// 				WRITE_COUNTRY_LOG_TO_DB(0, common::log_enum::source_type_country_hegemony, 0, 0, 0);
// 			} else {
// 				uint32_t left_time = m_global_country_data.period_time() - curr_time;
// 				start_create_countdown(left_time);
// 			}
// 			
// 			break;
// 		}
// 		case proto::common::country_period_hegemony: {
// 			break;
// 		}
// 		default: {
// 			log_error("country period time[%u] not define", m_global_country_data.period_type());
// 			return false;
// 		}
// 	}

	// 王城战全局数据
	global_data_mgr_t::load_king_war_data(proto::server::global_data_type_king_war, m_global_king_war_data);
	uint32_t curr_time = common::time_util_t::now_time();
	switch (m_global_king_war_data.state()) {
		case proto::common::king_war_state_end: { // 结束或未开始
			// 不用做任何处理
			break;
		}
		case proto::common::king_war_state_sign_up: { // 报名阶段
			// 起服的时候，时间已经过了这个这场王城争夺战的最后时间
			if (curr_time >= m_global_king_war_data.battle_time()) {
				m_global_king_war_data.set_state(proto::common::king_war_state_end);
				m_global_king_war_data.set_state_end_time(0);
				m_global_king_war_data.set_battle_time(0);
				global_data_mgr_t::save_king_war_data(proto::server::global_data_type_king_war, m_global_king_war_data);
			} else if (curr_time >= m_global_king_war_data.state_end_time()) { // 起服的时候，时间已经过了这个这场王城争夺战的报名时间
				m_global_king_war_data.set_state(proto::common::king_war_state_pre);
				m_global_king_war_data.set_state_end_time(0);
				global_data_mgr_t::save_king_war_data(proto::server::global_data_type_king_war, m_global_king_war_data);
			}
			break;
		}
		case proto::common::king_war_state_pre: {
			// 起服的时候，时间已经过了这个这场王城争夺战的最后时间
			if (curr_time >= m_global_king_war_data.battle_time()) {
				m_global_king_war_data.set_state(proto::common::king_war_state_end);
				m_global_king_war_data.set_state_end_time(0);
				m_global_king_war_data.set_battle_time(0);
				global_data_mgr_t::save_king_war_data(proto::server::global_data_type_king_war, m_global_king_war_data);
			}
			break;
		}
		case proto::common::king_war_state_prepare: 
		case proto::common::king_war_state_start: {
			// 起服的时候，时间已经过了这个这场王城争夺战的最后时间
			m_global_king_war_data.set_state(proto::common::king_war_state_end);
			m_global_king_war_data.set_state_end_time(0);
			m_global_king_war_data.set_battle_time(0);
			global_data_mgr_t::save_king_war_data(proto::server::global_data_type_king_war, m_global_king_war_data);
			break;
		}
		default: {
			log_error("king_war_state[%u] error", m_global_king_war_data.state());
			break;
		}
	}

	// 对各个国家的家族排名
	for (country_map::const_iterator citr = m_country_map.begin();
		citr != m_country_map.end(); ++citr) {
		if (citr->second != NULL) {
			citr->second->init(curr_time);
		}
	}

	return true;
}

void country_mgr_t::close()
{
	family_manager_t::close();

	m_country_map.clear();

// 	if (NULL != m_open_timer) {
// 		m_open_timer->cancel();
// 	}
}

void country_mgr_t::one_day()
{
	auto conf = GET_CONF(Comprehensive, common::comprehensive_common::country_strength_player_rank);
	uint32_t strength_player_rank = GET_COMPREHENSIVE_VALUE_1(conf);

	conf = GET_CONF(Comprehensive, common::comprehensive_common::country_strength_player_add);
	uint32_t strength_player_add = GET_COMPREHENSIVE_VALUE_1(conf);

	std::map<uint32_t, uint32_t> active_user_num;
	rank_level_ptr p_rank_level = rank_manager_t::get_rank_level();
	if (p_rank_level) {
		std::vector<uint64_t> uids;
		p_rank_level->get_rank_user(strength_player_rank, uids);

		uint32_t curr_time = common::time_util_t::now_time();
		for (std::vector<uint64_t>::const_iterator citr = uids.begin();
			citr != uids.end(); ++citr) {
			uint32_t country_id = global_user_data_mgr_t::get_user_country_id(*citr);
			if (country_id == 0) continue;

			uint32_t update_time = global_user_data_mgr_t::get_update_time(*citr);
			if (time_manager_t::check_previous_day(curr_time, update_time)) {
				active_user_num[country_id] += 1;
			}
		}
	}

	struct country_sort_t
	{
		uint32_t m_id = 0;
		uint32_t m_power = 0;

		country_sort_t(uint32_t id, uint32_t value) : m_id(id), m_power(value) {};

		bool operator< (const country_sort_t& other)
		{
			if (this->m_power != other.m_power) {
				return this->m_power > other.m_power;
			}

			return this->m_id < other.m_id;
		}
	};

	std::vector<country_sort_t> all_sort;

	for (country_map::iterator itr = m_country_map.begin();
		itr != m_country_map.end(); ++itr) {
		if (itr->second != NULL) {
			uint32_t active_rank_add = 0;
			std::map<uint32_t, uint32_t>::const_iterator active_user_itr = active_user_num.find(itr->first);
			if (active_user_itr != active_user_num.end()) {
				active_rank_add = active_user_itr->second * strength_player_add;
			}
			uint32_t new_power = itr->second->update_new_power(active_rank_add);
			all_sort.push_back(country_sort_t(itr->first, new_power));
		}
	}

	std::sort(all_sort.begin(), all_sort.end());

	// 霸主判定
	if (all_sort.size() > 2) {
		if (all_sort[0].m_power - all_sort[1].m_power >= all_sort[1].m_power * 0.3) {
			country_ptr p_country = get_country(all_sort[0].m_id);
			if (p_country) {
				p_country->update_power_level(proto::common::country_level_overlord);
			}
		} else {
			country_ptr p_country = get_country(all_sort[0].m_id);
			if (p_country) {
				p_country->update_power_level(proto::common::country_level_strong);
			}
		}

		for (uint32_t i = 1; i < all_sort.size(); ++i) {
			if (all_sort[0].m_power - all_sort[i].m_power >= all_sort[i].m_power * 0.3) {
				country_ptr p_country = get_country(all_sort[i].m_id);
				if (p_country) {
					p_country->update_power_level(proto::common::country_level_weak);
				}
			} else {
				country_ptr p_country = get_country(all_sort[i].m_id);
				if (p_country) {
					p_country->update_power_level(proto::common::country_level_normal);
				}
			}
		}
	}
}

// void country_mgr_t::start_create_countdown(uint32_t left_time)
// {
// 	if (left_time == 0) {
// 		log_error("country_mgr_t left_time == 0");
// 		return;
// 	}
// 
// 	if (NULL == m_open_timer) {
// 		m_open_timer.reset(new boost::asio::deadline_timer(env::server->get_ios()));
// 	}
// 
// 	if (NULL == m_open_timer) {
// 		log_error("country_mgr_t m_open_timer null error");
// 		return;
// 	}
// 
// 	m_open_timer->expires_from_now(boost::posix_time::seconds(left_time));
// 	m_open_timer->async_wait(boost::bind(&country_mgr_t::countdown_callback, boost::asio::placeholders::error));
// }

// void country_mgr_t::countdown_callback(const boost::system::error_code& ec)
// {
// 	if (ec == boost::asio::error::operation_aborted) {
// 		return;
// 	} else if (ec) {
// 		log_error("Timer error: %s", ec.message().c_str());
// 		return;
// 	}
// 
// 	m_global_country_data.set_period_type(proto::common::country_period_hegemony);
// 	// 保存
// 	global_data_mgr_t::save_country_data(proto::server::global_data_type_country, m_global_country_data);
// 
// 	WRITE_COUNTRY_LOG_TO_DB(0, common::log_enum::source_type_country_hegemony, 0, 0, 0);
// }

// void country_mgr_t::open_establish_callback(uint32_t id, bool is_in_time)
// {
// 	if (is_in_time) {
// 		family_ptr p_family = family_manager_t::is_can_create_country();
// 		if (NULL != p_family) open_country_system(p_family->get_name());
// 	}
// }

// void country_mgr_t::open_country_system(const std::string& family_name, bool is_notify /*= true*/)
// {
// 	auto conf = GET_CONF(Comprehensive, comprehensive_common::country_unlock_cd);
// 	uint32_t unlock_cd = GET_COMPREHENSIVE_VALUE_1(conf);
// 	uint32_t left_time = unlock_cd * DAY_SECOND;
// 
// 	m_global_country_data.set_period_type(proto::common::country_period_establish);
// 	m_global_country_data.set_period_time(common::time_util_t::now_time() + left_time);
// 
// 	WRITE_COUNTRY_LOG_TO_DB(0, common::log_enum::source_type_country_period_establish, 0, 0, 0);
// 
// 	start_create_countdown(left_time);
// 
// 	global_data_mgr_t::save_country_data(proto::server::global_data_type_country, m_global_country_data);
// 
// 	if (is_notify) { // 广播
// 		BROADCAST_SYS_NOTICE(sys_notice_enum::sys_notice_301, 0, 0, 0, family_name.c_str());
// 
// 		// 通知所有在线玩家
// 		proto::client::ec_country_open_notify ntf;
// 		ntf.set_period_type(proto::common::country_period_establish);
// 		ntf.set_start_time(m_global_country_data.period_time());
// 
// 		std::string cmd_data;
// 		network::wrap_msg(cmd_data, ntf);
// 
// 		proto::server::ea_broadcast_notify msg_to_gate;
// 		msg_to_gate.set_cmd(op_cmd::ec_country_open_notify);
// 		msg_to_gate.set_data(cmd_data);
// 		env::server->send_msg_to_all_gate(op_cmd::ea_broadcast_notify, 0, msg_to_gate);
// 	}
// }

uint32_t country_mgr_t::get_king_war_state()
{
	return m_global_king_war_data.state();
}

void country_mgr_t::start_king_war_sign_up(uint32_t start_time, uint32_t state_end_time)
{
	m_global_king_war_data.set_state(proto::common::king_war_state_sign_up);
	m_global_king_war_data.set_state_end_time(state_end_time);
	m_global_king_war_data.set_battle_time(start_time);
	global_data_mgr_t::save_king_war_data(proto::server::global_data_type_king_war, m_global_king_war_data);

	for (const auto& pair : m_country_map) {
		if (NULL != pair.second) {
			pair.second->start_king_war_sign_up(start_time, state_end_time);
		}
	}
}

void country_mgr_t::end_king_war_sign_up()
{
	m_global_king_war_data.set_state(proto::common::king_war_state_pre);
	m_global_king_war_data.set_state_end_time(0);
	global_data_mgr_t::save_king_war_data(proto::server::global_data_type_king_war, m_global_king_war_data);

	for (const auto& pair : m_country_map) {
		if (NULL != pair.second) {
			pair.second->end_king_war_sign_up();
		}
	}
}

void country_mgr_t::start_king_war_prepare(uint32_t state_end_time)
{
	m_global_king_war_data.set_state(proto::common::king_war_state_prepare);
	m_global_king_war_data.set_state_end_time(state_end_time);
	global_data_mgr_t::save_king_war_data(proto::server::global_data_type_king_war, m_global_king_war_data);

	for (const auto& pair : m_country_map) {
		if (NULL != pair.second) {
			pair.second->start_king_war_prepare(state_end_time);
		}
	}
}

void country_mgr_t::end_king_war_prepare()
{
	for (const auto& pair : m_country_map) {
		if (NULL != pair.second) {
			pair.second->end_king_war_prepare();
		}
	}
}

void country_mgr_t::start_king_war_fight(uint32_t state_end_time)
{
	m_global_king_war_data.set_state(proto::common::king_war_state_start);
	m_global_king_war_data.set_state_end_time(state_end_time);
	global_data_mgr_t::save_king_war_data(proto::server::global_data_type_king_war, m_global_king_war_data);

	for (const auto& pair : m_country_map) {
		if (NULL != pair.second) {
			pair.second->start_king_war_fight(state_end_time);
		}
	}
}

void country_mgr_t::end_king_war_fight()
{
	m_global_king_war_data.set_state(proto::common::king_war_state_end);
	m_global_king_war_data.set_state_end_time(0);
	m_global_king_war_data.set_battle_time(0);
	global_data_mgr_t::save_king_war_data(proto::server::global_data_type_king_war, m_global_king_war_data);

	for (const auto& pair : m_country_map) {
		if (NULL != pair.second) {
			pair.second->end_king_war_fight();
		}
	}
}

bool country_mgr_t::is_name_repeate(const std::string& name, uint32_t except)
{
	for (const auto& pair : m_country_map) {
		if (pair.first == except) continue;

		if (NULL != pair.second) {
			if (0 == strcmp(name.c_str(), pair.second->get_country_name().c_str())) {
				return true;
			}
		}
	}

	return false;
}

void country_mgr_t::world_trend_info(const global_user_ptr& p_user, proto::common::country_king_war_info* info)
{
	if (p_user == NULL) {
		log_error("p_user null error");
		return;
	}

	if (info == NULL) {
		log_error("info null error");
		return;
	}

	info->set_state(m_global_king_war_data.state());

	switch (m_global_king_war_data.state()) {
		case proto::common::king_war_state_end: { // 结束或未开始
			break;
		}
		case proto::common::king_war_state_sign_up: { // 报名阶段
			country_mgr_t::get_king_war_sign_up_info(p_user->get_family_id(), info->mutable_sign_up_info());
			break;
		}
		case proto::common::king_war_state_pre:
		case proto::common::king_war_state_prepare:
		case proto::common::king_war_state_start: {
			country_mgr_t::get_king_war_pre_info(p_user->get_uid(), info->mutable_pre_info());
			break;
		}
		default: {
			log_error("king_war_state[%u] error", m_global_king_war_data.state());
			break;
		}
	}
}

void country_mgr_t::get_king_war_sign_up_info(uint64_t family_id, proto::common::king_war_country_sign_up_info* info)
{
	if (info) {
		info->set_end_time(m_global_king_war_data.state_end_time());

		for (country_map::const_iterator citr = m_country_map.begin();
			citr != m_country_map.end(); ++citr) {
			if (citr->second && citr->second->get_king_war_state() == proto::common::king_war_state_sign_up) {
				citr->second->get_king_war_sign_up(family_id, info->add_countries());
			}
		}
	}
}

void country_mgr_t::get_king_war_pre_info(uint64_t uid, proto::common::king_war_country_pre_info* info)
{
	if (info) {
		if (m_global_king_war_data.state() != proto::common::king_war_state_start) {
			info->set_fight_time(m_global_king_war_data.battle_time());
		} else {
			info->set_fight_time(m_global_king_war_data.state_end_time());
		}

		for (country_map::const_iterator citr = m_country_map.begin();
			citr != m_country_map.end(); ++citr) {
			if (citr->second && citr->second->get_king_war_state() > proto::common::king_war_state_sign_up) {
				citr->second->get_king_war_fight(uid, info->add_countries());
			}
		}
	}
}

void country_mgr_t::user_online_king_war_check(const global_user_ptr& p_user, uint64_t family_uid)
{
	if (p_user == NULL) {
		log_error("p_user null error");
		return;
	}

	if (m_global_king_war_data.state() != proto::common::king_war_state_pre) {
		return;
	}

	proto::client::ec_king_war_invite_help_notify ntf;
	for (const auto& pair : m_country_map) {
		if (NULL != pair.second) {
			for (uint32_t i = 0; i < country_t::king_war_type_max; ++i) {
				uint64_t invited = pair.second->is_king_war_fight_invite(i, family_uid);
				if (invited != 0) {
					family_ptr p_invited_family = family_manager_t::get_family(invited);
					if (p_invited_family) {
						proto::common::king_war_helf_info* p_info = ntf.add_infos();
						if (p_info) {
							p_info->set_family_uid(common::string_util_t::uint64_to_string(invited));
							p_info->set_family_name(p_invited_family->get_name());
							p_info->set_country(pair.first);
							p_info->set_fight_time(m_global_king_war_data.battle_time());
						}
					}
				}
			}
		}
	}

	if (ntf.infos_size() != 0) {
		p_user->send_msg_to_client(op_cmd::ec_king_war_invite_help_notify, ntf);
	}
}

void country_mgr_t::clear_king_war_invite_info(const global_user_ptr& p_user)
{
	if (p_user == NULL) {
		log_error("p_user null error");
		return;
	}

	if (m_global_king_war_data.state() != proto::common::king_war_state_pre) {
		return;
	}

	uint64_t family_id = p_user->get_family_id();
	if (family_id != 0) {
		for (const auto& pair : m_country_map) {
			if (NULL != pair.second) {
				pair.second->del_all_king_war_fight_invite(family_id);
			}
		}
	}
}

uint32_t country_mgr_t::get_king_family_num()
{
	uint32_t num = 0;
	for (const auto& pair : m_country_map) {
		if (NULL != pair.second && pair.second->get_king_family() != 0) {
			num++;
		}
	}

	return num;
}

void country_mgr_t::sync_country_data(const network::tcp_socket_ptr& s)
{
	proto::server::eg_update_country_notify ntf;
	for (country_map::const_iterator citr = m_country_map.begin();
		citr != m_country_map.end(); ++citr) {
		if (NULL != citr->second) {
			citr->second->peek_common_data(ntf.add_countries());
		}
	}

	env::server->send_msg_to_game(s, op_cmd::eg_update_country_notify, 0, ntf);
}

void country_mgr_t::get_country_preview(proto::client::ec_country_preview_reply& reply)
{
	for (country_map::const_iterator citr = m_country_map.begin();
		citr != m_country_map.end(); ++citr) {
		if (citr->second != NULL) {
			citr->second->peek_base_data(reply.add_datas());
		}
	}
}

void country_mgr_t::get_country_info(uint32_t country_id, proto::client::ec_country_info_reply& reply)
{
	country_ptr p_country = country_mgr_t::get_country(country_id);
	if (!p_country) {
		log_error("country[%u] not define!", country_id);
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	p_country->peek_country_data(reply);
	reply.set_reply_code(common::errcode_enum::error_ok);
}

void country_mgr_t::ask_country_alliance(const global_user_ptr& p_user, uint32_t country_id, proto::client::ec_country_alliance_reply& reply)
{
	if (NULL == p_user) {
		log_error("p_user == null");
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	family_ptr p_family = family_manager_t::get_family(p_user->get_family_id());
	if (!p_family) {
		log_error("user[%lu] not join family!", p_user->get_uid());
		reply.set_reply_code(common::errcode_enum::notice_no_family);
		return;
	}

	if (p_family->get_country_id() != 0) {
		log_error("user[%lu] family[%lu] is already in other country[%u]", p_user->get_uid(), p_user->get_family_id(), p_family->get_country_id());
		reply.set_reply_code(common::errcode_enum::country_error_already_in_other_country);
		return;
	}

	if (p_family->get_general_id() != p_user->get_uid()) {
		log_error("user[%lu] not family general!", p_user->get_uid());
		reply.set_reply_code(common::errcode_enum::country_error_alliance_not_general);
		return;
	}

	country_ptr p_country = country_mgr_t::get_country(country_id);
	if (!p_country) {
		log_error("user[%lu] country[%u] not define!", p_user->get_uid(), country_id);
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	uint64_t king_family_id = p_country->get_king_family();
	if (king_family_id == 0) {
		log_error("user[%lu] country[%u] have no king family", p_user->get_uid(), country_id);
		reply.set_reply_code(common::errcode_enum::country_error_no_king_family);
		return;
	}

	family_ptr p_king_family = family_manager_t::get_family(king_family_id);
	if (!p_king_family) {
		log_error("user[%lu] country[%u] king family[%lu] null error", p_user->get_uid(), country_id, king_family_id);
		reply.set_reply_code(common::errcode_enum::country_error_no_king_family);
		return;
	}

	auto conf = GET_CONF(Comprehensive, comprehensive_common::country_join_para1);
	uint32_t join_param1 = GET_COMPREHENSIVE_VALUE_1(conf);
	if (p_family->get_level() < join_param1) {
		log_error("user[%lu] family level[%u] < %u", p_user->get_uid(), p_family->get_level(), join_param1);
		reply.set_reply_code(common::errcode_enum::country_error_level_insufficient);
		return;
	}

	conf = GET_CONF(Comprehensive, comprehensive_common::country_join_para2);
	uint32_t join_param2 = GET_COMPREHENSIVE_VALUE_1(conf);
	if (p_family->get_prosperity() < join_param2) {
		log_error("user[%lu] family prosperity[%u] < %u", p_user->get_uid(), p_family->get_prosperity(), join_param2);
		reply.set_reply_code(common::errcode_enum::country_error_prosperity_insufficient);
		return;
	}

	p_country->join_country(p_family);

	// 通知
	proto::client::ec_country_alliance_notify notify;
	notify.set_country_id(country_id);
	notify.set_family_name(p_king_family->get_name());
	p_family->send_msg_to_family(op_cmd::ec_country_alliance_notify, notify);

	MailTable* p_mail = GET_CONF(MailTable, common::mailcode_enum::sys_country_alliance);
	if (p_mail) {
		std::string content_text;
		StringTool::format(content_text, common::string_util_t::convert_to_utf8(p_mail->content()).c_str(), p_country->get_country_name().c_str(), p_king_family->get_name().c_str(), p_country->get_country_name().c_str());

		std::vector<uint64_t> uids;
		const family::family_member_map& member_map = p_family->get_all_member();
		for (family::family_member_map::const_iterator citr = member_map.begin();
			citr != member_map.end(); ++citr) {
			if (NULL != citr->second) {
				uids.push_back(citr->second->get_uid());
			}
		}

		mail_manager_t::send_mail(uids, proto::common::MAIL_TYPE_SYS, 0, common::string_util_t::convert_to_utf8(p_mail->from_name()), common::string_util_t::convert_to_utf8(p_mail->title()), content_text, std::map<uint32_t, uint32_t>());
	}

	reply.set_reply_code(common::errcode_enum::error_ok);
	reply.set_country_id(country_id);
	reply.set_family_name(p_king_family->get_name());
}

uint32_t country_mgr_t::quit_country_alliance(const global_user_ptr& p_user)
{
	if (NULL == p_user) {
		log_error("p_user == null");
		return common::errcode_enum::notice_unknown;
	}

	family_ptr p_family = family_manager_t::get_family(p_user->get_family_id());
	if (!p_family) {
		log_error("user[%lu] not join family!", p_user->get_uid());
		return common::errcode_enum::notice_no_family;
	}

	if (p_family->get_country_id() == 0) {
		log_error("user[%lu] family[%lu] country == 0", p_user->get_uid(), p_user->get_family_id());
		return common::errcode_enum::country_error_not_in_country;
	}

	if (p_family->is_king_family()) {
		log_error("user[%lu] family[%lu] is king family", p_user->get_uid(), p_user->get_family_id());
		return common::errcode_enum::country_error_king_can_not_levae;
	}

	country_ptr p_country = country_mgr_t::get_country(p_family->get_country_id());
	if (!p_country) {
		log_error("user[%lu] country[%u] not define!", p_user->get_uid(), p_family->get_country_id());
		return common::errcode_enum::notice_unknown;
	}

	if (p_family->get_general_id() != p_user->get_uid()) {
		log_error("user[%lu] not family general!", p_user->get_uid());
		return common::errcode_enum::country_error_alliance_not_general;
	}

	if (!p_family->check_quit_country_times()) {
		log_error("user[%lu] family quit country max times", p_user->get_uid());
		return common::errcode_enum::country_error_quit_max_time;
	}

	p_country->quit_country(p_family);

	return common::errcode_enum::error_ok;
}

uint32_t country_mgr_t::modify_year_name(const global_user_ptr& p_user, const std::string& name)
{
	if (NULL == p_user) {
		log_error("p_user == null");
		return common::errcode_enum::notice_unknown;
	}

	country_ptr p_country = country_mgr_t::get_country(p_user->get_country_id());
	if (!p_country) {
		log_error("user[%lu] country[%u] not define!", p_user->get_uid(), p_user->get_country_id());
		return common::errcode_enum::notice_unknown;
	}

	if (p_user->get_country_officer() == officer_type::officer_type_none || !country_t::has_family_privilige(p_user->get_country_officer(), privilige_type::country_edit_title)) {
		log_error("user[%lu] have no right modify year name", p_user->get_uid());
		return common::errcode_enum::country_error_no_right;
	}

	return p_country->modify_year_name(p_user, name);
}

void country_mgr_t::modify_country_year_name_back(const global_user_ptr& p_user, const proto::server::ge_country_year_name_reply& reply)
{
	if (NULL == p_user) {
		log_error("p_user == null");
		return;
	}

	if (reply.reply_code() != common::errcode_enum::error_ok) {
		COUNTRY_LOG("user[%lu] modify_year_name deduction of money error:%u", p_user->get_uid(), reply.reply_code());
		return;
	}

	country_ptr p_country = country_mgr_t::get_country(p_user->get_country_id());
	if (!p_country) {
		log_error("user[%lu] country[%u] not define!", p_user->get_uid(), p_user->get_country_id());
		return;
	}

	// 修改名字
	p_country->do_modify_year_name(p_user->get_uid(), reply.name());

	proto::client::ec_modify_year_name_reply ntf;
	ntf.set_reply_code(common::errcode_enum::error_ok);
	ntf.set_is_free(0);
	p_country->peek_country_info(ntf.mutable_data());
	proto::common::role_change_data* p_rcd = ntf.mutable_change_data();
	if (p_rcd) {
		p_rcd->CopyFrom(reply.change_data());
	}
	p_user->send_msg_to_client(op_cmd::ec_modify_year_name_reply, ntf);
}

uint32_t country_mgr_t::modify_country_notice(const global_user_ptr& p_user, const std::string& notice)
{
	if (NULL == p_user) {
		log_error("p_user == null");
		return common::errcode_enum::notice_unknown;
	}

	country_ptr p_country = country_mgr_t::get_country(p_user->get_country_id());
	if (!p_country) {
		log_error("user[%lu] country[%u] not define!", p_user->get_uid(), p_user->get_country_id());
		return common::errcode_enum::notice_unknown;
	}

	if (p_user->get_country_officer() == officer_type::officer_type_none || !country_t::has_family_privilige(p_user->get_country_officer(), privilige_type::country_edit_announce)) {
		log_error("user[%lu] have no right modify notice", p_user->get_uid());
		return common::errcode_enum::country_error_no_right;
	}

	auto conf = GET_CONF(Comprehensive, comprehensive_common::country_announce_number_upper);
	uint32_t announce_number = GET_COMPREHENSIVE_VALUE_1(conf);
	if (notice.size() > announce_number * 3) {
		log_error("user[%lu] modify notice size[%u] > %u", p_user->get_uid(), (int32_t)notice.size(), announce_number);
		return common::errcode_enum::country_error_notice_max_size;
	}

	p_country->modify_notice(p_user->get_uid(), notice);

	return common::errcode_enum::error_ok;
}

void country_mgr_t::open_appoint_request(const global_user_ptr& p_user, uint32_t officer_id, proto::client::ec_open_appoint_reply& reply)
{
	if (NULL == p_user) {
		log_error("p_user == null");
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	CountryOfficial* p_conf = GET_CONF(CountryOfficial, officer_id);
	if (!p_conf) {
		log_error("user[%lu] have no officer[%u]", p_user->get_uid(), officer_id);
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	country_ptr p_country = country_mgr_t::get_country(p_user->get_country_id());
	if (!p_country) {
		log_error("user[%lu] country[%u] not define!", p_user->get_uid(), p_user->get_country_id());
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	if (p_country->is_officer_appoint(officer_id)) {
		log_error("user[%lu] country[%u] officer[%u] is appointed!", p_user->get_uid(), p_user->get_country_id(), officer_id);
		reply.set_reply_code(common::errcode_enum::country_error_appoint_error1);
		return;
	}

	if (p_user->get_country_officer() == officer_type::officer_type_none || !country_t::has_family_privilige(p_user->get_country_officer(), p_conf->need_power())) {
		log_error("user[%lu] have no right appoint officer[%u]", p_user->get_uid(), officer_id);
		reply.set_reply_code(common::errcode_enum::country_error_no_right);
		return;
	}

	family_ptr p_family = family_manager_t::get_family(p_country->get_king_family());
	if (!p_family) {
		log_error("user[%lu] can not find country[%u] king family", p_user->get_uid(), p_country->get_country_id());
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	p_family->get_family_member_list(reply);
	reply.set_reply_code(common::errcode_enum::error_ok);
}

void country_mgr_t::appoint_officer_request(const global_user_ptr& p_user, uint64_t target_uid, uint32_t officer_id, proto::client::ec_appoint_officer_reply& reply)
{
	if (NULL == p_user) {
		log_error("p_user == null");
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	CountryOfficial* p_conf = GET_CONF(CountryOfficial, officer_id);
	if (!p_conf) {
		log_error("user[%lu] have no officer[%u]", p_user->get_uid(), officer_id);
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	country_ptr p_country = country_mgr_t::get_country(p_user->get_country_id());
	if (!p_country) {
		log_error("user[%lu] country[%u] not define!", p_user->get_uid(), p_user->get_country_id());
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	if (p_country->is_officer_appoint(officer_id)) {
		log_error("user[%lu] country[%u] officer[%u] is appointed!", p_user->get_uid(), p_user->get_country_id(), officer_id);
		reply.set_reply_code(common::errcode_enum::country_error_appoint_error1);
		return;
	}

	if (p_country->is_officer_appoint_cd(officer_id)) {
		log_error("user[%lu] country[%u] officer[%u] is cd!", p_user->get_uid(), p_user->get_country_id(), officer_id);
		reply.set_reply_code(common::errcode_enum::country_error_appoint_cd);
		return;
	}

	if (p_user->get_country_officer() == officer_type::officer_type_none || !country_t::has_family_privilige(p_user->get_country_officer(), p_conf->need_power())) {
		log_error("user[%lu] have no right appoint officer[%u]", p_user->get_uid(), officer_id);
		reply.set_reply_code(common::errcode_enum::country_error_no_right);
		return;
	}

	global_user_ptr p_target_user = global_user_manager_t::get_global_user_by_uid(target_uid);
	if (p_target_user) {
		if (p_target_user->get_family_id() != p_user->get_family_id()) {
			log_error("user[%lu] target_uid[%lu] is not same family", p_user->get_uid(), target_uid);
			reply.set_reply_code(common::errcode_enum::country_error_appoint_error3);
			return;
		}

		if (p_target_user->get_country_officer() != 0) {
			log_error("user[%lu] target_uid[%lu] is already have officer[%u]", p_user->get_uid(), target_uid, p_target_user->get_country_officer());
			reply.set_reply_code(common::errcode_enum::country_error_appoint_error2);
			return;
		}

		p_country->set_officer_appoint(officer_id, p_target_user, reply.mutable_officers());
	} else {
		if (!global_user_data_mgr_t::is_user_key_exsit(target_uid)) {
			log_error("user[%lu] appoint_officer target_uid[%lu] not find", p_user->get_uid(), target_uid);
			reply.set_reply_code(common::errcode_enum::notice_unknown);
			return;
		}

		if (global_user_data_mgr_t::get_user_family_id(target_uid) != p_user->get_family_id()) {
			log_error("user[%lu] target_uid[%lu] is not same family", p_user->get_uid(), target_uid);
			reply.set_reply_code(common::errcode_enum::country_error_appoint_error3);
			return;
		}

		if (global_user_data_mgr_t::get_user_country_officer(target_uid) != 0) {
			log_error("user[%lu] target_uid[%lu] is already have officer", p_user->get_uid(), target_uid);
			reply.set_reply_code(common::errcode_enum::country_error_appoint_error2);
			return;
		}

		p_country->set_officer_appoint(officer_id, target_uid, reply.mutable_officers());
	}

	reply.set_reply_code(common::errcode_enum::error_ok);
}

void country_mgr_t::fire_officer_request(const global_user_ptr& p_user, uint32_t officer_id, proto::client::ec_fire_officer_reply& reply)
{
	if (NULL == p_user) {
		log_error("p_user == null");
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	CountryOfficial* p_conf = GET_CONF(CountryOfficial, officer_id);
	if (!p_conf) {
		log_error("user[%lu] have no officer[%u]", p_user->get_uid(), officer_id);
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	country_ptr p_country = country_mgr_t::get_country(p_user->get_country_id());
	if (!p_country) {
		log_error("user[%lu] country[%u] not define!", p_user->get_uid(), p_user->get_country_id());
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	if (!p_country->is_officer_appoint(officer_id)) {
		log_error("user[%lu] country[%u] officer[%u] is empty", p_user->get_uid(), p_user->get_country_id(), officer_id);
		reply.set_reply_code(common::errcode_enum::country_error_appoint_error1);
		return;
	}

	if (p_user->get_country_officer() == officer_type::officer_type_none || !country_t::has_family_privilige(p_user->get_country_officer(), p_conf->need_power())) {
		log_error("user[%lu] have no right appoint officer[%u]", p_user->get_uid(), officer_id);
		reply.set_reply_code(common::errcode_enum::country_error_no_right);
		return;
	}

	p_country->fire_officer(officer_id, reply.mutable_officers());
	reply.set_reply_code(common::errcode_enum::error_ok);
}

void country_mgr_t::resign_officer_request(const global_user_ptr& p_user, proto::client::ec_resign_officer_reply& reply)
{
	if (NULL == p_user) {
		log_error("p_user == null");
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	country_ptr p_country = country_mgr_t::get_country(p_user->get_country_id());
	if (!p_country) {
		log_error("user[%lu] country[%u] not define!", p_user->get_uid(), p_user->get_country_id());
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	if (p_user->get_country_officer() == 0) {
		log_error("user[%lu] not officer", p_user->get_uid());
		reply.set_reply_code(common::errcode_enum::country_error_appoint_error4);
		return;
	}

	if (p_user->get_country_officer() == officer_type::officer_type_king) {
		log_error("user[%lu] is king, can't resign officer", p_user->get_uid());
		reply.set_reply_code(common::errcode_enum::country_error_appoint_error5);
		return;
	}

	p_country->resign_officer(p_user, reply.mutable_officers());
	reply.set_reply_code(common::errcode_enum::error_ok);
}

void country_mgr_t::get_country_flag_list(const global_user_ptr& p_user, proto::client::ec_country_flag_list_reply& reply)
{
	if (NULL == p_user) {
		log_error("p_user == null");
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	country_ptr p_country = country_mgr_t::get_country(p_user->get_country_id());
	if (!p_country) {
		log_error("user[%lu] country[%u] not define!", p_user->get_uid(), p_user->get_country_id());
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	if (p_user->get_country_officer() == officer_type::officer_type_none || !country_t::has_family_privilige(p_user->get_country_officer(), privilige_type::country_edit_title)) {
		log_error("user[%lu] have no right modify country flag", p_user->get_uid());
		reply.set_reply_code(common::errcode_enum::country_error_no_right);
		return;
	}

	reply.set_reply_code(common::errcode_enum::error_ok);
	p_country->peek_country_flag(reply);
}

uint32_t country_mgr_t::modify_country_name_flag(const global_user_ptr& p_user, uint32_t flag, const std::string& name)
{
	if (NULL == p_user) {
		log_error("p_user == null");
		return common::errcode_enum::notice_unknown;
	}

	country_ptr p_country = country_mgr_t::get_country(p_user->get_country_id());
	if (!p_country) {
		log_error("user[%lu] country[%u] not define!", p_user->get_uid(), p_user->get_country_id());
		return common::errcode_enum::notice_unknown;
	}

	if (p_user->get_country_officer() == officer_type::officer_type_none || !country_t::has_family_privilige(p_user->get_country_officer(), privilige_type::country_edit_title)) {
		log_error("user[%lu] have no right modify country flag", p_user->get_uid());
		return common::errcode_enum::country_error_no_right;
	}

	return p_country->modify_name_flag(p_user, name, flag);
}

void country_mgr_t::modify_country_name_flag_back(const global_user_ptr& p_user, const proto::server::ge_country_name_change_reply& reply)
{
	if (NULL == p_user) {
		log_error("p_user == null");
		return;
	}

	if (reply.reply_code() != common::errcode_enum::error_ok) {
		COUNTRY_LOG("user[%lu] modify_country_name deduction of money error:%u", p_user->get_uid(), reply.reply_code());
		return;
	}

	country_ptr p_country = country_mgr_t::get_country(p_user->get_country_id());
	if (!p_country) {
		log_error("user[%lu] country[%u] not define!", p_user->get_uid(), p_user->get_country_id());
		return;
	}

	// 修改名字
	p_country->do_modify_country_name(p_user->get_uid(), reply.name());
	// 修改旗帜
	p_country->do_modify_country_flag(p_user->get_uid(), reply.flag_id());

	proto::client::ec_change_country_name_reply ntf;
	ntf.set_reply_code(common::errcode_enum::error_ok);
	ntf.set_is_free(0);
	p_country->peek_country_info(ntf.mutable_data());
	proto::common::role_change_data* p_rcd = ntf.mutable_change_data();
	if (p_rcd) {
		p_rcd->CopyFrom(reply.change_data());
	}
	p_user->send_msg_to_client(op_cmd::ec_change_country_name_reply, ntf);

	// 同步game
	proto::server::eg_update_country_notify ntf_game;
	p_country->peek_common_data(ntf_game.add_countries());
	env::server->send_msg_to_all_games(op_cmd::eg_update_country_notify, 0, ntf_game);

	// 全服广播这个国家的名字改了
	proto::client::ec_update_country_notify notify;
	p_country->peek_common_data(notify.mutable_country());
	sys_notice_manager_t::broadcast_msg_to_all(op_cmd::ec_update_country_notify, notify);
}

country_ptr country_mgr_t::get_country(uint32_t id)
{
	country_map::iterator itr = m_country_map.find(id);
	if (itr != m_country_map.end()) {
		return itr->second;
	}

	return NULL;
}

void country_mgr_t::get_country_common(uint32_t id, proto::common::country_common* p_base)
{
	country_map::const_iterator itr = m_country_map.find(id);
	if (itr != m_country_map.end() && NULL != itr->second) {
		itr->second->peek_common_data(p_base);
	}
}

