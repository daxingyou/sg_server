#include "expedition_msg_handle.hpp"
#include "common/log_enum.hpp"
#include "role/role_manager.hpp"
#include "config/config_manager.hpp"
#include "center/center_msg_handler.hpp"
#include "errcode_enum.hpp"


USING_NS_NETWORK;
USING_NS_COMMON;

bool expedition_msg_handle_t::init_client_msg_handler()
{
	bool bresult = true;
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_expedition_data_request, handle_cg_expedition_data_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_expedition_fight_request, handle_cg_expedition_fight_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_expedition_refresh_request, handle_cg_expedition_refresh_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_expedition_help_request, handle_cg_expedition_help_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_expedition_shop_request, handle_cg_expedition_shop_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_expedition_award_request, handle_cg_expedition_award_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_expedition_box_request, handle_cg_expedition_box_request);
	return bresult;
}

bool expedition_msg_handle_t::init_center_msg_handler()
{
	bool bresult = true;
	bresult &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_expedition_fight_data_reply, handle_eg_expedition_fight_data_reply);
	bresult &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_expedition_refresh_reply, handle_eg_expedition_refresh_reply);
	bresult &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_expedition_help_reply, handle_eg_expedition_help_reply);
	return bresult;
}

bool expedition_msg_handle_t::handle_cg_expedition_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_expedition_data_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%llu]", uid);
		return false;
	}
	if (p_role->get_family_id() == 0)
	{
		log_error("p_role[%lu] not in family", uid);
		return false;
	}
	expedition_ptr p_expedition = p_role->get_expedition();
	if (NULL == p_expedition)
	{
		log_error("NULL == p_expedition[%llu]", uid);
		return false;
	}
	proto::client::gc_expedition_data_reply reply;
	p_expedition->peek_data(reply.mutable_expedition_data());
	p_role->send_msg_to_client(op_cmd::gc_expedition_data_reply, reply);
	return true;
}

bool expedition_msg_handle_t::handle_cg_expedition_fight_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_expedition_fight_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%llu]", uid);
		return false;
	}
	if (p_role->get_family_id() == 0)
	{
		log_error("p_role[%lu] not in family", uid);
		return false;
	}
	expedition_ptr p_expedition = p_role->get_expedition();
	if (NULL == p_expedition)
	{
		log_error("NULL == p_expedition[%llu]", uid);
		return false;
	}
	uint32_t reply_code = p_expedition->fight(msg.stage_id());
	if (errcode_enum::error_ok != reply_code)
	{
		proto::client::gc_expedition_fight_reply reply;
		reply.set_reply_code(reply_code);
		p_role->send_msg_to_client(op_cmd::gc_expedition_fight_reply, reply);
	}
	
	return true;
}

bool expedition_msg_handle_t::handle_cg_expedition_refresh_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_expedition_refresh_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%llu]", uid);
		return false;
	}
	if (p_role->get_family_id() == 0)
	{
		log_error("p_role[%lu] not in family", uid);
		return false;
	}
	expedition_ptr p_expedition = p_role->get_expedition();
	if (NULL == p_expedition)
	{
		log_error("NULL == p_expedition[%llu]", uid);
		return false;
	}
	uint32_t reply_code = p_expedition->refresh_request();
	if (reply_code != errcode_enum::error_ok)
	{
		proto::client::gc_expedition_refresh_reply reply;
		reply.set_reply_code(reply_code);
		p_role->send_msg_to_client(op_cmd::gc_expedition_refresh_reply, reply);
	}
	return true;
}

bool expedition_msg_handle_t::handle_cg_expedition_help_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_expedition_help_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%llu]", uid);
		return false;
	}
	if (p_role->get_family_id() == 0)
	{
		log_error("p_role[%lu] not in family", uid);
		return false;
	}
	expedition_ptr p_expedition = p_role->get_expedition();
	if (NULL == p_expedition)
	{
		log_error("NULL == p_expedition[%llu]", uid);
		return false;
	}
	std::vector<uint64_t> helper_list;
	for (int32_t i = 0; i < msg.helper_list_size(); ++i)
	{
		helper_list.push_back(string_util_t::string_to_uint64(msg.helper_list(i)));
	}
	uint32_t reply_code = p_expedition->help_request(helper_list);
	if (reply_code != errcode_enum::error_ok)
	{
		proto::client::gc_expedition_help_reply reply;
		reply.set_reply_code(reply_code);
		p_role->send_msg_to_client(op_cmd::gc_expedition_help_reply, reply);
	}
	return true;
}

bool expedition_msg_handle_t::handle_cg_expedition_award_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_expedition_award_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%llu]", uid);
		return false;
	}
	if (p_role->get_family_id() == 0)
	{
		log_error("p_role[%lu] not in family", uid);
		return false;
	}
	expedition_ptr p_expedition = p_role->get_expedition();
	if (NULL == p_expedition)
	{
		log_error("NULL == p_expedition[%llu]", uid);
		return false;
	}
	proto::client::gc_expedition_award_reply reply;
	uint32_t reply_code = p_expedition->award(msg.stage_id(), reply.mutable_rcd());
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_expedition_award_reply, reply);
	return true;
}

bool expedition_msg_handle_t::handle_cg_expedition_box_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_expedition_box_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%llu]", uid);
		return false;
	}
	if (p_role->get_family_id() == 0)
	{
		log_error("p_role[%lu] not in family", uid);
		return false;
	}
	expedition_ptr p_expedition = p_role->get_expedition();
	if (NULL == p_expedition)
	{
		log_error("NULL == p_expedition[%llu]", uid);
		return false;
	}
	proto::client::gc_expedition_box_reply reply;
	uint32_t reply_code = p_expedition->box(msg.box_id(), reply);
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_expedition_box_reply, reply);
	return true;
}

bool expedition_msg_handle_t::handle_cg_expedition_shop_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_expedition_shop_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("NULL == p_role[%llu]", uid);
		return false;
	}
	if (p_role->get_family_id() == 0)
	{
		log_error("p_role[%lu] not in family", uid);
		return false;
	}
	expedition_ptr p_expedition = p_role->get_expedition();
	if (NULL == p_expedition)
	{
		log_error("NULL == p_expedition[%llu]", uid);
		return false;
	}
	proto::client::gc_expedition_shop_reply reply;
	uint64_t hero_uid = string_util_t::string_to_uint64(msg.hero_uid());
	uint32_t reply_code = p_expedition->shop_request(msg.shop_id(), hero_uid, msg.buy_style(), reply);
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_expedition_shop_reply, reply);
	return true;
}

bool expedition_msg_handle_t::handle_eg_expedition_fight_data_reply(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_expedition_fight_data_reply);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}
	expedition_ptr p_expedition = p_role->get_expedition();
	if (NULL == p_expedition)
	{
		log_error("NULL == p_expedition[%llu]", uid);
		return false;
	}
	p_expedition->load_fight_data(msg);
	return true;
}

bool expedition_msg_handle_t::handle_eg_expedition_refresh_reply(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_expedition_refresh_reply);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}
	expedition_ptr p_expedition = p_role->get_expedition();
	if (NULL == p_expedition)
	{
		log_error("NULL == p_expedition[%llu]", uid);
		return false;
	}
	p_expedition->refresh_reply(msg);
	return true;
}

bool expedition_msg_handle_t::handle_eg_expedition_help_reply(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_expedition_help_reply);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}
	expedition_ptr p_expedition = p_role->get_expedition();
	if (NULL == p_expedition)
	{
		log_error("NULL == p_expedition[%llu]", uid);
		return false;
	}
	proto::client::gc_expedition_help_reply reply;
	reply.set_reply_code(msg.reply_code());
	p_role->send_msg_to_client(op_cmd::gc_expedition_help_reply, reply);
	return true;
}
