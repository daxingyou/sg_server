#include "user_troop_msg_handler.hpp"
#include "user_troop.hpp"
#include "config_mgr.h"
#include "center/center_msg_handler.hpp"
#include "gate/gate_msg_handler.hpp"
#include "user_troop_mgr.hpp"
#include "server.pb.h"
#include "tblh/errcode_enum.hpp"
#include "scene/scene_manager.hpp"

bool user_troop_msg_handle_t::init_gate_msg_handler()
{
	bool ret = true;

	ret &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_troop_create_request, handle_cg_troop_create_request);
	ret &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_troop_join_request, handle_cg_troop_join_request);
	ret &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_troop_auto_match_request, handle_cg_troop_auto_match_request);
	ret &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_troop_process_invite_request, handle_cg_troop_process_invite_request);
	ret &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_troop_back_request, handle_cg_troop_back_request);

	return ret;
}

bool user_troop_msg_handle_t::init_center_msg_handler()
{
	bool ret = true;

	ret &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_troop_event_notify, user_troop_msg_handle_t::handle_eg_troop_event_notify);
	ret &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_troop_agree_apply_request, user_troop_msg_handle_t::handle_eg_troop_agree_apply_request);
	
	return ret;
}


bool user_troop_msg_handle_t::handle_cg_troop_create_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_troop_create_request);

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role) {
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	uint32_t back_code = game_troop_mgr_t::create_troop(p_role, msg);
	if (back_code != common::errcode_enum::error_ok) {
		proto::client::gc_troop_create_reply reply;
		reply.set_reply_code(back_code);
		p_role->send_msg_to_client(op_cmd::gc_troop_create_reply, reply);
	}

	return true;
}


bool user_troop_msg_handle_t::handle_cg_troop_join_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_troop_join_request);

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role) {
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	uint32_t back_code = game_troop_mgr_t::join_troop(p_role, common::string_util_t::string_to_uint64(msg.troop_id()));
	if (back_code != common::errcode_enum::error_ok) {
		proto::client::gc_troop_join_reply reply;
		reply.set_reply_code(back_code);
		p_role->send_msg_to_client(op_cmd::gc_troop_join_reply, reply);
	}

	return true;
}


bool user_troop_msg_handle_t::handle_cg_troop_auto_match_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_troop_auto_match_request);

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role) {
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	uint32_t back_code = game_troop_mgr_t::auto_match(p_role, msg.type());
	if (back_code != common::errcode_enum::error_ok) {
		proto::client::gc_troop_auto_match_reply reply;
		reply.set_reply_code(back_code);
		p_role->send_msg_to_client(op_cmd::gc_troop_auto_match_reply, reply);
	}

	return true;
}

bool user_troop_msg_handle_t::handle_cg_troop_process_invite_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_troop_process_invite_request);

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role) {
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	game_troop_mgr_t::process_invite(p_role, common::string_util_t::string_to_uint64(msg.troop_id()), msg.process());

	return true;
}


bool user_troop_msg_handle_t::handle_cg_troop_back_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_troop_back_request);

	const role_ptr& p_role = role_manager_t::find_role(uid);
	if (NULL == p_role) {
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	uint32_t back_code = game_troop_mgr_t::back_troop(p_role);
	if (back_code != common::errcode_enum::error_ok) {
		proto::client::gc_troop_back_reply reply;
		reply.set_reply_code(back_code);
		p_role->send_msg_to_client(op_cmd::gc_troop_back_reply, reply);
	}

	return true;
}

bool user_troop_msg_handle_t::handle_eg_troop_event_notify(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_troop_event_notify);

	game_troop_mgr_t::troop_event(uid, msg.event(), msg.member_id(), msg.mutable_troop());

	return true;
}

bool user_troop_msg_handle_t::handle_eg_troop_agree_apply_request(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_troop_agree_apply_request);

	uint32_t back_code = game_troop_mgr_t::check_process_apply(uid);
	proto::server::ge_troop_agree_apply_reply reply;
	reply.set_apply_uid(uid);
	reply.set_reply_code(back_code);
	env::server->send_msg_to_center(op_cmd::ge_troop_agree_apply_reply, msg.apply_uid(), reply);

	return true;
}
