#include "friend_msg_handler.hpp"
#include "common.pb.h"
#include "server.pb.h"
#include "client.pb.h"
#include "gate/gate_msg_handler.hpp"
#include "center/center_msg_handler.hpp"
#include "role/role_manager.hpp"
#include "friend_manager.hpp"
#include "main/game_server_fwd.hpp"
#include "friendliness.h"

bool friend_msg_handle_t::init_center_msg_handler()
{
	bool bresult = true;

	bresult &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_friend_point_operation_request, handle_eg_friend_point_operation_request);
	bresult &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_get_friendliness_reply, handle_eg_get_friendliness_reply);
	bresult &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_change_friendliness_notify, handle_eg_friendliness_change);

	return bresult;
}

bool friend_msg_handle_t::init_gate_msg_handler()
{

	return true;
}

bool friend_msg_handle_t::handle_eg_friend_point_operation_request(const network::msg_buf_ptr & msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_friend_point_operation_request);

	proto::server::ge_friend_point_operation_reply reply;
	int32_t info1 = 0, info2 = 0, info4 = 0;
	reply.set_reply_code(friend_manager_t::handle_friend_point_operation(msg.role_id(), msg.operation(), msg.parameter1(), msg.parameter2(), reply.mutable_change_data(), info1, info2, reply.mutable_info3(), info4));
	reply.set_role_id(msg.role_id());
	reply.set_friend_role_id(msg.friend_role_id());
	reply.set_operation(msg.operation());
	reply.set_parameter1(msg.parameter1());
	reply.set_parameter2(msg.parameter2());
	reply.set_info1(info1);
	reply.set_info2(info2);
	reply.set_info4(info4);
	env::server->send_msg_to_center(op_cmd::ge_friend_point_operation_reply, 0, reply);

	return true;
}

bool friend_msg_handle_t::handle_eg_get_friendliness_reply(const network::msg_buf_ptr & msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_get_friendliness_reply);
	uint64_t role_id = msg.role_id();
	role_ptr p_role = role_manager_t::find_role(role_id);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", role_id);
		return false;
	}
	p_role->get_friendliness_data().on_get_friendliness_data_from_center(msg.friendliness());
	return true;
}

bool friend_msg_handle_t::handle_eg_friendliness_change(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_change_friendliness_notify);
	uint64_t role_id = msg.my_role_id();
	role_ptr p_role = role_manager_t::find_role(role_id);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", role_id);
		return false;
	}
	p_role->get_friendliness_data().change_friendliness(msg.friend_role_id(), msg.change_value(), msg.reason());
	return true;
}