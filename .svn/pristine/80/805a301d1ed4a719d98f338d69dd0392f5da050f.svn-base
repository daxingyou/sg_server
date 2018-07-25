#include "family_msg_handle.hpp"
#include "tblh/errcode_enum.hpp"
#include "utility.hpp"
#include "family_manager.hpp"
#include "gate/gate_msg_handler.hpp"
#include "game/game_msg_handler.hpp"
#include "user/global_user.hpp"
#include "user/global_user_manager.hpp"
#include "client.pb.h"
#include "server.pb.h"

bool family_msg_handle::init_gate_msg_handler()
{
	bool bresult = true;

	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_join_family_request, handle_ce_join_family_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_decline_join_family_request, handle_ce_decline_join_family_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_one_key_decline_join_family_request, handle_ce_one_key_decline_join_family_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_approve_join_family_request, handle_ce_approve_join_family_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_one_key_approve_join_family_request, handle_ce_one_key_approve_join_family_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_leave_family_request, handle_ce_leave_family_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_family_assign_request, handle_ce_family_assign_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_family_fire_request, handle_ce_family_fire_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_kick_family_member_request, handle_ce_kick_family_member_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_get_family_list_request, handle_ce_get_family_list_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_get_family_info_request, handle_ce_get_family_info_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_get_family_member_list_request, handle_ce_get_family_member_list_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_toggle_auto_newbie_request, handle_ce_toggle_auto_newbie_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_modify_family_declaration_request, handle_ce_modify_family_declaration_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_modify_family_notification_request, handle_ce_modify_family_notification_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_family_apply_list_request, handle_ce_family_apply_list_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_get_family_build_info_request, handle_ce_get_family_build_info_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_upgrade_family_build_request, handle_ce_upgrade_family_build_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_clear_family_build_upgrade_cd_request, handle_ce_clear_family_build_upgrade_cd_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_family_fuzzy_query_request, handle_ce_family_fuzzy_query_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_family_invite_request, handle_ce_family_invite_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_reply_family_invite_request, handle_ce_reply_family_invite_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_refuse_family_invite_request, handle_ce_refuse_family_invite_request);

	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_get_family_war_list_request, handle_ce_get_family_war_list_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_get_family_war_history_request, handle_ce_get_family_war_history_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_enter_family_war_scene_request, handle_ce_enter_family_war_scene_request);

	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_family_prayer_list_request, handle_ce_family_prayer_list_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_prayer_gift_reward_request, handle_ce_prayer_gift_reward_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_prayer_record_request, handle_ce_prayer_record_request);

	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_get_family_shop_request, handle_ce_get_family_shop_request);

	return bresult;
}

bool family_msg_handle::init_game_msg_handler()
{
	bool bresult = true;

 	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_create_family_request, handle_ge_create_family_request);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_prepare_family_war_reply, handle_ge_prepare_family_war_reply);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_family_war_early_result_notify, handle_ge_family_war_early_result_notify);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_family_war_result_notify, handle_ge_family_war_result_notify);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_family_update_glory_notify, handle_ge_family_update_glory_notify);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_family_prayer_request, handle_ge_family_prayer_request);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_family_prayer_gift_request, handle_ge_family_prayer_gift_request);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_modify_family_money_request, handle_ge_modify_family_money_request);

	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_family_shop_buy_request, handle_ge_family_shop_buy_request);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_family_shop_back_notify, handle_ge_family_shop_back_notify);

	return bresult;
}

bool family_msg_handle::handle_ce_join_family_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_join_family_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_join_family_reply reply;
	family_manager_t::join_family_request(p_user, common::string_util_t::string_to_uint64(msg.id()), reply);
	p_user->send_msg_to_client(op_cmd::ec_join_family_reply, reply);

	return true;
}

bool family_msg_handle::handle_ce_decline_join_family_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_decline_join_family_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_decline_join_family_reply reply;
	family_manager_t::decline_join_family(p_user, common::string_util_t::string_to_uint64(msg.role_id()), reply);
	p_user->send_msg_to_client(op_cmd::ec_decline_join_family_reply, reply);

	return true;
}

bool family_msg_handle::handle_ce_one_key_decline_join_family_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_one_key_decline_join_family_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_one_key_decline_join_family_reply reply;
	family_manager_t::one_key_decline_join_family(p_user, reply);
	p_user->send_msg_to_client(op_cmd::ec_one_key_decline_join_family_reply, reply);

	return true;
}

bool family_msg_handle::handle_ce_approve_join_family_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_approve_join_family_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_approve_join_family_reply reply;
	family_manager_t::approve_join_family(p_user, common::string_util_t::string_to_uint64(msg.role_id()), reply);
	p_user->send_msg_to_client(op_cmd::ec_approve_join_family_reply, reply);

	return true;
}

bool family_msg_handle::handle_ce_one_key_approve_join_family_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_one_key_approve_join_family_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_one_key_approve_join_family_reply reply;
	family_manager_t::one_key_approve_join_family(p_user, reply);
	p_user->send_msg_to_client(op_cmd::ec_one_key_approve_join_family_reply, reply);

	return true;
}

bool family_msg_handle::handle_ce_leave_family_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_leave_family_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_leave_family_reply reply;
	family_manager_t::leave_family(p_user, reply);
	p_user->send_msg_to_client(op_cmd::ec_leave_family_reply, reply);

	return true;
}

bool family_msg_handle::handle_ce_family_assign_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_family_assign_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_family_assign_reply reply;
	uint32_t back_code = family_manager_t::assign_member(p_user, common::string_util_t::string_to_uint64(msg.role_id()), msg.office());
	reply.set_reply_code(back_code);
	if (back_code == common::errcode_enum::error_ok)
	{
		reply.set_role_id(msg.role_id());
		reply.set_office(msg.office());
	}
	p_user->send_msg_to_client(op_cmd::ec_family_assign_reply, reply);

	return true;
}

bool family_msg_handle::handle_ce_family_fire_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_family_fire_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_family_fire_reply reply;
	uint32_t back_code = family_manager_t::fire_member(p_user, common::string_util_t::string_to_uint64(msg.role_id()));
	reply.set_reply_code(back_code);
	if (back_code == common::errcode_enum::error_ok)
	{
		reply.set_role_id(msg.role_id());
	}
	p_user->send_msg_to_client(op_cmd::ec_family_fire_reply, reply);

	return true;
}

bool family_msg_handle::handle_ce_kick_family_member_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_kick_family_member_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_kick_family_member_reply reply;
	uint32_t back_code = family_manager_t::kick_member(p_user, common::string_util_t::string_to_uint64(msg.role_id()));
	reply.set_reply_code(back_code);
	if (back_code == common::errcode_enum::error_ok)
	{
		reply.set_role_id(msg.role_id());
	}
	p_user->send_msg_to_client(op_cmd::ec_kick_family_member_reply, reply);

	return true;
}

bool family_msg_handle::handle_ce_get_family_list_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_get_family_list_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_get_family_list_reply reply;
	family_manager_t::get_family_list(msg.page(), reply);
	p_user->send_msg_to_client(op_cmd::ec_get_family_list_reply, reply);

	return true;
}

bool family_msg_handle::handle_ce_get_family_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_get_family_info_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_get_family_info_reply reply;
	family_manager_t::get_family_info(p_user, reply);
	p_user->send_msg_to_client(op_cmd::ec_get_family_info_reply, reply);

	return true;
}

bool family_msg_handle::handle_ce_get_family_member_list_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_get_family_member_list_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_get_family_member_list_reply reply;
	family_manager_t::get_family_member_list(p_user, reply);
	p_user->send_msg_to_client(op_cmd::ec_get_family_member_list_reply, reply);

	return true;
}

bool family_msg_handle::handle_ce_toggle_auto_newbie_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_toggle_auto_newbie_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_toggle_auto_newbie_reply reply;
	family_manager_t::toggle_auto_newbie(p_user, reply);
	p_user->send_msg_to_client(op_cmd::ec_toggle_auto_newbie_reply, reply);

	return true;
}

bool family_msg_handle::handle_ce_modify_family_declaration_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_modify_family_declaration_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_modify_family_declaration_reply reply;
	family_manager_t::modify_family_declaration(p_user, msg.declaration(), reply);
	p_user->send_msg_to_client(op_cmd::ec_modify_family_declaration_reply, reply);

	return true;
}

bool family_msg_handle::handle_ce_modify_family_notification_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_modify_family_notification_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_modify_family_notification_reply reply;
	family_manager_t::modify_family_notification(p_user, msg.notification(), reply);
	p_user->send_msg_to_client(op_cmd::ec_modify_family_notification_reply, reply);

	return true;
}

bool family_msg_handle::handle_ce_family_apply_list_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_family_apply_list_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_family_apply_list_reply reply;
	family_manager_t::get_family_apply_list(p_user, reply);
	p_user->send_msg_to_client(op_cmd::ec_family_apply_list_reply, reply);

	return true;
}

bool family_msg_handle::handle_ce_get_family_build_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_get_family_build_info_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_get_family_build_info_reply reply;
	family_manager_t::get_family_build_list(p_user, reply);
	p_user->send_msg_to_client(op_cmd::ec_get_family_build_info_reply, reply);

	return true;
}

bool family_msg_handle::handle_ce_upgrade_family_build_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_upgrade_family_build_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_upgrade_family_build_reply reply;
	family_manager_t::upgrade_family_build(p_user, msg.type(), reply);
	p_user->send_msg_to_client(op_cmd::ec_upgrade_family_build_reply, reply);

	return true;
}

bool family_msg_handle::handle_ce_clear_family_build_upgrade_cd_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_clear_family_build_upgrade_cd_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_clear_family_build_upgrade_cd_reply reply;
	family_manager_t::clear_family_build_cd(p_user, msg.type(), reply);
	p_user->send_msg_to_client(op_cmd::ec_clear_family_build_upgrade_cd_reply, reply);

	return true;
}

bool family_msg_handle::handle_ce_family_fuzzy_query_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_family_fuzzy_query_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_family_fuzzy_query_reply reply;
	family_manager_t::get_family_fuzzy_query(msg.name(), reply);
	p_user->send_msg_to_client(op_cmd::ec_family_fuzzy_query_reply, reply);

	return true;
}

bool family_msg_handle::handle_ce_family_invite_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_family_invite_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	if (p_user->get_family_id() != 0) {
		proto::client::ec_family_invite_reply reply;
		uint32_t back_code = family_manager_t::invite_family_member(p_user, common::string_util_t::string_to_uint64(msg.role_uid()));
		reply.set_reply_code(back_code);
		p_user->send_msg_to_client(op_cmd::ec_family_invite_reply, reply);
	} else {
		const global_user_ptr& p_join_user = global_user_manager_t::get_global_user_by_uid(common::string_util_t::string_to_uint64(msg.role_uid()));
		if (NULL == p_join_user) {
			log_error("NULL == p_join_user[%lu]", uid);
			return false;
		}

		proto::client::ec_join_family_reply reply;
		family_manager_t::join_family_request(p_user, p_join_user->get_family_id(), reply);
		p_user->send_msg_to_client(op_cmd::ec_join_family_reply, reply);
	}

	return true;
}

bool family_msg_handle::handle_ce_reply_family_invite_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_reply_family_invite_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	uint32_t back_code = family_manager_t::reply_family_invite(p_user, msg.result());
	if (back_code != common::errcode_enum::error_ok) {
		proto::client::ec_reply_family_invite_reply reply;
		reply.set_reply_code(back_code);
		p_user->send_msg_to_client(op_cmd::ec_reply_family_invite_reply, reply);
	}

	return true;
}

bool family_msg_handle::handle_ce_refuse_family_invite_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_refuse_family_invite_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	for (int32_t i = 0; i < msg.results_size(); ++i) {
		family_manager_t::reply_family_invite(p_user, msg.results(i));
	}

	return true;
}

bool family_msg_handle::handle_ge_create_family_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_create_family_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::server::eg_create_family_reply reply;
	family_manager_t::create_family_from_game(p_user, msg.name(), msg.declaration(), reply);
	env::server->send_msg_to_game(s, op_cmd::eg_create_family_reply, uid, reply);

	return true;
}

bool family_msg_handle::handle_ce_get_family_war_list_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_get_family_war_list_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_get_family_war_list_reply reply;
	family_manager_t::get_family_war_info(reply);
	p_user->send_msg_to_client(op_cmd::ec_get_family_war_list_reply, reply);

	return true;
}

bool family_msg_handle::handle_ce_get_family_war_history_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_get_family_war_history_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_get_family_war_history_reply reply;
	family_manager_t::get_history_info(msg.index(), reply);
	p_user->send_msg_to_client(op_cmd::ec_get_family_war_history_reply, reply);

	return true;
}

bool family_msg_handle::handle_ce_enter_family_war_scene_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_enter_family_war_scene_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	uint32_t back_code = family_manager_t::ask_enter_family_war_dungeon(p_user);
	if (back_code != common::errcode_enum::error_ok)
	{
		proto::client::ec_enter_family_war_scene_reply reply;
		reply.set_reply_code(back_code);
		p_user->send_msg_to_client(op_cmd::ec_enter_family_war_scene_reply, reply);
	}

	return true;
}

bool family_msg_handle::handle_ge_prepare_family_war_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_prepare_family_war_reply);

	uint32_t game_id = game_manager_t::get_game_id_by_socket(s);

	family_manager_t::prepare_family_war_reply(game_id, msg);

	return true;
}

bool family_msg_handle::handle_ge_family_war_early_result_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_family_war_early_result_notify);

	family_manager_t::family_war_early_result(msg.data());

	return true;
}

bool family_msg_handle::handle_ge_family_war_result_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_family_war_result_notify);

	family_manager_t::family_war_result(msg);

	return true;
}

bool family_msg_handle::handle_ge_family_update_glory_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_family_update_glory_notify);

	family_manager_t::family_update_glory(uid, msg.family_id(), msg.glory());

	return true;
}

//////////////////////////////////////////////////////////////////////////
bool family_msg_handle::handle_ce_family_prayer_list_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_family_prayer_list_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_family_prayer_list_reply reply;
	family_manager_t::get_family_prayer_list(p_user, reply);
	p_user->send_msg_to_client(op_cmd::ec_family_prayer_list_reply, reply);
	return true;
}

bool family_msg_handle::handle_ce_prayer_gift_reward_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_prayer_gift_reward_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_prayer_gift_reward_reply reply;
	family_manager_t::get_prayer_gift_reward(p_user, reply);
	p_user->send_msg_to_client(op_cmd::ec_prayer_gift_reward_reply, reply);
	return true;
}

bool family_msg_handle::handle_ce_prayer_record_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_prayer_record_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_prayer_record_reply reply;
	family_manager_t::get_prayer_record(p_user, reply);
	p_user->send_msg_to_client(op_cmd::ec_prayer_record_reply, reply);
	return true;
}

bool family_msg_handle::handle_ge_family_prayer_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_family_prayer_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	uint32_t back_code = family_manager_t::family_prayer_request(p_user, msg.hero_tid(), msg.hero_num());
	proto::client::ec_family_prayer_reply reply;
	reply.set_reply_code(back_code);
	p_user->send_msg_to_client(op_cmd::ec_family_prayer_reply, reply);

	return true;
}

bool family_msg_handle::handle_ge_family_prayer_gift_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_family_prayer_gift_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::server::eg_family_prayer_gift_failed_notify reply;
	family_manager_t::family_prayer_gift_request(p_user, msg.role_id(), msg.hero_tid(), msg.hero_material_id(), msg.hero_rarity(), msg.hero_num(), reply);
	reply.set_role_id(msg.role_id());
	reply.set_hero_tid(msg.hero_tid());
	reply.set_hero_num(msg.hero_num());
	reply.set_hero_material_id(msg.hero_material_id());
	// 赠送成功才需要把role_change_data 返回game_server
	// 失败直接告诉game_server 返回赠送的碎片
	if (reply.reply_code() == common::errcode_enum::error_ok) {
		proto::common::role_change_data* p_role_change_data = reply.mutable_change_data();
		if (p_role_change_data) p_role_change_data->CopyFrom(msg.change_data());
	}
	p_user->send_msg_to_game(op_cmd::eg_family_prayer_gift_failed_notify, reply);

	return true;
}

bool family_msg_handle::handle_ce_get_family_shop_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_get_family_shop_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_get_family_shop_reply reply;
	family_manager_t::get_family_shop_data(p_user, reply);
	p_user->send_msg_to_client(op_cmd::ec_get_family_shop_reply, reply);

	return true;
}

bool family_msg_handle::handle_ge_family_shop_buy_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_family_shop_buy_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	uint32_t back_code = family_manager_t::family_shop_buy_request(p_user, msg.index(), msg.shop_item(), msg.num());
	if (back_code != common::errcode_enum::error_ok) {
		proto::client::ec_family_shop_buy_reply reply;
		reply.set_reply_code(back_code);
		p_user->send_msg_to_client(op_cmd::ec_family_shop_buy_reply, reply);
	}

	return true;
}

bool family_msg_handle::handle_ge_family_shop_back_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_family_shop_back_notify);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	family_manager_t::family_shop_failed_back(p_user, msg.refresh_time(), msg.index(), msg.num());

	return true;
}


bool family_msg_handle::handle_ge_modify_family_money_request(const network::tcp_socket_ptr& s, const
	network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_modify_family_money_request);

	family_ptr p_family = family_manager_t::get_family(msg.family_id());
	if (NULL == p_family)
	{
		log_error("NULL == p_family[%lu]", msg.family_id());
		return false;
	}

	if (msg.type() == 0)
	{
		p_family->add_money(msg.num(), proto::server::family_money_change_type(msg.source()));
	}
	else
	{
		p_family->cost_money(msg.num(), proto::server::family_money_change_type(msg.source()));
	}
	return true;
}
