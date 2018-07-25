#include "family_skill_msg_handle.hpp"
#include "common/log_enum.hpp"
#include "family_skill_manager.hpp"
#include "role/role_manager.hpp"
#include "config/config_manager.hpp"
#include "errcode_enum.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

bool family_skill_msg_handle_t::init_msg_handler()
{
	bool bresult = true;
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_family_skill_data_request, handle_cg_family_skill_data_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_family_skill_levelup_request, handle_cg_family_skill_levelup_request);
	return bresult;
}

bool family_skill_msg_handle_t::handle_cg_family_skill_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_family_skill_data_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}
	family_skill_mgr_ptr p_family_skill_mgr = p_role->get_family_skill_mgr();
	if (NULL == p_family_skill_mgr)
	{
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}
	proto::client::gc_family_skill_data_reply reply;
	p_family_skill_mgr->peek_data(reply.mutable_skill_data());
	p_role->send_msg_to_client(op_cmd::gc_family_skill_data_reply, reply);
	return true;
}

bool family_skill_msg_handle_t::handle_cg_family_skill_levelup_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_family_skill_levelup_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}
	family_skill_mgr_ptr p_family_skill_mgr = p_role->get_family_skill_mgr();
	if (NULL == p_family_skill_mgr)
	{
		log_error("NULL == p_role[%lu]", uid);
		return false;
	}
	proto::client::gc_family_skill_levelup_reply reply;
	uint32_t reply_code = p_family_skill_mgr->levelup(msg.skill_id(), reply.mutable_skill(), reply.mutable_rcd());
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_family_skill_levelup_reply, reply);
	return true;
}
