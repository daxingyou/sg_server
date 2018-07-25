#include "office_msg_handle.hpp"
#include "common/log_enum.hpp"
#include "role/role_manager.hpp"
#include "config/config_manager.hpp"
#include "errcode_enum.hpp"


USING_NS_NETWORK;
USING_NS_COMMON;

bool office_msg_handle_t::init_msg_handler()
{
	bool bresult = true;
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_office_data_request, handle_cg_office_data_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_office_award_request, handle_cg_office_award_request);
	return bresult;
}

bool office_msg_handle_t::handle_cg_office_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_office_data_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}
	office_ptr p_office = p_role->get_office();
	if (NULL == p_office)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}
	proto::client::gc_office_data_reply reply;
	p_office->peek_client_data(reply.mutable_office_data());
	p_role->send_msg_to_client(op_cmd::gc_office_data_reply, reply);
	return true;
}

bool office_msg_handle_t::handle_cg_office_award_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_office_award_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}
	office_ptr p_office = p_role->get_office();
	if (NULL == p_office)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}
	proto::client::gc_office_award_reply reply;
	uint32_t reply_code = p_office->award(reply.mutable_rcd());
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_office_award_reply, reply);
	return true;
}
