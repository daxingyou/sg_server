#include "mail_manager.hpp"

#include "user/global_user_manager.hpp"
#include "client.pb.h"
#include "redis/mail_data_mgr.hpp"
#include "global_id.hpp"
#include "user/global_user_manager.hpp"
#include "redis/mail_data_mgr.hpp"
#include "tblh/MailTable.tbls.h"
#include "config/config_manager.hpp"
#include "log.hpp"
#include "tblh/Comprehensive.tbls.h"

uint64_t mail_manager_t::get_new_mail_uid()
{
	return common::g_id_allocator.gen_new_id(common::ID_ALLOCATOR_TYPE_MAIL);
}

void mail_manager_t::clear_old_mail()
{
	// 先清理在线玩家，后清理redis
	const global_user_manager_t::global_user_map_type& all_global_user_map = global_user_manager_t::get_all_global_users();

	for (global_user_manager_t::global_user_map_type::const_iterator itr = all_global_user_map.begin();
		itr != all_global_user_map.end(); ++itr)
	{
		global_user_ptr p_user = itr->second;
		if (nullptr != p_user)
		{
			p_user->get_mail_data().clear_time_out_mail();
		}
	}

	// 上线会清理
	//mail_data_mgr_t::clear_time_out_mails();
}

void mail_manager_t::send_mail(uint64_t to_uid, proto::common::em_mail_type mail_type, uint32_t content_id,
	proto::common::em_mail_get_type get_type /*= proto::common::MAIL_GET_TYPE_SYS*/)
{
	std::vector<uint64_t> uids;
	uids.push_back(to_uid);

	send_mail(uids, mail_type, content_id, get_type);
}

void mail_manager_t::send_mail(const std::vector<uint64_t>& to_uids, proto::common::em_mail_type mail_type, uint32_t content_id,
	proto::common::em_mail_get_type get_type /*= proto::common::MAIL_GET_TYPE_SYS*/)
{
	if (to_uids.empty())
	{
		log_error("vector to_uids empty return");
		return;
	}

	MailTable* p_mail = GET_CONF(MailTable, content_id);
	if (NULL == p_mail)
	{
		log_error("p_mail[%u] error null", content_id);
		return;
	}

	new_mail_define mail_define;

	uint32_t now_time = common::time_util_t::now_time();

	for (auto to_uid : to_uids)
	{
		if (to_uid == 0)
		{
			log_error("error:send_mail uid == 0 ");
			continue;
		}

		mail_define.reset();

		mail_define.mail_id = mail_manager_t::get_new_mail_uid();
		mail_define.mail_state = proto::common::MAIL_STATE_NEW;
		mail_define.mail_type = mail_type;
		mail_define.to_uid = to_uid;
		mail_define.from_uid = 0;
		mail_define.global_id = 0;

		mail_define.from_name = common::string_util_t::convert_to_utf8(p_mail->from_name());
		mail_define.title = common::string_util_t::convert_to_utf8(p_mail->title());
		//mail_define.content = p_mail->content();

		if (!p_mail->addenda().empty())
		{
			mail_define.items = p_mail->addenda();
			mail_define.item_state = proto::common::MAIL_ADDENDA_NEW;
		}

		mail_define.content_id = content_id;

		mail_define.create_time = now_time;
		mail_define.del_time = now_time + MAX_MAIL_TIME;

		mail_define.get_type = get_type;

		send_new_mail(mail_define);
	}
}

void mail_manager_t::send_mail(uint64_t to_uid, proto::common::em_mail_type mail_type,
	uint32_t content_id, const std::map<uint32_t, uint32_t>& item_map,
	proto::common::em_mail_get_type get_type /*= proto::common::MAIL_GET_TYPE_SYS*/)
{
	std::vector<uint64_t> uids;
	uids.push_back(to_uid);

	send_mail(uids, mail_type, content_id, item_map, get_type);
}

void mail_manager_t::send_mail(const std::vector<uint64_t>& to_uids, proto::common::em_mail_type mail_type,
	uint32_t content_id, const std::map<uint32_t, uint32_t>& item_map,
	proto::common::em_mail_get_type get_type /*= proto::common::MAIL_GET_TYPE_SYS*/)
{
	if (to_uids.empty())
	{
		log_error("vector to_uids empty return");
		return;
	}

	MailTable* p_mail = GET_CONF(MailTable, content_id);
	if (NULL == p_mail)
	{
		log_error("p_mail[%u] error null", content_id);
		return;
	}

	if (!item_map.empty())
	{
		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::mail_annex_num);
		uint32_t max_item_num = GET_COMPREHENSIVE_VALUE_1(p_conf);

		if (item_map.size() > max_item_num)
		{
			log_error("mail item size[%u] > max_size[%u]", (int32_t)item_map.size(), max_item_num);
			return;
		}
	}

	new_mail_define mail_define;

	uint32_t now_time = common::time_util_t::now_time();

	for (auto to_uid : to_uids)
	{
		if (to_uid == 0)
		{
			log_error("error:send_mail uid == 0 ");
			continue;
		}

		mail_define.reset();

		mail_define.mail_id = mail_manager_t::get_new_mail_uid();
		mail_define.mail_state = proto::common::MAIL_STATE_NEW;
		mail_define.mail_type = mail_type;
		mail_define.to_uid = to_uid;
		mail_define.from_uid = 0;
		mail_define.global_id = 0;

		mail_define.from_name = common::string_util_t::convert_to_utf8(p_mail->from_name());
		mail_define.title = common::string_util_t::convert_to_utf8(p_mail->title());
		//mail_define.content = p_mail->content();

		if (!item_map.empty())
		{
			std::ostringstream ss;
			for (std::map<uint32_t, uint32_t>::const_iterator itr = item_map.begin();
				itr != item_map.end(); ++itr)
			{
				if (!ss.str().empty())
				{
					ss << "$";
				}

				ss << itr->first << ":" << itr->second;
			}

			mail_define.items = ss.str();
			mail_define.item_state = proto::common::MAIL_ADDENDA_NEW;
		}

		mail_define.content_id = content_id;

		mail_define.create_time = now_time;
		mail_define.del_time = now_time + MAX_MAIL_TIME;

		mail_define.get_type = get_type;

		send_new_mail(mail_define);
	}
}

void mail_manager_t::send_mail(uint64_t to_uid, proto::common::em_mail_type mail_type,
	uint64_t from_uid, const std::string& from_name,
	const std::string& title, const std::string& content,
	const std::map<uint32_t, uint32_t>& item_map, proto::common::em_mail_get_type get_type /*= proto::common::MAIL_GET_TYPE_SYS*/)
{
	std::vector<uint64_t> uids;
	uids.push_back(to_uid);

	send_mail(uids, mail_type, from_uid, from_name, title, content, item_map, get_type);
}

void mail_manager_t::send_mail(const std::vector<uint64_t>& to_uids, proto::common::em_mail_type mail_type,
	uint64_t from_uid, const std::string& from_name,
	const std::string& title, const std::string& content,
	const std::map<uint32_t, uint32_t>& item_map, proto::common::em_mail_get_type get_type /*= proto::common::MAIL_GET_TYPE_SYS*/)
{
	if (to_uids.empty())
	{
		log_error("vector to_uids empty return");
		return;
	}

	if (!item_map.empty())
	{
		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::mail_annex_num);
		uint32_t max_item_num = GET_COMPREHENSIVE_VALUE_1(p_conf);

		if (item_map.size() > max_item_num)
		{
			log_error("mail item size[%u] > max_size[%u]", (int32_t)item_map.size(), max_item_num);
			return;
		}
	}

	new_mail_define mail_define;

	uint32_t now_time = common::time_util_t::now_time();

	for (auto to_uid : to_uids)
	{
		if (to_uid == 0)
		{
			log_error("error:send_mail uid == 0 ");
			continue;
		}

		mail_define.reset();

		mail_define.mail_id = mail_manager_t::get_new_mail_uid();
		mail_define.mail_state = proto::common::MAIL_STATE_NEW;
		mail_define.mail_type = mail_type;
		mail_define.to_uid = to_uid;

		mail_define.from_uid = from_uid;
		mail_define.from_name = from_name;
		mail_define.title = title;
		mail_define.content = content;

		mail_define.content_id = 0;
		mail_define.global_id = 0;

		if (!item_map.empty())
		{
			std::ostringstream ss;
			for (std::map<uint32_t, uint32_t>::const_iterator itr = item_map.begin();
				itr != item_map.end(); ++itr)
			{
				if (!ss.str().empty())
				{
					ss << "$";
				}

				ss << itr->first << ":" << itr->second;
			}

			mail_define.items = ss.str();
			mail_define.item_state = proto::common::MAIL_ADDENDA_NEW;
		}

		mail_define.create_time = now_time;
		mail_define.del_time = now_time + MAX_MAIL_TIME;

		mail_define.get_type = get_type;

		send_new_mail(mail_define);
	}
}

void mail_manager_t::send_new_mail(const new_mail_define& mail_define)
{
	mail_data_mgr_t::add_new_mail(mail_define);

	// 是否在线，在线就通知client
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(mail_define.to_uid);
	if (NULL != p_user)
	{
		p_user->get_mail_data().add_new_mail(mail_define, p_user);
	}
}

void mail_manager_t::send_new_global_mail(const new_mail_define& mail_define)
{
	mail_data_mgr_t::add_new_mail(mail_define);

	// 是否在线，在线就通知client
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(mail_define.to_uid);
	if (NULL != p_user)
	{
		p_user->get_mail_data().add_new_global_mail(mail_define, p_user);
	}
}
