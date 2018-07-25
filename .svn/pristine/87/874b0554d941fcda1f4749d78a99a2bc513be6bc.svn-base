#include "mail_data_mgr.hpp"

#include <vector>

#include "log.hpp"
#include "common/redis_client.hpp"
#include "utility.hpp"
#include "tblh/MailTable.tbls.h"
#include "config/config_manager.hpp"
#include "log/log_wrapper.hpp"

#define WRITE_MAIL_LOG_TO_DB(uid, mail_id, mail_type, item_type, items, item_state, type, param) \
    log_wrapper_t::send_mail_log(uid, mail_id, mail_type, item_type, items, item_state, type, param);

void mail_data_mgr_t::load_user_mails(uint64_t uid, mail_base_map& mail_map, std::set<uint64_t>& global_mail_set)
{
	std::vector<uint64_t> all_mails;
	redis_client_t::get_keys_by_field_condition<uint64_t, uint64_t, std::vector<uint64_t>>("mail", "to_uid", uid, all_mails);

	uint32_t now_time = common::time_util_t::now_time();

	for (auto mail_id : all_mails)
	{
		uint32_t state = redis_client_t::get_uint32("mail", "state", mail_id);

		uint64_t global_id = redis_client_t::get_uint64("mail", "global_id", mail_id);
		if (global_id != 0)
			global_mail_set.insert(global_id);

		// 邮件被删除标志
		if (state == proto::common::MAIL_STATE_REMOVE)
			continue;

		uint32_t del_time = redis_client_t::get_uint32("mail", "del_time", mail_id);

		// 邮件已经过了删除时间，在上线时候给设置个标志位
		if (now_time > del_time)
		{
			WRITE_MAIL_LOG_TO_DB(redis_client_t::get_uint64("mail", "to_uid", mail_id), mail_id,
				redis_client_t::get_uint32("mail", "type", mail_id), redis_client_t::get_uint32("mail", "item_type", mail_id),
				redis_client_t::get_string("mail", "items", mail_id), redis_client_t::get_uint32("mail", "item_state", mail_id),
				common::log_enum::source_type_mail_delete, EM_MAIL_DEL_TYPE::MAIL_DEL_TIME_OUT);

			redis_client_t::set_uint32("mail", "state", mail_id, (uint32_t)proto::common::MAIL_STATE_REMOVE);

			continue;
		}

		proto::common::mail_base_info mail_info;

		proto::common::mail_common_info* p_common_info = mail_info.mutable_common_info();
		if (NULL == p_common_info)
		{
			log_error("role[%lu] p_common_info null error", uid);
			continue;
		}

		p_common_info->set_id(common::string_util_t::uint64_to_string(mail_id));
		p_common_info->set_type((proto::common::em_mail_type)redis_client_t::get_uint32("mail", "type", mail_id));
		p_common_info->set_state((proto::common::em_mail_state)state);
		p_common_info->set_addenda((proto::common::em_mail_addenda)redis_client_t::get_uint32("mail", "item_state", mail_id));

		uint32_t content_id = redis_client_t::get_uint32("mail", "content_id", mail_id);
		if (content_id != 0)
		{
			mail_info.set_content_id(redis_client_t::get_uint32("mail", "content_id", mail_id));

			MailTable* p_mail = GET_CONF(MailTable, content_id);
			if (NULL != p_mail)
			{
				mail_info.set_from_name(common::string_util_t::convert_to_utf8(p_mail->from_name()));
				mail_info.set_title(common::string_util_t::convert_to_utf8(p_mail->title()));
			}
			else
				log_error("role[%lu] mail table not found content_id[%u]", uid, content_id);
		}
		else
		{
			mail_info.set_from_name(redis_client_t::get_string("mail", "from_name", mail_id));
			mail_info.set_title(redis_client_t::get_string("mail", "title", mail_id));
		}
		mail_info.set_create_time(redis_client_t::get_uint32("mail", "create_time", mail_id));
		mail_info.set_del_time(redis_client_t::get_uint32("mail", "del_time", mail_id));

		mail_map.insert(std::make_pair(mail_id, mail_info));
	}
}

void mail_data_mgr_t::clear_time_out_mails()
{
	std::vector<uint64_t> all_mails;
	redis_client_t::get_all_keys<uint64_t, std::vector<uint64_t>>("mail", all_mails);

	uint32_t now_time = common::time_util_t::now_time();

	for (auto mail_id : all_mails)
	{
		uint32_t state = redis_client_t::get_uint32("mail", "state", mail_id);

		// 邮件被设置删除标志
		if (state == proto::common::MAIL_STATE_REMOVE)
		{
			//redis_client_t::remove_key("mail", mail_id);
			continue;
		}

		uint32_t del_time = redis_client_t::get_uint32("mail", "del_time", mail_id);

		// 邮件已经过了删除时间
		if (now_time > del_time)
		{
			//uint32_t item_state = redis_client_t::get_uint32("mail", "item_state", mail_id);

			// 邮件有附件，做个日志
			//if (item_state == proto::common::MAIL_ADDENDA_NEW)
			{
				WRITE_MAIL_LOG_TO_DB(redis_client_t::get_uint64("mail", "to_uid", mail_id), mail_id, 
					redis_client_t::get_uint32("mail", "type", mail_id), redis_client_t::get_uint32("mail", "item_type", mail_id),
					redis_client_t::get_string("mail", "items", mail_id), redis_client_t::get_uint32("mail", "item_state", mail_id),
					common::log_enum::source_type_mail_delete, EM_MAIL_DEL_TYPE::MAIL_DEL_TIME_OUT);
			}

			//redis_client_t::remove_key("mail", mail_id);
			redis_client_t::set_uint32("mail", "state", mail_id, (uint32_t)proto::common::MAIL_STATE_REMOVE);
		}
	}
}

void mail_data_mgr_t::set_mail_delete(uint64_t mail_id, uint64_t uid, EM_MAIL_DEL_TYPE type)
{
	// 不存在直接return
	if (!is_mail_exsit(mail_id))
	{
		log_error("role[%lu] mail[%lu] is not find when set delete", uid, mail_id);
		return;
	}

	//uint32_t item_state = redis_client_t::get_uint32("mail", "item_state", mail_id);
	// 邮件有附件，做个日志
	//if (item_state == proto::common::MAIL_ADDENDA_NEW)
	{
		WRITE_MAIL_LOG_TO_DB(uid, mail_id,
			redis_client_t::get_uint32("mail", "type", mail_id), redis_client_t::get_uint32("mail", "item_type", mail_id),
			redis_client_t::get_string("mail", "items", mail_id), redis_client_t::get_uint32("mail", "item_state", mail_id),
			common::log_enum::source_type_mail_delete, type);
	}

	// 设置删除标志
	redis_client_t::set_uint32("mail", "state", mail_id, (uint32_t)proto::common::MAIL_STATE_REMOVE);
	//redis_client_t::remove_key("mail", mail_id);
}


void mail_data_mgr_t::set_mail_get_delete(uint64_t mail_id, uint64_t uid, EM_MAIL_DEL_TYPE type)
{
	// 不存在直接return
	if (!is_mail_exsit(mail_id))
	{
		log_error("role[%lu] mail[%lu] is not find when set delete", uid, mail_id);
		return;
	}

	uint32_t item_state = redis_client_t::get_uint32("mail", "item_state", mail_id);
	if (item_state == proto::common::MAIL_ADDENDA_NEW)
	{
		// 领取邮件
		redis_client_t::set_uint32("mail", "item_state", mail_id, (uint32_t)proto::common::MAIL_ADDENDA_GOT);

		// 邮件有附件，做个日志
		WRITE_MAIL_LOG_TO_DB(uid, mail_id,
			redis_client_t::get_uint32("mail", "type", mail_id), redis_client_t::get_uint32("mail", "item_type", mail_id),
			redis_client_t::get_string("mail", "items", mail_id), redis_client_t::get_uint32("mail", "item_state", mail_id),
			common::log_enum::source_type_mail_user_get_addenda, 0);
	}
	else
	{
		log_error("role[%lu] mail[%lu] is already got when role getting", uid, mail_id);
	}

	// 设置删除标志
	set_mail_delete(mail_id, uid, type);
}

bool mail_data_mgr_t::is_mail_exsit(uint64_t mail_id)
{
	return redis_client_t::is_key_exist("mail", mail_id);
}

bool mail_data_mgr_t::get_mail_content(uint64_t mail_id, uint64_t uid, proto::client::ec_mail_content_reply& reply)
{
	// 不存在直接return
	if (!is_mail_exsit(mail_id))
	{
		log_error("role[%lu] mail[%lu] is not find when get mail content", uid, mail_id);
		return false;
	}

	// 新邮件设置已读
	if (redis_client_t::get_uint32("mail", "state", mail_id) == (uint32_t)proto::common::MAIL_STATE_NEW)
	{
		// 设置已读
		WRITE_MAIL_LOG_TO_DB(uid, mail_id,
			redis_client_t::get_uint32("mail", "type", mail_id), redis_client_t::get_uint32("mail", "item_type", mail_id),
			redis_client_t::get_string("mail", "items", mail_id), redis_client_t::get_uint32("mail", "item_state", mail_id),
			common::log_enum::source_type_mail_open, 0);

		redis_client_t::set_uint32("mail", "state", mail_id, (uint32_t)proto::common::MAIL_STATE_OPENED);
	}

	uint32_t content_id = redis_client_t::get_uint32("mail", "content_id", mail_id);
	if (content_id != 0)
	{
		MailTable* p_mail = GET_CONF(MailTable, content_id);
		if (NULL != p_mail)
		{
			reply.set_content(common::string_util_t::convert_to_utf8(p_mail->content()));
		}
		else
			log_error("role[%lu] mail table not found content_id[%u]", uid, content_id);
	}
	else
	{
		reply.set_content(redis_client_t::get_string("mail", "content", mail_id));
	}

	// 没有附件信息，或者已经被领取就不用发
	uint32_t item_state = redis_client_t::get_uint32("mail", "item_state", mail_id);
	if (item_state != proto::common::MAIL_ADDENDA_NEW)
	{
		return true;
	}

	std::string items = redis_client_t::get_string("mail", "items", mail_id);
	if (!items.empty())
	{
		reply.set_objs(items);
	}

	return true;
}

bool mail_data_mgr_t::check_can_get(uint64_t mail_id, uint32_t now_time)
{
	// 不存在直接return
	if (!is_mail_exsit(mail_id))
	{
		log_error("mail[%lu] is not find when getting", mail_id);
		return false;
	}

	if (redis_client_t::get_uint32("mail", "item_state", mail_id) != proto::common::MAIL_ADDENDA_NEW)
	{
		log_error("mail[%lu] is already got or have no items", mail_id);
		return false;
	}

	uint32_t item_do_time = redis_client_t::get_uint32("mail", "item_do_time", mail_id);
	if (now_time > item_do_time + GET_MAIL_COOLTIME)
	{
		return true;
	}

	return false;
}

void mail_data_mgr_t::get_mail_addenda(uint64_t mail_id, uint32_t now_time, proto::server::eg_mail_get_addenda_request& addenda)
{
	// 不存在直接return
	if (!is_mail_exsit(mail_id))
	{
		log_error("mail[%lu] is not find when getting", mail_id);
		return;
	}

	proto::server::mail_get_addenda* addenda_info = addenda.add_addendas();
	if (NULL != addenda_info)
	{
		addenda_info->set_mail_id(mail_id);

		std::string items = redis_client_t::get_string("mail", "items", mail_id);
		if (!items.empty())
		{
			std::vector<std::string> item_vec;
			common::string_util_t::split<std::string>(items, item_vec, "$");
			std::vector<uint32_t> item_v;

			for (uint32_t i = 0; i < item_vec.size(); ++i)
			{
				item_v.clear();
				common::string_util_t::split<uint32_t>(item_vec[i], item_v, ":");
				if (item_v.size() == 2)
				{
					proto::common::mail_item_obj* p_mail_obj = addenda_info->add_objs();
					if (NULL != p_mail_obj)
					{
						p_mail_obj->set_item_id(item_v[0]);
						p_mail_obj->set_item_num(item_v[1]);
					}
				}
			}
		}

		addenda_info->set_item_type(redis_client_t::get_uint32("mail", "item_type", mail_id));

		// 设置正在被领取
		redis_client_t::set_uint32("mail", "item_do_time", mail_id, now_time);
	}
}

bool mail_data_mgr_t::add_new_mail(const new_mail_define& mail_define)
{
	redis_client_t::set_uint32("mail", "state", mail_define.mail_id, (uint32_t)mail_define.mail_state);
	redis_client_t::set_uint32("mail", "type", mail_define.mail_id, (uint32_t)mail_define.mail_type);
	redis_client_t::set_uint64("mail", "global_id", mail_define.mail_id, mail_define.global_id);
	redis_client_t::set_uint64("mail", "to_uid", mail_define.mail_id, mail_define.to_uid);
	redis_client_t::set_uint64("mail", "from_uid", mail_define.mail_id, mail_define.from_uid);
	
	redis_client_t::set_uint32("mail", "create_time", mail_define.mail_id, mail_define.create_time);
	redis_client_t::set_uint32("mail", "del_time", mail_define.mail_id, mail_define.del_time);
	redis_client_t::set_uint32("mail", "content_id", mail_define.mail_id, mail_define.content_id);

	if (mail_define.content_id > 0)
	{
		redis_client_t::set_string("mail", "from_name", mail_define.mail_id, "");
		redis_client_t::set_string("mail", "title", mail_define.mail_id, "");
		redis_client_t::set_string("mail", "content", mail_define.mail_id, "");
	}
	else
	{
		redis_client_t::set_string("mail", "from_name", mail_define.mail_id, mail_define.from_name);
		redis_client_t::set_string("mail", "title", mail_define.mail_id, mail_define.title);
		redis_client_t::set_string("mail", "content", mail_define.mail_id, mail_define.content);
	}
	
	redis_client_t::set_string("mail", "items", mail_define.mail_id, mail_define.items);
	redis_client_t::set_uint32("mail", "item_state", mail_define.mail_id, (uint32_t)mail_define.item_state);
	redis_client_t::set_uint32("mail", "item_do_time", mail_define.mail_id, 0);
	redis_client_t::set_uint32("mail", "item_type", mail_define.mail_id, (uint32_t)mail_define.get_type);

	// 有附件写个日志
	//if (mail_define.item_state == proto::common::MAIL_ADDENDA_NEW)
	{
		WRITE_MAIL_LOG_TO_DB(mail_define.to_uid, mail_define.mail_id,
			mail_define.mail_type, mail_define.get_type,
			mail_define.items, mail_define.item_state,
			common::log_enum::source_type_mail_new_mail, 0);
	}

	return true;
}


void mail_data_mgr_t::clear_get_mask(uint64_t mail_id)
{
	// 不存在直接return
	if (!is_mail_exsit(mail_id))
	{
		log_error("mail[%lu] is not find when clear_get_mask", mail_id);
		return;
	}

	// 设置正在被领取
	redis_client_t::set_uint32("mail", "item_do_time", mail_id, 0);
}

bool mail_data_mgr_t::add_new_global_mail(global_mail_ptr p_mail)
{
	if (NULL == p_mail)
	{
		log_error("p_mail null error");
		return false;
	}

	redis_client_t::set_uint32("global_mail", "type", p_mail->mail_id, (uint32_t)p_mail->mail_type);
	redis_client_t::set_uint32("global_mail", "param1", p_mail->mail_id, p_mail->mail_param1);
	redis_client_t::set_uint32("global_mail", "param2", p_mail->mail_id, p_mail->mail_param2);

	if (p_mail->mail_type == EM_GLOBAL_MAIL_TYPE::GLOBAL_MAIL_USERS || p_mail->mail_type == EM_GLOBAL_MAIL_TYPE::GLOBAL_MAIL_GM)
	{
		std::ostringstream ss;
		for (std::set<uint64_t>::const_iterator citr = p_mail->mail_uids.begin();
			citr != p_mail->mail_uids.end(); ++citr)
		{
			if (!ss.str().empty())
			{
				ss << "$";
			}

			ss << *citr;
		}

		redis_client_t::set_string("global_mail", "to_uids", p_mail->mail_id, ss.str());
	}

	redis_client_t::set_uint32("global_mail", "start_time", p_mail->mail_id, p_mail->start_time);
	redis_client_t::set_uint32("global_mail", "end_time", p_mail->mail_id, p_mail->end_time);

	redis_client_t::set_string("global_mail", "from_name", p_mail->mail_id, p_mail->from_name);
	redis_client_t::set_string("global_mail", "title", p_mail->mail_id, p_mail->title);
	redis_client_t::set_string("global_mail", "content", p_mail->mail_id, p_mail->content);
	redis_client_t::set_string("global_mail", "items", p_mail->mail_id, p_mail->items);
	redis_client_t::set_string("global_mail", "desc", p_mail->mail_id, p_mail->desc);

	MAIL_LOG("add global mail[%lu] type[%u] param1[%u] param2[%u] to_uid size[%u] start_time[%u] end_time[%u] from_name[%s] title[%s] items[%s] desc[%s]",
		p_mail->mail_id, p_mail->mail_type, p_mail->mail_param1, p_mail->mail_param2, (uint32_t)p_mail->mail_uids.size(), p_mail->start_time, p_mail->end_time,
		p_mail->from_name.c_str(), p_mail->title.c_str(), p_mail->items.c_str(), p_mail->desc.c_str());

	return true;
}

bool mail_data_mgr_t::load_global_mails(global_mail_ptr_map& global_mail_map)
{
	std::vector<uint64_t> all_mails;
	redis_client_t::get_all_keys<uint64_t, std::vector<uint64_t>>("global_mail", all_mails);

	uint32_t now_time = common::time_util_t::now_time();

	for (auto mail_id : all_mails)
	{
		uint32_t start_time = redis_client_t::get_uint32("global_mail", "start_time", mail_id);
		uint32_t end_time = redis_client_t::get_uint32("global_mail", "end_time", mail_id);

		if (end_time != 0 && now_time > end_time)
			continue;

		global_mail_ptr p_mail = global_mail_ptr(new global_mail_t());
		if (NULL == p_mail)
		{
			log_error("p_mail null error");
			continue;
		}

		p_mail->mail_id = mail_id;
		p_mail->start_time = start_time;
		p_mail->end_time = end_time;

		uint32_t type = redis_client_t::get_uint32("global_mail", "type", mail_id);
		p_mail->mail_type = type;

		p_mail->mail_param1 = redis_client_t::get_uint32("global_mail", "param1", mail_id);
		p_mail->mail_param2 = redis_client_t::get_uint32("global_mail", "param2", mail_id);

		switch (type)
		{
		case EM_GLOBAL_MAIL_TYPE::GLOBAL_MAIL_ALL:
			break;
		case EM_GLOBAL_MAIL_TYPE::GLOBAL_MAIL_USERS:
		case EM_GLOBAL_MAIL_TYPE::GLOBAL_MAIL_GM:
			{
				std::string uids = redis_client_t::get_string("global_mail", "to_uids", mail_id);
				if (!uids.empty())
				{
					std::vector<uint32_t> to_uids;
					common::string_util_t::ParseIds(uids, to_uids, '$');
					for (std::vector<uint32_t>::const_iterator itr = to_uids.begin();
						itr != to_uids.end(); ++itr)
					{
						p_mail->mail_uids.insert(*itr);
					}
				}
			}
			break;
		default:
			log_error("global mail type error");
			return false;
		}

		p_mail->from_name = redis_client_t::get_string("global_mail", "from_name", mail_id);
		p_mail->title = redis_client_t::get_string("global_mail", "title", mail_id);
		p_mail->content = redis_client_t::get_string("global_mail", "content", mail_id);
		p_mail->items = redis_client_t::get_string("global_mail", "items", mail_id);
		p_mail->desc = redis_client_t::get_string("global_mail", "desc", mail_id);

		global_mail_map.insert(std::make_pair(mail_id, p_mail));
	}

	return true;
}

