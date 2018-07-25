#include "user_troop_msg_handler.hpp"
#include "user_troop.hpp"
#include "config_mgr.h"
#include "gate/gate_msg_handler.hpp"
#include "game/game_msg_handler.hpp"
#include "tblh/Comprehensive.tbls.h"
#include "common.pb.h"
#include "client.pb.h"
#include "user_troop_mgr.hpp"
#include "tblh/errcode_enum.hpp"

bool user_troop_msg_handle_t::init_msg_handler()
{
	bool bresult = true;

	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_troop_kick_request,handle_ce_troop_kick_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_troop_apply_member_request,handle_ce_troop_apply_member_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_troop_process_apply_request,handle_ce_troop_process_apply_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_troop_invite_join_request,handle_ce_troop_invite_join_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_troop_invite_single_request, handle_ce_troop_invite_single_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_troop_auto_join_flag_request,handle_ce_troop_auto_join_flag_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_troop_quit_request, handle_ce_troop_quit_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_troop_leave_request,handle_ce_troop_leave_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_troop_list_request,handle_ce_troop_list_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_troop_modify_request,handle_ce_troop_modify_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_troop_transfer_leader_request,handle_ce_troop_transfer_leader_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_troop_invite_shadow_request, handle_ce_troop_invite_shadow_request);

	return bresult;
}


bool user_troop_msg_handle_t::init_game_msg_handler()
{
	bool bresult = true;

	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_troop_create_request, handle_ge_troop_create_request);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_troop_auto_match_request, handle_ge_troop_auto_match_request);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_troop_join_request, handle_ge_troop_join_request);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_troop_agree_apply_reply, handle_ge_troop_agree_apply_reply);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_troop_process_invite_request, handle_ge_troop_process_invite_request);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_troop_dissolve_notify, handle_ge_troop_dissolve_notify);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_troop_back_request, handle_ge_troop_back_request);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_troop_transmission_notify, handle_ge_troop_transmission_notify);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_troop_sync_task_request, handle_ge_troop_sync_task_request);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_troop_escape_notify, handle_ge_troop_escape_notify);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_troop_del_shadow_notify, handle_ge_troop_del_shadow_notify);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_troop_friend_notify, handle_ge_troop_friend_notify);

	return bresult;
}

bool user_troop_msg_handle_t::handle_ce_troop_kick_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_troop_kick_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if(NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_troop_kick_reply reply;
	uint32_t reply_code = global_troop_manager::kick_troop(p_user, common::string_util_t::string_to_uint64(msg.role_id()));
	reply.set_reply_code(reply_code);
	p_user->send_msg_to_client(op_cmd::ec_troop_kick_reply, reply);

	return true;
}

bool user_troop_msg_handle_t::handle_ce_troop_apply_member_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_troop_apply_member_request);
	
	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}
	
	troop_ptr p_troop = global_troop_manager::get_troop(p_user->get_troop_id());
	if (NULL == p_troop) {
		log_error("p_user[%lu] NULL == p_troop[%lu]", p_user->get_uid(), p_user->get_troop_id());
		return false;
	}

	proto::client::ec_troop_apply_member_reply reply;
	p_troop->peek_apply_member_data(reply);
	p_user->send_msg_to_client(op_cmd::ec_troop_apply_member_reply, reply);

	return true;
}


bool user_troop_msg_handle_t::handle_ce_troop_process_apply_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_troop_process_apply_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	uint32_t reply_code = global_troop_manager::process_apply(p_user, common::string_util_t::string_to_uint64(msg.apply_member_id()), msg.process());
	if (reply_code != common::errcode_enum::error_ok || msg.process() != 0) {
		proto::client::ec_troop_process_apply_reply reply;
		reply.set_reply_code(reply_code);
		p_user->send_msg_to_client(op_cmd::ec_troop_process_apply_reply, reply);
	}

	return true;
}

bool user_troop_msg_handle_t::handle_ce_troop_invite_join_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_troop_invite_join_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	global_troop_manager::troop_invite_join(p_user, msg);

	return true;
}


bool user_troop_msg_handle_t::handle_ce_troop_invite_single_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_troop_invite_single_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	uint32_t invite_code = global_troop_manager::troop_invite_member(p_user, common::string_util_t::string_to_uint64(msg.role_id()));
	if (invite_code != common::errcode_enum::error_ok) {
		proto::client::ec_troop_invite_join_reply reply;
		reply.set_reply_code(invite_code);
		p_user->send_msg_to_client(op_cmd::ec_troop_invite_join_reply, reply);
	}

	return true;
}

bool user_troop_msg_handle_t::handle_ce_troop_auto_join_flag_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_troop_auto_join_flag_request);
	
	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	troop_ptr p_troop = global_troop_manager::get_troop(p_user->get_troop_id());
	if (NULL == p_troop) {
		log_error("p_user[%lu] NULL == p_troop[%lu]", p_user->get_uid(), p_user->get_troop_id());
		return false;
	}
	
	proto::client::ec_troop_auto_join_flag_reply reply;
	uint32_t reply_code = p_troop->toggle_join_flag(uid);
	reply.set_reply_code(reply_code);
	p_user->send_msg_to_client(op_cmd::ec_troop_auto_join_flag_reply, reply);
	return true;
}

bool user_troop_msg_handle_t::handle_ce_troop_quit_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_troop_quit_request);
	
	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_troop_quit_reply reply;
	uint32_t reply_code = global_troop_manager::quit_troop(p_user);
	reply.set_reply_code(reply_code);
	p_user->send_msg_to_client(op_cmd::ec_troop_quit_reply, reply);
	return true;
}


bool user_troop_msg_handle_t::handle_ce_troop_leave_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_troop_leave_request);
	
	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	troop_ptr p_troop = global_troop_manager::get_troop(p_user->get_troop_id());
	if (NULL == p_troop) {
		log_error("p_user[%lu] NULL == p_troop[%lu]", p_user->get_uid(), p_user->get_troop_id());
		return false;
	}

	uint32_t back_code = p_troop->set_leave_troop(p_user);
	if (back_code != common::errcode_enum::error_ok) {
		proto::client::ec_troop_leave_reply reply;
		reply.set_reply_code(back_code);
		p_user->send_msg_to_client(op_cmd::ec_troop_leave_reply, reply);
	}

	return true;
}

bool user_troop_msg_handle_t::handle_ce_troop_list_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_troop_list_request);
	
	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_troop_list_reply reply;
	global_troop_manager::get_troop_list(p_user, msg.type(), reply);
	p_user->send_msg_to_client(op_cmd::ec_troop_list_reply, reply);

	return true;
}

bool user_troop_msg_handle_t::handle_ce_troop_modify_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_troop_modify_request);
	
	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_troop_modify_reply reply;
	uint32_t reply_code = global_troop_manager::modify_troop(p_user, msg.type(), msg.level_min(), msg.level_max());
	reply.set_reply_code(reply_code);
	p_user->send_msg_to_client(op_cmd::ec_troop_modify_reply, reply);

	return true;
}


bool user_troop_msg_handle_t::handle_ce_troop_transfer_leader_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_troop_transfer_leader_request);
	
	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	troop_ptr p_troop = global_troop_manager::get_troop(p_user->get_troop_id());
	if (NULL == p_troop) {
		log_error("p_user[%lu] NULL == p_troop[%lu]", p_user->get_uid(), p_user->get_troop_id());
		return false;
	}

	proto::client::ec_troop_transfer_leader_reply reply;
	uint32_t reply_code = p_troop->transfer_leader(p_user, common::string_util_t::string_to_uint64(msg.role_id()));
	reply.set_reply_code(reply_code);
	p_user->send_msg_to_client(op_cmd::ec_troop_transfer_leader_reply, reply);

    return true;
}


bool user_troop_msg_handle_t::handle_ce_troop_invite_shadow_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_troop_invite_shadow_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	uint32_t invite_code = global_troop_manager::troop_invite_shadow(p_user, common::string_util_t::string_to_uint64(msg.friend_uid()));
	if (invite_code != common::errcode_enum::error_ok) {
		proto::client::ec_troop_invite_shadow_reply reply;
		reply.set_reply_code(invite_code);
		p_user->send_msg_to_client(op_cmd::ec_troop_invite_shadow_reply, reply);
	}

	return true;
}

bool user_troop_msg_handle_t::handle_ge_troop_create_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_troop_create_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_troop_create_reply reply;
	uint32_t reply_code = global_troop_manager::create_new_troop(p_user, msg.type());
	reply.set_reply_code(reply_code);
	p_user->send_msg_to_client(op_cmd::ec_troop_create_reply, reply);

	if (msg.has_role_uid()) {
		uint32_t invite_code = global_troop_manager::troop_invite_member(p_user, msg.role_uid());
		if (invite_code != common::errcode_enum::error_ok) {
			proto::client::ec_troop_invite_join_reply reply;
			reply.set_reply_code(invite_code);
			p_user->send_msg_to_client(op_cmd::ec_troop_invite_join_reply, reply);
		}
	}

	return true;
}

bool user_troop_msg_handle_t::handle_ge_troop_auto_match_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_troop_auto_match_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	global_troop_manager::auto_match(p_user, msg.type());

	return true;
}

bool user_troop_msg_handle_t::handle_ge_troop_join_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_troop_join_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_troop_join_reply reply;
	reply.set_troop_id(common::string_util_t::uint64_to_string(msg.troop_id()));
	uint32_t reply_code = global_troop_manager::join_troop(p_user, msg.troop_id());
	reply.set_reply_code(reply_code);
	p_user->send_msg_to_client(op_cmd::ec_troop_join_reply, reply);

	return true;
}

bool user_troop_msg_handle_t::handle_ge_troop_agree_apply_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_troop_agree_apply_reply);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_troop_process_apply_reply reply;
	if (msg.reply_code() != common::errcode_enum::error_ok) {
		reply.set_reply_code(msg.reply_code());
	} else {
		uint32_t back_code = global_troop_manager::check_agree_success(p_user, msg.apply_uid());
		reply.set_reply_code(back_code);
	}
	p_user->send_msg_to_client(op_cmd::ec_troop_process_apply_reply, reply);

	return true;
}

bool user_troop_msg_handle_t::handle_ge_troop_process_invite_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_troop_process_invite_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_troop_process_invite_reply reply; 
	troop_ptr p_troop = global_troop_manager::get_troop(msg.troop_id());
	if (NULL == p_troop) {
		reply.set_reply_code(common::errcode_enum::user_troop_err_code_not_exsit);
	} else {
		if (msg.game_code() != common::errcode_enum::error_ok) {
			// 不从邀请列表删除
			// p_troop->del_invite(uid);
			reply.set_reply_code(msg.game_code());
		} else {
			uint32_t reply_code = p_troop->process_invite(p_user, msg.process());
			reply.set_reply_code(reply_code);
		}
	}
	p_user->send_msg_to_client(op_cmd::ec_troop_process_invite_reply, reply);

	return true;
}

bool user_troop_msg_handle_t::handle_ge_troop_dissolve_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_troop_dissolve_notify);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	global_troop_manager::dissolve_troop(p_user);

	return true;
}

bool user_troop_msg_handle_t::handle_ge_troop_back_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_troop_back_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	troop_ptr p_troop = global_troop_manager::get_troop(p_user->get_troop_id());
	if (NULL == p_troop) {
		log_error("role[%lu] NULL == p_troop[%lu]", p_user->get_uid(), p_user->get_troop_id());
		return false;
	}

	uint32_t back_code = p_troop->back_troop(p_user);
	if (back_code != common::errcode_enum::error_ok) {
		proto::client::ec_troop_leave_reply reply;
		reply.set_reply_code(back_code);
		p_user->send_msg_to_client(op_cmd::ec_troop_leave_reply, reply);
	}

	return true;
}

bool user_troop_msg_handle_t::handle_ge_troop_transmission_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_troop_transmission_notify);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	troop_ptr p_troop = global_troop_manager::get_troop(p_user->get_troop_id());
	if (NULL == p_troop) {
		log_error("role[%lu] NULL == p_troop[%lu]", p_user->get_uid(), p_user->get_troop_id());
		return false;
	}

	p_troop->do_transmission_result(uid, msg.result());

	return true;
}


bool user_troop_msg_handle_t::handle_ge_troop_sync_task_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_troop_sync_task_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	troop_ptr p_troop = global_troop_manager::get_troop(p_user->get_troop_id());
	if (NULL == p_troop) {
		log_error("role[%lu] NULL == p_troop[%lu]", p_user->get_uid(), p_user->get_troop_id());
		return false;
	}

	p_troop->sync_task_info( p_user, msg.task_info() );

	return true;
}

bool user_troop_msg_handle_t::handle_ge_troop_escape_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_troop_escape_notify);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	troop_ptr p_troop = global_troop_manager::get_troop(p_user->get_troop_id());
	if (NULL == p_troop) {
		log_error("role[%lu] NULL == p_troop[%lu]", p_user->get_uid(), p_user->get_troop_id());
		return false;
	}

	uint32_t back_code = p_troop->set_leave_troop(p_user);
	if (back_code != common::errcode_enum::error_ok) {
		proto::client::ec_troop_leave_reply reply;
		reply.set_reply_code(back_code);
		p_user->send_msg_to_client(op_cmd::ec_troop_leave_reply, reply);
	}

	return true;
}

bool user_troop_msg_handle_t::handle_ge_troop_del_shadow_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_troop_del_shadow_notify);

	troop_ptr p_troop = global_troop_manager::get_troop(msg.troop_id());
	if (NULL == p_troop) {
		log_error("NULL == p_troop[%lu]", msg.troop_id());
		return false;
	}

	p_troop->remove_shadow_member(msg.member_id());
	return true;
}

bool user_troop_msg_handle_t::handle_ge_troop_friend_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_troop_friend_notify);

	troop_ptr p_troop = global_troop_manager::get_troop(msg.troop_id());
	if (NULL == p_troop) 
	{
		log_error("NULL == p_troop[%lu]", msg.troop_id());
		return false;
	}

	p_troop->friend_notify();
	return true;
}
