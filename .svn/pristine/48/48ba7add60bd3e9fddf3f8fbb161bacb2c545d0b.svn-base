#include "gate_msg_handler.hpp"
#include "network.hpp"
#include "user/user_manager.hpp"
#include "country/country_manager.hpp"
#include "family/family_manager.hpp"
#include "game/send_game_msg.hpp"
#include "config/config_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "tblh/ForbidWordTable.tbls.h"
#include "tblh/Comprehensive.tbls.h"
#include "hero_strategy/hero_strategy_manager.hpp"
//#include "offline_friend_msg_mgr/offline_friend_msg_mgr.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<op_cmd::CH_BEGIN, op_cmd::CH_END, gate_msg_handler_t::msg_handler_func> gate_msg_handler_t::m_gate_msg_handle_from_client;
server_msg_handler_t<op_cmd::AH_BEGIN, op_cmd::AH_END, gate_msg_handler_t::msg_handler_func> gate_msg_handler_t::m_gate_msg_handle;

bool gate_msg_handler_t::init_msg_handler()
{
    bool ret = true;

    // CH client --> gate -- > chat
	ret &= m_gate_msg_handle_from_client.register_func(op_cmd::ch_test_request,						handle_ch_test_request);
	ret &= m_gate_msg_handle_from_client.register_func(op_cmd::ch_channel_chat_request,				handle_ch_channel_chat_request);
	ret &= m_gate_msg_handle_from_client.register_func(op_cmd::ch_get_server_channel_list_request,	handle_ch_get_server_channel_list_request);
	ret &= m_gate_msg_handle_from_client.register_func(op_cmd::ch_change_server_channel_request,	handle_ch_change_server_channel_request);
	ret &= m_gate_msg_handle_from_client.register_func(op_cmd::ch_server_channel_chat_request,		handle_ch_server_channel_chat_request);
//	ret &= m_gate_msg_handle_from_client.register_func(op_cmd::ch_personal_chat_request,			handle_ch_personal_chat_request);
	ret &= m_gate_msg_handle_from_client.register_func(op_cmd::ch_hero_strategy_hero_info_request,		handle_ch_hero_strategy_hero_info_request);
	ret &= m_gate_msg_handle_from_client.register_func(op_cmd::ch_hero_strategy_get_comments_request,	handle_ch_hero_strategy_get_comments_request);
	ret &= m_gate_msg_handle_from_client.register_func(op_cmd::ch_hero_strategy_like_request,			handle_ch_hero_strategy_like_request);
	ret &= m_gate_msg_handle_from_client.register_func(op_cmd::ch_hero_strategy_create_comment_request, handle_ch_hero_strategy_create_comment_request);
    // AH gate --> chat
	ret &= m_gate_msg_handle.register_func(op_cmd::ah_syn_gate_info_notify, handle_ah_syn_gate_info_notify);

    return ret;
}

// client --> gate -- > chat------------------------------------------------------------------------------------
bool gate_msg_handler_t::handle_ch_test_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG(proto::client::ch_test_request);
    //log_warn("%lu chat recv [ch_test_request] from client", uid);
    return true;
}

bool gate_msg_handler_t::handle_ch_channel_chat_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ch_channel_chat_request);

	user_ptr p_user = user_manager_t::get_user(uid);
	if (NULL == p_user)
	{
		log_warn("NULL == p_user");
		return false;
	}
	std::string chat_info = msg.chat_info();
	uint32_t reply_code = check_chat_info_valid(chat_info);

	if (reply_code != errcode_enum::error_ok)
	{
		proto::client::hc_channel_chat_reply reply;
		reply.set_reply_code(reply_code);
		p_user->send_msg_to_client(op_cmd::hc_channel_chat_reply, reply);
		return false;
	}

	proto::client::hc_channel_chat_notify ntf;
	ntf.set_chat_channel(msg.chat_channel());
	std::string suid = string_util_t::uint64_to_string(uid);
	ntf.set_src_uid(suid);
	ntf.set_src_name(p_user->get_nickname());
	ntf.set_chat_info(chat_info);
	ntf.set_head_icon_id(msg.head_icon_id());
	ntf.set_role_level(msg.role_level());
	ntf.set_param(msg.param());

	bool is_need_verify = false;
	///log_warn("role[%lu] send chat info chat_param_size[%d]",uid,msg.chat_param_size());
	std::vector<proto::common::chat_param> chat_param_vec;
	if (0 != msg.chat_param_size())
	{
		for (int i = 0; i < msg.chat_param_size(); ++i)
		{
			proto::common::chat_param param;
			param.CopyFrom(msg.chat_param(i));
			chat_param_vec.push_back(param);
			///log_warn("param type[%d] value[%s]",param.type(),param.value().c_str());
		}
		///log_warn("chat_param_vec size[%d]", chat_param_vec.size());
		if (!chat_param_vec.empty())
		{
			is_need_verify = true;
		}	
	}

	if (is_need_verify)
	{
		proto::common::chat_common_data chat_info_data;
		chat_info_data.set_chat_channel(msg.chat_channel());
		std::string suid = string_util_t::uint64_to_string(uid);
		chat_info_data.set_src_uid(suid);
		chat_info_data.set_src_name(p_user->get_nickname());
		chat_info_data.set_chat_info(chat_info);
		chat_info_data.set_head_icon_id(msg.head_icon_id());
		chat_info_data.set_role_level(msg.role_level());
		chat_info_data.set_param(msg.param());
		send_game_msg_t::send_chat_get_role_info_request(p_user->get_game_id(),uid, chat_info_data, chat_param_vec);
		return true;
	}
	else
	{
		switch (msg.chat_channel())
		{
		case proto::common::chat_channel_vicinity:
		case proto::common::chat_channel_team:
		{
			proto::common::chat_common_role_info chat_data;
			send_game_msg_t::send_get_nearby_roles_request(p_user->get_game_id(), uid, msg.head_icon_id(), msg.role_level(),
				msg.chat_channel(), msg.chat_info(), msg.param(), chat_data);
			reply_code = 0;
		}break;
		case proto::common::chat_channel_create_team:
		case proto::common::chat_channel_world:
		case proto::common::chat_channel_system:
		case proto::common::chat_channel_team_hold:
			user_manager_t::send_to_all_users(op_cmd::hc_channel_chat_notify, ntf);
			reply_code = 0;
			break;

		case proto::common::chat_channel_country:
			reply_code = country_manager_t::send_country_msg(p_user->get_country_id(), op_cmd::hc_channel_chat_notify, ntf);
			break;

		case proto::common::chat_channel_family:
		{
			reply_code = family_manager_t::send_family_msg(p_user->get_family_id(), op_cmd::hc_channel_chat_notify, ntf);
			proto::server::he_family_chat_notify game_msg;
			game_msg.set_src_uid(uid);
			game_msg.set_family_uid(p_user->get_family_id());
			reply_code &= family_manager_t::send_family_msg(p_user->get_family_id(), op_cmd::he_family_chat_notify, game_msg);
		}
		break;
		default:
			reply_code = 1;
			break;
		}
	}
	if (errcode_enum::error_ok == reply_code)
	{
		proto::server::hg_notify_progress_state_notify notify;
		notify.set_type(proto::common::Achieve_Event_Type::ACHIEVE_CHANNEL_CHAT);
		notify.set_param1(msg.chat_channel());
		notify.set_param2(1);
		env::server->send_msg_to_game(game_manager_t::get_game_socket_by_id(p_user->get_game_id()), op_cmd::hg_notify_progress_state_notify, uid, notify);
	}

	proto::client::hc_channel_chat_reply reply;
	reply.set_reply_code(reply_code);
	p_user->send_msg_to_client(op_cmd::hc_channel_chat_reply, reply);
	return true;
}


bool gate_msg_handler_t::handle_ch_get_server_channel_list_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ch_get_server_channel_list_request);
	user_ptr p_user = user_manager_t::get_user(uid);
	if (NULL == p_user)
	{
		log_warn("NULL == p_user");
		return false;
	}

	proto::server::hf_get_server_channel_list_request msg_req;
	env::server->send_msg_to_transfer(op_cmd::hf_get_server_channel_list_request, uid, msg_req);

	return true;
}

bool gate_msg_handler_t::handle_ch_change_server_channel_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ch_change_server_channel_request);
	user_ptr p_user = user_manager_t::get_user(uid);
	if (NULL == p_user)
	{
		log_warn("NULL == p_user");
		return false;
	}

	proto::server::hf_change_server_channel_request msg_req;
	msg_req.set_dst_channel_id(msg.dst_channel_id());
	msg_req.set_user_name(msg.user_name());
	msg_req.set_type(msg.type());
	env::server->send_msg_to_transfer(op_cmd::hf_change_server_channel_request, uid, msg_req);

	return true;
}


bool gate_msg_handler_t::handle_ch_server_channel_chat_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ch_server_channel_chat_request);
	user_ptr p_user = user_manager_t::get_user(uid);
	if (NULL == p_user)
	{
		log_warn("NULL == p_user");
		return false;
	}

	std::string chat_info = msg.chat_info();
	uint32_t reply_code = check_chat_info_valid(chat_info);

	if (reply_code != errcode_enum::error_ok)
	{
		proto::client::hc_server_channel_chat_reply reply;
		reply.set_reply_code(reply_code);
		p_user->send_msg_to_client(op_cmd::hc_server_channel_chat_reply, reply);
		return false;
	}

	proto::server::hf_server_channel_chat_request msg_req;
	msg_req.set_chat_info(chat_info);
	env::server->send_msg_to_transfer(op_cmd::hf_server_channel_chat_request, uid, msg_req);

	return true;
}

bool gate_msg_handler_t::handle_ch_hero_strategy_hero_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ch_hero_strategy_hero_info_request);
	user_ptr p_user = user_manager_t::get_user(uid);
	if (NULL == p_user)
	{
		log_warn("NULL == p_user");
		return false;
	}

	hero_strategy_manager_t::get_hero_strategy_info(p_user, msg.hero_tid());
	return true;
}

bool gate_msg_handler_t::handle_ch_hero_strategy_get_comments_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ch_hero_strategy_get_comments_request);

	return true;
}

bool gate_msg_handler_t::handle_ch_hero_strategy_like_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ch_hero_strategy_like_request);

	return true;
}

bool gate_msg_handler_t::handle_ch_hero_strategy_create_comment_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ch_hero_strategy_create_comment_request);

	return true;
}

// 
// bool gate_msg_handler_t::handle_ch_personal_chat_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
// {
// 	PRE_S2S_MSG(proto::client::ch_personal_chat_request);
// 
// 	user_ptr p_src_user = user_manager_t::get_user(uid);
// 	if (NULL == p_src_user)
// 	{
// 		log_warn("NULL == p_src_user");
// 		return true;
// 	}
// 
// 	proto::client::hc_personal_chat_reply reply_msg;
// 	std::string chat_info = msg.chat_info();
// 	uint32_t reply_code = check_chat_info_valid(chat_info);
// 	reply_msg.set_reply_code(reply_code);
// 	if (reply_code != errcode_enum::error_ok)
// 	{
// 		p_src_user->send_msg_to_client(op_cmd::hc_personal_chat_reply, reply_msg);
// 		return true;
// 	}
// 
// 	// 如果是跨服的聊天发给transfer_server
// 	if (msg.channel_id() != 0)
// 	{
// 		proto::server::hf_personal_chat_request chat_msg;
// 		chat_msg.mutable_msg()->CopyFrom(msg);
// 		chat_msg.set_src_role_name(p_src_user->get_nickname());
// 		env::server->send_msg_to_transfer(op_cmd::hf_personal_chat_request, uid, chat_msg);
// 
// 		p_src_user->send_msg_to_client(op_cmd::hc_personal_chat_reply, reply_msg);

// 	}
// 	else
// 	{
//         proto::client::hc_personal_chat_notify notify_msg;
//         notify_msg.set_src_role_id(string_util_t::uint64_to_string(uid));
//         notify_msg.set_src_role_name(p_src_user->get_nickname());
//         notify_msg.set_chat_info(msg.chat_info());
// 
// 		user_ptr p_tar_user = user_manager_t::get_user(string_util_t::string_to_uint64(msg.tar_role_id()));
// 		if (NULL == p_tar_user)
// 		{
//             
//             offline_friend_msg_mgr_t::add_offline_friend_msg(string_util_t::string_to_uint64(msg.tar_role_id()), notify_msg);
// 			reply_msg.set_reply_code(0);
// 		}
// 
// 		p_src_user->send_msg_to_client(op_cmd::hc_personal_chat_reply, reply_msg);

// 		
// 		if (p_tar_user)
// 		{
// 			p_tar_user->send_msg_to_client(op_cmd::hc_personal_chat_notify, notify_msg);
// 		}

// 	}
// 
// 	uint64_t role_target_uid = string_util_t::string_to_uint64(msg.tar_role_id());
// 	proto::server::he_personal_chat_notify he_msg;
// 	he_msg.set_src_uid(uid);
// 	he_msg.set_tar_uid(role_target_uid);
// 	env::server->send_msg_to_center(op_cmd::he_personal_chat_notify, uid, he_msg);
// 
// 	return true;
// }

// gate --> chat------------------------------------------------------------------------------------
bool gate_msg_handler_t::handle_ah_syn_gate_info_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG(proto::server::ah_syn_gate_info_notify);

    uint32_t gate_id = msg.gate_id();
    if (gate_id == 0)
    {
        log_error("gate_id == 0");
        return false;
    }

    gate_manager_t::set_gate(s, gate_id);

    return true;
}

int32_t gate_msg_handler_t::check_chat_info_valid(std::string& chat_info)
{
	auto conf = GET_CONF(Comprehensive, comprehensive_common::msg_strlen);
	uint32_t chat_max_len = GET_COMPREHENSIVE_VALUE_1(conf);
	chat_max_len = chat_max_len > 1024 ? 1024 : chat_max_len;	// 设定一个程序的聊天上限1024
	// 检查长度
	///log_warn("length[%d] size[%d]", chat_info.length(), chat_info.size());
	if (chat_info.length() < 1 || chat_info.length() > chat_max_len)
	{
		log_error("chat_info[%s] length[%d] error!", chat_info.c_str(), chat_info.length());
		return errcode_enum::notice_content_legal;
	}
	// 检查特殊字符

	// 检查敏感字
	/*
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
	*/
	return errcode_enum::error_ok;
}
