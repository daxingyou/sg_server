#include "account_msg_handler.hpp"
#include "main/db_server.hpp"
#include "main/db_server_fwd.hpp"
#include "network.hpp"
#include "game/game_manager.hpp"
#include "redis/redis_data_manager.hpp"
#include "redis/role_data_manager.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<op_cmd::ND_BEGIN, op_cmd::ND_END, account_msg_handler_t::msg_handler_func> account_msg_handler_t::m_account_msg_handle;

bool account_msg_handler_t::init_msg_handler()
{
	m_account_msg_handle.register_func(op_cmd::nd_create_role_reply, handle_nd_create_role_reply);
	return true;
}

bool account_msg_handler_t::handle_nd_create_role_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::nd_create_role_reply);

	if (0 == msg.reply_code()) {
        ROLE_LOG("create role[%lu] success!", uid);
		role_data_manager::create_new_role(uid, msg.role_data());
	} else {
		role_data_manager::del_temp_name(msg.role_data().name());
	}

	proto::server::de_create_role_reply ret;
	ret.set_uid(uid);
	ret.set_reply_code(msg.reply_code());
	ret.set_gate_id(msg.gate_id());
	ret.set_name(msg.role_data().name());
	ret.set_create_time(msg.role_data().create_time());
	ret.set_role_type(msg.role_data().species());
	ret.set_channel_id(msg.role_data().channel_id());
	env::server->send_msg_to_center(op_cmd::de_create_role_reply, uid, ret);

	return true;
}
