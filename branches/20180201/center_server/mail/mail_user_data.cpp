#include "mail_user_data.hpp"

#include "log.hpp"
#include "user/global_user_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "redis/mail_data_mgr.hpp"
#include "user/global_user.hpp"
#include "mail_manager.hpp"
#include "tblh/MailTable.tbls.h"
#include "config/config_manager.hpp"
#include "red_point/red_point_manager.hpp"
USING_NS_COMMON;

mail_user_data_t::mail_user_data_t(uint64_t role_uid)
	: m_role_uid(role_uid)
{

}

void mail_user_data_t::on_login_load()
{
	clear();

	mail_data_mgr_t::load_user_mails(m_role_uid, m_mail_map, m_global_mail_map);

	// 由于可能收到离线邮件，所以上线要清理以保证邮件数量在100之内

	// 当数量超过最大值时执行对已读并且没有附件的邮件的删除
	if (m_mail_map.size() > MAX_MIAL_SIZE)
	{
		clear_opened_mail();
	}

	// 当全不能删除时，删除最早的一封邮件
	if (m_mail_map.size() > MAX_MIAL_SIZE)
	{
		force_del_mail(m_mail_map.size() - MAX_MIAL_SIZE);
	}

	if (is_have_new())
	{
		global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(m_role_uid);
		if (NULL == p_user)
		{
			log_error("p_user[%lu] null error", m_role_uid);
			return;
		}
	}
}

void mail_user_data_t::clear_opened_mail()
{
	for (mail_base_map::iterator itr = m_mail_map.begin();
		itr != m_mail_map.end();)
	{
		// 删除已经打开的并且没有附件的邮件
		if (itr->second.common_info().state() != proto::common::MAIL_STATE_NEW && itr->second.common_info().addenda() != proto::common::MAIL_ADDENDA_NEW)
		{
			mail_data_mgr_t::set_mail_delete(itr->first, m_role_uid, MAIL_DEL_TOO_MUCH);

			m_mail_map.erase(itr++);
		}
		else
		{
			++itr;
		}
	}
}

void mail_user_data_t::clear_time_out_mail()
{
	uint32_t now_time = common::time_util_t::now_time();

	for (mail_base_map::iterator itr = m_mail_map.begin();
		itr != m_mail_map.end();)
	{
		if (now_time > itr->second.del_time())
		{
			mail_data_mgr_t::set_mail_delete(itr->first, m_role_uid, MAIL_DEL_TIME_OUT);

			m_mail_map.erase(itr++);
		}
		else
		{
			++itr;
		}
	}
}

void mail_user_data_t::force_del_mail(uint32_t num /*= 1*/)
{
	if (num == 0)
	{
		log_error("role[%lu] force_del_mail del num == zero", m_role_uid);
		return;
	}

	// 删除id小的，也是最老的邮件
	uint32_t del_num = 0;
	for (mail_base_map::iterator itr = m_mail_map.begin();
		itr != m_mail_map.end() && del_num < num; ++del_num)
	{
		mail_data_mgr_t::set_mail_delete(itr->first, m_role_uid, MAIL_DEL_TOO_MUCH);

		m_mail_map.erase(itr++);
	}

	m_force_del_num += num;
}

// void mail_user_data_t::force_del_mail(proto::client::ec_mail_state_change_notify& notify, uint32_t num /*= 1*/)
// {
// 	if (num == 0)
// 	{
// 		log_error("role[%lu] force_del_mail del num == zero", m_role_uid);
// 		return;
// 	}
// 
// 	// 删除id小的，也是最老的邮件
// 	for (mail_base_map::iterator itr = m_mail_map.begin();
// 		itr != m_mail_map.end() && num > 0; num--)
// 	{
// 		itr->second.mutable_common_info()->set_state(proto::common::MAIL_STATE_REMOVE);
// 
// 		proto::common::mail_common_info* common_info = notify.add_common_infos();
// 		if (NULL != common_info)
// 		{
// 			common_info->CopyFrom(itr->second.common_info());
// 		}
// 
// 		mail_data_mgr_t::set_mail_delete(itr->first, m_role_uid, MAIL_DEL_TOO_MUCH);
// 
// 		m_mail_map.erase(itr++);
// 	}
// }

void mail_user_data_t::clear()
{
	m_mail_map.clear();

	m_global_mail_map.clear();
}

bool mail_user_data_t::is_have_new()
{
	// 有未读邮件或者未领取邮件
	for (mail_base_map::const_iterator itr = m_mail_map.begin();
		itr != m_mail_map.end(); ++itr)
	{
		if (itr->second.common_info().state() == proto::common::MAIL_STATE_NEW || itr->second.common_info().addenda() == proto::common::MAIL_ADDENDA_NEW)
		{
			return true;
		}
	}

	return false;
}

bool mail_user_data_t::is_have_new_mail()
{
	// 有未读邮件 不包括已读未领取的
	for (mail_base_map::const_iterator itr = m_mail_map.begin();
		itr != m_mail_map.end(); ++itr)
	{
		if (itr->second.common_info().state() == proto::common::MAIL_STATE_NEW)
		{
			return true;
		}
	}

	return false;
}

void mail_user_data_t::set_mail_get_delete(uint64_t mail_id, proto::client::ec_mail_state_change_notify& notify)
{
	mail_base_map::iterator itr = m_mail_map.find(mail_id);
	if (itr == m_mail_map.end())
	{
		log_error("role[%lu] mail[%lu] is not find", m_role_uid, mail_id);
		return;
	}

	if (itr->second.common_info().state() == proto::common::MAIL_STATE_REMOVE)
	{
		log_error("role[%lu] mail[%lu] is removed", m_role_uid, mail_id);
		return;
	}

	itr->second.mutable_common_info()->set_state(proto::common::MAIL_STATE_REMOVE);
	itr->second.mutable_common_info()->set_addenda(proto::common::MAIL_ADDENDA_GOT);

	proto::common::mail_common_info* common_info = notify.add_common_infos();
	if (NULL != common_info)
	{
		common_info->CopyFrom(itr->second.common_info());
	}

	mail_data_mgr_t::set_mail_get_delete(mail_id, m_role_uid, MAIL_DEL_GET);

	m_mail_map.erase(itr);
}

void mail_user_data_t::get_mail_list(proto::client::ec_mail_list_notify& notify)
{
	if (m_mail_map.empty())
		return;

	uint32_t now_time = common::time_util_t::now_time();

	// 发给客户端之前先清理过期邮件
	for (mail_base_map::iterator itr = m_mail_map.begin();
		itr != m_mail_map.end();)
	{
		if (now_time > itr->second.del_time())
		{
			mail_data_mgr_t::set_mail_delete(itr->first, m_role_uid, MAIL_DEL_TIME_OUT);

			m_mail_map.erase(itr++);
		}
		else
		{
			proto::common::mail_base_info* p_mail_base = notify.add_base_infos();
			if (NULL != p_mail_base)
			{
				p_mail_base->CopyFrom(itr->second);
			}

			++itr;
		}
	}

	if (m_force_del_num > 0)
	{
		notify.set_force_del_flag(m_force_del_num);
		m_force_del_num = 0;
	}
}

void mail_user_data_t::get_mail_content(uint64_t mail_id, proto::client::ec_mail_content_reply& reply)
{
	mail_base_map::iterator itr = m_mail_map.find(mail_id);
	if (itr == m_mail_map.end())
	{
		log_error("role[%lu] mail[%lu] is not find", m_role_uid, mail_id);
		reply.set_reply_code(errcode_enum::mail_error_not_find);
		return;
	}

	if (itr->second.common_info().state() == proto::common::MAIL_STATE_REMOVE)
	{
		log_error("role[%lu] mail[%lu] is removed", m_role_uid, mail_id);
		reply.set_reply_code(errcode_enum::mail_error_is_removed);
		return;
	}

	uint32_t now_time = common::time_util_t::now_time();
	if (now_time > itr->second.del_time())
	{
		log_error("role[%lu] mail[%lu] is time out", m_role_uid, mail_id);
		reply.set_reply_code(errcode_enum::mail_error_is_time_out);
		return;
	}

	// 新邮件设置为已经打开过
	if (itr->second.common_info().state() == proto::common::MAIL_STATE_NEW)
		itr->second.mutable_common_info()->set_state(proto::common::MAIL_STATE_OPENED);

	reply.set_reply_code(errcode_enum::error_ok);

	proto::common::mail_base_info* p_mail_base = reply.mutable_base_info();
	if (NULL != p_mail_base)
	{
		p_mail_base->CopyFrom(itr->second);
	}

	mail_data_mgr_t::get_mail_content(mail_id, m_role_uid, reply);

	// 没有附件邮件打开后就删除
// 	if (itr->second.common_info().addenda() == proto::common::MAIL_ADDENDA_NONE)
// 	{
// 		mail_data_mgr_t::set_mail_delete(mail_id, m_role_uid, MAIL_DEL_OPEN);
// 
// 		m_mail_map.erase(itr);
// 	}
}

void mail_user_data_t::del_mail_by_id(uint64_t mail_id, proto::client::ec_mail_state_change_notify& notify, EM_MAIL_DEL_TYPE type /*= MAIL_DEL_USER*/)
{
	mail_base_map::iterator itr = m_mail_map.find(mail_id);
	if (itr == m_mail_map.end())
	{
		log_error("role[%lu] mail[%lu] is not find", m_role_uid, mail_id);
		notify.set_reply_code(errcode_enum::mail_error_not_find);
		return;
	}

	if (itr->second.common_info().state() == proto::common::MAIL_STATE_REMOVE)
	{
		log_error("role[%lu] mail[%lu] is removed", m_role_uid, mail_id);
		notify.set_reply_code(errcode_enum::mail_error_is_removed);
		return;
	}

	uint32_t now_time = common::time_util_t::now_time();
	if (now_time > itr->second.del_time())
	{
		log_error("role[%lu] mail[%lu] is time out", m_role_uid, mail_id);
		notify.set_reply_code(errcode_enum::mail_error_is_time_out);
		return;
	}

	itr->second.mutable_common_info()->set_state(proto::common::MAIL_STATE_REMOVE);

	notify.set_reply_code(errcode_enum::error_ok);

	proto::common::mail_common_info* common_info = notify.add_common_infos();
	if (NULL != common_info)
	{
		common_info->CopyFrom(itr->second.common_info());
	}

	mail_data_mgr_t::set_mail_delete(mail_id, m_role_uid, type);

	m_mail_map.erase(itr);
}

void mail_user_data_t::clear_mail(proto::client::ec_mail_state_change_notify& notify, EM_MAIL_DEL_TYPE type/* = MAIL_DEL_USER*/)
{
	uint32_t now_time = common::time_util_t::now_time();

	for (mail_base_map::iterator itr = m_mail_map.begin();
		itr != m_mail_map.end();)
	{
		// 删除已经打开的并且没有附件的邮件
		if (itr->second.common_info().state() != proto::common::MAIL_STATE_NEW && itr->second.common_info().addenda() != proto::common::MAIL_ADDENDA_NEW)
		{
			itr->second.mutable_common_info()->set_state(proto::common::MAIL_STATE_REMOVE);

			proto::common::mail_common_info* common_info = notify.add_common_infos();
			if (NULL != common_info)
			{
				common_info->CopyFrom(itr->second.common_info());
			}

			mail_data_mgr_t::set_mail_delete(itr->first, m_role_uid, type);

			m_mail_map.erase(itr++);
		}
		//  删除超时
		else if (now_time > itr->second.del_time())
		{
			itr->second.mutable_common_info()->set_state(proto::common::MAIL_STATE_REMOVE);

			proto::common::mail_common_info* common_info = notify.add_common_infos();
			if (NULL != common_info)
			{
				common_info->CopyFrom(itr->second.common_info());
			}

			mail_data_mgr_t::set_mail_delete(itr->first, m_role_uid, MAIL_DEL_TIME_OUT);

			m_mail_map.erase(itr++);
		}
		else
		{
			++itr;
		}
	}

	if (notify.common_infos_size() > 0)
	{
		notify.set_reply_code(errcode_enum::error_ok);
	}
	else
	{
		notify.set_reply_code(errcode_enum::mail_error_clear_stop);
	}
}

void mail_user_data_t::clear_mail(EM_MAIL_DEL_TYPE type /*= MAIL_DEL_USER*/)
{
	uint32_t now_time = common::time_util_t::now_time();

	for (mail_base_map::iterator itr = m_mail_map.begin();
		itr != m_mail_map.end();)
	{
		// 删除已经打开的并且没有附件的邮件
		if (itr->second.common_info().state() != proto::common::MAIL_STATE_NEW && itr->second.common_info().addenda() != proto::common::MAIL_ADDENDA_NEW)
		{
			mail_data_mgr_t::set_mail_delete(itr->first, m_role_uid, type);

			m_mail_map.erase(itr++);
		}
		//  删除超时
		else if (now_time > itr->second.del_time())
		{
			mail_data_mgr_t::set_mail_delete(itr->first, m_role_uid, MAIL_DEL_TIME_OUT);

			m_mail_map.erase(itr++);
		}
		else
		{
			++itr;
		}
	}
}

uint32_t mail_user_data_t::mail_get_send_to_game(uint64_t mail_id, global_user_ptr p_user)
{
	if (NULL == p_user)
	{
		log_error("p_user null error");
		return errcode_enum::mail_error_not_find;
	}

	mail_base_map::iterator itr = m_mail_map.find(mail_id);
	if (itr == m_mail_map.end())
	{
		log_error("role[%lu] mail[%lu] is not find", m_role_uid, mail_id);
		return errcode_enum::mail_error_not_find;
	}

	if (itr->second.common_info().state() == proto::common::MAIL_STATE_REMOVE)
	{
		log_error("role[%lu] mail[%lu] is removed", m_role_uid, mail_id);
		return errcode_enum::mail_error_is_removed;
	}

	uint32_t now_time = common::time_util_t::now_time();
	if (now_time > itr->second.del_time())
	{
		log_error("role[%lu] mail[%lu] is time out", m_role_uid, mail_id);
		return errcode_enum::mail_error_is_time_out;
	}

	if (itr->second.common_info().addenda() != proto::common::MAIL_ADDENDA_NEW)
	{
		log_error("role[%lu] mail[%lu] is already got or have no item", m_role_uid, mail_id);
		return errcode_enum::mail_error_addenda_is_got;
	}

	if (!mail_data_mgr_t::check_can_get(mail_id, now_time))
	{
		log_error("role[%lu] mail[%lu] is can not get now", m_role_uid, mail_id);
		return errcode_enum::mail_error_is_getting;
	}

	proto::server::eg_mail_get_addenda_request request;
	request.set_uid(m_role_uid);
	mail_data_mgr_t::get_mail_addenda(mail_id, now_time, request);

	p_user->send_msg_to_game(op_cmd::eg_mail_get_addenda_request, request);

	return errcode_enum::error_ok;
}

void mail_user_data_t::mail_get_game_back(global_user_ptr p_user, const proto::server::ge_mail_get_addenda_reply& reply)
{
	if (NULL == p_user)
	{
		log_error("p_user null error");
		return;
	}

	proto::client::ec_mail_state_change_notify notify;
	uint32_t back_code = errcode_enum::error_ok;
	for (int32_t i = 0; i < reply.backs_size(); ++i)
	{
		const proto::server::mail_get_addenda_back& back_info = reply.backs(i);

		if (back_info.reply_code() == errcode_enum::error_ok)
		{
			set_mail_get_delete(back_info.mail_id(), notify);
		}
		else
		{
			mail_data_mgr_t::clear_get_mask(back_info.mail_id());

			back_code = back_info.reply_code();
		}
	}

	notify.set_reply_code(back_code);
	p_user->send_msg_to_client(op_cmd::ec_mail_state_change_notify, notify);
}

void mail_user_data_t::get_all_mails(global_user_ptr p_user)
{
	if (NULL == p_user)
	{
		log_error("p_user null error");
		return;
	}

	uint32_t now_time = common::time_util_t::now_time();

	proto::server::eg_mail_get_addenda_request request;
	request.set_uid(m_role_uid);

	for (mail_base_map::iterator itr = m_mail_map.begin();
		itr != m_mail_map.end(); ++itr)
	{
		if (itr->second.common_info().state() == proto::common::MAIL_STATE_REMOVE)
			continue;

		if (now_time > itr->second.del_time())
			continue;

		if (itr->second.common_info().addenda() != proto::common::MAIL_ADDENDA_NEW)
			continue;

		if (!mail_data_mgr_t::check_can_get(itr->first, now_time))
			continue;

		mail_data_mgr_t::get_mail_addenda(itr->first, now_time, request);
	}

	p_user->send_msg_to_game(op_cmd::eg_mail_get_addenda_request, request);
}

bool mail_user_data_t::add_new_mail(const new_mail_define& mail_define, global_user_ptr p_user)
{
	if (NULL == p_user)
	{
		log_error("p_user null error");
		return false;
	}

	if (m_mail_map.size() > MAX_MIAL_SIZE - 1)
	{
		clear_mail(MAIL_DEL_TOO_MUCH);

		// 当全不能删除时，删除最早的一封邮件
		if (m_mail_map.size() > MAX_MIAL_SIZE - 1)
		{
			force_del_mail(m_mail_map.size() - MAX_MIAL_SIZE + 1);
		}
	}

	proto::common::mail_base_info mail_info;
	proto::common::mail_common_info* p_common_info = mail_info.mutable_common_info();
	if (NULL == p_common_info)
	{
		log_error("role[%lu] p_common_info null error", m_role_uid);
		return false;
	}

	p_common_info->set_id(common::string_util_t::uint64_to_string(mail_define.mail_id));
	p_common_info->set_type((proto::common::em_mail_type)mail_define.mail_type);
	p_common_info->set_state((proto::common::em_mail_state)mail_define.mail_state);
	p_common_info->set_addenda((proto::common::em_mail_addenda)mail_define.item_state);

	if (mail_define.content_id > 0)
	{
		mail_info.set_content_id(mail_define.content_id);
	}

	mail_info.set_from_name(mail_define.from_name);
	mail_info.set_title(mail_define.title);

	mail_info.set_create_time(mail_define.create_time);
	mail_info.set_del_time(mail_define.del_time);

	//proto::client::ec_mail_list_notify notify;
	//proto::common::mail_base_info* p_mail_base = notify.add_base_infos();
	//if (NULL != p_mail_base)
	//{
	//	p_mail_base->CopyFrom(mail_info);
	//	p_user->send_msg_to_client(op_cmd::ec_mail_list_notify, notify);
	//}

	m_mail_map.insert(std::make_pair(mail_define.mail_id, mail_info));

	red_point_manager_t::red_point_notify(m_role_uid,proto::common::RED_POINT_MAIL);

	return true;
}

bool mail_user_data_t::add_new_global_mail(const new_mail_define& mail_define, global_user_ptr p_user)
{
	mail_user_data_t::add_new_mail(mail_define, p_user);
	return true;
}


