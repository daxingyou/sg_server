#include "family.hpp"
#include "redis_client.hpp"
#include "main/center_server_fwd.hpp"
#include "config/config_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "redis/family_data_mgr.hpp"
#include "redis/global_user_data_mgr.hpp"
#include "tblh/errcode_enum.hpp"
#include "user/global_user_manager.hpp"
#include "red_point/red_point_manager.hpp"
#include "tblh/Family.tbls.h"
#include "rank/rank_manager.hpp"
#include "tblh/rank_common.hpp"
#include "log/log_wrapper.hpp"
#include "achieve/achieve_common.hpp"
#include "sys_notice/sys_notice_manager.hpp"
#include "offline/offline_role_manager.hpp"
#include "tblh/TowerTrigger.tbls.h"
#include "country/country_mgr.hpp"
#include "country/country_common.hpp"
#include "common/time_manager.hpp"
#include "tblh/time_type_enum.hpp"
#include "tblh/ShopItemTable.tbls.h"

#define WRITE_FAMILY_LOG_TO_DB(family_uid, role_uid, target_uid, type, param) \
    log_wrapper_t::send_family_log(family_uid, role_uid, target_uid, type, param);

family::family(uint64_t family_id)
	: m_family_id(family_id)
	, m_family_shop(family_id)
{
	m_data.set_id(common::string_util_t::uint64_to_string(m_family_id));
}

family::~family()
{

}

bool family::init(const global_user_ptr& p_user, const std::string& family_name, const std::string& declaration)
{
	if (NULL == p_user)
	{
		log_error("NULL == p_user");
		return false;
	}

	m_data.set_name(family_name);

	// 家族建筑优先初始化
	for (uint32_t i = proto::common::family_build_type_MIN; i < proto::common::family_build_type_ARRAYSIZE; ++i)
	{
		family_build_ptr p_family_build(new family_build((proto::common::family_build_type)i, m_family_id));
		if (NULL == p_family_build)
		{
			log_error("NULL == p_family_build");
			return false;
		}

		p_family_build->init();

		m_build_map.insert(family::family_build_map::value_type((proto::common::family_build_type)i, p_family_build));
	}

	// 家族等级等于议事厅等级
	if (NULL != m_build_map[proto::common::family_build_type_congress])
	{
		m_data.set_level(m_build_map[proto::common::family_build_type_congress]->get_level());
	}
	else
	{
		m_data.set_level(1);
	}

	m_create_time = common::time_util_t::now_time();

	// 更新和家族建筑等级相关的数据
	update_member_size();
	update_max_money();

	m_data.set_patriarch(common::string_util_t::uint64_to_string(p_user->get_uid()));
	m_data.set_creator(p_user->get_name());

	// 初始资金
	auto conf = GET_CONF(Comprehensive, comprehensive_common::family_initial_gold);
	uint32_t family_money = GET_COMPREHENSIVE_VALUE_1(conf);
	m_data.set_money(family_money);
	m_data.set_declaration(declaration);

	// 默认公告
	conf = GET_CONF(Comprehensive, comprehensive_common::family_announce);
	if (conf && !conf->parameter1().empty())
	{
		std::string notification = common::string_util_t::convert_to_utf8(conf->parameter1());
		m_data.set_notice(notification);
	}
	else
	{
		m_data.set_notice("");
	}

	m_data.set_prosperity(0);
	m_data.set_prestige(0);
	m_data.set_territory(0);
	m_data.set_auto_newbie(true);

	// 初始化家族维护资金
	uint32_t cost = get_normal_maintain_cost();

	// 家族资金少于正常消耗进入低维护状态
	if (get_money() < cost)
	{
		// 计算低维护的钱
		auto conf = GET_CONF(Comprehensive, comprehensive_common::family_maint_para4);
		uint32_t param4 = GET_COMPREHENSIVE_VALUE_1(conf);
		cost = get_level() * param4;

		m_is_low_maintenance = true;

		m_data.set_matain_cost(cost);
	}
	else
	{
		m_is_low_maintenance = false;

		m_data.set_matain_cost(cost);
	}

	// 不用保存，下面一起存
	const family_member_ptr& p_member = member_in(p_user, family_officer_type_general, false);
	if (NULL == p_member)
	{
		log_error("family[%lu] user[%lu] member in error", m_family_id, p_user->get_uid());
		return false;
	}

	// 初始化成功
	// 保存redis
	family_data_mgr_t::save_all(shared_from_this());

	return true;
}

family_member_ptr family::member_in(const global_user_ptr& p_user, uint32_t officer /*= family_officer_type_newbie*/, bool is_save /*= true*/)
{
	if (NULL == p_user)
	{
		log_error("NULL == p_user");
		return NULL;
	}

	if (officer == family_officer_type_none || officer > family_officer_type_newbie)
	{
		log_error("user[%lu] officer[%u] type error", p_user->get_uid(), officer);
		return NULL;
	}

	family_member_ptr p_family_member(new family_member(p_user->get_uid(), m_family_id));
	if (NULL == p_family_member)
	{
		log_error("NULL == p_family_member");
		return NULL;
	}

	if (!p_family_member->init(p_user, officer))
	{
		log_error("user[%lu] family member init error", p_user->get_uid());
		return NULL;
	}

	std::pair<family::family_member_map::iterator, bool> ret = m_member_map.insert(family::family_member_map::value_type(p_user->get_uid(), p_family_member));
	if (!ret.second)
	{
		log_error("family[%lu] member[%lu] insert error, it is exist", m_family_id, p_user->get_uid());
		return NULL;
	}

	// 分类
	m_officer_map[officer].insert(p_user->get_uid());

	if (is_save)
	{
		// 保存redis
		family_data_mgr_t::save_one_member(p_family_member);
	}
	achieve_common_t::notify_progress_state(p_user->get_uid(), proto::common::ACHIEVE_FAMILY, officer);
	return p_family_member;
}

family_member_ptr family::member_in(uint64_t uid, uint32_t officer /*= family_officer_type_newbie*/)
{
	if (!global_user_data_mgr_t::is_user_key_exsit(uid))
	{
		log_error("user[%lu] not exsit", uid);
		return NULL;
	}

	if (officer == family_officer_type_none || officer > family_officer_type_newbie)
	{
		log_error("user[%lu] officer[%u] type error", uid, officer);
		return NULL;
	}

	family_member_ptr p_family_member(new family_member(uid, m_family_id));
	if (NULL == p_family_member)
	{
		log_error("NULL == p_family_member");
		return NULL;
	}

	if (!p_family_member->init(officer))
	{
		log_error("user[%lu] family member init error", uid);
		return NULL;
	}

	std::pair<family::family_member_map::iterator, bool> ret = m_member_map.insert(family::family_member_map::value_type(uid, p_family_member));
	if (!ret.second)
	{
		log_error("family[%lu] member[%lu] insert error, it is exist", m_family_id, uid);
		return NULL;
	}

	// 分类
	m_officer_map[officer].insert(uid);

	// 保存redis
	family_data_mgr_t::save_one_member(p_family_member);
	achieve_common_t::notify_progress_state(uid, proto::common::ACHIEVE_FAMILY, officer);
	return p_family_member;
}

bool family::member_out(uint64_t uid)
{
	family_member_map::iterator itr = m_member_map.find(uid);
	if (itr == m_member_map.end())
	{
		log_error("family[%lu] have no member[%lu]", m_family_id, uid);
		return false;
	}

	const family_member_ptr& p_family_member = itr->second;
	if (NULL == p_family_member)
	{
		log_error("NULL == p_family_member");
		return false;
	}
	update_member_glory(uid, 0);

	uint32_t officer = p_family_member->get_officer();

	// 管理器中删除
	m_officer_map[officer].erase(uid);
	m_member_map.erase(itr);

	// redis 删除
	family_data_mgr_t::del_family_member(uid);
	BROADCAST_SYS_NOTICE_SYS_CHANNEL(uid, proto::common::sys_notify_type_leave_family, 0, get_name().c_str());
	return true;
}

family_member_ptr family::get_family_member(uint64_t uid)
{
	family_member_map::const_iterator citr = m_member_map.find(uid);
	if (citr != m_member_map.end())
	{
		return citr->second;
	}

	return NULL;
}

family_build_ptr family::get_family_build(proto::common::family_build_type type)
{
	family_build_map::const_iterator citr = m_build_map.find(type);
	if (citr != m_build_map.end())
	{
		return citr->second;
	}

	return NULL;
}

void family::add_money(uint32_t val, uint32_t type, uint64_t uid)
{
	uint32_t family_money = m_data.money();
	family_money += val;
	m_data.set_money(family_money);

	family_data_mgr_t::update_family_money(m_family_id, family_money);

	proto::client::ec_family_money_change_notify ntf;
	ntf.set_money(family_money);
	send_msg_to_family(op_cmd::ec_family_money_change_notify, ntf);

	log_wrapper_t::send_family_log(m_family_id, uid, 0, common::log_enum::source_type_family_money_add, type, family_money);
}

void family::cost_money(uint32_t val, uint32_t type, uint64_t uid)
{
	uint32_t family_money = m_data.money();

	if (family_money >= val )
	{
		family_money -= val;
	}
	else
	{
		family_money = 0;
	}

	m_data.set_money(family_money);

	family_data_mgr_t::update_family_money(m_family_id, family_money);

	proto::client::ec_family_money_change_notify ntf;
	ntf.set_money(family_money);
	send_msg_to_family(op_cmd::ec_family_money_change_notify, ntf);

	log_wrapper_t::send_family_log(m_family_id, uid, 0, common::log_enum::source_type_family_money_cost, type, family_money);
}

void family::set_maintain(uint32_t cost, bool is_low)
{
	m_is_low_maintenance = is_low;

	m_data.set_matain_cost(cost);

	family_data_mgr_t::update_family_maintain(m_family_id, cost, is_low);
}

void family::update_member_size()
{
	uint32_t level = 0;
	if (NULL != m_build_map[proto::common::family_build_type_house])
	{
		level = m_build_map[proto::common::family_build_type_house]->get_level();
	}

	// 更新最大成员数
	{
		auto conf = GET_CONF(Comprehensive, comprehensive_common::family_mem_cnt);
		uint32_t initial_cnt = GET_COMPREHENSIVE_VALUE_1(conf);

		conf = GET_CONF(Comprehensive, comprehensive_common::family_mem_add);
		uint32_t add_cnt = GET_COMPREHENSIVE_VALUE_1(conf);

		m_data.set_member_cnt_max(initial_cnt + add_cnt * level);
	}

	// 更新和最大成员数相关的加成
	{
		// 最大成员数
		uint32_t max_member_cnt = get_max_member_size();

		auto conf = GET_CONF(Comprehensive, comprehensive_common::family_memberactive3);
		uint32_t pre_online = GET_COMPREHENSIVE_VALUE_1(conf);
		m_online_prosperity_daily_max = max_member_cnt * pre_online / 10000;

		conf = GET_CONF(Comprehensive, comprehensive_common::family_memberactive6);
		uint32_t pre_chat = GET_COMPREHENSIVE_VALUE_1(conf);
		m_chat_prosperity_daily_max = max_member_cnt * pre_chat * 5 / 10000;

		conf = GET_CONF(Comprehensive, comprehensive_common::family_memberactive9);
		uint32_t pre_task = GET_COMPREHENSIVE_VALUE_1(conf);
		m_task_prosperity_daily_max = max_member_cnt * pre_task * 15 / 10000;
	}

	// 更新最大学徒数量
	{
		auto conf = GET_CONF(Comprehensive, comprehensive_common::family_new_cnt);
		uint32_t initial_cnt = GET_COMPREHENSIVE_VALUE_1(conf);

		conf = GET_CONF(Comprehensive, comprehensive_common::family_new_add);
		uint32_t add_cnt = GET_COMPREHENSIVE_VALUE_1(conf);

		m_newbie_cnt_max = initial_cnt + add_cnt * level;
	}
}

void family::update_maintain_cost()
{
	uint32_t cost = get_normal_maintain_cost();

	// 家族资金少于正常消耗进入低维护状态
	if (get_money() < cost)
	{
		// 计算低维护的钱
		auto conf = GET_CONF(Comprehensive, comprehensive_common::family_maint_para4);
		uint32_t param4 = GET_COMPREHENSIVE_VALUE_1(conf);
		cost = get_level() * param4;

		cost_money(cost, proto::server::family_money_change_maintain_cost);

		set_maintain(cost, true);
	}
	else
	{
		cost_money(cost, proto::server::family_money_change_maintain_cost);

		set_maintain(cost, false);
	}
}

uint32_t family::get_normal_maintain_cost()
{
	//	每天维护费用 = 系数1 *（家族等级* 系数2 + （民居等级 + 计略府等级 + 仓库等级）*系数3）
	auto conf = GET_CONF(Comprehensive, comprehensive_common::family_maint_para1);
	uint32_t param1 = GET_COMPREHENSIVE_VALUE_1(conf);
	conf = GET_CONF(Comprehensive, comprehensive_common::family_maint_para2);
	uint32_t param2 = GET_COMPREHENSIVE_VALUE_1(conf);
	conf = GET_CONF(Comprehensive, comprehensive_common::family_maint_para3);
	uint32_t param3 = GET_COMPREHENSIVE_VALUE_1(conf);

	uint32_t level = 0;
	if (NULL != m_build_map[proto::common::family_build_type_house])
	{
		level += m_build_map[proto::common::family_build_type_house]->get_level();
	}

	if (NULL != m_build_map[proto::common::family_build_type_ploy])
	{
		level += m_build_map[proto::common::family_build_type_ploy]->get_level();
	}

	if (NULL != m_build_map[proto::common::family_build_type_barn])
	{
		level += m_build_map[proto::common::family_build_type_barn]->get_level();
	}

	return param1 * (get_level() * param2 + (level * param3));
}

void family::update_max_money()
{
	uint32_t level = 0;
	if (NULL != m_build_map[proto::common::family_build_type_barn])
	{
		level = m_build_map[proto::common::family_build_type_barn]->get_level();
	}

	auto conf1 = GET_CONF(Comprehensive, comprehensive_common::family_found_cnt);
	uint32_t initial_cnt = GET_COMPREHENSIVE_VALUE_1(conf1);
	auto conf2 = GET_CONF(Comprehensive, comprehensive_common::family_found_add);
	uint32_t increment = GET_COMPREHENSIVE_VALUE_1(conf2);

	m_data.set_max_money(initial_cnt + level * increment);
}

void family::update_prosperity()
{
	m_data.set_prosperity(m_online_prosperity_add_daily + m_chat_prosperity_add_daily + m_task_prosperity_add_daily);

	m_online_prosperity_add_daily = 0;
	m_chat_prosperity_add_daily = 0;
	m_task_prosperity_add_daily = 0;

	family_data_mgr_t::update_family_prosperity(m_family_id, m_data.prosperity());
}

void family::update_prestige_daily()
{
	if (is_king_family())
	{
		auto conf = GET_CONF(Comprehensive, comprehensive_common::family_prestige1);
		m_prestige_add_daily += GET_COMPREHENSIVE_VALUE_1(conf);
	}
	else
	{
		auto conf = GET_CONF(Comprehensive, comprehensive_common::family_prestige3);
		m_prestige_add_daily += GET_COMPREHENSIVE_VALUE_1(conf);
	}

	uint32_t newprestige = m_data.prestige() + m_prestige_add_daily;

	auto conf = GET_CONF(Comprehensive, comprehensive_common::family_prestige4);
	uint32_t prestige_dec_day = GET_COMPREHENSIVE_VALUE_1(conf);

	// 每周递减日期
	if (common::time_util_t::get_weekday() == prestige_dec_day)
	{
		auto conf = GET_CONF(Comprehensive, comprehensive_common::family_prestige5);
		uint32_t prestige_dec_ratio = GET_COMPREHENSIVE_VALUE_1(conf);

		newprestige -= newprestige * prestige_dec_ratio / 100;
	}

	m_data.set_prestige(newprestige);

	m_prestige_add_daily = 0;

	family_data_mgr_t::update_family_prestige(m_family_id, newprestige);
}

void family::update_impeacher()
{
	// TODO:
}

bool family::is_auto_newbie(uint32_t level)
{
	// 能否自动加入新军
	if (!m_data.auto_newbie())
	{
		return false;
	}

	// 等级
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_to_new_lv);
	uint32_t auto_newbie_level = GET_COMPREHENSIVE_VALUE_1(p_conf);
	if (level >= auto_newbie_level)
	{
		return false;
	}

	// 新军已经满
	if (get_officer_count(family_officer_type_newbie) >= get_max_newbie_size())
	{
		return false;
	}

	return true;
}

bool family::add_appliy_list(uint64_t uid)
{
	family_apply_map::iterator itr = m_apply_map.find(uid);

	// 对于在申请列表中的人更新时间
	if (itr != m_apply_map.end())
	{
		itr->second = common::time_util_t::now_time();
		family_data_mgr_t::save_apply_list(shared_from_this());
		return true;
	}
	
	// 验证申请列表是否满
	if (m_apply_map.size() >= family_manager_t::get_max_apply_size())
	{
		return false;
	}

	m_apply_map[uid] = common::time_util_t::now_time();
	family_data_mgr_t::save_apply_list(shared_from_this());

	// 通知官员
	for (uint32_t i = family_officer_type_general; i < family_officer_type_member; ++i)
	{
		// 是否有权限
		if (family_manager_t::has_family_privilige(i, privilige_type_family_member_audit))
		{
			officer_map::const_iterator citr = m_officer_map.find(i);

			if (citr != m_officer_map.end())
			{
				for (const auto& uid : citr->second)
				{
					red_point_manager_t::red_point_notify(uid, proto::common::RED_POINT_TYPE::RED_POINT_FAMILY_APPLY_LIST);
				}
			}
		}
	}

	return true;
}

uint32_t family::get_officer_count(uint32_t officer)
{
	officer_map::const_iterator citr = m_officer_map.find(officer);
	if (citr != m_officer_map.end())
	{
		return citr->second.size();
	}

	return 0;
}

bool family::decline_apply_user(uint64_t uid)
{
	family_apply_map::iterator itr = m_apply_map.find(uid);

	// 没在列表中
	if (itr == m_apply_map.end())
	{
		return false;
	}

	m_apply_map.erase(itr);

	// save
	family_data_mgr_t::save_apply_list(shared_from_this());

	return true;
}

void family::decline_all_apply_user()
{
	m_apply_map.clear();
	// save
	family_data_mgr_t::save_apply_list(shared_from_this());
}

uint32_t family::approve_join(uint64_t master_uid, uint64_t uid, bool is_save/* = true*/)
{
	family_apply_map::iterator itr = m_apply_map.find(uid);

	// 没在列表中
	if (itr == m_apply_map.end())
	{
		log_error("family[%lu] apply list have no user[%lu]", m_family_id, uid);
		return common::errcode_enum::notice_unknown;
	}

	// 已经加入了其他家族
	uint32_t is_add_other = global_user_data_mgr_t::get_user_family_id(uid);
	if (is_add_other != 0)
	{
		m_apply_map.erase(itr);
		
		// 是否立即保存
		if (is_save)
		{
			family_data_mgr_t::save_apply_list(shared_from_this());
		}

		return common::errcode_enum::error_ok;
	}

	if (m_member_map.size() >= get_max_member_size())
	{
		FAMILY_LOG("family[%lu] is max_member_size", m_family_id);
		return common::errcode_enum::family_err_code_excess_max_member_number;
	}

	uint32_t officer = family_officer_type_none;
	// 等级
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_to_new_lv);
	uint32_t auto_newbie_level = GET_COMPREHENSIVE_VALUE_1(p_conf);
	if (global_user_data_mgr_t::get_user_level(uid) < auto_newbie_level)
	{
		// 新军已经满
		if (get_officer_count(family_officer_type_newbie) >= get_max_newbie_size())
		{
			FAMILY_LOG("family[%lu] is max newbie member size", m_family_id);
			return common::errcode_enum::family_err_code_excess_max_newbie_number;
		}

		officer = family_officer_type_newbie;
	}
	else
	{
		officer = family_officer_type_member;
	}

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL != p_user)
	{
		if (!join_in(p_user, officer))
		{
			return errcode_enum::notice_unknown;
		}
	}
	else
	{
		if (!join_in(uid, officer))
		{
			return errcode_enum::notice_unknown;
		}
	}

	// 删除
	m_apply_map.erase(itr);

	// 是否立即保存
	if (is_save)
	{
		family_data_mgr_t::save_apply_list(shared_from_this());
	}

	WRITE_FAMILY_LOG_TO_DB(m_family_id, master_uid, uid, common::log_enum::source_type_family_approve_join, officer);

	return common::errcode_enum::error_ok;
}

bool family::join_in(uint64_t uid, uint32_t officer)
{
	const family_member_ptr& p_new_member = member_in(uid, officer);
	if (NULL == p_new_member)
	{
		log_error("family[%lu] user[%lu] member in error", m_family_id, uid);
		return false;
	}

	// 存redis
	global_user_data_mgr_t::set_user_family_id(uid, m_family_id);
	global_user_data_mgr_t::set_user_family_officer(uid, officer);

	global_user_data_mgr_t::set_user_country_id(uid, m_data.country());
	global_user_data_mgr_t::set_user_country_officer(uid, 0);

	proto::client::ec_family_members_in_notify ntf_family;
	ntf_family.set_member(p_new_member->get_name());
	send_msg_to_family(op_cmd::ec_family_members_in_notify, ntf_family);
	BROADCAST_SYS_NOTICE_SYS_CHANNEL(uid, proto::common::sys_notify_type_join_family,0, get_name().c_str());
	return true;
}

bool family::join_in(const global_user_ptr& p_user, uint32_t officer)
{
	if (NULL == p_user)
	{
		log_error("NULL == p_user");
		return false;
	}

	const family_member_ptr& p_new_member = member_in(p_user, officer);
	if (NULL == p_new_member)
	{
		log_error("family[%lu] user[%lu] member in error", m_family_id, p_user->get_uid());
		return false;
	}

	// 加入家族成功
	p_user->join_family(m_family_id, officer, m_data.country());

	// 更新到逻辑服
	proto::server::eg_update_family_info_notify ntf;
	ntf.set_id(m_family_id);
	ntf.set_name(m_data.name());
	ntf.set_officer(officer);
	ntf.set_country_id(m_data.country());
	p_user->send_msg_to_game(op_cmd::eg_update_family_info_notify, ntf);

	proto::client::ec_family_id_notify ntf_client;
	ntf_client.set_family_id(string_util_t::uint64_to_string(m_family_id));
	ntf_client.set_family_name(m_data.name());
	p_user->send_msg_to_client(op_cmd::ec_family_id_notify, ntf_client);

	family_manager_t::notify_chat_member_change(m_family_id, p_user->get_uid(), 1);

	proto::client::ec_family_members_in_notify ntf_family;
	ntf_family.set_member(p_user->get_name());
	send_msg_to_family(op_cmd::ec_family_members_in_notify, ntf_family);
	BROADCAST_SYS_NOTICE_SYS_CHANNEL(p_user->get_uid(), proto::common::sys_notify_type_join_family, 0, get_name().c_str());
	return true;
}

bool family::leave_out(const global_user_ptr& p_user)
{
	if (NULL == p_user)
	{
		log_error("NULL == p_user");
		return false;
	}

	if (!member_out(p_user->get_uid()))
	{
		log_error("user[%lu] leave family[%lu] failed", p_user->get_uid(), m_family_id);
		return false;
	}

	uint32_t old_country_officer = p_user->get_country_officer();

	// 离开家族成功
	p_user->leave_family();

	if (old_country_officer != 0) {
		country_ptr p_country = country_mgr_t::get_country(m_data.country());
		if (NULL != p_country) {
			p_country->del_officer(old_country_officer, p_user->get_uid());
		}
	}

	// 更新到逻辑服
	proto::server::eg_update_family_info_notify ntf;
	ntf.set_id(0);
	p_user->send_msg_to_game(op_cmd::eg_update_family_info_notify, ntf);

	family_manager_t::notify_chat_member_change(m_family_id, p_user->get_uid(), 0);

	proto::client::ec_family_members_out_notify ntf_family;
	ntf_family.set_member(p_user->get_name());
	send_msg_to_family(op_cmd::ec_family_members_out_notify, ntf_family);

	return true;
}

bool family::leave_out(uint64_t uid)
{
	if (!member_out(uid))
	{
		log_error("user[%lu] leave family[%lu] failed", uid, m_family_id);
		return false;
	}

	uint32_t old_country_officer = global_user_data_mgr_t::get_user_country_officer(uid);
	if (old_country_officer != 0) {
		country_ptr p_country = country_mgr_t::get_country(m_data.country());
		if (NULL != p_country) {
			p_country->del_officer(old_country_officer, uid);
		}
	}

	// 存redis
	global_user_data_mgr_t::set_user_family_id(uid, 0);
	global_user_data_mgr_t::set_user_family_officer(uid, 0);

	global_user_data_mgr_t::set_user_country_id(uid, 0);
	global_user_data_mgr_t::set_user_country_officer(uid, 0);

	proto::client::ec_family_members_out_notify ntf_family;
	ntf_family.set_member(global_user_data_mgr_t::get_name(uid));
	send_msg_to_family(op_cmd::ec_family_members_out_notify, ntf_family);

	return true;
}

void family::del_from_apply_list(uint64_t uid)
{
	family_apply_map::iterator itr = m_apply_map.find(uid);
	if (itr != m_apply_map.end())
	{
		m_apply_map.erase(itr);
		family_data_mgr_t::save_apply_list(shared_from_this());
	}
}

uint32_t family::approve_all_apply_user(uint64_t master_uid, proto::client::ec_one_key_approve_join_family_reply& reply)
{
	// 申请列表空的
	if (m_apply_map.empty())
	{
		log_error("family[%lu] apply list empty", m_family_id);
		return common::errcode_enum::notice_unknown;
	}

	// 先按时间排序
	std::multimap<uint32_t, uint64_t> apply_multimap;
	for (family_apply_map::const_iterator citr = m_apply_map.begin();
		citr != m_apply_map.end(); ++citr)
	{
		apply_multimap.insert(std::make_pair(citr->second, citr->first));
	}

	uint32_t back_code = common::errcode_enum::error_ok;

	for (std::multimap<uint32_t, uint64_t>::const_iterator citr = apply_multimap.begin();
		citr != apply_multimap.end(); ++citr)
	{
		back_code = approve_join(master_uid, citr->second, false);

		// 失败了就跳出循环
		if (back_code != common::errcode_enum::error_ok)
		{
			break;
		}

		reply.add_all_uids(common::string_util_t::uint64_to_string(citr->second));
	}

	// 延后存一下
	family_data_mgr_t::save_apply_list(shared_from_this());

	return back_code;
}

void family::auto_newbie_member()
{
	officer_map::iterator itr = m_officer_map.find(family_officer_type_newbie);
	if (itr == m_officer_map.end())
	{
		// 没有学徒
		return;
	}

	if (itr->second.empty())
	{
		// 列表中没学徒
		return;
	}

	// 等级
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_to_mem_lv);
	uint32_t auto_member_level = GET_COMPREHENSIVE_VALUE_1(p_conf);

	// 贡献
	p_conf = GET_CONF(Comprehensive, comprehensive_common::family_to_mem_ctrb);
	uint32_t auto_member_ctrb = GET_COMPREHENSIVE_VALUE_1(p_conf);

	// 加入时间
	p_conf = GET_CONF(Comprehensive, comprehensive_common::family_to_mem_exp);
	uint32_t auto_member_exp = GET_COMPREHENSIVE_VALUE_1(p_conf);

	uint32_t time_now = common::time_util_t::now_time();

	for (officer_vec::iterator off_itr = itr->second.begin();
		off_itr != itr->second.end();)
	{
		uint64_t uid = *off_itr;

		const family_member_ptr& p_family_member = get_family_member(uid);

		if (NULL != p_family_member &&
			p_family_member->get_level() >= auto_member_level &&
			p_family_member->get_total_contribution() >= auto_member_ctrb &&
			time_now >= p_family_member->get_join_time() + auto_member_exp)
		{
			// 更新职位
			p_family_member->update_officer(family_officer_type_member);
			m_officer_map[family_officer_type_member].insert(uid);

			// 老的删除
			off_itr = itr->second.erase(off_itr);
		}
		else
		{
			++off_itr;
		}
	}
}

void family::update_member_online_state(const global_user_ptr& p_user)
{
	if (NULL == p_user)
	{
		log_error("NULL == p_user");
		return;
	}

	const family_member_ptr& p_family_member = get_family_member(p_user->get_uid());
	if (NULL == p_family_member)
	{
		log_error("p_user[%lu] not member in family[%lu]", p_user->get_uid(), m_family_id);
		return;
	}

	if (p_user->is_online())
	{
		p_family_member->set_online();
	}
	else
	{
		p_family_member->set_offline(p_user->get_last_logout_time());
	}
}

void family::update_member_attr(const global_user_ptr& p_user)
{
	if (NULL == p_user)
	{
		log_error("NULL == p_user");
		return;
	}

	const family_member_ptr& p_family_member = get_family_member(p_user->get_uid());
	if (NULL == p_family_member)
	{
		log_error("p_user[%lu] not member in family[%lu]", p_user->get_uid(), m_family_id);
		return;
	}

	p_family_member->update_attr(p_user);
}

uint32_t family::assign_member(uint64_t master_id, uint64_t uid, uint32_t officer)
{
	const family_member_ptr& p_family_member = get_family_member(uid);
	if (NULL == p_family_member)
	{
		log_error("family[%lu] have no member[%lu]", m_family_id, uid);
		return common::errcode_enum::notice_unknown;
	}

	// 从老的删除
	m_officer_map[p_family_member->get_officer()].erase(uid);
	// 加入新的管理器中
	m_officer_map[officer].insert(uid);

	// 更新职位
	p_family_member->update_officer(officer);

	// 职位变动，通知家族
	proto::client::ec_family_officer_changed_notify ntf;
	ntf.set_role_id(common::string_util_t::uint64_to_string(uid));
	ntf.set_officer(officer);
	send_msg_to_family(op_cmd::ec_family_officer_changed_notify, ntf);

	FAMILY_LOG("family[%lu] user[%lu] new office[%u]", m_family_id, uid, officer);

	WRITE_FAMILY_LOG_TO_DB(m_family_id, master_id, uid, common::log_enum::source_type_family_assign, officer);

	return common::errcode_enum::error_ok;
}

void family::peek_data(proto::common::family_list_info* p_data)
{
	if (NULL != p_data)
	{
		proto::common::family_data* p_base = p_data->mutable_base();
		if (NULL != p_base)
		{
			p_base->CopyFrom(m_data);
		}

		p_data->set_member_cnt(m_member_map.size());
		p_data->set_patriarch_uid(m_data.patriarch());
		p_data->set_patriarch_name(global_user_data_mgr_t::get_name(common::string_util_t::string_to_uint64(m_data.patriarch())));
	}
}

void family::get_family_info(proto::client::ec_get_family_info_reply& reply)
{
	// 家族信息
	proto::common::family_data* p_base = reply.mutable_data();
	if (NULL != p_base)
	{
		p_base->CopyFrom(m_data);
	}

	// 成员信息
	for (family_member_map::const_iterator citr = m_member_map.begin();
		citr != m_member_map.end(); ++citr)
	{
		if (citr->second != NULL)
		{
			citr->second->peek_data(reply.add_members());
		}
	}
}

void family::get_family_member_list(proto::client::ec_get_family_member_list_reply& reply)
{
	// 成员信息
	for (family_member_map::const_iterator citr = m_member_map.begin();
		citr != m_member_map.end(); ++citr)
	{
		if (citr->second != NULL)
		{
			citr->second->peek_data(reply.add_members());
		}
	}
}

void family::get_family_member_list(proto::client::ec_open_appoint_reply& reply)
{
	// 成员信息
	for (family_member_map::const_iterator citr = m_member_map.begin();
		citr != m_member_map.end(); ++citr)
	{
		if (citr->second != NULL)
		{
			citr->second->peek_data(reply.add_members());
		}
	}
}

void family::toggle_aoto_newbie()
{
	m_data.set_auto_newbie(!m_data.auto_newbie());

	family_data_mgr_t::update_family_auto_newbie(m_family_id, m_data.auto_newbie());
}

const std::string& family::update_family_declaration(const std::string& str)
{
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_declaration_cnt);
	uint32_t max_cnt = GET_COMPREHENSIVE_VALUE_1(p_conf);

	if (common::string_util_t::get_character_cnt(str) > max_cnt)
	{
		m_data.set_declaration(common::string_util_t::substr(str, 0, max_cnt));
	}
	else
	{
		m_data.set_declaration(str);
	}

	family_data_mgr_t::update_family_declaration(m_family_id, m_data.declaration());

	proto::client::ec_family_declaration_change_notify ntf_family;
	ntf_family.set_declaration(m_data.declaration());
	send_msg_to_family(op_cmd::ec_family_declaration_change_notify, ntf_family);

	return m_data.declaration();
}

const std::string& family::update_family_notification(const std::string& str)
{
	auto p_conf = GET_CONF(Comprehensive, comprehensive_common::family_announce_cnt);
	uint32_t max_cnt = GET_COMPREHENSIVE_VALUE_1(p_conf);

	if (common::string_util_t::get_character_cnt(str) > max_cnt)
	{
		m_data.set_notice(common::string_util_t::substr(str, 0, max_cnt));
	}
	else
	{
		m_data.set_notice(str);
	}

	family_data_mgr_t::update_family_notification(m_family_id, m_data.notice());

	proto::client::ec_family_notification_change_notify ntf_family;
	ntf_family.set_notification(m_data.notice());
	send_msg_to_family(op_cmd::ec_family_notification_change_notify, ntf_family);

	return m_data.notice();
}

void family::get_family_apply_list(proto::client::ec_family_apply_list_reply& reply)
{
	for (family_apply_map::const_iterator citr = m_apply_map.begin();
		citr != m_apply_map.end(); ++citr)
	{
		proto::common::family_apply_member* p_apply_member = reply.add_members();
		if (NULL != p_apply_member)
		{
			p_apply_member->set_role_id(common::string_util_t::uint64_to_string(citr->first));
			p_apply_member->set_role_name(global_user_data_mgr_t::get_name(citr->first));
			p_apply_member->set_level(global_user_data_mgr_t::get_user_level(citr->first));
			p_apply_member->set_fighting(global_user_data_mgr_t::get_user_fighting_value(citr->first));
			p_apply_member->set_plugin(global_user_data_mgr_t::get_user_hero_plugin(citr->first));
			p_apply_member->set_apply_time(citr->second);
		}
	}
}

void family::get_family_build_list(proto::client::ec_get_family_build_info_reply& reply)
{
	for (family_build_map::const_iterator citr = m_build_map.begin();
		citr != m_build_map.end(); ++citr)
	{
		if (NULL != citr->second)
		{
			citr->second->peek_data(reply.add_builds());
		}
	}
}

uint32_t family::upgrade_build(proto::common::family_build_type type, proto::client::ec_upgrade_family_build_reply& reply)
{
	const family_build_ptr& p_family_build = get_family_build(type);
	if (NULL == p_family_build)
	{
		log_error("family[%lu] build[%u] not find", m_family_id, type);
		return common::errcode_enum::notice_unknown;
	}

	uint32_t m_level = p_family_build->get_level();

	if (m_level >= FAMILY_BUILD_MAX_LEVEL)
	{
		log_error("family[%lu] build[%u] level[%u] max level", m_family_id, type, m_level);
		return errcode_enum::family_err_code_build_max_level;
	}

	if (type == proto::common::family_build_type_congress)
	{
		uint32_t other_build_total = 0;
		if (NULL != m_build_map[proto::common::family_build_type_house])
		{
			other_build_total += m_build_map[proto::common::family_build_type_house]->get_level();
		}

		if (NULL != m_build_map[proto::common::family_build_type_ploy])
		{
			other_build_total += m_build_map[proto::common::family_build_type_ploy]->get_level();
		}

		if (NULL != m_build_map[proto::common::family_build_type_barn])
		{
			other_build_total += m_build_map[proto::common::family_build_type_barn]->get_level();
		}

		if (m_level >= other_build_total)
		{
			log_error("upgrade family build failed cause of congress build level[%u] greater than other builds level total[%u]", m_level, other_build_total);
			return errcode_enum::family_err_code_congress_level_greater_than_others;
		}
	}
	else
	{
		uint32_t congress_build_level = 0;
		if (NULL != m_build_map[proto::common::family_build_type_congress])
		{
			congress_build_level = m_build_map[proto::common::family_build_type_congress]->get_level();
		}

		if (m_level >= congress_build_level)
		{
			log_error("upgrade family build failed cause of level[%u] excess congress level[%u]", m_level, congress_build_level);
			return errcode_enum::family_err_code_other_build_level_greater_than_congress_level;
		}
	}

	uint32_t conf_id = (uint32_t)type * 100 + m_level;
	Family* p_family_conf = GET_CONF(Family, conf_id);
	if (NULL == p_family_conf)
	{
		log_error("build[%u] conf_id[%u] not find", type, conf_id);
		return errcode_enum::notice_unknown;
	}

	if (get_money() < (uint32_t)p_family_conf->require())
	{
		log_error("upgrade family build failed casuse of not enough family money[%u] while required[%d]", get_money(), p_family_conf->require());
		return errcode_enum::notice_gold_money_not_enough;
	}

	uint32_t time_now = common::time_util_t::now_time();
	if (time_now < p_family_build->get_cd_time() + (uint32_t)p_family_conf->cd_time())
	{
		log_error("upgrade family build failed cause of in cd time");
		return errcode_enum::family_err_code_build_in_cd;
	}

	cost_money((uint32_t)p_family_conf->cost(), proto::server::family_money_update_build);

	p_family_build->upgrade(time_now);
	family_data_mgr_t::save_one_build(p_family_build);

	switch (type) {
		case proto::common::family_build_type_congress: {
			set_level(p_family_build->get_level());
			break;
		}
		case proto::common::family_build_type_house: {
			update_member_size();
			break;
		}
		case proto::common::family_build_type_barn: {
			update_max_money();
			break;
		}
		default:
			break;
	}

	p_family_build->peek_data(reply.mutable_builds());
	proto::common::family_data* p_base = reply.mutable_data();
	if (p_base) p_base->CopyFrom(m_data);

	return errcode_enum::error_ok;
}

void family::set_level(uint32_t val)
{
	m_data.set_level(val);

	family_data_mgr_t::update_family_level(m_family_id, val);

	if (country_mgr_t::get_country_period() == proto::common::country_period_none && !time_manager_t::is_timer_activating(time_type_enum::time_country_unlock)) {
		auto conf = GET_CONF(Comprehensive, comprehensive_common::country_unlock_para1);
		uint32_t unlock_param = GET_COMPREHENSIVE_VALUE_1(conf);
		if (val >= unlock_param) { // 开启国加系统
			country_mgr_t::open_country_system(get_name());
		}
	}
}

uint32_t family::clear_build_cd(proto::common::family_build_type type)
{
	const family_build_ptr& p_family_build = get_family_build(type);
	if (NULL == p_family_build)
	{
		log_error("family[%lu] build[%u] not find", m_family_id, type);
		return common::errcode_enum::notice_unknown;
	}

	uint32_t m_level = p_family_build->get_level();

	uint32_t conf_id = (uint32_t)type * 100 + m_level;
	Family* p_family_conf = GET_CONF(Family, conf_id);
	if (NULL == p_family_conf)
	{
		log_error("build[%u] conf_id[%u] not find", type, conf_id);
		return errcode_enum::notice_unknown;
	}

	if (get_money() < (uint32_t)p_family_conf->cd_clean_cost())
	{
		log_error("clear upgrade family build cd failed casuse of not enough family money[%u] while required[%d]", get_money(), p_family_conf->cd_clean_cost());
		return errcode_enum::notice_gold_money_not_enough;
	}

	cost_money((uint32_t)p_family_conf->cd_clean_cost(), proto::server::family_money_clear_build_cd);

	p_family_build->clear_cd();
	family_data_mgr_t::save_one_build(p_family_build);

	return errcode_enum::error_ok;
}

uint32_t family::get_family_war_member_cnt(uint32_t level)
{
	uint32_t count = 0;
	for (family_member_map::const_iterator citr = m_member_map.begin();
		citr != m_member_map.end(); ++citr)
	{
		if (citr->second != NULL && citr->second->get_level() >= level)
		{
			count++;
		}
	}

	return count;
}

void family::update_member_glory(uint64_t role_uid, uint32_t glory)
{
	family_member_ptr p_member = get_family_member(role_uid);
	if (NULL == p_member)
	{
		log_error("NULL == p_member[%lu]", role_uid);
		return;
	}
	p_member->set_glory(glory);
	update_glory();
}

void family::update_glory()
{
	m_glory = 0;
	for (auto it : m_member_map)
	{
		auto p_member = it.second;
		if (NULL == p_member)
		{
			continue;
		}
		m_glory += p_member->get_glory();
	}
	rank_manager_t::update_rank(m_family_id, rank_common::family_glory_rank, m_glory);
}

void family::add_family_contribution(uint64_t uid, uint32_t val)
{
	const family_member_ptr& p_family_member = get_family_member(uid);

	if (NULL == p_family_member) {
		log_error("p_user[%lu] not member in family[%lu]", uid, m_family_id);
		return;
	}

	p_family_member->add_total_contribution(val);
}

void family::get_family_prayer_list(proto::common::family_prayer_list* p_list)
{
	if (!p_list) {
		log_error("family_prayer_list null error");
		return;
	}
	// 成员信息
	for (family_member_map::const_iterator citr = m_member_map.begin();
		citr != m_member_map.end(); ++citr) {
		if (citr->second != NULL && 
			citr->second->is_have_prayer_hero()) {
			citr->second->peek_prayer_data(p_list->add_lists());
		}
	}
}

void family::tower_trigger_notify(uint64_t role_uid, const proto::server::ge_tower_trigger_notify& msg)
{
	global_user_ptr p_self = global_user_manager_t::get_global_user_by_uid(role_uid);
	if (NULL == p_self)
	{
		log_error("NULL == p_self[%lu]", role_uid);
		return;
	}
	auto p_notify_str = GET_CONF(Comprehensive, comprehensive_common::dreamland_hide_find);
	const std::string& notify_str = p_notify_str->parameter1();
	proto::client::ec_personal_chat_notify notify_msg;
	notify_msg.set_src_role_id(string_util_t::uint64_to_string(role_uid));
	notify_msg.set_src_role_name(p_self->get_name());
	notify_msg.set_src_head_id(p_self->get_hero_plugin());
	notify_msg.set_src_level(p_self->get_level());
	notify_msg.set_contact_time(time_util_t::now_time());

	proto::server::eg_tower_trigger_notify ntf;
	for (int32_t i = 0; i < msg.tower_trigger_size(); ++i)
	{
		ntf.add_tower_trigger()->CopyFrom(msg.tower_trigger(i));
	}
	for (auto it : m_member_map)
	{
		auto p_member = it.second;
		if (NULL == p_member)
		{
			continue;
		}
		uint64_t member_uid = p_member->get_uid();
		if (member_uid == role_uid)
		{
			continue;
		}
		global_user_ptr p_role = global_user_manager_t::get_global_user_by_uid(member_uid);
		if (NULL != p_role)
		{
			//在线直接推送
			p_role->send_msg_to_game(op_cmd::eg_tower_trigger_notify, ntf);
			for (int32_t i = 0; i < msg.tower_trigger_size(); ++i)
			{
				uint32_t trigger_id = msg.tower_trigger(i).tid();
				auto p_conf = GET_CONF(TowerTrigger, trigger_id);
				if (NULL == p_conf)
				{
					continue;
				}
				std::string notice_text;
				StringTool::format(notice_text, notify_str.c_str(), p_conf->name().c_str());
				notify_msg.set_chat_info(common::string_util_t::convert_to_utf8(notice_text));
				p_role->send_msg_to_client(op_cmd::ec_personal_chat_notify, notify_msg);
			}
		}
		else
		{
			//离线数据处理
			offline_role_manager_t::add_other_trigger(member_uid, msg);
		}
	}
}

void family::change_country(uint32_t new_country_id)
{
	m_data.set_country(new_country_id);
	family_data_mgr_t::update_family_country(m_family_id, new_country_id);

	auto conf = GET_CONF(Comprehensive, comprehensive_common::country_loyalty_default);
	uint32_t country_loyalty_default = GET_COMPREHENSIVE_VALUE_1(conf);
	m_data.set_loyalty(country_loyalty_default);
	family_data_mgr_t::update_family_loyalty(m_family_id, country_loyalty_default);

	proto::server::eg_family_country_change_notify notify;
	notify.set_country_id(new_country_id);
	for (family_member_map::const_iterator citr = m_member_map.begin();
		citr != m_member_map.end(); ++citr) {
		global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(citr->first);
		if (p_user) {
			p_user->set_country_id(new_country_id);
			notify.add_role_list(citr->first);
		} else {
			global_user_data_mgr_t::set_user_country_id(citr->first, new_country_id);
			global_user_data_mgr_t::set_user_country_officer(citr->first, 0);
		}
	}

	env::server->send_msg_to_all_games(op_cmd::eg_family_country_change_notify, 0, notify);
}

void family::change_king_family(bool is_king)
{
	set_king_family(is_king);

	auto conf = GET_CONF(Comprehensive, is_king ? comprehensive_common::country_loyalty_upper : comprehensive_common::country_loyalty_default);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(conf);

	m_data.set_loyalty(val);
	family_data_mgr_t::update_family_loyalty(m_family_id, val);
}

bool family::check_quit_country_times()
{
	uint32_t local_month = common::time_util_t::get_local_month(common::time_util_t::now_time());
	if (local_month == m_quit_time) {
		auto conf = GET_CONF(Comprehensive, comprehensive_common::country_quit_times);
		uint32_t val = GET_COMPREHENSIVE_VALUE_1(conf);
		if (m_quit_count >= val) {
			return false;
		}

		m_quit_count += 1;
	} else {
		m_quit_time = local_month;
		m_quit_count = 1;
	}

	family_data_mgr_t::update_quit_country(m_family_id, m_quit_time, m_quit_count);

	return true;
}

bool family::is_in_invite_list_cd(uint64_t uid)
{
	invite_list_map::const_iterator citr = m_invite_list_map.find(uid);
	if (citr != m_invite_list_map.end() && common::time_util_t::now_time() <= citr->second) return true;

	return false;
}

void family::add_in_invite_list(uint64_t uid)
{
	auto conf = GET_CONF(Comprehensive, comprehensive_common::family_invite_overtime);
	uint32_t val = GET_COMPREHENSIVE_VALUE_1(conf);
	m_invite_list_map[uid] = common::time_util_t::now_time() + val;
}

void family::del_from_invite_list(uint64_t uid)
{
	invite_list_map::iterator itr = m_invite_list_map.find(uid);
	if (itr != m_invite_list_map.end()) {
		m_invite_list_map.erase(itr);
	}
}

uint32_t family::agree_invite(const global_user_ptr& p_user, uint64_t inviter_uid)
{
	invite_list_map::iterator itr = m_invite_list_map.find(p_user->get_uid());
	if (itr == m_invite_list_map.end()) {
		log_error("user[%lu] not in family[%lu] invite list", p_user->get_uid(), m_family_id);
		return common::errcode_enum::family_error_not_in_invite_list;
	}

	uint32_t invite_out_time = itr->second;
	m_invite_list_map.erase(itr);

	if (invite_out_time < common::time_util_t::now_time()) {
		log_error("user[%lu] invite time out", p_user->get_uid());
		return common::errcode_enum::family_error_invite_time_out;
	}

	if (p_user->get_family_id() != 0) {
		log_error("user[%lu] is already join other family[%lu]", p_user->get_uid(), p_user->get_family_id());
		return common::errcode_enum::family_err_code_is_join_other;
	}

	auto conf = GET_CONF(Comprehensive, comprehensive_common::family_join_lv);
	uint32_t need_level = GET_COMPREHENSIVE_VALUE_1(conf);
	if (p_user->get_level() < need_level) {
		log_error("user[%lu] join family level not enough!!", p_user->get_uid());
		return common::errcode_enum::notice_join_family_lv;
	}

	if (m_member_map.size() >= get_max_member_size()) {
		FAMILY_LOG("user[%lu] invite family[%lu] is max_member_size", p_user->get_uid(), m_family_id);
		return common::errcode_enum::family_err_code_excess_max_member_number;
	}

	if (!join_in(p_user, family_officer_type_member)) {
		return common::errcode_enum::notice_unknown;
	}

	WRITE_FAMILY_LOG_TO_DB(m_family_id, inviter_uid, p_user->get_uid(), common::log_enum::source_type_family_invite_join, family_officer_type_member);

	return common::errcode_enum::error_ok;
}

bool family::check_shop_expired()
{
	uint32_t curr_time = common::time_util_t::now_time();
	if (curr_time > m_family_shop.get_next_refresh_time()) {
		m_family_shop.refresh_family_shop(m_data.level());
		return true;
	}

	return false;
}

void family::peek_family_shop_data(proto::common::family_shop_data* p_data)
{
	check_shop_expired();

	if (p_data) p_data->CopyFrom(m_family_shop.get_family_shop_data());
}

uint32_t family::family_shop_buy(const global_user_ptr& p_user, uint32_t index, uint32_t shop_item, uint32_t num)
{
	if (NULL == p_user) {
		log_error("null == p_user");
		return common::errcode_enum::notice_unknown;
	}

	const family_member_ptr& p_member = get_family_member(p_user->get_uid());
	if (NULL == p_member) {
		log_error("user[%lu] not family[%lu] member", p_user->get_uid(), m_family_id);
		return common::errcode_enum::notice_unknown;
	}

	// 检测商店信息是否国期
	if (check_shop_expired()) {
		log_error("user[%lu] family shop expired", p_user->get_uid());
		return common::errcode_enum::family_shop_data_expired;
	}

	const proto::common::family_shop_data& shop_data = m_family_shop.get_family_shop_data();
	if ((int32_t)index >= shop_data.items_size()) {
		log_error("user[%lu] family_shop index[%u] not find", p_user->get_uid(), index);
		return common::errcode_enum::family_shop_data_error;
	}

	const proto::common::family_shop_item& shop_item_data = shop_data.items(index);
	if (shop_item_data.shop_item() != shop_item) {
		log_error("user[%lu] family_shop index[%u] item[%u] != %u", p_user->get_uid(), index, shop_item_data.shop_item(), shop_item);
		return common::errcode_enum::family_shop_data_error;
	}

	ShopItemTable* p_ShopItem = GET_CONF(ShopItemTable, shop_item);
	if (NULL == p_ShopItem) {
		log_error("user[%lu] family_shop shop_item[%u] not find", p_user->get_uid(), shop_item);
		return common::errcode_enum::random_shop_error_item_not_find;
	}

	// 家族等级检查
	if (p_ShopItem->family_level() != 0 && get_build_level(proto::common::family_build_type_shop) < p_ShopItem->family_level()) {
		log_error("user[%lu] family_level[%u] < shop_item[%u] family_level[%u]", p_user->get_uid(), get_level(), shop_item, p_ShopItem->family_level());
		return common::errcode_enum::family_shop_family_level_limit;
	}

	// 家族限购检查
	if (p_ShopItem->family_limit() != 0) {
		if (shop_item_data.sold_count() >= p_ShopItem->family_limit()) {
			log_error("user[%lu] family_shop shop_item[%u] sold_count[%u] >= family_limit[%u] ", p_user->get_uid(), shop_item, shop_item_data.sold_count(), p_ShopItem->family_limit());
			return common::errcode_enum::family_shop_limit_max;
		}

		if (shop_item_data.sold_count() + num > p_ShopItem->family_limit()) {
			log_error("user[%lu] family_shop shop_item[%u] sold_count + num[%u] > family_limit[%u] ", p_user->get_uid(), shop_item, shop_item_data.sold_count() + num, p_ShopItem->family_limit());
			return common::errcode_enum::family_shop_count_limit;
		}
	}

	// 个人限购检查
	if (p_ShopItem->personal_limit() != 0) {
		uint32_t personal_limit = p_member->get_personal_buy_count(index);
		if (personal_limit >= p_ShopItem->personal_limit()) {
			log_error("user[%lu] family_shop shop_item[%u] personal_buy_count[%u] >= personal_limit[%u] ", p_user->get_uid(), shop_item, personal_limit, p_ShopItem->personal_limit());
			return common::errcode_enum::family_shop_personal_limit_max;
		}

		if (personal_limit + num > p_ShopItem->personal_limit()) {
			log_error("user[%lu] family_shop shop_item[%u] personal_buy_count + num[%u] >= personal_limit[%u] ", p_user->get_uid(), shop_item, personal_limit + num, p_ShopItem->personal_limit());
			return common::errcode_enum::family_shop_personal_count_limit;
		}

		// 限购判断成功
		p_member->add_personal_buy_count(m_family_shop.get_next_refresh_time(), index, num);
	}

	// 判断成功在加数量
	if (p_ShopItem->family_limit() != 0) {
		m_family_shop.add_shop_buy_count(index, num);
	}

	// 通知game扣钱
	proto::server::eg_family_shop_buy_reply reply;
	reply.set_refresh_time(m_family_shop.get_next_refresh_time());
	reply.set_index(index);
	reply.set_shop_item(shop_item);
	reply.set_num(num);
	proto::common::family_shop_data* p_shop_data = reply.mutable_shop_data();
	if (p_shop_data) p_shop_data->CopyFrom(m_family_shop.get_family_shop_data());
	proto::common::family_shop_personal_data* p_shop_personal_data = reply.mutable_personal_data();
	if (p_shop_personal_data) p_shop_personal_data->CopyFrom(p_member->get_shop_personal_data());
	p_user->send_msg_to_game(op_cmd::eg_family_shop_buy_reply, reply);

	return common::errcode_enum::error_ok;
}

void family::family_shop_failed_back(const global_user_ptr& p_user, uint32_t refresh_time, uint32_t index, uint32_t num)
{
	if (NULL == p_user) {
		log_error("null == p_user");
		return;
	}

	const family_member_ptr& p_member = get_family_member(p_user->get_uid());
	if (NULL == p_member) {
		log_error("user[%lu] not family[%lu] member", p_user->get_uid(), m_family_id);
		return;
	}

	if (m_family_shop.get_next_refresh_time() != refresh_time) {
		log_error("user[%lu] failed back error, is already refresh", p_user->get_uid());
		return;
	}

	const proto::common::family_shop_data& shop_data = m_family_shop.get_family_shop_data();
	if ((int32_t)index >= shop_data.items_size()) {
		log_error("user[%lu] family_shop index[%u] not find", p_user->get_uid(), index);
		return;
	}

	const proto::common::family_shop_item& shop_item_data = shop_data.items(index);
	ShopItemTable* p_ShopItem = GET_CONF(ShopItemTable, shop_item_data.shop_item());
	if (NULL == p_ShopItem) {
		log_error("user[%lu] family_shop shop_item[%u] not find", p_user->get_uid(), shop_item_data.shop_item());
		return;
	}

	if (p_ShopItem->family_limit() != 0) {
		m_family_shop.shop_failed_back(index, num);
	}

	if (p_ShopItem->personal_limit() != 0) {
		p_member->personal_failed_back(refresh_time, index, num);
	}
}

uint64_t family::get_deputy_general_id() const
{
	officer_map::const_iterator citr = m_officer_map.find(family_officer_type::family_officer_type_vice_general);
	if (citr != m_officer_map.end() && !citr->second.empty()) {
		officer_vec::const_iterator sitr = citr->second.begin();
		if (sitr != citr->second.end()) {
			return *sitr;
		}
	}

	return 0;
}

uint32_t family::get_build_level(proto::common::family_build_type type) const
{
	family_build_map::const_iterator citr = m_build_map.find(type);
	if (citr != m_build_map.end() && citr->second != NULL) {
		return citr->second->get_level();
	}

	return 0;
}
