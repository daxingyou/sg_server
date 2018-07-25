#include "arena_msg_handler.hpp"
#include "gate/gate_msg_handler.hpp"
#include "arena/arena_manager.hpp"
#include "tblh/errcode_enum.hpp"
USING_NS_COMMON;


bool arena_msg_handler_t::init_msg_handler()
{
	bool bresult = true;
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_enter_arena_request, handle_cg_enter_arena_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_arena_join_request, handle_cg_arena_join_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_arena_buy_ticket_request, handle_cg_arena_buy_ticket_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_arena_match_request, handle_cg_arena_match_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_get_arena_rank_data_request, handle_cg_get_arena_rank_data_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_arena_cancel_match_request, handle_cg_arena_cancel_match_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_arena_self_rank_request, handle_cg_arena_self_rank_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_leave_arena_request, handle_cg_leave_arena_request);
	return bresult;
}

bool arena_msg_handler_t::handle_cg_enter_arena_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_enter_arena_request);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role NULL error");
		return false;
	}

	uint32_t back_code = arena_manager_t::enter_arena(p_role);
	if (back_code != common::errcode_enum::error_ok)
	{
		proto::client::gc_enter_arena_reply reply;
		reply.set_reply_code(back_code);
		p_role->send_msg_to_client(op_cmd::gc_enter_arena_reply, reply);
	}

	return true;
}

bool arena_msg_handler_t::handle_cg_arena_join_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_arena_join_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("arena_msg p_role NULL");
		return false;
	}

	proto::client::gc_arena_join_reply reply;
	uint32_t reply_code = arena_manager_t::join_arena(p_role, reply.mutable_rcd());
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_arena_join_reply, reply);

	return true;
}

bool arena_msg_handler_t::handle_cg_arena_buy_ticket_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_arena_buy_ticket_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("arena_msg p_role NULL");
		return false;
	}

	proto::client::gc_arena_buy_ticket_reply reply;

	uint32_t reply_code = arena_manager_t::buy_ticket(p_role, reply.mutable_rcd());

	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_arena_buy_ticket_reply, reply);

	return true;
}

bool arena_msg_handler_t::handle_cg_arena_match_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_arena_match_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("arena_msg p_role NULL");
		return false;
	}

	uint32_t reply_code = arena_manager_t::match_rank_for_battle(p_role);
	if (reply_code != errcode_enum::error_ok)
	{
		proto::client::gc_arena_match_reply reply;
		reply.set_reply_code(reply_code);
		p_role->send_msg_to_client(op_cmd::gc_arena_match_reply, reply);
	}

	return true;
}

bool arena_msg_handler_t::handle_cg_get_arena_rank_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_get_arena_rank_data_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("arena_msg p_role NULL");
		return false;
	}

	proto::server::gr_get_arena_rank_data_request msg_req;
	msg_req.set_rank_start_pos(msg.rank_start_pos());
	msg_req.set_server_id(p_role->get_server_id());
	env::server->send_msg_to_area(op_cmd::gr_get_arena_rank_data_request, uid, msg_req);

	return true;
}

bool arena_msg_handler_t::handle_cg_arena_cancel_match_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_arena_cancel_match_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("arena_msg p_role NULL");
		return false;
	}
	proto::server::gr_arena_cancel_match_request req;
	env::server->send_msg_to_area(op_cmd::gr_arena_cancel_match_request, uid, req);

	return true;
}

bool arena_msg_handler_t::handle_cg_arena_self_rank_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_arena_self_rank_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("arena_msg p_role NULL");
		return false;
	}
	proto::server::gr_arena_self_rank_request req;
	req.set_server_id(p_role->get_server_id());
	env::server->send_msg_to_area(op_cmd::gr_arena_self_rank_request, uid, req);

	return true;
}

bool arena_msg_handler_t::handle_cg_leave_arena_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_leave_arena_request);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role) {
		log_error("p_role NULL error");
		return false;
	}

	arena_manager_t::leave_arena(p_role);

	return true;
}
