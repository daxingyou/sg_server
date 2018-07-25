#include "challenge_msg_handle.hpp"

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
#include "challenge_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "redis/friend_data_mgr.hpp"

USING_NS_COMMON;

bool challenge_msg_handle_t::init_client_msg_handler()
{
	bool bresult = true;

	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_challenge_request, handle_ce_challenge_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_challenge_record_request, handle_ce_challenge_record_request);

	return bresult;
}

bool challenge_msg_handle_t::init_game_msg_handler()
{
	bool bresult = true;

	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_challenge_request, handle_ge_challenge_request);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_challenge_process_request, handle_ge_challenge_process_request);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_challenge_fight_reply, handle_ge_challenge_fight_reply);
	return bresult;
}

bool challenge_msg_handle_t::handle_ce_challenge_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_challenge_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	uint64_t target_uid = 0;
	std::string target_name;
	if (msg.has_target_uid())
	{
		target_uid = string_util_t::string_to_uint64(msg.target_uid());
	}
	if (msg.has_target_name())
	{
		target_name = msg.target_name();
	}
	proto::client::ec_challenge_reply reply;
	
	if (friend_data_mgr_t::search_user_info_by_id(target_uid, reply.mutable_target_info()))
	{
		reply.set_reply_code(0);
	}
	else if (friend_data_mgr_t::search_user_info_by_name(target_name, reply.mutable_target_info()))
	{
		reply.set_reply_code(0);
	}
	else
	{
		reply.set_reply_code(errcode_enum::notice_rt_no_target);
	}
	
	p_user->send_msg_to_client(op_cmd::ec_challenge_reply, reply);

	return true;
}

bool challenge_msg_handle_t::handle_ce_challenge_record_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_challenge_record_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_challenge_record_reply reply;
	challenge_manager_t::peek_challenge_record(reply);
	p_user->send_msg_to_client(op_cmd::ec_challenge_record_reply, reply);
	return true;
}

bool challenge_msg_handle_t::handle_ge_challenge_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_challenge_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}
	proto::server::eg_challenge_reply reply;
	uint32_t reply_code = challenge_manager_t::challenge_request(p_user, msg.target_uid());
	reply.set_reply_code(reply_code);
	p_user->send_msg_to_game(op_cmd::eg_challenge_reply, reply);

	return true;
}

bool challenge_msg_handle_t::handle_ge_challenge_process_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_challenge_process_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}
	proto::server::eg_challenge_process_reply reply;
	uint32_t reply_code = challenge_manager_t::challenge_process_request(p_user, msg.process());
	reply.set_reply_code(reply_code);
	p_user->send_msg_to_game(op_cmd::eg_challenge_process_reply, reply);

	return true;
}

bool challenge_msg_handle_t::handle_ge_challenge_fight_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_challenge_fight_reply);
	challenge_manager_t::challenge_fight_result(uid, msg.result());
	return true;
}
