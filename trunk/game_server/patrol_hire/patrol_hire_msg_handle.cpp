#include "patrol_hire_msg_handle.hpp"
#include "role/role_manager.hpp"
#include "gate/gate_msg_handler.hpp"
#include "patrol_hire_manager.hpp"

bool patrol_hire_msg_handle_t::init_msg_handler()
{
	bool bresult = true;
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_patrol_hire_info_request, handle_cg_patrol_hire_info_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_get_patrol_hire_rwd_request, handle_cg_get_patrol_hire_rwd_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_fast_patrol_hire_request, handle_cg_fast_patrol_hire_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_change_patrol_hire_request, handle_cg_change_patrol_hire_request);
	return bresult;
}

bool patrol_hire_msg_handle_t::handle_cg_patrol_hire_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_patrol_hire_info_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role) {
		log_error("p_role == NULL");
		return false;
	}

	patrol_hire_ptr p_patrol_hire = p_role->get_patrol_hire();
	if (!p_patrol_hire) {
		log_error("role[%lu] patrol_hire == NULL", uid);
		return false;
	}

	proto::client::gc_patrol_hire_info_reply reply;
	p_patrol_hire->peek_data(reply.mutable_info());
	p_role->send_msg_to_client(op_cmd::gc_patrol_hire_info_reply, reply);
	
	return true;
}

bool patrol_hire_msg_handle_t::handle_cg_get_patrol_hire_rwd_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_get_patrol_hire_rwd_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role) {
		log_error("p_role == NULL");
		return false;
	}

	proto::client::gc_get_patrol_hire_rwd_reply reply;
	uint32_t back_code = patrol_hire_manager_t::get_patrol_hire_reward(p_role, reply.mutable_reward());
	reply.set_reply_code(back_code);
	const patrol_hire_ptr& p_patrol_hire = p_role->get_patrol_hire();
	if (p_patrol_hire) {
		p_patrol_hire->peek_data(reply.mutable_info());
	}
	p_role->send_msg_to_client(op_cmd::gc_get_patrol_hire_rwd_reply, reply);

	return true;
}

bool patrol_hire_msg_handle_t::handle_cg_fast_patrol_hire_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_fast_patrol_hire_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role) {
		log_error("p_role == NULL");
		return false;
	}

	proto::client::gc_fast_patrol_hire_reply reply;
	patrol_hire_manager_t::fast_patrol_hire(p_role, msg.map_id(), reply);
	p_role->send_msg_to_client(op_cmd::gc_fast_patrol_hire_reply, reply);

	return true;
	
}

bool patrol_hire_msg_handle_t::handle_cg_change_patrol_hire_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_change_patrol_hire_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role) {
		log_error("p_role == NULL");
		return false;
	}

	proto::client::gc_change_patrol_hire_reply reply;
	patrol_hire_manager_t::change_patrol_hire_map(p_role, msg.map_id(), reply);
	p_role->send_msg_to_client(op_cmd::gc_change_patrol_hire_reply, reply);

	return true;
}

