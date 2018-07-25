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

#define WRITE_COUNTRY_LOG_TO_DB(country, type, param1, param2, param3) \
    log_wrapper_t::send_country_log(country, type, param1, param2, param3);

country_t::country_t(uint32_t country_id) 
	: m_id(country_id)
{
}

void country_t::init(uint32_t now_time)
{
	// 对国家中的全部家族排名
	calc_rank();

	// 没有王城战信息
	if (m_country_info.m_king_war_info.m_state == proto::common::king_war_state_end)
		return;

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

					// TODO:记日志报名成功，进入预告期
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
		{
			// 起服的时候，时间已经过了这个这场王城争夺战的最后时间
			if (now_time >= m_country_info.m_king_war_info.m_start_time)
			{
				king_war_end(false, 0);
			}
			else if (now_time >= m_country_info.m_king_war_info.m_state_end_time) // 起服的时候，时间已经过了这个这场王城争夺战的备战期
			{
				// 状态不变，时间回调会处理
			}
		}
		break;
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

	// 其他的家族竞价资金退回
	for (king_war_apply_map::iterator itr = m_king_war_apply_map.begin();
		itr != m_king_war_apply_map.end(); ++itr)
	{
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

		const family_ptr& p_family = family_manager_t::get_family(itr->first);
		if (NULL == p_family)
		{
			log_error("p_family[%lu] null error", itr->first);
			continue;
		}

		// 战斗没开启退回竞价资金
		p_family->add_money(p_king_war_apply->m_bid_cost, proto::server::family_money_change_king_war_fail_back);
	}

	// 清空
	m_king_war_apply_map.clear();

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

		const family_ptr& p_family = family_manager_t::get_family(itr->first);
		if (NULL == p_family)
		{
			log_error("p_family[%lu] null error", itr->first);
			continue;
		}

		// 战斗没开启退回竞价资金
		p_family->add_money(p_king_war_apply->m_bid_cost, proto::server::family_money_change_king_war_error_back);
	}

	// 清空
	m_king_war_apply_map.clear();
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
				// 报名的家族直接成为国王家族
				king_war_apply_ptr p_win_apply = get_king_war_attack_family();
				if (p_win_apply != NULL)
				{
					m_country_info.m_king_family_id = p_win_apply->m_family_id;

					country_data_mgr_t::update_country_king_family_id(m_id, p_win_apply->m_family_id);

					KINGWAR_LOG("country[%u] only has one sign up family, new king family[%lu]", m_id, p_win_apply->m_family_id);
				}

				// 清空
				m_king_war_apply_map.clear();

				// 这种情况也算正常结束
				is_normal_end = true;
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

	// 结束的公告
	m_country_info.m_king_war_info.m_state = proto::common::king_war_state_end;

	notify_self_king_war_state();

	// 重置王城战信息
	m_country_info.m_king_war_info.reset();
	country_data_mgr_t::clear_king_war_info(m_id);

	// TODO：记日志，王城战结果
	if (is_normal_end)
	{
		log_info("country[%u] fight success end, win family[%u]", m_id, m_country_info.m_king_family_id);

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
				} else {
					global_user_data_mgr_t::set_user_country_officer(deputy_general_id, officer_type::officer_type_prime_minister);

					m_country_info.officers[officer_type::officer_type_prime_minister].set_name(global_user_data_mgr_t::get_name(deputy_general_id));
					m_country_info.officers[officer_type::officer_type_prime_minister].set_plugin(global_user_data_mgr_t::get_user_hero_plugin(deputy_general_id));
					m_country_info.officers[officer_type::officer_type_prime_minister].set_level(global_user_data_mgr_t::get_user_level(deputy_general_id));
					m_country_info.officers[officer_type::officer_type_prime_minister].set_fighting(global_user_data_mgr_t::get_user_fighting_value(deputy_general_id));
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
		m_country_info.m_king_war_info.reset();
		country_data_mgr_t::clear_king_war_info(m_id);

		log_error("country[%u] start_king_war_sign_up error m_state[%u] != proto::common::king_war_state_end", m_id, m_country_info.m_king_war_info.m_state);
	}

	// 阶段
	m_country_info.m_king_war_info.m_state = proto::common::king_war_state_sign_up;
	m_country_info.m_king_war_info.m_state_end_time = state_end_time;

	// 王城战开始结束时间
	m_country_info.m_king_war_info.m_start_time = start_time;

	// save
	country_data_mgr_t::update_country_king_war_info(m_id, m_country_info.m_king_war_info);

	// TODO:写日志
	KINGWAR_LOG("country[%u] start king war sign up!!!!!!", m_id);
}

void country_t::end_king_war_sign_up()
{
	// 状态验证(可能这国家王城战没有符合条件的家族)
	if (m_country_info.m_king_war_info.m_state != proto::common::king_war_state_sign_up)
	{
		log_warn("country[%u] end_king_war_sign_up error m_state[%u] != proto::common::king_war_state_pre", m_id, m_country_info.m_king_war_info.m_state);
		return;
	}

	// 验证家族战报名信息 如果成功状态设置为预告期
	if (king_war_apply_end())
	{
		m_country_info.m_king_war_info.m_state = proto::common::king_war_state_pre;
		m_country_info.m_king_war_info.m_state_end_time = 0;
		//save
		country_data_mgr_t::update_country_king_war_info(m_id, m_country_info.m_king_war_info);

		// TODO:记日志报名成功，进入预告期

		KINGWAR_LOG("country[%u] sign up success def_family[%lu] att_family[%lu]", m_id, m_country_info.m_king_war_info.m_family_defensive, m_country_info.m_king_war_info.m_family_attack);

		notify_self_king_war_state();

		// 系统通知
		const family_ptr& p_att_family = family_manager_t::get_family(m_country_info.m_king_war_info.m_family_defensive);
		const family_ptr& p_def_family = family_manager_t::get_family(m_country_info.m_king_war_info.m_family_attack);

		if (NULL != p_att_family && NULL != p_def_family)
		{
			BROADCAST_SYS_NOTICE(sys_notice_enum::sys_notice_21, 0, 0, m_id, p_att_family->get_name().c_str(), p_def_family->get_name().c_str());
		}
	}
	else
	{
		king_war_end(false, 0);

		// TODO:公告
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

	//save
	country_data_mgr_t::update_country_king_war_info(m_id, m_country_info.m_king_war_info);

	notify_self_king_war_state();

	// TODO:通知game（目前当一个game来用）
	if (NULL == env::server)
	{
		log_error("env::server null error");
		return;
	}

	proto::server::eg_king_war_state_notify notify;
	notify.set_country_id(m_id);
	notify.set_state(m_country_info.m_king_war_info.m_state);

	notify.set_def_family_id(m_country_info.m_king_war_info.m_family_defensive);
	const family_ptr& p_def_family = family_manager_t::get_family(m_country_info.m_king_war_info.m_family_defensive);
	if (NULL != p_def_family)
	{
		notify.set_def_family_name(p_def_family->get_name());
	}

	notify.set_att_family_id(m_country_info.m_king_war_info.m_family_attack);
	const family_ptr& p_att_family = family_manager_t::get_family(m_country_info.m_king_war_info.m_family_attack);
	if (NULL != p_att_family)
	{
		notify.set_att_family_name(p_att_family->get_name());
	}

	env::server->send_msg_to_all_games(op_cmd::eg_king_war_state_notify, 0, notify);

	KINGWAR_LOG("country[%u] king war prepare!!!!!!!!!!!", m_id);

	// TODO:写日志
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
	m_country_info.m_king_war_info.m_state_end_time = 0;
	//save
	country_data_mgr_t::update_country_king_war_info(m_id, m_country_info.m_king_war_info);

	// TODO:写日志
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

	// TODO:通知game（目前当一个game来用）
	if (NULL == env::server)
	{
		log_error("env::server null error");
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

	notify.set_att_family_id(m_country_info.m_king_war_info.m_family_attack);
	const family_ptr& p_att_family = family_manager_t::get_family(m_country_info.m_king_war_info.m_family_attack);
	if (p_att_family != NULL)
	{
		notify.set_att_family_name(p_att_family->get_name());
	}

	env::server->send_msg_to_all_games(op_cmd::eg_king_war_state_notify, 0, notify);

	KINGWAR_LOG("country[%u] king war start", m_id);

	// TODO:写日志
}

void country_t::end_king_war_fight()
{
	// 状态验证(可能这国家王城战没有符合条件的家族,或者已经提前结束)
	if (m_country_info.m_king_war_info.m_state != proto::common::king_war_state_start)
	{
		log_warn("country[%u] end_king_war_fight error m_state[%u] != proto::common::king_war_state_start", m_id, m_country_info.m_king_war_info.m_state);
		return;
	}

	// TODO:通知game（目前当一个game来用）
	if (NULL == env::server)
	{
		log_error("env::server null error");
		return;
	}

	proto::server::eg_king_war_time_end_notify notify;
	notify.set_country_id(m_id);
	env::server->send_msg_to_all_games(op_cmd::eg_king_war_time_end_notify, 0, notify);

	// TODO:写日志
}

country_t::king_war_apply_ptr country_t::add_king_war_family(uint64_t family_id)
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
	p_apply_family->m_bid_cost = 0;
	p_apply_family->m_time = common::time_util_t::now_time();

	m_king_war_apply_map.insert(std::make_pair(family_id, p_apply_family));

	country_data_mgr_t::update_king_war_apply_cost(family_id, p_apply_family);

	return p_apply_family;
}

void country_t::notify_self_king_war_state()
{
	uint32_t time_now = common::time_util_t::now_time();

	proto::client::ec_king_war_notice_notify ntf;
	ntf.set_state(m_country_info.m_king_war_info.m_state);

	if (m_country_info.m_king_war_info.m_state != proto::common::king_war_state_end)
	{
		// 开启的时候显示结束时间
		if (m_country_info.m_king_war_info.m_state == proto::common::king_war_state_start)
		{
			if (m_country_info.m_king_war_info.m_state_end_time > time_now)
			{
				ntf.set_time(m_country_info.m_king_war_info.m_state_end_time - time_now);
			}
			else
			{
				ntf.set_time(0);
			}
		}
		else
		{
			if (m_country_info.m_king_war_info.m_start_time > time_now)
			{
				ntf.set_time(m_country_info.m_king_war_info.m_start_time - time_now);
			}
			else
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

		ntf.set_is_king(m_country_info.m_king_war_info.m_family_defensive == m_country_info.m_king_family_id);
	}
	else
	{
		ntf.set_def_family_id(common::string_util_t::uint64_to_string(m_country_info.m_king_war_info.m_family_defensive));
		ntf.set_att_family_id(common::string_util_t::uint64_to_string(m_country_info.m_king_war_info.m_family_attack));
	}

	global_user_manager_t::send_msg_to_country(m_id, op_cmd::ec_king_war_notice_notify, ntf);

	log_warn("country[%u] ec_king_war_notice_notify state[%u]", m_id, m_country_info.m_king_war_info.m_state);
}

void country_t::ready_enter_scene(global_user_ptr p_user)
{
	if (NULL == p_user)
	{
		log_error("p_user null error");
		return;
	}

	// 如果不在时间不用通知
	if (m_country_info.m_king_war_info.m_state < proto::common::king_war_state_pre)
	{
		return;
	}

	uint32_t time_now = common::time_util_t::now_time();

	proto::client::ec_king_war_notice_notify ntf;
	ntf.set_state(m_country_info.m_king_war_info.m_state);

	// 开启的时候显示结束时间
	if (m_country_info.m_king_war_info.m_state == proto::common::king_war_state_start)
	{
		if (m_country_info.m_king_war_info.m_state_end_time > time_now)
		{
			ntf.set_time(m_country_info.m_king_war_info.m_state_end_time - time_now);
		}
		else
		{
			ntf.set_time(0);
		}
	}
	else
	{
		if (m_country_info.m_king_war_info.m_start_time > time_now)
		{
			ntf.set_time(m_country_info.m_king_war_info.m_start_time - time_now);
		}
		else
		{
			ntf.set_time(0);
		}
	}

	ntf.set_def_family_id(common::string_util_t::uint64_to_string(m_country_info.m_king_war_info.m_family_defensive));
	const family_ptr& p_def_family = family_manager_t::get_family(m_country_info.m_king_war_info.m_family_defensive);
	if (NULL != p_def_family)
	{
		ntf.set_defensive_name(p_def_family->get_name());
	}

	ntf.set_att_family_id(common::string_util_t::uint64_to_string(m_country_info.m_king_war_info.m_family_attack));
	const family_ptr& p_att_family = family_manager_t::get_family(m_country_info.m_king_war_info.m_family_attack);
	if (NULL != p_att_family)
	{
		ntf.set_attack_name(p_att_family->get_name());
	}

	ntf.set_is_king(m_country_info.m_king_war_info.m_family_defensive == m_country_info.m_king_family_id);

	p_user->send_msg_to_client(op_cmd::ec_king_war_notice_notify, ntf);
}

void country_t::set_king_war_result(uint64_t win_family_id)
{
	if (m_country_info.m_king_war_info.m_state != proto::common::king_war_state_start)
	{
		log_warn("country[%u] set_king_war_result error m_state[%u] != proto::common::king_war_state_start", m_id, m_country_info.m_king_war_info.m_state);
		return;
	}

	if (win_family_id != m_country_info.m_king_war_info.m_family_defensive && win_family_id != m_country_info.m_king_war_info.m_family_attack)
	{
		log_warn("country[%u] win_family[%lu] not family_defensive[%lu] and not family_attack[%lu]", m_id, m_country_info.m_king_war_info.m_family_defensive, m_country_info.m_king_war_info.m_family_attack);
		
		king_war_end(false, 0);
	}
	else
	{
		king_war_end(true, win_family_id);
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

void country_t::del_family(const family_ptr& p_family)
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

	// 是国王家族的处理
	if (uid == m_country_info.m_king_family_id) {
		reset_king_family();
	}

	m_family_force_vec.erase(itr);
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
	country_data_mgr_t::save_country_officer(m_id, itr->first, itr->second);

	if (p_data) p_data->CopyFrom(itr->second);

	WRITE_COUNTRY_LOG_TO_DB(m_id, common::log_enum::source_type_country_resign_officer, p_user->get_uid(), itr->first, 0);
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

void country_t::update_officer_attr(uint32_t officer_type, const global_user_ptr& p_user)
{
	if (NULL == p_user) {
		log_error("p_user == null");
		return;
	}

	std::map<uint32_t, proto::common::country_officer_data>::iterator itr = m_country_info.officers.find(officer_type);
	if (itr != m_country_info.officers.end()) {
		uint64_t officer_uid = common::string_util_t::string_to_uint64(itr->second.uid());
		if (officer_uid != p_user->get_uid()) {
			log_error("country[%u] officer[%u] uid[%lu] != %lu", m_id, officer_type, officer_uid, p_user->get_uid());
			return;
		}

		itr->second.set_plugin(p_user->get_hero_plugin());
		itr->second.set_level(p_user->get_level());
		itr->second.set_fighting(p_user->get_fighting_value());
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

