#include "chat_msg_handler.hpp"
#include "main/gate_server.hpp"
#include "main/gate_server_fwd.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<op_cmd::HA_BEGIN, op_cmd::HA_END, chat_msg_handler_t::msg_handler_func> chat_msg_handler_t::m_chat_msg_handle;

bool chat_msg_handler_t::init_msg_handler()
{
    bool ret = true;

	ret &= m_chat_msg_handle.register_func(op_cmd::ha_test_reply, handle_ha_test_reply);
	ret &= m_chat_msg_handle.register_func(op_cmd::ha_change_server_channel_notify, handle_ha_change_server_channel_notify);
	ret &= m_chat_msg_handle.register_func(op_cmd::ha_server_channel_chat_notify, handle_ha_server_channel_chat_notify);
	ret &= m_chat_msg_handle.register_func(op_cmd::ha_personal_chat_notify, handle_ha_personal_chat_notify);

    return ret;
}

bool chat_msg_handler_t::handle_ha_test_reply(const msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG(proto::server::ha_test_reply);
    //log_warn("%lu gate recv [ha_test_reply] from chat", uid);

    return true;
}

bool chat_msg_handler_t::handle_ha_change_server_channel_notify(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ha_change_server_channel_notify);

	proto::client::hc_change_server_channel_notify msg_to_client;
	msg_to_client.CopyFrom(msg.fh_msg().hc_msg());

	for (uint32_t i = 0; i < (uint32_t)msg.fh_msg().role_list().notify_role_arrary_size(); ++i)
	{
		uint64_t user_id = msg.fh_msg().role_list().notify_role_arrary(i);
		const user_ptr& p_user = user_manager_t::get_user(user_id);
		if (NULL == p_user)
		{
			log_warn("NULL == p_user[%lu]", user_id);
			continue;
		}
		p_user->send_msg_to_client(op_cmd::hc_change_server_channel_notify, msg_to_client);
	}
	return true;
}

bool chat_msg_handler_t::handle_ha_server_channel_chat_notify(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ha_server_channel_chat_notify);

	proto::client::hc_server_channel_chat_notify msg_to_client;
	msg_to_client.CopyFrom(msg.fh_msg().hc_msg());

	for (uint32_t i = 0; i < (uint32_t)msg.fh_msg().role_list().notify_role_arrary_size(); ++i)
	{
		uint64_t user_id = msg.fh_msg().role_list().notify_role_arrary(i);
		const user_ptr& p_user = user_manager_t::get_user(user_id);
		if (NULL == p_user)
		{
			log_warn("NULL == p_user[%lu]", user_id);
			continue;
		}
		p_user->send_msg_to_client(op_cmd::hc_server_channel_chat_notify, msg_to_client);
	}
	return true;
}

bool chat_msg_handler_t::handle_ha_personal_chat_notify(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ha_personal_chat_notify);

	const user_ptr& p_user = user_manager_t::get_user(uid);
	if (NULL == p_user)
	{
		log_error("[wys] NULL == p_user[%lu]", uid);
		return true;
	}

// 	proto::client::ec_personal_chat_notify msg_to_client;
// 	msg_to_client.CopyFrom(msg.msg());
// 	p_user->send_msg_to_client(op_cmd::hc_personal_chat_notify, msg_to_client);
	return true;
}
