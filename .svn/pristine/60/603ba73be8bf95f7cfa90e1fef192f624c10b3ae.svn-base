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
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_king_war_undo_sign_up_request, handle_ce_king_war_undo_sign_up_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_king_war_notice_request, handle_ce_king_war_notice_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_king_war_like_request, handle_ce_king_war_like_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_king_war_scene_num_request, handle_ce_king_war_scene_num_request);

	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_king_war_help_family_list_request, handle_ce_king_war_help_family_list_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_king_war_help_family_fuzzy_request, handle_ce_king_war_help_family_fuzzy_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_king_war_invite_help_request, handle_ce_king_war_invite_help_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_king_war_invite_process_request, handle_ce_king_war_invite_process_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_king_war_invites_clear_request, handle_ce_king_war_invites_clear_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_cencel_king_war_invite_request, handle_ce_cencel_king_war_invite_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_cencel_king_war_help_request, handle_ce_cencel_king_war_help_request);

	return bresult;
}

bool king_war_msg_handle_t::init_game_msg_handler()
{
	bool bresult = true;

	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_king_war_scene_user_num_notify, handle_ge_king_war_scene_user_num_notify);
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
	king_war_manager_t::apply_king_war_request(p_user, msg.country_id(), reply);
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
	king_war_manager_t::bid_king_war_request(p_user, msg.country_id(), msg.money(), reply);
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
	king_war_manager_t::get_king_war_apply_list(p_user, msg.country_id(), reply);
	p_user->send_msg_to_client(op_cmd::ec_king_war_sign_up_list_reply, reply);

	return true;
}

bool king_war_msg_handle_t::handle_ce_king_war_undo_sign_up_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_king_war_undo_sign_up_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_king_war_undo_sign_up_reply reply;
	king_war_manager_t::sudo_king_war_apply(p_user, reply);
	p_user->send_msg_to_client(op_cmd::ec_king_war_undo_sign_up_reply, reply);

	return true;
}

bool king_war_msg_handle_t::handle_ce_king_war_notice_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_king_war_notice_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_king_war_notice_reply reply;
	king_war_manager_t::update_king_war_notice(p_user, msg.country_id(), msg.notice(), reply);
	p_user->send_msg_to_client(op_cmd::ec_king_war_notice_reply, reply);

	return true;
}

bool king_war_msg_handle_t::handle_ce_king_war_like_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_king_war_like_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_king_war_like_reply reply;
	king_war_manager_t::update_king_war_like(p_user, msg.country_id(), common::string_util_t::string_to_uint64(msg.family_id()), msg.like(), reply);
	p_user->send_msg_to_client(op_cmd::ec_king_war_like_reply, reply);

	return true;
}

bool king_war_msg_handle_t::handle_ce_king_war_scene_num_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_king_war_scene_num_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_king_war_scene_num_reply reply;
	king_war_manager_t::get_king_war_scene_num(msg.country_id(), reply);
	p_user->send_msg_to_client(op_cmd::ec_king_war_scene_num_reply, reply);

	return true;
}

bool king_war_msg_handle_t::handle_ce_king_war_help_family_list_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_king_war_help_family_list_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_king_war_help_family_list_reply reply;
	king_war_manager_t::get_king_war_help_family_list(p_user, msg.page(), reply);
	p_user->send_msg_to_client(op_cmd::ec_king_war_help_family_list_reply, reply);

	return true;
}

bool king_war_msg_handle_t::handle_ce_king_war_help_family_fuzzy_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_king_war_help_family_fuzzy_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_king_war_help_family_fuzzy_reply reply;
	king_war_manager_t::get_king_war_help_family_fuzzy(p_user, msg.name(), reply);
	p_user->send_msg_to_client(op_cmd::ec_king_war_help_family_fuzzy_reply, reply);

	return true;
}

bool king_war_msg_handle_t::handle_ce_king_war_invite_help_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_king_war_invite_help_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	uint32_t back_code = king_war_manager_t::invite_king_war_help_family(p_user, common::string_util_t::string_to_uint64(msg.family_uid()));
	proto::client::ec_king_war_invite_help_reply reply;
	reply.set_reply_code(back_code);
	reply.set_family_uid(msg.family_uid());
	p_user->send_msg_to_client(op_cmd::ec_king_war_invite_help_reply, reply);

	return true;
}

bool king_war_msg_handle_t::handle_ce_king_war_invite_process_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_king_war_invite_process_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	uint32_t back_code = king_war_manager_t::king_war_process_invited(p_user, common::string_util_t::string_to_uint64(msg.family_uid()), msg.process());
	proto::client::ec_king_war_invite_process_reply reply;
	reply.set_reply_code(back_code);
	p_user->send_msg_to_client(op_cmd::ec_king_war_invite_process_reply, reply);

	return true;

}

bool king_war_msg_handle_t::handle_ce_king_war_invites_clear_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_king_war_invites_clear_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	country_mgr_t::clear_king_war_invite_info(p_user);

	return true;
}

bool king_war_msg_handle_t::handle_ce_cencel_king_war_invite_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_cencel_king_war_invite_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_cencel_king_war_invite_reply reply;
	king_war_manager_t::king_war_cancel_invited(p_user, reply);
	p_user->send_msg_to_client(op_cmd::ec_cencel_king_war_invite_reply, reply);

	return true;
}

bool king_war_msg_handle_t::handle_ce_cencel_king_war_help_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_cencel_king_war_help_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_cencel_king_war_help_reply reply;
	king_war_manager_t::king_war_cancel_help(p_user, reply);
	p_user->send_msg_to_client(op_cmd::ec_cencel_king_war_help_reply, reply);

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
	
	p_country->set_king_war_result(msg);

	return true;
}

bool king_war_msg_handle_t::handle_ge_king_war_scene_user_num_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_king_war_scene_user_num_notify);

	uint32_t country_id = msg.country_id();
	country_ptr p_country = country_mgr_t::get_country(country_id);
	if (NULL == p_country)
	{
		log_error("p_country[%u] null error", country_id);
		return false;
	}

	p_country->set_king_war_scene_role_num(msg.def_num(), msg.att_num());

	return true;
}

