#include "general_event_msg_handler.hpp"
#include "general_event_manager.hpp"
#include "general_info_manager.hpp"
#include "role/role_manager.hpp"


bool general_event_msg_handler_t::init_gate_msg_handler()
{
	bool result = true;

	result &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_get_general_event_info_request, handle_cg_get_general_event_info_request);
	result &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_get_general_event_detail_request, handle_cg_get_general_event_detail_request);
	result &=gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_get_general_event_prize_request, handle_cg_get_general_event_prize_request);
	result &=gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_get_server_level_limit_request, handle_cg_get_server_level_limit_request);

	return result;
}

bool general_event_msg_handler_t::init_center_msg_handler()
{
	bool bresult = true;

	bresult &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_sync_general_event_count_info, handler_eg_sync_general_event_count_info);
	bresult &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_sync_general_event_data, handler_eg_sync_general_event_data);
	bresult &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_sync_general_event_open, handler_eg_sync_general_event_open);
	bresult &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_sync_general_event_done, handler_eg_sync_general_event_done);
	bresult &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_sync_server_level, handle_eg_sync_server_level);
	return bresult;
}


bool general_event_msg_handler_t::handle_cg_get_general_event_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_get_general_event_info_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	proto::client::gc_get_general_event_info_reply  reply;
	general_event_manager_t::get_general_event_info(p_role, reply);
	p_role->send_msg_to_client(op_cmd::gc_get_general_event_info_reply, reply);
	return true;
}

bool general_event_msg_handler_t::handle_cg_get_general_event_detail_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_get_general_event_detail_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	proto::client::gc_get_general_event_detail_reply reply;
	general_event_manager_t::get_general_event_dtail_info(p_role, msg.id(), reply);
	p_role->send_msg_to_client(op_cmd::gc_get_general_event_detail_reply, reply);
	return true;
}

bool general_event_msg_handler_t::handle_cg_get_general_event_prize_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_get_general_event_prize_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	proto::client::gc_get_general_event_prize_reply reply;
	general_event_manager_t::get_general_event_prize(p_role, msg.index(), reply.mutable_rcd());
	p_role->send_msg_to_client(op_cmd::gc_get_general_event_prize_reply, reply);
	return true;
}

bool general_event_msg_handler_t::handle_cg_get_server_level_limit_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_get_server_level_limit_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}

	proto::client::gc_get_server_level_limit_reply reply;
	uint32_t level = general_info_manager_t::get_server_level();
	reply.set_level( level );
	p_role->send_msg_to_client(op_cmd::gc_get_server_level_limit_reply, reply);
	return true;
}


bool general_event_msg_handler_t::handler_eg_sync_general_event_count_info(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_sync_general_event_count_info);
	
	general_info_manager_t::sync_general_event_count_data(msg);

	return true;
}

bool general_event_msg_handler_t::handler_eg_sync_general_event_data(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_sync_general_event_data);
	
	general_info_manager_t::sync_general_event_data(msg);

	return true;
}


bool general_event_msg_handler_t::handler_eg_sync_general_event_open(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_sync_general_event_open);
	
	general_info_manager_t::on_genreal_event_start(msg);

	return true;
}

bool general_event_msg_handler_t::handler_eg_sync_general_event_done(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_sync_general_event_done);
	
	general_info_manager_t::on_genreal_event_done(msg);

	return true;
}

bool general_event_msg_handler_t::handle_eg_sync_server_level(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_sync_server_level);

	general_info_manager_t::sync_level_data(msg.level());

	return true;
}

