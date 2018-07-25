#include "game_msg_handler.hpp"
#include "network.hpp"
#include "user/user_manager.hpp"
#include "family/family_manager.hpp"
#include "country/country_manager.hpp"
#include "protos_fwd.hpp"
#include "config/config_manager.hpp"
#include "common/DataManager.h"
#include "gate/gate_msg_handler.hpp"
#include "send_game_msg.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<op_cmd::GH_BEGIN, op_cmd::GH_END, game_msg_handler_t::msg_handler_func> game_msg_handler_t::m_game_msg_handle;

bool game_msg_handler_t::init_msg_handler()
{
    bool ret = true;

	ret &= m_game_msg_handle.register_func(op_cmd::gh_syn_game_info_notify,				handle_gh_syn_game_info_notify);
    ret &= m_game_msg_handle.register_func(op_cmd::gh_syn_online_role_notify,           handle_gh_syn_online_role_notify);
    ret &= m_game_msg_handle.register_func(op_cmd::gh_syn_online_role_list_notify,      handle_gh_syn_online_role_list_notify);
    ret &= m_game_msg_handle.register_func(op_cmd::gh_kick_role_notify,                 handle_gh_kick_role_notify);
	ret &= m_game_msg_handle.register_func(op_cmd::gh_get_nearby_roles_reply,			handle_gh_get_nearby_roles_reply);
	ret &= m_game_msg_handle.register_func(op_cmd::gh_task_seek_help_reply,				handle_gh_task_seek_help_reply);
	ret &= m_game_msg_handle.register_func(op_cmd::gh_gm_cmd_notify,					handle_gh_gm_cmd_notify);
	ret &= m_game_msg_handle.register_func(op_cmd::gh_reload_config_notify,				handle_gh_reload_config_notify);
	ret &= m_game_msg_handle.register_func(op_cmd::gh_chat_get_role_info_reply,			handle_gh_chat_get_role_info_reply);
    return ret;
}

bool game_msg_handler_t::handle_gh_syn_game_info_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::gh_syn_game_info_notify);

	game_manager_t::set_game(s, msg.game_id());

	return true;
}

bool game_msg_handler_t::handle_gh_syn_online_role_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG(proto::server::gh_syn_online_role_notify);

    user_ptr p_user = user_manager_t::get_user(uid);
    if (NULL == p_user)
    {
        uint32_t country_id = msg.chat_info().country_id();
        uint64_t family_id = msg.chat_info().family_id();
        std::string nickname = msg.chat_info().nickname();

        p_user = user_ptr(new user_t(uid, nickname, country_id, family_id));
        if (NULL == p_user)
        {
            log_error("NULL == p_user");
            return false;
		}

        uint32_t gate_id = msg.chat_info().gate_id();
        uint32_t game_id = game_manager_t::get_game_id_by_socket(s);
        user_manager_t::add_user(p_user, gate_id, game_id);
		if (country_id != 0)
		{
			country_manager_t::add_user(country_id, uid);
		}

		if (family_id != 0)
		{
			family_manager_t::add_user(family_id, uid);
		}

    }

    return true;
}

bool game_msg_handler_t::handle_gh_syn_online_role_list_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG(proto::server::gh_syn_online_role_list_notify);

    for (auto sys_info : msg.user_list())
    {
        uint64_t uid = sys_info.uid();
        uint32_t country_id = sys_info.country_id();
        uint64_t family_id = sys_info.family_id();
        std::string nickname = sys_info.nickname();
        uint32_t gate_id = sys_info.gate_id();
        uint32_t channel_id = sys_info.channel_id();
        uint32_t game_id = game_manager_t::get_game_id_by_socket(s);

        user_ptr p_user = user_manager_t::get_user(uid);
        if (NULL == p_user)
        {
            p_user = user_ptr(new user_t(uid, nickname, country_id, family_id));
            user_manager_t::add_user(p_user, gate_id, game_id);
        }

        if (country_id != 0)
        {
            country_manager_t::add_user(country_id, uid);
        }

        if (family_id != 0)
        {
            family_manager_t::add_user(family_id, uid);
        }

        if (channel_id != 0)
        {
            proto::server::hf_change_server_channel_request msg_req;
            msg_req.set_dst_channel_id(channel_id);
            msg_req.set_user_name(nickname);
            msg_req.set_type(1);
            env::server->send_msg_to_transfer(op_cmd::hf_change_server_channel_request, uid, msg_req);
        }
        
    }
    return true;
}

bool game_msg_handler_t::handle_gh_reload_config_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	reloadAllDTManager(env::xml->config_path);
	return true;
}

bool game_msg_handler_t::handle_gh_kick_role_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG(proto::server::gh_kick_role_notify);
    user_manager_t::kick_user(uid);
    return true;
}

bool game_msg_handler_t::handle_gh_chat_get_role_info_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::gh_chat_get_role_info_reply);
	user_ptr p_user = user_manager_t::get_user(uid);
	if (NULL == p_user)
	{
		log_warn("NULL == p_user");
		return false;
	}
	const proto::common::chat_common_data& chat_notify = msg.chat_notify();
	const proto::common::chat_common_role_info& chat_role_info = msg.chat_role_info();
	std::string chat_info = chat_notify.chat_info();
	uint32_t reply_code = gate_msg_handler_t::check_chat_info_valid(chat_info);
	if (msg.reply_code() == 0)
	{
		proto::client::hc_channel_chat_notify ntf;
		ntf.set_chat_channel(chat_notify.chat_channel());
		std::string suid = string_util_t::uint64_to_string(uid);
		ntf.set_src_uid(suid);
		ntf.set_src_name(p_user->get_nickname());
		ntf.set_chat_info(chat_info);
		ntf.set_head_icon_id(chat_notify.head_icon_id());
		ntf.set_role_level(chat_notify.role_level());
		ntf.set_param(chat_notify.param());
		proto::common::chat_common_role_info* p_data = ntf.mutable_chat_role_data();
		p_data->CopyFrom(chat_role_info);
		///log_warn("item_size[%d] hero_size[%d]", p_data->item_single_size(), p_data->hero_single_size());
		switch (chat_notify.chat_channel())
		{
		case proto::common::chat_channel_vicinity:
		case proto::common::chat_channel_team:
		{
			send_game_msg_t::send_get_nearby_roles_request(p_user->get_game_id(), uid, chat_notify.head_icon_id(), chat_notify.role_level(),
				chat_notify.chat_channel(), chat_notify.chat_info(), chat_notify.param(), chat_role_info);
			reply_code = 0;
		}
		break;
		case proto::common::chat_channel_create_team:
		case proto::common::chat_channel_world:
		case proto::common::chat_channel_system:
		{
			user_manager_t::send_to_all_users(op_cmd::hc_channel_chat_notify, ntf);
			reply_code = 0;
		}
		break;
		case proto::common::chat_channel_country:
		{
			reply_code = country_manager_t::send_country_msg(p_user->get_country_id(), op_cmd::hc_channel_chat_notify, ntf);
			break;
		}
		break;
		case proto::common::chat_channel_family:
		{
			reply_code = family_manager_t::send_family_msg(p_user->get_family_id(), op_cmd::hc_channel_chat_notify, ntf);

            // 暂时认为没有作用 by王思源
			//proto::server::he_family_chat_notify game_msg;
			//game_msg.set_src_uid(uid);
			//game_msg.set_family_uid(p_user->get_family_id());
			//reply_code &= family_manager_t::send_family_msg(p_user->get_family_id(), op_cmd::he_family_chat_notify, game_msg);
		}
		break;
		default:
			reply_code = 1;
			break;
		}
	}

	proto::client::hc_channel_chat_reply reply;
	reply.set_reply_code(reply_code);
	p_user->send_msg_to_client(op_cmd::hc_channel_chat_reply, reply);
	return true;
}

bool game_msg_handler_t::handle_gh_get_nearby_roles_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::gh_get_nearby_roles_reply);


	user_ptr p_user = user_manager_t::get_user(uid);
	if (NULL == p_user)
	{
		log_warn("NULL == p_user");
		return true;
	}

	if (msg.reply_code() == 0)
	{
		proto::client::hc_channel_chat_notify ntf;
		ntf.set_chat_channel(msg.chat_channel());
		std::string suid = string_util_t::uint64_to_string(uid);
		ntf.set_src_uid(suid);
		ntf.set_src_name(p_user->get_nickname());
		ntf.set_chat_info(msg.chat_info());
		ntf.set_head_icon_id(msg.head_icon_id());
		ntf.set_role_level(msg.role_level());
		ntf.set_param(msg.param());
		proto::common::chat_common_role_info* p_data = ntf.mutable_chat_role_data();
		p_data->CopyFrom(msg.chat_role_info());
		for (uint64_t role_uid : msg.role_uids())
		{
			user_ptr p_dst_user = user_manager_t::get_user(role_uid);
			if (p_dst_user != nullptr)
			{
				p_dst_user->send_msg_to_client(op_cmd::hc_channel_chat_notify, ntf);
			}
		}
	}
	return true;
}

bool game_msg_handler_t::handle_gh_task_seek_help_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::gh_task_seek_help_reply);

	return true;
}

bool game_msg_handler_t::handle_gh_gm_cmd_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::gh_gm_cmd_notify);

	env::server->send_msg_to_transfer(op_cmd::hf_gm_cmd_notify, uid, msg);
	return true;
}