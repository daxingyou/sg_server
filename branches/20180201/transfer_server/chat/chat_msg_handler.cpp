#include "chat_msg_handler.hpp"
#include "main/transfer_server.hpp"
#include "main/transfer_server_fwd.hpp"
#include "protos_fwd.hpp"
#include "main/transfer_server_fwd.hpp"
#include "main/transfer_server.hpp"
#include "channel/chat_channel_manager.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<op_cmd::HF_BEGIN, op_cmd::HF_END, chat_msg_handler_t::msg_handler_func> chat_msg_handler_t::m_chat_msg_handle;

bool chat_msg_handler_t::init_msg_handler()
{
	bool ret = true;
	ret &= m_chat_msg_handle.register_func(op_cmd::hf_syn_chat_info_notify,					handle_hf_syn_chat_info_notify);
	ret &= m_chat_msg_handle.register_func(op_cmd::hf_get_server_channel_list_request,		handle_hf_get_server_channel_list_request);
	ret &= m_chat_msg_handle.register_func(op_cmd::hf_change_server_channel_request,		handle_hf_change_server_channel_request);
	ret &= m_chat_msg_handle.register_func(op_cmd::hf_server_channel_chat_request,			handle_hf_server_channel_chat_request);
	ret &= m_chat_msg_handle.register_func(op_cmd::hf_gm_cmd_notify,						handle_hf_gm_cmd_notify);
	ret &= m_chat_msg_handle.register_func(op_cmd::hf_personal_chat_request,				handle_hf_personal_chat_request);
    return ret;
}

bool chat_msg_handler_t::handle_hf_syn_chat_info_notify(const network::tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG(proto::server::hf_syn_chat_info_notify);

	chat_ptr p_chat = chat_manager_t::get_chat(s);
	if (p_chat == nullptr)
	{
		return false;
	}

    chat_manager_t::set_chat_server_info(s, msg.server_group_id(), msg.server_name());
	uint32_t channel_id = chat_channel_manager_t::add_channel(msg.server_name(), proto::common::channel_type_server);
	p_chat->set_channel_id(channel_id);
	log_info("chat login [%u, %s]", msg.server_group_id(), msg.server_name().c_str());
	return true;
}

bool chat_msg_handler_t::handle_hf_get_server_channel_list_request(const network::tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::hf_get_server_channel_list_request);
	chat_ptr p_chat = chat_manager_t::get_chat(s);
	if (p_chat == nullptr)
	{
		return false;
	}
	proto::server::fh_get_server_channel_list_reply reply;
	reply.set_reply_code(0);
	chat_channel_manager_t::peek_all_datas(reply.mutable_server_channel_data());
	env::server->send_msg_to_chat(p_chat->get_server_id(), op_cmd::fh_get_server_channel_list_reply, uid, reply);
	log_debug("user [%lu] get channel list", uid);
	return true;
}

bool chat_msg_handler_t::handle_hf_change_server_channel_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::hf_change_server_channel_request);
	chat_ptr p_src_chat = chat_manager_t::get_chat(s);
	if (p_src_chat == nullptr)
	{
		log_error("p_src_chat == nullptr");
		return false;
	}

	uint32_t src_chat_id = p_src_chat->get_server_id();
	std::string src_chat_name = p_src_chat->get_server_name();
	uint32_t dst_channel_id = msg.dst_channel_id();
	std::string user_name = msg.user_name();
	uint32_t type = msg.type();
	uint32_t reply_code = 0;

	chat_channel_ptr p_channel = chat_channel_manager_t::get_channel(dst_channel_id);
	if (nullptr == p_channel)
	{
		log_error("nullptr == p_channel");
		return false;
	}

	chat_user_ptr p_user = p_src_chat->get_user(uid);
	if (nullptr == p_user)
	{
		p_user = chat_user_ptr(new chat_user_t());
		if (nullptr == p_user)
		{
			log_error("nullptr == p_user");
			return false;
		}
		p_user->init_chat_usser(uid, user_name, src_chat_id);
		p_src_chat->add_chat_user(p_user);
	}

	if (type == 0)
	{
		p_channel->earse_user(uid);
		p_user->set_channel_id(0);
	}
	else
	{
		uint32_t src_channel_id = p_user->get_channel_id();
		if (0 != src_channel_id)
		{
			chat_channel_ptr p_src_channel = chat_channel_manager_t::get_channel(dst_channel_id);
			p_src_channel->earse_user(uid);
		}
		if (!p_channel->add_user(p_user))
		{
			reply_code = 1;
		}
		else
		{
			p_user->set_channel_id(dst_channel_id);
		}
	}

	proto::server::fh_change_server_channel_reply reply;
	reply.set_reply_code(reply_code);
	reply.set_channel_id(dst_channel_id);
	p_user->send_chat_msg(op_cmd::fh_change_server_channel_reply, reply);

	proto::server::fh_change_server_channel_notify ntf;

	proto::client::hc_change_server_channel_notify *p_info = ntf.mutable_hc_msg();
	p_info->set_src_chat_id(src_chat_id);
	p_info->set_src_chat_name(src_chat_name);
	p_info->set_user_uid(string_util_t::uint64_to_string(uid));
	p_info->set_user_name(user_name);
	p_info->set_type(type);
	p_channel->channel_send_chat_msg(op_cmd::fh_change_server_channel_notify, uid, ntf, ntf.mutable_role_list());

	log_debug("user [%lu, %s] change server_channel from[%u, %s], type [%u]", uid, user_name.c_str(), src_chat_id, src_chat_name.c_str(), type);
	return true;
}

bool chat_msg_handler_t::handle_hf_server_channel_chat_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::hf_server_channel_chat_request);

	chat_ptr p_src_chat = chat_manager_t::get_chat(s);
	if (p_src_chat == nullptr)
	{
		log_error("nullptr == p_src_chat");
		return false;
	}

	chat_user_ptr p_user = p_src_chat->get_user(uid);
	if (nullptr == p_user)
	{
		log_error("nullptr == p_user");
		return false;
	}

	uint32_t src_chat_id = p_src_chat->get_server_id();
	std::string src_chat_name = p_src_chat->get_server_name();
	uint32_t dst_channel_id = p_user->get_channel_id();
	std::string user_name = p_user->get_user_name();

	chat_channel_ptr p_channel = chat_channel_manager_t::get_channel(dst_channel_id);
	if (nullptr == p_channel)
	{
		log_error("nullptr == p_channel");
		return false;
	}

	proto::server::fh_server_channel_chat_reply reply;
	reply.set_reply_code(0);
	p_user->send_chat_msg(op_cmd::fh_server_channel_chat_reply, reply);


	proto::server::fh_server_channel_chat_notify ntf;
	proto::client::hc_server_channel_chat_notify *p_info = ntf.mutable_hc_msg();
	p_info->set_src_chat_id(src_chat_id);
	p_info->set_src_chat_name(src_chat_name);
	p_info->set_user_uid(string_util_t::uint64_to_string(uid));
	p_info->set_user_name(user_name);
	p_info->set_chat_info(msg.chat_info());
	p_channel->channel_send_chat_msg(op_cmd::fh_server_channel_chat_notify, uid, ntf, ntf.mutable_role_list());
	
	log_debug("user [%lu, %s] server_channel chat from[%u, %s], info [%s]", uid, user_name.c_str(), src_chat_id, src_chat_name.c_str(), msg.chat_info().c_str());
	return true;
}

bool chat_msg_handler_t::handle_hf_gm_cmd_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::hf_gm_cmd_notify);
	chat_ptr p_src_chat = chat_manager_t::get_chat(s);
	if (p_src_chat == nullptr)
	{
		log_error("nullptr == p_src_chat");
		return false;
	}
	if (msg.cmds_size() == 0)
	{
		log_error("msg.cmds_size() == 0");
		return false;
	}
	const std::string cmd = msg.cmds(0);
	if (cmd == "/createchannel")
	{
		if (msg.cmds_size() < 2)
		{
			return false;
		}
		std::string channel_name = msg.cmds(1);
		chat_channel_manager_t::add_channel(channel_name, proto::common::channel_type_features);
	}
    return true;
}

bool chat_msg_handler_t::handle_hf_personal_chat_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::hf_personal_chat_request);

// 	chat_channel_ptr p_channel = chat_channel_manager_t::get_channel(msg.msg().channel_id());
// 	if (nullptr == p_channel)
// 	{
// 		log_error("[wys] nullptr == p_channel id:%d", msg.msg().channel_id());
// 		return false;
// 	}
//     uint64_t role_target_uid = string_util_t::string_to_uint64(msg.msg().tar_role_id());
// 	chat_user_ptr tar_user = p_channel->get_user(role_target_uid);
// 	if (nullptr == tar_user)
// 	{
// 		log_error("[wys] nullptr == tar_user id:%lu", role_target_uid);
// 		return false;
// 	}
// 	
// 	proto::server::fh_personal_chat_notify notify_msg;
// 
// 	notify_msg.mutable_msg()->set_src_role_id(string_util_t::uint64_to_string(uid));
// 	notify_msg.mutable_msg()->set_src_role_name(msg.src_role_name());
// 	notify_msg.mutable_msg()->set_chat_info(msg.msg().chat_info());
// 
// 	tar_user->send_chat_msg(op_cmd::fh_personal_chat_notify, notify_msg);
// 
// 	log_debug("[wys] personal transfer to chat uid[%lu]", role_target_uid);
	return true;
}

