#include "friend_mgr.hpp"

#include "redis/friend_data_mgr.hpp"
#include "utility.hpp"
#include "config_mgr.h"
#include "user/global_user_manager.hpp"
#include "user/global_user.hpp"
#include "friend_manager.hpp"
#include "log/log_wrapper.hpp"
#include "tblh/errcode_enum.hpp"
#include "red_point/red_point_manager.hpp"
#include "achieve/achieve_common.hpp"
#include "offline/offline_role_manager.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "tblh/TowerTrigger.tbls.h"
#include "common/time_manager.hpp"
#include "tblh/time_type_enum.hpp"
#include "tblh/Friendliness.tbls.h"
#include "tblh/FriendAward.tbls.h"
#include "rank/rank_manager.hpp"
#include "chat/chat_msg_handler.hpp"
#include "tblh/LanguageTable.tbls.h"
#include "common/StringTool.h"
#include "sys_notice/sys_notice_manager.hpp"
#include "redis/global_user_data_mgr.hpp"
#include "user/global_user_msg_handle.hpp"
#include "offline_fight/offline_fight_manager.hpp"
#include "family/family_manager.hpp"
#include "user_troop/user_troop.hpp"

#define WRITE_FRIEND_LOG_TO_DBS(uid, friend_uid, oldtype, newtype, type, param)              \
    log_wrapper_t::send_friend_log(uid, friend_uid, oldtype, newtype, type, param);

uint32_t friend_mgr_t::s_m_last_reset_fat_time = 0;
uint32_t friend_mgr_t::s_m_last_reset_fp_count_time = 0;
//uint32_t friend_mgr_t::s_m_fp_send_initial_limit = friend_manager_t::m_fp_send_initial_limit;
//uint32_t friend_mgr_t::s_m_fp_receive_initial_limit = friend_manager_t::m_fp_receive_initial_limit;

friend_mgr_t::friend_mgr_t(uint64_t role_uid) :m_role_uid(role_uid)
{

}

void friend_mgr_t::clear_friends_not_login_long_time(proto::client::ec_clear_friends_reply& reply)
{
	for (friend_info_map::iterator itr = m_friend_relations.begin();
		itr != m_friend_relations.end();)
	{
		bool is_del = false;

		uint64_t friend_uid = itr->first;
		friend_info_ptr p_friend = itr->second;
		if (p_friend == NULL)
		{
			log_error("role[%lu] p_friend null error when clear_friends_not_login_long_time", m_role_uid);
			is_del = true;
		}

		if (NULL != p_friend && (p_friend->m_member.ftype() == proto::common::friend_type_one_side_friend || p_friend->m_member.ftype() == proto::common::friend_type_two_side_friend))
		{
			global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(friend_uid);
			
			uint32_t offline_time = 0;
			if (NULL == p_user || p_user->is_logout())
			{
				offline_time = common::time_util_t::now_time() - friend_data_mgr_t::get_offline_logout_time(friend_uid);
			}

			if (offline_time > friend_manager_t::get_friendoutdel_limit() * 24 * 3600)
			{
				// 标记删除
				is_del = true;

				// 如果对方是双向好友变为单向
				if (p_friend->m_member.ftype() == proto::common::friend_type_two_side_friend)
				{
					global_user_ptr p_friend_user = global_user_manager_t::get_global_user_by_uid(friend_uid);

					if (p_friend_user != NULL) // 处理在线
					{
						p_friend_user->get_friend_mgr().update_friend_one_side(m_role_uid);
					}
					else // 离线处理直接写redis
					{
						friend_data_mgr_t::update_offline_friend_type(friend_uid, m_role_uid, proto::common::friend_type_one_side_friend);
					}
				}

				// 之前是最近联系人
				if (p_friend->m_member.contact_time() > 0)
				{
					clear_contact_info(p_friend);
				}

				if (p_friend->m_member.is_top() > 0)
				{
					// 不需要clear_top_info， 等下直接清除这条社会关系
					if (m_top_size > 0)
						m_top_size--;
				}

				// 删除社会关系
				friend_data_mgr_t::clear_friend_info(m_role_uid, p_friend->m_redis_key);

				WRITE_FRIEND_LOG_TO_DBS(m_role_uid, friend_uid, p_friend->m_member.ftype(), proto::common::friend_type_none, common::log_enum::source_type_friend_remove_friend, EM_FRIEND_PARAM::FRIEND_CLEAR_LONG_TIME);

				// 好友数量－1
				if (m_friend_size > 0)
					m_friend_size--;

				reply.add_del_uids(common::string_util_t::uint64_to_string(friend_uid));
			}
		}

		// 判断是否删除
		if (is_del)
		{
			m_friend_relations.erase(itr++);
		}
		else
		{
			++itr;
		}
	}
}

void friend_mgr_t::clear_recent_contact_friends()
{
	for (friend_contact_list::iterator itr = m_contact_list.begin();
		itr != m_contact_list.end();)
	{
		friend_info_ptr p_friend = *itr;
		if (p_friend != NULL && p_friend->m_member.contact_time() > 0)
		{
			uint64_t friend_uid = p_friend->m_friend_id;
			proto::common::friend_type old_type = p_friend->m_member.ftype();
			// 单单就是陌生人，并且也不是好友申请人，则强制清除好友数据（不需要通知相关玩家）
			if (old_type == proto::common::friend_type_stranger && p_friend->m_member.time() == 0)
			{
				// 从主map中删除
				if (del_relation(friend_uid))
				{
					WRITE_FRIEND_LOG_TO_DBS(m_role_uid, friend_uid, old_type, proto::common::friend_type_none, common::log_enum::source_type_friend_remove_friend, EM_FRIEND_PARAM::FRIEND_CLEAR_RECENT_CONTACT);
				}
			}
			else // 否则就是单单清楚最近聊天时间
			{
				p_friend->m_member.set_contact_time(0);

				if (friend_data_mgr_t::update_contact_time(m_role_uid, p_friend->m_redis_key, 0))
				{
					WRITE_FRIEND_LOG_TO_DBS(m_role_uid, friend_uid, old_type, old_type, common::log_enum::source_type_friend_clear_recent_friends, 0);
				}
			}
		}

		m_contact_list.erase(itr++);
	}

	// 清空数量
	m_contact_size = 0;
}

void friend_mgr_t::clear_all_black_friends()
{
	for (friend_info_map::iterator itr = m_friend_relations.begin();
		itr != m_friend_relations.end();)
	{
		bool is_del = false;

		uint64_t friend_uid = itr->first;
		friend_info_ptr p_friend = itr->second;
		if (p_friend == NULL)
		{
			log_error("role[%lu] p_friend[%lu] null error when clear_friends_not_login_long_time", m_role_uid, friend_uid);
			is_del = true;
		}

		if (NULL != p_friend && p_friend->m_member.ftype() == proto::common::friend_type_black)
		{
			// 标记删除
			is_del = true;

			// 删除社会关系
			friend_data_mgr_t::clear_friend_info(m_role_uid, p_friend->m_redis_key);

			WRITE_FRIEND_LOG_TO_DBS(m_role_uid, friend_uid, p_friend->m_member.ftype(), proto::common::friend_type_none, common::log_enum::source_type_friend_remove_friend, EM_FRIEND_PARAM::FRIEND_CLEAR_BLACK_FRIENDS);
		}

		// 判断是否删除
		if (is_del)
		{
			m_friend_relations.erase(itr++);
		}
		else
		{
			++itr;
		}
	}

	m_black_size = 0;
}


void friend_mgr_t::clear_all_apply_friends()
{
	for (friend_apply_list::iterator itr = m_apply_list.begin();
		itr != m_apply_list.end();)
	{
		friend_info_ptr p_friend = *itr;
		if (p_friend != NULL && p_friend->m_member.time() > 0)
		{
			uint64_t friend_uid = p_friend->m_friend_id;
			proto::common::friend_type old_type = p_friend->m_member.ftype();

			// 如果同时还是最近联系人或者组队好友，则只把申请时间清0
			if (p_friend->m_member.contact_time() > 0 || old_type == proto::common::friend_type_troop_stranger)
			{
				p_friend->m_member.set_time(0);

				if (friend_data_mgr_t::update_apply_time(m_role_uid, p_friend->m_redis_key, 0))
				{
					WRITE_FRIEND_LOG_TO_DBS(m_role_uid, friend_uid, p_friend->m_member.ftype(), p_friend->m_member.ftype(), common::log_enum::source_type_friend_clear_friend_appliers, 0);
				}
			}
			else // 否则就是单单的好友申请人，则强制清除好友数据（不需要通知相关玩家）
			{
				// 从主map中删除
				if (del_relation(friend_uid))
				{
					WRITE_FRIEND_LOG_TO_DBS(m_role_uid, friend_uid, old_type, proto::common::friend_type_none, common::log_enum::source_type_friend_remove_friend, EM_FRIEND_PARAM::FRIEND_CLEAR_FRIEND_APPLIERS);
				}
			}
		}

		m_apply_list.erase(itr++);
	}

	m_apply_size = 0;
}

void friend_mgr_t::cancel_black_friend_by_uid(uint64_t friend_uid, proto::client::ec_cancel_black_friend_reply& reply)
{
	// 查询自己好友列表有没这个人的信息
	friend_info_ptr p_friend = get_friend_by_id(friend_uid);
	if (p_friend == NULL)
	{
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	// 不是拉黑人
	proto::common::friend_type old_type = p_friend->m_member.ftype();
	if (old_type != proto::common::friend_type_black)
	{
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	// 从主map中删除
	if (del_relation(friend_uid))
	{
		WRITE_FRIEND_LOG_TO_DBS(m_role_uid, friend_uid, old_type, proto::common::friend_type_none, common::log_enum::source_type_friend_remove_friend, EM_FRIEND_PARAM::FRIEND_CANCEL_BLACK);
	}

	if (m_black_size > 0)
		m_black_size--;

	reply.set_friend_uid(common::string_util_t::uint64_to_string(friend_uid));
	reply.set_reply_code(errcode_enum::error_ok);
}


void friend_mgr_t::get_recent_friends_list(proto::client::ec_get_recent_friends_reply& reply)
{
	for (friend_contact_list::iterator itr = m_contact_list.begin();
		itr != m_contact_list.end(); ++itr)
	{
		friend_info_ptr p_friend = *itr;
		if (p_friend == NULL)
		{
			log_error("role[%lu] p_friend null error", m_role_uid);
			continue;
		}

		// 更新下玩家数据
		friend_data_mgr_t::update_user_info(p_friend);

		proto::common::friend_member* p_member = reply.add_friends();
		if (p_member != NULL)
			p_member->CopyFrom(p_friend->m_member);
	}

	reply.set_reply_code(errcode_enum::error_ok);
}


void friend_mgr_t::get_black_friends_list(proto::client::ec_get_black_friends_reply& reply)
{
	for (friend_info_map::iterator itr = m_friend_relations.begin();
		itr != m_friend_relations.end(); ++itr)
	{
		friend_info_ptr p_friend = itr->second;
		if (p_friend == NULL)
		{
			log_error("role[%lu] p_friend null error", m_role_uid);
			continue;
		}

		if (p_friend->m_member.ftype() == proto::common::friend_type_black)
		{
			// 更新下玩家数据
			friend_data_mgr_t::update_user_info(p_friend);

			proto::common::friend_member* p_member = reply.add_friends();
			if (p_member != NULL) {
				//log_info("blackfriendliness:%d", p_friend->m_member.friendliness());
				p_member->CopyFrom(p_friend->m_member);
			}
		}
	}

	reply.set_reply_code(errcode_enum::error_ok);
}


void friend_mgr_t::get_apply_friends_list(proto::client::ec_get_friend_applier_list_reply& reply)
{
	for (friend_apply_list::iterator itr = m_apply_list.begin();
		itr != m_apply_list.end(); ++itr)
	{
		friend_info_ptr p_friend = *itr;
		if (p_friend == NULL)
		{
			log_error("role[%lu] p_friend null error", m_role_uid);
			continue;
		}

		// 更新下玩家数据
		friend_data_mgr_t::update_user_info(p_friend);

		proto::common::friend_member* p_member = reply.add_friends();
		if (p_member != NULL)
			p_member->CopyFrom(p_friend->m_member);
	}

	reply.set_reply_code(errcode_enum::error_ok);
}


uint32_t friend_mgr_t::summon_shadow_request(uint64_t friend_uid)
{
	// 查询自己好友列表有没这个人的信息
	friend_info_ptr p_friend = get_friend_by_id(friend_uid);
	if (p_friend == NULL) {
		log_error("user[%lu] friend[%lu] not two_side_friend", m_role_uid, friend_uid);
		return errcode_enum::user_troop_shadow_not_friend;
	}


	proto::common::friend_type old_type = p_friend->m_member.ftype();
	if (old_type != proto::common::friend_type_two_side_friend) {
		log_error("user[%lu] [%lu] not two_side_friend", m_role_uid, friend_uid);
		return errcode_enum::user_troop_shadow_not_friend;
	}

	if (!time_manager_t::check_same_period(common::time_type_enum::time_one_day, p_friend->m_member.shadow_time())) {
		p_friend->m_member.set_shadow_time(common::time_util_t::now_time());
		p_friend->m_member.set_shadow_count(1);
	} else {
		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::team_ai_friend_invite_num);
		uint32_t val = GET_COMPREHENSIVE_VALUE_1(p_conf);
		uint32_t shadow_count = p_friend->m_member.shadow_count();
		if (shadow_count >= val) {
			log_error("user[%lu] friend[%lu] shadow count[%u] > %u", m_role_uid, friend_uid, shadow_count, val);
			return errcode_enum::user_troop_shadow_count_limit;
		}

		p_friend->m_member.set_shadow_time(common::time_util_t::now_time());
		p_friend->m_member.set_shadow_count(shadow_count + 1);
	}

	friend_data_mgr_t::update_shadow_time(m_role_uid, p_friend);

	return common::errcode_enum::error_ok;
}

void friend_mgr_t::tower_trigger_notify(const proto::server::ge_tower_trigger_notify& msg)
{
	global_user_ptr p_self = global_user_manager_t::get_global_user_by_uid(m_role_uid);
	if (NULL == p_self)
	{
		log_error("NULL == p_self[%lu]", m_role_uid);
		return;
	}
	auto* p_notify_str = GET_CONF(Comprehensive, comprehensive_common::dreamland_hide_find);
	if(p_notify_str == NULL){
		log_error("dreamland_hide_find in Comprehensive is NULL");
		return;
	}
	const std::string& notify_str = p_notify_str->parameter1();
	proto::client::ec_personal_chat_notify notify_msg;
	notify_msg.set_src_role_id(string_util_t::uint64_to_string(m_role_uid));
	notify_msg.set_src_role_name(p_self->get_name());
	notify_msg.set_src_head_id(p_self->get_hero_plugin());
	notify_msg.set_src_level(p_self->get_level());
	notify_msg.set_contact_time(time_util_t::now_time());

	proto::server::eg_tower_trigger_notify ntf;
	for (int32_t i = 0; i < msg.tower_trigger_size(); ++i)
	{
		ntf.add_tower_trigger()->CopyFrom(msg.tower_trigger(i));
	}
	if (0 == msg.friend_uid())
	{
		//全部好友
		for (auto it : m_friend_relations)
		{
			friend_info_ptr p_friend = it.second;
			if (NULL == p_friend)
			{
				continue;
			}
			if (p_friend->m_member.ftype() != proto::common::friend_type_two_side_friend)
			{
				continue;
			}
			uint64_t role_uid = p_friend->m_friend_id;
			global_user_ptr p_role = global_user_manager_t::get_global_user_by_uid(role_uid);
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
				offline_role_manager_t::add_other_trigger(role_uid, msg);
			}
		}
	}
	else
	{
		global_user_ptr p_role = global_user_manager_t::get_global_user_by_uid(msg.friend_uid());
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
			offline_role_manager_t::add_other_trigger(msg.friend_uid(), msg);
		}
	}
}

void friend_mgr_t::chat_notice_all_friend(const proto::client::ce_personal_chat_notice_request& msg)
{
	global_user_ptr p_src_user = global_user_manager_t::get_global_user_by_uid(m_role_uid);
	if (NULL == p_src_user)
	{
		log_error("chat_notice_all_friend NULL == p_src_user");
		return;
	}

	proto::client::ec_personal_chat_reply reply_msg;

	auto conf = GET_CONF(Comprehensive, comprehensive_common::chat_max_len);
	uint32_t chat_max_len = GET_COMPREHENSIVE_VALUE_1(conf);
	chat_max_len = std::min<uint32_t>(1024, chat_max_len);

	if (msg.chat_info().length() < 1 || msg.chat_info().length() > chat_max_len)
	{
		log_error("role[%lu] msg length[%d] error!", m_role_uid, (int32_t)msg.chat_info().length());
		reply_msg.set_reply_code(errcode_enum::notice_content_legal);
		p_src_user->send_msg_to_client(op_cmd::ec_personal_chat_reply, reply_msg);
		return;
	}

	uint32_t now_time = common::time_util_t::now_time();

	proto::client::ec_personal_chat_notify notify_msg;
	notify_msg.set_src_role_id(string_util_t::uint64_to_string(p_src_user->get_uid()));
	notify_msg.set_src_role_name(p_src_user->get_name());
	notify_msg.set_chat_info(msg.chat_info());
	notify_msg.set_src_head_id(msg.head_id());
	notify_msg.set_src_level(p_src_user->get_level());
	notify_msg.set_contact_time(now_time);

	global_user_ptr p_role = NULL;
	uint64_t msg_friend_uid = 0;
	for (int32_t i = 0; i < msg.friend_uid_list_size(); ++i)
	{
		msg_friend_uid = string_util_t::string_to_uint64(msg.friend_uid_list(i));
		//全部好友
		for (auto it : m_friend_relations)
		{
			friend_info_ptr p_friend = it.second;
			if (NULL == p_friend)
			{
				continue;
			}
			if (p_friend->m_member.ftype() != proto::common::friend_type_two_side_friend &&
				p_friend->m_member.ftype() != proto::common::friend_type_one_side_friend)
			{
				continue;
			}
			if(p_friend->m_friend_id != msg_friend_uid)
				continue;

			p_role = global_user_manager_t::get_global_user_by_uid(p_friend->m_friend_id);
			if (NULL == p_role)
				continue;

			// 判断对方是否在我黑名单
			if (get_friend_type_by_uid(p_src_user->get_uid()) == proto::common::friend_type_black)
			{
				continue;
			}
			// 判断我是否在对方黑名单
			if (p_role->get_friend_mgr().get_friend_type_by_uid(p_src_user->get_uid()) == proto::common::friend_type_black)
			{
				continue;
			}
			// 更新自己最近联系时间
			p_src_user->get_friend_mgr().update_contact_time(p_role->get_uid(), now_time, true);
			// 更新对方最近联系时间
			p_role->get_friend_mgr().update_contact_time(p_src_user->get_uid(), now_time, true);

			p_role->send_msg_to_client(op_cmd::ec_personal_chat_notify, notify_msg);
		}
	}

	reply_msg.set_reply_code(errcode_enum::error_ok);
	reply_msg.set_contact_time(now_time);
	p_src_user->send_msg_to_client(op_cmd::ec_personal_chat_reply, reply_msg);
}

void friend_mgr_t::troop_notify(const troop_member_vec& members)
{
	uint32_t troop_friend_count = global_user_data_mgr_t::get_troop_friend_count(m_role_uid);
	if (troop_friend_count >= get_troop_notify_count())
	{
		return;
	}
	if (m_friend_size >= get_troop_notify_friend_count())
	{
		return;
	}
	uint32_t self_level = global_user_data_mgr_t::get_user_level(m_role_uid);
	if (self_level >= get_troop_notify_level())
	{
		return;
	}
	global_user_ptr p_role = global_user_manager_t::get_global_user_by_uid(m_role_uid);
	if (NULL == p_role)
	{
		return;
	}
	for (auto p_member : members)
	{
		if (NULL == p_member)
		{
			continue;
		}
		uint64_t uid = p_member->get_uid();
		if (uid == m_role_uid)
		{
			continue;
		}
		if (p_member->get_object_type() != proto::common::SCENEOBJECT_USER)
		{
			continue;
		}
		proto::common::friend_type ft = get_friend_type_by_uid(uid);
		if (ft >= proto::common::friend_type_one_side_friend)
		{
			continue;
		}
		
		proto::client::ec_friend_troop_notify ntf;
		ntf.set_role_uid(string_util_t::uint64_to_string(uid));
		ntf.set_name(p_member->get_name());
		ntf.set_level(p_member->get_level());
		ntf.set_headpic(p_member->get_plugin());
		p_role->send_msg_to_client(op_cmd::ec_friend_troop_notify, ntf);
		global_user_data_mgr_t::set_troop_friend_count(m_role_uid, troop_friend_count + 1);
		return;
	}
}

uint32_t friend_mgr_t::get_troop_notify_count()
{
	return 5;
}


uint32_t friend_mgr_t::get_troop_notify_friend_count()
{
	return 20;
}


uint32_t friend_mgr_t::get_troop_notify_level()
{
	return 50;
}

void friend_mgr_t::load_all_relations()
{
	friend_data_mgr_t::load_all_friends(m_role_uid, m_friend_relations, m_contact_list, m_contact_size, m_apply_list, m_apply_size, m_friend_size, m_black_size, m_top_size);
	modify_friendliness_data_on_player_enter();
	// 数据处理（申请人超过数量就清除，最近联系人太多就清除）

	// 申请人超过数量就清除
	if (m_apply_size > friend_manager_t::get_friend_cnt4())
	{
		uint32_t del_num = m_apply_size - friend_manager_t::get_friend_cnt4();
		
		clear_apply_list(del_num);
	}

	// 最近联系人超过数量就清除
	if (m_contact_size > friend_manager_t::get_friend_cnt1())
	{
		uint32_t del_num = m_contact_size - friend_manager_t::get_friend_cnt1();

		clear_contact_list(del_num);
	}
}

void friend_mgr_t::load_all_friend_related()
{
	int8_t initialization = 0;
	friend_data_mgr_t::load_all_friend_related(m_role_uid, m_fp_has_sent_today, m_last_refresh_sent_time, m_fp_has_received_today, 
		m_last_refresh_received_time, m_fp_send_limit, m_fp_receive_limit, initialization, m_got_relation_award);
	if (initialization == 0)
	{
		m_fp_send_limit = friend_manager_t::m_fp_send_initial_limit;
		save_send_limit();
		m_fp_receive_limit = friend_manager_t::m_fp_receive_initial_limit;
		save_receive_limit();
		save_initialization();
	}
}


void friend_mgr_t::clear_all()
{
	m_contact_size = 0;
	m_apply_size = 0;
	m_friend_size = 0;
	m_black_size = 0;
	m_top_size = 0;

	m_apply_list.clear();
	m_contact_list.clear();
	m_friend_relations.clear();
}

void friend_mgr_t::clear_apply_list(uint32_t del_num)
{
	if (del_num == 0)
	{
		log_error("role[%lu] del apply list num == 0", m_role_uid);
		return;
	}

	if (m_apply_list.empty())
	{
		log_error("role[%lu] del empty apply_list ", m_role_uid);
		return;
	}

	// 升序
	m_apply_list.sort(friend_apply_t());

	for (uint32_t i = 0; i < del_num; ++i)
	{
		friend_info_ptr p_friend = m_apply_list.front();

		if (NULL == p_friend)
		{
			log_error("p_friend null error");

			m_apply_list.pop_front();

			if (m_apply_size > 0)
				m_apply_size--;

			continue;
		}

		uint64_t friend_uid = p_friend->m_friend_id;
		proto::common::friend_type old_type = p_friend->m_member.ftype();

		// 如果同时还是最近联系人或者组队好友，则只把申请时间清0
		if (p_friend->m_member.contact_time() > 0 || old_type == proto::common::friend_type_troop_stranger)
		{
			p_friend->m_member.set_time(0);

			if (friend_data_mgr_t::update_apply_time(m_role_uid, p_friend->m_redis_key, 0))
			{
				WRITE_FRIEND_LOG_TO_DBS(m_role_uid, friend_uid, old_type, old_type, common::log_enum::source_type_friend_clear_apply_time, EM_FRIEND_PARAM::FRIEND_APPLY_TOO_MUCH);
			}
		}
		else // 否则就是单单的好友申请人，则强制清除好友数据（不需要通知相关玩家）
		{
			if (del_relation(p_friend->m_friend_id))
			{
				WRITE_FRIEND_LOG_TO_DBS(m_role_uid, friend_uid, old_type, proto::common::friend_type_none, common::log_enum::source_type_friend_remove_friend, EM_FRIEND_PARAM::FRIEND_APPLY_TOO_MUCH_ADN_DEL);
			}
		}

		m_apply_list.pop_front();

		if (m_apply_size > 0)
			m_apply_size--;
	}
}

void friend_mgr_t::clear_contact_list(uint32_t del_num)
{
	if (del_num == 0)
	{
		log_error("role[%lu] del contact list num == 0", m_role_uid);
		return;
	}

	if (m_contact_list.empty())
	{
		log_error("role[%lu] del empty contact_list ", m_role_uid);
		return;
	}

	// 升序
	m_contact_list.sort(friend_contact_t());

	for (uint32_t i = 0; i < del_num; ++i)
	{
		friend_info_ptr p_friend = m_contact_list.front();

		if (NULL == p_friend)
		{
			log_error("p_friend null error");

			m_contact_list.pop_front();

			if (m_contact_size > 0)
				m_contact_size--;

			continue;
		}

		uint64_t friend_uid = p_friend->m_friend_id;
		proto::common::friend_type old_type = p_friend->m_member.ftype();

		// 单单就是陌生人，并且也不是好友申请人，则强制清除好友数据（不需要通知相关玩家）
		if (old_type == proto::common::friend_type_stranger && p_friend->m_member.time() == 0)
		{
			if (del_relation(p_friend->m_friend_id))
			{
				WRITE_FRIEND_LOG_TO_DBS(m_role_uid, friend_uid, old_type, proto::common::friend_type_none, common::log_enum::source_type_friend_remove_friend, EM_FRIEND_PARAM::FRIEND_CONTACT_TOO_MUCH_ADN_DEL);
			}
		}
		else // 否则就是单单清楚最近聊天时间
		{
			p_friend->m_member.set_contact_time(0);

			if (friend_data_mgr_t::update_contact_time(m_role_uid, p_friend->m_redis_key, 0))
			{
				WRITE_FRIEND_LOG_TO_DBS(m_role_uid, friend_uid, old_type, old_type, common::log_enum::source_type_friend_clear_content_time, EM_FRIEND_PARAM::FRIEND_CONTACT_TOO_MUCH);
			}
		}

		m_contact_list.pop_front();

		if (m_contact_size > 0)
			m_contact_size--;
	}
}

bool friend_mgr_t::del_relation(uint64_t friend_uid)
{
	friend_info_map::iterator friend_itr = m_friend_relations.find(friend_uid);
	if (friend_itr != m_friend_relations.end())
	{
		friend_info_ptr p_friend = friend_itr->second;
		if (NULL == p_friend)
		{
			log_error("role[%lu] del friend[%lu] error", m_role_uid, friend_uid);
			m_friend_relations.erase(friend_itr);
			return false;
		}

		friend_data_mgr_t::clear_friend_info(m_role_uid, p_friend->m_redis_key);

		m_friend_relations.erase(friend_itr);

		return true;
	}

	return false;
}


friend_info_ptr friend_mgr_t::get_friend_by_id(uint64_t friend_uid)
{
	friend_info_map::iterator itr = m_friend_relations.find(friend_uid);
	if (itr != m_friend_relations.end())
	{
		return itr->second;
	}

	return NULL;
}


bool friend_mgr_t::clear_apply_info(friend_info_ptr p_friend)
{
	if (p_friend == NULL)
	{
		log_error("role[%lu] p_friend null error", m_role_uid);
		return false;
	}

	p_friend->m_member.set_time(0);

	friend_data_mgr_t::update_apply_time(m_role_uid, p_friend->m_redis_key, 0);

	WRITE_FRIEND_LOG_TO_DBS(m_role_uid, p_friend->m_friend_id, p_friend->m_member.ftype(), p_friend->m_member.ftype(), common::log_enum::source_type_friend_clear_apply_time, 0);

	// 目前唯一的瓶颈就是list的查询，list元素不能太多
	friend_apply_list::iterator itr = std::find(m_apply_list.begin(), m_apply_list.end(), p_friend);
	if (itr == m_apply_list.end())
	{
		log_error("role[%lu] can not find friend[%lu]", m_role_uid, p_friend->m_friend_id);
		return false;
	}

	m_apply_list.erase(itr);
	if (m_apply_size > 0)
		m_apply_size--;

	return true;
}

bool friend_mgr_t::clear_contact_info(friend_info_ptr p_friend)
{
	if (p_friend == NULL)
	{
		log_error("role[%lu] p_friend null error", m_role_uid);
		return false;
	}

	p_friend->m_member.set_contact_time(0);

	friend_data_mgr_t::update_contact_time(m_role_uid, p_friend->m_redis_key, 0);

	WRITE_FRIEND_LOG_TO_DBS(m_role_uid, p_friend->m_friend_id, p_friend->m_member.ftype(), p_friend->m_member.ftype(), common::log_enum::source_type_friend_clear_content_time, 0);
	
	// 目前唯一的瓶颈就是list的查询，list元素不能太多
	friend_contact_list::iterator itr = std::find(m_contact_list.begin(), m_contact_list.end(), p_friend);
	if (itr == m_contact_list.end())
	{
		log_error("role[%lu] can not find friend[%lu]", m_role_uid, p_friend->m_friend_id);
		return false;
	}

	m_contact_list.erase(itr);
	if (m_contact_size > 0)
		m_contact_size--;

	return true;
}


bool friend_mgr_t::clear_top_info(friend_info_ptr p_friend)
{
	if (p_friend == NULL)
	{
		log_error("role[%lu] p_friend null error", m_role_uid);
		return false;
	}

	p_friend->m_member.set_top_time(0);
	p_friend->m_member.set_is_top(0);

	friend_data_mgr_t::update_top_time(m_role_uid, p_friend->m_redis_key, false, 0);

	WRITE_FRIEND_LOG_TO_DBS(m_role_uid, p_friend->m_friend_id, p_friend->m_member.ftype(), p_friend->m_member.ftype(), common::log_enum::source_type_friend_cancel_top_friend, 0);

	if (m_top_size > 0)
		m_top_size--;

	return true;
}

void friend_mgr_t::get_friend_list(proto::client::ec_get_friends_reply& reply)
{
	for (friend_info_map::iterator itr = m_friend_relations.begin();
		itr != m_friend_relations.end(); ++itr)
	{
		friend_info_ptr p_friend = itr->second;
		if (p_friend == NULL)
		{
			log_error("role[%lu] p_friend null error", m_role_uid);
			continue;
		}

		if (p_friend->m_member.ftype() == proto::common::friend_type_one_side_friend || p_friend->m_member.ftype() == proto::common::friend_type_two_side_friend)
		{
			// 更新下玩家数据
			friend_data_mgr_t::update_user_info(p_friend);

			proto::common::friend_member* p_member = reply.add_friends();
			if (p_member != NULL) {
				p_member->CopyFrom(p_friend->m_member);
				//log_info("can_get_friendship_point:%d", p_member->can_get_friendship_point());
			}
		}
	}
	reply.set_fp_has_sent_today(m_fp_has_sent_today);
	reply.set_fp_has_received_today(m_fp_has_received_today);

	reply.set_reply_code(errcode_enum::error_ok);
}


void friend_mgr_t::add_friend_by_uid(uint64_t friend_uid, proto::client::ec_add_friend_reply& reply)
{
	// 自己不能加自己
	if (m_role_uid == friend_uid)
	{
		reply.set_reply_code(errcode_enum::friend_error_add_own);
		return;
	}

	// 查询自己好友列表有没这个人的信息
	friend_info_ptr p_friend = get_friend_by_id(friend_uid);

	proto::common::friend_type old_type = proto::common::friend_type_none;
	if (p_friend != NULL)
	{
		old_type = p_friend->m_member.ftype();
	}

	if (p_friend != NULL && (old_type == proto::common::friend_type_one_side_friend || old_type == proto::common::friend_type_two_side_friend))
	{
		reply.set_reply_code(errcode_enum::friend_error_added_friend);
		return;
	}

	if (p_friend != NULL && old_type == proto::common::friend_type_black)
	{
		reply.set_reply_code(errcode_enum::friend_error_add_blacklist);
		return;
	}

	if (m_friend_size >= friend_manager_t::get_friend_cnt2())
	{
		reply.set_reply_code(errcode_enum::friend_error_excess_max_friend_limit);
		return;
	}

	// 没有这个人
	if (!friend_data_mgr_t::is_user_key_exsit(friend_uid))
	{
		reply.set_reply_code(errcode_enum::notice_role_not_exist);
		return;
	}

	bool is_new_friend = false;

	// 加好友时间
	uint32_t time_now = common::time_util_t::now_time();

	if (p_friend != NULL)
	{
		// 如果是好友申请人，从好友申请列表移除
		if (p_friend->m_member.time() > 0)
		{
			clear_apply_info(p_friend);
		}
	}
	else
	{
		p_friend = friend_info_ptr(new friend_info_t());
		if (NULL == p_friend)
		{
			log_error("role[%lu] p_friend null error", m_role_uid);
			reply.set_reply_code(errcode_enum::notice_unknown);
			return;
		}

		is_new_friend = true;

		std::string rk;
		std::ostringstream oss;
		oss << m_role_uid << friend_uid;

		p_friend->m_friend_id = friend_uid;
		p_friend->m_redis_key = oss.str();
		p_friend->m_member.set_friend_uid(common::string_util_t::uint64_to_string(friend_uid));
		p_friend->m_member.set_contact_time(0);
		p_friend->m_member.set_top_time(0);
		p_friend->m_member.set_is_top(0);
		p_friend->m_member.set_time(time_now);
		p_friend->m_member.set_shadow_time(0);
		p_friend->m_member.set_shadow_count(0);
		p_friend->m_member.set_friendliness(0);
		p_friend->m_member.set_friendliness_added_today(0);
		p_friend->m_member.set_friendliness_refresh_time(0);
		p_friend->m_member.set_can_get_friendship_point(0);
		p_friend->m_member.set_last_get_fp_time(0);
		p_friend->m_member.set_last_sent_fp_time(0);
	}

	// 好友关系
	// 我相对与要加好友人关系
	// 这个关系是好友那边显示的我的关系
	proto::common::friend_type friend_me_type = proto::common::friend_type_none;

	global_user_ptr p_friend_user = global_user_manager_t::get_global_user_by_uid(friend_uid);
	if (p_friend_user != NULL)
	{
		friend_info_ptr p_friend_me = p_friend_user->get_friend_mgr().get_friend_by_id(m_role_uid);
		if (p_friend_me != NULL)
		{
			friend_me_type = p_friend_me->m_member.ftype();
		}
	}
	else
	{
		friend_me_type = friend_data_mgr_t::get_offline_friend_type(friend_uid, m_role_uid);
	}

	// 处理加好友后的关系
	switch (friend_me_type)
	{
	case proto::common::friend_type_none:
	case proto::common::friend_type_stranger:
	case proto::common::friend_type_troop_stranger:
		{
			p_friend->m_member.set_ftype(proto::common::friend_type_one_side_friend);

			// 更新社会关系
			update_friend_info(old_type, p_friend, is_new_friend);

			// 把自己添加到对方的好友申请人中，并建立社会联系
			if (p_friend_user != NULL)	// 处理在线
			{
				p_friend_user->get_friend_mgr().add_apply_time(m_role_uid, time_now);
			}
			else // 离线处理直接写redis
			{
				friend_data_mgr_t::update_offline_friend_apply_time(friend_uid, m_role_uid, time_now);
			}
		}
		break;
	case proto::common::friend_type_one_side_friend:
		{
			p_friend->m_member.set_ftype(proto::common::friend_type_two_side_friend);

			//更新亲密度
			//成为双向好友后双方亲密度为1
			int32_t change_value = friend_manager_t::get_initial_friends_friendliness();
			friend_mgr_t::change_online_friendliness_value(p_friend, change_value);
			WRITE_FRIEND_LOG_TO_DBS(m_role_uid, friend_uid, 0, change_value, common::log_enum::source_type_friend_plus_friendliness, FRIENDLINESS_CHANGE_REASON::BE_FRIENDS);
			// 更新社会关系
			// 这一步会将数据写入redis并插入m_friend_relations
			update_friend_info(old_type, p_friend, is_new_friend);

			// 更新下好友关系
			//更改友方亲密度的数据
			friend_mgr_t::change_friendliness_and_save_data(friend_uid, m_role_uid, friend_manager_t::get_initial_friends_friendliness(), FRIENDLINESS_CHANGE_REASON::BE_FRIENDS);
			if (p_friend_user != NULL)	// 处理在线
			{
				p_friend_user->get_friend_mgr().update_friend_two_side(m_role_uid);
				
			}
			else // 离线处理直接写redis
			{
				friend_data_mgr_t::update_offline_friend_type(friend_uid, m_role_uid, proto::common::friend_type_two_side_friend);
				offline_role_manager_t::notify_self_trigger_to_friend(friend_uid, m_role_uid);
			}
		}
		break;
	case proto::common::friend_type_two_side_friend:
		{
			log_error("role[%lu] add friend[%lu], error friend_me_type == friend_type_two_side_friend", m_role_uid, friend_uid);
		}
		break;
	case proto::common::friend_type_black:
		{
			p_friend->m_member.set_ftype(proto::common::friend_type_one_side_friend);

			// 更新社会关系
			update_friend_info(old_type, p_friend, is_new_friend);

			// 自己在别人黑名单里对方不用处理
		}
		break;
	default:
		{
			log_error("role[%lu] add friend[%lu], error friend_me_type[%u]", m_role_uid, friend_uid, friend_me_type);
		}
		break;
	}

	// 好友数量增加
	m_friend_size++;

	// 更新好友为最近联系人,不需要额外通知因为加成功会自动通知信息
	update_contact_time(friend_uid, time_now, false);

	// 更新下玩家数据
	friend_data_mgr_t::update_user_info(p_friend);

	proto::common::friend_member* p_member = reply.mutable_member_info();
	if (p_member != NULL)
		p_member->CopyFrom(p_friend->m_member);

	reply.set_reply_code(errcode_enum::error_ok);
	achieve_common_t::notify_progress_state(m_role_uid, proto::common::ACHIEVE_HAVE_FRIEND_NUMBER, 0);
	//achieve_common_t::notify_progress_state(friend_uid, proto::common::ACHIEVE_HAVE_FRIEND_NUMBER);///对方不在线
}

void friend_mgr_t::delete_friend_by_uid(uint64_t friend_uid, proto::client::ec_remove_friend_reply& reply)
{
	// 查询自己好友列表有没这个人的信息
	friend_info_ptr p_friend = get_friend_by_id(friend_uid);
	if (p_friend == NULL)
	{
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	// 不是好友在删除好友
	proto::common::friend_type old_type = p_friend->m_member.ftype();
	if (old_type != proto::common::friend_type_one_side_friend && old_type != proto::common::friend_type_two_side_friend)
	{
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	// 如果对方是双向好友变为单向
	if (old_type == proto::common::friend_type_two_side_friend)
	{
		global_user_ptr p_friend_user = global_user_manager_t::get_global_user_by_uid(friend_uid);

		if (p_friend_user != NULL) // 处理在线
		{
			p_friend_user->get_friend_mgr().update_friend_one_side(m_role_uid);
		}
		else // 离线处理直接写redis
		{
			friend_data_mgr_t::update_offline_friend_type(friend_uid, m_role_uid, proto::common::friend_type_one_side_friend);
		}
		friend_mgr_t::clear_friendliness_value_and_save_data(friend_uid, m_role_uid, FRIENDLINESS_CHANGE_REASON::BE_NO_FRIENDS);
	}

	// 之前是最近联系人
	if (p_friend->m_member.contact_time() > 0)
	{
		clear_contact_info(p_friend);
	}

	if (p_friend->m_member.is_top() > 0)
	{
		// 不需要clear_top_info， 等下直接清除这条社会关系
		if (m_top_size > 0)
			m_top_size--;
	}

	// 删除社会关系
	if (del_relation(friend_uid))
	{
		WRITE_FRIEND_LOG_TO_DBS(m_role_uid, friend_uid, old_type, proto::common::friend_type_none, common::log_enum::source_type_friend_remove_friend, EM_FRIEND_PARAM::FRIEND_ROLE_DEL);
	}

	if (m_friend_size > 0)
		m_friend_size--;

	reply.set_friend_uid(common::string_util_t::uint64_to_string(friend_uid));
	reply.set_reply_code(errcode_enum::error_ok);
}

void friend_mgr_t::black_friend_by_uid(uint64_t friend_uid, proto::client::ec_black_friend_reply& reply)
{
	if (m_black_size >= friend_manager_t::get_friend_cnt3())
	{
		reply.set_reply_code(errcode_enum::friend_error_excess_max_black_list_limit);
		return;
	}

	bool is_new_friend = false;

	// 查询自己好友列表有没这个人的信息
	friend_info_ptr p_friend = get_friend_by_id(friend_uid);

	proto::common::friend_type old_type = proto::common::friend_type_none;
	if (p_friend != NULL)
	{
		old_type = p_friend->m_member.ftype();
	}

	if (p_friend != NULL)
	{
		// 如果对方是双向好友变为单向
		if (old_type == proto::common::friend_type_two_side_friend)
		{
			global_user_ptr p_friend_user = global_user_manager_t::get_global_user_by_uid(friend_uid);
			
			if (p_friend_user != NULL) // 处理在线
			{
				p_friend_user->get_friend_mgr().update_friend_one_side(m_role_uid);
			}
			else // 离线处理直接写redis
			{
				friend_data_mgr_t::update_offline_friend_type(friend_uid, m_role_uid, proto::common::friend_type_one_side_friend);
			}
			//清除亲密度
			friend_mgr_t::clear_friendliness_value_and_save_data(m_role_uid, friend_uid, FRIENDLINESS_CHANGE_REASON::BE_NO_FRIENDS);
			friend_mgr_t::clear_friendliness_value_and_save_data(friend_uid, m_role_uid, FRIENDLINESS_CHANGE_REASON::BE_NO_FRIENDS);
		}

		// 之前是好友申请人
		if (p_friend->m_member.time() > 0)
		{
			clear_apply_info(p_friend);
		}

		// 之前是好友
		if (old_type == proto::common::friend_type_one_side_friend || old_type == proto::common::friend_type_two_side_friend)
		{
			if (m_friend_size > 0)
				m_friend_size--;

			if (p_friend->m_member.is_top() > 0)
			{
				clear_top_info(p_friend);
			}
		}

		// 之前是最近联系人
		if (p_friend->m_member.contact_time() > 0)
		{
			clear_contact_info(p_friend);
		}
	}
	else
	{
		p_friend = friend_info_ptr(new friend_info_t());
		if (NULL == p_friend)
		{
			log_error("role[%lu] p_friend null error", m_role_uid);
			reply.set_reply_code(errcode_enum::notice_unknown);
			return;
		}

		is_new_friend = true;

		std::string rk;
		std::ostringstream oss;
		oss << m_role_uid << friend_uid;

		p_friend->m_friend_id = friend_uid;
		p_friend->m_redis_key = oss.str();
		p_friend->m_member.set_friend_uid(common::string_util_t::uint64_to_string(friend_uid));
		p_friend->m_member.set_contact_time(0);
		p_friend->m_member.set_top_time(0);
		p_friend->m_member.set_is_top(0);
		p_friend->m_member.set_time(0);
		p_friend->m_member.set_shadow_time(0);
		p_friend->m_member.set_shadow_count(0);
		p_friend->m_member.set_friendliness(0);
		p_friend->m_member.set_friendliness_added_today(0);
		p_friend->m_member.set_friendliness_refresh_time(0);
		p_friend->m_member.set_can_get_friendship_point(0);
		p_friend->m_member.set_last_get_fp_time(0);
		p_friend->m_member.set_last_sent_fp_time(0);
	}

	p_friend->m_member.set_ftype(proto::common::friend_type_black);

	// 更新社会关系
	update_friend_info(old_type, p_friend, is_new_friend);

	m_black_size++;

	// 更新下玩家数据
	friend_data_mgr_t::update_user_info(p_friend);

	proto::common::friend_member* p_member = reply.mutable_member_info();
	if (p_member != NULL)
		p_member->CopyFrom(p_friend->m_member);

	reply.set_reply_code(errcode_enum::error_ok);
}


void friend_mgr_t::get_recommend_friends(proto::client::ec_refresh_recommend_friends_list_reply& reply)
{
	uint32_t self_country = global_user_data_mgr_t::get_user_country_id(m_role_uid);
	std::vector<uint64_t> uids;
	//获取等级排行榜前100的玩家
	rank_level_ptr p_rank_level = rank_manager_t::get_rank_level();
	if (NULL != p_rank_level)
	{
		std::vector<uint64_t> rank_uids;
		std::vector<uint64_t> self_country_uids;
		std::vector<uint64_t> other_country_uids;
		p_rank_level->get_rank_user(100, rank_uids);
		for (auto uid : rank_uids)
		{
			if (uid == m_role_uid)
			{
				continue;
			}
			proto::common::friend_type ft = get_friend_type_by_uid(uid);
			if (ft >= proto::common::friend_type_one_side_friend)
			{
				continue;
			}
			global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
			if (NULL == p_user)
			{
				continue;
			}
			uint32_t country_id = global_user_data_mgr_t::get_user_country_id(uid);
			if (self_country > 0 && country_id == self_country)
			{
				self_country_uids.push_back(uid);
			}
			else
			{
				other_country_uids.push_back(uid);
			}
		}
		if (!self_country_uids.empty())
		{
			//本国玩家
			uint32_t index = random_util_t::randMin(0, self_country_uids.size());
			uint64_t target_uid = self_country_uids.at(index);
			uids.push_back(target_uid);
			proto::common::recommend_member* p_member = reply.add_recommend_friends();
			proto::common::user_info* p_info = p_member->mutable_info();
			friend_data_mgr_t::search_user_info_by_uid(target_uid, p_info);
			p_member->set_src(proto::common::recommend_source_country_top);
		}
		if (!other_country_uids.empty())
		{
			//别国玩家
			uint32_t index = random_util_t::randMin(0, other_country_uids.size());
			uint64_t target_uid = other_country_uids.at(index);
			uids.push_back(target_uid);
			proto::common::recommend_member* p_member = reply.add_recommend_friends();
			proto::common::user_info* p_info = p_member->mutable_info();
			friend_data_mgr_t::search_user_info_by_uid(target_uid, p_info);
			p_member->set_src(proto::common::recommend_source_top);
		}
	}
	//家族在线玩家
	uint64_t self_family = global_user_data_mgr_t::get_user_family_id(m_role_uid);
	family_ptr p_family = family_manager_t::get_family(self_family);
	if (NULL != p_family)
	{
		const family::family_member_map& family_members = p_family->get_all_member();
		std::vector<uint64_t> family_uids;
		for (auto it : family_members)
		{
			uint64_t uid = it.first;
			if (uid == m_role_uid)
			{
				continue;
			}
			proto::common::friend_type ft = get_friend_type_by_uid(uid);
			if (ft >= proto::common::friend_type_one_side_friend)
			{
				continue;
			}
			global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
			if (NULL == p_user)
			{
				continue;
			}
			if (std::find(uids.begin(), uids.end(), uid) != uids.end())
			{
				continue;
			}
			family_uids.push_back(uid);
		}
		if (!family_uids.empty())
		{
			uint32_t random_num = 1;
			if (uids.size() < 2)
			{
				random_num = 2;
			}
			std::vector<uint64_t> tmp_vec;
			random_util_t::random_from_vec(family_uids, random_num, tmp_vec);
			uids.insert(uids.end(), tmp_vec.begin(), tmp_vec.end());
			for (auto target_uid : tmp_vec)
			{
				proto::common::recommend_member* p_member = reply.add_recommend_friends();
				proto::common::user_info* p_info = p_member->mutable_info();
				friend_data_mgr_t::search_user_info_by_uid(target_uid, p_info);
				p_member->set_src(proto::common::recommend_source_family);
			}
		}
	}
	//离线竞技玩家池
	std::vector<uint64_t> offline_fight_uids;
	uint32_t fighting = global_user_data_mgr_t::get_user_fighting_value(m_role_uid);
	//log_warn("fighting[%d]", fighting);
	offline_fight_manager_t::get_friend_recommend(fighting, offline_fight_uids);
	uint32_t self_level = global_user_data_mgr_t::get_user_level(m_role_uid);

	std::vector<uint64_t> country_uids;
	std::vector<uint64_t> level_uids;
	std::vector<uint64_t> other_uids;
	for (auto uid : offline_fight_uids)
	{
		if (uid == m_role_uid)
		{
			continue;
		}
		proto::common::friend_type ft = get_friend_type_by_uid(uid);
		if (ft >= proto::common::friend_type_one_side_friend)
		{
			continue;
		}
		global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
		if (NULL == p_user)
		{
			continue;
		}
		if (std::find(uids.begin(), uids.end(), uid) != uids.end())
		{
			continue;
		}
		uint32_t country_id = global_user_data_mgr_t::get_user_country_id(uid);
		if (self_country > 0 && country_id == self_country)
		{
			country_uids.push_back(uid);
		}
		else
		{
			uint32_t level = global_user_data_mgr_t::get_user_level(uid);
			int32_t delta_level = level - self_level;
			if (abs(delta_level) <= 5)
			{
				level_uids.push_back(uid);
			}
			else
			{
				other_uids.push_back(uid);
			}
		}
	}
	//同一国家在线
	if (!country_uids.empty())
	{
		uint32_t random_num = 1;
		if (uids.size() < 3)
		{
			random_num = 2;
		}
		std::vector<uint64_t> tmp_vec;
		random_util_t::random_from_vec(country_uids, random_num, tmp_vec);
		uids.insert(uids.end(), tmp_vec.begin(), tmp_vec.end());
		for (auto target_uid : tmp_vec)
		{
			proto::common::recommend_member* p_member = reply.add_recommend_friends();
			proto::common::user_info* p_info = p_member->mutable_info();
			friend_data_mgr_t::search_user_info_by_uid(target_uid, p_info);
			p_member->set_src(proto::common::recommend_source_country);
		}
	}
	//等级相近在线
	if (!level_uids.empty())
	{
		uint32_t random_num = 2;
		if (uids.size() < 4)
		{
			random_num += 4 - uids.size();
			if (random_num > 4)
			{
				random_num = 4;
			}
		}
		std::vector<uint64_t> tmp_vec;
		random_util_t::random_from_vec(level_uids, random_num, tmp_vec);
		uids.insert(uids.end(), tmp_vec.begin(), tmp_vec.end());
		for (auto target_uid : tmp_vec)
		{
			proto::common::recommend_member* p_member = reply.add_recommend_friends();
			proto::common::user_info* p_info = p_member->mutable_info();
			friend_data_mgr_t::search_user_info_by_uid(target_uid, p_info);
			p_member->set_src(proto::common::recommend_source_same_level);
		}
	}
	//其他在线
	if (!other_uids.empty())
	{
		uint32_t random_num = 2;
		if (uids.size() < 6)
		{
			random_num += 6 - uids.size();
		}
		std::vector<uint64_t> tmp_vec;
		random_util_t::random_from_vec(level_uids, random_num, tmp_vec);
		uids.insert(uids.end(), tmp_vec.begin(), tmp_vec.end());
		for (auto target_uid : tmp_vec)
		{
			proto::common::recommend_member* p_member = reply.add_recommend_friends();
			proto::common::user_info* p_info = p_member->mutable_info();
			friend_data_mgr_t::search_user_info_by_uid(target_uid, p_info);
			p_member->set_src(proto::common::recommend_source_rand);
		}
	}
	//不够用其他在线玩家补齐
	if (uids.size() < 8)
	{
		uint32_t rest_count = 8 - uids.size();
		const global_user_manager_t::global_user_map_type& all_global_users = global_user_manager_t::get_all_global_users();
		for (auto it : all_global_users)
		{
			uint64_t uid = it.first;
			if (uid == m_role_uid)
			{
				continue;
			}
			proto::common::friend_type ft = get_friend_type_by_uid(uid);
			if (ft >= proto::common::friend_type_one_side_friend)
			{
				continue;
			}
			if (std::find(uids.begin(), uids.end(), uid) != uids.end())
			{
				continue;
			}
			proto::common::recommend_member* p_member = reply.add_recommend_friends();
			proto::common::user_info* p_info = p_member->mutable_info();
			friend_data_mgr_t::search_user_info_by_uid(uid, p_info);
			p_member->set_src(proto::common::recommend_source_rand);
			--rest_count;
			if (0 == rest_count)
			{
				break;
			}
		}
	}
}


void friend_mgr_t::top_friend_by_uid(uint64_t friend_uid, proto::client::ec_top_friend_reply& reply)
{
	if (m_top_size >= friend_manager_t::get_friend_cnt5())
	{
		reply.set_reply_code(errcode_enum::friend_error_excess_max_top_limit);
		return;
	}

	// 查询自己好友列表有没这个人的信息
	friend_info_ptr p_friend = get_friend_by_id(friend_uid);
	if (p_friend == NULL)
	{
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	// 不是好友在置顶
	proto::common::friend_type old_type = p_friend->m_member.ftype();
	if (old_type != proto::common::friend_type_one_side_friend && old_type != proto::common::friend_type_two_side_friend)
	{
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	// 已经是了
	if (p_friend->m_member.is_top() > 0)
	{
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	uint32_t time_now = common::time_util_t::now_time();

	p_friend->m_member.set_top_time(time_now);
	p_friend->m_member.set_is_top(1);

	friend_data_mgr_t::update_top_time(m_role_uid, p_friend->m_redis_key, true, time_now);

	WRITE_FRIEND_LOG_TO_DBS(m_role_uid, friend_uid, old_type, old_type, common::log_enum::source_type_friend_top_friend, 0);

	m_top_size++;

	reply.set_friend_uid(common::string_util_t::uint64_to_string(friend_uid));
	reply.set_reply_code(errcode_enum::error_ok);
}


void friend_mgr_t::cancel_top_friend_by_uid(uint64_t friend_uid, proto::client::ec_cancel_top_friend_reply& reply)
{
	// 查询自己好友列表有没这个人的信息
	friend_info_ptr p_friend = get_friend_by_id(friend_uid);

	if (p_friend == NULL)
	{
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	// 不是好友在取消置顶
	proto::common::friend_type old_type = p_friend->m_member.ftype();
	if (old_type != proto::common::friend_type_one_side_friend && old_type != proto::common::friend_type_two_side_friend)
	{
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	// 已经取消了
	if (p_friend->m_member.is_top() == 0)
	{
		reply.set_reply_code(errcode_enum::notice_unknown);
		return;
	}

	clear_top_info(p_friend);

	reply.set_friend_uid(common::string_util_t::uint64_to_string(friend_uid));
	reply.set_reply_code(errcode_enum::error_ok);
}

void friend_mgr_t::add_apply_time(uint64_t friend_uid, uint32_t time_now)
{
	bool is_new_apply = false;

	friend_info_ptr p_friend = get_friend_by_id(friend_uid);

	proto::common::friend_type old_type = proto::common::friend_type_none;
	if (p_friend != NULL)
	{
		old_type = p_friend->m_member.ftype();
	}

	if (p_friend != NULL)
	{
		// 排除一些错误的关系
		if (old_type >= proto::common::friend_type_one_side_friend)
		{
			log_error("role[%lu] add apply time error friend[%lu] friend_type = [%u]", m_role_uid, friend_uid, old_type);
			return;
		}

		// 是否是新的申请
		if (p_friend->m_member.time() == 0)
		{
			is_new_apply = true;
		}

		p_friend->m_member.set_time(time_now);

		// 不更新社会关系只更新申请好友时间
		friend_data_mgr_t::update_apply_time(m_role_uid, p_friend->m_redis_key, time_now);

		WRITE_FRIEND_LOG_TO_DBS(m_role_uid, friend_uid, old_type, old_type, common::log_enum::source_type_friend_update_apply_time, EM_FRIEND_PARAM::FRIEND_ADD_FRIEND_ME);
	}
	else
	{
		p_friend = friend_info_ptr(new friend_info_t());
		if (NULL == p_friend)
		{
			log_error("role[%lu] p_friend null error", m_role_uid);
			return;
		}

		is_new_apply = true;

		std::string rk;
		std::ostringstream oss;
		oss << m_role_uid << friend_uid;

		p_friend->m_friend_id = friend_uid;
		p_friend->m_redis_key = oss.str();
		p_friend->m_member.set_ftype(proto::common::friend_type_stranger);
		p_friend->m_member.set_friend_uid(common::string_util_t::uint64_to_string(friend_uid));
		p_friend->m_member.set_contact_time(0);
		p_friend->m_member.set_top_time(0);
		p_friend->m_member.set_is_top(0);
		p_friend->m_member.set_time(time_now);
		p_friend->m_member.set_shadow_time(0);
		p_friend->m_member.set_shadow_count(0);
		p_friend->m_member.set_friendliness(0);
		p_friend->m_member.set_friendliness_added_today(0);
		p_friend->m_member.set_friendliness_refresh_time(0);
		p_friend->m_member.set_can_get_friendship_point(0);
		p_friend->m_member.set_last_get_fp_time(0);
		p_friend->m_member.set_last_sent_fp_time(0);

		// 更新社会关系
		update_friend_info(proto::common::friend_type_none, p_friend, true);
	}

	if (is_new_apply)
	{
		// 更新好友申请列表，必须留出一个空位
		if (m_apply_size >= friend_manager_t::get_friend_cnt4())
		{
			uint32_t del_num = m_apply_size - friend_manager_t::get_friend_cnt4() + 1;

			clear_apply_list(del_num);
		}

		// 加入好友申请列表
		m_apply_list.push_back(p_friend);
		m_apply_size++;
	}
	red_point_manager_t::red_point_notify(m_role_uid, proto::common::RED_POINT_FRIEND_APPLY_LIST);
}


void friend_mgr_t::update_friend_info(proto::common::friend_type old_type, friend_info_ptr p_friend, bool is_new_friend)
{
	if (p_friend == NULL)
	{
		log_error("role[%lu] p_friend null error", m_role_uid);
		return;
	}

	// 新的社会关系
	if (is_new_friend)
	{
		//写入redis
		friend_data_mgr_t::update_all_friend_info(m_role_uid, p_friend);

		// 设置一些最基本的信息
		friend_data_mgr_t::load_user_base_info(p_friend->m_friend_id, p_friend->m_member.mutable_info());

		//插入m_friend_relations
		m_friend_relations.insert(std::make_pair(p_friend->m_friend_id, p_friend));

		WRITE_FRIEND_LOG_TO_DBS(m_role_uid, p_friend->m_friend_id, old_type, p_friend->m_member.ftype(), common::log_enum::source_type_friend_new_relation, 0);
	}
	else
	{
		friend_data_mgr_t::update_friend_type(m_role_uid, p_friend->m_redis_key, p_friend->m_member.ftype());
		friend_data_mgr_t::update_friend_friendliness_in_redis(m_role_uid, p_friend->m_friend_id, p_friend->m_member.friendliness());
		WRITE_FRIEND_LOG_TO_DBS(m_role_uid, p_friend->m_friend_id, old_type, p_friend->m_member.ftype(), common::log_enum::source_type_friend_update_friend_type, 0);
	}

	switch (p_friend->m_member.ftype()) {
		case proto::common::friend_type_two_side_friend: {
			do_two_side_friend(p_friend);
		}
		break;
		default:
			break;
	}
}


void friend_mgr_t::update_friend_two_side(uint64_t friend_uid)
{
	friend_info_ptr p_friend = get_friend_by_id(friend_uid);
	if (p_friend == NULL || p_friend->m_member.ftype() != proto::common::friend_type_one_side_friend)
	{
		log_error("role[%lu] change friend[%lu] friend_type to friend_type_two_side_friend error", m_role_uid, friend_uid);
		return;
	}

	p_friend->m_member.set_ftype(proto::common::friend_type_two_side_friend);

	update_friend_info(proto::common::friend_type_one_side_friend, p_friend, false);
}

void friend_mgr_t::update_friend_one_side(uint64_t friend_uid)
{
	friend_info_ptr p_friend = get_friend_by_id(friend_uid);
	if (p_friend == NULL || p_friend->m_member.ftype() != proto::common::friend_type_two_side_friend)
	{
		log_error("role[%lu] change friend[%lu] friend_type to friend_type_one_side_friend error", m_role_uid, friend_uid);
		return;
	}

	p_friend->m_member.set_ftype(proto::common::friend_type_one_side_friend);

	update_friend_info(proto::common::friend_type_two_side_friend, p_friend, false);
}


bool friend_mgr_t::update_contact_time(uint64_t friend_uid, uint32_t time_now, bool is_need_send /*= true*/)
{
	bool is_new_contact = false;

	friend_info_ptr p_friend = get_friend_by_id(friend_uid);

	// 已经有关系了不需要重新建立，直接更新时间
	if (p_friend != NULL)
	{
		// 是否是新的最近联系人
		if (p_friend->m_member.contact_time() == 0)
		{
			is_new_contact = true;
		}

		p_friend->m_member.set_contact_time(time_now);

		friend_data_mgr_t::update_contact_time(m_role_uid, p_friend->m_redis_key, time_now);

		WRITE_FRIEND_LOG_TO_DBS(m_role_uid, friend_uid, p_friend->m_member.ftype(), p_friend->m_member.ftype(), common::log_enum::source_type_friend_update_content_time, 0);
	}
	else // 需要重新建立
	{
		p_friend = friend_info_ptr(new friend_info_t());

		if (NULL == p_friend)
		{
			log_error("role[%lu] p_friend null error", m_role_uid);
			return false;
		}

		is_new_contact = true;

		std::string rk;
		std::ostringstream oss;
		oss << m_role_uid << friend_uid;

		p_friend->m_friend_id = friend_uid;
		p_friend->m_redis_key = oss.str();
		p_friend->m_member.set_ftype(proto::common::friend_type_stranger);
		p_friend->m_member.set_friend_uid(common::string_util_t::uint64_to_string(friend_uid));
		p_friend->m_member.set_contact_time(time_now);
		p_friend->m_member.set_top_time(0);
		p_friend->m_member.set_is_top(0);
		p_friend->m_member.set_time(0);
		p_friend->m_member.set_shadow_time(0);
		p_friend->m_member.set_shadow_count(0);
		p_friend->m_member.set_friendliness(0);
		p_friend->m_member.set_friendliness_added_today(0);
		p_friend->m_member.set_friendliness_refresh_time(0);
		p_friend->m_member.set_can_get_friendship_point(0);
		p_friend->m_member.set_last_get_fp_time(0);
		p_friend->m_member.set_last_sent_fp_time(0);

		// 更新社会关系
		update_friend_info(proto::common::friend_type_none, p_friend, true);
	}

	if (is_new_contact)
	{
		// 更新最近联系人，必须留出一个空位
		if (m_contact_size >= friend_manager_t::get_friend_cnt1())
		{
			uint32_t del_num = m_contact_size - friend_manager_t::get_friend_cnt1() + 1;

			clear_contact_list(del_num);
		}

		// 加入好友申请列表
		m_contact_list.push_back(p_friend);
		m_contact_size++;
	}

	if (is_need_send && is_new_contact)
	{
		// 通知一下客户端
		global_user_ptr p_role = global_user_manager_t::get_global_user_by_uid(m_role_uid);
		if (NULL != p_role)
		{
			// 发送前更新下玩家数据
			friend_data_mgr_t::update_user_info(p_friend);

			proto::client::ec_friend_member_update_notify ntf;
			proto::common::friend_member* p_member = ntf.mutable_member();
			if (p_member != NULL)
			{
				p_member->CopyFrom(p_friend->m_member);
			}
			p_role->send_msg_to_client(op_cmd::ec_friend_member_update_notify, ntf);
		}
	}

	return true;
}

proto::common::friend_type friend_mgr_t::get_friend_type_by_uid(uint64_t friend_uid)
{
	friend_info_ptr p_friend = get_friend_by_id(friend_uid);

	if (p_friend != NULL)
	{
		return p_friend->m_member.ftype();
	}

	return proto::common::friend_type_none;
}

void friend_mgr_t::do_two_side_friend(friend_info_ptr p_friend)
{
	if (NULL == p_friend)
	{
		log_error("NULL == p_friend");
		return;
	}
	// TODO:
	global_user_ptr p_role = global_user_manager_t::get_global_user_by_uid(m_role_uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", m_role_uid);
		return;
	}
	proto::server::eg_two_side_friend_notify ntf;
	ntf.set_friend_uid(p_friend->m_friend_id);
	p_role->send_msg_to_game(op_cmd::eg_two_side_friend_notify, ntf);
}

Friendliness* friend_mgr_t::get_config_by_friendliness(int32_t friendliness_value)
{
	if (friendliness_value <= 0){
		if (friendliness_value == 0) {
			FRIEND_LOG("friendliness_value is 0");
		}
		else {
			log_error("friendliness_value[%d] is wrong", friendliness_value);
		}
		return NULL;
	}
	uint32_t size = FriendlinessManager::getInstancePtr()->getSize();
	for (uint32_t i = 1; i <= size; ++i) {
		auto* config = GET_CONF(Friendliness, i);
		if (config == NULL) {
			log_error("id[%d] is out of config[FriendlinessManager] range", i);
			continue;
		}
		if (friendliness_value <= config->friendliness_range()){
			FRIEND_LOG("return friendliness config[%u]", config->id());
			return config;
		}
	}
	log_error("friendliness_value[%d] is out of range", friendliness_value);
	return NULL;
}

ChangeFriendlinessErrCode friend_mgr_t::change_friendliness_value_logic(int32_t& player_friendliness_value,
	int32_t& player_friendliness_added_today, int32_t& change_value)
{
	//1、检查player_friendliness_value和player_friendliness_added_today本身是否合法
	if (player_friendliness_value < 0 || player_friendliness_added_today < 0)
	{
		log_error("friendliness error player_friendliness_added_today %d or player_friendliness_value %d wrong", player_friendliness_added_today, player_friendliness_value);
		change_value = 0;
		return ChangeFriendlinessErrCode::ChangeFriendlinessFailUnknown;
	}
	if (change_value > 0 && player_friendliness_value >= friend_manager_t::get_max_friendliness()) {
		change_value = 0;
		FRIEND_LOG("user_friendliness_value[%d] reaches total limit", player_friendliness_value);
		return ChangeFriendlinessErrCode::ChangeFriendlinessReachTotalLimit;
	}
	if (change_value > 0 && player_friendliness_added_today >= friend_manager_t::get_max_friendliness_add_one_day()) {
		change_value = 0;
		FRIEND_LOG("user_friendliness_value[%d] reaches day limit", player_friendliness_value);
		return ChangeFriendlinessErrCode::ChangeFriendlinessReachDayLimit;
	}
	
	//2、检查player_friendliness_value加上change_value后是否合法
	//先检查是否超过了总限制
	ChangeFriendlinessErrCode err_code = ChangeFriendlinessErrCode::ChangeFriendlinessOK;
	int32_t no_check_total = player_friendliness_value + change_value;
	if (no_check_total < 0) {
		change_value = -player_friendliness_value;
	}
	if (no_check_total >= friend_manager_t::get_max_friendliness()) {
		change_value -= (no_check_total - friend_manager_t::get_max_friendliness());
		err_code = ChangeFriendlinessErrCode::ChangeFriendlinessReachDayLimit;
	}
	//再检查是否超过了每日限制
	int32_t no_check_today = player_friendliness_added_today;
	if (change_value > 0)
		no_check_today += change_value;
	
	if (no_check_today >= friend_manager_t::get_max_friendliness_add_one_day()) {
		change_value -= (no_check_today - friend_manager_t::get_max_friendliness_add_one_day());
		err_code = ChangeFriendlinessErrCode::ChangeFriendlinessReachDayLimit;
	}
	
	//3、结算
	player_friendliness_value += change_value;
	if(change_value > 0)
		player_friendliness_added_today += change_value;
	FRIEND_LOG("change_friendliness_value_logic get err_code[%d]", err_code);
	return err_code;
}

ChangeFriendlinessErrCode friend_mgr_t::change_online_friendliness_value(friend_info_ptr p_friend, int32_t& change_value)
{
	if (p_friend == NULL) {
		log_error("p_friend is NULL");
		return ChangeFriendlinessErrCode::ChangeFriendlinessFailUnknown;
	}
	int32_t added_today = p_friend->m_member.friendliness_added_today();
	int32_t current_value = p_friend->m_member.friendliness();
	ChangeFriendlinessErrCode code = change_friendliness_value_logic(current_value, added_today, change_value);

	p_friend->m_member.set_friendliness(current_value);
	p_friend->m_member.set_friendliness_added_today(added_today);
	FRIEND_LOG("online friend[%lu] get a change friendliness return code[%d]", p_friend->m_friend_id, code);
	return code;
}

void friend_mgr_t::clear_friendliness_value_and_save_data(uint64_t my_role_id, uint64_t friend_role_id, FRIENDLINESS_CHANGE_REASON reason)
{
	int32_t current_value = friend_data_mgr_t::get_offline_friend_friendliness(my_role_id, friend_role_id);
	change_friendliness_and_save_data(my_role_id, friend_role_id, -current_value, reason);
}

void friend_mgr_t::change_friendliness_and_save_data(uint64_t my_role_uid, uint64_t friend_uid, int32_t value, FRIENDLINESS_CHANGE_REASON reason, bool notify_client, int32_t* real_change, ChangeFriendlinessErrCode* output_err_code)
{
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(my_role_uid);
	int32_t change_value = value;
	int32_t old_value = 0;
	int32_t new_value = 0;
	int32_t added_today = 0;
	ChangeFriendlinessErrCode err_code = ChangeFriendlinessErrCode::ChangeFriendlinessOK;
	if (output_err_code != NULL)
		*output_err_code = err_code;
	//在线
	if (p_user != NULL)
	{
		friend_info_ptr p_friend = p_user->get_friend_mgr().get_friend_by_id(friend_uid);
		if (p_friend != NULL){
			old_value = p_friend->m_member.friendliness();
			err_code = friend_mgr_t::change_online_friendliness_value(p_friend, change_value);
			new_value = old_value + change_value;
			added_today = p_friend->m_member.friendliness_added_today();
			p_user->get_friend_mgr().update_current_relation_level();
			notify_game_friendliness_change(my_role_uid, friend_uid, change_value, (proto::common::change_friendliness_reason)reason);
			if (notify_client)
			{

				std::string name = global_user_data_mgr_t::get_name(friend_uid);
				p_user->get_friend_mgr().notify_player_friendliness_change(friend_uid, name, change_value, err_code, (proto::common::change_friendliness_reason)reason);
				//只要增加就通知
				if (change_value > 0){
					BROADCAST_SYS_NOTICE_SYS_CHANNEL(my_role_uid, proto::common::sys_notify_type::sys_notify_type_friendliness, 0, name.c_str(), change_value);
				}
				//达到上限也通知
				switch (err_code)
				{
				case ChangeFriendlinessErrCode::ChangeFriendlinessReachDayLimit:{
					BROADCAST_SYS_NOTICE_SYS_CHANNEL(my_role_uid, proto::common::sys_notify_type::sys_notify_type_friendliness_limit, 1, name.c_str());
				}
					break;
				case ChangeFriendlinessErrCode::ChangeFriendlinessReachTotalLimit:{
					BROADCAST_SYS_NOTICE_SYS_CHANNEL(my_role_uid, proto::common::sys_notify_type::sys_notify_type_friendliness_limit, 2, name.c_str());
				}
					break;
				default:
					break;
				}
			}
		}
		else{
			log_error("NULL == p_friend[%lu], my_role_id[%lu]", friend_uid, my_role_uid);
			if (output_err_code != NULL){
				*output_err_code = ChangeFriendlinessErrCode::ChangeFriendlinessFailUnknown;
			}
			return;
		}
	}
	//离线
	else
	{
		int32_t current_value = friend_data_mgr_t::get_offline_friend_friendliness(my_role_uid, friend_uid);
		old_value = current_value;
		added_today = friend_data_mgr_t::get_offline_friend_friendliness_added_today(my_role_uid, friend_uid);
		int32_t last_refresh_time = friend_data_mgr_t::get_offline_friend_friendliness_last_refresh_time(my_role_uid, friend_uid);
		if (friend_mgr_t::check_if_need_reset_friendliness_added_today(last_refresh_time)) {
			friend_data_mgr_t::reset_offline_friend_friendliness_added_today(my_role_uid, friend_uid);
			added_today = 0;
		}
		friend_mgr_t::change_friendliness_value_logic(current_value, added_today, change_value);
		new_value = current_value;
	}
	//更新redis
	if (change_value > 0){
		friend_data_mgr_t::update_friend_friendliness_in_redis(friend_uid, my_role_uid, new_value);
		friend_data_mgr_t::update_offline_friend_friendliness_added_today(friend_uid, my_role_uid, added_today);
	}
	if (real_change != NULL)
		*real_change = change_value;
	FRIEND_LOG("user[%lu] got an err_code[%d] when change friend's[%lu] friendliness", my_role_uid, err_code, friend_uid);
	if (change_value != 0) {
		if (change_value > 0) {
			WRITE_FRIEND_LOG_TO_DBS(my_role_uid, friend_uid, old_value, new_value, common::log_enum::source_type_friend_plus_friendliness, reason);
		}
		else {
			WRITE_FRIEND_LOG_TO_DBS(my_role_uid, friend_uid, old_value, new_value, common::log_enum::source_type_friend_minus_friendliness, reason);
		}
	}
}

bool friend_mgr_t::check_if_need_reset_friendliness_added_today(uint32_t last_refresh_time)
{
	return !time_manager_t::check_same_period(common::time_type_enum::time_one_day, last_refresh_time);
}

bool friend_mgr_t::check_if_need_reset_fp_count(uint32_t last_refresh_time)
{
	return !time_manager_t::check_same_period(common::time_type_enum::time_one_day, last_refresh_time);
}

bool friend_mgr_t::check_if_last_send_time_expired(uint32_t last_send_time)
{
	return !time_manager_t::check_same_period(common::time_type_enum::time_one_day, last_send_time);
}

bool friend_mgr_t::check_if_last_receive_time_expired(uint32_t last_receive_time)
{
	return !time_manager_t::check_same_period(common::time_type_enum::time_one_day, last_receive_time);
}

void friend_mgr_t::set_can_get_fp(uint64_t role_id, uint64_t friend_id, uint32_t value)
{
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(role_id);
	//在线
	if (p_user != NULL){
		friend_info_ptr p_friend = p_user->get_friend_mgr().get_friend_by_id(friend_id);
		if (p_friend != NULL){
			p_friend->m_member.set_can_get_friendship_point(value);
		}
		else{
			log_error("NULL == p_friend, my_role_id[%lu], friend_role_id[%lu]", role_id, friend_id);
			return;
		}
	}
	//在线离线都需要更新redis
	friend_data_mgr_t::update_i_can_get_fp_from_friend(role_id, friend_id, value);
}

uint32_t friend_mgr_t::check_if_can_get_relation_award()
{
	uint32_t size = FriendAwardManager::getInstancePtr()->getSize();
	uint32_t award_level = 0;
	for (uint32_t i = 1; i <= size; ++i){
		if (!check_if_can_get_relation_award_by_level(i)){
			return award_level;
		}
		else
			award_level = i;
	}
	return award_level;
}

uint32_t friend_mgr_t::get_satisfied_relation_award_friends_quantity(uint32_t level)
{
	if (level == 0){
		FRIEND_LOG("relation award level is 0");
		return 0;
	}
	auto* config = GET_CONF(FriendAward, level);
	if (config == NULL){
		log_error("relation award level[%u] is out of config[FriendAward] range", level);
		return 0;
	}
	int32_t amount = 0;
	for (auto it = m_friend_relations.begin(); it != m_friend_relations.end(); ++it)
	{
		if(it->second == NULL){
			log_error("NULL in m_friend_relations, rold_id:%lu", m_role_uid);
			continue;
		}
		if (it->second->m_member.friendliness() >= config->need_friendliness())
			++amount;
	}
	return amount;
}

bool friend_mgr_t::check_if_can_get_relation_award_by_level(uint32_t level)
{
	if (level == 0) {
		FRIEND_LOG("no award because relation award level is 0");
		return false;
	}
	auto* config = GET_CONF(FriendAward, level);
	if (config == NULL) {
		log_error("level[%u] is out of config[FriendAward] range", level);
		return false;
	}
	int32_t amount = get_satisfied_relation_award_friends_quantity(level);
	if (amount >= config->need_friends_count())
		return true;
	return false;
}

bool friend_mgr_t::check_if_has_got_relation_award(uint32_t level)
{
	if (level == 0 || level > sizeof(uint32_t))
		return false;
	return (1 >> (level - 1)) & m_got_relation_award;
}

void friend_mgr_t::mark_has_got_relation_award(uint32_t level)
{
	if (level == 0 || level > sizeof(uint32_t))
		return;
	m_got_relation_award |= (1 >> (level - 1));
}

void friend_mgr_t::get_relation_award(uint32_t level)
{
	if (check_if_has_got_relation_award(level)) {
		FRIEND_LOG("no award can be got at this relation award level[%u]", level);
		send_msg_to_client_of_get_relation_award_reply(level, errcode_enum::qinmidu_reward_error, NULL);
		return;
	}
	auto* config = GET_CONF(FriendAward, level);
	if (config == NULL) {
		log_error("level[%u] is out of config[FriendAward] range", level);
		send_msg_to_client_of_get_relation_award_reply(level, errcode_enum::notice_unknown, NULL);
		return;
	}
	//告诉gameserver发送道具config->award_id()
	friend_mgr_t::request_gameserver_to_do_friend_point_operation(m_role_uid, 0, proto::server::player_get_relation_award, config->award_id(), level);
}

void friend_mgr_t::get_relation_award_callback(uint32_t level, int32_t reply_code, const proto::common::role_change_data* p_change_data)
{
	auto* config = GET_CONF(FriendAward, level);
	if (config == NULL) {
		log_error("level[%u] is out of config[FriendAward] range", level);
		return;
	}

	m_fp_send_limit = config->give_count();
	m_fp_receive_limit = config->get_count();
	save_send_limit();
	save_receive_limit();
	mark_has_got_relation_award(level);
	save_got_relation_award();
	send_msg_to_client_of_get_relation_award_reply(level, reply_code, p_change_data);

	WRITE_FRIEND_LOG_TO_DBS(m_role_uid, 0, 0, 0, common::log_enum::source_type_friend_get_relation_award, level);
}

void friend_mgr_t::send_msg_to_client_of_get_relation_award_reply(uint32_t level, int32_t reply_code, const proto::common::role_change_data* p_change_data)
{
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(m_role_uid);
	if (NULL == p_user){
		log_error("user[%lu] null error", m_role_uid);
		return;
	}
	proto::client::ec_get_relation_award_reply reply;
	reply.set_reply_code(reply_code);
	reply.set_level(level);
	if (p_change_data != NULL){
		auto* p = reply.mutable_change_data();
		if (p != NULL)
			p->CopyFrom(*p_change_data);
	}
	p_user->send_msg_to_client(op_cmd::ec_get_relation_award_reply, reply);
}

void friend_mgr_t::update_current_relation_level()
{
	uint32_t current_level = check_if_can_get_relation_award();
	if (current_level > m_current_relation_level){
		m_current_relation_level = current_level;
		notify_client_can_get_relation_award();
	}
}

uint32_t friend_mgr_t::get_can_get_relation_award()
{
	if (m_current_relation_level == 0) {
		FRIEND_LOG("no relation award can be got");
		return 0;
	}
	uint32_t can_get_relation_award = 1;
	bit_util_t::move_left_filling_1(can_get_relation_award, m_current_relation_level - 1);
	can_get_relation_award -= m_got_relation_award;
	return can_get_relation_award;
}

void friend_mgr_t::notify_client_can_get_relation_award()
{
	if (get_can_get_relation_award() != 0){
		red_point_manager_t::red_point_notify(m_role_uid, proto::common::RED_POINT_TYPE_RELATION_AWARD);
	}
}

void friend_mgr_t::tell_client_data_on_relation_interface()
{
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(m_role_uid);
	if (NULL == p_user){
		log_error("user[%lu] null error", m_role_uid);
		return;
	}
	proto::client::ec_get_relation_award_data_reply reply;

	for (uint32_t i = 1; i <= m_current_relation_level; ++i){
		auto* config = GET_CONF(FriendAward, i);
		if (config == NULL){
			log_error("level[%u] is out of config[FriendAward] range", i);
			continue;
		}
		auto* p_data = reply.add_data();
		p_data->set_level(i);
		p_data->set_friends_quantity(config->need_friends_count());
		p_data->set_has_got(check_if_has_got_relation_award(i));
	}

	uint32_t max_level = FriendAwardManager::getInstancePtr()->getSize();
	for (uint32_t i = m_current_relation_level + 1; i <= max_level; ++i){
		auto* p_data = reply.add_data();
		p_data->set_level(i);
		p_data->set_friends_quantity(get_satisfied_relation_award_friends_quantity(i));
		p_data->set_has_got(false);
	}

	p_user->send_msg_to_client(op_cmd::ec_get_relation_award_data_reply, reply);
}

void friend_mgr_t::give_present(uint64_t target_role_id, uint32_t item_id, uint32_t item_quantity, bool confirm)
{
	//1、检查目标id是否存在
	if (!friend_data_mgr_t::is_user_key_exsit(target_role_id))
	{
		send_msg_to_client_of_give_present_reply(target_role_id, errcode_enum::gift_give_tips_03, NULL, 0);
		FRIEND_LOG("target_role_id[%lu] is not exist", target_role_id);
		return;
	}
	//2、检查非双向好友是否已确认
	if (!confirm && get_friend_type_by_uid(target_role_id) != proto::common::friend_type_two_side_friend)
	{
		send_msg_to_client_of_give_present_reply(target_role_id, errcode_enum::gift_give_tips_04, NULL, 0);
		FRIEND_LOG("no confirm when send gift to not_two_side_friend");
		return;
	}
	//3、向game_server请求扣除礼物
	friend_mgr_t::request_gameserver_to_do_friend_point_operation(m_role_uid, target_role_id, proto::server::player_give_present, item_id, item_quantity);
}

void friend_mgr_t::give_present_callback(uint64_t target_role_id, int32_t reply_code, const proto::common::role_change_data * p_change_data, uint32_t item_id, uint32_t item_quantity, uint32_t item_add_friendliness, uint32_t item_add_charm, const std::string& item_name, uint32_t language_id)
{
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(m_role_uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", m_role_uid);
		return;
	}
	FRIEND_LOG("user[%lu] give quantity[%u] present[%u] to friend[%lu] callback get a reply_code[%d], item_add_friendliness[%u], item_add_charm[%u], language_id[%u]", m_role_uid, item_quantity, item_id, target_role_id, reply_code, item_add_friendliness, item_add_charm, language_id);
	if (reply_code != 0)
	{
		send_msg_to_client_of_give_present_reply(m_role_uid, reply_code, NULL, 0);
		return;
	}
	//增加被送礼物者的魅力值
	uint32_t target_charm = friend_mgr_t::add_charm(target_role_id, item_quantity * item_add_charm);
	uint32_t add_friendliness = 0;
	int32_t role_real_change = 0;
	int32_t target_real_change = 0;
	if (get_friend_type_by_uid(target_role_id) == proto::common::friend_type_two_side_friend)
	{
		add_friendliness = item_quantity * item_add_friendliness;
		change_friendliness_and_save_data(m_role_uid, target_role_id, add_friendliness, FRIENDLINESS_CHANGE_REASON::SEND_GIFT, true, &role_real_change);
		change_friendliness_and_save_data(target_role_id, m_role_uid, add_friendliness, FRIENDLINESS_CHANGE_REASON::SEND_GIFT, true, &target_real_change);
	}
	send_msg_to_client_of_give_present_reply(target_role_id, reply_code, p_change_data, (uint32_t)role_real_change);
	//通知被送礼物的人
	notify_player_get_present(item_id, item_quantity, target_charm, m_role_uid, p_user->get_name(), target_role_id, (uint32_t)target_real_change);
	//私聊
	proto::client::ce_personal_chat_request msg;
	msg.set_tar_role_id(string_util_t::uint64_to_string(target_role_id));
	msg.set_channel_id(p_user->get_channel_id());
	msg.set_level(p_user->get_level());
	msg.set_head_id(p_user->get_hero_plugin());
	auto* p_config = GET_CONF(Comprehensive, comprehensive_common::gift_talk);
	if (p_config != NULL)
	{
		std::string buff;
		StringTool::format(buff, common::string_util_t::convert_to_utf8(p_config->parameter1()).c_str(), item_quantity, item_name.c_str());
		msg.set_chat_info(buff);
	}
	//私聊
	chat_msg_handler_t::one_chat_to_another(m_role_uid, msg);
	//跑马灯
	if (language_id != 0)
	{
		BROADCAST_SYS_NOTICE(language_id, m_role_uid, 0, 0, p_user->get_name().c_str(), global_user_data_mgr_t::get_name(target_role_id).c_str());
	}
	WRITE_FRIEND_LOG_TO_DBS(m_role_uid, target_role_id, item_id, item_quantity, common::log_enum::source_type_friend_send_gift, 0);
}

void friend_mgr_t::send_msg_to_client_of_give_present_reply(uint64_t target_role_id, int32_t reply_code, const proto::common::role_change_data* p_change_data, uint32_t add_friendliness)
{
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(m_role_uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", m_role_uid);
		return;
	}
	proto::client::ec_give_present_reply reply;
	reply.set_target_role_uid(string_util_t::uint64_to_string(target_role_id));
	reply.set_reply_code(reply_code);
	reply.set_add_friendliness(add_friendliness);
	if (p_change_data != NULL)
	{
		auto* p = reply.mutable_change_data();
		if(p != NULL)
		 p->CopyFrom(*p_change_data);
	}
	p_user->send_msg_to_client(op_cmd::ec_give_present_reply, reply);
}

uint32_t friend_mgr_t::add_charm(uint64_t target_role_id, uint32_t add_value)
{
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(target_role_id);
	uint32_t charm = 0;
	uint32_t old_value = 0;
	uint32_t new_value = 0;
	if (p_user != NULL) // 在线
	{
		charm = p_user->get_charm();
		old_value = charm;
		add_charm_logic(charm, add_value);
		new_value = charm;
		p_user->set_charm(charm);
	}
	else
	{
		charm = global_user_data_mgr_t::get_user_charm(target_role_id);
		old_value = charm;
		add_charm_logic(charm, add_value);
		new_value = charm;
	}
	//更新排行榜
	rank_manager_t::update_rank_charm(target_role_id, charm);
	//存入redis
	global_user_data_mgr_t::set_user_charm(target_role_id, charm);
	// 同步game
	global_user_msg_handle_t::sync_data_to_game(target_role_id, proto::server::data_sync_type_charm, charm, true);
	//系统通知
	BROADCAST_SYS_NOTICE_SYS_CHANNEL(target_role_id, proto::common::sys_notify_type::sys_notify_type_charm, 0, new_value - old_value);
	WRITE_FRIEND_LOG_TO_DBS(target_role_id, 0, old_value, new_value, common::log_enum::source_type_friend_plus_charm, 0);
	FRIEND_LOG("user[%lu] add charm[%u]", target_role_id, add_value);
	return charm;
}

void friend_mgr_t::add_charm_logic(uint32_t & charm_value, uint32_t add_value)
{
	charm_value += add_value;
	auto* p_conf = GET_CONF(Comprehensive, comprehensive_common::meili_max);
	if (!p_conf) {
		log_error("can't find comprehensive_common::meili_max in Comprehensive");
		return;
	}
	uint32_t max = GET_COMPREHENSIVE_VALUE_1(p_conf);
	if (charm_value >= max)
		charm_value = max;
}

void friend_mgr_t::notify_player_get_present(uint32_t item_id, uint32_t item_quantity, uint32_t current_charm, uint64_t giver_role_id, const std::string& giver_role_name, uint64_t player_role_id, uint32_t add_friendliness)
{
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(player_role_id);
	if (NULL == p_user){
		FRIEND_LOG("user[%lu] in not online or not exist", player_role_id);
		return;
	}
	proto::client::ec_get_present_notify notify;
	notify.set_item_id(item_id);
	notify.set_item_quantity(item_quantity);
	notify.set_giver_role_id(string_util_t::uint64_to_string(giver_role_id));
	notify.set_giver_name(giver_role_name);
	notify.set_add_friendliness(add_friendliness);
	auto* p_change_data = notify.mutable_change_data();
	if (p_change_data != NULL)
		p_change_data->set_charm(current_charm);
	p_user->send_msg_to_client(op_cmd::ec_get_present_notify, notify);
}

void friend_mgr_t::reset_friendliness_added_today(int32_t& friendliness_added_today, uint32_t& friendliness_refresh_time, uint32_t now)
{
	friendliness_added_today = 0;
	friendliness_refresh_time = now;
}

void friend_mgr_t::notify_player_friendliness_change(uint64_t friend_role_id, const std::string& friend_role_name, int32_t change_value, int32_t err_code, proto::common::change_friendliness_reason reason)
{
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(m_role_uid);
	if (p_user == NULL) {
		FRIEND_LOG("user[%lu] in not online or not exist", m_role_uid);
		return;
	}
	proto::client::ec_friendliness_change_notify notify;
	notify.set_friend_role_id(string_util_t::uint64_to_string(friend_role_id));
	notify.set_friend_name(friend_role_name);
	notify.set_change_value(change_value);
	notify.set_err_code(err_code);
	notify.set_reason(reason);
	p_user->send_msg_to_client(op_cmd::ec_friendliness_change_notify, notify);
}

void friend_mgr_t::notify_game_friendliness_change(uint64_t my_role_id, uint64_t friend_role_id, int32_t change_value,
	proto::common::change_friendliness_reason reason)
{
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(my_role_id);
	if (p_user == NULL) {
		FRIEND_LOG("user[%lu] in not online or not exist", my_role_id);
		return;
	}
	proto::server::eg_change_friendliness_notify notify;
	notify.set_my_role_id(my_role_id);
	notify.set_friend_role_id(friend_role_id);
	notify.set_change_value(change_value);
	notify.set_reason(reason);
	p_user->send_msg_to_game(op_cmd::eg_change_friendliness_notify, notify);
}

void friend_mgr_t::update_online(uint32_t timestamp)
{
	if (friend_mgr_t::check_if_need_reset_friendliness_added_today(friend_mgr_t::s_m_last_reset_fat_time))
	{
		//遍历全体在线玩家
		const auto& user_map = global_user_manager_t::get_all_global_users();
		for (auto it = user_map.begin(); it != user_map.end(); ++it) {
			if (it->second == NULL) {
				continue;
			}
			auto& friend_mgr = it->second->get_friend_mgr();
			friend_mgr.reset_fat_in_my_relations(timestamp, true);
		}
		s_m_last_reset_fat_time = timestamp;
		log_info("reset all online players friendliness added today");
	}

	if (friend_mgr_t::check_if_need_reset_fp_count(friend_mgr_t::s_m_last_reset_fp_count_time))
	{
		//遍历全体在线玩家
		const auto& user_map = global_user_manager_t::get_all_global_users();
		for (auto it = user_map.begin(); it != user_map.end(); ++it) {
			if (it->second == NULL) {
				continue;
			}
			auto& friend_mgr = it->second->get_friend_mgr();
			friend_mgr.reset_fp_sent_count(timestamp, true);
			friend_mgr.reset_fp_received_count(timestamp, true);
		}
		s_m_last_reset_fp_count_time = timestamp;
		log_info("reset all online players receive and send friendship point count");
	}
	
}

void friend_mgr_t::modify_friendliness_data_on_player_enter()
{
	uint32_t now = common::time_util_t::now_time();
	reset_fat_in_my_relations(now, false);
}

void friend_mgr_t::modify_relation_award_data_on_player_enter()
{
	m_current_relation_level = check_if_can_get_relation_award();
}

void friend_mgr_t::reset_fat_in_my_relations(uint32_t now, bool reset_without_check)
{
	for (auto it = m_friend_relations.begin(); it != m_friend_relations.end(); ++it)
	{
		friend_info_ptr p_friend = it->second;
		if (p_friend == NULL) {
			log_error("user[%lu] has a NULL pointer(key is %lu) in m_friend_relations", m_role_uid, it->first);
			continue;
		}
		//更新过期的日增亲密度
		if (reset_without_check || friend_mgr_t::check_if_need_reset_friendliness_added_today(p_friend->m_member.friendliness_refresh_time()))
		{
			int32_t added_today = p_friend->m_member.friendliness_added_today();
			uint32_t refresh_time = p_friend->m_member.friendliness_refresh_time();
			friend_mgr_t::reset_friendliness_added_today(added_today,
				refresh_time, now);
			p_friend->m_member.set_friendliness_added_today(added_today);
			p_friend->m_member.set_friendliness_refresh_time(refresh_time);
			//更新redis
			friend_data_mgr_t::update_all_friend_info(m_role_uid, p_friend);
			FRIEND_LOG("user[%lu] reset friendliness added today at %u timestamp", m_role_uid, now);
		}
	}
}

void friend_mgr_t::pack_my_friendliness_data(proto::server::eg_get_friendliness_reply& reply)
{
	reply.set_role_id(m_role_uid);
	for (auto pair : m_friend_relations)
	{
		if (pair.second == NULL) {
			log_error("user[%lu] has a NULL pointer(key is %lu) in m_friend_relations", m_role_uid, pair.first);
			continue;
		}
		auto* p_data = reply.add_friendliness();
		// if (p_data == NULL) {
		// 	log_error("protobuf error");
		// 	continue;
		// }
		p_data->set_friend_role_id(pair.second->m_friend_id);
		p_data->set_friendliness(pair.second->m_member.friendliness());
	}
}

void friend_mgr_t::send_game_my_friendliness_data()
{
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(m_role_uid);
	if (p_user == NULL) {
		log_error("user[%lu] is not online and should be", m_role_uid);
		return;
	}
	proto::server::eg_get_friendliness_reply reply;
	pack_my_friendliness_data(reply);
	p_user->send_msg_to_game(op_cmd::eg_get_friendliness_reply, reply);
}

void friend_mgr_t::reset_fp_sent_count(uint32_t now, bool reset_without_check)
{
	if (reset_without_check || friend_mgr_t::check_if_need_reset_fp_count(m_last_refresh_sent_time)){
		m_fp_has_sent_today = 0;
		m_last_refresh_sent_time = now;
		save_fp_has_sent_today();
		save_refresh_sent_time();
	}
}

void friend_mgr_t::reset_fp_received_count(uint32_t now, bool reset_without_check)
{
	if (reset_without_check || friend_mgr_t::check_if_need_reset_fp_count(m_last_refresh_received_time)){
		m_fp_has_received_today = 0;
		m_last_refresh_received_time = now;
		save_fp_has_received_today();
		save_refresh_received_time();
	}
}

void friend_mgr_t::request_gameserver_to_do_friend_point_operation(uint64_t role_id, uint64_t friend_role_id, proto::server::friend_point_operation operation, int32_t parameter1, int32_t parameter2)
{
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(role_id);
	if (NULL == p_user){
		FRIEND_LOG("user[%lu] null", role_id);
		return;
	}
	proto::server::eg_friend_point_operation_request request;
	request.set_role_id(role_id);
	request.set_friend_role_id(friend_role_id);
	request.set_operation(operation);
	request.set_parameter1(parameter1);
	request.set_parameter2(parameter2);
	p_user->send_msg_to_game(op_cmd::eg_friend_point_operation_request, request);
}

int32_t friend_mgr_t::give_friend_friend_point(uint64_t friend_role_id)
{
	int32_t err_code = 0;
	//1检查今日次数限制
	if (m_fp_has_sent_today >= m_fp_send_limit + friend_manager_t::m_fp_send_initial_limit){
		err_code = errcode_enum::youqingdian_give_max;
		send_msg_to_client_of_give_friend_point_reply(friend_role_id, err_code, 0, 0);
		FRIEND_LOG("user[%lu] give friend point count[%u] reached limit[%u]", m_role_uid, m_fp_has_sent_today, m_fp_send_limit + friend_manager_t::m_fp_send_initial_limit);
		return err_code;
	}
	//2检查上次赠送对方的时间是否过期
	friend_info_ptr p_friend = get_friend_by_id(friend_role_id);
	if (p_friend == NULL) {
		log_error("NULL == p_friend[%lu], my_role_id[%lu]", friend_role_id, m_role_uid);
		err_code = errcode_enum::notice_unknown;
		send_msg_to_client_of_give_friend_point_reply(friend_role_id, err_code, 0, 0);
		return err_code;
	}
	if (!friend_mgr_t::check_if_last_send_time_expired(p_friend->m_member.last_sent_fp_time())) {
		FRIEND_LOG("user[%lu] give friend[%lu] friend point failed, reason:last sent time not expired", m_role_uid, friend_role_id);
		err_code = errcode_enum::youqingdian_tips_01;
		send_msg_to_client_of_give_friend_point_reply(friend_role_id, err_code, 0, 0);
		return err_code;
	}
	//3检查是否是双向好友
	if (p_friend->m_member.ftype() != proto::common::friend_type_two_side_friend){
		FRIEND_LOG("user[%lu] give friend[%lu] friend point failed, reason:not two side friend", m_role_uid, friend_role_id);
		err_code = errcode_enum::youqingdian_tips_03;
		send_msg_to_client_of_give_friend_point_reply(friend_role_id, err_code, 0, 0);
		return err_code;
	}
		
	//4赠送
	set_can_get_fp(friend_role_id, m_role_uid, friend_manager_t::get_initial_friends_friendliness());
	//增加双方的亲密度
	friend_mgr_t::change_friendliness_and_save_data(friend_role_id, m_role_uid, friend_manager_t::m_friendliness_added_by_send_fp, FRIENDLINESS_CHANGE_REASON::SEND_FRIEND_POINT);
	int32_t add_friendliness = 0;
	friend_mgr_t::change_friendliness_and_save_data(m_role_uid, friend_role_id, friend_manager_t::m_friendliness_added_by_send_fp, FRIENDLINESS_CHANGE_REASON::SEND_FRIEND_POINT, true, &add_friendliness);
	//5设置赠送时间、次数
	uint32_t now = common::time_util_t::now_time();
	++m_fp_has_sent_today;
	//存储
	save_fp_has_sent_today();
	p_friend->m_member.set_last_sent_fp_time(now);
	friend_data_mgr_t::update_last_i_sent_fp_timestamp(m_role_uid, friend_role_id, now);
	send_msg_to_client_of_give_friend_point_reply(friend_role_id, err_code, now, add_friendliness);
	WRITE_FRIEND_LOG_TO_DBS(m_role_uid, friend_role_id, 0, 0, common::log_enum::source_type_friend_send_fp, 0);
	return err_code;
}

int32_t friend_mgr_t::get_friend_point_from_friend(uint64_t friend_role_id)
{
	//1检查今日次数限制
	if (m_fp_has_received_today >= m_fp_receive_limit + friend_manager_t::m_fp_receive_initial_limit)
		return errcode_enum::youqingdian_receive_max;
	//2检查是否是双向好友
	friend_info_ptr p_friend = get_friend_by_id(friend_role_id);
	if (p_friend == NULL) {
		log_error("NULL == p_friend[%lu], my_role_id[%lu]", friend_role_id, m_role_uid);
		return errcode_enum::notice_unknown;
	}
	if (p_friend->m_member.ftype() != proto::common::friend_type_two_side_friend) {
		FRIEND_LOG("user[%lu] get friend[%lu] friend point failed, reason: not two side friend", m_role_uid, friend_role_id);
		return errcode_enum::youqingdian_tips_03;
	}
	//3检查对方是否有赠送
	if (!p_friend->m_member.can_get_friendship_point()) {
		FRIEND_LOG("user[%lu] get friend[%lu] friend point failed, reason: he didn't give", m_role_uid, friend_role_id);
		return errcode_enum::youqingdian_tips_04;
	}
	//4检查上次领取时间是否过期
	if (!friend_mgr_t::check_if_last_receive_time_expired(p_friend->m_member.last_get_fp_time())) {
		FRIEND_LOG("user[%lu] get friend[%lu] friend point failed, reason: last got time not expired", m_role_uid, friend_role_id);
		return errcode_enum::youqingdian_tips_02;
	}
	//5领取
	//根据亲密度获得领取值
	auto* config = friend_mgr_t::get_config_by_friendliness(p_friend->m_member.friendliness());
	if (config == NULL) {
		log_error("NULL == config[%d]", p_friend->m_member.friendliness());
		return errcode_enum::notice_unknown;
	}
	uint32_t add_value = config->friend_point();

	//通知game，请求更改友情点
	friend_mgr_t::request_gameserver_to_do_friend_point_operation(m_role_uid, friend_role_id, proto::server::player_get_point_from_friend, add_value, 0);
	FRIEND_LOG("user[%lu] pre-get friend[%lu] friend point success", m_role_uid, friend_role_id);
	return 0;
}

void friend_mgr_t::get_friend_point_from_friend_game_callback(uint64_t friend_role_id, uint32_t value, int32_t reply_code, const proto::common::role_change_data* p_change_data)
{
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(m_role_uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", m_role_uid);
		return;
	}

	friend_info_ptr p_friend = get_friend_by_id(friend_role_id);
	if (p_friend == NULL) {
		log_error("NULL == p_friend[%lu], my_role_id[%lu]", friend_role_id, m_role_uid);
		return;
	}
	uint32_t now = common::time_util_t::now_time();
	p_friend->m_member.set_last_get_fp_time(now);
	++m_fp_has_received_today;
	save_fp_has_received_today();
	set_can_get_fp(m_role_uid, friend_role_id, 0);
	friend_data_mgr_t::update_last_i_got_fp_timestamp(m_role_uid, friend_role_id, now);
	send_msg_to_client_of_get_friend_point_reply(friend_role_id, reply_code, now, p_change_data);

	WRITE_FRIEND_LOG_TO_DBS(m_role_uid, friend_role_id, 0, 0, common::log_enum::source_type_friend_get_fp, value);
}

void friend_mgr_t::send_msg_to_client_of_get_friend_point_reply(uint64_t friend_uid, int32_t reply_code, uint32_t get_timestamp, const proto::common::role_change_data* p_change_data)
{
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(m_role_uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", m_role_uid);
		return;
	}
	proto::client::ec_get_friend_point_reply reply;
	reply.set_friend_uid(string_util_t::uint64_to_string(friend_uid));
	reply.set_reply_code(reply_code);
	reply.set_get_timestamp(get_timestamp);
	if (p_change_data != NULL)
	{
		auto* p = reply.mutable_change_data();
		if (p != NULL)
			p->CopyFrom(*p_change_data);
	}
	p_user->send_msg_to_client(op_cmd::ec_get_friend_point_reply, reply);
}

void friend_mgr_t::send_msg_to_client_of_give_friend_point_reply(uint64_t friend_uid, int32_t reply_code, uint32_t give_timestamp, int32_t change_friendliness)
{
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(m_role_uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", m_role_uid);
		return;
	}
	proto::client::ec_give_friend_point_reply reply;
	reply.set_friend_uid(string_util_t::uint64_to_string(friend_uid));
	reply.set_reply_code(reply_code);
	reply.set_give_timestamp(give_timestamp);
	reply.set_add_friendliness(uint32_t(change_friendliness));
	p_user->send_msg_to_client(op_cmd::ec_give_friend_point_reply, reply);
}

void friend_mgr_t::save_fp_has_sent_today()
{
	friend_data_mgr_t::update_fp_has_sent_today(m_role_uid, m_fp_has_sent_today);
}

void friend_mgr_t::save_refresh_sent_time()
{
	friend_data_mgr_t::update_refresh_sent_time(m_role_uid, m_last_refresh_sent_time);
}

void friend_mgr_t::save_fp_has_received_today()
{
	friend_data_mgr_t::update_fp_has_received_today(m_role_uid, m_fp_has_received_today);
}

void friend_mgr_t::save_refresh_received_time()
{
	friend_data_mgr_t::update_refresh_received_time(m_role_uid, m_last_refresh_received_time);
}

void friend_mgr_t::save_send_limit()
{
	friend_data_mgr_t::update_send_limit(m_role_uid, m_fp_send_limit);
}

void friend_mgr_t::save_receive_limit()
{
	friend_data_mgr_t::update_receive_limit(m_role_uid, m_fp_receive_limit);
}

void friend_mgr_t::save_initialization()
{
	friend_data_mgr_t::update_initialization(m_role_uid);
}

void friend_mgr_t::save_got_relation_award()
{
	friend_data_mgr_t::update_got_relation_award(m_role_uid, m_got_relation_award);
}

void friend_mgr_t::load_compelete()
{
	FRIEND_LOG("user[%lu] friend_mgr load compelete, m_game_requested[%d]", m_role_uid, m_game_requested);
	m_data_loaded = true;
	if (m_game_requested == true)
	{
		send_game_my_friendliness_data();
		m_game_requested = false;
	}
}