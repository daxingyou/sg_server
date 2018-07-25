#include "offline_arena_msg_handle.hpp"

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
#include "offline_arena_manager.hpp"
#include "tblh/errcode_enum.hpp"

USING_NS_COMMON;

bool offline_arena_msg_handle_t::init_client_msg_handler()
{
	bool bresult = true;

	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_offline_arena_data_request, handle_ce_offline_arena_data_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_offline_arena_enemy_data_request, handle_ce_offline_arena_enemy_data_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_offline_arena_fight_request, handle_ce_offline_arena_fight_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_offline_arena_record_award_request, handle_ce_offline_arena_record_award_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_offline_arena_rank_request, handle_ce_offline_arena_rank_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_offline_arena_record_request, handle_ce_offline_arena_record_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_offline_arena_sweep_request, handle_ce_offline_arena_sweep_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_offline_arena_score_award_request, handle_ce_offline_arena_score_award_request);
	return bresult;
}

bool offline_arena_msg_handle_t::init_game_msg_handler()
{
	bool bresult = true;

	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_offline_arena_form_notify, handle_ge_offline_arena_form_notify);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_offline_arena_fight_reply, handle_ge_offline_arena_fight_reply);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_offline_arena_buy_request, handle_ge_offline_arena_buy_request);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_offline_arena_gmrank_notify, handle_ge_offline_arena_gmrank_notify);
	return bresult;
}

bool offline_arena_msg_handle_t::handle_ce_offline_arena_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_offline_arena_data_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}
	if (p_user->get_troop_id() > 0)
	{
		log_error("user[%lu] in troop", uid);
		return false;
	}
	proto::client::ec_offline_arena_data_reply reply;
	offline_arena_manager_t::peek_data(uid, reply);
	p_user->send_msg_to_client(op_cmd::ec_offline_arena_data_reply, reply);
	return true;
}

bool offline_arena_msg_handle_t::handle_ce_offline_arena_enemy_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_offline_arena_enemy_data_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}
	if (p_user->get_troop_id() > 0)
	{
		log_error("user[%lu] in troop", uid);
		return false;
	}
	uint64_t role_uid = string_util_t::string_to_uint64(msg.role_uid());
	proto::client::ec_offline_arena_enemy_data_reply reply;
	offline_arena_manager_t::peek_enemy_data(role_uid, reply.mutable_form_data());
	p_user->send_msg_to_client(op_cmd::ec_offline_arena_enemy_data_reply, reply);
	return true;
}

bool offline_arena_msg_handle_t::handle_ce_offline_arena_fight_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_offline_arena_fight_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}
	if (p_user->get_troop_id() > 0)
	{
		log_error("user[%lu] in troop", uid);
		return false;
	}
	uint64_t role_uid = string_util_t::string_to_uint64(msg.role_uid());
	uint32_t reply_code = offline_arena_manager_t::fight(uid, msg.self_rank(), msg.object_type(), role_uid, msg.enemy_rank());
	if (reply_code != errcode_enum::error_ok)
	{
		proto::client::ec_offline_arena_fight_reply reply;
		reply.set_reply_code(reply_code);
		p_user->send_msg_to_client(op_cmd::ec_offline_arena_fight_reply, reply);
	}
	return true;
}

bool offline_arena_msg_handle_t::handle_ce_offline_arena_record_award_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_offline_arena_record_award_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}
	if (p_user->get_troop_id() > 0)
	{
		log_error("user[%lu] in troop", uid);
		return false;
	}
	offline_arena_manager_t::record_award(uid);
	return true;
}

bool offline_arena_msg_handle_t::handle_ce_offline_arena_rank_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_offline_arena_rank_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}
	
	proto::client::ec_offline_arena_rank_reply reply;
	offline_arena_manager_t::peek_rank_data(uid, msg.offine_arena_class(), reply);
	p_user->send_msg_to_client(op_cmd::ec_offline_arena_rank_reply, reply);
	return true;
}

bool offline_arena_msg_handle_t::handle_ce_offline_arena_record_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_offline_arena_record_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_offline_arena_record_reply reply;
	offline_arena_manager_t::peek_record_data(uid, reply.mutable_record_data());
	p_user->send_msg_to_client(op_cmd::ec_offline_arena_record_reply, reply);
	return true;
}

bool offline_arena_msg_handle_t::handle_ce_offline_arena_sweep_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_offline_arena_sweep_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}
	uint64_t role_uid = string_util_t::string_to_uint64(msg.role_uid());
	proto::client::ec_offline_arena_sweep_reply reply;
	reply.set_reply_code(offline_arena_manager_t::sweep(uid, msg.self_rank(), msg.object_type(), role_uid, msg.enemy_rank(), msg.count()));
	p_user->send_msg_to_client(op_cmd::ec_offline_arena_sweep_reply, reply);
	return true;
}

bool offline_arena_msg_handle_t::handle_ce_offline_arena_score_award_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_offline_arena_score_award_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}
	offline_arena_manager_t::score_award(uid, msg.score());
	return true;
}

bool offline_arena_msg_handle_t::handle_ge_offline_arena_form_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_offline_arena_form_notify);
	offline_arena_manager_t::form_notify(uid, msg.form_data());
	return true;
}

bool offline_arena_msg_handle_t::handle_ge_offline_arena_fight_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_offline_arena_fight_reply);
	offline_arena_manager_t::fight_result(uid, msg.is_win());
	return true;
}

bool offline_arena_msg_handle_t::handle_ge_offline_arena_buy_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_offline_arena_buy_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}
	if (p_user->get_troop_id() > 0)
	{
		log_error("user[%lu] in troop", uid);
		return false;
	}
	uint32_t reply_code = offline_arena_manager_t::buy(uid, msg.money());
	proto::server::eg_offline_arena_buy_reply reply;
	reply.set_reply_code(reply_code);
	reply.set_money(msg.money());
	p_user->send_msg_to_game(op_cmd::eg_offline_arena_buy_reply, reply);
	return true;
}

bool offline_arena_msg_handle_t::handle_ge_offline_arena_gmrank_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_offline_arena_gmrank_notify);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}
	
	offline_arena_manager_t::gm_rank(uid, msg.rank());
	return true;
}

