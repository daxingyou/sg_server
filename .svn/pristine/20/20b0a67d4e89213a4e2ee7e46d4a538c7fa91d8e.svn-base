#include "pk_msg_handle.hpp"
#include "role/role_manager.hpp"
#include "pk_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "center/center_msg_handler.hpp"
#include "role/money_manager.hpp"
#include "scene/scene_manager.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

bool pk_msg_handle_t::init_msg_handler()
{
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_pk_request, handle_cg_pk_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_challenge_request, handle_cg_challenge_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_challenge_process_request, handle_cg_challenge_process_request);
	
	center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_challenge_reply, handle_eg_challenge_reply);
	center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_challenge_process_reply, handle_eg_challenge_process_reply);
	center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_challenge_fight_request, handle_eg_challenge_fight_request);
	center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_challenge_transfer_notify, handle_eg_challenge_transfer_notify);
	return true;
}

bool pk_msg_handle_t::handle_cg_pk_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_pk_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("role[%lu] NULL", uid);
		return false;
	}
	uint64_t target_uid = string_util_t::string_to_uint64(msg.role_uid());
	auto p_target = role_manager_t::find_role(target_uid);
	if (NULL == p_target)
	{
		log_error("p_target[%lu] NULL", target_uid);
		return false;
	}
	proto::client::gc_pk_reply reply;
	uint32_t reply_code = pk_manager_t::do_pk(p_role, p_target, reply.mutable_rcd());
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_pk_reply, reply);
	return true;
}

bool pk_msg_handle_t::handle_cg_challenge_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_challenge_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("role[%lu] NULL", uid);
		return false;
	}
	uint64_t target_uid = string_util_t::string_to_uint64(msg.target_uid());
	uint32_t reply_code = pk_manager_t::do_challenge(p_role, target_uid);
	if (reply_code != errcode_enum::error_ok)
	{
		proto::client::gc_challenge_reply reply;
		reply.set_reply_code(reply_code);
		p_role->send_msg_to_client(op_cmd::gc_challenge_reply, reply);
		return false;
	}
	return true;
}


bool pk_msg_handle_t::handle_cg_challenge_process_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_challenge_process_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("role[%lu] NULL", uid);
		return false;
	}
	
	uint32_t reply_code = pk_manager_t::process_challenge(p_role, msg.process());
	if (reply_code != errcode_enum::error_ok)
	{
		proto::client::gc_challenge_process_reply reply;
		reply.set_reply_code(reply_code);
		p_role->send_msg_to_client(op_cmd::gc_challenge_process_reply, reply);
		return false;
	}
	return true;
}

bool pk_msg_handle_t::handle_eg_challenge_reply(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_challenge_reply);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("role[%lu] NULL", uid);
		return false;
	}
	proto::client::gc_challenge_reply reply;
	reply.set_reply_code(msg.reply_code());
	
	if (msg.reply_code() == errcode_enum::error_ok)
	{
		proto::common::personal_info *p_info = reply.mutable_rcd()->mutable_per_info();
		p_role->get_personal_info(p_info);
	}
	else
	{
		money_manager_t::add_money(p_role, proto::common::MONEY_TYPE_YUANBAO, pk_manager_t::get_challenge_gold(), log_enum::source_type_role_challenge, 0);
		PK_LOG("role[%lu] challenge failed, return money", uid);
	}
	p_role->send_msg_to_client(op_cmd::gc_challenge_reply, reply);
	return true;
}

bool pk_msg_handle_t::handle_eg_challenge_process_reply(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_challenge_process_reply);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("role[%lu] NULL", uid);
		return false;
	}
	proto::client::gc_challenge_process_reply reply;
	reply.set_reply_code(msg.reply_code());

	if (msg.reply_code() == errcode_enum::error_ok)
	{
		proto::common::personal_info *p_info = reply.mutable_rcd()->mutable_per_info();
		p_role->get_personal_info(p_info);
	}
	else
	{
		money_manager_t::add_money(p_role, proto::common::MONEY_TYPE_YUANBAO, pk_manager_t::get_challenge_gold(), log_enum::source_type_role_challenge, 0);
		PK_LOG("role[%lu] challenge failed, return money", uid);
	}
	p_role->send_msg_to_client(op_cmd::gc_challenge_process_reply, reply);
	return true;
}

bool pk_msg_handle_t::handle_eg_challenge_fight_request(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_challenge_fight_request);
	pk_manager_t::challenge_fight(uid, msg.target_uid());
	return true;
}

bool pk_msg_handle_t::handle_eg_challenge_transfer_notify(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_challenge_transfer_notify);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("role[%lu] NULL", uid);
		return false;
	}
	if (p_role->is_in_king_city())
	{
		uint32_t challenge_point = pk_manager_t::get_challenge_point();
		//´«ËÍ
		
		scene_manager_t::role_jump_point(p_role, challenge_point);
	}
	return true;
}
