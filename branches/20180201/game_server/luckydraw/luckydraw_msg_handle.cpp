#include "luckydraw_msg_handle.hpp"
#include "common/log_enum.hpp"
#include "luckydraw_manager.hpp"
#include "role/role_manager.hpp"
#include "config/config_manager.hpp"
#include "errcode_enum.hpp"
#include "center/center_msg_handler.hpp"


USING_NS_NETWORK;
USING_NS_COMMON;

bool luckydraw_msg_handle_t::init_msg_handler()
{
	bool bresult = true;
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_luckydraw_request, handle_cg_luckydraw_request);
	return bresult;
}

bool luckydraw_msg_handle_t::init_center_msg_handler()
{
	bool bresult = true;
	bresult &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_buy_wish_summon_request, handle_eg_buy_wish_summon_request);
	bresult &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_king_open_wish_summon_request, handle_eg_king_open_wish_summon_request);
	return bresult;
}

bool luckydraw_msg_handle_t::handle_cg_luckydraw_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_luckydraw_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%llu]", uid);
		return false;
	}
	
	proto::client::gc_luckydraw_reply reply;
	uint32_t reply_code = luckydraw_manager_t::luckydraw(p_role, msg.type(), msg.count(), reply);
	reply.set_reply_code(reply_code);
	reply.set_type(msg.type());
	p_role->send_msg_to_client(op_cmd::gc_luckydraw_reply, reply);
	return true;
}


bool luckydraw_msg_handle_t::handle_eg_buy_wish_summon_request(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_buy_wish_summon_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%llu]", uid);
		return false;
	}
	
	proto::client::gc_buy_wish_summon_reply reply;
	if (msg.reply_code() != errcode_enum::error_ok)
	{
		reply.set_summon_id(msg.summon_id());
		reply.set_reply_code(msg.reply_code());
	}
	else
	{
		uint32_t reply_code = luckydraw_manager_t::buy_wish_summon(p_role, msg.summon_id(), reply);
		proto::common::role_change_data* p_rcd = reply.mutable_rcd();
		if (p_rcd) {
			p_role->get_personal_info(p_rcd->mutable_per_info());
		}
		reply.set_summon_id(msg.summon_id());
		reply.set_reply_code(reply_code);
	}
	p_role->send_msg_to_client(op_cmd::gc_buy_wish_summon_reply, reply);
	return true;
}

bool luckydraw_msg_handle_t::handle_eg_king_open_wish_summon_request(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_king_open_wish_summon_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%llu]", uid);
		return false;
	}

	if ( msg.reply_code() == errcode_enum::error_ok )
	{
		luckydraw_manager_t::open_king_wish_summon(p_role, msg.summon_id());
	}
	return true;
}
