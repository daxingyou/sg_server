#include "collect_msg_handler.hpp"
#include "gate/gate_msg_handler.hpp"
#include "collect/collect_manager.hpp"
#include "tblh/errcode_enum.hpp"

USING_NS_COMMON;


bool collect_msg_handler_t::init_msg_handler()
{
	bool bresult = true;
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_start_collect_request, handle_cg_start_collect_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_end_collect_request, handle_cg_end_collect_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_cancel_collect_request, handle_cg_cancel_collect_request);
	return bresult;
}

bool collect_msg_handler_t::handle_cg_start_collect_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_start_collect_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("collect_msg p_role NULL");
		return false;
	}
	proto::client::gc_start_collect_reply reply;
	uint64_t collect_point_uid = string_util_t::string_to_uint64(msg.collect_point_uid());

	uint32_t reply_code = collect_manager_t::start_collect(p_role, collect_point_uid);
	reply.set_reply_code(reply_code);
	reply.set_collect_point_uid(msg.collect_point_uid());

	p_role->send_msg_to_client(op_cmd::gc_start_collect_reply, reply);

	return true;
}

bool collect_msg_handler_t::handle_cg_end_collect_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_end_collect_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("collect_msg p_role NULL");
		return false;
	}
	proto::client::gc_end_collect_reply reply;
	uint64_t collect_point_uid = string_util_t::string_to_uint64(msg.collect_point_uid());

	uint32_t reply_code = collect_manager_t::end_collect(p_role, collect_point_uid, reply.mutable_rcd());
	reply.set_reply_code(reply_code);
	reply.set_collect_point_uid(msg.collect_point_uid());

	p_role->send_msg_to_client(op_cmd::gc_end_collect_reply, reply);

	return true;
}

bool collect_msg_handler_t::handle_cg_cancel_collect_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_cancel_collect_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("collect_msg p_role NULL");
		return false;
	}

	proto::client::gc_cancel_collect_reply reply;
	uint64_t collect_point_uid = string_util_t::string_to_uint64(msg.collect_point_uid());

	uint32_t reply_code = collect_manager_t::cancel_collect(p_role, collect_point_uid);
	reply.set_reply_code(reply_code);
	reply.set_collect_point_uid(msg.collect_point_uid());

	p_role->send_msg_to_client(op_cmd::gc_cancel_collect_reply, reply);

	return true;
}
