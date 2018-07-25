#include "tower_msg_handle.hpp"
#include "common/log_enum.hpp"
#include "tower_manager.hpp"
#include "role/role_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "config/config_manager.hpp"


USING_NS_NETWORK;
USING_NS_COMMON;

bool tower_msg_handle_t::init_msg_handler()
{
	bool bresult = true;
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_tower_data_request, handle_cg_tower_data_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_tower_fight_request, handle_cg_tower_fight_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_tower_auto_fight_request, handle_cg_tower_auto_fight_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_tower_auto_fight_immediately_request, handle_cg_tower_auto_fight_immediately_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_tower_auto_fight_reward_request, handle_cg_tower_auto_fight_reward_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_tower_reset_request, handle_cg_tower_reset_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_tower_achieve_reward_request, handle_cg_tower_achieve_reward_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_tower_trigger_fight_request, handle_cg_tower_trigger_fight_request);
	return bresult;
}

bool tower_msg_handle_t::handle_cg_tower_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_tower_data_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%llu]", uid);
		return false;
	}
	tower_ptr p_tower = p_role->get_tower();
	if (NULL == p_tower)
	{
		log_error("NULL == p_tower[%llu]", uid);
		return false;
	}
	proto::client::gc_tower_data_reply reply;
	p_tower->peek_data(reply.mutable_tower_info());
	p_role->send_msg_to_client(op_cmd::gc_tower_data_reply, reply);
	return true;
}

bool tower_msg_handle_t::handle_cg_tower_fight_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_tower_fight_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%llu]", uid);
		return false;
	}
	uint32_t reply_code = tower_manager_t::fight(p_role);
	if (reply_code != errcode_enum::error_ok)
	{
		proto::client::gc_tower_fight_reply reply;
		reply.set_reply_code(reply_code);
		p_role->send_msg_to_client(op_cmd::gc_tower_fight_reply, reply);
	}
	return true;
}

bool tower_msg_handle_t::handle_cg_tower_auto_fight_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_tower_auto_fight_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%llu]", uid);
		return false;
	}
	proto::client::gc_tower_auto_fight_reply reply;
	uint32_t auto_fight_time = 0;
	uint32_t reply_code = tower_manager_t::auto_fight(p_role, auto_fight_time);
	if (reply_code == errcode_enum::error_ok)
	{
		reply.set_auto_fight_time(auto_fight_time);
	}
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_tower_auto_fight_reply, reply);
	return true;
}

bool tower_msg_handle_t::handle_cg_tower_auto_fight_immediately_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_tower_auto_fight_immediately_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%llu]", uid);
		return false;
	}
	proto::client::gc_tower_auto_fight_immediately_reply reply;
	uint32_t auto_fight_time = 0;
	uint32_t reply_code = tower_manager_t::auto_fight_immediately(p_role, auto_fight_time, reply.mutable_rcd());
	if (reply_code == errcode_enum::error_ok)
	{
		reply.set_auto_fight_time(auto_fight_time);
	}
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_tower_auto_fight_immediately_reply, reply);
	return true;
}

bool tower_msg_handle_t::handle_cg_tower_auto_fight_reward_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_tower_auto_fight_reward_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%llu]", uid);
		return false;
	}
	proto::client::gc_tower_auto_fight_reward_reply reply;
	uint32_t reply_code = tower_manager_t::auto_fight_reward(p_role, reply.mutable_tower_info(), reply.mutable_rcd());
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_tower_auto_fight_reward_reply, reply);
	return true;
}

bool tower_msg_handle_t::handle_cg_tower_reset_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_tower_reset_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%llu]", uid);
		return false;
	}
	proto::client::gc_tower_reset_reply reply;
	uint32_t current_tower = 0;
	uint32_t reset_count = 0;
	uint32_t reply_code = tower_manager_t::reset(p_role, current_tower, reset_count);
	if (reply_code == errcode_enum::error_ok)
	{
		reply.set_current_tower(current_tower);
		reply.set_reset_count(reset_count);
	}
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_tower_reset_reply, reply);
	return true;
}

bool tower_msg_handle_t::handle_cg_tower_achieve_reward_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_tower_achieve_reward_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%llu]", uid);
		return false;
	}
	proto::client::gc_tower_achieve_reward_reply reply;
	uint32_t current_achieve = 0;
	uint32_t reply_code = tower_manager_t::achieve_reward(p_role, current_achieve, reply.mutable_rcd());
	if (reply_code == errcode_enum::error_ok)
	{
		reply.set_current_achieve(current_achieve);
	}
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_tower_achieve_reward_reply, reply);
	return true;
}

bool tower_msg_handle_t::handle_cg_tower_trigger_fight_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_tower_trigger_fight_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%llu]", uid);
		return false;
	}
	uint64_t trigger_uid = string_util_t::string_to_uint64(msg.trigger_uid());
	uint32_t reply_code = tower_manager_t::fight_trigger(p_role, trigger_uid);
	if (reply_code != errcode_enum::error_ok)
	{
		proto::client::gc_tower_trigger_fight_reply reply;
		reply.set_reply_code(reply_code);
		p_role->send_msg_to_client(op_cmd::gc_tower_trigger_fight_reply, reply);
	}
	return true;
}
