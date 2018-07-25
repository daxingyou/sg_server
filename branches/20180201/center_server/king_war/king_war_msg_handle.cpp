#include "king_war_msg_handle.hpp"

#include "gate/gate_msg_handler.hpp"
#include "game/game_msg_handler.hpp"

#include "common.pb.h"
#include "client.pb.h"
#include "server.pb.h"

#include "user/global_user_manager.hpp"
#include "utility.hpp"
#include "main/center_server_fwd.hpp"
#include "main/center_server.hpp"
#include "user/global_user.hpp"
#include "tblh/errcode_enum.hpp"
#include "country/country_mgr.hpp"
#include "king_war_manager.hpp"
#include "country/country_mgr.hpp"


bool king_war_msg_handle_t::init_gate_msg_handler()
{
	bool bresult = true;

	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_king_war_sign_up_request, handle_ce_king_war_sign_up_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_king_war_offer_money_request, handle_ce_king_war_offer_money_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_king_war_sign_up_list_request, handle_ce_king_war_sign_up_list_request);

	return bresult;
}

bool king_war_msg_handle_t::init_game_msg_handler()
{
	bool bresult = true;

	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_king_war_result_notify, handle_ge_king_war_result_notify);

	return bresult;
}

bool king_war_msg_handle_t::handle_ce_king_war_sign_up_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_king_war_sign_up_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_king_war_sign_up_reply reply;
	king_war_manager_t::apply_king_war_request(p_user, reply);
	p_user->send_msg_to_client(op_cmd::ec_king_war_sign_up_reply, reply);

	return true;
}

bool king_war_msg_handle_t::handle_ce_king_war_offer_money_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_king_war_offer_money_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_king_war_offer_money_reply reply;
	king_war_manager_t::bid_king_war_request(p_user, msg.money(), reply);
	p_user->send_msg_to_client(op_cmd::ec_king_war_offer_money_reply, reply);

	return true;
}

bool king_war_msg_handle_t::handle_ce_king_war_sign_up_list_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_king_war_sign_up_list_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_king_war_sign_up_list_reply reply;
	king_war_manager_t::get_king_war_apply_list(p_user, reply);
	p_user->send_msg_to_client(op_cmd::ec_king_war_sign_up_list_reply, reply);

	return true;
}

bool king_war_msg_handle_t::handle_ge_king_war_result_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_king_war_result_notify);

	uint32_t country_id = msg.country_id();
	country_ptr p_country = country_mgr_t::get_country(country_id);
	if (NULL == p_country)
	{
		log_error("p_country[%u] null error", country_id);
		return false;
	}

	uint64_t family_id = msg.win_family_id();
	
	p_country->set_king_war_result(family_id);

	return true;
}

