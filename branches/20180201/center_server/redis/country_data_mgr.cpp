#include "country_data_mgr.hpp"
#include <vector>

#include "redis_client.hpp"
#include "utility.hpp"
#include "log.hpp"
#include "config/config_manager.hpp"
#include "tblh/Country.tbls.h"
#include "global_user_data_mgr.hpp"
#include "family_data_mgr.hpp"
#include "tblh/CountryOfficial.tbls.h"

bool country_data_mgr_t::load_all_datas(country_mgr_t::country_map& countries)
{
	countries.clear();

	// 国家配置表
	std::map<uint32_t, Country*> all_Country;
	GET_ALL_CONF(Country, all_Country);

	// 官职配置表
	std::map<uint32_t, CountryOfficial*> all_Officials;
	GET_ALL_CONF(CountryOfficial, all_Officials);

	for (std::map<uint32_t, Country*>::iterator itr = all_Country.begin();
		itr != all_Country.end(); ++itr) {
		Country* p_conf = itr->second;
		if (!p_conf) {
			log_error("NULL == p_conf");
			return false;
		}

		country_ptr p_country(new country_t(itr->first));
		if (NULL == p_country) {
			log_error("NULL == p_country");
			return false;
		}

		if (!is_country_key_exsit(itr->first)) { //新国家处理
			log_info("country[%u] is new, init base data", itr->first);

			// 初始化国家基础信息，主要是为了存一下
			redis_client_t::set_string("country", "name", itr->first, common::string_util_t::convert_to_utf8(p_conf->name()));
			redis_client_t::set_uint32("country", "flag_id", itr->first, 1);	// 旗子默认值
			redis_client_t::set_uint32("country", "free_times", itr->first, 0);
			redis_client_t::set_uint32("country", "free_times2", itr->first, 0);
			redis_client_t::set_uint32("country", "money", itr->first, 0);
			redis_client_t::set_uint32("country", "power", itr->first, 0);
			redis_client_t::set_uint32("country", "level_type", itr->first, 0);
			redis_client_t::set_uint64("country", "king_family", itr->first, 0);
			redis_client_t::set_uint32("country", "state", itr->first, (uint32_t)proto::common::king_war_state_end);

			// 初始化官职
			for (std::map<uint32_t, CountryOfficial*>::iterator official_itr = all_Officials.begin();
				official_itr != all_Officials.end(); ++official_itr) {
				uint32_t key = (itr->first << 16) | official_itr->first;

				redis_client_t::set_uint32("country_office", "country_id", key, itr->first);
				redis_client_t::set_uint32("country_office", "country_officer_type", key, official_itr->first);
				redis_client_t::set_uint64("country_office", "role_uid", key, 0);
				redis_client_t::set_uint32("country_office", "reappoint_time", key, 0);
			}
		} 

		p_country->m_country_info.m_king_family_id = redis_client_t::get_uint64("country", "king_family", itr->first);

		// 加载基础信息
		proto::common::country_base_data* p_base_info = p_country->m_country_info.info.mutable_base();
		if (p_base_info) {
			p_base_info->set_id(itr->first);
			if (p_country->m_country_info.m_king_family_id != 0) {
				p_base_info->set_king_family(family_data_mgr_t::get_family_name(p_country->m_country_info.m_king_family_id));
			}
			p_base_info->set_notice(redis_client_t::get_string("country", "notice", itr->first));
			p_base_info->set_name(redis_client_t::get_string("country", "name", itr->first));
			p_base_info->set_flag(redis_client_t::get_uint32("country", "flag_id", itr->first));
			p_base_info->set_year_name(redis_client_t::get_string("country", "year_name", itr->first));
		}

		p_country->m_country_info.info.set_money(redis_client_t::get_uint32("country", "money", itr->first));
		p_country->m_country_info.info.set_power(redis_client_t::get_uint32("country", "power", itr->first));
		p_country->m_country_info.info.set_level_type(redis_client_t::get_uint32("country", "level_type", itr->first));
		p_country->m_country_info.info.set_free_times(redis_client_t::get_uint32("country", "free_times", itr->first));
		p_country->m_country_info.info.set_free_times2(redis_client_t::get_uint32("country", "free_times2", itr->first));

		std::string flag_list = redis_client_t::get_string("country", "flag_list", itr->first);
		if (!flag_list.empty()) {
			std::vector<uint32_t> item_vec;
			common::string_util_t::split<std::uint32_t>(flag_list, item_vec, "$");
			for (const auto& item : item_vec) {
				p_country->m_country_info.m_flag_list.insert(item);
			}
		}

		// 加载官职信息
		for (std::map<uint32_t, CountryOfficial*>::iterator official_itr = all_Officials.begin();
			official_itr != all_Officials.end(); ++official_itr) {
			uint32_t key = (itr->first << 16) | official_itr->first;
			
			if (!redis_client_t::is_key_exist("country_office", key)) { // 对于新的官职初始化
				redis_client_t::set_uint32("country_office", "country_id", key, itr->first);
				redis_client_t::set_uint32("country_office", "country_officer_type", key, official_itr->first);
				redis_client_t::set_uint64("country_office", "role_uid", key, 0);
				redis_client_t::set_uint32("country_office", "reappoint_time", key, 0);
			}

			uint64_t officer_uid = redis_client_t::get_uint64("country_office", "role_uid", key);
			p_country->m_country_info.officers[official_itr->first].set_uid(common::string_util_t::uint64_to_string(officer_uid));
			if (officer_uid != 0) {
				p_country->m_country_info.officers[official_itr->first].set_name(global_user_data_mgr_t::get_name(officer_uid));
				p_country->m_country_info.officers[official_itr->first].set_plugin(global_user_data_mgr_t::get_user_hero_plugin(officer_uid));
				p_country->m_country_info.officers[official_itr->first].set_level(global_user_data_mgr_t::get_user_level(officer_uid));
				p_country->m_country_info.officers[official_itr->first].set_fighting(global_user_data_mgr_t::get_user_fighting_value(officer_uid));
			}

			p_country->m_country_info.officers[official_itr->first].set_type(official_itr->first);
			p_country->m_country_info.officers[official_itr->first].set_refresh(redis_client_t::get_uint32("country_office", "reappoint_time", key));
			if (officer_uid != 0 && official_itr->first == officer_type::officer_type_king && p_base_info) {
				p_base_info->set_king_name(p_country->m_country_info.officers[official_itr->first].name());
			}
		}

		// 王城战信息
		p_country->m_country_info.m_king_war_info.m_state = (proto::common::king_war_state)redis_client_t::get_uint32("country", "state", itr->first);
		p_country->m_country_info.m_king_war_info.m_family_defensive = redis_client_t::get_uint64("country", "family_defensive", itr->first);
		p_country->m_country_info.m_king_war_info.m_defensive_cost = redis_client_t::get_uint32("country", "defensive_cost", itr->first);
		p_country->m_country_info.m_king_war_info.m_family_attack = redis_client_t::get_uint64("country", "family_attack", itr->first);
		p_country->m_country_info.m_king_war_info.m_attack_cost = redis_client_t::get_uint32("country", "attack_cost", itr->first);
		p_country->m_country_info.m_king_war_info.m_state_end_time = redis_client_t::get_uint32("country", "state_end_time", itr->first);
		p_country->m_country_info.m_king_war_info.m_start_time = redis_client_t::get_uint32("country", "war_start_time", itr->first);

		std::pair<country_mgr_t::country_map::iterator, bool> ret = countries.insert(country_mgr_t::country_map::value_type(itr->first, p_country));
		if (!ret.second) {
			log_error("country[%u] insert error, it is exist", itr->first);
			return false;
		}
	}

	return true;
}

void country_data_mgr_t::update_country_year_name(uint32_t country_id, const std::string& name)
{
	if (!is_country_key_exsit(country_id)) {
		log_error("country[%u] is not exsit", country_id);
		return;
	}

	redis_client_t::set_string("country", "year_name", country_id, name);
}

void country_data_mgr_t::update_country_notice(uint32_t country_id, const std::string& notice)
{
	if (!is_country_key_exsit(country_id)) {
		log_error("country[%u] is not exsit", country_id);
		return;
	}

	redis_client_t::set_string("country", "notice", country_id, notice);
}

void country_data_mgr_t::update_country_money(uint32_t country_id, uint32_t val)
{
	if (!is_country_key_exsit(country_id)) {
		log_error("country[%u] is not exsit", country_id);
		return;
	}

	redis_client_t::set_uint32("country", "money", country_id, val);
}

void country_data_mgr_t::update_country_power(uint32_t country_id, uint32_t val)
{
	if (!is_country_key_exsit(country_id)) {
		log_error("country[%u] is not exsit", country_id);
		return;
	}

	redis_client_t::set_uint32("country", "power", country_id, val);
}

void country_data_mgr_t::update_power_level(uint32_t country_id, uint32_t val)
{
	if (!is_country_key_exsit(country_id)) {
		log_error("country[%u] is not exsit", country_id);
		return;
	}

	redis_client_t::set_uint32("country", "level_type", country_id, val);
}

void country_data_mgr_t::save_country_officer(uint32_t country_id, uint32_t officer_id, const proto::common::country_officer_data& data)
{
	if (!is_country_key_exsit(country_id)) {
		log_error("country[%u] is not exsit", country_id);
		return;
	}

	uint32_t key = (country_id << 16) | officer_id;
	
	redis_client_t::set_uint64("country_office", "role_uid", key, common::string_util_t::string_to_uint64(data.uid()));
	redis_client_t::set_uint32("country_office", "reappoint_time", key, data.refresh());
}

bool country_data_mgr_t::is_country_key_exsit(uint32_t country_id)
{
	return redis_client_t::is_key_exist("country", country_id);
}

bool country_data_mgr_t::is_king_war_apply_key_exsit(uint64_t family_id)
{
	return redis_client_t::is_key_exist("king_war_apply", family_id);
}

void country_data_mgr_t::init_king_war_offer_info(uint32_t king_war_id, country_t::king_war_apply_map& apply_map)
{
	apply_map.clear();

	std::vector<uint64_t> all_familys;
	redis_client_t::get_keys_by_field_condition<uint64_t, uint64_t, std::vector<uint64_t>>("king_war_apply", "king_war_id", king_war_id, all_familys);

	for (auto family_id : all_familys) {
		EM_BID_STATE state = (EM_BID_STATE)redis_client_t::get_uint32("king_war_apply", "bid_state", family_id);

		if (state == EM_BID_STATE_NONE) continue;

		country_t::king_war_apply_ptr p_family(new country_t::king_war_apply());
		if (NULL == p_family) {
			log_error("p_family null error");
			continue;
		}

		p_family->m_family_id = family_id;
		p_family->m_king_war_id = king_war_id;
		p_family->m_bid_state = state;
		p_family->m_bid_cost = redis_client_t::get_uint32("king_war_apply", "bid_cost", family_id);
		p_family->m_time = redis_client_t::get_uint32("king_war_apply", "bid_time", family_id);

		apply_map.insert(std::make_pair(family_id, p_family));
	}
}

void country_data_mgr_t::update_country_name(uint32_t country_id, const std::string& name)
{
	if (!is_country_key_exsit(country_id)) {
		log_error("country[%u] is not exsit", country_id);
		return;
	}

	redis_client_t::set_string("country", "name", country_id, name);
}

void country_data_mgr_t::update_country_flag(uint32_t country_id, uint32_t flag)
{
	if (!is_country_key_exsit(country_id)) {
		log_error("country[%u] is not exsit", country_id);
		return;
	}

	redis_client_t::set_uint32("country", "flag_id", country_id, flag);
}

void country_data_mgr_t::update_country_flag_list(uint32_t country_id, const std::set<uint32_t>& list)
{
	if (!is_country_key_exsit(country_id)) {
		log_error("country[%u] is not exsit", country_id);
		return;
	}

	std::ostringstream ss;
	for (std::set<uint32_t>::const_iterator citr = list.begin();
		citr != list.end(); ++citr) {
		if (!ss.str().empty())
		{
			ss << "$";
		}

		ss << *citr;
	}

	redis_client_t::set_string("country", "flag_list", country_id, ss.str());
}

void country_data_mgr_t::update_country_free_times(uint32_t country_id, uint32_t val)
{
	if (!is_country_key_exsit(country_id)) {
		log_error("country[%u] is not exsit", country_id);
		return;
	}

	redis_client_t::set_uint32("country", "free_times", country_id, val);
}

void country_data_mgr_t::update_year_name_free_times(uint32_t country_id, uint32_t val)
{
	if (!is_country_key_exsit(country_id)) {
		log_error("country[%u] is not exsit", country_id);
		return;
	}

	redis_client_t::set_uint32("country", "free_times2", country_id, val);
}

void country_data_mgr_t::update_country_king_war_info(uint32_t country_id, const country_t::king_war_info& info)
{
	// 不存在直接return
	if (!is_country_key_exsit(country_id)) {
		log_error("country[%u] is not exsit", country_id);
		return;
	}

	redis_client_t::set_uint32("country", "state", country_id, (uint32_t)info.m_state);
	redis_client_t::set_uint64("country", "family_defensive", country_id, info.m_family_defensive);
	redis_client_t::set_uint32("country", "defensive_cost", country_id, info.m_defensive_cost);
	redis_client_t::set_uint64("country", "family_attack", country_id, info.m_family_attack);
	redis_client_t::set_uint32("country", "attack_cost", country_id, info.m_attack_cost);
	redis_client_t::set_uint32("country", "state_end_time", country_id, info.m_state_end_time);
	redis_client_t::set_uint32("country", "war_start_time", country_id, info.m_start_time);
}

void country_data_mgr_t::update_country_king_family_id(uint32_t country_id, uint64_t family_id)
{
	// 不存在直接return
	if (!is_country_key_exsit(country_id)) {
		log_error("country[%u] is not exsit", country_id);
		return;
	}

	redis_client_t::set_uint64("country", "king_family", country_id, family_id);
}

void country_data_mgr_t::clear_king_war_info(uint32_t country_id)
{
	// 不存在直接return
	if (!is_country_key_exsit(country_id)) {
		log_error("country[%u] is not exsit", country_id);
		return;
	}

	redis_client_t::set_uint32("country", "state", country_id, 0);
	redis_client_t::set_uint64("country", "family_defensive", country_id, 0);
	redis_client_t::set_uint32("country", "defensive_cost", country_id, 0);
	redis_client_t::set_uint64("country", "family_attack", country_id, 0);
	redis_client_t::set_uint32("country", "attack_cost", country_id, 0);
	redis_client_t::set_uint32("country", "state_end_time", country_id, 0);
	redis_client_t::set_uint32("country", "war_start_time", country_id, 0);
}

void country_data_mgr_t::clear_king_war_apply_info(uint64_t family_id)
{
	// 不存在直接return
	if (!is_king_war_apply_key_exsit(family_id)) {
		log_error("king_war_apply[%lu] is not exsit", family_id);
		return;
	}

	redis_client_t::set_uint32("king_war_apply", "king_war_id", family_id, 0);
	redis_client_t::set_uint32("king_war_apply", "bid_cost", family_id, 0);
	redis_client_t::set_uint32("king_war_apply", "bid_state", family_id, 0);
	redis_client_t::set_uint32("king_war_apply", "bid_time", family_id, 0);
}

void country_data_mgr_t::update_king_war_apply_cost(uint64_t family_id, country_t::king_war_apply_ptr info)
{
	// 可能是新的家族竞价

	redis_client_t::set_uint32("king_war_apply", "king_war_id", family_id, info->m_king_war_id);
	redis_client_t::set_uint32("king_war_apply", "bid_cost", family_id, info->m_bid_cost);
	redis_client_t::set_uint32("king_war_apply", "bid_state", family_id, (uint32_t)info->m_bid_state);
	redis_client_t::set_uint32("king_war_apply", "bid_time", family_id, info->m_time);
}


void country_data_mgr_t::update_king_luckydraw_info(uint32_t country_id, std::string &val)
{
	redis_client_t::set_string("country", "king_luckydraw_info", country_id, val);
}

void country_data_mgr_t::update_king_luckydraw_money(uint32_t country_id, uint32_t val)
{
	redis_client_t::set_uint32("country", "king_luckydraw_money", country_id, val);
}

//获得国王礼包信息
std::string country_data_mgr_t::load_king_luckydraw_info(uint32_t country_id)
{
	return redis_client_t::get_string("country", "king_luckydraw_info", country_id);
}

//获得国王资金信息
uint32_t country_data_mgr_t::load_king_luckydraw_money(uint32_t country_id)
{
	return redis_client_t::get_uint32("country", "king_luckydraw_money", country_id);
}

