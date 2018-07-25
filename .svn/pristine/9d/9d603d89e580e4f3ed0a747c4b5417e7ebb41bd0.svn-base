#include "country_msg_handle.hpp"
#include "log.hpp"
#include "gate/gate_msg_handler.hpp"
#include "game/game_msg_handler.hpp"
#include "user/global_user.hpp"
#include "user/global_user_manager.hpp"
#include "client.pb.h"
#include "server.pb.h"
#include "country_mgr.hpp"
#include "tblh/errcode_enum.hpp"
#include "family/family_manager.hpp"

bool country_msg_handle_t::init_gate_msg_handler()
{
	bool bresult = true;

	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_country_preview_request, handle_ce_country_preview_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_country_alliance_request, handle_ce_country_alliance_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_dismiss_alliance_request, handle_ce_dismiss_alliance_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_country_info_request, handle_ce_country_info_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_modify_year_name_request, handle_ce_modify_year_name_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_modify_notice_request, handle_ce_modify_notice_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_open_appoint_request, handle_ce_open_appoint_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_appoint_officer_request, handle_ce_appoint_officer_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_fire_officer_request, handle_ce_fire_officer_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_resign_officer_request, handle_ce_resign_officer_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_country_family_list_request, handle_ce_country_family_list_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_country_create_list_request, handle_ce_country_create_list_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_country_flag_list_request, handle_ce_country_flag_list_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_change_country_name_request, handle_ce_change_country_name_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_get_king_info_request, handle_ce_get_king_info_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_world_trend_info_request, handle_ce_world_trend_info_request);

	return bresult;
}

bool country_msg_handle_t::init_game_msg_handler()
{
	bool bresult = true;

	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_gm_set_king_family_notify, handle_ge_gm_set_king_family_notify);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_country_name_change_reply, handle_ge_country_name_change_reply);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_country_year_name_reply, handle_ge_country_year_name_reply);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_modify_country_money_request, handle_ge_modify_country_money_request);

	return bresult;
}


bool country_msg_handle_t::handle_ce_country_preview_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_country_preview_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_country_preview_reply reply;
	country_mgr_t::get_country_preview(reply);
	p_user->send_msg_to_client(op_cmd::ec_country_preview_reply, reply);

	return true;
}


bool country_msg_handle_t::handle_ce_country_alliance_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_country_alliance_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_country_alliance_reply reply;
	country_mgr_t::ask_country_alliance(p_user, msg.country_id(), reply);
	p_user->send_msg_to_client(op_cmd::ec_country_alliance_reply, reply);

	return true;
}


bool country_msg_handle_t::handle_ce_dismiss_alliance_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_dismiss_alliance_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	uint32_t back_code = country_mgr_t::quit_country_alliance(p_user);
	proto::client::ec_dismiss_alliance_reply reply;
	reply.set_reply_code(back_code);
	p_user->send_msg_to_client(op_cmd::ec_dismiss_alliance_reply, reply);

	return true;
}


bool country_msg_handle_t::handle_ce_country_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_country_info_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_country_info_reply reply;
	country_mgr_t::get_country_info(msg.country_id(), reply);
	p_user->send_msg_to_client(op_cmd::ec_country_info_reply, reply);

	return true;
}


bool country_msg_handle_t::handle_ce_modify_year_name_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_modify_year_name_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	uint32_t back_code = country_mgr_t::modify_year_name(p_user, msg.name());
	if (back_code != common::errcode_enum::error_ok) {
		proto::client::ec_modify_year_name_reply reply;
		reply.set_reply_code(back_code);
		p_user->send_msg_to_client(op_cmd::ec_modify_year_name_reply, reply);
	}

	return true;
}


bool country_msg_handle_t::handle_ce_modify_notice_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_modify_notice_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_modify_notice_reply reply;
	uint32_t back_code = country_mgr_t::modify_country_notice(p_user, msg.name());
	reply.set_reply_code(back_code);
	p_user->send_msg_to_client(op_cmd::ec_modify_notice_reply, reply);

	return true;
}


bool country_msg_handle_t::handle_ce_open_appoint_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_open_appoint_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_open_appoint_reply reply;
	country_mgr_t::open_appoint_request(p_user, msg.officer_id(), reply);
	p_user->send_msg_to_client(op_cmd::ec_open_appoint_reply, reply);

	return true;
}


bool country_msg_handle_t::handle_ce_appoint_officer_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_appoint_officer_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_appoint_officer_reply reply;
	country_mgr_t::appoint_officer_request(p_user, common::string_util_t::string_to_uint64(msg.uid()), msg.officer_id(), reply);
	p_user->send_msg_to_client(op_cmd::ec_appoint_officer_reply, reply);
	
	return true;
}


bool country_msg_handle_t::handle_ce_fire_officer_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_fire_officer_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_fire_officer_reply reply;
	country_mgr_t::fire_officer_request(p_user, msg.officer_id(), reply);
	p_user->send_msg_to_client(op_cmd::ec_fire_officer_reply, reply);

	return true;
}


bool country_msg_handle_t::handle_ce_resign_officer_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_resign_officer_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_resign_officer_reply reply;
	country_mgr_t::resign_officer_request(p_user, reply);
	p_user->send_msg_to_client(op_cmd::ec_resign_officer_reply, reply);

	return true;
}


bool country_msg_handle_t::handle_ce_country_family_list_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_country_family_list_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	country_ptr p_country = country_mgr_t::get_country(msg.country_id());
	if (!p_country) {
		log_error("p_country[%u] not find", msg.country_id());
		return false;
	}

	proto::client::ec_country_family_list_reply reply;
	p_country->get_country_family_list(reply);
	p_user->send_msg_to_client(op_cmd::ec_country_family_list_reply, reply);

	return true;
}


bool country_msg_handle_t::handle_ce_country_create_list_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_country_create_list_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_country_create_list_reply reply;
	// TODO:
	p_user->send_msg_to_client(op_cmd::ec_country_create_list_reply, reply);

	return true;
}


bool country_msg_handle_t::handle_ce_country_flag_list_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_country_flag_list_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_country_flag_list_reply reply;
	country_mgr_t::get_country_flag_list(p_user, reply);
	p_user->send_msg_to_client(op_cmd::ec_country_flag_list_reply, reply);

	return true;
}


bool country_msg_handle_t::handle_ce_change_country_name_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_change_country_name_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}
	
	uint32_t back_code = country_mgr_t::modify_country_name_flag(p_user, msg.flag_id(), msg.name());
	if (back_code != common::errcode_enum::error_ok) {
		proto::client::ec_change_country_name_reply reply;
		reply.set_reply_code(back_code);
		p_user->send_msg_to_client(op_cmd::ec_change_country_name_reply, reply);
	}

	return true;
}

bool country_msg_handle_t::handle_ge_gm_set_king_family_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_gm_set_king_family_notify);

	country_ptr p_country = country_mgr_t::get_country(msg.country_id());
	if (!p_country) {
		log_error("country[%u] not find", msg.country_id());
		return false;
	}

	p_country->set_new_king_family(msg.family_uid());

	return true;
}

bool country_msg_handle_t::handle_ge_country_name_change_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_country_name_change_reply);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	country_mgr_t::modify_country_name_flag_back(p_user, msg);

	return true;
}

bool country_msg_handle_t::handle_ge_country_year_name_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_country_year_name_reply);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	country_mgr_t::modify_country_year_name_back(p_user, msg);

	return true;
}

bool country_msg_handle_t::handle_ge_modify_country_money_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_modify_country_money_request);

	country_ptr p_country = country_mgr_t::get_country(msg.country_id());
	if (!p_country) 
	{
		log_error("country[%u] not find", msg.country_id());
		return false;
	}

	if (msg.type() == 0)
	{
		p_country->add_money(msg.num(), msg.source_type(), uid);
	}
	else
	{
		p_country->cost_money(msg.num(), msg.source_type(), uid);
	}

	return true;
}

bool country_msg_handle_t::handle_ce_get_king_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_get_king_info_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	uint32_t country_id = p_user->get_country_id();

	proto::client::ec_get_king_info_reply reply;
	country_ptr p_country = country_mgr_t::get_country(country_id);
	if (!p_country) 
	{
		reply.set_role_uid("");
		reply.set_plugin(0);
		reply.set_name("");
		p_user->send_msg_to_client(op_cmd::ec_get_king_info_reply, reply);
		return true;
	}
	
	const auto &country_info = p_country->get_country_info();
	auto officer_map= country_info.officers; // ¹ÙÖ°ĞÅÏ¢	

	auto iter = officer_map.find(officer_type::officer_type_king);
	if (iter == officer_map.end())
	{
		reply.set_role_uid("");
		reply.set_plugin(0);
		reply.set_name("");
	}else
	{
		proto::common::country_officer_data &king_info = iter->second;
		reply.set_role_uid(king_info.uid());
		reply.set_plugin(king_info.plugin());
		reply.set_name(king_info.name());
	}
	p_user->send_msg_to_client(op_cmd::ec_get_king_info_reply, reply);
	return true;
}

bool country_msg_handle_t::handle_ce_world_trend_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_world_trend_info_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_world_trend_info_reply reply;
	country_mgr_t::world_trend_info(p_user, reply.mutable_king_war());
	family_manager_t::world_trend_info_family_war(p_user, reply);
	p_user->send_msg_to_client(op_cmd::ec_world_trend_info_reply, reply);

	return true;
}

