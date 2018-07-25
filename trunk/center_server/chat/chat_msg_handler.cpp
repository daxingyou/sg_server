#include "chat_msg_handler.hpp"
#include "main/center_server.hpp"
#include "main/center_server_fwd.hpp"
#include "gate/gate_msg_handler.hpp"
#include "config/config_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "tblh/ForbidWordTable.tbls.h"
#include "tblh/Comprehensive.tbls.h"
#include "offline_friend_msg_mgr/offline_friend_msg_mgr.hpp"
#include "user/global_user.hpp"
#include "user/global_user_manager.hpp"
#include "redis/friend_data_mgr.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;


server_msg_handler_t<op_cmd::HE_BEGIN, op_cmd::HE_END, chat_msg_handler_t::msg_handler_func> chat_msg_handler_t::m_chat_msg_handler;

bool chat_msg_handler_t::init_msg_handler()
{
    bool ret = true;

	ret &= m_chat_msg_handler.register_func(op_cmd::he_test_reply, handle_he_test_reply);
	ret &= m_chat_msg_handler.register_func(op_cmd::he_family_chat_notify, handle_he_family_chat_notify);

    return ret;
}


bool chat_msg_handler_t::init_client_msg_handler()
{
	bool ret = true;
	ret &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_personal_chat_request, handle_ce_personal_chat_request);
	ret &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_personal_chat_notice_request, handle_ce_personal_chat_notice_request);
	return ret;
}

bool chat_msg_handler_t::handle_he_test_reply(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::he_test_reply);
	//log_warn("%lu center recv [he_test_reply] from chat", uid);

	return true;
}

bool chat_msg_handler_t::handle_he_family_chat_notify(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::he_family_chat_notify);
	return true;
}

bool chat_msg_handler_t::one_chat_to_another(uint64_t uid, const proto::client::ce_personal_chat_request & msg, proto::client::ec_personal_chat_reply* reply)
{
	global_user_ptr p_src_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_src_user)
	{
		log_error("NULL == p_src_user");
		return false;
	}

	// 聊天的屏蔽字过滤由客户端做
	//std::string chat_info = msg.chat_info();
	//uint32_t reply_code = check_chat_info_valid(chat_info);
	//reply_msg.set_reply_code(reply_code);
	//if (reply_code != errcode_enum::error_ok)
	//{
	//	p_src_user->send_msg_to_client(op_cmd::ec_personal_chat_reply, reply_msg);
	//	return true;
	//}

	auto conf = GET_CONF(Comprehensive, comprehensive_common::chat_max_len);
	uint32_t chat_max_len = GET_COMPREHENSIVE_VALUE_1(conf);
	chat_max_len = std::min<uint32_t>(1024, chat_max_len);

	if (msg.chat_info().length() < 1 || msg.chat_info().length() > chat_max_len)
	{
		log_error("role[%lu] msg length[%d] error!", uid, (int32_t)msg.chat_info().length());
		if (reply)
		{
			reply->set_reply_code(errcode_enum::notice_content_legal);
			p_src_user->send_msg_to_client(op_cmd::ec_personal_chat_reply, *reply);
		}
		return true;
	}

	uint64_t target_uid = string_util_t::string_to_uint64(msg.tar_role_id());

	// 判断对方是否在我黑名单
	if (p_src_user->get_friend_mgr().get_friend_type_by_uid(target_uid) == proto::common::friend_type_black)
	{
		if (reply)
		{
			reply->set_reply_code(errcode_enum::friend_chat_black_other);
			p_src_user->send_msg_to_client(op_cmd::ec_personal_chat_reply, *reply);
		}
		return true;
	}

	// 得到我相对于对方的关系
	proto::common::friend_type friend_me_type = proto::common::friend_type_none;

	global_user_ptr p_tar_user = global_user_manager_t::get_global_user_by_uid(target_uid);
	if (p_tar_user != NULL)
	{
		friend_me_type = p_tar_user->get_friend_mgr().get_friend_type_by_uid(uid);
	}
	else
	{
		friend_me_type = friend_data_mgr_t::get_offline_friend_type(target_uid, uid);
	}

	// 判断我是否在对方黑名单
	if (friend_me_type == proto::common::friend_type_black)
	{
		if (reply)
		{
			reply->set_reply_code(errcode_enum::friend_chat_black);
			p_src_user->send_msg_to_client(op_cmd::ec_personal_chat_reply, *reply);
		}
		return true;
	}

	if (0 != msg.chat_param_size())
	{
		proto::server::eg_chat_get_role_info_request msg_reg;
		for (int i = 0; i < msg.chat_param_size(); ++i)
		{
			proto::common::chat_param* p_param = msg_reg.add_param_list();
			p_param->CopyFrom(msg.chat_param(i));
			///log_warn("param type[%d] value[%s]", p_param->type(), p_param->value().c_str());
		}

		proto::common::chat_common_data chat_info_data;
		chat_info_data.set_src_uid(string_util_t::uint64_to_string(uid));
		chat_info_data.set_src_name(p_src_user->get_name());
		chat_info_data.set_chat_info(msg.chat_info());
		chat_info_data.set_head_icon_id(msg.head_id());
		chat_info_data.set_role_level(p_src_user->get_level());
		chat_info_data.set_tar_role_id(msg.tar_role_id());
		proto::common::chat_common_data* p_chat_data = msg_reg.mutable_chat_notify();
		p_chat_data->CopyFrom(chat_info_data);
		p_src_user->send_msg_to_game(op_cmd::eg_chat_get_role_info_request, msg_reg);
		return true;
	}

	uint32_t now_time = common::time_util_t::now_time();

	proto::client::ec_personal_chat_notify notify_msg;
	notify_msg.set_src_role_id(string_util_t::uint64_to_string(uid));
	notify_msg.set_src_role_name(p_src_user->get_name());
	notify_msg.set_chat_info(msg.chat_info());
	notify_msg.set_src_head_id(msg.head_id());
	notify_msg.set_src_level(p_src_user->get_level());
	notify_msg.set_contact_time(now_time);

	// 更新自己最近联系时间
	p_src_user->get_friend_mgr().update_contact_time(target_uid, now_time, true);

	// 更新对方最近联系时间
	if (p_tar_user != NULL)
	{
		p_tar_user->get_friend_mgr().update_contact_time(uid, now_time, true);
	}
	else
	{
		friend_data_mgr_t::update_offline_friend_contact_time(target_uid, uid, now_time);
	}

	// 对方不在线 记录离线消息
	if (NULL == p_tar_user || p_tar_user->get_online_state() != ROLE_ONLINE_STATE_ONLINE)
	{
		offline_friend_msg_mgr_t::add_offline_friend_msg(target_uid, notify_msg);
	}
	else
	{
		p_tar_user->send_msg_to_client(op_cmd::ec_personal_chat_notify, notify_msg);
	}

	if (reply)
	{
		reply->set_reply_code(errcode_enum::error_ok);
		reply->set_contact_time(now_time);
		p_src_user->send_msg_to_client(op_cmd::ec_personal_chat_reply, *reply);
	}
	return true;
}

bool chat_msg_handler_t::handle_ce_personal_chat_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_personal_chat_request);
	proto::client::ec_personal_chat_reply reply_msg;
	return chat_msg_handler_t::one_chat_to_another(uid, msg, &reply_msg);
}

bool chat_msg_handler_t::handle_ce_personal_chat_notice_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_personal_chat_notice_request);

	global_user_ptr p_src_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_src_user)
	{
		log_error("NULL == p_src_user");
		return false;
	}

	p_src_user->get_friend_mgr().chat_notice_all_friend(msg);
	return true;
}

int32_t chat_msg_handler_t::check_chat_info_valid(std::string& chat_info)
{
	auto conf = GET_CONF(Comprehensive, comprehensive_common::chat_max_len);
	uint32_t chat_max_len = GET_COMPREHENSIVE_VALUE_1(conf);
	chat_max_len = chat_max_len > 1024 ? 1024 : chat_max_len;	// 设定一个程序的聊天上限1024
																// 检查长度
	if (chat_info.length() < 1 || chat_info.length() > chat_max_len)
	{
		log_error("chat_info[%s] length[%d] error!", chat_info.c_str(), (int32_t)chat_info.length());
		return errcode_enum::notice_content_legal;
	}
	// 检查特殊字符

	// 检查敏感字
    std::map<uint32_t, ForbidWordTable*> all_confs;
    GET_ALL_CONF(ForbidWordTable, all_confs);
    std::string replace = "*";
    for (auto conf : all_confs)
    {
        std::string word = conf.second->word();
        std::string::size_type pos = 0;
        while ((pos = chat_info.find(word, pos)) != std::string::npos)
        {
            chat_info.replace(pos, word.size(), replace);
            pos += 1;
        }
    }
	return errcode_enum::error_ok;
}
