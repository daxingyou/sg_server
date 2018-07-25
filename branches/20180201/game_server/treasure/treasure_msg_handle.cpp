#include "treasure_msg_handle.hpp"
#include "common/log_enum.hpp"
#include "role/role_manager.hpp"
#include "user_troop/user_troop_mgr.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

bool treasure_msg_handle_t::init_msg_handler()
{
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_treasure_data_request, handle_cg_treasure_data_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_treasure_drop_request, handle_cg_treasure_drop_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_treasure_gather_request, handle_cg_treasure_gather_request);
    return true;
}

bool treasure_msg_handle_t::handle_cg_treasure_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_treasure_data_request);
	auto role = role_manager_t::find_role(uid);
	if (NULL == role)
	{
		log_error("role[%lu] NULL", uid);
		return false;
	}
	uint64_t troop_id = role->get_troop_id();
	if (0 == troop_id)
	{
		log_error("role[%lu] not in troop", uid);
		return false;
	}
	troop_ptr p_troop = game_troop_mgr_t::get_troop(troop_id);
	if (NULL == p_troop)
	{
		log_error("role[%lu] not in troop", uid);
		return false;
	}
	p_troop->open_treasure(uid);
	return true;
}

bool treasure_msg_handle_t::handle_cg_treasure_drop_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_treasure_drop_request);
	auto role = role_manager_t::find_role(uid);
	if (NULL == role)
	{
		log_error("role[%lu] NULL", uid);
		return false;
	}
	uint64_t troop_id = role->get_troop_id();
	if (0 == troop_id)
	{
		log_error("role[%lu] not in troop", uid);
		return false;
	}
	troop_ptr p_troop = game_troop_mgr_t::get_troop(troop_id);
	if (NULL == p_troop)
	{
		log_error("role[%lu] not in troop", uid);
		return false;
	}
	p_troop->drop_treasure(uid);
	return true;
}

bool treasure_msg_handle_t::handle_cg_treasure_gather_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_treasure_gather_request);
	auto role = role_manager_t::find_role(uid);
	if (NULL == role)
	{
		log_error("role[%lu] NULL", uid);
		return false;
	}
	uint64_t troop_id = role->get_troop_id();
	if (0 == troop_id)
	{
		log_error("role[%lu] not in troop", uid);
		return false;
	}
	troop_ptr p_troop = game_troop_mgr_t::get_troop(troop_id);
	if (NULL == p_troop)
	{
		log_error("role[%lu] not in troop", uid);
		return false;
	}
	p_troop->gather_treasure(uid);
	return true;
}
