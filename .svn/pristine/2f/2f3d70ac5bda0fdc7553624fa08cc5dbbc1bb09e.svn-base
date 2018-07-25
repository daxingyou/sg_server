#include "liandan_msg_handle.hpp"
#include "common/log_enum.hpp"
#include "liandan_manager.hpp"
#include "role/role_manager.hpp"
#include "config/config_manager.hpp"
#include "errcode_enum.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

bool liandan_msg_handle_t::init_msg_handler()
{
	bool bresult = true;
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_liandan_data_request, handle_cg_liandan_data_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_liandan_unlock_request, handle_cg_liandan_unlock_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_liandan_doing_request, handle_cg_liandan_doing_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_liandan_done_request, handle_cg_liandan_done_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_liandan_return_request, handle_cg_liandan_return_request);
	return bresult;
}

bool liandan_msg_handle_t::handle_cg_liandan_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_liandan_data_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}
	liandan_mgr_ptr p_liandan_mgr = p_role->get_liandan_mgr();
	if (NULL == p_liandan_mgr)
	{
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}
	proto::client::gc_liandan_data_reply reply;
	p_liandan_mgr->peek_data(reply.mutable_liandan_data());
	p_role->send_msg_to_client(op_cmd::gc_liandan_data_reply, reply);
	return true;
}

bool liandan_msg_handle_t::handle_cg_liandan_unlock_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_liandan_unlock_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}
	liandan_mgr_ptr p_liandan_mgr = p_role->get_liandan_mgr();
	if (NULL == p_liandan_mgr)
	{
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}
	proto::client::gc_liandan_unlock_reply reply;
	uint32_t reply_code = p_liandan_mgr->unlock(reply.mutable_rcd());
	if (errcode_enum::error_ok == reply_code)
	{
		p_liandan_mgr->peek_data(reply.mutable_liandan_data());
	}
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_liandan_unlock_reply, reply);
	return true;
}

bool liandan_msg_handle_t::handle_cg_liandan_doing_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_liandan_doing_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}
	liandan_mgr_ptr p_liandan_mgr = p_role->get_liandan_mgr();
	if (NULL == p_liandan_mgr)
	{
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}
	proto::client::gc_liandan_doing_reply reply;
	uint32_t reply_code = p_liandan_mgr->doing(msg.liandan_tid(), msg.count(), reply.mutable_rcd());
	if (errcode_enum::error_ok == reply_code)
	{
		p_liandan_mgr->peek_data(reply.mutable_liandan_data());
	}
	reply.set_reply_code(reply_code);
	reply.set_liandan_tid(msg.liandan_tid());
	p_role->send_msg_to_client(op_cmd::gc_liandan_doing_reply, reply);
	return true;
}

bool liandan_msg_handle_t::handle_cg_liandan_done_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_liandan_done_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}
	liandan_mgr_ptr p_liandan_mgr = p_role->get_liandan_mgr();
	if (NULL == p_liandan_mgr)
	{
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}
	proto::client::gc_liandan_done_reply reply;
	uint32_t reply_code = p_liandan_mgr->done(msg.liandan_index(), reply.mutable_rcd());
	if (errcode_enum::error_ok == reply_code)
	{
		p_liandan_mgr->peek_data(reply.mutable_liandan_data());
	}
	reply.set_reply_code(reply_code);
	reply.set_liandan_index(msg.liandan_index());
	p_role->send_msg_to_client(op_cmd::gc_liandan_done_reply, reply);
	return true;
}

bool liandan_msg_handle_t::handle_cg_liandan_return_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_liandan_return_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}
	liandan_mgr_ptr p_liandan_mgr = p_role->get_liandan_mgr();
	if (NULL == p_liandan_mgr)
	{
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}
	proto::client::gc_liandan_return_reply reply;
	uint32_t reply_code = p_liandan_mgr->return_back(msg.liandan_index(), reply.mutable_rcd());
	if (errcode_enum::error_ok == reply_code)
	{
		p_liandan_mgr->peek_data(reply.mutable_liandan_data());
	}
	reply.set_reply_code(reply_code);
	reply.set_liandan_index(msg.liandan_index());
	p_role->send_msg_to_client(op_cmd::gc_liandan_return_reply, reply);
	return true;
}
