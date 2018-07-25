#include "transfer_msg_handler.hpp"
#include "user/user_manager.hpp"
#include "game/send_game_msg.hpp"
#include "transfer/transfer_manager.hpp"
#include "hero_strategy/hero_strategy_manager.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<op_cmd::FH_BEGIN, op_cmd::FH_END, transfer_msg_handler_t::msg_handler_func> transfer_msg_handler_t::m_transfer_msg_handle;
server_msg_handler_t<op_cmd::RH_BEGIN, op_cmd::RH_END, transfer_msg_handler_t::msg_handler_func> transfer_msg_handler_t::m_area_msg_handle;

bool transfer_msg_handler_t::init_msg_handler()
{
    bool ret = true;

	ret &= m_transfer_msg_handle.register_func(op_cmd::fh_test_reply,						handle_fh_test_reply);
	ret &= m_transfer_msg_handle.register_func(op_cmd::fh_get_server_channel_list_reply,	handle_fh_get_server_channel_list_reply);
	ret &= m_transfer_msg_handle.register_func(op_cmd::fh_change_server_channel_reply,		handle_fh_change_server_channel_reply);
	ret &= m_transfer_msg_handle.register_func(op_cmd::fh_change_server_channel_notify,		handle_fh_change_server_channel_notify);
	ret &= m_transfer_msg_handle.register_func(op_cmd::fh_server_channel_chat_reply,		handle_fh_server_channel_chat_reply);
	ret &= m_transfer_msg_handle.register_func(op_cmd::fh_server_channel_chat_notify,		handle_fh_server_channel_chat_notify);
	ret &= m_transfer_msg_handle.register_func(op_cmd::fh_personal_chat_notify,				handle_fh_personal_chat_notify);
	ret &= m_transfer_msg_handle.register_func(op_cmd::fh_syn_transfer_info_notify,			handle_fh_syn_transfer_info_notify);

	ret &= m_area_msg_handle.register_func(op_cmd::rh_get_hero_strategy_info_reply,		handle_rh_get_hero_strategy_info_reply);
		
    return ret;
}

bool transfer_msg_handler_t::handle_fh_test_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG(proto::server::fh_test_reply);
    return true;
}

bool transfer_msg_handler_t::handle_fh_get_server_channel_list_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::fh_get_server_channel_list_reply);
	user_ptr p_user = user_manager_t::get_user(uid);
	if (NULL == p_user)
	{
        log_error("NULL == p_user");
		return false;
	}
	p_user->send_msg_to_client(op_cmd::hc_get_server_channel_list_reply, msg);

	return true;
}

bool transfer_msg_handler_t::handle_fh_change_server_channel_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::fh_change_server_channel_reply);
	user_ptr p_user = user_manager_t::get_user(uid);
    if (NULL == p_user)
    {
        log_error("NULL == p_user");
        return false;
    }
	send_game_msg_t::send_user_channel_notify(p_user->get_game_id(), uid, msg.channel_id());
	p_user->send_msg_to_client(op_cmd::hc_change_server_channel_reply, msg);
	return true;
}

bool transfer_msg_handler_t::handle_fh_change_server_channel_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::fh_change_server_channel_notify);

	user_ptr p_user = user_manager_t::get_user(uid);
	if (NULL == p_user)
    {
        log_error("NULL == p_user");
        return false;
    }

	proto::server::ha_change_server_channel_notify ha_msg;
	ha_msg.mutable_fh_msg()->CopyFrom(msg);
	
	p_user->send_msg_to_client(op_cmd::ha_change_server_channel_notify, ha_msg);
	return true;
}

bool transfer_msg_handler_t::handle_fh_server_channel_chat_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::fh_server_channel_chat_reply);

	user_ptr p_user = user_manager_t::get_user(uid);
	if (NULL == p_user)
    {
        log_error("NULL == p_user");
        return false;
    }
	p_user->send_msg_to_client(op_cmd::hc_server_channel_chat_reply, msg);
	return true;
}

bool transfer_msg_handler_t::handle_fh_server_channel_chat_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::fh_server_channel_chat_notify);

	user_ptr p_user = user_manager_t::get_user(uid);
	if (NULL == p_user)
    {
        log_error("NULL == p_user");
        return false;
    }
	proto::server::ha_server_channel_chat_notify ha_msg;
	ha_msg.mutable_fh_msg()->CopyFrom(msg);
	p_user->send_msg_to_client(op_cmd::ha_server_channel_chat_notify, ha_msg);
	return true;
}

bool transfer_msg_handler_t::handle_fh_personal_chat_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::fh_personal_chat_notify);

	user_ptr p_user = user_manager_t::get_user(uid);
	if (NULL == p_user)
    {
        log_error("NULL == p_user");
        return false;
    }

// 	proto::server::ha_personal_chat_notify chat_notify_msg;
// 	chat_notify_msg.mutable_msg()->CopyFrom(msg.msg());
// 	p_user->send_msg_to_client(op_cmd::ha_personal_chat_notify, chat_notify_msg);


	return true;
}

bool transfer_msg_handler_t::handle_fh_syn_transfer_info_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::fh_syn_transfer_info_notify);

	transfer_manager_t::set_transfer(s, msg.server_id());

	return true;
}

bool transfer_msg_handler_t::handle_rh_get_hero_strategy_info_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::rh_get_hero_strategy_info_reply);

	hero_strategy_manager_t::load_hero_strategy_info(msg);
	
	return true;
}

