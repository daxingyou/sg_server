#include "global_mail_manager.hpp"
#include "redis/mail_data_mgr.hpp"
#include "mail_manager.hpp"
#include "user/global_user.hpp"
#include "user/global_user_manager.hpp"

global_mail_ptr_map global_mail_manager_t::m_global_mail_map;

bool global_mail_manager_t::init()
{
	m_global_mail_map.clear();

	if (!mail_data_mgr_t::load_global_mails(m_global_mail_map))
	{
		return false;
	}

	return true;
}

bool global_mail_manager_t::close_clear()
{
	m_global_mail_map.clear();

	return true;
}

bool global_mail_manager_t::check_user_new(global_user_ptr p_user)
{
	if (NULL == p_user)
	{
		log_error("p_user null error");
		return false;
	}

	bool ret = false;
	uint32_t now_time = common::time_util_t::now_time();

	for (global_mail_ptr_map::const_iterator itr = m_global_mail_map.begin();
		itr != m_global_mail_map.end(); ++itr)
	{
		global_mail_ptr p_mail = itr->second;
		if (NULL == p_mail)
		{
			log_error("p_mail null error");
			continue;
		}

		if (p_mail->start_time > now_time)
		{
			continue;
		}

		if (p_mail->end_time != 0 && now_time > p_mail->end_time)
		{
			continue;
		}

		if ( ( p_mail->mail_type == EM_GLOBAL_MAIL_TYPE::GLOBAL_MAIL_USERS || p_mail->mail_type == EM_GLOBAL_MAIL_TYPE::GLOBAL_MAIL_GM )
			&& !p_mail->is_have_uid(p_user->get_uid()))
		{
			continue;
		}
		else if (p_mail->mail_type == EM_GLOBAL_MAIL_TYPE::GLOBAL_MAIL_AREAN)
		{
			// TODO
		}
		else if (p_mail->mail_type == EM_GLOBAL_MAIL_TYPE::GLOBAL_MAIL_GM)
		{
			//TODO
			if (p_user->get_create_time() > p_mail->start_time)
			{
				continue;
			}
		}
		else if (p_mail->mail_type == EM_GLOBAL_MAIL_TYPE::GLOBAL_MAIL_ALL)
		{
			//TODO
		}
		else
		{
			log_error("global mail type[%u] error!", p_mail->mail_type);
			continue;
		}

		if (p_user->get_mail_data().is_have_global_mail(itr->first))
		{
			continue;
		}

		new_mail_define mail_define;
		mail_define.reset();

		mail_define.mail_id = mail_manager_t::get_new_mail_uid();
		mail_define.mail_state = proto::common::MAIL_STATE_NEW;
		mail_define.mail_type = proto::common::MAIL_TYPE_SYS;
		mail_define.global_id = itr->first;
		mail_define.to_uid = p_user->get_uid();

		mail_define.from_name = p_mail->from_name;

		mail_define.create_time = now_time;
		mail_define.del_time = now_time + MAX_MAIL_TIME;

		mail_define.title = p_mail->title;
		mail_define.content = p_mail->content;

		if (!p_mail->items.empty())
		{
			mail_define.items = p_mail->items;
			mail_define.item_state = proto::common::MAIL_ADDENDA_NEW;
		}

		mail_define.get_type = proto::common::MAIL_GET_TYPE_GLOBAL_MAIL;
		//mail_define.desc = p_mail->desc;

		mail_data_mgr_t::add_new_mail(mail_define);
		p_user->get_mail_data().add_new_global_mail(mail_define, p_user);
		p_user->get_mail_data().set_have_global_mail(itr->first);

		ret = true;
	}

	return ret;
}

bool global_mail_manager_t::add_new_global_mail(uint32_t type, /* 全局邮件类型 */ 
	uint32_t param1, /* 附加参数1 */ 
	uint32_t param2, /* 附加参数2 */ 
	std::set<uint64_t>& uids, /* type=GLOBAL_MAIL_USERS 时填写指定玩家的id */ 
	uint32_t start_time, /* 开始时间 */ 
	uint32_t end_time, /* 结束时间 0 表示一直有效 */ 
	const std::string& from_name, /* 发信人 */ 
	const std::string& title, /* 标题 */ 
	const std::string& content, /* 内容 */ 
	const std::string& items, /* 附件（itemid:num$itemid:num） */ 
	const std::string& desc /* 备注 */)
{
	if (type >= EM_GLOBAL_MAIL_TYPE::GLOBAL_MAIL_MAX)
	{
		log_error("global mail type error");
		return false;
	}

	if (end_time != 0 && end_time < common::time_util_t::now_time())
	{
		log_error("end time error");
		return false;
	}

	global_mail_ptr p_mail = global_mail_ptr(new global_mail_t());
	if (NULL == p_mail)
	{
		log_error("global mail null error");
		return false;
	}

	p_mail->mail_id = mail_manager_t::get_new_mail_uid();
	p_mail->mail_type = type;
	p_mail->mail_param1 = param1;
	p_mail->mail_param2 = param2;
	if (p_mail->mail_type == EM_GLOBAL_MAIL_TYPE::GLOBAL_MAIL_USERS || p_mail->mail_type == EM_GLOBAL_MAIL_TYPE::GLOBAL_MAIL_GM )
	{
		p_mail->mail_uids = std::move(uids);
	}

	p_mail->start_time = start_time;
	p_mail->end_time = end_time;
	p_mail->from_name = from_name;
	p_mail->title = title;
	p_mail->content = content;
	p_mail->items = items;
	p_mail->desc = desc;

	if (mail_data_mgr_t::add_new_global_mail(p_mail))
	{
		m_global_mail_map.insert(std::make_pair(p_mail->mail_id, p_mail));
	}

	//通知小红点
	for (auto uid : uids)
	{
		global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
		if (NULL != p_user)
		{
			global_mail_manager_t::check_user_new(p_user);
		}
	}
	return true;
}

//void global_mail_manager_t::test_one_global_mail()
//{
//	std::set<uint64_t> uids;
//	
//	add_new_global_mail(EM_GLOBAL_MAIL_TYPE::GLOBAL_MAIL_ALL, 1, 2, uids, common::time_util_t::now_time(), 0, common::string_util_t::convert_to_utf8("钱还将"), common::string_util_t::convert_to_utf8("测试"), common::string_util_t::convert_to_utf8("测试测试测试"), "", common::string_util_t::convert_to_utf8("测试"));
//}
//
//void global_mail_manager_t::test_one_global_mail_to_some_one()
//{
//	std::set<uint64_t> uids;
//	uids.insert(11111);
//	uids.insert(22222);
//
//	add_new_global_mail(EM_GLOBAL_MAIL_TYPE::GLOBAL_MAIL_USERS, 1, 2, uids, common::time_util_t::now_time(), 0, common::string_util_t::convert_to_utf8("钱还将"), common::string_util_t::convert_to_utf8("测试"), common::string_util_t::convert_to_utf8("测试测试测试"), "", common::string_util_t::convert_to_utf8("测试"));
//}
