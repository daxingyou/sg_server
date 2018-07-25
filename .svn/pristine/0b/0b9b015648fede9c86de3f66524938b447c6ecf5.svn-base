#include "mail_msg_handle.hpp"

#include "gate/gate_msg_handler.hpp"
#include "game/game_msg_handler.hpp"

#include "common.pb.h"
#include "client.pb.h"
#include "server.pb.h"

#include "user/global_user_manager.hpp"
#include "utility.hpp"
#include "main/center_server_fwd.hpp"
#include "main/center_server.hpp"
#include "user/global_user.hpp"
#include "mail/mail_user_data.hpp"
#include "tblh/errcode_enum.hpp"
#include "redis/mail_data_mgr.hpp"
#include "mail_manager.hpp"
#include "tblh/MailTable.tbls.h"
#include "config/config_manager.hpp"
#include "mail_common.hpp"
#include "global_mail_manager.hpp"

USING_NS_COMMON;

bool mail_msg_handle_t::init_gate_msg_handler()
{
	bool bresult = true;

	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_mail_list_request, handle_ce_mail_list_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_mail_content_request, handle_ce_mail_content_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_mail_get_addenda_request, handle_ce_mail_get_addenda_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_mail_remove_request, handle_ce_mail_remove_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_mail_batch_request, handle_ce_mail_batch_request);

	return bresult;
}

bool mail_msg_handle_t::init_game_msg_handler()
{
	bool bresult = true;

	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_mail_get_addenda_reply, handle_ge_mail_get_addenda_reply);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_mail_new_notify, handle_ge_mail_new_notify);

	return bresult;
}

bool mail_msg_handle_t::handle_ce_mail_list_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_mail_list_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	// 全局邮件是否有可以领取的
	global_mail_manager_t::check_user_new(p_user);

	proto::client::ec_mail_list_notify notify;
	p_user->get_mail_data().get_mail_list(notify);

	p_user->send_msg_to_client(op_cmd::ec_mail_list_notify, notify);

	return true;
}

bool mail_msg_handle_t::handle_ce_mail_content_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_mail_content_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	uint64_t mail_id = common::string_util_t::string_to_uint64(msg.m_id());

	proto::client::ec_mail_content_reply reply;
	p_user->get_mail_data().get_mail_content(mail_id, reply);
	p_user->send_msg_to_client(op_cmd::ec_mail_content_reply, reply);

	return true;
}

bool mail_msg_handle_t::handle_ce_mail_get_addenda_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_mail_get_addenda_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	uint64_t mail_id = common::string_util_t::string_to_uint64(msg.m_id());

	uint32_t back_code = p_user->get_mail_data().mail_get_send_to_game(mail_id, p_user);
	if (back_code != errcode_enum::error_ok)
	{
		proto::client::ec_mail_state_change_notify notify;
		notify.set_reply_code(back_code);
		p_user->send_msg_to_client(op_cmd::ec_mail_state_change_notify, notify);
	}

	return true;
}

bool mail_msg_handle_t::handle_ce_mail_remove_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_mail_remove_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	uint64_t mail_id = common::string_util_t::string_to_uint64(msg.m_id());

	proto::client::ec_mail_state_change_notify notify;
	p_user->get_mail_data().del_mail_by_id(mail_id, notify, MAIL_DEL_USER);
	p_user->send_msg_to_client(op_cmd::ec_mail_state_change_notify, notify);

	return true;
}

bool mail_msg_handle_t::handle_ce_mail_batch_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_mail_batch_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	switch (msg.act())
	{
	case proto::common::MAIL_BATCH_GET:
		{
			p_user->get_mail_data().get_all_mails(p_user);
		}
		break;
	case proto::common::MAIL_BATCH_CLEAR:
		{
			proto::client::ec_mail_state_change_notify notify;
			p_user->get_mail_data().clear_mail(notify);
			p_user->send_msg_to_client(op_cmd::ec_mail_state_change_notify, notify);
		}
		break;
	default:
		{
			log_error("user[%lu] handle_ce_mail_batch_request error type", uid);
		}
		break;
	}

	return true;
}

bool mail_msg_handle_t::handle_ge_mail_get_addenda_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_mail_get_addenda_reply);
	uint64_t user_id = msg.uid();
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(user_id);

	// 如果人物不存在，直接调用redis
	if (NULL != p_user)
	{
		p_user->get_mail_data().mail_get_game_back(p_user, msg);
	}
	else
	{
		log_error("p_user[%lu] null error when get addenda do redis", user_id);

		for (int32_t i = 0; i < msg.backs_size(); ++i)
		{
			const proto::server::mail_get_addenda_back& back_info = msg.backs(i);

			if (back_info.reply_code() == errcode_enum::error_ok)
			{
				mail_data_mgr_t::set_mail_get_delete(back_info.mail_id(), user_id, MAIL_DEL_GET);
			}
			else
			{
				mail_data_mgr_t::clear_get_mask(back_info.mail_id());
			}
		}
	}

	return true;
}

bool mail_msg_handle_t::handle_ge_mail_new_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_mail_new_notify);

	new_mail_define mail_define;

	uint32_t now_time = common::time_util_t::now_time();

	for (int32_t i = 0; i < msg.uids_size(); ++i)
	{
		uint64_t to_uid = msg.uids(i);
		mail_define.reset();

		mail_define.mail_id = mail_manager_t::get_new_mail_uid();
		mail_define.mail_state = proto::common::MAIL_STATE_NEW;
		mail_define.mail_type = msg.mail_type();
		mail_define.to_uid = to_uid;

		if (msg.has_content_id() && msg.content_id() > 0)
		{
			mail_define.content_id = msg.content_id();

			MailTable* p_mail = GET_CONF(MailTable, mail_define.content_id);
			if (NULL != p_mail)
			{
				mail_define.from_name = common::string_util_t::convert_to_utf8(p_mail->from_name());
				mail_define.title = common::string_util_t::convert_to_utf8(p_mail->title());
				//mail_define.content = p_mail->content();

				if (msg.has_items() && !msg.items().empty())
				{
					mail_define.items = msg.items();
				}
				else
				{
					if (!p_mail->addenda().empty())
						mail_define.items = p_mail->addenda();
				}
			}
			else
			{
				log_error("uid[%lu] content_id[%u] is not exist", to_uid, msg.content_id());
				continue;
			}
		}
		else
		{
			mail_define.from_uid = msg.from_id();
			mail_define.from_name = msg.from_name();
			mail_define.title = msg.title();
			mail_define.content = msg.content();
			
			if (msg.has_items() && !msg.items().empty())
			{
				mail_define.items = msg.items();
			}
		}

		mail_define.create_time = now_time;
		mail_define.del_time = now_time + MAX_MAIL_TIME;

		if (!mail_define.items.empty())
		{
			mail_define.item_state = proto::common::MAIL_ADDENDA_NEW;
		}

		mail_define.get_type = msg.get_type();

		mail_data_mgr_t::add_new_mail(mail_define);

		// 是否在线，在线就通知client
		global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(to_uid);
		if (NULL != p_user)
		{
			p_user->get_mail_data().add_new_mail(mail_define, p_user);
		}
	}

	return true;
}
