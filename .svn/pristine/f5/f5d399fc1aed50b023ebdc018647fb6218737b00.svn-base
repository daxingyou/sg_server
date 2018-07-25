#include "center_msg_handler.hpp"
#include "main/db_server.hpp"
#include "main/db_server_fwd.hpp"

#include "server.pb.h"
#include "tblh/errcode_enum.hpp"
#include "redis/role_data_manager.hpp"
#include "redis/redis_data_manager.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

server_msg_handler_t<op_cmd::ED_BEGIN, op_cmd::ED_END, center_msg_handler_t::msg_handler_func> center_msg_handler_t::m_center_msg_handler;

bool center_msg_handler_t::init_msg_handler()
{
    bool ret = true;

	ret &= m_center_msg_handler.register_func(op_cmd::ed_create_role_request, handle_ed_create_role_request);
	ret &= m_center_msg_handler.register_func(op_cmd::ed_role_leave_game_notify, handle_ed_role_leave_game_notify);
	ret &= m_center_msg_handler.register_func(op_cmd::ed_query_role_data_request, handle_ed_query_role_data_request);

    return true;
}

bool center_msg_handler_t::handle_ed_create_role_request(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ed_create_role_request);

	uint32_t reply_code = role_data_manager::check_new_role(uid, msg.gate_id(), msg.nickname(), msg.role_type(), common::time_util_t::now_time());
	if (reply_code != common::errcode_enum::error_ok)
	{
		proto::server::de_create_role_reply ret;
		ret.set_uid(uid);
		ret.set_reply_code(reply_code);
		ret.set_gate_id(msg.gate_id());
		env::server->send_msg_to_center(op_cmd::de_create_role_reply, uid, ret);

		log_error("user[%lu] gate[%u] create role error:%u", uid, msg.gate_id(), reply_code);
	}

	return true;
}

bool center_msg_handler_t::handle_ed_role_leave_game_notify(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ed_role_leave_game_notify);

	redis_data_manager_t::handle_syn_role_data(uid);

	return true;
}

bool center_msg_handler_t::handle_ed_query_role_data_request(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ed_query_role_data_request);

	if (!role_data_manager::query_role_data(uid, msg.gate_id()))
	{
		// 查询db有没这个人物信息，正常情况应该没有并返回这个消息通知创建角色
		proto::server::de_query_role_data_reply ret;
		ret.set_uid(uid);
		ret.set_gate_id(msg.gate_id());
		env::server->send_msg_to_center(op_cmd::de_query_role_data_reply, uid, ret);
	}

	return true;
}

