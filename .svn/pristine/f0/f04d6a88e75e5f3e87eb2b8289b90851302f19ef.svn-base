#include "mail_manager.hpp"

#include "main/game_server_fwd.hpp"
#include "main/game_server.hpp"
#include "utility.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "config/config_manager.hpp"

USING_NS_COMMON;

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

	if (NULL == env::server)
	{
		log_error("env::server null error");
		return;
	}

	proto::server::ge_mail_new_notify notify;

	for (std::vector<uint64_t>::const_iterator itr = to_uids.begin();
		itr != to_uids.end(); ++itr)
	{
		uint64_t uid = *itr;

		if (uid == 0)
		{
			log_error("error:send_mail uid == 0 ");
			continue;
		}

		notify.add_uids(uid);
	}

	if (notify.uids_size() > 0)
	{
		notify.set_mail_type(mail_type);
		notify.set_content_id(content_id);
		notify.set_get_type(get_type);

		env::server->send_msg_to_center(op_cmd::ge_mail_new_notify, 0, notify);
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

	if (NULL == env::server)
	{
		log_error("env::server null error");
		return;
	}

	if (!item_map.empty())
	{
		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::mail_annex_num);
		uint32_t max_item_num = GET_COMPREHENSIVE_VALUE_1(p_conf);

		if (item_map.size() > max_item_num)
		{
			log_error("mail item size[%u] > max_size[%u]", item_map.size(), max_item_num);
			return;
		}
	}

	proto::server::ge_mail_new_notify notify;

	for (std::vector<uint64_t>::const_iterator itr = to_uids.begin();
		itr != to_uids.end(); ++itr)
	{
		uint64_t uid = *itr;

		if (uid == 0)
		{
			log_error("error:send_mail uid == 0 ");
			continue;
		}

		notify.add_uids(uid);
	}

	if (notify.uids_size() == 0)
	{
		log_error("send_mail uid_size() == 0");
		return;
	}

	notify.set_mail_type(mail_type);
	notify.set_content_id(content_id);

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

		notify.set_items(ss.str());
	}
	notify.set_get_type(get_type);

	env::server->send_msg_to_center(op_cmd::ge_mail_new_notify, 0, notify);
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

	if (NULL == env::server)
	{
		log_error("env::server null error");
		return;
	}

	if (!item_map.empty())
	{
		auto p_conf = GET_CONF(Comprehensive, comprehensive_common::mail_annex_num);
		uint32_t max_item_num = GET_COMPREHENSIVE_VALUE_1(p_conf);

		if (item_map.size() > max_item_num)
		{
			log_error("mail item size[%u] > max_size[%u]", item_map.size(), max_item_num);
			return;
		}
	}

	proto::server::ge_mail_new_notify notify;

	for (std::vector<uint64_t>::const_iterator itr = to_uids.begin();
		itr != to_uids.end(); ++itr)
	{
		uint64_t uid = *itr;

		if (uid == 0)
		{
			log_error("error:send_mail uid == 0 ");
			continue;
		}

		notify.add_uids(uid);
	}

	if (notify.uids_size() == 0)
	{
		log_error("send_mail uid_size() == 0");
		return;
	}

	notify.set_mail_type(mail_type);

	notify.set_from_id(from_uid);
	notify.set_from_name(from_name);
	notify.set_title(title);
	notify.set_content(content);

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

		notify.set_items(ss.str());
	}
	notify.set_get_type(get_type);

	env::server->send_msg_to_center(op_cmd::ge_mail_new_notify, 0, notify);
}
