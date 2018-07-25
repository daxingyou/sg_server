#include "friend_msg_handle.hpp"
#include "log.hpp"
#include "config/config_manager.hpp"
#include "gate/gate_msg_handler.hpp"
#include "client.pb.h"
#include "user/global_user.hpp"
#include "user/global_user_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "game/game_msg_handler.hpp"
#include "utility.hpp"
#include "friend_manager.hpp"
#include "redis/friend_data_mgr.hpp"
#include "redis/global_user_data_mgr.hpp"

bool friend_msg_handle_t::init_msg_handler()
{
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_get_friends_request,handle_ce_get_friends_request_s);
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_get_recent_friends_request,handle_ce_get_recent_friends_request_s);
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_get_black_friends_request,handle_ce_get_black_friends_request_s);
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_add_friend_request,handle_ce_add_friend_request_s);
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_approve_add_friend_request,handle_ce_add_all_application_request_s);
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_remove_friend_request,handle_ce_remove_friend_request_s);
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_black_friend_request,handle_ce_black_friend_request_s);
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_friend_search_user_by_id_request,handle_ce_friend_search_user_by_id_request_s);
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_friend_search_user_by_name_request,handle_ce_friend_search_user_by_name_request_s);
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_refresh_recommend_friends_list_request,handle_ce_refresh_recommend_friends_list_request_s);
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_top_friend_request,handle_ce_top_friend_request_s);
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_cancel_top_friend_request,handle_ce_cancel_top_friend_request_s);
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_clear_friends_request,handle_ce_clear_friends_request_s);
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_cancel_black_friend_request,handle_ce_cancel_black_friend_request_s);
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_clear_recent_friends_request,handle_ce_clear_recent_friends_request_s);
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_clear_black_friends_request,handle_ce_clear_black_friends_request_s);
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_clear_friend_appliers_request,handle_ce_clear_friend_appliers_request_s);
    gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_get_friend_applier_list_request, handle_ce_get_friend_applier_list_request_s);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_give_friend_point_request, handle_ce_give_friend_point_request_s);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_get_friend_point_request, handle_ce_get_friend_point_request_s);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_get_relation_award_request, handle_ce_get_relation_award_request_s);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_get_relation_award_data_request, handle_ce_get_relation_award_data_request_s);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_give_present_request, handle_ce_give_present_request_s);
	
	game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_get_friendliness_request, handle_ge_get_friendliness_request_s);
	game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_change_friendliness_request, handle_ge_change_friendliness_request_s);
	game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_friend_point_operation_reply, handle_ge_friend_point_operation_reply_s);
	return true;
}

bool friend_msg_handle_t::handle_ce_get_friends_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_get_friends_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_get_friends_reply reply;
	p_user->get_friend_mgr().get_friend_list(reply);
	p_user->send_msg_to_client(op_cmd::ec_get_friends_reply, reply);

	return true;
}

bool friend_msg_handle_t::handle_ce_add_friend_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_add_friend_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	uint64_t friend_uid = string_util_t::string_to_uint64(msg.friend_uid());
	if (friend_uid == 0)
	{
		log_error("user[%lu] friend_uid == 0", uid);
		return false;
	}

	proto::client::ec_add_friend_reply reply;
	p_user->get_friend_mgr().add_friend_by_uid(friend_uid, reply);
	// 错误给个名字，用于提示
	if (reply.reply_code() != errcode_enum::error_ok)
	{
		// 可能不在线，所以直接去redis读
		reply.set_error_name(global_user_data_mgr_t::get_name(friend_uid));
	}

	p_user->send_msg_to_client(op_cmd::ec_add_friend_reply, reply);

	return true;
}

bool friend_msg_handle_t::handle_ce_add_all_application_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_approve_add_friend_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}
	auto apply_list_copy = p_user->get_friend_mgr().get_apply_list_copy();
	apply_list_copy.sort(friend_apply_reverse_t());
	proto::client::ec_approve_add_friend_reply myreply;
	for(auto& list_member : apply_list_copy){
		if(list_member == NULL)
			continue;
		auto* p_reply = myreply.add_reply();
		p_user->get_friend_mgr().add_friend_by_uid(list_member->m_friend_id, *p_reply);
		// 错误给个名字，用于提示
		if (p_reply->reply_code() != errcode_enum::error_ok)
		{
			// 可能不在线，所以直接去redis读
			p_reply->set_error_name(global_user_data_mgr_t::get_name(list_member->m_friend_id));
		}
		if(p_reply->reply_code() == errcode_enum::friend_error_excess_max_friend_limit)
			break;
	}
	p_user->send_msg_to_client(op_cmd::ec_approve_add_friend_reply, myreply);
	return true;
}

bool friend_msg_handle_t::handle_ce_remove_friend_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_remove_friend_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	uint64_t friend_uid = string_util_t::string_to_uint64(msg.friend_uid());
	if (friend_uid == 0)
	{
		log_error("user[%lu] friend_uid == 0", uid);
		return false;
	}

	proto::client::ec_remove_friend_reply reply;
	p_user->get_friend_mgr().delete_friend_by_uid(friend_uid, reply);
	p_user->send_msg_to_client(op_cmd::ec_remove_friend_reply, reply);

	return true;
}

bool friend_msg_handle_t::handle_ce_black_friend_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_black_friend_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	uint64_t friend_uid = string_util_t::string_to_uint64(msg.friend_uid());
	if (friend_uid == 0)
	{
		log_error("user[%lu] friend_uid == 0", uid);
		return false;
	}

	proto::client::ec_black_friend_reply reply;
	p_user->get_friend_mgr().black_friend_by_uid(friend_uid, reply);
	p_user->send_msg_to_client(op_cmd::ec_black_friend_reply, reply);

	return true;
}

bool friend_msg_handle_t::handle_ce_friend_search_user_by_id_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_friend_search_user_by_id_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	uint32_t search_id = string_util_t::string_to_uint32(msg.uid());
	if (search_id == 0)
	{
		log_error("user[%lu] search_id == 0", uid);
		return false;
	}

	proto::client::ec_friend_search_user_by_id_reply reply;
	if (!friend_data_mgr_t::search_user_info_by_did(search_id, reply.mutable_info()))
	{
		reply.set_reply_code(errcode_enum::friend_error_id);
	}
	else
	{
		reply.set_reply_code(errcode_enum::error_ok);
	}

	p_user->send_msg_to_client(op_cmd::ec_friend_search_user_by_id_reply, reply);

	return true;
}

bool friend_msg_handle_t::handle_ce_friend_search_user_by_name_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_friend_search_user_by_name_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_friend_search_user_by_name_reply reply;
	if (!friend_data_mgr_t::search_user_info_by_name(msg.name(), reply.mutable_info()))
	{
		reply.set_reply_code(errcode_enum::friend_error_id);
	}
	else
	{
		reply.set_reply_code(errcode_enum::error_ok);
	}

	p_user->send_msg_to_client(op_cmd::ec_friend_search_user_by_name_reply, reply);

	return true;
}

bool friend_msg_handle_t::handle_ce_refresh_recommend_friends_list_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_refresh_recommend_friends_list_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_refresh_recommend_friends_list_reply reply;
	p_user->get_friend_mgr().get_recommend_friends(reply);
	reply.set_reply_code(errcode_enum::error_ok);
	p_user->send_msg_to_client(op_cmd::ec_refresh_recommend_friends_list_reply, reply);

	return true;
}

bool friend_msg_handle_t::handle_ce_top_friend_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_top_friend_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	uint64_t friend_uid = string_util_t::string_to_uint64(msg.friend_uid());
	if (friend_uid == 0)
	{
		log_error("user[%lu] friend_uid == 0", uid);
		return false;
	}

	proto::client::ec_top_friend_reply reply;
	p_user->get_friend_mgr().top_friend_by_uid(friend_uid, reply);
	p_user->send_msg_to_client(op_cmd::ec_top_friend_reply, reply);
	return true;
}


bool friend_msg_handle_t::handle_ce_cancel_top_friend_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_cancel_top_friend_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	uint64_t friend_uid = string_util_t::string_to_uint64(msg.friend_uid());
	if (friend_uid == 0)
	{
		log_error("user[%lu] friend_uid == 0", uid);
		return false;
	}

	proto::client::ec_cancel_top_friend_reply reply;
	p_user->get_friend_mgr().cancel_top_friend_by_uid(friend_uid, reply);
	p_user->send_msg_to_client(op_cmd::ec_cancel_top_friend_reply, reply);
	return true;
}


bool friend_msg_handle_t::handle_ce_clear_friends_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_clear_friends_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_clear_friends_reply reply;
	p_user->get_friend_mgr().clear_friends_not_login_long_time(reply);
	reply.set_reply_code(errcode_enum::error_ok);
	p_user->send_msg_to_client(op_cmd::ec_clear_friends_reply, reply);

	return true;
}


bool friend_msg_handle_t::handle_ce_clear_recent_friends_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_clear_recent_friends_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_clear_recent_friends_reply reply;
	p_user->get_friend_mgr().clear_recent_contact_friends();
	reply.set_reply_code(errcode_enum::error_ok);
	p_user->send_msg_to_client(op_cmd::ec_clear_recent_friends_reply, reply);

	return true;
}


bool friend_msg_handle_t::handle_ce_clear_black_friends_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_clear_black_friends_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_clear_black_friends_reply reply;
	p_user->get_friend_mgr().clear_all_black_friends();
	reply.set_reply_code(errcode_enum::error_ok);
	p_user->send_msg_to_client(op_cmd::ec_clear_black_friends_reply, reply);

	return true;
}


bool friend_msg_handle_t::handle_ce_cancel_black_friend_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_cancel_black_friend_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	uint64_t friend_uid = string_util_t::string_to_uint64(msg.friend_uid());
	if (friend_uid == 0)
	{
		log_error("user[%lu] friend_uid == 0", uid);
		return false;
	}

	proto::client::ec_cancel_black_friend_reply reply;
	p_user->get_friend_mgr().cancel_black_friend_by_uid(friend_uid, reply);
	p_user->send_msg_to_client(op_cmd::ec_cancel_black_friend_reply, reply);

	return true;
}


bool friend_msg_handle_t::handle_ce_get_recent_friends_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_get_recent_friends_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_get_recent_friends_reply reply;
	p_user->get_friend_mgr().get_recent_friends_list(reply);
	p_user->send_msg_to_client(op_cmd::ec_get_recent_friends_reply, reply);

	return true;
}


bool friend_msg_handle_t::handle_ce_get_black_friends_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_get_black_friends_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_get_black_friends_reply reply;
	p_user->get_friend_mgr().get_black_friends_list(reply);
	p_user->send_msg_to_client(op_cmd::ec_get_black_friends_reply, reply);

	return true;
}


bool friend_msg_handle_t::handle_ce_approve_add_friend_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	log_error("ce_approve_add_friend_request is invalid");

	return true;
}

bool friend_msg_handle_t::handle_ce_get_friend_applier_list_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_get_friend_applier_list_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_get_friend_applier_list_reply reply;
	p_user->get_friend_mgr().get_apply_friends_list(reply);
	p_user->send_msg_to_client(op_cmd::ec_get_friend_applier_list_reply, reply);

	return true;
}


bool friend_msg_handle_t::handle_ce_clear_friend_appliers_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_clear_friend_appliers_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_clear_friend_appliers_reply reply;
	p_user->get_friend_mgr().clear_all_apply_friends();
	reply.set_reply_code(errcode_enum::error_ok); 
	p_user->send_msg_to_client(op_cmd::ec_clear_friend_appliers_reply, reply);

	return true;
}

bool friend_msg_handle_t::handle_ge_get_friendliness_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_get_friendliness_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}
	if (p_user->get_friend_mgr().has_data_loaded())
	{
		p_user->get_friend_mgr().send_game_my_friendliness_data();
	}
	else
		p_user->get_friend_mgr().m_game_requested = true;

	return true;
	
}

bool friend_msg_handle_t::handle_ge_change_friendliness_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_change_friendliness_request);

	friend_mgr_t::change_friendliness_and_save_data(msg.my_role_id(), msg.friend_role_id(), msg.change_value(), (FRIENDLINESS_CHANGE_REASON)msg.reason());

	return true;
}

bool friend_msg_handle_t::handle_ge_friend_point_operation_reply_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_friend_point_operation_reply);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(msg.role_id());
	if (NULL == p_user)
	{
		return false;
	}

	switch (msg.operation()) 
	{
	case proto::server::friend_point_operation::player_get_point_from_friend:
	{
		p_user->get_friend_mgr().get_friend_point_from_friend_game_callback(msg.friend_role_id(), (uint32_t)msg.parameter1(), msg.reply_code(), &msg.change_data());
	}
	break;
	case proto::server::friend_point_operation::player_get_relation_award:
		p_user->get_friend_mgr().get_relation_award_callback(msg.parameter2(), msg.reply_code(), &msg.change_data());
	break;
	case proto::server::friend_point_operation::player_give_present:
		p_user->get_friend_mgr().give_present_callback(msg.friend_role_id(), msg.reply_code(), &msg.change_data(), msg.parameter1(), msg.parameter2(), msg.info1(), msg.info2(), msg.info3(), msg.info4());
	break;
	}

	return true;
}

bool friend_msg_handle_t::handle_ce_give_friend_point_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_give_friend_point_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}
	uint64_t friend_uid = string_util_t::string_to_uint64(msg.friend_uid());
	p_user->get_friend_mgr().give_friend_friend_point(friend_uid);
	return true;
}

bool friend_msg_handle_t::handle_ce_get_friend_point_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_get_friend_point_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}
	
	uint64_t friend_uid = string_util_t::string_to_uint64(msg.friend_uid());
	int32_t err_code = p_user->get_friend_mgr().get_friend_point_from_friend(friend_uid);
	if(err_code != 0)
	{
		p_user->get_friend_mgr().send_msg_to_client_of_get_friend_point_reply(friend_uid, err_code, 0, NULL);
	}
	return true;
}

bool friend_msg_handle_t::handle_ce_get_relation_award_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_get_relation_award_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	p_user->get_friend_mgr().get_relation_award(msg.level());
	return true;
}

bool friend_msg_handle_t::handle_ce_get_relation_award_data_request_s(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_get_relation_award_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}
	p_user->get_friend_mgr().tell_client_data_on_relation_interface();
	return true;
}

bool friend_msg_handle_t::handle_ce_give_present_request_s(const network::tcp_socket_ptr & s, const network::msg_buf_ptr & msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_give_present_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}
	p_user->get_friend_mgr().give_present(string_util_t::string_to_uint64(msg.target_role_uid()), msg.item_id(), msg.item_quantity(), msg.confirm());
	return false;
}
