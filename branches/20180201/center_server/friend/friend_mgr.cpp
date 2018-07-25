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

#define WRITE_FRIEND_LOG_TO_DBS(uid, friend_uid, oldtype, newtype, type, param)              \
    log_wrapper_t::send_friend_log(uid, friend_uid, oldtype, newtype, type, param);

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
			if (p_member != NULL)
				p_member->CopyFrom(p_friend->m_member);
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


void friend_mgr_t::tower_trigger_notify(const proto::server::ge_tower_trigger_notify& msg)
{
	global_user_ptr p_self = global_user_manager_t::get_global_user_by_uid(m_role_uid);
	if (NULL == p_self)
	{
		log_error("NULL == p_self[%lu]", m_role_uid);
		return;
	}
	auto p_notify_str = GET_CONF(Comprehensive, comprehensive_common::dreamland_hide_find);
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
		log_error("role[%lu] msg length[%d] error!", m_role_uid, msg.chat_info().length());
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

void friend_mgr_t::load_all_relations()
{
	friend_data_mgr_t::load_all_friends(m_role_uid, m_friend_relations, m_contact_list, m_contact_size, m_apply_list, m_apply_size, m_friend_size, m_black_size, m_top_size);

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
			if (p_member != NULL)
				p_member->CopyFrom(p_friend->m_member);
		}
	}

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
	}

	// 好友关系
	// 我相对与要加好友人关系
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

			// 更新社会关系
			update_friend_info(old_type, p_friend, is_new_friend);

			// 更新下好友关系
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
	achieve_common_t::notify_progress_state(m_role_uid, proto::common::ACHIEVE_HAVE_FRIEND_NUMBER);
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
	for (friend_info_map::iterator itr = m_friend_relations.begin();
		itr != m_friend_relations.end(); ++itr)
	{
		friend_info_ptr p_friend = itr->second;
		if (p_friend == NULL)
		{
			log_error("role[%lu] p_friend null error", m_role_uid);
			continue;
		}

		if (p_friend->m_member.ftype() >= proto::common::friend_type_one_side_friend)
		{
			continue;
		}

		if (p_friend->m_member.contact_time() > 0)
		{
			proto::common::recommend_member* p_recommend_member = reply.add_recommend_friends();
			if (p_recommend_member != NULL)
			{
				p_recommend_member->set_src(proto::common::recommend_source_contact);
				// 更新下玩家数据
				friend_data_mgr_t::update_user_info(p_friend);

				proto::common::user_info* p_user_info = p_recommend_member->mutable_info();
				if (p_user_info != NULL)
				{
					p_user_info->CopyFrom(p_friend->m_member.info());
				}
			}
		}
		else if (p_friend->m_member.ftype() == proto::common::friend_type_troop_stranger)
		{
			proto::common::recommend_member* p_recommend_member = reply.add_recommend_friends();
			if (p_recommend_member != NULL)
			{
				p_recommend_member->set_src(proto::common::recommend_source_troop);
				// 更新下玩家数据
				friend_data_mgr_t::update_user_info(p_friend);

				proto::common::user_info* p_user_info = p_recommend_member->mutable_info();
				if (p_user_info != NULL)
				{
					p_user_info->CopyFrom(p_friend->m_member.info());
				}
			}
		}

		// 人数够了
		if ((uint32_t)reply.recommend_friends_size() >= friend_manager_t::get_friend_cnt6())
		{
			break;
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
		friend_data_mgr_t::update_all_friend_info(m_role_uid, p_friend);

		// 设置一些最基本的信息
		friend_data_mgr_t::load_user_base_info(p_friend->m_friend_id, p_friend->m_member.mutable_info());

		m_friend_relations.insert(std::make_pair(p_friend->m_friend_id, p_friend));

		WRITE_FRIEND_LOG_TO_DBS(m_role_uid, p_friend->m_friend_id, old_type, p_friend->m_member.ftype(), common::log_enum::source_type_friend_new_relation, 0);
	}
	else
	{
		friend_data_mgr_t::update_friend_type(m_role_uid, p_friend->m_redis_key, p_friend->m_member.ftype());

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