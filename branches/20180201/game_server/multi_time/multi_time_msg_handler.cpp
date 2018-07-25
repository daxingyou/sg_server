#include "multi_time_msg_handler.hpp"
#include "multi_time_manager.hpp"
#include "gate/gate_msg_handler.hpp"
bool multi_time_msg_handler_t::init_msg_handler()
{
	bool bresult = true;
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_change_multi_state_request, handle_cg_change_multi_state_request);
	return bresult;
}

bool multi_time_msg_handler_t::handle_cg_change_multi_state_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_change_multi_state_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role role[%lu]", uid);
		return false;
	}
	proto::client::gc_change_multi_state_reply reply;
	uint32_t reply_code = multi_time_manager_t::change_multi_state(uid,msg.open_or_close_type(),reply.mutable_change_data());
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_change_multi_state_reply, reply);
	return true;
}

