#include "recharge_msg_handle.hpp"
#include "common/log_enum.hpp"
#include "role/role_manager.hpp"
#include "config/config_manager.hpp"
#include "errcode_enum.hpp"


USING_NS_NETWORK;
USING_NS_COMMON;

bool recharge_msg_handle_t::init_msg_handler()
{
	bool bresult = true;
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_recharge_reward_request, handle_cg_recharge_reward_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_recharge_request, handle_cg_recharge_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_recharge_cheap_request, handle_cg_recharge_cheap_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_recharge_cheap_buy_request, handle_cg_recharge_cheap_buy_request);
	return bresult;
}

bool recharge_msg_handle_t::handle_cg_recharge_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_recharge_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}
	recharge_ptr p_recharge = p_role->get_recharge();
	if (NULL == p_recharge)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}
	proto::client::gc_recharge_reply reply;
	proto::common::role_change_data* p_data = reply.mutable_rcd();
	uint32_t reply_code = p_recharge->rmb_recharge(msg.tid(), p_data);
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_recharge_reply, reply);
	return true;
}

bool recharge_msg_handle_t::handle_cg_recharge_reward_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_recharge_reward_request);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}
	recharge_ptr p_recharge = p_role->get_recharge();
	if (NULL == p_recharge)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}
	proto::client::gc_recharge_reward_reply reply;
	uint32_t reply_code = p_recharge->get_recharge_gift(reply.mutable_rcd());
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_recharge_reward_reply, reply);
	return true;
}

bool recharge_msg_handle_t::handle_cg_recharge_cheap_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_recharge_cheap_request);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}
	recharge_ptr p_recharge = p_role->get_recharge();
	if (NULL == p_recharge)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}
	proto::client::gc_recharge_cheap_reply reply;
	p_recharge->get_recharge_cheap(reply);
	p_role->send_msg_to_client(op_cmd::gc_recharge_cheap_reply, reply);
	return true;
}

bool recharge_msg_handle_t::handle_cg_recharge_cheap_buy_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_recharge_cheap_buy_request);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}
	recharge_ptr p_recharge = p_role->get_recharge();
	if (NULL == p_recharge)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}
	proto::client::gc_recharge_cheap_buy_reply reply;
	uint32_t reply_code = p_recharge->cheap_buy(msg.cheap_id(), reply);
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_recharge_cheap_buy_reply, reply);
	return true;
}
