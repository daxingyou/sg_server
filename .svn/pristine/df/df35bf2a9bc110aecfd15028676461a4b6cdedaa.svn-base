#include "country.hpp"
#include "config/config_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "user/global_user_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "family/family_manager.hpp"
#include "redis/country_data_mgr.hpp"
#include "redis/global_user_data_mgr.hpp"
#include "sys_notice/sys_notice_manager.hpp"
#include "tblh/sys_notice_enum.hpp"
#include "tblh/CountryOfficial.tbls.h"
#include "common/time_manager.hpp"
#include "tblh/time_type_enum.hpp"
#include "log/log_wrapper.hpp"
#include "tblh/CountryFlag.tbls.h"
#include "redis_client.hpp"
#include "scene/global_scene_manager.hpp"
#include "tblh/Country.tbls.h"

#include "tblh/KingWarPersonal.tbls.h"
#include "tblh/KingWarRank.tbls.h"
#include "mail/mail_manager.hpp"
#include "tblh/mailcode_enum.hpp"
#include "tblh/MailTable.tbls.h"
#include "general_event/general_info_manager.hpp"

#define WRITE_COUNTRY_LOG_TO_DB(country, type, param1, param2, param3) \
    log_wrapper_t::send_country_log(country, type, param1, param2, param3);

country_t::country_t(uint32_t country_id) 
	: m_id(country_id)
{
}

void country_t::init(uint32_t now_time)
{
	for (uint32_t i = 0; i < king_war_type_max; ++i) {
		std::ostringstream temp;
		temp << "kingwar:" << m_id << ":" << i;
		m_family_like_key[i] = std::move(temp.str());
	}

	// 对国家中的全部家族排名
	calc_rank();

	switch (m_country_info.m_king_war_info.m_state)
	{
	case proto::common::king_war_state_end:	// 结束或未开始
		{
			// 不用做任何处理
		}
		break;
	case proto::common::king_war_state_sign_up:	// 报名阶段
		{
			// 加载所有竞价列表
			country_data_mgr_t::init_king_war_offer_info(m_id, m_king_war_apply_map);

			// 起服的时候，时间已经过了这个这场王城争夺战的最后时间
			if (now_time >= m_country_info.m_king_war_info.m_start_time)
			{
				king_war_end(false, 0);
			}
			else if (now_time >= m_country_info.m_king_war_info.m_state_end_time) // 起服的时候，时间已经过了这个这场王城争夺战的报名时间
			{
				// 验证家族战报名信息 如果成功状态设置为预告期
				if (king_war_apply_end())
				{
					m_country_info.m_king_war_info.m_state = proto::common::king_war_state_pre;
					m_country_info.m_king_war_info.m_state_end_time = 0;
					// save
					country_data_mgr_t::update_country_king_war_info(m_id, m_country_info.m_king_war_info);

					WRITE_COUNTRY_LOG_TO_DB(m_id, common::log_enum::source_type_country_king_war_state, m_country_info.m_king_war_info.m_state, 0, 0);
					
					KINGWAR_LOG("country[%u] sign up success def_family[%lu] att_family[%lu]", m_id, m_country_info.m_king_war_info.m_family_defensive, m_country_info.m_king_war_info.m_family_attack);
				}
				else
				{
					king_war_end(false, 0);
				}
			}
		}
		break;
	case proto::common::king_war_state_pre:
		{
			// 起服的时候，时间已经过了这个这场王城争夺战的最后时间
			if (now_time >= m_country_info.m_king_war_info.m_start_time)
			{
				king_war_end(false, 0);
			}

			// 状态不变，时间回调会处理
		}
		break;
	case proto::common::king_war_state_prepare:
	case proto::common::king_war_state_start:
		{
			// 起服的时候，状态是已经开战，无论时间有没超过最后时间都算这场战斗无效
			king_war_end(false, 0);
		}
		break;
	default:
		{
			log_error("country[%u] king_war_state[%u] error", m_id, m_country_info.m_king_war_info.m_state);
		}
		break;
	}

	if (m_country_info.m_king_war_info.m_state >= proto::common::king_war_state_pre) { // 大于预告说明已经产生对战家族
		family_ptr p_family_defensive = family_manager_t::get_family(m_country_info.m_king_war_info.m_family_defensive);
		if (p_family_defensive) {
			p_family_defensive->set_king_war_fight_country(m_id);
		}

		if (m_country_info.m_king_war_info.m_defensive_help_family != 0) {
			family_ptr p_defensive_help = family_manager_t::get_family(m_country_info.m_king_war_info.m_defensive_help_family);
			if (p_defensive_help) {
				p_defensive_help->set_king_war_fight_country(m_id);
			}
		}

		family_ptr p_family_attack = family_manager_t::get_family(m_country_info.m_king_war_info.m_family_attack);
		if (p_family_attack) {
			p_family_attack->set_king_war_fight_country(m_id);
		}

		if (m_country_info.m_king_war_info.m_attack_help_family != 0) {
			family_ptr p_attack_help = family_manager_t::get_family(m_country_info.m_king_war_info.m_attack_help_family);
			if (p_attack_help) {
				p_attack_help->set_king_war_fight_country(m_id);
			}
		}
	}
}

bool country_t::king_war_apply_end()
{
	if (m_king_war_apply_map.size() == 0)
	{
		log_error("country[%u] king war sign up family size == 0, can not start war", m_id);
		return false;
	}
	
	if (m_country_info.m_king_family_id == 0 && m_king_war_apply_map.size() <= 1)
	{
		log_error("country[%u] king war sign up family size == 1, can not start war", m_id);
		return false;
	}

	// 由于m_king_war_apply_map最多只遍历2次，所以不排序了

	// 有国王家族，防守方一定是他
	if (m_country_info.m_king_family_id != 0)
	{
		king_war_apply_ptr p_att_apply = get_king_war_attack_family(m_country_info.m_king_family_id);
		if (p_att_apply == NULL) // 没有有效的进攻家族
		{
			log_warn("country[%u] have no attack family", m_id);
			return false;
		}

		// 设置王城战家族
		m_country_info.m_king_war_info.m_family_defensive = m_country_info.m_king_family_id;
		m_country_info.m_king_war_info.m_defensive_cost = 0;
		m_country_info.m_king_war_info.m_family_attack = p_att_apply->m_family_id;
		m_country_info.m_king_war_info.m_attack_cost = p_att_apply->m_bid_cost;
	}
	else
	{
		king_war_apply_ptr p_def_apply = get_king_war_attack_family();
		if (p_def_apply == NULL)// 没有有效的防守家族
		{
			log_warn("country[%u] have no defensive family", m_id);
			return false;
		}

		king_war_apply_ptr p_att_apply = get_king_war_attack_family(p_def_apply->m_family_id);
		if (p_att_apply == NULL) // 没有有效的进攻家族
		{
			log_warn("country[%u] have no attack family", m_id);
			return false;
		}

		// 设置王城战家族
		m_country_info.m_king_war_info.m_family_defensive = p_def_apply->m_family_id;
		m_country_info.m_king_war_info.m_defensive_cost = p_def_apply->m_bid_cost;
		m_country_info.m_king_war_info.m_family_attack = p_att_apply->m_family_id;
		m_country_info.m_king_war_info.m_attack_cost = p_att_apply->m_bid_cost;
	}

	Country* p_country_conf = GET_CONF(Country, m_id);
	if (p_country_conf) {
		family_ptr p_def_family = family_manager_t::get_family(m_country_info.m_king_war_info.m_family_defensive);
		if (p_def_family) {
			p_def_family->set_king_war_fight_country(m_id);

			uint64_t general_id = p_def_family->get_general_id();
			if (general_id != 0) {
				m_country_info.m_king_war_info.m_defensive_notice_user = global_user_data_mgr_t::get_name(general_id);
				m_country_info.m_king_war_info.m_defensive_notice_officer = family_officer_type::family_officer_type_general;
			}

			std::vector<uint64_t> members;
			std::map<uint32_t, uint32_t> item_map;
			const family::family_member_map& member_map = p_def_family->get_all_member();
			for (family::family_member_map::const_iterator citr = member_map.begin();
				citr != member_map.end(); ++citr) {
				members.push_back(citr->first);
			}

			MailTable* p_conf = GET_CONF(MailTable, common::mailcode_enum::sys_mail_king_signup_success);
			if (p_conf) {
				std::string content_text;
				StringTool::format(content_text, common::string_util_t::convert_to_utf8(p_conf->content()).c_str(), common::string_util_t::convert_to_utf8(p_country_conf->capital_name()).c_str());

				mail_manager_t::send_mail(members, proto::common::MAIL_TYPE_SYS, 0, common::string_util_t::convert_to_utf8(p_conf->from_name()), common::string_util_t::convert_to_utf8(p_conf->title()), content_text,
					item_map);
			}
		} else {
			log_error("country[%u] king war family[%lu] not find", m_id, m_country_info.m_king_war_info.m_family_defensive);
		}

		family_ptr p_att_family = family_manager_t::get_family(m_country_info.m_king_war_info.m_family_attack);
		if (p_att_family) {
			p_att_family->set_king_war_fight_country(m_id);

			uint64_t general_id = p_att_family->get_general_id();
			if (general_id != 0) {
				m_country_info.m_king_war_info.m_attack_notice_user = global_user_data_mgr_t::get_name(general_id);
				m_country_info.m_king_war_info.m_attack_notice_officer = family_officer_type::family_officer_type_general;
			}

			std::vector<uint64_t> members;
			std::map<uint32_t, uint32_t> item_map;
			const family::family_member_map& member_map = p_att_family->get_all_member();
			for (family::family_member_map::const_iterator citr = member_map.begin();
				citr != member_map.end(); ++citr) {
				members.push_back(citr->first);
			}

			MailTable* p_conf = GET_CONF(MailTable, common::mailcode_enum::sys_mail_king_signup_success);
			if (p_conf) {
				std::string content_text;
				StringTool::format(content_text, common::string_util_t::convert_to_utf8(p_conf->content()).c_str(), common::string_util_t::convert_to_utf8(p_country_conf->capital_name()).c_str());

				mail_manager_t::send_mail(members, proto::common::MAIL_TYPE_SYS, 0, common::string_util_t::convert_to_utf8(p_conf->from_name()), common::string_util_t::convert_to_utf8(p_conf->title()), content_text,
					item_map);
			}
		} else {
			log_error("country[%u] king war family[%lu] not find", m_id, m_country_info.m_king_war_info.m_family_attack);
		}
	} else {
		log_error("country[%u] country not find", m_id);
	}

	// 其他的家族竞价资金退回
	for (king_war_apply_map::iterator itr = m_king_war_apply_map.begin();
		itr != m_king_war_apply_map.end(); ++itr)
	{
		const family_ptr& p_family = family_manager_t::get_family(itr->first);
		if (NULL == p_family)
		{
			log_error("p_family[%lu] null error", itr->first);
			continue;
		}

		p_family->set_king_war_apply_country(0);

		// 先清理数据库
		country_data_mgr_t::clear_king_war_apply_info(itr->first);

		// 竞价成功的家族不退回
		if (itr->first == m_country_info.m_king_war_info.m_family_defensive || itr->first == m_country_info.m_king_war_info.m_family_attack)
			continue;

		king_war_apply_ptr p_king_war_apply = itr->second;
		if (NULL == p_king_war_apply)
		{
			log_error("p_king_war_apply[%lu] null error", itr->first);
			continue;
		}

		// 只是报名了没出钱不用走下面逻辑
		if (p_king_war_apply->m_bid_cost == 0)
			continue;

		// 战斗没开启退回竞价资金
		p_family->add_money(p_king_war_apply->m_bid_cost, proto::server::family_money_change_king_war_fail_back);
	}

	WRITE_COUNTRY_LOG_TO_DB(m_id, common::log_enum::source_type_country_king_war_family, m_country_info.m_king_war_info.m_family_defensive, 0, 0);
	WRITE_COUNTRY_LOG_TO_DB(m_id, common::log_enum::source_type_country_king_war_family, m_country_info.m_king_war_info.m_family_attack, 0, 0);

	return true;
}

country_t::king_war_apply_ptr country_t::get_king_war_attack_family(uint64_t except /*= 0*/)
{
	king_war_apply_ptr p_cost_most_apply = NULL;

	for (king_war_apply_map::iterator itr = m_king_war_apply_map.begin();
		itr != m_king_war_apply_map.end(); ++itr)
	{
		if (except != 0 && itr->first == except)
			continue;

		king_war_apply_ptr p_king_war_apply = itr->second;
		if (NULL == p_king_war_apply)
		{
			log_error("p_king_war_apply[%lu] null error", itr->first);
			continue;
		}

		if (p_cost_most_apply == NULL)
		{
			p_cost_most_apply = p_king_war_apply;
		}
		else
		{
			if (p_king_war_apply->m_bid_cost > p_cost_most_apply->m_bid_cost)
			{
				p_cost_most_apply = p_king_war_apply;
			}
		}
	}

	return p_cost_most_apply;
}

void country_t::king_war_apply_back()
{
	for (king_war_apply_map::iterator itr = m_king_war_apply_map.begin();
		itr != m_king_war_apply_map.end(); ++itr)
	{
		const family_ptr& p_family = family_manager_t::get_family(itr->first);
		if (NULL == p_family)
		{
			log_error("p_family[%lu] null error", itr->first);
			continue;
		}
		p_family->set_king_war_apply_country(0);

		// 先清理数据库
		country_data_mgr_t::clear_king_war_apply_info(itr->first);

		king_war_apply_ptr p_king_war_apply = itr->second;
		if (NULL == p_king_war_apply)
		{
			log_error("p_king_war_apply[%lu] null error", itr->first);
			continue;
		}

		// 只是报名了没出钱不用走下面逻辑
		if (p_king_war_apply->m_bid_cost == 0)
			continue;

		// 战斗没开启退回竞价资金
		p_family->add_money(p_king_war_apply->m_bid_cost, proto::server::family_money_change_king_war_error_back);
	}
}


void country_t::king_war_end(bool is_normal_end, uint64_t king_family_id /*= 0*/)
{
	switch (m_country_info.m_king_war_info.m_state)
	{
	case proto::common::king_war_state_sign_up:	// 报名阶段
		{
			// 失败是不是因为只有一个人报名的原因
			if (m_country_info.m_king_family_id == 0 && m_king_war_apply_map.size() == 1)
			{
				king_war_apply_map::iterator ibegin = m_king_war_apply_map.begin();
				if (ibegin != m_king_war_apply_map.end()) {
					const king_war_apply_ptr& p_win_apply = ibegin->second;
					if (p_win_apply) {
						// 设置国王家族，国王家族交替
						if (!set_new_king_family(p_win_apply->m_family_id)) {
							king_war_apply_back();
							log_error("country[%u] only has one sign up family, set new king family[%lu] error", m_id, p_win_apply->m_family_id);
						} else {
							// 这种情况也算正常结束
							is_normal_end = true;
							KINGWAR_LOG("country[%u] only has one sign up family, new king family[%lu]", m_id, p_win_apply->m_family_id);
							WRITE_COUNTRY_LOG_TO_DB(m_id, common::log_enum::source_type_country_king_war_end, p_win_apply->m_family_id, 0, 0);
						}
					}
				}
			}
			else
			{
				// 返还报名家族的资金
				king_war_apply_back();
			}
		}
		break;
	case proto::common::king_war_state_pre:
	case proto::common::king_war_state_prepare:
		{
			// 返回王城战双方的资金，如果守方是国王家族则只有进攻方需要返还
			king_war_error_back();
		}
		break;
	case proto::common::king_war_state_start:
		{
			// 是否是正常结束
			if (is_normal_end && king_family_id != 0)
			{
				// 设置国王家族，国王家族交替
				if (!set_new_king_family(king_family_id))
				{
					king_war_error_back();
				} else {
					WRITE_COUNTRY_LOG_TO_DB(m_id, common::log_enum::source_type_country_king_war_end, king_family_id, 0, 0);
				}
			}
			else
			{
				// 返回王城战双方的资金，如果守方是国王家族则只有进攻方需要返还
				king_war_error_back();
			}
		}
		break;
	default:
		{
			log_error("country[%u] king_war_state[%u] error", m_id, m_country_info.m_king_war_info.m_state);
		}
		break;
	}

	WRITE_COUNTRY_LOG_TO_DB(m_id, common::log_enum::source_type_country_king_war_failed, m_country_info.m_king_war_info.m_state, 0, 0);

	// 结束的公告
	m_country_info.m_king_war_info.m_state = proto::common::king_war_state_end;

	notify_self_king_war_state();

	// 重置王城战信息
	reset_king_war();

	if (is_normal_end)
	{
		log_info("country[%u] fight success end, win family[%lu]", m_id, m_country_info.m_king_family_id);

		// 系统通知
		const family_ptr& p_family = family_manager_t::get_family(m_country_info.m_king_family_id);
		if (NULL != p_family)
		{
			uint64_t general_id = p_family->get_general_id();
			if (general_id != 0)
			{
				std::string general_name = global_user_data_mgr_t::get_name(general_id);
				BROADCAST_SYS_NOTICE(sys_notice_enum::sys_notice_24, 0, 0, m_id, p_family->get_name().c_str(), general_name.c_str());
			}
		}
	}
	else
	{
		log_warn("country[%u] fight failed end", m_id);
	}
}

void country_t::king_war_end_by_del(uint64_t del_family_id)
{
	switch (m_country_info.m_king_war_info.m_state) {
		case proto::common::king_war_state_pre:
		case proto::common::king_war_state_prepare:
		case proto::common::king_war_state_start: {
			uint64_t win_family = 0;
			if (m_country_info.m_king_war_info.m_family_defensive == del_family_id) {
				win_family = m_country_info.m_king_war_info.m_family_attack;
			} else if (m_country_info.m_king_war_info.m_family_attack == del_family_id) {
				win_family = m_country_info.m_king_war_info.m_family_defensive;
			} else {
				log_error("country[%u] king_war_end_by_del error", m_id);
				return;
			}

			set_new_king_family(win_family);

			// 结束的公告
			m_country_info.m_king_war_info.m_state = proto::common::king_war_state_end;

			notify_self_king_war_state();

			// 重置王城战信息
			reset_king_war();

			WRITE_COUNTRY_LOG_TO_DB(m_id, common::log_enum::source_type_country_king_war_end, win_family, 0, 0);

			log_info("country[%u] fight success end by family[%lu] del, win family[%lu]", m_id, del_family_id, win_family);

			// 系统通知
			const family_ptr& p_family = family_manager_t::get_family(m_country_info.m_king_family_id);
			if (NULL != p_family)
			{
				uint64_t general_id = p_family->get_general_id();
				if (general_id != 0)
				{
					std::string general_name = global_user_data_mgr_t::get_name(general_id);
					BROADCAST_SYS_NOTICE(sys_notice_enum::sys_notice_24, 0, 0, m_id, p_family->get_name().c_str(), general_name.c_str());
				}
			}

			break;
		}
		default: {
			log_error("country[%u] king_war_end_by_del king_war_state[%u] error", m_id, m_country_info.m_king_war_info.m_state);
			break;
		}
	}
}

country_t::king_war_apply_ptr country_t::get_king_war_apply_info(uint64_t family_id)
{
	king_war_apply_map::iterator itr = m_king_war_apply_map.find(family_id);
	if (itr != m_king_war_apply_map.end())
	{
		return itr->second;
	}

	return NULL;
}

void country_t::king_war_error_back()
{
	// 返回王城战双方的资金，如果守方是国王家族则只有进攻方需要返还
	if (m_country_info.m_king_war_info.m_family_defensive != m_country_info.m_king_family_id)
	{
		const family_ptr& p_def_family = family_manager_t::get_family(m_country_info.m_king_war_info.m_family_defensive);
		if (NULL != p_def_family)
		{
			p_def_family->add_money(m_country_info.m_king_war_info.m_defensive_cost, proto::server::family_money_change_king_war_error_back);
		}
	}

	const family_ptr& p_att_family = family_manager_t::get_family(m_country_info.m_king_war_info.m_family_attack);
	if (NULL != p_att_family)
	{
		p_att_family->add_money(m_country_info.m_king_war_info.m_attack_cost, proto::server::family_money_change_king_war_error_back);
	}
}

bool country_t::set_new_king_family(uint64_t king_family_id)
{
	bool is_new_king = false;
	if (m_country_info.m_king_family_id == 0) is_new_king = true;

	// 产生新的国王家族
	if (m_country_info.m_king_family_id != king_family_id) {
		// 重置老的国王信息
		reset_king_family();

		family_ptr p_new_king_family = family_manager_t::get_family(king_family_id);
		if (p_new_king_family) {
			if (p_new_king_family->get_country_id() != m_id) {
				join_country(p_new_king_family);
			}

			// 设置国王家族
			m_country_info.m_king_family_id = king_family_id;
			country_data_mgr_t::update_country_king_family_id(m_id, king_family_id);

			proto::common::country_base_data* p_base_info = m_country_info.info.mutable_base();
			if (p_base_info) { // 国王家族名字
				p_base_info->set_king_family(p_new_king_family->get_name());
			}

			p_new_king_family->change_king_family(true);
			update_loyalty(p_new_king_family);

			// 设置国王
			uint64_t general_id = p_new_king_family->get_general_id();
			if (general_id != 0) {
				m_country_info.officers[officer_type::officer_type_king].set_uid(common::string_util_t::uint64_to_string(general_id));
				global_user_ptr p_general = global_user_manager_t::get_global_user_by_uid(general_id);
				if (p_general) {
					p_general->set_country_officer(officer_type::officer_type_king);
					if (p_base_info) { // 国王名字
						p_base_info->set_king_name(p_general->get_name());
					}

					m_country_info.officers[officer_type::officer_type_king].set_name(p_general->get_name());
					m_country_info.officers[officer_type::officer_type_king].set_plugin(p_general->get_hero_plugin());
					m_country_info.officers[officer_type::officer_type_king].set_level(p_general->get_level());
					m_country_info.officers[officer_type::officer_type_king].set_fighting(p_general->get_fighting_value());
					m_country_info.officers[officer_type::officer_type_king].set_hero_plugin(p_general->get_plugin());
				} else {
					global_user_data_mgr_t::set_user_country_officer(general_id, officer_type::officer_type_king);
					std::string king_name = global_user_data_mgr_t::get_name(general_id);
					if (p_base_info) { // 国王名字
						p_base_info->set_king_name(king_name);
					}

					m_country_info.officers[officer_type::officer_type_king].set_name(king_name);
					m_country_info.officers[officer_type::officer_type_king].set_plugin(global_user_data_mgr_t::get_user_hero_plugin(general_id));
					m_country_info.officers[officer_type::officer_type_king].set_level(global_user_data_mgr_t::get_user_level(general_id));
					m_country_info.officers[officer_type::officer_type_king].set_fighting(global_user_data_mgr_t::get_user_fighting_value(general_id));
					m_country_info.officers[officer_type::officer_type_king].set_hero_plugin(global_user_data_mgr_t::get_user_plugin(general_id));
				}

				// 只需要上面的数据就可以
				country_data_mgr_t::save_country_officer(m_id, officer_type::officer_type_king, m_country_info.officers[officer_type::officer_type_king]);
			}

			// 设置丞相
			uint64_t deputy_general_id = p_new_king_family->get_deputy_general_id();
			if (deputy_general_id != 0) {
				m_country_info.officers[officer_type::officer_type_prime_minister].set_uid(common::string_util_t::uint64_to_string(deputy_general_id));
				global_user_ptr p_deputy_general = global_user_manager_t::get_global_user_by_uid(deputy_general_id);
				if (p_deputy_general) {
					p_deputy_general->set_country_officer(officer_type::officer_type_prime_minister);

					m_country_info.officers[officer_type::officer_type_prime_minister].set_name(p_deputy_general->get_name());
					m_country_info.officers[officer_type::officer_type_prime_minister].set_plugin(p_deputy_general->get_hero_plugin());
					m_country_info.officers[officer_type::officer_type_prime_minister].set_level(p_deputy_general->get_level());
					m_country_info.officers[officer_type::officer_type_prime_minister].set_fighting(p_deputy_general->get_fighting_value());
					m_country_info.officers[officer_type::officer_type_prime_minister].set_hero_plugin(p_deputy_general->get_plugin());
				} else {
					global_user_data_mgr_t::set_user_country_officer(deputy_general_id, officer_type::officer_type_prime_minister);

					m_country_info.officers[officer_type::officer_type_prime_minister].set_name(global_user_data_mgr_t::get_name(deputy_general_id));
					m_country_info.officers[officer_type::officer_type_prime_minister].set_plugin(global_user_data_mgr_t::get_user_hero_plugin(deputy_general_id));
					m_country_info.officers[officer_type::officer_type_prime_minister].set_level(global_user_data_mgr_t::get_user_level(deputy_general_id));
					m_country_info.officers[officer_type::officer_type_prime_minister].set_fighting(global_user_data_mgr_t::get_user_fighting_value(deputy_general_id));
					m_country_info.officers[officer_type::officer_type_prime_minister].set_hero_plugin(global_user_data_mgr_t::get_user_plugin(deputy_general_id));
				}

				// 只需要上面的数据就可以
				country_data_mgr_t::save_country_officer(m_id, officer_type::officer_type_prime_minister, m_country_info.officers[officer_type::officer_type_prime_minister]);
			}

			WRITE_COUNTRY_LOG_TO_DB(m_id, common::log_enum::source_type_country_king_family, king_family_id, 0, 0);
		}

		m_country_info.info.set_free_times(1);
		country_data_mgr_t::update_country_free_times(m_id, m_country_info.info.free_times());

		m_country_info.info.set_free_times2(1);
		country_data_mgr_t::update_year_name_free_times(m_id, m_country_info.info.free_times2());

		calc_rank();

		if (is_new_king) general_info_manager_t::add_num(proto::common::ACHIEVE_COUNTY_NUM, country_mgr_t::get_king_family_num());

		KINGWAR_LOG("country[%u] new king family[%lu]", m_id, king_family_id);
	} else {
		// 防守方胜利，什么都不变
		KINGWAR_LOG("country[%u] king family[%lu] win", m_id, m_country_info.m_king_family_id);
	}

	return true;
}

void country_t::start_king_war_sign_up(uint32_t start_time, uint32_t state_end_time)
{
	// 起服加载已经在这个状态了
	if (m_country_info.m_king_war_info.m_state == proto::common::king_war_state_sign_up)
	{
		return;
	}

	if (m_country_info.m_king_war_info.m_state != proto::common::king_war_state_end)
	{
		// 重置王城战信息
		king_war_end(false, 0);
		log_error("country[%u] start_king_war_sign_up error m_state[%u] != proto::common::king_war_state_end", m_id, m_country_info.m_king_war_info.m_state);
	}

	// 阶段
	m_country_info.m_king_war_info.m_state = proto::common::king_war_state_sign_up;
	m_country_info.m_king_war_info.m_state_end_time = state_end_time;

	// 王城战开始结束时间
	m_country_info.m_king_war_info.m_start_time = start_time;

	// save
	country_data_mgr_t::update_country_king_war_info(m_id, m_country_info.m_king_war_info);

	KINGWAR_LOG("country[%u] start king war sign up!!!!!!", m_id);

	WRITE_COUNTRY_LOG_TO_DB(m_id, common::log_enum::source_type_country_king_war_state, m_country_info.m_king_war_info.m_state, 0, 0);
}

void country_t::end_king_war_sign_up()
{
	// 状态验证(可能这国家王城战没有符合条件的家族)
	if (m_country_info.m_king_war_info.m_state != proto::common::king_war_state_sign_up)
	{
		log_warn("country[%u] end_king_war_sign_up error m_state[%u] != proto::common::king_war_state_sign_up", m_id, m_country_info.m_king_war_info.m_state);
		return;
	}

	// 验证家族战报名信息 如果成功状态设置为预告期
	if (king_war_apply_end())
	{
		m_country_info.m_king_war_info.m_state = proto::common::king_war_state_pre;
		m_country_info.m_king_war_info.m_state_end_time = 0;
		//save
		country_data_mgr_t::update_country_king_war_info(m_id, m_country_info.m_king_war_info);

		WRITE_COUNTRY_LOG_TO_DB(m_id, common::log_enum::source_type_country_king_war_state, m_country_info.m_king_war_info.m_state, 0, 0);

		KINGWAR_LOG("country[%u] sign up success def_family[%lu] att_family[%lu]", m_id, m_country_info.m_king_war_info.m_family_defensive, m_country_info.m_king_war_info.m_family_attack);

		notify_self_king_war_state();
	}
	else
	{
		king_war_end(false, 0);

		KINGWAR_LOG("country[%u] sign up failed!!!!!!!!", m_id);
	}
}

void country_t::start_king_war_prepare(uint32_t state_end_time)
{
	// 状态验证(可能这国家王城战没有符合条件的家族)
	if (m_country_info.m_king_war_info.m_state != proto::common::king_war_state_pre)
	{
		log_error("country[%u] start_king_war_prepare error m_state[%u] != proto::common::king_war_state_pre", m_id, m_country_info.m_king_war_info.m_state);
		return;
	}

	// 阶段
	m_country_info.m_king_war_info.m_state = proto::common::king_war_state_prepare;
	m_country_info.m_king_war_info.m_state_end_time = state_end_time;
	m_country_info.m_king_war_info.m_defensive_num = 0;
	m_country_info.m_king_war_info.m_attack_num = 0;

	//save
	country_data_mgr_t::update_country_king_war_info(m_id, m_country_info.m_king_war_info);

	notify_self_king_war_state();

	KINGWAR_LOG("country[%u] king war prepare!!!!!!!!!!!", m_id);

	Country* p_conf = GET_CONF(Country, m_id);
	if (NULL == p_conf) {
		log_error("country[%u] config not find", m_id);
		return;
	} 

	global_scene_ptr p_scene = global_scene_manager_t::get_scene_by_id(p_conf->capital());
	if (p_scene == NULL) {
		log_error("country[%u] capital[%u] not find or not reg", m_id, p_conf->capital());
		return;
	}

	proto::server::eg_king_war_state_notify notify;
	notify.set_country_id(m_id);
	notify.set_state(m_country_info.m_king_war_info.m_state);

	notify.set_def_family_id(m_country_info.m_king_war_info.m_family_defensive);
	const family_ptr& p_def_family = family_manager_t::get_family(m_country_info.m_king_war_info.m_family_defensive);
	if (NULL != p_def_family) {
		notify.set_def_family_name(p_def_family->get_name());
	}

	if (m_country_info.m_king_war_info.m_defensive_help_family != 0) {
		const family_ptr& p_def_help_family = family_manager_t::get_family(m_country_info.m_king_war_info.m_defensive_help_family);
		if (NULL != p_def_help_family) {
			notify.set_def_help_family(m_country_info.m_king_war_info.m_defensive_help_family);
			notify.set_def_help_family_name(p_def_help_family->get_name());
		}
	}

	notify.set_att_family_id(m_country_info.m_king_war_info.m_family_attack);
	const family_ptr& p_att_family = family_manager_t::get_family(m_country_info.m_king_war_info.m_family_attack);
	if (NULL != p_att_family) {
		notify.set_att_family_name(p_att_family->get_name());
	}

	if (m_country_info.m_king_war_info.m_attack_help_family != 0) {
		const family_ptr& p_att_help_family = family_manager_t::get_family(m_country_info.m_king_war_info.m_attack_help_family);
		if (NULL != p_att_help_family) {
			notify.set_att_help_family(m_country_info.m_king_war_info.m_attack_help_family);
			notify.set_att_help_family_name(p_att_help_family->get_name());
		}
	}

	p_scene->send_msg_to_game(op_cmd::eg_king_war_state_notify, notify);

	WRITE_COUNTRY_LOG_TO_DB(m_id, common::log_enum::source_type_country_king_war_state, m_country_info.m_king_war_info.m_state, 0, 0);
}

void country_t::end_king_war_prepare()
{
	// 状态验证(可能这国家王城战没有符合条件的家族)
	if (m_country_info.m_king_war_info.m_state != proto::common::king_war_state_prepare)
	{
		log_warn("country[%u] end_king_war_prepare error m_state[%u] != proto::common::king_war_state_prepare", m_id, m_country_info.m_king_war_info.m_state);
		return;
	}

	// 备战结束不用改变状态
	//m_country_info.m_king_war_info.m_state = proto::common::king_war_state_prepare;
	//m_country_info.m_king_war_info.m_state_end_time = 0;
	//save
	// country_data_mgr_t::update_country_king_war_info(m_id, m_country_info.m_king_war_info);
	KINGWAR_LOG("country[%u] king war prepare end!!!!!!!!!!!", m_id);
}

void country_t::start_king_war_fight(uint32_t state_end_time)
{
	// 进入战斗可以是准备或者备战两个状态
	if (m_country_info.m_king_war_info.m_state != proto::common::king_war_state_prepare && m_country_info.m_king_war_info.m_state != proto::common::king_war_state_pre)
	{
		log_warn("country[%u] end_king_war_prepare error m_state[%u] != proto::common::king_war_state_prepare && proto::common::king_war_state_pre", m_id, m_country_info.m_king_war_info.m_state);
		return;
	}

	// 战斗开启，通知game
	// 阶段
	m_country_info.m_king_war_info.m_state = proto::common::king_war_state_start;
	m_country_info.m_king_war_info.m_state_end_time = state_end_time;

	//save
	country_data_mgr_t::update_country_king_war_info(m_id, m_country_info.m_king_war_info);

	// 通知客户端战斗开启了
	notify_self_king_war_state();

	KINGWAR_LOG("country[%u] king war start", m_id);

	Country* p_conf = GET_CONF(Country, m_id);
	if (NULL == p_conf) {
		log_error("country[%u] config not find", m_id);
		return;
	}

	global_scene_ptr p_scene = global_scene_manager_t::get_scene_by_id(p_conf->capital());
	if (p_scene == NULL) {
		log_error("country[%u] capital[%u] not find or not reg", m_id, p_conf->capital());
		return;
	}

	proto::server::eg_king_war_state_notify notify;
	notify.set_country_id(m_id);
	notify.set_state(m_country_info.m_king_war_info.m_state);

	notify.set_def_family_id(m_country_info.m_king_war_info.m_family_defensive);
	const family_ptr& p_def_family = family_manager_t::get_family(m_country_info.m_king_war_info.m_family_defensive);
	if (p_def_family != NULL)
	{
		notify.set_def_family_name(p_def_family->get_name());
	}

	if (m_country_info.m_king_war_info.m_defensive_help_family != 0) {
		const family_ptr& p_def_help_family = family_manager_t::get_family(m_country_info.m_king_war_info.m_defensive_help_family);
		if (NULL != p_def_help_family) {
			notify.set_def_help_family(m_country_info.m_king_war_info.m_defensive_help_family);
			notify.set_def_help_family_name(p_def_help_family->get_name());
		}
	}

	notify.set_att_family_id(m_country_info.m_king_war_info.m_family_attack);
	const family_ptr& p_att_family = family_manager_t::get_family(m_country_info.m_king_war_info.m_family_attack);
	if (p_att_family != NULL)
	{
		notify.set_att_family_name(p_att_family->get_name());
	}

	if (m_country_info.m_king_war_info.m_attack_help_family != 0) {
		const family_ptr& p_att_help_family = family_manager_t::get_family(m_country_info.m_king_war_info.m_attack_help_family);
		if (NULL != p_att_help_family) {
			notify.set_att_help_family(m_country_info.m_king_war_info.m_attack_help_family);
			notify.set_att_help_family_name(p_att_help_family->get_name());
		}
	}

	p_scene->send_msg_to_game(op_cmd::eg_king_war_state_notify, notify);

	WRITE_COUNTRY_LOG_TO_DB(m_id, common::log_enum::source_type_country_king_war_state, m_country_info.m_king_war_info.m_state, 0, 0);
}

void country_t::end_king_war_fight()
{
	// 状态验证(可能这国家王城战没有符合条件的家族,或者已经提前结束)
	if (m_country_info.m_king_war_info.m_state != proto::common::king_war_state_start)
	{
		log_warn("country[%u] end_king_war_fight error m_state[%u] != proto::common::king_war_state_start", m_id, m_country_info.m_king_war_info.m_state);
		return;
	}

	KINGWAR_LOG("country[%u] king war time end, notift game", m_id);

	Country* p_conf = GET_CONF(Country, m_id);
	if (NULL == p_conf) {
		log_error("country[%u] config not find", m_id);
		return;
	}

	global_scene_ptr p_scene = global_scene_manager_t::get_scene_by_id(p_conf->capital());
	if (p_scene == NULL) {
		log_error("country[%u] capital[%u] not find or not reg", m_id, p_conf->capital());
		return;
	}

	proto::server::eg_king_war_time_end_notify notify;
	notify.set_country_id(m_id);
	p_scene->send_msg_to_game(op_cmd::eg_king_war_time_end_notify, notify);

	WRITE_COUNTRY_LOG_TO_DB(m_id, common::log_enum::source_type_country_king_war_time_end, 0, 0, 0);
}

country_t::king_war_apply_ptr country_t::add_king_war_family(uint64_t family_id, uint32_t money)
{
	king_war_apply_ptr p_apply_family = king_war_apply_ptr(new king_war_apply());
	if (NULL == p_apply_family)
	{
		log_error("p_apply_family[%lu] null error", family_id);
		return NULL;
	}

	p_apply_family->m_family_id = family_id;
	p_apply_family->m_king_war_id = m_id;
	p_apply_family->m_bid_state = EM_BID_STATE_BIDING;
	p_apply_family->m_bid_cost = money;
	p_apply_family->m_time = common::time_util_t::now_time();

	m_king_war_apply_map.insert(std::make_pair(family_id, p_apply_family));

	country_data_mgr_t::update_king_war_apply_cost(family_id, p_apply_family);

	return p_apply_family;
}

void country_t::del_king_war_apply_family(const family_ptr& p_family)
{
	if (p_family == NULL) {
		log_error("p_family null error");
		return;
	}

	p_family->set_king_war_apply_country(0);

	king_war_apply_map::iterator itr = m_king_war_apply_map.find(p_family->get_family_id());
	if (itr == m_king_war_apply_map.end()) {
		log_error("family[%lu] not in country[%u] king war apply list", p_family->get_family_id(), m_id);
		return;
	}

	if (itr->second != NULL) {
		p_family->add_money(itr->second->m_bid_cost, proto::server::family_money_king_war_back);
	}

	country_data_mgr_t::clear_king_war_apply_info(p_family->get_family_id());

	m_king_war_apply_map.erase(itr);
}

void country_t::notify_self_king_war_state()
{
	proto::client::ec_king_war_notice_notify ntf;
	peek_king_war_state(ntf);

	family_ptr p_def_family = family_manager_t::get_family(m_country_info.m_king_war_info.m_family_defensive);
	if (p_def_family) {
		p_def_family->send_msg_to_family(op_cmd::ec_king_war_notice_notify, ntf);
	}

	if (m_country_info.m_king_war_info.m_defensive_help_family != 0) {
		family_ptr p_def_help_family = family_manager_t::get_family(m_country_info.m_king_war_info.m_defensive_help_family);
		if (p_def_help_family) {
			p_def_help_family->send_msg_to_family(op_cmd::ec_king_war_notice_notify, ntf);
		}
	}

	family_ptr p_att_family = family_manager_t::get_family(m_country_info.m_king_war_info.m_family_attack);
	if (p_att_family) {
		p_att_family->send_msg_to_family(op_cmd::ec_king_war_notice_notify, ntf);
	}

	if (m_country_info.m_king_war_info.m_attack_help_family != 0) {
		family_ptr p_att_help_family = family_manager_t::get_family(m_country_info.m_king_war_info.m_attack_help_family);
		if (p_att_help_family) {
			p_att_help_family->send_msg_to_family(op_cmd::ec_king_war_notice_notify, ntf);
		}
	}

	//log_info("country[%u] ec_king_war_notice_notify state[%u]", m_id, m_country_info.m_king_war_info.m_state);
}

void country_t::set_king_war_result(const proto::server::ge_king_war_result_notify& ntf)
{
	if (m_country_info.m_king_war_info.m_state != proto::common::king_war_state_start)
	{
		log_warn("country[%u] set_king_war_result error m_state[%u] != proto::common::king_war_state_start", m_id, m_country_info.m_king_war_info.m_state);
		return;
	}

	if (ntf.win_family_id() != m_country_info.m_king_war_info.m_family_defensive && ntf.win_family_id() != m_country_info.m_king_war_info.m_family_attack)
	{
		log_warn("country[%u] win_family[%lu] not family_defensive[%lu] and not family_attack[%lu]", m_id, ntf.win_family_id(), m_country_info.m_king_war_info.m_family_defensive, m_country_info.m_king_war_info.m_family_attack);
		
		king_war_end(false, 0);
	}
	else
	{
		uint64_t win_family_id = ntf.win_family_id();
		uint64_t win_help_family_id = 0;
		if (ntf.win_family_id() == m_country_info.m_king_war_info.m_family_defensive) {
			win_help_family_id = m_country_info.m_king_war_info.m_defensive_help_family;
		} else if (ntf.win_family_id() == m_country_info.m_king_war_info.m_family_attack) {
			win_help_family_id = m_country_info.m_king_war_info.m_attack_help_family;
		}

		king_war_end(true, ntf.win_family_id());

		Comprehensive* p_conf = GET_CONF(Comprehensive, comprehensive_common::king_victory_reward);
		std::map<uint32_t, uint32_t> win_rwd;
		if (p_conf) {
			common::Misc::Parse2SeperatorDataInfo(std::string(p_conf->parameter1()), win_rwd, "$");
		}

		p_conf = GET_CONF(Comprehensive, comprehensive_common::king_defeat_reward);
		std::map<uint32_t, uint32_t> lose_rwd;
		if (p_conf) {
			common::Misc::Parse2SeperatorDataInfo(std::string(p_conf->parameter1()), lose_rwd, "$");
		}

		std::map<uint32_t, std::vector<uint64_t>> rank_rwd;
		std::map<uint32_t, std::vector<uint64_t>> exploit_rwd;

		for (int32_t i = 0; i < ntf.all_roles_size(); ++i) {
			const proto::server::king_war_personal& personal = ntf.all_roles(i);
			uint64_t family_id = 0;
			global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(personal.uid());
			if (p_user) {
				family_id = p_user->get_family_id();
			} else {
				family_id = global_user_data_mgr_t::get_user_family_id(personal.uid());
			}

			if (family_id != 0) {
				if (win_family_id == family_id || (win_help_family_id != 0 && win_help_family_id == family_id)) {
					mail_manager_t::send_mail(personal.uid(), proto::common::MAIL_TYPE_SYS, common::mailcode_enum::sys_mail_king_victory_reward, win_rwd, proto::common::MAIL_GET_TYPE_KING_WAR);
				} else {
					mail_manager_t::send_mail(personal.uid(), proto::common::MAIL_TYPE_SYS, common::mailcode_enum::sys_mail_king_defeat_reward, lose_rwd, proto::common::MAIL_GET_TYPE_KING_WAR);
				}

				if (personal.rank_level() != 0) {
					rank_rwd[personal.rank_level()].push_back(personal.uid());
				}

				if (personal.exploit_level() != 0) {
					exploit_rwd[personal.exploit_level()].push_back(personal.uid());
				}
			}
		}

		MailTable* p_king_merit_rank = GET_CONF(MailTable, common::mailcode_enum::sys_mail_king_merit_rank);
		if (p_king_merit_rank) {
			for (std::map<uint32_t, std::vector<uint64_t>>::const_iterator citr = rank_rwd.begin();
				citr != rank_rwd.end(); ++citr) {
				KingWarRank* p_KingWarRank = GET_CONF(KingWarRank, citr->first);
				if (!p_KingWarRank) {
					log_error("king war rank reward id[%u] not find", citr->first);
					continue;
				}

				std::string content_text;
				StringTool::format(content_text, common::string_util_t::convert_to_utf8(p_king_merit_rank->content()).c_str(), citr->first);

				mail_manager_t::send_mail(citr->second, proto::common::MAIL_TYPE_SYS, 0, common::string_util_t::convert_to_utf8(p_king_merit_rank->from_name()), common::string_util_t::convert_to_utf8(p_king_merit_rank->title()), content_text,
					p_KingWarRank->reward(), proto::common::MAIL_GET_TYPE_KING_WAR);
			}
		}

		MailTable* p_king_merit_level = GET_CONF(MailTable, common::mailcode_enum::sys_mail_king_merit_level);
		if (p_king_merit_level) {
			for (std::map<uint32_t, std::vector<uint64_t>>::const_iterator citr = exploit_rwd.begin();
				citr != exploit_rwd.end(); ++citr) {
				KingWarPersonal* p_KingWarPersonal = GET_CONF(KingWarPersonal, citr->first);
				if (!p_KingWarPersonal) {
					log_error("king war rank reward personal id[%u] not find", citr->first);
					continue;
				}

				std::string content_text;
				StringTool::format(content_text, common::string_util_t::convert_to_utf8(p_king_merit_level->content()).c_str(), citr->first);

				mail_manager_t::send_mail(citr->second, proto::common::MAIL_TYPE_SYS, 0, common::string_util_t::convert_to_utf8(p_king_merit_level->from_name()), common::string_util_t::convert_to_utf8(p_king_merit_level->title()), content_text,
					p_KingWarPersonal->reward(), proto::common::MAIL_GET_TYPE_KING_WAR);
			}
		}
	}
}

void country_t::add_family(const family_ptr& p_family)
{
	if (!p_family) {
		log_error("p_family null error");
		return;
	}

	if (p_family->get_family_id() == m_country_info.m_king_family_id) { // 国王家族设置
		p_family->set_king_family(true);
	}

	family_force_t data(p_family->get_family_id());
	data.base.set_family_name(p_family->get_name());
	family_member_ptr p_family_member = p_family->get_family_member(p_family->get_general_id());
	if (p_family_member) {
		data.base.set_patriarch(p_family_member->get_name());
	}
	data.base.set_level(p_family->get_level());
	data.base.set_prestige(p_family->get_prestige());
	data.base.set_loyalty(p_family->get_loyalty());
	m_family_force_vec.push_back(data);
}

void country_t::del_family(const family_ptr& p_family, bool is_del_family /*= false*/)
{
	if (!p_family) {
		log_error("p_family null error");
		return;
	}

	// 是国王家族的处理
	if (p_family->get_family_id() == m_country_info.m_king_family_id) {
		reset_king_family();
	}

	switch (m_country_info.m_king_war_info.m_state)
	{
		case proto::common::king_war_state_end:	// 结束或未开始
		{
			// 不用做任何处理
		}
		break;
		case proto::common::king_war_state_sign_up:	// 报名阶段
		{
			if (p_family->get_king_war_apply_country() == m_id) { // 删除报名信息
				del_king_war_apply_family(p_family);
			}
		}
		break;
		case proto::common::king_war_state_pre:
		case proto::common::king_war_state_prepare:
		case proto::common::king_war_state_start:
		{
			if (is_del_family) {
				if (m_country_info.m_king_war_info.m_family_defensive == p_family->get_family_id() ||
					m_country_info.m_king_war_info.m_family_attack == p_family->get_family_id()) { // 参战方，删除家族的话直接另外一方获胜
					king_war_end_by_del(p_family->get_family_id());
				} else {
					uint32_t kingwar_type = get_king_war_help_family_type(p_family->get_family_id());
					if (kingwar_type != country_t::king_war_type_error) { // 助战家族
						cancel_king_war_fight_help(kingwar_type, p_family);
					}
				}
			}
		}
		break;
		default:
		break;
	}
	
	uint64_t uid = p_family->get_family_id();
	family_force_vec::iterator itr = std::find_if(m_family_force_vec.begin(), m_family_force_vec.end(),
		[uid](const family_force_t& family)
	{
		if (family.family_uid == uid) return true;
		return false;
	});

	if (itr != m_family_force_vec.end()) {
		m_family_force_vec.erase(itr);
	}
}

void country_t::del_family_king_war_clear(const family_ptr& p_family)
{
	if (!p_family) {
		log_error("p_family null error");
		return;
	}

	switch (m_country_info.m_king_war_info.m_state)
	{
		case proto::common::king_war_state_end:	// 结束或未开始
		{
			// 不用做任何处理
		}
		break;
		case proto::common::king_war_state_sign_up:	// 报名阶段
		{
			if (p_family->get_king_war_apply_country() == m_id) { // 删除报名信息
				del_king_war_apply_family(p_family);
			}
		}
		break;
		case proto::common::king_war_state_pre:
		case proto::common::king_war_state_prepare:
		case proto::common::king_war_state_start:
		{
			if (m_country_info.m_king_war_info.m_family_defensive == p_family->get_family_id() ||
				m_country_info.m_king_war_info.m_family_attack == p_family->get_family_id()) { // 参战方，删除家族的话直接另外一方获胜
				king_war_end_by_del(p_family->get_family_id());
			} else {
				uint32_t kingwar_type = get_king_war_help_family_type(p_family->get_family_id());
				if (kingwar_type != country_t::king_war_type_error) { // 助战家族
					cancel_king_war_fight_help(kingwar_type, p_family);
				}
			}
		}
		break;
		default:
			break;
	}
}

void country_t::update_loyalty(const family_ptr& p_family)
{
	if (!p_family) {
		log_error("p_family null error");
		return;
	}

	uint64_t uid = p_family->get_family_id();
	family_force_vec::iterator itr = std::find_if(m_family_force_vec.begin(), m_family_force_vec.end(),
		[uid](const family_force_t& family)
	{
		if (family.family_uid == uid) return true;
		return false;
	});

	if (itr == m_family_force_vec.end()) {
		log_error("family[%lu] not find in country[%u]", uid, m_id);
		return;
	}

	(*itr).base.set_loyalty(p_family->get_loyalty());
}

void country_t::calc_rank()
{
	std::sort(m_family_force_vec.begin(), m_family_force_vec.end());
}

void country_t::join_country(const family_ptr& p_family)
{
	if (!p_family) {
		log_error("p_family null error");
		return;
	}

	if (p_family->get_country_id() == m_id) {
		log_error("p_family[%lu] is already in country[%u]", p_family->get_family_id(), m_id);
		return;
	}

	if (p_family->get_country_id() != 0) {
		country_ptr p_old_country = country_mgr_t::get_country(p_family->get_country_id());
		if (p_old_country) {
			p_old_country->del_family(p_family);
		}
	}

	// 先设置家族国家id
	p_family->change_country(m_id);

	add_family(p_family);

	WRITE_COUNTRY_LOG_TO_DB(m_id, common::log_enum::source_type_country_alliance, p_family->get_family_id(), 0, 0);
}

void country_t::quit_country(const family_ptr& p_family)
{
	if (!p_family) {
		log_error("p_family null error");
		return;
	}

	p_family->change_country(0);

	del_family(p_family);

	WRITE_COUNTRY_LOG_TO_DB(m_id, common::log_enum::source_type_country_dismiss_alliance, p_family->get_family_id(), 0, 0);
}

void country_t::reset_king_family()
{
	if (m_country_info.m_king_family_id == 0) return;

	family_ptr p_old_king = family_manager_t::get_family(m_country_info.m_king_family_id);
	if (p_old_king) {
		p_old_king->change_king_family(false);
		update_loyalty(p_old_king);
	}

	m_country_info.m_king_family_id = 0;
	country_data_mgr_t::update_country_king_family_id(m_id, m_country_info.m_king_family_id);

	proto::common::country_base_data* p_base_info = m_country_info.info.mutable_base();
	if (p_base_info) { // 国王家族名字
		p_base_info->set_king_family("");
	}

	for (std::map<uint32_t, proto::common::country_officer_data>::iterator itr = m_country_info.officers.begin();
		itr != m_country_info.officers.end(); ++itr) {
		uint64_t uid = common::string_util_t::string_to_uint64(itr->second.uid());
		if (uid != 0) {
			global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
			if (NULL != p_user) {
				p_user->set_country_officer(0);
			} else {
				global_user_data_mgr_t::set_user_country_officer(uid, 0);
			}
		}

		itr->second.set_uid("0");
		itr->second.set_name("");
		itr->second.set_plugin(0);
		itr->second.set_level(0);
		itr->second.set_fighting(0);
		itr->second.set_refresh(0); // 重置时间
		itr->second.set_hero_plugin(0);

		country_data_mgr_t::save_country_officer(m_id, itr->first, itr->second);
	}
}

void country_t::peek_base_data(proto::common::country_base_data* p_base)
{
	if (p_base) {
		p_base->CopyFrom(m_country_info.info.base());
	}
}

void country_t::peek_country_info(proto::common::country_info_data* p_data)
{
	if (p_data) {
		p_data->CopyFrom(m_country_info.info);
	}
}

void country_t::peek_country_data(proto::client::ec_country_info_reply& reply)
{
	proto::common::country_info_data* p_country_info = reply.mutable_base_info();
	if (p_country_info) {
		p_country_info->CopyFrom(m_country_info.info);
	}

	for (const auto& officer : m_country_info.officers) {
		proto::common::country_officer_data* p_officer_data = reply.add_officers();
		if (p_officer_data) {
			p_officer_data->CopyFrom(officer.second);
		}
	}
}

bool country_t::has_family_privilige(uint32_t officer, uint32_t type)
{
	CountryOfficial* conf = GET_CONF(CountryOfficial, officer);
	if (conf && conf->is_power(type))
	{
		return true;
	}

	return false;
}

void country_t::do_modify_year_name(uint64_t uid, const std::string& name)
{
	proto::common::country_base_data* p_data = m_country_info.info.mutable_base();
	if (p_data) p_data->set_year_name(name);

	country_data_mgr_t::update_country_year_name(m_id, name);

	WRITE_COUNTRY_LOG_TO_DB(m_id, common::log_enum::source_type_country_modify_year_name, uid, 0, 0);
}

uint32_t country_t::modify_year_name(const global_user_ptr& p_user, const std::string& name)
{
	if (p_user == NULL) {
		log_error("p_user == NULL");
		return common::errcode_enum::notice_unknown;
	}

	if (!common::string_util_t::is_no_special_char(name))
	{
		log_error("country year_name[%s] has special char!", name.c_str());
		return errcode_enum::sys_notice_not_chinese_word;
	}

	auto conf = GET_CONF(Comprehensive, comprehensive_common::country_title_number_upper);
	uint32_t title_number = GET_COMPREHENSIVE_VALUE_1(conf);
	uint32_t name_cnt = common::string_util_t::get_char_size(name);
	if (name_cnt > title_number) {
		log_error("user[%lu] modify year name size[%u] > %u", p_user->get_uid(), name_cnt, title_number);
		return common::errcode_enum::country_error_name_max_size;
	}

	if (m_country_info.info.free_times2() > 0) { // 免费修改
		do_modify_year_name(p_user->get_uid(), name);

		m_country_info.info.set_free_times2(m_country_info.info.free_times2() - 1);
		country_data_mgr_t::update_year_name_free_times(m_id, m_country_info.info.free_times2());

		proto::client::ec_modify_year_name_reply reply;
		reply.set_reply_code(common::errcode_enum::error_ok);
		reply.set_is_free(1);
		peek_country_info(reply.mutable_data());
		p_user->send_msg_to_client(op_cmd::ec_modify_year_name_reply, reply);

		WRITE_COUNTRY_LOG_TO_DB(m_id, common::log_enum::source_type_country_change_year_name_free, p_user->get_uid(), 0, 0);
	} else { // 通知game扣钱
		proto::server::eg_country_year_name_request eg_req;
		eg_req.set_name(name);
		p_user->send_msg_to_game(op_cmd::eg_country_year_name_request, eg_req);
	}

	return common::errcode_enum::error_ok;
}

void country_t::modify_notice(uint64_t uid, const std::string& notice)
{
	proto::common::country_base_data* p_data = m_country_info.info.mutable_base();
	if (p_data) p_data->set_notice(notice);

	country_data_mgr_t::update_country_notice(m_id, notice);

	WRITE_COUNTRY_LOG_TO_DB(m_id, common::log_enum::source_type_country_modify_notice, uid, 0, 0);
}

void country_t::del_officer(uint32_t officer_id, uint64_t uid)
{
	std::map<uint32_t, proto::common::country_officer_data>::iterator itr = m_country_info.officers.find(officer_id);
	if (itr == m_country_info.officers.end()) {
		log_error("country[%u] have no officer[%u]", m_id, officer_id);
		return;
	}

	if (uid != common::string_util_t::string_to_uint64(itr->second.uid())) {
		log_error("country[%u] officer[%u] uid[%s] != %lu", m_id, officer_id, itr->second.uid().c_str(), uid);
		return;
	}

	// 设置空
	itr->second.set_uid("0");
	itr->second.set_name("");
	itr->second.set_plugin(0);
	itr->second.set_level(0);
	itr->second.set_fighting(0);
	itr->second.set_hero_plugin(0);
	country_data_mgr_t::save_country_officer(m_id, itr->first, itr->second);
}

bool country_t::is_officer_appoint(uint32_t officer_id)
{
	std::map<uint32_t, proto::common::country_officer_data>::const_iterator citr = m_country_info.officers.find(officer_id);
	if (citr == m_country_info.officers.end()) {
		log_error("country[%u] officer[%u] not find", m_id, officer_id);
		return true;
	}

	uint64_t uid = common::string_util_t::string_to_uint64(citr->second.uid());
	return uid != 0;
}

bool country_t::is_officer_appoint_cd(uint32_t officer_id)
{
	std::map<uint32_t, proto::common::country_officer_data>::const_iterator citr = m_country_info.officers.find(officer_id);
	if (citr != m_country_info.officers.end()) {
		if (citr->second.refresh() > common::time_util_t::now_time()) {
			return true;
		}
	}

	return false;
}

void country_t::set_officer_appoint(uint32_t officer_id, const global_user_ptr& p_user, proto::common::country_officer_data* p_data)
{
	if (!p_user) {
		log_error("p_user == null");
		return;
	}

	if (p_user->get_country_id() != m_id) {
		log_error("user[%lu] country[%u] != country[%u], when set officer appoint", p_user->get_uid(), p_user->get_country_id(), m_id);
		return;
	}

	std::map<uint32_t, proto::common::country_officer_data>::iterator itr = m_country_info.officers.find(officer_id);
	if (itr == m_country_info.officers.end()) {
		log_error("country[%u] officer[%u] not define", m_id, officer_id);
		return;
	}

	p_user->set_country_officer(officer_id);

	itr->second.set_uid(common::string_util_t::uint64_to_string(p_user->get_uid()));
	itr->second.set_name(p_user->get_name());
	itr->second.set_plugin(p_user->get_hero_plugin());
	itr->second.set_refresh(time_manager_t::get_next_refresh_time(common::time_type_enum::time_one_day));
	itr->second.set_level(p_user->get_level());
	itr->second.set_fighting(p_user->get_fighting_value());
	itr->second.set_hero_plugin(p_user->get_plugin());
	country_data_mgr_t::save_country_officer(m_id, itr->first, itr->second);

	if (p_data) p_data->CopyFrom(itr->second);

	WRITE_COUNTRY_LOG_TO_DB(m_id, common::log_enum::source_type_country_appoint_officer, p_user->get_uid(), officer_id, 0);
}

void country_t::set_officer_appoint(uint32_t officer_id, uint64_t uid, proto::common::country_officer_data* p_data)
{
	std::map<uint32_t, proto::common::country_officer_data>::iterator itr = m_country_info.officers.find(officer_id);
	if (itr == m_country_info.officers.end()) {
		log_error("country[%u] officer[%u] not define", m_id, officer_id);
		return;
	}

	uint32_t country_id = global_user_data_mgr_t::get_user_country_id(uid);
	if (country_id != m_id) {
		log_error("user[%lu] country[%u] != country[%u], when set officer appoint", uid, country_id, m_id);
		return;
	}

	global_user_data_mgr_t::set_user_country_officer(uid, officer_id);

	itr->second.set_uid(common::string_util_t::uint64_to_string(uid));
	itr->second.set_name(global_user_data_mgr_t::get_name(uid));
	itr->second.set_plugin(global_user_data_mgr_t::get_user_hero_plugin(uid));
	itr->second.set_refresh(time_manager_t::get_next_refresh_time(common::time_type_enum::time_one_day));
	itr->second.set_level(global_user_data_mgr_t::get_user_level(uid));
	itr->second.set_fighting(global_user_data_mgr_t::get_user_fighting_value(uid));
	itr->second.set_hero_plugin(global_user_data_mgr_t::get_user_plugin(uid));
	country_data_mgr_t::save_country_officer(m_id, itr->first, itr->second);

	if (p_data) p_data->CopyFrom(itr->second);

	WRITE_COUNTRY_LOG_TO_DB(m_id, common::log_enum::source_type_country_appoint_officer, uid, officer_id, 0);
}

void country_t::fire_officer(uint32_t officer_id, proto::common::country_officer_data* p_data)
{
	std::map<uint32_t, proto::common::country_officer_data>::iterator itr = m_country_info.officers.find(officer_id);
	if (itr == m_country_info.officers.end()) {
		log_error("country[%u] officer[%u] not define", m_id, officer_id);
		return;
	}

	uint64_t uid = common::string_util_t::string_to_uint64(itr->second.uid());
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (p_user) {
		p_user->set_country_officer(0);
	} else {
		global_user_data_mgr_t::set_user_country_officer(uid, 0);
	}

	// 设置空
	itr->second.set_uid("0");
	itr->second.set_name("");
	itr->second.set_plugin(0);
	itr->second.set_level(0);
	itr->second.set_fighting(0);
	itr->second.set_hero_plugin(0);
	country_data_mgr_t::save_country_officer(m_id, itr->first, itr->second);

	if (p_data) p_data->CopyFrom(itr->second);

	WRITE_COUNTRY_LOG_TO_DB(m_id, common::log_enum::source_type_country_fire_officer, uid, officer_id, 0);
}

void country_t::resign_officer(const global_user_ptr& p_user, proto::common::country_officer_data* p_data)
{
	if (!p_user) {
		log_error("p_user == null");
		return;
	}

	std::map<uint32_t, proto::common::country_officer_data>::iterator itr = m_country_info.officers.find(p_user->get_country_officer());
	if (itr == m_country_info.officers.end()) {
		log_error("country[%u] officer[%u] not define", m_id, p_user->get_country_officer());
		return;
	}

	p_user->set_country_officer(0);

	// 设置空
	itr->second.set_uid("0");
	itr->second.set_name("");
	itr->second.set_plugin(0);
	itr->second.set_level(0);
	itr->second.set_fighting(0);
	itr->second.set_hero_plugin(0);
	country_data_mgr_t::save_country_officer(m_id, itr->first, itr->second);

	if (p_data) p_data->CopyFrom(itr->second);

	WRITE_COUNTRY_LOG_TO_DB(m_id, common::log_enum::source_type_country_resign_officer, p_user->get_uid(), itr->first, 0);
}

void country_t::demise_king(const global_user_ptr& p_user, uint64_t uid)
{
	if (p_user == NULL) {
		log_error("p_user == null");
		return;
	}

	std::map<uint32_t, proto::common::country_officer_data>::iterator itr = m_country_info.officers.find(officer_type::officer_type_king);
	if (itr == m_country_info.officers.end()) {
		log_error("country[%u] officer[%u] not define", m_id, officer_type::officer_type_king);
		return;
	}
	uint32_t old_country_officer = 0;
	global_user_ptr p_new_king = global_user_manager_t::get_global_user_by_uid(uid);
	if (p_new_king) {
		// 老的官职
		old_country_officer = p_new_king->get_country_officer();

		p_new_king->set_country_officer(officer_type::officer_type_king);

		itr->second.set_uid(common::string_util_t::uint64_to_string(p_new_king->get_uid()));
		itr->second.set_name(p_new_king->get_name());
		itr->second.set_plugin(p_new_king->get_hero_plugin());
		itr->second.set_refresh(time_manager_t::get_next_refresh_time(common::time_type_enum::time_one_day));
		itr->second.set_level(p_new_king->get_level());
		itr->second.set_fighting(p_new_king->get_fighting_value());
		itr->second.set_hero_plugin(p_new_king->get_plugin());

		// 设置国王家族
		proto::common::country_base_data* p_base_info = m_country_info.info.mutable_base();
		if (p_base_info) { // 国王名字
			p_base_info->set_king_name(p_new_king->get_name());
		}
	} else {
		// 老的官职
		old_country_officer = global_user_data_mgr_t::get_user_country_officer(uid);

		global_user_data_mgr_t::set_user_country_officer(uid, officer_type::officer_type_king);

		std::string name = std::move(global_user_data_mgr_t::get_name(uid));
		itr->second.set_uid(common::string_util_t::uint64_to_string(uid));
		itr->second.set_name(name);
		itr->second.set_plugin(global_user_data_mgr_t::get_user_hero_plugin(uid));
		itr->second.set_refresh(time_manager_t::get_next_refresh_time(common::time_type_enum::time_one_day));
		itr->second.set_level(global_user_data_mgr_t::get_user_level(uid));
		itr->second.set_fighting(global_user_data_mgr_t::get_user_fighting_value(uid));
		itr->second.set_hero_plugin(global_user_data_mgr_t::get_user_plugin(uid));

		// 设置国王家族
		proto::common::country_base_data* p_base_info = m_country_info.info.mutable_base();
		if (p_base_info) { // 国王名字
			p_base_info->set_king_name(name);
		}
	}

	if (old_country_officer != 0) {
		std::map<uint32_t, proto::common::country_officer_data>::iterator old_itr = m_country_info.officers.find(old_country_officer);
		if (old_itr != m_country_info.officers.end()) {
			// 设置空
			old_itr->second.set_uid("0");
			old_itr->second.set_name("");
			old_itr->second.set_plugin(0);
			old_itr->second.set_level(0);
			old_itr->second.set_fighting(0);
			old_itr->second.set_hero_plugin(0);
			country_data_mgr_t::save_country_officer(m_id, old_itr->first, old_itr->second);
		}
	}

	p_user->set_country_officer(0);

	country_data_mgr_t::save_country_officer(m_id, itr->first, itr->second);

	WRITE_COUNTRY_LOG_TO_DB(m_id, common::log_enum::source_type_country_demise_king, p_user->get_uid(), 0, 0);
	WRITE_COUNTRY_LOG_TO_DB(m_id, common::log_enum::source_type_country_appoint_officer, uid, officer_type::officer_type_king, 0);
}

void country_t::get_country_family_list(proto::client::ec_country_family_list_reply& reply)
{
	proto::common::country_info_data* p_base_info = reply.mutable_base_info();
	if (p_base_info) {
		p_base_info->CopyFrom(m_country_info.info);
	}

	proto::common::country_family_list* p_list = reply.mutable_list();
	if (p_list) {
		for (family_force_vec::iterator itr = m_family_force_vec.begin();
			itr != m_family_force_vec.end(); ++itr) {
			itr->peek_data(p_list->add_datas());
		}
	}

	reply.set_reply_code(common::errcode_enum::error_ok);
}

uint32_t country_t::update_new_power(uint32_t rank_value)
{
	auto conf = GET_CONF(Comprehensive, common::comprehensive_common::country_strength_family1);
	uint32_t need_level = GET_COMPREHENSIVE_VALUE_1(conf);

	conf = GET_CONF(Comprehensive, common::comprehensive_common::country_strength_family2);
	uint32_t need_prosperity = GET_COMPREHENSIVE_VALUE_1(conf);

	uint32_t base_add = 0;
	for (family_force_vec::const_iterator citr = m_family_force_vec.begin();
		citr != m_family_force_vec.end(); ++citr) {
		const family_ptr& p_family = family_manager_t::get_family(citr->family_uid);
		if (p_family && p_family->get_level() >= need_level && p_family->get_prosperity() >= need_prosperity) {
			base_add += p_family->get_prestige();
		}
	}

	uint32_t new_power = base_add + rank_value;

	COUNTRY_LOG("country[%u] new_power[%u] = base[%u] + rank_value[%u]", m_id, new_power, base_add, rank_value);

	m_country_info.info.set_power(new_power);
	country_data_mgr_t::update_country_power(m_id, new_power);
	return new_power;
}

void country_t::update_power_level(uint32_t type)
{
	if (m_country_info.info.level_type() != type) {
		m_country_info.info.set_level_type(type);

		country_data_mgr_t::update_power_level(m_id, type);
	}

	COUNTRY_LOG("country[%u] update power[%u]", m_id, type);

	WRITE_COUNTRY_LOG_TO_DB(m_id, common::log_enum::source_type_country_country_level, 0, type, 0);
}

void country_t::update_officer_attr(uint32_t type, const global_user_ptr& p_user)
{
	if (NULL == p_user) {
		log_error("p_user == null");
		return;
	}

	std::map<uint32_t, proto::common::country_officer_data>::iterator itr = m_country_info.officers.find(type);
	if (itr != m_country_info.officers.end()) {
		uint64_t officer_uid = common::string_util_t::string_to_uint64(itr->second.uid());
		if (officer_uid != p_user->get_uid()) {
			log_error("country[%u] officer[%u] uid[%lu] != %lu", m_id, type, officer_uid, p_user->get_uid());
			return;
		}

		itr->second.set_plugin(p_user->get_hero_plugin());
		itr->second.set_level(p_user->get_level());
		itr->second.set_fighting(p_user->get_fighting_value());
		itr->second.set_hero_plugin(p_user->get_plugin());
		itr->second.set_name(p_user->get_name());

		if (type == officer_type::officer_type_king) {
			proto::common::country_base_data* p_base_info = m_country_info.info.mutable_base();
			if (p_base_info) { // 国王家族名字
				p_base_info->set_king_name(p_user->get_name());
			}
		}
	}
}

void country_t::add_money(uint32_t val, uint32_t type, uint64_t uid /*= 0*/)
{
	uint32_t country_money = m_country_info.info.money();
	country_money += val;

	m_country_info.info.set_money(country_money);
	country_data_mgr_t::update_country_money(m_id, country_money);

	WRITE_COUNTRY_LOG_TO_DB(m_id, common::log_enum::source_type_country_money_add, uid, type, country_money);
}

void country_t::cost_money(uint32_t val, uint32_t type, uint64_t uid /*= 0*/)
{
	uint32_t country_money = m_country_info.info.money();
	if (country_money >= val) {
		country_money -= val;
	} else {
		country_money = 0;
	}

	m_country_info.info.set_money(country_money);
	country_data_mgr_t::update_country_money(m_id, country_money);

	WRITE_COUNTRY_LOG_TO_DB(m_id, common::log_enum::source_type_country_money_cost, uid, type, country_money);
}

void country_t::peek_country_flag(proto::client::ec_country_flag_list_reply& reply)
{
	if (m_country_info.m_flag_list.empty()) return;

	for (std::set<uint32_t>::const_iterator citr = m_country_info.m_flag_list.begin();
		citr != m_country_info.m_flag_list.end(); ++citr) {
		reply.add_flag_lists(*citr);
	}
}

uint32_t country_t::modify_name_flag(const global_user_ptr& p_user, const std::string& name, uint32_t flag)
{
	if (p_user == NULL) {
		log_error("p_user == NULL");
		return common::errcode_enum::notice_unknown;
	}

	if (name.empty()) {
		log_error("user[%lu] modify country name empty", p_user->get_uid());
		return common::errcode_enum::country_error_name_empty;
	}

	if (!common::string_util_t::is_no_special_char(name))
	{
		log_error("country name[%s] has special char!", name.c_str());
		return errcode_enum::sys_notice_not_chinese_word;
	}

	auto conf = GET_CONF(Comprehensive, comprehensive_common::country_name_number_upper);
	uint32_t name_number = GET_COMPREHENSIVE_VALUE_1(conf);
	uint32_t name_cnt = common::string_util_t::get_char_size(name);
	if (name_cnt != name_number) {
		log_error("user[%lu] modify country name size[%u] != %u", p_user->get_uid(), name_cnt, name_number);
		return common::errcode_enum::country_error_name_size_limit;
	}

	if (country_mgr_t::is_name_repeate(name, m_id)) {
		log_error("user[%lu] modify country name repeate", p_user->get_uid());
		return common::errcode_enum::country_error_name_repeat;
	}

	if (0 == strcmp(name.c_str(), m_country_info.info.base().name().c_str()) && flag == m_country_info.info.base().flag()) {
		COUNTRY_LOG("user[%lu] modify country name and flag same", p_user->get_uid());
		return common::errcode_enum::country_error_name_no_change;
	}

	CountryFlag* p_conf_flag = GET_CONF(CountryFlag, flag);
	if (!p_conf_flag) {
		log_error("user[%lu] country flag[%u] not define", p_user->get_uid(), flag);
		return common::errcode_enum::notice_unknown;
	}

	if (p_conf_flag->unlock_condi() != 0 && !is_have_flag(flag)) { // 默认不解锁
		COUNTRY_LOG("user[%lu] country flag[%u] not activate", p_user->get_uid(), flag);
		return common::errcode_enum::country_error_flag_not_activation;
	}

	if (m_country_info.info.free_times() > 0) { // 免费修改
		do_modify_country_name(p_user->get_uid(), name);
		do_modify_country_flag(p_user->get_uid(), flag);

		m_country_info.info.set_free_times(m_country_info.info.free_times() - 1);
		country_data_mgr_t::update_country_free_times(m_id, m_country_info.info.free_times());

		proto::client::ec_change_country_name_reply reply;
		reply.set_reply_code(common::errcode_enum::error_ok);
		reply.set_is_free(1);
		peek_country_info(reply.mutable_data());
		p_user->send_msg_to_client(op_cmd::ec_change_country_name_reply, reply);

		// 同步game
		proto::server::eg_update_country_notify ntf_game;
		peek_common_data(ntf_game.add_countries());
		env::server->send_msg_to_all_games(op_cmd::eg_update_country_notify, 0, ntf_game);

		// 全服广播这个国家的名字改了
		proto::client::ec_update_country_notify notify;
		peek_common_data(notify.mutable_country());
		sys_notice_manager_t::broadcast_msg_to_all(op_cmd::ec_update_country_notify, notify);

		WRITE_COUNTRY_LOG_TO_DB(m_id, common::log_enum::source_type_country_change_name_free, p_user->get_uid(), 0, 0);
	} else { // 通知game扣钱
		proto::server::eg_country_name_change_request eg_req;
		eg_req.set_name(name);
		eg_req.set_flag_id(flag);
		p_user->send_msg_to_game(op_cmd::eg_country_name_change_request, eg_req);
	}

	return common::errcode_enum::error_ok;
}

bool country_t::is_have_flag(uint32_t flag)
{
	std::set<uint32_t>::const_iterator citr = m_country_info.m_flag_list.find(flag);
	return citr != m_country_info.m_flag_list.end();
}

void country_t::do_modify_country_name(uint64_t uid, const std::string& name)
{
	if (0 != strcmp(name.c_str(), m_country_info.info.base().name().c_str())) {
		proto::common::country_base_data* p_data = m_country_info.info.mutable_base();
		if (p_data) p_data->set_name(name);

		country_data_mgr_t::update_country_name(m_id, name);

		WRITE_COUNTRY_LOG_TO_DB(m_id, common::log_enum::source_type_country_change_name, uid, 0, 0);
	}
}

void country_t::do_modify_country_flag(uint64_t uid, uint32_t flag)
{
	if (flag != m_country_info.info.base().flag()) {
		proto::common::country_base_data* p_data = m_country_info.info.mutable_base();
		if (p_data) p_data->set_flag(flag);

		country_data_mgr_t::update_country_flag(m_id, flag);

		WRITE_COUNTRY_LOG_TO_DB(m_id, common::log_enum::source_type_country_change_flag, uid, flag, 0);
	}
}

void country_t::peek_common_data(proto::common::country_common* p_base)
{
	if (p_base) {
		p_base->set_country_id(m_id);
		p_base->set_country_name(get_country_name());
		p_base->set_country_flag(get_country_flag());
	}
}

void country_t::get_king_war_sign_up(uint64_t family_id, proto::common::king_war_country_sign_up_single* info)
{
	if (info) {
		peek_base_data(info->mutable_country_base());
		info->set_money(m_country_info.info.money());
		info->set_power(m_country_info.info.power());
		info->set_family_number(m_family_force_vec.size());
		info->set_sign_up_number(m_king_war_apply_map.size());

		king_war_apply_map::const_iterator citr = m_king_war_apply_map.find(family_id);
		if (citr != m_king_war_apply_map.end()) {
			info->set_is_sign_up(true);
		} else {
			info->set_is_sign_up(false);
		}
	}
}

void country_t::get_king_war_fight(uint64_t uid, proto::common::king_war_country_pre_single* info)
{
	if (info) {
		info->set_country_id(m_id);
		peek_king_war_fight_by_type(king_war_defensive, uid, info->mutable_left_family());
		peek_king_war_fight_by_type(king_war_attack, uid, info->mutable_right_family());
	}
}

uint32_t country_t::get_king_war_family_type(uint64_t family_id) const
{
	if (m_country_info.m_king_war_info.m_family_defensive == family_id) {
		return king_war_defensive;
	} else if (m_country_info.m_king_war_info.m_family_attack == family_id) {
		return king_war_attack;
	}

	return king_war_type_error;
}

void country_t::update_king_war_notice(const global_user_ptr& p_user, const std::string& notice)
{
	if (p_user == NULL) {
		log_error("p_user null error");
		return;
	}

	auto conf = GET_CONF(Comprehensive, comprehensive_common::king_declaration_num);
	uint32_t declaration_num = GET_COMPREHENSIVE_VALUE_1(conf);

	if (m_country_info.m_king_war_info.m_family_defensive == p_user->get_family_id()) {
		m_country_info.m_king_war_info.m_defensive_notice_user = p_user->get_name();
		m_country_info.m_king_war_info.m_defensive_notice_officer = p_user->get_family_officer();
		if (common::string_util_t::get_character_cnt(notice) > declaration_num) {
			m_country_info.m_king_war_info.m_defensive_notice = common::string_util_t::substr(notice, 0, declaration_num);
		} else {
			m_country_info.m_king_war_info.m_defensive_notice = notice;
		}
		country_data_mgr_t::update_country_notice_info(m_id, king_war_defensive, m_country_info.m_king_war_info);
	} else if (m_country_info.m_king_war_info.m_family_attack == p_user->get_family_id()) {
		m_country_info.m_king_war_info.m_attack_notice_user = p_user->get_name();
		m_country_info.m_king_war_info.m_attack_notice_officer = p_user->get_family_officer();
		if (common::string_util_t::get_character_cnt(notice) > declaration_num) {
			m_country_info.m_king_war_info.m_attack_notice = common::string_util_t::substr(notice, 0, declaration_num);
		} else {
			m_country_info.m_king_war_info.m_attack_notice = notice;
		}
		country_data_mgr_t::update_country_notice_info(m_id, king_war_attack, m_country_info.m_king_war_info);
	}
}

void country_t::peek_king_war_fight_by_type(uint32_t type, uint64_t uid, proto::common::king_war_family_info* info)
{
	if (!info) {
		log_error("info null error");
		return;
	}

	if (type == king_war_defensive) {
		info->set_family_id(common::string_util_t::uint64_to_string(m_country_info.m_king_war_info.m_family_defensive));
		const family_ptr& p_def_family = family_manager_t::get_family(m_country_info.m_king_war_info.m_family_defensive);
		if (NULL != p_def_family) {
			info->set_family_name(p_def_family->get_name());
			info->set_is_king_family(m_country_info.m_king_war_info.m_family_defensive == m_country_info.m_king_family_id);
			if (m_country_info.m_king_war_info.m_defensive_help_family != 0) {
				const family_ptr& p_def_help_family = family_manager_t::get_family(m_country_info.m_king_war_info.m_defensive_help_family);
				if (p_def_help_family) {
					info->set_friend_family_name(p_def_help_family->get_name());
				}
				info->set_friend_family_id(common::string_util_t::uint64_to_string(m_country_info.m_king_war_info.m_defensive_help_family));
			} else {
				info->set_friend_family_id("0");
			}
			info->set_notice_user(m_country_info.m_king_war_info.m_defensive_notice_user);
			info->set_family_officer(m_country_info.m_king_war_info.m_defensive_notice_officer);
			info->set_notice(m_country_info.m_king_war_info.m_defensive_notice);
			info->set_like_num(m_country_info.m_king_war_info.m_defensive_like_num);
			info->set_dislike_num(m_country_info.m_king_war_info.m_defensive_dislike_num);
			info->set_is_like(redis_client_t::is_set_member(m_family_like_key[type].c_str(), uid));
		}
	} else if (type == king_war_attack) {
		info->set_family_id(common::string_util_t::uint64_to_string(m_country_info.m_king_war_info.m_family_attack));
		const family_ptr& p_att_family = family_manager_t::get_family(m_country_info.m_king_war_info.m_family_attack);
		if (NULL != p_att_family) {
			info->set_family_name(p_att_family->get_name());
			info->set_is_king_family(m_country_info.m_king_war_info.m_family_attack == m_country_info.m_king_family_id);
			if (m_country_info.m_king_war_info.m_attack_help_family != 0) {
				const family_ptr& p_att_help_family = family_manager_t::get_family(m_country_info.m_king_war_info.m_attack_help_family);
				if (p_att_help_family) {
					info->set_friend_family_name(p_att_help_family->get_name());
				}
				info->set_friend_family_id(common::string_util_t::uint64_to_string(m_country_info.m_king_war_info.m_attack_help_family));
			} else {
				info->set_friend_family_id("0");
			}
			info->set_notice_user(m_country_info.m_king_war_info.m_attack_notice_user);
			info->set_family_officer(m_country_info.m_king_war_info.m_attack_notice_officer);
			info->set_notice(m_country_info.m_king_war_info.m_attack_notice);
			info->set_like_num(m_country_info.m_king_war_info.m_attack_like_num);
			info->set_dislike_num(m_country_info.m_king_war_info.m_attack_dislike_num);
			info->set_is_like(redis_client_t::is_set_member(m_family_like_key[type].c_str(), uid));
		}
	}
}

bool country_t::is_do_like(uint32_t type, uint64_t uid)
{
	if (type < king_war_type_max) {
		return redis_client_t::is_set_member(m_family_like_key[type].c_str(), uid);
	}

	return true;
}

void country_t::do_king_war_like(uint32_t type, uint64_t uid, uint32_t like)
{
	if (type == king_war_defensive) {
		if (like == 0) {
			m_country_info.m_king_war_info.m_defensive_like_num += 1;
		} else {
			m_country_info.m_king_war_info.m_defensive_dislike_num += 1;
		}

		country_data_mgr_t::update_country_like_info(m_id, type, m_country_info.m_king_war_info);
		redis_client_t::add_set_member(m_family_like_key[type].c_str(), uid);
	} else if (type == king_war_attack) {
		if (like == 0) {
			m_country_info.m_king_war_info.m_attack_like_num += 1;
		} else {
			m_country_info.m_king_war_info.m_attack_dislike_num += 1;
		}

		country_data_mgr_t::update_country_like_info(m_id, type, m_country_info.m_king_war_info);
		redis_client_t::add_set_member(m_family_like_key[type].c_str(), uid);
	}
}

void country_t::reset_king_war()
{
	// 清空
	m_king_war_apply_map.clear();

	if (m_country_info.m_king_war_info.m_family_defensive != 0) {
		family_ptr p_family_defensive = family_manager_t::get_family(m_country_info.m_king_war_info.m_family_defensive);
		if (p_family_defensive) {
			p_family_defensive->set_king_war_fight_country(0);
		}
	}

	if (m_country_info.m_king_war_info.m_defensive_help_family != 0) {
		family_ptr p_defensive_help = family_manager_t::get_family(m_country_info.m_king_war_info.m_defensive_help_family);
		if (p_defensive_help) {
			p_defensive_help->set_king_war_fight_country(0);
		}
	}

	if (m_country_info.m_king_war_info.m_family_attack != 0) {
		family_ptr p_family_attack = family_manager_t::get_family(m_country_info.m_king_war_info.m_family_attack);
		if (p_family_attack) {
			p_family_attack->set_king_war_fight_country(0);
		}
	}

	if (m_country_info.m_king_war_info.m_attack_help_family != 0) {
		family_ptr p_attack_help = family_manager_t::get_family(m_country_info.m_king_war_info.m_attack_help_family);
		if (p_attack_help) {
			p_attack_help->set_king_war_fight_country(0);
		}
	}

	m_country_info.m_king_war_info.reset();
	country_data_mgr_t::clear_king_war_info(m_id);
	// 清理点赞信息
	for (uint32_t i = 0; i < king_war_type_max; ++i) {
		redis_client_t::del_set_all(m_family_like_key[i].c_str());
	}
}

void country_t::king_war_user_on_login(const global_user_ptr& p_user)
{
	if (p_user == NULL) {
		log_error("p_user null error");
		return;
	}

	// 不是备战或开战不用通知
	if (m_country_info.m_king_war_info.m_state < proto::common::king_war_state_prepare) {
		return;
	}

	if (is_king_war_family(p_user->get_family_id())) {
		proto::client::ec_king_war_notice_notify ntf;
		peek_king_war_state(ntf);
		p_user->send_msg_to_client(op_cmd::ec_king_war_notice_notify, ntf);
	}
}

void country_t::peek_king_war_state(proto::client::ec_king_war_notice_notify& ntf)
{
	ntf.set_country_id(m_id);
	ntf.set_state(m_country_info.m_king_war_info.m_state);

	if (m_country_info.m_king_war_info.m_state != proto::common::king_war_state_end)
	{
		uint32_t time_now = common::time_util_t::now_time();
		// 开启的时候显示结束时间
		if (m_country_info.m_king_war_info.m_state == proto::common::king_war_state_start)
		{
			if (m_country_info.m_king_war_info.m_state_end_time > time_now)
			{
				ntf.set_time(m_country_info.m_king_war_info.m_state_end_time - time_now);
			} else
			{
				ntf.set_time(0);
			}
		} else
		{
			if (m_country_info.m_king_war_info.m_start_time > time_now)
			{
				ntf.set_time(m_country_info.m_king_war_info.m_start_time - time_now);
			} else
			{
				ntf.set_time(0);
			}
		}

		ntf.set_def_family_id(common::string_util_t::uint64_to_string(m_country_info.m_king_war_info.m_family_defensive));
		const family_ptr& p_def_family = family_manager_t::get_family(m_country_info.m_king_war_info.m_family_defensive);
		if (p_def_family != NULL)
		{
			ntf.set_defensive_name(p_def_family->get_name());
		}

		ntf.set_att_family_id(common::string_util_t::uint64_to_string(m_country_info.m_king_war_info.m_family_attack));
		const family_ptr& p_att_family = family_manager_t::get_family(m_country_info.m_king_war_info.m_family_attack);
		if (p_att_family != NULL)
		{
			ntf.set_attack_name(p_att_family->get_name());
		}

		if (m_country_info.m_king_war_info.m_defensive_help_family != 0) {
			const family_ptr& p_def_help_family = family_manager_t::get_family(m_country_info.m_king_war_info.m_defensive_help_family);
			if (NULL != p_def_help_family) {
				ntf.set_def_help_family(common::string_util_t::uint64_to_string(m_country_info.m_king_war_info.m_defensive_help_family));
				ntf.set_def_help_family_name(p_def_help_family->get_name());
			}
		}

		if (m_country_info.m_king_war_info.m_attack_help_family != 0) {
			const family_ptr& p_att_help_family = family_manager_t::get_family(m_country_info.m_king_war_info.m_attack_help_family);
			if (NULL != p_att_help_family) {
				ntf.set_att_help_family(common::string_util_t::uint64_to_string(m_country_info.m_king_war_info.m_attack_help_family));
				ntf.set_att_help_family_name(p_att_help_family->get_name());
			}
		}

		ntf.set_is_king(m_country_info.m_king_war_info.m_family_defensive == m_country_info.m_king_family_id);
	} 
	else
	{
		ntf.set_def_family_id(common::string_util_t::uint64_to_string(m_country_info.m_king_war_info.m_family_defensive));
		ntf.set_att_family_id(common::string_util_t::uint64_to_string(m_country_info.m_king_war_info.m_family_attack));

		if (m_country_info.m_king_war_info.m_defensive_help_family != 0) {
			ntf.set_def_help_family(common::string_util_t::uint64_to_string(m_country_info.m_king_war_info.m_defensive_help_family));
		}

		if (m_country_info.m_king_war_info.m_attack_help_family != 0) {
			ntf.set_att_help_family(common::string_util_t::uint64_to_string(m_country_info.m_king_war_info.m_attack_help_family));
		}
	}
}

bool country_t::is_king_war_family(uint64_t family_id)
{
	if (family_id == 0) {
		return false;
	}

	if (family_id == m_country_info.m_king_war_info.m_family_defensive ||
		family_id == m_country_info.m_king_war_info.m_family_attack ||
		family_id == m_country_info.m_king_war_info.m_defensive_help_family ||
		family_id == m_country_info.m_king_war_info.m_attack_help_family) {
		return true;
	}

	return false;
}

void country_t::set_king_war_scene_role_num(uint32_t def_num, uint32_t att_num)
{
	if (m_country_info.m_king_war_info.m_state < proto::common::king_war_state_prepare) {
		return;
	}

	m_country_info.m_king_war_info.m_defensive_num = def_num;
	m_country_info.m_king_war_info.m_attack_num = att_num;
}

uint32_t country_t::get_king_war_help_family_type(uint64_t family_id) const
{
	if (m_country_info.m_king_war_info.m_defensive_help_family == family_id) {
		return king_war_defensive;
	} else if (m_country_info.m_king_war_info.m_attack_help_family == family_id) {
		return king_war_attack;
	}

	return king_war_type_error;
}

uint64_t country_t::get_king_war_help_family(uint32_t type) const
{
	switch (type) {
		case king_war_defensive: {
			return m_country_info.m_king_war_info.m_defensive_help_family;
			break;
		}
		case king_war_attack: {
			return m_country_info.m_king_war_info.m_attack_help_family;
			break;
		}
		default: {
			break;
		}
	}

	return 0;
}

const std::set<uint64_t>& country_t::get_king_war_invited_list(uint32_t type) const
{
	if (type < king_war_type_max) {
		return m_country_info.m_king_war_info.m_invited_family[type];
	}

	log_error("country[%u] get king_war_invited_list type[%u] error", m_id, type);
	return m_country_info.m_king_war_info.m_invited_family[king_war_defensive];
}

const std::set<uint64_t>& country_t::get_king_war_refused_list(uint32_t type) const
{
	if (type < king_war_type_max) {
		return m_country_info.m_king_war_info.m_refused_family[type];
	}

	log_error("country[%u] get king_war_invited_list type[%u] error", m_id, type);
	return m_country_info.m_king_war_info.m_refused_family[king_war_defensive];
}

void country_t::set_king_war_help_family(uint32_t type, const family_ptr& p_invite_family)
{
	if (p_invite_family == NULL) {
		log_error("p_invite_family null error");
		return;
	}

	if (p_invite_family->get_king_war_fight_country() != 0) {
		log_error("p_invite_family[%lu] king_war_fight_country != 0 error", p_invite_family->get_family_id());
		return;
	}

	p_invite_family->set_king_war_fight_country(m_id);

	switch (type) {
		case king_war_defensive: {
			m_country_info.m_king_war_info.m_defensive_help_family = p_invite_family->get_family_id();
			country_data_mgr_t::update_country_help_family(m_id, type, p_invite_family->get_family_id());
			break;
		}
		case king_war_attack: {
			m_country_info.m_king_war_info.m_attack_help_family = p_invite_family->get_family_id();
			country_data_mgr_t::update_country_help_family(m_id, type, p_invite_family->get_family_id());
			break;
		}
		default: {
			break;
		}
	}
}

void country_t::add_king_war_help_invited_list(uint32_t type, uint64_t family_uid)
{
	if (type < king_war_type_max) {
		m_country_info.m_king_war_info.m_invited_family[type].insert(family_uid);
	}
}

void country_t::add_king_war_help_refused_list(uint32_t type, uint64_t family_uid)
{
	if (type < king_war_type_max) {
		m_country_info.m_king_war_info.m_refused_family[type].insert(family_uid);
	}
}

uint64_t country_t::is_king_war_fight_invite(uint32_t type, uint64_t family_uid)
{
	if (type == king_war_defensive) {
		std::set<uint64_t>::const_iterator citr = m_country_info.m_king_war_info.m_invited_family[king_war_defensive].find(family_uid);
		if (citr != m_country_info.m_king_war_info.m_invited_family[king_war_defensive].end()) {
			return m_country_info.m_king_war_info.m_family_defensive;
		}
	} else if (type == king_war_attack) {
		std::set<uint64_t>::const_iterator citr = m_country_info.m_king_war_info.m_invited_family[king_war_attack].find(family_uid);
		if (citr != m_country_info.m_king_war_info.m_invited_family[king_war_attack].end()) {
			return m_country_info.m_king_war_info.m_family_attack;
		}
	}

	return 0;
}

void country_t::del_king_war_fight_invite(uint32_t type, uint64_t family_id)
{
	if (type < king_war_type_max) {
		m_country_info.m_king_war_info.m_invited_family[type].erase(family_id);
	}
}

void country_t::agree_king_war_fight_invite(uint32_t type, const family_ptr& p_invited_family, const family_ptr& p_family)
{
	if (p_invited_family == NULL) {
		log_error("p_invited_family null error");
		return;
	}

	if (p_family == NULL) {
		log_error("p_family null error");
		return;
	}

	if (type >= king_war_type_error) {
		return;
	}

	if (p_family->get_king_war_fight_country() != 0) {
		log_error("family[%lu] is already king war fight family", p_family->get_family_id());
		return;
	}

	// 清理所有其他的邀请
	m_country_info.m_king_war_info.m_invited_family[type].clear();
	m_country_info.m_king_war_info.m_refused_family[type].erase(p_family->get_family_id());

	// 设置为参战家族
	p_family->set_king_war_fight_country(m_id);

	WRITE_COUNTRY_LOG_TO_DB(m_id, common::log_enum::source_type_country_king_war_help_add, p_family->get_family_id(), type, 0);

	if (type == king_war_defensive) {
		m_country_info.m_king_war_info.m_defensive_help_family = p_family->get_family_id();
	} else if (type == king_war_attack) {
		m_country_info.m_king_war_info.m_attack_help_family = p_family->get_family_id();
	}

	// redis 存储
	country_data_mgr_t::update_country_help_family(m_id, type, p_family->get_family_id());

	Country* p_country_conf = GET_CONF(Country, m_id);
	if (p_country_conf) {
		struct tm day_time;
		common::time_util_t::get_local_time(day_time, m_country_info.m_king_war_info.m_start_time);
		uint32_t mon = day_time.tm_mon + 1;
		uint32_t day = day_time.tm_mday;

		// 给参战家族发邮件
		std::vector<uint64_t> members;
		std::map<uint32_t, uint32_t> item_map;
		const family::family_member_map& member_map = p_invited_family->get_all_member();
		for (family::family_member_map::const_iterator citr = member_map.begin();
			citr != member_map.end(); ++citr) {
			members.push_back(citr->first);
		}

		MailTable* p_conf = GET_CONF(MailTable, common::mailcode_enum::sys_mail_assist_to_main);
		if (p_conf) {
			std::string content_text;
			StringTool::format(content_text, common::string_util_t::convert_to_utf8(p_conf->content()).c_str(), mon, day, common::string_util_t::convert_to_utf8(p_country_conf->capital_name()).c_str(), p_family->get_name().c_str());

			mail_manager_t::send_mail(members, proto::common::MAIL_TYPE_SYS, 0, common::string_util_t::convert_to_utf8(p_conf->from_name()), common::string_util_t::convert_to_utf8(p_conf->title()), content_text,
				item_map);
		}

		// 给助战家族发邮件
		members.clear();
		const family::family_member_map& member_map2 = p_family->get_all_member();
		for (family::family_member_map::const_iterator citr = member_map2.begin();
			citr != member_map2.end(); ++citr) {
			members.push_back(citr->first);
		}

		p_conf = GET_CONF(MailTable, common::mailcode_enum::sys_mail_assist_to_friend);
		if (p_conf) {
			std::string content_text;
			StringTool::format(content_text, common::string_util_t::convert_to_utf8(p_conf->content()).c_str(), mon, day, common::string_util_t::convert_to_utf8(p_country_conf->capital_name()).c_str(), p_invited_family->get_name().c_str());

			mail_manager_t::send_mail(members, proto::common::MAIL_TYPE_SYS, 0, common::string_util_t::convert_to_utf8(p_conf->from_name()), common::string_util_t::convert_to_utf8(p_conf->title()), content_text,
				item_map);
		}
	} else {
		log_error("country[%u] config not find", m_id);
	}
}

void country_t::del_all_king_war_fight_invite(uint64_t family_id)
{
	for (uint32_t i = 0; i < king_war_type_max; ++i) {
		std::set<uint64_t>::iterator itr = m_country_info.m_king_war_info.m_invited_family[i].find(family_id);
		if (itr != m_country_info.m_king_war_info.m_invited_family[i].end()) {
			m_country_info.m_king_war_info.m_invited_family[i].erase(itr);
		}
	}
}

void country_t::cancel_king_war_fight_invite(uint32_t type, const family_ptr& p_invite_family)
{
	if (type >= king_war_type_error) {
		return;
	}

	if (!p_invite_family) {
		log_error("p_invite_family null error");
		return;
	}

	family_ptr p_family = NULL;
	if (type == king_war_defensive) {
		p_family = family_manager_t::get_family(m_country_info.m_king_war_info.m_defensive_help_family);
		m_country_info.m_king_war_info.m_defensive_help_family = 0;
	} else if (type == king_war_attack) {
		p_family = family_manager_t::get_family(m_country_info.m_king_war_info.m_attack_help_family);
		m_country_info.m_king_war_info.m_attack_help_family = 0;
	}

	if (p_family) {
		p_family->set_king_war_fight_country(0);

		WRITE_COUNTRY_LOG_TO_DB(m_id, common::log_enum::source_type_country_king_war_help_del, p_family->get_family_id(), type, 0);

		Country* p_country_conf = GET_CONF(Country, m_id);
		if (p_country_conf) {
			struct tm day_time;
			common::time_util_t::get_local_time(day_time, m_country_info.m_king_war_info.m_start_time);
			uint32_t mon = day_time.tm_mon + 1;
			uint32_t day = day_time.tm_mday;

			// 给助战家族发邮件
			std::vector<uint64_t> members;
			std::map<uint32_t, uint32_t> item_map;
			const family::family_member_map& member_map = p_family->get_all_member();
			for (family::family_member_map::const_iterator citr = member_map.begin();
				citr != member_map.end(); ++citr) {
				members.push_back(citr->first);
			}

			MailTable* p_conf = GET_CONF(MailTable, common::mailcode_enum::sys_mail_main_cancel_assist);
			if (p_conf) {
				std::string content_text;
				StringTool::format(content_text, common::string_util_t::convert_to_utf8(p_conf->content()).c_str(), mon, day, common::string_util_t::convert_to_utf8(p_country_conf->capital_name()).c_str(), p_invite_family->get_name().c_str());

				mail_manager_t::send_mail(members, proto::common::MAIL_TYPE_SYS, 0, common::string_util_t::convert_to_utf8(p_conf->from_name()), common::string_util_t::convert_to_utf8(p_conf->title()), content_text,
					item_map);
			}
		} else {
			log_error("country[%u] config not find", m_id);
		}
	}
	// redis 存储
	country_data_mgr_t::update_country_help_family(m_id, type, 0);
}

void country_t::cancel_king_war_fight_help(uint32_t type, const family_ptr& p_family)
{
	if (type >= king_war_type_error) {
		return;
	}

	if (!p_family) {
		log_error("p_family null error");
		return;
	}

	family_ptr p_invited_family = NULL;
	if (type == king_war_defensive) {
		m_country_info.m_king_war_info.m_defensive_help_family = 0;
		p_invited_family = family_manager_t::get_family(m_country_info.m_king_war_info.m_family_defensive);
	} else if (type == king_war_attack) {
		m_country_info.m_king_war_info.m_attack_help_family = 0;
		p_invited_family = family_manager_t::get_family(m_country_info.m_king_war_info.m_family_attack);
	}

	p_family->set_king_war_fight_country(0);

	WRITE_COUNTRY_LOG_TO_DB(m_id, common::log_enum::source_type_country_king_war_help_del, p_family->get_family_id(), type, 0);

	// redis 存储
	country_data_mgr_t::update_country_help_family(m_id, type, 0);

	if (p_invited_family == NULL) {
		log_error("p_invited_family null error");
		return;
	}

	Country* p_country_conf = GET_CONF(Country, m_id);
	if (p_country_conf) {
		struct tm day_time;
		common::time_util_t::get_local_time(day_time, m_country_info.m_king_war_info.m_start_time);
		uint32_t mon = day_time.tm_mon + 1;
		uint32_t day = day_time.tm_mday;

		// 给参战家族发邮件
		std::vector<uint64_t> members;
		std::map<uint32_t, uint32_t> item_map;
		const family::family_member_map& member_map = p_invited_family->get_all_member();
		for (family::family_member_map::const_iterator citr = member_map.begin();
			citr != member_map.end(); ++citr) {
			members.push_back(citr->first);
		}

		MailTable* p_conf = GET_CONF(MailTable, common::mailcode_enum::sys_mail_cancel_assist_to_main);
		if (p_conf) {
			std::string content_text;
			StringTool::format(content_text, common::string_util_t::convert_to_utf8(p_conf->content()).c_str(), mon, day, common::string_util_t::convert_to_utf8(p_country_conf->capital_name()).c_str(), p_family->get_name().c_str());

			mail_manager_t::send_mail(members, proto::common::MAIL_TYPE_SYS, 0, common::string_util_t::convert_to_utf8(p_conf->from_name()), common::string_util_t::convert_to_utf8(p_conf->title()), content_text,
				item_map);
		}

		// 给助战家族发邮件
		members.clear();
		const family::family_member_map& member_map2 = p_family->get_all_member();
		for (family::family_member_map::const_iterator citr = member_map2.begin();
			citr != member_map2.end(); ++citr) {
			members.push_back(citr->first);
		}

		p_conf = GET_CONF(MailTable, common::mailcode_enum::sys_mail_cancel_assist_to_friend);
		if (p_conf) {
			std::string content_text;
			StringTool::format(content_text, common::string_util_t::convert_to_utf8(p_conf->content()).c_str(), mon, day, common::string_util_t::convert_to_utf8(p_country_conf->capital_name()).c_str(), p_invited_family->get_name().c_str());

			mail_manager_t::send_mail(members, proto::common::MAIL_TYPE_SYS, 0, common::string_util_t::convert_to_utf8(p_conf->from_name()), common::string_util_t::convert_to_utf8(p_conf->title()), content_text,
				item_map);
		}
	} else {
		log_error("country[%u] config not find", m_id);
	}
}

