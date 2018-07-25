#include "world_cup_msg_handle.hpp"
#include "gate/gate_msg_handler.hpp"
#include "client.pb.h"
#include "world_cup/world_cup_manager.hpp"
#include "user/global_user_manager.hpp"

USING_NS_COMMON;

bool world_cup_msg_handle_t::init_client_msg_handler()
{
	bool bresult = true;

	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_world_cup_data_request, handle_ce_world_cup_data_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_world_cup_odds_request, handle_ce_world_cup_odds_request);
	return bresult;
}

bool world_cup_msg_handle_t::handle_ce_world_cup_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_world_cup_data_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::server::er_world_cup_bet_data_request req;
	req.set_server_id(env::xml->srv_group.group_id);
	env::server->send_msg_to_transfer(op_cmd::er_world_cup_bet_data_request, uid, req);
	return true;
}

bool world_cup_msg_handle_t::handle_ce_world_cup_odds_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_world_cup_odds_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::server::er_world_cup_odds_request req;
	req.set_server_id(env::xml->srv_group.group_id);
	req.set_battle_uid(string_util_t::string_to_uint64(msg.battle_uid()));
	env::server->send_msg_to_transfer(op_cmd::er_world_cup_odds_request, uid, req);
	return true;
}
