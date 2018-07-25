#include "trade_msg_handler.hpp"
#include "gate/gate_msg_handler.hpp"
#include "role_trade_manager.hpp"
#include "city/city_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "trade/trade_manager.hpp"
USING_NS_NETWORK;
USING_NS_COMMON;


bool trade_msg_handler_t::init_msg_handler()
{
	bool bresult = true;
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_trade_buy_item_request, handle_cg_trade_buy_item_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_trade_sell_item_request, handle_cg_trade_sell_item_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_get_city_trade_shop_request, handle_cg_get_city_trade_shop_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_trade_supplement_request, handle_cg_trade_supplement_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_trade_event_request, handle_cg_trade_event_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_troop_trade_request, handle_cg_troop_trade_request);
	return bresult;
}

bool trade_msg_handler_t::handle_cg_trade_buy_item_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_trade_buy_item_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("trade_msg p_role NULL");
		return false;
	}
	proto::client::gc_trade_buy_item_reply reply;
	uint32_t reply_code = role_trade_manager_t::trade_buy_all_items(p_role, msg, reply);
	reply.set_reply_code(reply_code);
	reply.set_city_id(msg.city_id());
	//reply.set_buy_type(msg.buy_type());
	p_role->send_msg_to_client(op_cmd::gc_trade_buy_item_reply, reply);

	return true;
}

bool trade_msg_handler_t::handle_cg_trade_sell_item_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_trade_sell_item_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("trade_msg p_role NULL");
		return false;
	}
	proto::client::gc_trade_sell_item_reply reply;
	uint32_t reply_code = role_trade_manager_t::trade_sell_all_items(p_role, msg, reply);
	reply.set_reply_code(reply_code);
	reply.set_city_id(msg.city_id());
	p_role->send_msg_to_client(op_cmd::gc_trade_sell_item_reply, reply);

	return true;
}

bool trade_msg_handler_t::handle_cg_get_city_trade_shop_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_get_city_trade_shop_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("trade_msg p_role NULL");
		return false;
	}

	proto::client::gc_get_city_trade_shop_reply reply;
	uint32_t reply_code = errcode_enum::error_ok;

	reply_code = role_trade_manager_t::get_city_trade_shop_info(msg.city_id(), p_role, reply);

	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_get_city_trade_shop_reply, reply);

	return true;
}

bool trade_msg_handler_t::handle_cg_trade_supplement_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_trade_supplement_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("trade_msg p_role NULL");
		return false;
	}
	uint64_t rebuy_item_uid = string_util_t::string_to_uint64(msg.rebuy_item_uid());

	proto::client::gc_trade_supplement_reply reply;
	reply.set_city_id(msg.city_id());

	uint32_t reply_code = role_trade_manager_t::trade_supplement_item(p_role, msg.city_id(), rebuy_item_uid,
												reply.mutable_change_data(), reply.mutable_city_shop_info());

	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_trade_supplement_reply, reply);

	return true;
}

bool trade_msg_handler_t::handle_cg_trade_event_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_trade_event_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("trade_msg p_role NULL");
		return false;
	}
	
	proto::client::gc_trade_event_reply msg_reply;
	role_trade_manager_t::get_all_city_trade_event(msg_reply,msg.type());
	p_role->send_msg_to_client(op_cmd::gc_trade_event_reply, msg_reply);

	return true;
}

bool trade_msg_handler_t::handle_cg_troop_trade_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_troop_trade_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("trade_msg p_role NULL");
		return false;
	}
	proto::client::gc_troop_trade_notify notify;
	uint32_t reply_code =  trade_manager_t::troop_trade_view_notify(uid,msg.trade_id());
	notify.set_reply_code(reply_code);
	notify.set_trade_id(msg.trade_id());
	p_role->send_msg_to_client(op_cmd::gc_troop_trade_notify,notify);
	return true;
}
