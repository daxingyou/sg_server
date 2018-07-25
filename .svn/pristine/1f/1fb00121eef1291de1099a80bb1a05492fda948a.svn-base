#include "db_msg_handler.hpp"
#include "main/center_server.hpp"
#include "main/center_server_fwd.hpp"
#include "user/global_user_manager.hpp"
#include "network/network.hpp"
#include "network/msg_handler.hpp"
#include "user/global_user_manager.hpp"


USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<op_cmd::DE_BEGIN, op_cmd::DE_END, db_msg_handler_t::msg_handler_func> db_msg_handler_t::m_db_msg_handle;

bool db_msg_handler_t::init_msg_handler()
{
    bool ret = true;

    ret &= m_db_msg_handle.register_func(op_cmd::de_create_role_reply, handle_de_create_role_reply);
	ret &= m_db_msg_handle.register_func(op_cmd::de_query_role_data_reply, handle_de_query_role_data_reply);

    return ret;
}


bool db_msg_handler_t::handle_de_create_role_reply(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::de_create_role_reply);
	// db 创建成功
	if (0 == msg.reply_code())
	{
		global_user_manager_t::create_user_success( msg.gate_id(), uid, msg.name(), msg.create_time(), msg.role_type(), msg.channel_id() );
	}
	else
	{
		const network::tcp_socket_ptr& p_gate_socket = gate_manager_t::get_gate_socket_by_id(msg.gate_id());
		if (NULL == p_gate_socket)
		{
			log_error("user[%lu] NULL == p_gate_socket[%u]: send cmd[de_create_role_reply]", uid, msg.gate_id());
			return false;
		}

		proto::server::ea_create_role_reply ret;
		ret.set_uid(msg.uid());
		ret.set_reply_code(msg.reply_code());
		env::server->send_msg_to_gate(p_gate_socket, op_cmd::ea_create_role_reply, msg.uid(), ret);
	}

    return true;
}

bool db_msg_handler_t::handle_de_query_role_data_reply(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::de_query_role_data_reply);

	const network::tcp_socket_ptr& p_gate_socket = gate_manager_t::get_gate_socket_by_id(msg.gate_id());
	if (NULL == p_gate_socket)
	{
		log_error("user[%lu] NULL == p_gate_socket[%u]: send cmd[de_create_role_reply]", uid, msg.gate_id());
		return false;
	}

	ROLE_LOG("user[%lu] have no role, notify client create role", uid);

	proto::server::ea_unreg_user_center_notify ret;
	ret.set_uid(uid);
	ret.set_unreg_code(common::UNREGUSER_RET_NO_ROLE);
	env::server->send_msg_to_gate(p_gate_socket, op_cmd::ea_unreg_user_center_notify, uid, ret);

	return true;
}
