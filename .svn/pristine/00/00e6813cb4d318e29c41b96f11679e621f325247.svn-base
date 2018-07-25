#include "activity_msg_handle.hpp"
#include "role/role_manager.hpp"
#include "role/role.hpp"
#include "gate/gate_msg_handler.hpp"

bool activity_msg_handle_t::init_msg_handler()
{
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_activity_data_request, handle_cg_activity_data_request);

	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_liveness_box_request, handle_cg_liveness_box_request);
	return true;
}

bool activity_msg_handle_t::handle_cg_activity_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_activity_data_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}
	
	auto p_activity_mgr = p_role->get_activity_mgr();
	if (NULL == p_activity_mgr)
	{
		log_error("NULL == p_activity_mgr[%lu]", uid);
		return false;
	}
	proto::client::gc_activity_data_reply reply;
	p_activity_mgr->peek_data(reply.mutable_activities_data());
	p_role->send_msg_to_client(op_cmd::gc_activity_data_reply, reply);
	return true;
}

bool activity_msg_handle_t::handle_cg_liveness_box_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_liveness_box_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}

	auto p_activity_mgr = p_role->get_activity_mgr();
	if (NULL == p_activity_mgr)
	{
		log_error("NULL == p_activity_mgr[%lu]", uid);
		return false;
	}
	proto::client::gc_liveness_box_reply reply;
	uint32_t reply_code = 0;
	if (!p_activity_mgr->liveness_box(msg.box_id(), reply.mutable_rcd()))
	{
		reply_code = 1;
	}
	p_activity_mgr->peek_data(reply.mutable_activities_data());
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_liveness_box_reply, reply);
	return true;
}
