#include "family_manager.hpp"
#include "redis/family_data_mgr.hpp"
#include "log.hpp"
#include "config/config_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "common/time_manager.hpp"
#include "tblh/time_type_enum.hpp"
#include "global_id.hpp"
#include "tblh/errcode_enum.hpp"
#include "tblh/FamilyOfficial.tbls.h"
#include "user/global_user_manager.hpp"
#include "tblh/TimeTable.tbls.h"
#include "common/timer_common.hpp"
#include "redis/global_data_mgr.hpp"
#include "tblh/MapTable.tbls.h"
#include "scene/global_scene_manager.hpp"
#include <algorithm>
#include "log/log_wrapper.hpp"
#include "sys_notice/sys_notice_manager.hpp"
#include "country/country_mgr.hpp"

#include "tblh/FamilyWarPersonal.tbls.h"
#include "tblh/FamilyWarRank.tbls.h"
#include "mail/mail_manager.hpp"
#include "tblh/mailcode_enum.hpp"
#include "tblh/MailTable.tbls.h"
#include "redis/global_user_data_mgr.hpp"
#include "rank/rank_manager.hpp"

#define WRITE_FAMILY_LOG_TO_DB(family_uid, role_uid, target_uid, type, param) \
    log_wrapper_t::send_family_log(family_uid, role_uid, target_uid, type, param);

family_manager_t::family_map family_manager_t::m_family_map;

family_manager_t::family_sort_vec family_manager_t::m_family_sort_vec;

family_manager_t::family_war_info_t family_manager_t::m_family_war_info;

family_manager_t::against_data_map family_manager_t::m_against_data_map;

proto::common::family_against_list family_manager_t::m_against_list;

family_manager_t::family_against_id_map family_manager_t::m_family_against_id_map;

family_manager_t::history_data_deque family_manager_t::m_history_deque;

bool family_manager_t::init()
{
	// 注册时间回调
	time_manager_t::register_func(time_type_enum::time_one_day, family_manager_t::daily_update_callback);

	time_manager_t::register_func(time_type_enum::time_family_war1, family_manager_t::war_init_callback);
	time_manager_t::register_func(time_type_enum::time_family_war2, family_manager_t::war_prepare_callback);
	time_manager_t::register_func(time_type_enum::time_family_war3, family_manager_t::war_start_callback);

	// 加载家族数据
	if (!family_data_mgr_t::load_all_datas(m_family_map))
	{
		log_error("load family data error");
		return false;
	}

	process_family_sort();

	// 加载家族战数据
	global_data_mgr_t::load_family_war_data(proto::server::global_data_type_family_war, m_family_war_info.family_war_data);

	uint32_t now_time = common::time_util_t::now_time();
	uint32_t week_id = get_week_id(now_time);
	uint32_t history_week_id = week_id;			// 历史记录从哪一周开始读

	if (week_id == m_family_war_info.family_war_data.week_id()) // 同个周期内
	{
		switch (m_family_war_info.family_war_data.state())
		{
		case proto::common::family_war_state_end:// 结束或未开始
			{
			}
			break;
		case proto::common::family_war_state_init:// 公示期，检测时间
			{
				// 检测当前时间是否已经过了备战时期
				std::pair<uint32_t, uint32_t> next_start_time = time_manager_t::get_refresh_time(time_type_enum::time_family_war2);
				uint32_t next_time_week = get_week_id(next_start_time.first);

				// 开战时间已经到下个周期了或者当前时间已经过了开战期
				if (next_time_week != week_id || now_time >= next_start_time.first)
				{
					// 时间已经超过开战时间,重置家族战状态
					m_family_war_info.family_war_data.set_state(proto::common::family_war_state_end);
					// 保存状态
					global_data_mgr_t::save_family_war_data(proto::server::global_data_type_family_war, m_family_war_info.family_war_data);
				}
				else
				{
					history_week_id = week_id - 1;	// 当前还处于公示期
				}
			}
			break;
		case proto::common::family_war_state_prepare:	// 备战
		case proto::common::family_war_state_start:		// 开战
			{
				// 直接设置结束
				m_family_war_info.family_war_data.set_state(proto::common::family_war_state_end);
				// 保存状态
				global_data_mgr_t::save_family_war_data(proto::server::global_data_type_family_war, m_family_war_info.family_war_data);
			}
			break;
		}

		// 加载对战家族列表
		family_data_mgr_t::load_all_war_data(week_id, m_against_data_map, m_family_against_id_map);
		update_against_list();
	}
	else // 不在同一时期内
	{
		// 检测当前时间是否已经过了备战时期
		std::pair<uint32_t, uint32_t> next_start_time = time_manager_t::get_refresh_time(time_type_enum::time_family_war2);
		uint32_t next_time_week = get_week_id(next_start_time.first);

		// 备战期已经在在下个周期说明已经过了备战期
		if (next_time_week != week_id || now_time >= next_start_time.first)
		{
			m_family_war_info.family_war_data.set_week_id(week_id);
			m_family_war_info.family_war_data.set_state(proto::common::family_war_state_end);

			// 保存状态
			global_data_mgr_t::save_family_war_data(proto::server::global_data_type_family_war, m_family_war_info.family_war_data);

			// 对战家族列表还未生成，已经过了开战时期，此时表示本轮轮空
		}
		else
		{
			// 还在公示期，生成对战列表
			if (generate_war_list(week_id))
			{
				m_family_war_info.family_war_data.set_state(proto::common::family_war_state_init);
			}
			else
			{
				m_family_war_info.family_war_data.set_state(proto::common::family_war_state_end);
			}

			m_family_war_info.family_war_data.set_week_id(week_id);
			global_data_mgr_t::save_family_war_data(proto::server::global_data_type_family_war, m_family_war_info.family_war_data);

			history_week_id = week_id - 1;
		}
	}

	m_family_war_info.family_war_time = get_war_time(week_id);

	// 加载家族战历史数据
	for (uint32_t i = 0; i < FAMILY_WAR_HISTORY_SIZE; ++i)
	{
		proto::common::family_war_history_data data;

		if (family_data_mgr_t::load_all_war_history_data(history_week_id - i, data))
		{
			m_history_deque.push_front(data);
		}
	}

	return true;
}

bool family_manager_t::close()
{
	m_family_map.clear();

	return true;
}

void family_manager_t::daily_update_callback(uint32_t id, bool is_in_time)
{
	if (is_in_time)
	{
		for (family_map::const_iterator citr = m_family_map.begin();
			citr != m_family_map.end(); ++citr)
		{
			const family_ptr& p_family = citr->second;
			if (NULL != p_family)
			{
				p_family->auto_newbie_member();
				p_family->update_prosperity();
				p_family->update_prestige_daily();
				p_family->update_maintain_cost();
			}
		}

		// 更新排名
		process_family_sort();
	}
}

void family_manager_t::war_init_callback(uint32_t id, bool is_in_time)
{
	if (is_in_time)
	{
		uint32_t now_time = common::time_util_t::now_time();
		uint32_t week_id = get_week_id(now_time);

		m_family_war_info.family_war_data.set_week_id(week_id);

		if (generate_war_list(week_id))
		{
			m_family_war_info.family_war_data.set_state(proto::common::family_war_state_init);

			std::pair<uint32_t, uint32_t> battle_time = time_manager_t::get_refresh_time(time_type_enum::time_family_war3);

			m_family_war_info.family_war_time = battle_time.first;
		}
		else
		{
			m_family_war_info.family_war_data.set_state(proto::common::family_war_state_end);
		}

		global_data_mgr_t::save_family_war_data(proto::server::global_data_type_family_war, m_family_war_info.family_war_data);
	}
}

void family_manager_t::war_prepare_callback(uint32_t id, bool is_in_time)
{
	if (is_in_time)
	{
		if (m_family_war_info.family_war_data.state() != proto::common::family_war_state_init)
		{
			log_warn("family war prepare failed, family_war state[%u] error", m_family_war_info.family_war_data.state());
			return;
		}

		auto conf = GET_CONF(Comprehensive, comprehensive_common::family_war_dungeon);
		uint32_t dungeon_scene = GET_COMPREHENSIVE_VALUE_1(conf);
		MapTable* p_map_table = GET_CONF(MapTable, dungeon_scene);

		if (NULL == p_map_table)
		{
			log_error("family war dungeon scene[%u] not find", dungeon_scene);
			// 设置家族战结束
			m_family_war_info.family_war_data.set_state(proto::common::family_war_state_end);
			global_data_mgr_t::save_family_war_data(proto::server::global_data_type_family_war, m_family_war_info.family_war_data);
			return;
		}

		const network::tcp_socket_ptr s = game_manager_t::get_game_socket_by_id(p_map_table->main_game().first);
		if (NULL == s)
		{
			log_error("game[%u] not open!!!!!!!!!!!!!", p_map_table->main_game().first);
			// 设置家族战结束
			m_family_war_info.family_war_data.set_state(proto::common::family_war_state_end);
			global_data_mgr_t::save_family_war_data(proto::server::global_data_type_family_war, m_family_war_info.family_war_data);
			return;
		}

		m_family_war_info.family_war_data.set_state(proto::common::family_war_state_prepare);
		global_data_mgr_t::save_family_war_data(proto::server::global_data_type_family_war, m_family_war_info.family_war_data);

		proto::client::ec_family_war_state_notify ntf;
		ntf.set_state(proto::common::family_war_state_prepare);
		ntf.set_war_time(m_family_war_info.family_war_time);

		// 备战生成对战副本
		proto::server::eg_prepare_family_war_request request;
		request.set_start_time(m_family_war_info.family_war_time);
		
		for (against_data_map::iterator itr = m_against_data_map.begin();
			itr != m_against_data_map.end(); ++itr)
		{
			family_against_data_t& war_data = itr->second;

			uint64_t left_family = common::string_util_t::string_to_uint64(war_data.m_data.left_side().family_id());
			family_ptr p_family_left = get_family(left_family);
			if (p_family_left == NULL) {
				left_family = 0;
				war_data.m_state = proto::common::family_war_state_end;
			}

			uint64_t right_family = common::string_util_t::string_to_uint64(war_data.m_data.right_side().family_id());
			family_ptr p_family_right = get_family(right_family);
			if (p_family_right == NULL) {
				right_family = 0;
				war_data.m_state = proto::common::family_war_state_end;
			}

			if (left_family == 0 && right_family == 0) {
				log_error("family[%s] and family[%s] all not find when open family war", war_data.m_data.left_side().family_id().c_str(), war_data.m_data.right_side().family_id().c_str());
				continue;
			} else if (left_family == 0) {
				FAMILY_LOG("family[%s] not find when open family war , family[%lu] win", war_data.m_data.left_side().family_id().c_str(), right_family);
				proto::common::family_war_data* p_left_data = itr->second.m_data.mutable_left_side();
				if (p_left_data) {
					p_left_data->set_result(proto::common::family_war_result_failure);
				}
				proto::common::family_war_data* p_right_data = itr->second.m_data.mutable_right_side();
				if (p_right_data) {
					p_right_data->set_result(proto::common::family_war_result_win);
				}
				family_war_early_end(right_family);
				continue;
			} else if (right_family == 0) {
				FAMILY_LOG("family[%s] not find when open family war , family[%lu] win", war_data.m_data.right_side().family_id().c_str(), left_family);
				proto::common::family_war_data* p_left_data = itr->second.m_data.mutable_left_side();
				if (p_left_data) {
					p_left_data->set_result(proto::common::family_war_result_win);
				}
				proto::common::family_war_data* p_right_data = itr->second.m_data.mutable_right_side();
				if (p_right_data) {
					p_right_data->set_result(proto::common::family_war_result_failure);
				}
				family_war_early_end(left_family);
				continue;
			}

			proto::server::family_war_dungeon_data* p_data = request.add_family_war_data();
			if (NULL != p_data)
			{
				p_data->set_id(itr->first);

				proto::server::family_war_dungeon_info* p_left_family = p_data->mutable_left_family();
				if (NULL != p_left_family)
				{
					p_left_family->set_family_id(left_family);
					p_left_family->set_family_name(war_data.m_data.left_side().family_name());
				}

				proto::server::family_war_dungeon_info* p_right_family = p_data->mutable_right_family();
				if (NULL != p_right_family)
				{
					p_right_family->set_family_id(right_family);
					p_right_family->set_family_name(war_data.m_data.right_side().family_name());
				}
			}

			if (p_family_left) { // 备战通知
				p_family_left->send_msg_to_family(op_cmd::ec_family_war_state_notify, ntf);
				p_family_left->send_sys_notice_to_member(sys_notice_enum::sys_notice_114);
			}

			if (p_family_right) { // 备战通知
				p_family_right->send_msg_to_family(op_cmd::ec_family_war_state_notify, ntf);
				p_family_right->send_sys_notice_to_member(sys_notice_enum::sys_notice_114);
			}

			war_data.m_state = proto::common::family_war_state_prepare;
		}

		env::server->send_msg_to_game(s, op_cmd::eg_prepare_family_war_request, 0, request);
	}
}

void family_manager_t::war_start_callback(uint32_t id, bool is_in_time)
{
	if (is_in_time)
	{
		if (m_family_war_info.family_war_data.state() != proto::common::family_war_state_prepare)
		{
			log_warn("family war start failed, family_war state[%u] error", m_family_war_info.family_war_data.state());
			return;
		}

		// 设置家族战开始，通知逻辑服
		auto conf = GET_CONF(Comprehensive, comprehensive_common::family_war_dungeon);
		uint32_t dungeon_scene = GET_COMPREHENSIVE_VALUE_1(conf);
		MapTable* p_map_table = GET_CONF(MapTable, dungeon_scene);

		if (NULL == p_map_table)
		{
			log_error("family war dungeon scene[%u] not find", dungeon_scene);
			// 设置家族战结束
			m_family_war_info.family_war_data.set_state(proto::common::family_war_state_end);
			global_data_mgr_t::save_family_war_data(proto::server::global_data_type_family_war, m_family_war_info.family_war_data);
			return;
		}

		const network::tcp_socket_ptr s = game_manager_t::get_game_socket_by_id(p_map_table->main_game().first);
		if (NULL == s)
		{
			log_error("game[%u] not open!!!!!!!!!!!!!", p_map_table->main_game().first);
			// 设置家族战结束
			m_family_war_info.family_war_data.set_state(proto::common::family_war_state_end);
			global_data_mgr_t::save_family_war_data(proto::server::global_data_type_family_war, m_family_war_info.family_war_data);
			return;
		}

		std::pair<uint32_t, uint32_t> battle_time = time_manager_t::get_refresh_time(time_type_enum::time_family_war3);
		m_family_war_info.family_war_data.set_state(proto::common::family_war_state_start);
		m_family_war_info.family_war_time = battle_time.second;
		global_data_mgr_t::save_family_war_data(proto::server::global_data_type_family_war, m_family_war_info.family_war_data);

		proto::server::eg_family_war_start_notify notify;
		notify.set_end_time(battle_time.second);
		env::server->send_msg_to_game(s, op_cmd::eg_family_war_start_notify, 0, notify);

		proto::client::ec_family_war_state_notify ntf;
		ntf.set_state(proto::common::family_war_state_start);
		ntf.set_war_time(m_family_war_info.family_war_time);

		// 激战通知
		for (against_data_map::iterator citr = m_against_data_map.begin();
			citr != m_against_data_map.end(); ++citr) {
			family_against_data_t& war_data = citr->second;
			if (war_data.m_war_scene_id == 0) {
				war_data.m_state = proto::common::family_war_state_end;
				continue;
			}

			uint64_t left_family = common::string_util_t::string_to_uint64(war_data.m_data.left_side().family_id());
			family_ptr p_family_left = get_family(left_family);
			if (p_family_left != NULL) {
				p_family_left->send_msg_to_family(op_cmd::ec_family_war_state_notify, ntf);
			}

			uint64_t right_family = common::string_util_t::string_to_uint64(war_data.m_data.right_side().family_id());
			family_ptr p_family_right = get_family(right_family);
			if (p_family_right != NULL) {
				p_family_right->send_msg_to_family(op_cmd::ec_family_war_state_notify, ntf);
			}

			war_data.m_state = proto::common::family_war_state_start;
		}
	}
	else
	{
		if (m_family_war_info.family_war_data.state() != proto::common::family_war_state_start)
		{
			log_warn("family war end failed, family_war state[%u] error", m_family_war_info.family_war_data.state());
			return;
		}

		// 设置家族战结束
		family_war_end();

		// 设置家族战开始，通知逻辑服
		auto conf = GET_CONF(Comprehensive, comprehensive_common::family_war_dungeon);
		uint32_t dungeon_scene = GET_COMPREHENSIVE_VALUE_1(conf);
		MapTable* p_map_table = GET_CONF(MapTable, dungeon_scene);

		if (NULL == p_map_table)
		{
			log_error("family war dungeon scene[%u] not find", dungeon_scene);
			return;
		}

		const network::tcp_socket_ptr s = game_manager_t::get_game_socket_by_id(p_map_table->main_game().first);
		if (NULL == s)
		{
			log_error("game[%u] not open!!!!!!!!!!!!!", p_map_table->main_game().first);
			return;
		}

		proto::server::eg_family_war_end_notify notify;
		env::server->send_msg_to_game(s, op_cmd::eg_family_war_end_notify, 0, notify);
	}
}

uint32_t family_manager_t::get_max_apply_size()
{
	auto conf = GET_CONF(Comprehensive, comprehensive_common::family_apply_cnt);
	return GET_COMPREHENSIVE_VALUE_1(conf);
}

// 议事厅是1级 其他默认0级
uint32_t family_manager_t::get_build_define_level(proto::common::family_build_type type)
{
	switch (type)
	{
	case proto::common::family_build_type_congress:
		{
			auto conf = GET_CONF(Comprehensive, comprehensive_common::family_build_base_level);
			return GET_COMPREHENSIVE_VALUE_1(conf);
		}
		break;
	default:
		{
			return 0;
		}
		break;
	}

	return 0;
}

uint64_t family_manager_t::get_new_family_uid()
{
	return common::g_id_allocator.gen_new_id(common::ID_ALLOCATOR_TYPE_FAMILY);
}

void family_manager_t::create_family_from_game(const global_user_ptr& p_user, const std::string& name, const std::string& declaration, proto::server::eg_create_family_reply& reply)
{
	if (NULL == p_user)
	{
		reply.set_reply_code(errcode_enum::notice_unknown);
		log_error("null == p_user");
		return;
	}

	/*if (0 == p_user->get_country_id())
	{
		reply.set_reply_code(errcode_enum::country_error_not_in_country);
		FAMILY_LOG("user[%lu] not join country yet", p_user->get_uid());
		return;
	}*/

	if (0 != p_user->get_family_id())
	{
		reply.set_reply_code(errcode_enum::family_err_code_already_join_other_family);
		FAMILY_LOG("user[%lu] already join other family!! can't create family", p_user->get_uid());
		return;
	}

	uint32_t vaild_code = is_family_name_vaild(name);
	if (vaild_code != errcode_enum::error_ok)
	{
		reply.set_reply_code(vaild_code);
		return;
	}

	uint64_t new_family_id = get_new_family_uid();
	if (0 == new_family_id)
	{
		reply.set_reply_code(errcode_enum::notice_unknown);
		log_error("user[%lu] create new_family_id error", p_user->get_uid());
		return;
	}

	family_ptr p_family(new family(new_family_id));
	if (NULL == p_family)
	{
		reply.set_reply_code(errcode_enum::notice_unknown);
		log_error("NULL == p_family");
		return;
	}

	if (!p_family->init(p_user, name, declaration))
	{
		reply.set_reply_code(errcode_enum::notice_unknown);
		log_error("user[%lu] create family error", p_user->get_uid());
		return;
	}

	std::pair<family_map::iterator, bool> ret = m_family_map.insert(family_map::value_type(new_family_id, p_family));
	if (!ret.second)
	{
		reply.set_reply_code(errcode_enum::notice_unknown);
		log_error("family[%lu] insert error, it is exist", new_family_id);
		return;
	}

	m_family_sort_vec.push_back(family_sort_t(p_family->get_family_id(), p_family->get_level(), p_family->get_name(), p_family->get_prestige()));

	FAMILY_LOG("user[%lu] create family[%lu] name[%s] success", p_user->get_uid(), p_family->get_family_id(), name.c_str());

	WRITE_FAMILY_LOG_TO_DB(new_family_id, p_user->get_uid(), 0, common::log_enum::source_type_family_create, 0);

	reply.set_reply_code(errcode_enum::error_ok);
	proto::server::eg_update_family_info_notify* info = reply.mutable_info();
	if (info)
	{
		info->set_id(p_family->get_family_id());
		info->set_name(name);
		info->set_officer(family_officer_type_general);
		info->set_country_id(0);
		info->set_family_lv(p_family->get_level());
	}

	// 创建家族成功，设置玩家主人物数据
	p_user->join_family(p_family->get_family_id(), family_officer_type_general, 0);

	WRITE_FAMILY_LOG_TO_DB(new_family_id, p_user->get_uid(), 0, common::log_enum::source_type_family_join, family_officer_type_general);


	BROADCAST_SYS_NOTICE_SYS_CHANNEL(p_user->get_uid(), proto::common::sys_notify_type_create_family, 0, name.c_str());	
	notify_chat_member_change(p_family->get_family_id(), p_user->get_uid(), 1);
}

bool family_manager_t::del_family_when_no_member(uint64_t family_id)
{
	family_map::iterator itr = m_family_map.find(family_id);
	if (itr == m_family_map.end())
	{
		log_error("family[%lu] is not find when del", family_id);
		return false;
	}

	m_family_map.erase(itr);

	rank_manager_t::rank_del_family(family_id);

	family_data_mgr_t::del_family(family_id);

	FAMILY_LOG("family[%lu] del success", family_id);

	// 删除家族不需要从m_family_sort_vec中删除，在拿列表的时候可能找不到，这个无所谓
	WRITE_FAMILY_LOG_TO_DB(family_id, 0, 0, common::log_enum::source_type_family_delete, 0);

	return true;
}

void family_manager_t::process_family_sort()
{
	m_family_sort_vec.clear();

	for (family_map::const_iterator citr = m_family_map.begin();
		citr != m_family_map.end(); ++citr)
	{
		if (citr->second != NULL)
		{
			m_family_sort_vec.push_back(family_sort_t(citr->second->get_family_id(), citr->second->get_level(), citr->second->get_name(), citr->second->get_prestige()));
		}
	}

	std::sort(m_family_sort_vec.begin(), m_family_sort_vec.end());
}

uint32_t family_manager_t::invite_family_member(const global_user_ptr& p_user, uint64_t invite_uid)
{
	if (NULL == p_user) {
		log_error("p_user == null");
		return common::errcode_enum::notice_unknown;
	}

	global_user_ptr p_invite_user = global_user_manager_t::get_global_user_by_uid(invite_uid);
	if (p_invite_user == NULL) {
		log_error("p_user[%lu] invite_user[%lu] not find", p_user->get_uid(), invite_uid);
		return common::errcode_enum::notice_unknown;
	}

	const family_ptr& p_family = get_family(p_user->get_family_id());
	if (NULL == p_family) {
		log_error("user[%lu] family[%lu] not find", p_user->get_uid(), p_user->get_family_id());
		return common::errcode_enum::family_err_code_not_join_family;
	}

	if (!has_family_privilige(p_user->get_family_officer(), privilige_type_family_member_audit)) {
		log_error("user[%lu] has no right to audit member", p_user->get_uid());
		return common::errcode_enum::family_err_code_no_right;
	}

	if (0 != p_invite_user->get_family_id()) {
		FAMILY_LOG("user[%lu] invite_user[%lu] already join other family", p_user->get_uid(), invite_uid);
		return errcode_enum::family_error_is_join_other_family;
	}

	auto conf = GET_CONF(Comprehensive, comprehensive_common::family_join_lv);
	uint32_t need_level = GET_COMPREHENSIVE_VALUE_1(conf);
	if (p_invite_user->get_level() < need_level) {
		FAMILY_LOG("user[%lu] invite_user[%lu] level not enough!!", p_invite_user->get_uid(), invite_uid);
		return common::errcode_enum::family_error_invite_level_limit;
	}

	if (p_family->is_member_full()) {
		FAMILY_LOG("user[%lu] family[%lu] member full", p_user->get_uid(), p_family->get_family_id());
		return common::errcode_enum::family_err_code_excess_max_member_number;
	}

	if (!p_invite_user->is_online()) {
		FAMILY_LOG("user[%lu] invite_user[%lu] not online", p_user->get_uid(), invite_uid);
		return common::errcode_enum::notice_not_online;
	}

	if (p_family->is_in_invite_list_cd(invite_uid)) {
		FAMILY_LOG("user[%lu] invite_user[%lu] not invite cd", p_user->get_uid(), invite_uid);
		return common::errcode_enum::family_error_is_already_invite;
	}

	p_family->add_in_invite_list(invite_uid);

	proto::client::ec_family_invite_notify notify;
	notify.set_inviter_uid(common::string_util_t::uint64_to_string(p_user->get_uid()));
	notify.set_inviter_name(p_user->get_name());
	notify.set_family_uid(common::string_util_t::uint64_to_string(p_family->get_family_id()));
	notify.set_family_name(p_family->get_name());
	notify.set_family_level(p_family->get_level());
	p_invite_user->send_msg_to_client(op_cmd::ec_family_invite_notify, notify);

	return common::errcode_enum::error_ok;
}

void family_manager_t::join_family_request(const global_user_ptr& p_user, uint64_t family_id, proto::client::ec_join_family_reply& reply)
{
	if (NULL == p_user)
	{
		reply.set_reply_code(errcode_enum::notice_unknown);
		log_error("null == p_user");
		return;
	}

	/*if (0 == p_user->get_country_id())
	{
		reply.set_reply_code(errcode_enum::country_error_not_in_country);
		FAMILY_LOG("user[%lu] not join country yet", p_user->get_uid());
		return;
	}*/

	if (family_id == 0) {
		reply.set_reply_code(errcode_enum::family_error_user_no_family);
		FAMILY_LOG("user[%lu] join family_id == 0", p_user->get_uid());
		return;
	}

	if (0 != p_user->get_family_id())
	{
		reply.set_reply_code(errcode_enum::family_err_code_already_join_other_family);
		FAMILY_LOG("user[%lu] already join other family!! can't create family", p_user->get_uid());
		return;
	}

	const family_ptr& p_family = get_family(family_id);
	if (NULL == p_family)
	{
		reply.set_reply_code(errcode_enum::family_err_code_invalid_family);
		FAMILY_LOG("user[%lu] family[%lu] not find", p_user->get_uid(), family_id);
		return;
	}

	auto conf = GET_CONF(Comprehensive, comprehensive_common::family_join_lv);
	uint32_t need_level = GET_COMPREHENSIVE_VALUE_1(conf);
	if (p_user->get_level() < need_level) {
		reply.set_reply_code(errcode_enum::notice_join_family_lv);
		FAMILY_LOG("user[%lu] join family level[%u] < %u!!", p_user->get_uid(), p_user->get_level(), need_level);
		return;
	}

	if (p_family->is_member_full()) {
		reply.set_reply_code(errcode_enum::family_err_code_excess_max_member_number);
		FAMILY_LOG("user[%lu] family[%lu] member full", p_user->get_uid(), family_id);
		return;
	}

// 	if (p_family->get_country_id() != p_user->get_country_id())
// 	{
// 		FAMILY_LOG("user[%lu] country[%u] != family[%lu] country[%u]", p_user->get_uid(), p_user->get_country_id(), family_id, p_family->get_country_id());
// 		reply.set_reply_code(errcode_enum::family_err_code_not_in_same_country);
// 		return;
// 	}

	const family_member_ptr& p_family_member = p_family->get_family_member(p_user->get_uid());
	if (NULL != p_family_member)
	{
		FAMILY_LOG("user[%lu] is already in family[%lu]", p_user->get_uid(), family_id);
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	if (p_family->is_auto_newbie(p_user->get_level()))
	{
		// 加入家族
		if (!p_family->join_in(p_user, family_officer_type_newbie))
		{
			reply.set_reply_code(errcode_enum::notice_unknown);
			return;
		}

		// 如果在申请列表中就删除
		p_family->del_from_apply_list(p_user->get_uid());

		reply.set_member_type(proto::common::family_member_type_newbie);

		FAMILY_LOG("user[%lu] join family[%lu] success", p_user->get_uid(), family_id);

		WRITE_FAMILY_LOG_TO_DB(family_id, p_user->get_uid(), 0, common::log_enum::source_type_family_auto_join, 0);
	}
	else
	{
		if (!p_family->add_appliy_list(p_user->get_uid()))
		{
			FAMILY_LOG("family[%lu] apply list is full", family_id);
			reply.set_reply_code(common::errcode_enum::family_err_code_excess_max_apply_number);
			return;
		}

		reply.set_member_type(proto::common::family_member_type_applay);

		FAMILY_LOG("user[%lu] add family[%lu] apply list success", p_user->get_uid(), family_id);

		WRITE_FAMILY_LOG_TO_DB(family_id, p_user->get_uid(), 0, common::log_enum::source_type_family_join_apply_list, 0);
	}

	reply.set_id(common::string_util_t::uint64_to_string(family_id));
	reply.set_reply_code(common::errcode_enum::error_ok);
}

void family_manager_t::decline_join_family(const global_user_ptr& p_user, uint64_t uid, proto::client::ec_decline_join_family_reply& reply)
{
	if (NULL == p_user)
	{
		reply.set_reply_code(errcode_enum::notice_unknown);
		log_error("null == p_user");
		return;
	}

	const family_ptr& p_family = get_family(p_user->get_family_id());
	if (NULL == p_family)
	{
		reply.set_reply_code(errcode_enum::family_err_code_not_join_family);
		log_error("user[%lu] family[%lu] not find", p_user->get_uid(), p_user->get_family_id());
		return;
	}

	if (!has_family_privilige(p_user->get_family_officer(), privilige_type_family_member_audit))
	{
		log_error("user[%lu] has no right to audit member", p_user->get_uid());
		reply.set_reply_code(common::errcode_enum::family_err_code_no_right);
		return;
	}

	if (!p_family->decline_apply_user(uid))
	{
		log_error("user[%lu] decline apply user[%lu] failed", p_user->get_uid(), uid);
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	reply.set_role_id(common::string_util_t::uint64_to_string(uid));
	reply.set_reply_code(common::errcode_enum::error_ok);

	WRITE_FAMILY_LOG_TO_DB(p_user->get_family_id(), p_user->get_uid(), uid, common::log_enum::source_type_family_decline_join, 0);

	FAMILY_LOG("user[%lu] decline_join user[%lu]", p_user->get_uid(), uid);
}

void family_manager_t::one_key_decline_join_family(const global_user_ptr& p_user, proto::client::ec_one_key_decline_join_family_reply& reply)
{
	if (NULL == p_user)
	{
		reply.set_reply_code(errcode_enum::notice_unknown);
		log_error("null == p_user");
		return;
	}

	const family_ptr& p_family = get_family(p_user->get_family_id());
	if (NULL == p_family)
	{
		reply.set_reply_code(errcode_enum::family_err_code_not_join_family);
		log_error("user[%lu] family[%lu] not find", p_user->get_uid(), p_user->get_family_id());
		return;
	}

	if (!has_family_privilige(p_user->get_family_officer(), privilige_type_family_member_audit))
	{
		log_error("user[%lu] has no right to audit member", p_user->get_uid());
		reply.set_reply_code(common::errcode_enum::family_err_code_no_right);
		return;
	}

	p_family->decline_all_apply_user();

	reply.set_reply_code(common::errcode_enum::error_ok);

	WRITE_FAMILY_LOG_TO_DB(p_user->get_family_id(), p_user->get_uid(), 0, common::log_enum::source_type_family_decline_all_join, 0);

	FAMILY_LOG("user[%lu] decline_join all users", p_user->get_uid());
}

void family_manager_t::approve_join_family(const global_user_ptr& p_user, uint64_t uid, proto::client::ec_approve_join_family_reply& reply)
{
	if (NULL == p_user)
	{
		reply.set_reply_code(errcode_enum::notice_unknown);
		log_error("null == p_user");
		return;
	}

	const family_ptr& p_family = get_family(p_user->get_family_id());
	if (NULL == p_family)
	{
		reply.set_reply_code(errcode_enum::family_err_code_not_join_family);
		log_error("user[%lu] family[%lu] not find", p_user->get_uid(), p_user->get_family_id());
		return;
	}

	if (!has_family_privilige(p_user->get_family_officer(), privilige_type_family_member_audit))
	{
		log_error("user[%lu] has no right to audit member", p_user->get_uid());
		reply.set_reply_code(common::errcode_enum::family_err_code_no_right);
		return;
	}

	uint32_t back_code = p_family->approve_join(p_user->get_uid(), uid);
	if (common::errcode_enum::error_ok == back_code)
	{
		FAMILY_LOG("user[%lu] approve user[%lu] join family", p_user->get_uid(), uid);
	}

	reply.set_id(common::string_util_t::uint64_to_string(uid));
	reply.set_reply_code(back_code);
}

uint32_t family_manager_t::reply_family_invite(const global_user_ptr& p_user, const proto::common::family_invite_result& result)
{
	if (NULL == p_user) {
		log_error("null == p_user");
		return common::errcode_enum::notice_unknown;
	}

	uint64_t family_uid = common::string_util_t::string_to_uint64(result.family_uid());
	uint64_t inviter_uid = common::string_util_t::string_to_uint64(result.inviter_uid());

	const family_ptr& p_family = get_family(family_uid);
	if (NULL == p_family) {
		log_error("user[%lu] join family[%lu] not find", p_user->get_uid(), family_uid);
		return common::errcode_enum::family_err_code_invalid_family;
	}

	if (result.result() == 0) { // 拒绝
		p_family->del_from_invite_list(p_user->get_uid());

		global_user_ptr p_inviter = global_user_manager_t::get_global_user_by_uid(inviter_uid);
		if (p_inviter && p_inviter->is_online()) { // 通知邀请人
			proto::client::ec_family_invite_refuse_notify notify;
			notify.set_inviter_name(p_user->get_name());
			p_inviter->send_msg_to_client(op_cmd::ec_family_invite_refuse_notify, notify);
		}
	} else { // 同意
		return p_family->agree_invite(p_user, inviter_uid);
	}

	return common::errcode_enum::error_ok;
}

void family_manager_t::one_key_approve_join_family(const global_user_ptr& p_user, proto::client::ec_one_key_approve_join_family_reply& reply)
{
	if (NULL == p_user)
	{
		reply.set_reply_code(errcode_enum::notice_unknown);
		log_error("null == p_user");
		return;
	}

	const family_ptr& p_family = get_family(p_user->get_family_id());
	if (NULL == p_family)
	{
		reply.set_reply_code(errcode_enum::family_err_code_not_join_family);
		log_error("user[%lu] family[%lu] not find", p_user->get_uid(), p_user->get_family_id());
		return;
	}

	if (!has_family_privilige(p_user->get_family_officer(), privilige_type_family_member_audit))
	{
		log_error("user[%lu] has no right to audit member", p_user->get_uid());
		reply.set_reply_code(common::errcode_enum::family_err_code_no_right);
		return;
	}

	uint32_t back_code = p_family->approve_all_apply_user(p_user->get_uid(), reply);
	reply.set_reply_code(back_code);
}

void family_manager_t::leave_family(const global_user_ptr& p_user, proto::client::ec_leave_family_reply& reply)
{
	if (NULL == p_user)
	{
		reply.set_reply_code(errcode_enum::notice_unknown);
		log_error("null == p_user");
		return;
	}

	const family_ptr& p_family = get_family(p_user->get_family_id());
	if (NULL == p_family)
	{
		reply.set_reply_code(errcode_enum::family_err_code_invalid_family);
		log_error("user[%lu] family[%lu] not find", p_user->get_uid(), p_user->get_family_id());
		return;
	}

	if (family_manager_t::is_family_war_member(p_family->get_family_id())) {
		reply.set_reply_code(errcode_enum::family_err_war_can_not_quit);
		log_error("user[%lu] family[%lu] is family war member", p_user->get_uid(), p_user->get_family_id());
		return;
	}

	if (p_user->get_family_officer() == family_officer_type_general && p_family->get_member_size() > 1)
	{
		log_error("general[%lu] can't leave family", p_user->get_uid());
		reply.set_reply_code(common::errcode_enum::family_err_general_level_family);
		return;
	}

	if (!p_family->leave_out(p_user))
	{
		FAMILY_LOG("user[%lu] leave family failed", p_user->get_uid());
		reply.set_reply_code(errcode_enum::notice_unknown);
	}
	else
	{
		FAMILY_LOG("user[%lu] leave family success", p_user->get_uid());
		reply.set_reply_code(errcode_enum::error_ok);

		WRITE_FAMILY_LOG_TO_DB(p_family->get_family_id(), p_user->get_uid(), 0, common::log_enum::source_type_family_leave, 0);

		// 检测家族中是否还有人
		if (p_family->get_member_size() == 0)
		{
			if (p_family->get_country_id() != 0) {
				country_ptr p_country = country_mgr_t::get_country(p_family->get_country_id());
				if (p_country) {
					p_country->del_family(p_family, true);
				}
			} else {
				if (p_family->get_king_war_apply_country() != 0) {
					country_ptr p_country = country_mgr_t::get_country(p_family->get_king_war_apply_country());
					if (p_country) {
						p_country->del_family_king_war_clear(p_family);
					}
				}

				if (p_family->get_king_war_fight_country() != 0) {
					country_ptr p_country = country_mgr_t::get_country(p_family->get_king_war_fight_country());
					if (p_country) {
						p_country->del_family_king_war_clear(p_family);
					}
				}
			}

			family_manager_t::del_family_when_no_member(p_family->get_family_id());
		}
	}
}

uint32_t family_manager_t::assign_member(const global_user_ptr& p_user, uint64_t uid, uint32_t officer)
{
	if (NULL == p_user)
	{
		log_error("null == p_user");
		return errcode_enum::notice_unknown;
	}

	const family_ptr& p_family = get_family(p_user->get_family_id());
	if (NULL == p_family)
	{
		log_error("user[%lu] family[%lu] not find", p_user->get_uid(), p_user->get_family_id());
		return errcode_enum::family_err_code_invalid_family;
	}

	uint32_t user_officer = p_user->get_family_officer();

	switch (officer)
	{
	case family_officer_type_vice_general:
		if (!has_family_privilige(user_officer, privilige_type_family_assign_vice_general))
		{
			FAMILY_LOG("user[%lu] has no privilige to assign vice general", p_user->get_uid());
			return common::errcode_enum::family_err_code_no_right;
		}
		break;
	case family_officer_type_elit:
		if (!has_family_privilige(user_officer, privilige_type_family_assign_elite))
		{
			FAMILY_LOG("user[%lu] has no privilige to assign elite", p_user->get_uid());
			return common::errcode_enum::family_err_code_no_right;
		}
		break;
	case family_officer_type_member:
		if (!has_family_privilige(user_officer, privilige_type_family_accept_member))
		{
			FAMILY_LOG("user[%lu] has no privilige to accept member", p_user->get_uid());
			return common::errcode_enum::family_err_code_no_right;
		}
		break;
	default:
		log_error("no such family officer[%u]", officer);
		return common::errcode_enum::notice_unknown;
		break;
	}

	if (country_mgr_t::get_king_war_state() >= proto::common::king_war_state_prepare) {
		log_error("user[%lu] king war state", p_user->get_uid());
		return errcode_enum::king_war_error_family_can_not_assign;
	}

	uint32_t officer_max = 0;

	FamilyOfficial* p_conf = GET_CONF(FamilyOfficial, officer);
	if (NULL != p_conf)
	{
		officer_max = (uint32_t)p_conf->player_cnt();
	}

	if (officer_max != 0 && p_family->get_officer_count(officer) >= officer_max)
	{
		FAMILY_LOG("family[%lu] officer[%u] count max size", p_user->get_family_id(), officer);
		return common::errcode_enum::family_err_code_excess_max_officer_number;
	}

	return p_family->assign_member(p_user->get_uid(), uid, officer);
}

uint32_t family_manager_t::family_demise(const global_user_ptr& p_user, uint64_t uid)
{
	if (NULL == p_user) {
		log_error("null == p_user");
		return errcode_enum::notice_unknown;
	}

	const family_ptr& p_family = get_family(p_user->get_family_id());
	if (NULL == p_family) {
		log_error("user[%lu] family[%lu] not find", p_user->get_uid(), p_user->get_family_id());
		return errcode_enum::family_err_code_invalid_family;
	}

	if (p_family->get_general_id() != p_user->get_uid()) {
		log_error("user[%lu] is not family general", p_user->get_uid());
		return common::errcode_enum::family_err_code_no_right;
	}

	if (country_mgr_t::get_king_war_state() >= proto::common::king_war_state_prepare) {
		log_error("user[%lu] king war state", p_user->get_uid());
		return errcode_enum::king_war_error_family_can_not_assign;
	}

	return p_family->family_demise(p_user, uid);
}

uint32_t family_manager_t::fire_member(const global_user_ptr& p_user, uint64_t uid)
{
	if (NULL == p_user)
	{
		log_error("null == p_user");
		return errcode_enum::notice_unknown;
	}

	const family_ptr& p_family = get_family(p_user->get_family_id());
	if (NULL == p_family)
	{
		log_error("user[%lu] family[%lu] not find", p_user->get_uid(), p_user->get_family_id());
		return errcode_enum::family_err_code_invalid_family;
	}

	const family_member_ptr& p_family_member = p_family->get_family_member(uid);
	if (NULL == p_family_member)
	{
		log_error("family[%lu] have no member[%lu]", p_family->get_family_id(), uid);
		return common::errcode_enum::notice_unknown;
	}

	uint32_t user_officer = p_user->get_family_officer();
	uint32_t member_officer = p_family_member->get_officer();

	switch (member_officer)
	{
	case family_officer_type_vice_general:
		if (!has_family_privilige(user_officer, privilige_type_family_assign_vice_general))
		{
			FAMILY_LOG("user[%lu] has no privilige to assign vice general", p_user->get_uid());
			return common::errcode_enum::family_err_code_no_right;
		}
		break;
	case family_officer_type_elit:
		if (!has_family_privilige(user_officer, privilige_type_family_assign_elite))
		{
			FAMILY_LOG("user[%lu] has no privilige to assign elite", p_user->get_uid());
			return common::errcode_enum::family_err_code_no_right;
		}
		break;
	default:
		log_error("can't fire family officer[%u]", member_officer);
		return common::errcode_enum::notice_unknown;
		break;
	}

	return p_family->assign_member(p_user->get_uid(), uid, family_officer_type_member);
}

uint32_t family_manager_t::kick_member(const global_user_ptr& p_user, uint64_t uid)
{
	if (NULL == p_user)
	{
		log_error("null == p_user");
		return errcode_enum::notice_unknown;
	}

	const family_ptr& p_family = get_family(p_user->get_family_id());
	if (NULL == p_family)
	{
		log_error("user[%lu] family[%lu] not find", p_user->get_uid(), p_user->get_family_id());
		return errcode_enum::family_err_code_invalid_family;
	}

	if (family_manager_t::is_family_war_member(p_family->get_family_id())) {
		log_error("user[%lu] family[%lu] is family war member", p_user->get_uid(), p_user->get_family_id());
		return common::errcode_enum::family_err_war_can_not_kick;
	}

	const family_member_ptr& p_family_member = p_family->get_family_member(uid);
	if (NULL == p_family_member)
	{
		log_error("family[%lu] have no member[%lu]", p_family->get_family_id(), uid);
		return common::errcode_enum::notice_unknown;
	}

	uint32_t user_officer = p_user->get_family_officer();

	if (!has_family_privilige(user_officer, privilige_type_family_remove_member))
	{
		FAMILY_LOG("user[%lu] has no privilige remove member", p_user->get_uid());
		return common::errcode_enum::family_err_code_no_right;
	}

	// 踢的人是官员
	uint32_t member_officer = p_family_member->get_officer();
	switch (member_officer)
	{
	case family_officer_type_vice_general:
		if (!has_family_privilige(user_officer, privilige_type_family_assign_vice_general))
		{
			FAMILY_LOG("user[%lu] has no privilige to assign vice general", p_user->get_uid());
			return common::errcode_enum::family_err_code_no_right;
		}
		break;
	case family_officer_type_elit:
		if (!has_family_privilige(user_officer, privilige_type_family_assign_elite))
		{
			FAMILY_LOG("user[%lu] has no privilige to assign elite", p_user->get_uid());
			return common::errcode_enum::family_err_code_no_right;
		}
		break;
	}

	const global_user_ptr& p_kick_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL != p_kick_user)
	{
		if (!p_family->leave_out(p_kick_user))
		{
			FAMILY_LOG("user[%lu] leave family failed", p_user->get_uid());
			return errcode_enum::notice_unknown;
		}

		proto::client::ec_family_id_notify ntf;
		ntf.set_family_id("0");
		p_kick_user->send_msg_to_client(op_cmd::ec_family_id_notify, ntf);
	}
	else
	{
		if (!p_family->leave_out(uid))
		{
			FAMILY_LOG("user[%lu] leave family failed", p_user->get_uid());
			return errcode_enum::notice_unknown;
		}
	}

	FAMILY_LOG("user[%lu] family[%lu] kick user[%lu] success", p_user->get_uid(), p_user->get_family_id(), uid);

	WRITE_FAMILY_LOG_TO_DB(p_user->get_family_id(), p_user->get_uid(), uid, common::log_enum::source_type_family_kick, 0);

	return errcode_enum::error_ok;
}

void family_manager_t::get_family_list(uint32_t page, proto::client::ec_get_family_list_reply& reply)
{
	uint32_t family_page_count = m_family_sort_vec.size() / FAMILY_PAGE_COUNT;
	if (page > family_page_count)
	{
		reply.set_reply_code(errcode_enum::family_err_code_list_max);
		return;
	}

	for (uint32_t i = page * FAMILY_PAGE_COUNT; i < m_family_sort_vec.size() && i < (page + 1) * FAMILY_PAGE_COUNT; ++i)
	{
		const family_ptr& p_family = get_family(m_family_sort_vec[i].m_family_id);
		if (NULL != p_family)
		{
			p_family->peek_data(reply.add_family_list());
		}
	}

	reply.set_page(page);
	reply.set_reply_code(errcode_enum::error_ok);
}

void family_manager_t::get_help_family_list(uint32_t page, const std::set<uint64_t>& invited_list, const std::set<uint64_t>& redused_list, proto::client::ec_king_war_help_family_list_reply& reply)
{
	uint32_t family_page_count = m_family_sort_vec.size() / FAMILY_PAGE_COUNT;
	if (page > family_page_count) {
		reply.set_reply_code(errcode_enum::family_err_code_list_max);
		return;
	}

	auto conf = GET_CONF(Comprehensive, comprehensive_common::king_signup_help_cond1);
	uint32_t limit_lv = GET_COMPREHENSIVE_VALUE_1(conf);

	for (uint32_t i = page * FAMILY_PAGE_COUNT; i < m_family_sort_vec.size() && i < (page + 1) * FAMILY_PAGE_COUNT; ++i) {
		const family_ptr& p_family = get_family(m_family_sort_vec[i].m_family_id);
		if (NULL == p_family) continue;

		if (p_family->get_level() < limit_lv) break;

		proto::common::king_war_help_family* p_help_family = reply.add_family_list();
		if (p_help_family) {
			p_family->peek_data(p_help_family->mutable_info());

			if (p_family->get_king_war_fight_country() != 0) {
				p_help_family->set_state(proto::common::king_war_help_fighted);
			} else if (invited_list.find(p_family->get_family_id()) != invited_list.end()) {
				p_help_family->set_state(proto::common::king_war_help_invited);
			} else if (redused_list.find(p_family->get_family_id()) != redused_list.end()) {
				p_help_family->set_state(proto::common::king_war_help_refused);
			} else {
				p_help_family->set_state(proto::common::king_war_help_none);
			}
		}
	}

	if (reply.family_list_size() == 0) {
		reply.set_reply_code(errcode_enum::family_err_code_list_max);
		return;
	}

	reply.set_page(page);
	reply.set_reply_code(errcode_enum::error_ok);
}

void family_manager_t::get_family_fuzzy_query(const std::string& name, proto::client::ec_family_fuzzy_query_reply& reply)
{
	if (name.empty())
	{
		log_error("family fuzzy query name is empty");
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	for (family_sort_vec::const_iterator citr = m_family_sort_vec.begin();
		citr != m_family_sort_vec.end(); ++citr)
	{
		const family_sort_t& family_sort = *citr;

		// 模糊搜索，如果家族很多，需要优化
		if (family_sort.m_family_name.find(name) != std::string::npos)
		{
			const family_ptr& p_family = get_family(family_sort.m_family_id);
			if (NULL != p_family)
			{
				p_family->peek_data(reply.add_family_list());
			}
		}
	}

	reply.set_reply_code(errcode_enum::error_ok);
}

void family_manager_t::get_help_family_fuzzy_list(const std::string& name, const std::set<uint64_t>& invited_list, const std::set<uint64_t>& redused_list, proto::client::ec_king_war_help_family_fuzzy_reply& reply)
{
	if (name.empty()) {
		log_error("family fuzzy query name is empty");
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	for (family_sort_vec::const_iterator citr = m_family_sort_vec.begin();
		citr != m_family_sort_vec.end(); ++citr) {
		const family_sort_t& family_sort = *citr;
		// 模糊搜索，如果家族很多，需要优化
		if (family_sort.m_family_name.find(name) != std::string::npos) {
			const family_ptr& p_family = get_family(family_sort.m_family_id);
			if (NULL != p_family) {
				proto::common::king_war_help_family* p_help_family = reply.add_family_list();
				if (p_help_family) {
					p_family->peek_data(p_help_family->mutable_info());

					if (p_family->get_king_war_fight_country() != 0) {
						p_help_family->set_state(proto::common::king_war_help_fighted);
					} else if (invited_list.find(p_family->get_family_id()) != invited_list.end()) {
						p_help_family->set_state(proto::common::king_war_help_invited);
					} else if (redused_list.find(p_family->get_family_id()) != redused_list.end()) {
						p_help_family->set_state(proto::common::king_war_help_refused);
					} else {
						p_help_family->set_state(proto::common::king_war_help_none);
					}
				}
			}
		}
	}

	reply.set_reply_code(errcode_enum::error_ok);
}

void family_manager_t::get_family_info(const global_user_ptr& p_user, proto::client::ec_get_family_info_reply& reply)
{
	if (NULL == p_user)
	{
		log_error("null == p_user");
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	const family_ptr& p_family = get_family(p_user->get_family_id());
	if (NULL == p_family)
	{
		log_error("user[%lu] family[%lu] not find", p_user->get_uid(), p_user->get_family_id());
		reply.set_reply_code(errcode_enum::family_err_code_invalid_family);
		return;
	}

	p_family->get_family_info(reply);
	reply.set_reply_code(errcode_enum::error_ok);
}

void family_manager_t::get_family_member_list(const global_user_ptr& p_user, proto::client::ec_get_family_member_list_reply& reply)
{
	if (NULL == p_user)
	{
		log_error("null == p_user");
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	const family_ptr& p_family = get_family(p_user->get_family_id());
	if (NULL == p_family)
	{
		log_error("user[%lu] family[%lu] not find", p_user->get_uid(), p_user->get_family_id());
		reply.set_reply_code(errcode_enum::family_err_code_invalid_family);
		return;
	}

	p_family->get_family_member_list(reply);
	reply.set_reply_code(errcode_enum::error_ok);
}

void family_manager_t::toggle_auto_newbie(const global_user_ptr& p_user, proto::client::ec_toggle_auto_newbie_reply& reply)
{
	if (NULL == p_user)
	{
		log_error("null == p_user");
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	const family_ptr& p_family = get_family(p_user->get_family_id());
	if (NULL == p_family)
	{
		log_error("user[%lu] family[%lu] not find", p_user->get_uid(), p_user->get_family_id());
		reply.set_reply_code(errcode_enum::family_err_code_invalid_family);
		return;
	}

	if (!has_family_privilige(p_user->get_family_officer(), privilige_type_family_member_audit))
	{
		log_error("user[%lu] has no right to audit member", p_user->get_uid());
		reply.set_reply_code(common::errcode_enum::family_err_code_no_right);
		return;
	}

	p_family->toggle_aoto_newbie();

	reply.set_status(p_family->is_auto_newbie_state() ? 1 : 0);
	reply.set_reply_code(errcode_enum::error_ok);

	WRITE_FAMILY_LOG_TO_DB(p_user->get_family_id(), p_user->get_uid(), 0, common::log_enum::source_type_family_toggle_auto_newbie, p_family->is_auto_newbie_state() ? 1 : 0);
}

void family_manager_t::modify_family_declaration(const global_user_ptr& p_user, const std::string& declaration, proto::client::ec_modify_family_declaration_reply& reply)
{
	if (NULL == p_user)
	{
		log_error("null == p_user");
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	const family_ptr& p_family = get_family(p_user->get_family_id());
	if (NULL == p_family)
	{
		log_error("user[%lu] family[%lu] not find", p_user->get_uid(), p_user->get_family_id());
		reply.set_reply_code(errcode_enum::family_err_code_invalid_family);
		return;
	}

	if (!has_family_privilige(p_user->get_family_officer(), privilige_type_family_modify_declaration))
	{
		log_error("user[%lu] has no right to audit member", p_user->get_uid());
		reply.set_reply_code(common::errcode_enum::family_err_code_no_right);
		return;
	}

	const std::string& new_declaration = p_family->update_family_declaration(declaration);

	reply.set_declaration(new_declaration);
	reply.set_reply_code(errcode_enum::error_ok);

	WRITE_FAMILY_LOG_TO_DB(p_user->get_family_id(), p_user->get_uid(), 0, common::log_enum::source_type_family_modify_declaration, 0);
}

void family_manager_t::modify_family_notification(const global_user_ptr& p_user, const std::string& notification, proto::client::ec_modify_family_notification_reply& reply)
{
	if (NULL == p_user)
	{
		log_error("null == p_user");
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	const family_ptr& p_family = get_family(p_user->get_family_id());
	if (NULL == p_family)
	{
		log_error("user[%lu] family[%lu] not find", p_user->get_uid(), p_user->get_family_id());
		reply.set_reply_code(errcode_enum::family_err_code_invalid_family);
		return;
	}

	if (!has_family_privilige(p_user->get_family_officer(), privilige_type_family_modify_notice))
	{
		log_error("user[%lu] has no right to audit member", p_user->get_uid());
		reply.set_reply_code(common::errcode_enum::family_err_code_no_right);
		return;
	}

	const std::string& new_notification = p_family->update_family_notification(notification);

	reply.set_notification(new_notification);
	reply.set_reply_code(errcode_enum::error_ok);

	WRITE_FAMILY_LOG_TO_DB(p_user->get_family_id(), p_user->get_uid(), 0, common::log_enum::source_type_family_modify_notification, 0);
}

void family_manager_t::get_family_apply_list(const global_user_ptr& p_user, proto::client::ec_family_apply_list_reply& reply)
{
	if (NULL == p_user)
	{
		log_error("null == p_user");
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	const family_ptr& p_family = get_family(p_user->get_family_id());
	if (NULL == p_family)
	{
		log_error("user[%lu] family[%lu] not find", p_user->get_uid(), p_user->get_family_id());
		reply.set_reply_code(errcode_enum::family_err_code_invalid_family);
		return;
	}

	if (!has_family_privilige(p_user->get_family_officer(), privilige_type_family_member_audit))
	{
		log_error("user[%lu] has no right to audit member", p_user->get_uid());
		reply.set_reply_code(common::errcode_enum::family_err_code_no_right);
		return;
	}

	p_family->get_family_apply_list(reply);
	reply.set_reply_code(errcode_enum::error_ok);
}

void family_manager_t::get_family_build_list(const global_user_ptr& p_user, proto::client::ec_get_family_build_info_reply& reply)
{
	if (NULL == p_user)
	{
		log_error("null == p_user");
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	const family_ptr& p_family = get_family(p_user->get_family_id());
	if (NULL == p_family)
	{
		log_error("user[%lu] family[%lu] not find", p_user->get_uid(), p_user->get_family_id());
		reply.set_reply_code(errcode_enum::family_err_code_invalid_family);
		return;
	}

	p_family->get_family_build_list(reply);
	reply.set_reply_code(errcode_enum::error_ok);
}

void family_manager_t::upgrade_family_build(const global_user_ptr& p_user, proto::common::family_build_type type, proto::client::ec_upgrade_family_build_reply& reply)
{
	if (NULL == p_user)
	{
		log_error("null == p_user");
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	const family_ptr& p_family = get_family(p_user->get_family_id());
	if (NULL == p_family)
	{
		log_error("user[%lu] family[%lu] not find", p_user->get_uid(), p_user->get_family_id());
		reply.set_reply_code(errcode_enum::family_err_code_invalid_family);
		return;
	}

	if (!has_family_privilige(p_user->get_family_officer(), privilige_type_family_build))
	{
		log_error("user[%lu] has no right to audit member", p_user->get_uid());
		reply.set_reply_code(common::errcode_enum::family_err_code_no_right);
		return;
	}

	uint32_t back_code = p_family->upgrade_build(type, reply);
	reply.set_reply_code(back_code);

	WRITE_FAMILY_LOG_TO_DB(p_user->get_family_id(), p_user->get_uid(), 0, common::log_enum::source_type_family_upgrade_build, type);
}

void family_manager_t::clear_family_build_cd(const global_user_ptr& p_user, proto::common::family_build_type type, proto::client::ec_clear_family_build_upgrade_cd_reply& reply)
{
	if (NULL == p_user)
	{
		log_error("null == p_user");
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	const family_ptr& p_family = get_family(p_user->get_family_id());
	if (NULL == p_family)
	{
		log_error("user[%lu] family[%lu] not find", p_user->get_uid(), p_user->get_family_id());
		reply.set_reply_code(errcode_enum::family_err_code_invalid_family);
		return;
	}

	if (!has_family_privilige(p_user->get_family_officer(), privilige_type_family_build))
	{
		log_error("user[%lu] has no right to audit member", p_user->get_uid());
		reply.set_reply_code(common::errcode_enum::family_err_code_no_right);
		return;
	}

	uint32_t back_code = p_family->clear_build_cd(type);
	reply.set_type(type);
	reply.set_reply_code(back_code);

	WRITE_FAMILY_LOG_TO_DB(p_user->get_family_id(), p_user->get_uid(), 0, common::log_enum::source_type_family_clear_upgrade_build_cd, type);
}

void family_manager_t::family_update_glory(uint64_t role_uid, uint64_t family_id, uint32_t glory)
{
	family_ptr p_family = get_family(family_id);
	if (NULL == p_family)
	{
		log_error("NULL == p_family[%lu]", family_id);
		return;
	}
	p_family->update_member_glory(role_uid, glory);
}

uint32_t family_manager_t::is_family_name_vaild(const std::string& name)
{
	if (!common::string_util_t::is_no_special_char(name))
	{
		log_error("family_name[%s] has special char!", name.c_str());
		return errcode_enum::sys_notice_not_chinese_word;
	}

	auto conf = GET_CONF(Comprehensive, comprehensive_common::family_name_lower);
	uint32_t name_min = GET_COMPREHENSIVE_VALUE_1(conf);
	conf = GET_CONF(Comprehensive, comprehensive_common::family_name_upper);
	uint32_t name_max = GET_COMPREHENSIVE_VALUE_1(conf);

	uint32_t name_cnt = common::string_util_t::get_char_size(name);
	if (name_cnt < name_min || name_cnt > name_max)
	{
		FAMILY_LOG("create family name invalid:name[%s], size[%u] error", name.c_str(), name_cnt);
		return common::errcode_enum::family_err_code_invalid_name;
	}

	if (family_data_mgr_t::is_family_name_exsit(name))
	{
		FAMILY_LOG("create family name invalid:name[%s], is exsit", name.c_str());
		return common::errcode_enum::family_err_code_name_exsit;
	}

	return common::errcode_enum::error_ok;
}

void family_manager_t::notify_chat_member_change(uint64_t family_id, uint64_t role_uid, uint32_t change_type)
{
	proto::server::eh_family_member_change_notify msg_ntf;
	msg_ntf.set_family_id(family_id);
	msg_ntf.set_change_type(change_type);
	msg_ntf.set_role_uid(role_uid);
	env::server->send_msg_to_chat(op_cmd::eh_family_member_change_notify, role_uid, msg_ntf);
}

bool family_manager_t::has_family_privilige(uint32_t officer, family_privilige_type type)
{
	FamilyOfficial* conf = GET_CONF(FamilyOfficial, officer);
	if (conf && conf->is_power((uint32_t)type))
	{
		return true;
	}

	return false;
}

family_ptr family_manager_t::get_family(uint64_t family_id)
{
	family_map::const_iterator citr = m_family_map.find(family_id);
	if (citr != m_family_map.end())
	{
		return citr->second;
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////

uint32_t family_manager_t::get_week_id(uint32_t t)
{
	uint32_t time_unique_id = time_manager_t::get_time_unique_id(time_type_enum::time_family_war3, t);
	if (time_unique_id != 0) {
		return time_unique_id;
	}

	log_error("family war time[%u] type error", time_type_enum::time_family_war3);
	return 0;
}

uint32_t family_manager_t::get_war_time(uint32_t id)
{
	uint32_t time = time_manager_t::get_time_by_unique_id(time_type_enum::time_family_war3, id);
	if (time != 0) {
		return time;
	}

	log_error("family war time[%u] type error", time_type_enum::time_family_war3);
	return 0;
}

void family_manager_t::update_against_list()
{
	proto::common::family_against_list swap_data;
	m_against_list.Swap(&swap_data);

	for (against_data_map::const_iterator citr = m_against_data_map.begin();
		citr != m_against_data_map.end(); ++citr)
	{
		proto::common::family_against_data* p_data = m_against_list.add_data_list();
		if (NULL != p_data)
		{
			p_data->CopyFrom(citr->second.m_data);
		}
	}
}

void family_manager_t::update_new_history()
{
	proto::common::family_war_history_data new_history_data;

	for (against_data_map::const_iterator citr = m_against_data_map.begin();
		citr != m_against_data_map.end(); ++citr)
	{
		proto::common::family_war_data* left_side_data = new_history_data.add_data();
		if (NULL != left_side_data)
		{
			left_side_data->CopyFrom(citr->second.m_data.left_side());
		}

		proto::common::family_war_data* right_side_data = new_history_data.add_data();
		if (NULL != right_side_data)
		{
			right_side_data->CopyFrom(citr->second.m_data.right_side());
		}
	}

	m_history_deque.push_front(new_history_data);

	if (m_history_deque.size() > FAMILY_WAR_HISTORY_SIZE)
	{
		m_history_deque.pop_back();
	}
}

void family_manager_t::get_family_war_info(proto::client::ec_get_family_war_list_reply& reply)
{
	reply.set_state(m_family_war_info.family_war_data.state());
	proto::common::family_against_list* list = reply.mutable_list();
	if (NULL != list)
	{
		list->CopyFrom(m_against_list);
	}
	reply.set_war_time(m_family_war_info.family_war_time);
}

void family_manager_t::get_history_info(uint32_t index, proto::client::ec_get_family_war_history_reply& reply)
{
	if (index >= FAMILY_WAR_HISTORY_SIZE)
	{
		log_error("history index[%u] error", index);
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	if (index < m_history_deque.size())
	{
		proto::common::family_war_history_data* p_data = reply.mutable_history_data();
		if (NULL != p_data)
		{
			p_data->CopyFrom(m_history_deque[index]);
		}
	}

	reply.set_reply_code(common::errcode_enum::error_ok);
}

uint32_t family_manager_t::ask_enter_family_war_dungeon(const global_user_ptr& p_user)
{
	if (NULL == p_user)
	{
		log_error("p_user null error");
		return common::errcode_enum::notice_unknown;
	}

	if (m_family_war_info.family_war_data.state() <= proto::common::family_war_state_init)
	{
		FAMILY_LOG("family war not in time");
		return common::errcode_enum::family_err_war_not_in_time;
	}

	if (0 == p_user->get_family_id())
	{
		FAMILY_LOG("user[%lu] not join family", p_user->get_uid());
		return common::errcode_enum::family_err_code_not_join_family;
	}

	const family_ptr& p_family = get_family(p_user->get_family_id());
	if (NULL == p_family)
	{
		log_warn("family[%lu] not find", p_user->get_family_id());
		return common::errcode_enum::notice_unknown;
	}

	family_against_id_map::const_iterator citr = m_family_against_id_map.find(p_user->get_family_id());
	if (citr == m_family_against_id_map.end())
	{
		FAMILY_LOG("user[%lu] family not in against list", p_user->get_uid());
		return common::errcode_enum::family_err_war_not_in_list;
	}

	against_data_map::iterator against_citr = m_against_data_map.find(citr->second);
	if (against_citr == m_against_data_map.end())
	{
		log_error("user[%lu] family_war id[%u] not find", p_user->get_uid(), citr->second);
		return common::errcode_enum::notice_unknown;
	}

	if (against_citr->second.m_war_scene_id == 0) {
		if (against_citr->second.m_state == proto::common::family_war_state_end) {
			FAMILY_LOG("user[%lu] family_war early end", p_user->get_uid());
			return common::errcode_enum::family_err_war_early_end;
		} else {
			FAMILY_LOG("user[%lu] family_war state[%u] error", p_user->get_uid(), against_citr->second.m_state);
			return common::errcode_enum::notice_unknown;
		}
	}

	if (against_citr->second.m_state == proto::common::family_war_state_end) { // 提前结束
		FAMILY_LOG("user[%lu] family_war early result", p_user->get_uid());
		return common::errcode_enum::family_err_war_early_result;
	}

	const global_scene_ptr& p_family_dungeon = global_scene_manager_t::get_scene_by_id(against_citr->second.m_war_scene_id);
	if (NULL == p_family_dungeon)
	{
		log_error("user[%lu] family_war dungeon[%lu] not find", p_user->get_uid(), against_citr->second.m_war_scene_id);
		return common::errcode_enum::notice_unknown;
	}

	if (p_family_dungeon->get_scene_state() != SceneState::SCENE_STATE_NORMAL)
	{
		FAMILY_LOG("user[%lu] family_war dungeon[%lu] scene_state[%u] error", p_user->get_uid(), against_citr->second.m_war_scene_id, p_family_dungeon->get_scene_state());
		return common::errcode_enum::family_err_war_early_result;
	}

	if (p_user->get_troop_id() > 0)
	{
		FAMILY_LOG("user[%lu] troop can't enter family war scene", p_user->get_uid());
		return common::errcode_enum::family_err_war_troop_can_not_enter;
	}

	proto::server::eg_enter_family_war_scene_request request;
	request.set_uid(p_user->get_uid());
	request.set_family_dungeon_id(p_family_dungeon->get_scene_id());
	p_user->send_msg_to_game(op_cmd::eg_enter_family_war_scene_request, request);

	return common::errcode_enum::error_ok;
}

bool family_manager_t::is_family_war_member(uint64_t family_id)
{
	if (m_family_war_info.family_war_data.state() > proto::common::family_war_state_init) {
		family_against_id_map::const_iterator against_citr = m_family_against_id_map.find(family_id);
		if (against_citr != m_family_against_id_map.end()) {
			FAMILY_LOG("family[%lu] is family war member", family_id);
			return true;
		}
	}

	return false;
}

void family_manager_t::family_war_early_end(uint64_t win_family_id)
{
	// 胜利方加家族资金
	family_ptr p_win_family = family_manager_t::get_family(win_family_id);
	if (p_win_family) {
		Comprehensive* conf = GET_CONF(Comprehensive, comprehensive_common::family_war_victory_family_money);
		uint32_t money = GET_COMPREHENSIVE_VALUE_1(conf);
		p_win_family->add_money(money, proto::server::family_money_family_war_win);
	}
}

void family_manager_t::family_war_early_result(const proto::server::family_war_result_data& data)
{
	against_data_map::iterator itr = m_against_data_map.find(data.id());
	if (itr == m_against_data_map.end())
	{
		log_warn("family war[%u] not find", data.id());
		return;
	}

	// 设置结束
	itr->second.m_state = proto::common::family_war_state_end;

	uint64_t win_family_id = data.win_family_id();
	uint64_t lose_family = 0;
	proto::common::family_war_data* p_left_data = itr->second.m_data.mutable_left_side();
	if (p_left_data)
	{
		p_left_data->set_family_score(data.left_family_score());

		uint64_t left_family = common::string_util_t::string_to_uint64(p_left_data->family_id());
		if (left_family == win_family_id)
		{
			p_left_data->set_result(proto::common::family_war_result_win);
		}
		else
		{
			p_left_data->set_result(proto::common::family_war_result_failure);
			lose_family = left_family;
		}
	}

	proto::common::family_war_data* p_right_data = itr->second.m_data.mutable_right_side();
	if (p_right_data)
	{
		p_right_data->set_family_score(data.right_family_score());

		uint64_t right_family = common::string_util_t::string_to_uint64(p_right_data->family_id());
		if (right_family == win_family_id)
		{
			p_right_data->set_result(proto::common::family_war_result_win);
		}
		else
		{
			p_right_data->set_result(proto::common::family_war_result_failure);
			lose_family = right_family;
		}
	}

	// 胜利方加家族资金
	family_ptr p_win_family = family_manager_t::get_family(win_family_id);
	if (p_win_family) {
		Comprehensive* conf = GET_CONF(Comprehensive, comprehensive_common::family_war_victory_family_money);
		uint32_t money = GET_COMPREHENSIVE_VALUE_1(conf);
		p_win_family->add_money(money, proto::server::family_money_family_war_win);
	}

	// 失败方加家族资金
	family_ptr p_lose_family = family_manager_t::get_family(lose_family);
	if (p_lose_family) {
		Comprehensive* conf = GET_CONF(Comprehensive, comprehensive_common::family_war_defeat_family_money);
		uint32_t money = GET_COMPREHENSIVE_VALUE_1(conf);
		p_lose_family->add_money(money, proto::server::family_money_family_war_lose);
	}

	Comprehensive* p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_victory_reward);
	std::map<uint32_t, uint32_t> win_rwd;
	if (p_conf) {
		common::Misc::Parse2SeperatorDataInfo(std::string(p_conf->parameter1()), win_rwd, "$");
	}

	p_conf = GET_CONF(Comprehensive, comprehensive_common::family_war_defeat_reward);
	std::map<uint32_t, uint32_t> lose_rwd;
	if (p_conf) {
		common::Misc::Parse2SeperatorDataInfo(std::string(p_conf->parameter1()), lose_rwd, "$");
	}

	std::map<uint32_t, std::vector<uint64_t>> rank_rwd;
	std::map<uint32_t, std::vector<uint64_t>> exploit_rwd;

	for (int32_t i = 0; i < data.all_roles_size(); ++i) {
		const proto::server::family_war_personal& personal = data.all_roles(i);
		uint64_t family_id = 0;
		global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(personal.uid());
		if (p_user) {
			family_id = p_user->get_family_id();
		} else {
			family_id = global_user_data_mgr_t::get_user_family_id(personal.uid());
		}

		if (family_id != 0) {
			if (win_family_id == family_id) {
				mail_manager_t::send_mail(personal.uid(), proto::common::MAIL_TYPE_SYS, common::mailcode_enum::sys_mail_family_war_victory_reward, win_rwd, proto::common::MAIL_GET_TYPE_FAMILY_WAR);
			} else if (lose_family == family_id) {
				mail_manager_t::send_mail(personal.uid(), proto::common::MAIL_TYPE_SYS, common::mailcode_enum::sys_mail_family_war_defeat_reward, lose_rwd, proto::common::MAIL_GET_TYPE_FAMILY_WAR);
			}

			if (personal.rank_level() != 0) {
				rank_rwd[personal.rank_level()].push_back(personal.uid());
			}

			if (personal.exploit_level() != 0) {
				exploit_rwd[personal.exploit_level()].push_back(personal.uid());
			}
		}
	}

	MailTable* p_family_merit_rank = GET_CONF(MailTable, common::mailcode_enum::sys_mail_family_war_merit_rank);
	if (p_family_merit_rank) {
		for (std::map<uint32_t, std::vector<uint64_t>>::const_iterator citr = rank_rwd.begin();
			citr != rank_rwd.end(); ++citr) {
			FamilyWarRank* p_FamilyWarRank = GET_CONF(FamilyWarRank, citr->first);
			if (!p_FamilyWarRank) {
				log_error("family war rank reward id[%u] not find", citr->first);
				continue;
			}

			std::string content_text;
			StringTool::format(content_text, common::string_util_t::convert_to_utf8(p_family_merit_rank->content()).c_str(), citr->first);

			mail_manager_t::send_mail(citr->second, proto::common::MAIL_TYPE_SYS, 0, common::string_util_t::convert_to_utf8(p_family_merit_rank->from_name()), common::string_util_t::convert_to_utf8(p_family_merit_rank->title()), content_text,
				p_FamilyWarRank->reward(), proto::common::MAIL_GET_TYPE_FAMILY_WAR);
		}
	}

	MailTable* p_family_merit_level = GET_CONF(MailTable, common::mailcode_enum::sys_mail_family_war_merit_level);
	if (p_family_merit_level) {
		for (std::map<uint32_t, std::vector<uint64_t>>::const_iterator citr = exploit_rwd.begin();
			citr != exploit_rwd.end(); ++citr) {
			FamilyWarPersonal* p_FamilyWarPersonal = GET_CONF(FamilyWarPersonal, citr->first);
			if (!p_FamilyWarPersonal) {
				log_error("family war rank reward personal id[%u] not find", citr->first);
				continue;
			}

			std::string content_text;
			StringTool::format(content_text, common::string_util_t::convert_to_utf8(p_family_merit_level->content()).c_str(), citr->first);

			mail_manager_t::send_mail(citr->second, proto::common::MAIL_TYPE_SYS, 0, common::string_util_t::convert_to_utf8(p_family_merit_level->from_name()), common::string_util_t::convert_to_utf8(p_family_merit_level->title()), content_text,
				p_FamilyWarPersonal->reward(), proto::common::MAIL_GET_TYPE_FAMILY_WAR);
		}
	}


	proto::client::ec_family_war_state_notify ntf;
	ntf.set_state(proto::common::family_war_state_end);
	
	if (p_win_family) {
		p_win_family->send_msg_to_family(op_cmd::ec_family_war_state_notify, ntf);
	}

	if (p_lose_family) {
		p_lose_family->send_msg_to_family(op_cmd::ec_family_war_state_notify, ntf);
	}
}

void family_manager_t::family_war_result(const proto::server::ge_family_war_result_notify& results)
{
	for (int32_t i = 0; i < results.datas_size(); ++i)
	{
		family_war_early_result(results.datas(i));
	}

	rank_war_list();

	update_new_history();
}

void family_manager_t::world_trend_info_family_war(const global_user_ptr& p_user, proto::client::ec_world_trend_info_reply& reply)
{
	if (NULL == p_user) {
		log_error("p_user null error");
		return;
	}

	if (m_family_war_info.family_war_data.state() < proto::common::family_war_state_init) {
		return;
	}

	if (0 == p_user->get_family_id()) {
		return;
	}

	const family_ptr& p_family = get_family(p_user->get_family_id());
	if (NULL == p_family) {
		log_warn("user[%lu] family[%lu] not find", p_user->get_uid(), p_user->get_family_id());
		return;
	}

	family_against_id_map::const_iterator citr = m_family_against_id_map.find(p_user->get_family_id());
	if (citr == m_family_against_id_map.end()) {
		return;
	}

	against_data_map::iterator against_citr = m_against_data_map.find(citr->second);
	if (against_citr == m_against_data_map.end()) {
		log_error("user[%lu] family_war id[%u] not find", p_user->get_uid(), citr->second);
		return;
	}

	if (against_citr->second.m_state < proto::common::family_war_state_init) { // 提前结束
		return;
	}

	proto::common::family_war_info* p_war_info = reply.mutable_family_war();
	if (p_war_info) {
		p_war_info->set_state(m_family_war_info.family_war_data.state());
		p_war_info->set_war_time(m_family_war_info.family_war_time);

		proto::common::family_against_data* p_data = p_war_info->mutable_data();
		if (p_data) {
			p_data->CopyFrom(against_citr->second.m_data);
		}
	}
}

void family_manager_t::user_online_family_war_notify(const global_user_ptr& p_user)
{
	if (NULL == p_user) {
		log_error("p_user null error");
		return;
	}

	if (m_family_war_info.family_war_data.state() <= proto::common::family_war_state_init) {
		return;
	}

	if (0 == p_user->get_family_id()) {
		return;
	}

	family_against_id_map::const_iterator citr = m_family_against_id_map.find(p_user->get_family_id());
	if (citr == m_family_against_id_map.end()) {
		return;
	}

	against_data_map::iterator against_citr = m_against_data_map.find(citr->second);
	if (against_citr == m_against_data_map.end()) {
		log_error("user[%lu] family_war id[%u] not find", p_user->get_uid(), citr->second);
		return;
	}

	if (against_citr->second.m_war_scene_id != 0 && against_citr->second.m_state > proto::common::family_war_state_end) {
		proto::client::ec_family_war_state_notify ntf;
		ntf.set_state(m_family_war_info.family_war_data.state());
		ntf.set_war_time(m_family_war_info.family_war_time);
		p_user->send_msg_to_client(op_cmd::ec_family_war_state_notify, ntf);
	}
}

bool family_manager_t::generate_war_list(uint32_t week_id)
{
	m_against_data_map.clear();
	m_family_against_id_map.clear();

	auto conf = GET_CONF(Comprehensive, comprehensive_common::family_war_level);
	uint32_t level = GET_COMPREHENSIVE_VALUE_1(conf);

	conf = GET_CONF(Comprehensive, comprehensive_common::family_war_prosperity);
	uint32_t prosperity = GET_COMPREHENSIVE_VALUE_1(conf);

	conf = GET_CONF(Comprehensive, comprehensive_common::family_war_member_lv);
	uint32_t member_lv = GET_COMPREHENSIVE_VALUE_1(conf);

	conf = GET_CONF(Comprehensive, comprehensive_common::family_war_member_cnt);
	uint32_t member_cnt = GET_COMPREHENSIVE_VALUE_1(conf);

	conf = GET_CONF(Comprehensive, comprehensive_common::family_war_create_time);
	uint32_t create_time = GET_COMPREHENSIVE_VALUE_1(conf);

	uint32_t now_time = common::time_util_t::now_time();

	std::vector<uint64_t> war_list;

	// 本生m_family_sort_vec已经是按威望排序了
	for (family_sort_vec::const_iterator citr = m_family_sort_vec.begin();
		citr != m_family_sort_vec.end(); ++citr)
	{
		const family_sort_t& family_sort_data = *citr;

		if (family_sort_data.m_family_lv < level)
			continue;

		const family_ptr& p_family = get_family(family_sort_data.m_family_id);
		if (NULL == p_family)
		{
			log_error("family[%lu] not find when generate war list", family_sort_data.m_family_id);
			continue;
		}

		if (p_family->get_prosperity() < prosperity)
			continue;

		if (p_family->get_family_war_member_cnt(member_lv) < member_cnt)
			continue;

		if (p_family->get_create_time() + create_time > now_time)
			continue;

		war_list.push_back(family_sort_data.m_family_id);
	}

	// 检查家族数量
	if (war_list.size() > FAMILY_WAR_MAX_COUNT)
	{
		war_list.erase(war_list.begin() + FAMILY_WAR_MAX_COUNT, war_list.end());
	}

	// 查看是否是奇数，如果是去掉最后一个
	if (war_list.size() % 2 == 1)
	{
		war_list.pop_back();
	}

	// 开始两两随机配对
	FAMILY_LOG("family war generate_war_list size:%u", (uint32_t)war_list.size());

	// 每一组池子的个数
	conf = GET_CONF(Comprehensive, comprehensive_common::family_war_group_cnt);
	uint32_t group_cnt = GET_COMPREHENSIVE_VALUE_1(conf);

	// 根据互相换数据 给每个池子里的id互相匹配
	for (uint32_t i = 0; i < war_list.size(); i += group_cnt)
	{
		std::vector<uint64_t>::iterator ibegin = war_list.begin() + i;
		uint32_t end = i + group_cnt;
		if (end > war_list.size()) end = war_list.size();
		std::vector<uint64_t>::iterator iend = war_list.begin() + end;
		std::random_shuffle(ibegin, iend);
	}

	// 编号
	uint32_t index = 1;
	for (uint32_t i = 0; i < war_list.size(); i += 2)
	{
		uint64_t left_family_id = war_list[i];
		const family_ptr& p_family_left = get_family(left_family_id);
		if (NULL != p_family_left)
		{
			m_family_against_id_map.insert(std::make_pair(left_family_id, index));

			proto::common::family_war_data* p_left_data = m_against_data_map[index].m_data.mutable_left_side();
			if (NULL != p_left_data)
			{
				p_left_data->set_family_id(common::string_util_t::uint64_to_string(left_family_id));
				p_left_data->set_family_name(p_family_left->get_name());
				p_left_data->set_family_prestige(p_family_left->get_prestige());
				p_left_data->set_family_score(0);
				p_left_data->set_family_rank(0);
				p_left_data->set_result(proto::common::family_war_result_none);
			}

			// 通知和邮件
			p_family_left->send_sys_notice_to_member(sys_notice_enum::sys_notice_113);
			p_family_left->send_sys_mail_to_member(mailcode_enum::sys_mail_family_war_announce);
		}

		uint64_t right_family_id = war_list[i + 1];
		const family_ptr& p_family_right = get_family(right_family_id);
		if (NULL != p_family_right)
		{
			m_family_against_id_map.insert(std::make_pair(right_family_id, index));

			proto::common::family_war_data* p_right_data = m_against_data_map[index].m_data.mutable_right_side();
			if (NULL != p_right_data)
			{
				p_right_data->set_family_id(common::string_util_t::uint64_to_string(right_family_id));
				p_right_data->set_family_name(p_family_right->get_name());
				p_right_data->set_family_prestige(p_family_right->get_prestige());
				p_right_data->set_family_score(0);
				p_right_data->set_family_rank(0);
				p_right_data->set_result(proto::common::family_war_result_none);
			}

			// 通知和邮件
			p_family_right->send_sys_notice_to_member(sys_notice_enum::sys_notice_113);
			p_family_right->send_sys_mail_to_member(mailcode_enum::sys_mail_family_war_announce);
		}

		m_against_data_map[index].m_state = proto::common::family_war_state_init;

		index++;
	}

	family_data_mgr_t::save_all_war_data(week_id, m_against_data_map);

	update_against_list();

	FAMILY_LOG("king war generate war_list success");

	return true;
}

void family_manager_t::rank_war_list()
{
	std::vector<score_sort_t> all_sort;

	for (against_data_map::const_iterator citr = m_against_data_map.begin();
		citr != m_against_data_map.end(); ++citr)
	{
		uint32_t index = citr->first;
		const family_against_data_t& war_data = citr->second;

		{
			const proto::common::family_war_data& left_side_data = war_data.m_data.left_side();
			uint32_t left_id = (index << 16) + family_war_side::family_war_side_left;
			all_sort.push_back(score_sort_t(left_id, common::string_util_t::string_to_uint64(left_side_data.family_id()), left_side_data.family_score(), left_side_data.family_prestige()));
		}

		{
			const proto::common::family_war_data& right_side_data = war_data.m_data.right_side();
			uint32_t right_id = (index << 16) + family_war_side::family_war_side_right;
			all_sort.push_back(score_sort_t(right_id, common::string_util_t::string_to_uint64(right_side_data.family_id()), right_side_data.family_score(), right_side_data.family_prestige()));
		}
	}

	std::sort(all_sort.begin(), all_sort.end());

	for (uint32_t i = 0; i < all_sort.size(); ++i)
	{
		uint32_t index = all_sort[i].m_id >> 16;
		uint32_t side = all_sort[i].m_id & 0x0000FFFF;

		against_data_map::iterator itr = m_against_data_map.find(index);
		if (itr != m_against_data_map.end())
		{
			proto::common::family_war_data* p_data = NULL;
			switch (side)
			{
			case family_war_side::family_war_side_left:
				{
					p_data = itr->second.m_data.mutable_left_side();
				}
				break;
			case family_war_side::family_war_side_right:
				{
					p_data = itr->second.m_data.mutable_right_side();
				}
				break;
			default:
				log_error("rank war list side[%u] type error", side);
				break;
			}

			if (NULL != p_data)
			{
				p_data->set_family_rank(i + 1);
			}
		}
	}

	family_data_mgr_t::save_all_war_data(m_family_war_info.family_war_data.week_id(), m_against_data_map);

	update_against_list();
}

void family_manager_t::family_war_end()
{
	for (against_data_map::iterator citr = m_against_data_map.begin();
		citr != m_against_data_map.end(); ++citr)
	{
		if (citr->second.m_war_scene_id != 0)
		{
			const global_scene_ptr& p_global_scene = global_scene_manager_t::get_scene_by_id(citr->second.m_war_scene_id);
			if (NULL != p_global_scene)
			{
				// 设置副本状态，防止人进去
				p_global_scene->set_scene_state(SceneState::SCENE_STATE_UNLOADING);
			}
		}
	}

	m_family_war_info.family_war_data.set_state(proto::common::family_war_state_end);
	global_data_mgr_t::save_family_war_data(proto::server::global_data_type_family_war, m_family_war_info.family_war_data);
}

void family_manager_t::prepare_family_war_reply(uint32_t game_id, const proto::server::ge_prepare_family_war_reply& reply)
{
	if (m_family_war_info.family_war_data.state() != proto::common::family_war_state_prepare)
	{
		log_warn("family_war state[%u] error, when update family war dungeon", m_family_war_info.family_war_data.state());
		return;
	}

	for (int32_t i = 0; i < reply.dungeons_size(); ++i)
	{
		const proto::server::family_war_dungeon_back& data = reply.dungeons(i);
		
		against_data_map::iterator itr = m_against_data_map.find(data.id());
		if (itr != m_against_data_map.end())
		{
			if (global_scene_manager_t::add_damily_dungeon(game_id, data.id(), data.info()))
			{
				itr->second.m_war_scene_id = data.info().scene_id();
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void family_manager_t::get_family_prayer_list(const global_user_ptr& p_user, proto::client::ec_family_prayer_list_reply& reply)
{
	if (NULL == p_user) {
		log_error("null == p_user");
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	const family_ptr& p_family = get_family(p_user->get_family_id());
	if (NULL == p_family) {
		log_error("user[%lu] family[%lu] not find", p_user->get_uid(), p_user->get_family_id());
		reply.set_reply_code(errcode_enum::family_err_code_invalid_family);
		return;
	}

	const family_member_ptr& p_member = p_family->get_family_member(p_user->get_uid());
	if (NULL == p_member) {
		log_error("user[%lu] not family[%lu] member", p_user->get_uid(), p_user->get_family_id());
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	p_family->get_family_prayer_list(reply.mutable_data());
	p_member->peek_prayer_times_data(reply);
	reply.set_reply_code(errcode_enum::error_ok);
}

void family_manager_t::get_prayer_gift_reward(const global_user_ptr& p_user, proto::client::ec_prayer_gift_reward_reply& reply)
{
	if (NULL == p_user) {
		log_error("null == p_user");
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	const family_ptr& p_family = get_family(p_user->get_family_id());
	if (NULL == p_family) {
		log_error("user[%lu] family[%lu] not find", p_user->get_uid(), p_user->get_family_id());
		reply.set_reply_code(errcode_enum::family_err_code_invalid_family);
		return;
	}

	const family_member_ptr& p_member = p_family->get_family_member(p_user->get_uid());
	if (NULL == p_member) {
		log_error("user[%lu] not family[%lu] member", p_user->get_uid(), p_user->get_family_id());
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	reply.set_gift_times(p_member->get_prayer_gift_times());
	reply.set_reply_code(errcode_enum::error_ok);
}

void family_manager_t::get_prayer_record(const global_user_ptr& p_user, proto::client::ec_prayer_record_reply& reply)
{
	if (NULL == p_user) {
		log_error("null == p_user");
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	const family_ptr& p_family = get_family(p_user->get_family_id());
	if (NULL == p_family) {
		log_error("user[%lu] family[%lu] not find", p_user->get_uid(), p_user->get_family_id());
		reply.set_reply_code(errcode_enum::family_err_code_invalid_family);
		return;
	}

	const family_member_ptr& p_member = p_family->get_family_member(p_user->get_uid());
	if (NULL == p_member) {
		log_error("user[%lu] not family[%lu] member", p_user->get_uid(), p_user->get_family_id());
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	p_member->get_prayer_record_list(reply.mutable_data());
	reply.set_reply_code(errcode_enum::error_ok);
}

uint32_t family_manager_t::family_prayer_request(const global_user_ptr& p_user, uint32_t hero_id, uint32_t num)
{
	if (NULL == p_user) {
		log_error("null == p_user");
		return common::errcode_enum::notice_unknown;
	}

	const family_ptr& p_family = get_family(p_user->get_family_id());
	if (NULL == p_family) {
		log_error("user[%lu] family[%lu] not find", p_user->get_uid(), p_user->get_family_id());
		return common::errcode_enum::family_err_code_invalid_family;
	}

	const family_member_ptr& p_member = p_family->get_family_member(p_user->get_uid());
	if (NULL == p_member) {
		log_error("user[%lu] not family[%lu] member", p_user->get_uid(), p_user->get_family_id());
		return common::errcode_enum::notice_unknown;
	}

	auto conf = GET_CONF(Comprehensive, comprehensive_common::family_pray_condition1);
	uint32_t condition1 = GET_COMPREHENSIVE_VALUE_1(conf);
	if (common::time_util_t::now_time() < p_member->get_join_time() + condition1 * 3600) {
		FAMILY_LOG("user[%lu] join family time < %u hour", p_user->get_uid(), condition1);
		return common::errcode_enum::family_error_prayer_join_time;
	}

	uint32_t prayer_times = p_member->get_prayer_times();
	if (!prayer_times) {
		FAMILY_LOG("user[%lu] prayer_times == 0", p_user->get_uid());
		return common::errcode_enum::family_error_prayer_times_zero;
	}

	p_member->ask_prayer_hero(hero_id, num);

	return common::errcode_enum::error_ok;
}

void family_manager_t::family_prayer_gift_request(const global_user_ptr& p_user,
	uint64_t gift_uid, uint32_t hero_tid, uint32_t hero_material_id, uint32_t rarity, uint32_t hero_num,
	proto::server::eg_family_prayer_gift_failed_notify& reply)
{
	if (NULL == p_user) {
		log_error("null == p_user");
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	const family_ptr& p_family = get_family(p_user->get_family_id());
	if (NULL == p_family) {
		log_error("user[%lu] family[%lu] not find", p_user->get_uid(), p_user->get_family_id());
		reply.set_reply_code(common::errcode_enum::family_err_code_invalid_family);
		return;
	}

	const family_member_ptr& p_member = p_family->get_family_member(p_user->get_uid());
	if (NULL == p_member) {
		log_error("user[%lu] not family[%lu] member", p_user->get_uid(), p_user->get_family_id());
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	const family_member_ptr& p_gift_member = p_family->get_family_member(gift_uid);
	if (NULL == p_gift_member) {
		log_error("user[%lu] gift user[%lu] not family member", p_user->get_uid(), gift_uid);
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	auto conf = GET_CONF(Comprehensive, comprehensive_common::family_pray_gift_condition1);
	uint32_t condition1 = GET_COMPREHENSIVE_VALUE_1(conf);
	if (common::time_util_t::now_time() < p_member->get_join_time() + condition1 * 3600) {
		FAMILY_LOG("user[%lu] join family time < %u hour", p_user->get_uid(), condition1);
		reply.set_reply_code(common::errcode_enum::family_error_gift_join_time);
		return;
	}

	uint32_t back_code = p_gift_member->ask_gift_hero(p_member, hero_tid, hero_material_id, rarity, hero_num);
	reply.set_reply_code(back_code);
	if (back_code == common::errcode_enum::error_ok) {
		uint32_t gift_rwd_item = 0;
		p_member->do_gift_reward(p_gift_member, hero_tid, hero_material_id, rarity, hero_num, gift_rwd_item);

		if (gift_rwd_item != 0)
			reply.set_rwd_item(gift_rwd_item);
	}
}

// family_ptr family_manager_t::is_can_create_country()
// {
// 	auto conf = GET_CONF(Comprehensive, comprehensive_common::country_unlock_para1);
// 	uint32_t unlock_param = GET_COMPREHENSIVE_VALUE_1(conf);
// 
// 	for (family_map::iterator itr = m_family_map.begin();
// 		itr != m_family_map.end(); ++itr) {
// 		if (NULL != itr->second && itr->second->get_level() >= unlock_param) {
// 			return itr->second;
// 		}
// 	}
// 
// 	return NULL;
// }

void family_manager_t::get_family_shop_data(const global_user_ptr& p_user, proto::client::ec_get_family_shop_reply& reply)
{
	if (NULL == p_user) {
		log_error("null == p_user");
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	if (0 == p_user->get_family_id()) {
		FAMILY_LOG("user[%lu] not join family", p_user->get_uid());
		reply.set_reply_code(common::errcode_enum::notice_no_family);
		return;
	}

	const family_ptr& p_family = get_family(p_user->get_family_id());
	if (NULL == p_family) {
		log_error("user[%lu] family[%lu] not find", p_user->get_uid(), p_user->get_family_id());
		reply.set_reply_code(common::errcode_enum::family_err_code_invalid_family);
		return;
	}

	const family_member_ptr& p_member = p_family->get_family_member(p_user->get_uid());
	if (NULL == p_member) {
		log_error("user[%lu] not family[%lu] member", p_user->get_uid(), p_user->get_family_id());
		reply.set_reply_code(common::errcode_enum::notice_unknown);
		return;
	}

	p_family->peek_family_shop_data(reply.mutable_shop_data());
	p_member->peek_family_shop_personal_data(reply.mutable_personal_data());
	reply.set_shop_lv(p_family->get_build_level(proto::common::family_build_type_shop));
	reply.set_reply_code(common::errcode_enum::error_ok);
}

uint32_t family_manager_t::family_shop_buy_request(const global_user_ptr& p_user, uint32_t index, uint32_t shop_item, uint32_t num)
{
	if (NULL == p_user) {
		log_error("null == p_user");
		return common::errcode_enum::notice_unknown;
	}

	if (0 == p_user->get_family_id()) {
		FAMILY_LOG("user[%lu] not join family", p_user->get_uid());
		return common::errcode_enum::notice_no_family;
	}

	const family_ptr& p_family = get_family(p_user->get_family_id());
	if (NULL == p_family) {
		log_error("user[%lu] family[%lu] not find", p_user->get_uid(), p_user->get_family_id());
		return common::errcode_enum::family_err_code_invalid_family;
	}

	return p_family->family_shop_buy(p_user, index, shop_item, num);
}

void family_manager_t::family_shop_failed_back(const global_user_ptr& p_user, uint32_t refresh_time, uint32_t index, uint32_t num)
{
	if (NULL == p_user) {
		log_error("null == p_user");
		return;
	}

	if (0 == p_user->get_family_id()) {
		FAMILY_LOG("user[%lu] not join family", p_user->get_uid());
		return;
	}

	const family_ptr& p_family = get_family(p_user->get_family_id());
	if (NULL == p_family) {
		log_error("user[%lu] family[%lu] not find", p_user->get_uid(), p_user->get_family_id());
		return;
	}

	p_family->family_shop_failed_back(p_user, refresh_time, index, num);
}
