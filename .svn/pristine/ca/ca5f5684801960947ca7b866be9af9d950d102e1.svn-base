#include "offline_fight_msg_handle.hpp"

#include "game/game_msg_handler.hpp"

#include "common.pb.h"
#include "client.pb.h"
#include "server.pb.h"

#include "user/global_user_manager.hpp"
#include "utility.hpp"
#include "main/center_server_fwd.hpp"
#include "main/center_server.hpp"
#include "offline_fight/offline_fight_manager.hpp"

USING_NS_COMMON;

bool offline_fight_msg_handle_t::init_game_msg_handler()
{
	bool bresult = true;

	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_arena_fight_data_notify, handle_ge_arena_fight_data_notify);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_expedition_fight_data_request, handle_ge_expedition_fight_data_request);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_expedition_refresh_request, handle_ge_expedition_refresh_request);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_expedition_refresh_stage_request, handle_ge_expedition_refresh_stage_request);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_expedition_help_request, handle_ge_expedition_help_request);
	return bresult;
}

bool offline_fight_msg_handle_t::handle_ge_arena_fight_data_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_arena_fight_data_notify);
	offline_fight_manager_t::update_fight_data(uid, msg.fight_value(), msg.mutable_fight_data());
	return true;
}

bool offline_fight_msg_handle_t::handle_ge_expedition_fight_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_expedition_fight_data_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}
	
	proto::server::eg_expedition_fight_data_reply reply;
	offline_fight_manager_t::make_fight_data(msg.level(), reply);
	p_user->send_msg_to_game(op_cmd::eg_expedition_fight_data_reply, reply);
	return true;
}

bool offline_fight_msg_handle_t::handle_ge_expedition_refresh_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_expedition_refresh_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::server::eg_expedition_refresh_reply reply;
	offline_fight_manager_t::refresh_fight_data(msg.level(), reply);
	p_user->send_msg_to_game(op_cmd::eg_expedition_refresh_reply, reply);
	return true;
}

bool offline_fight_msg_handle_t::handle_ge_expedition_refresh_stage_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_expedition_refresh_stage_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::server::eg_expedition_refresh_stage_reply reply;
	offline_fight_manager_t::refresh_stage_data(msg.level(), msg.stage_id(), reply);
	p_user->send_msg_to_game(op_cmd::eg_expedition_refresh_stage_reply, reply);
	return true;
}

bool offline_fight_msg_handle_t::handle_ge_expedition_help_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_expedition_help_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}
	std::vector<uint64_t> helper_list;
	for (int32_t i = 0; i < msg.helper_list_size(); ++i)
	{
		helper_list.push_back(msg.helper_list(i));
	}
	proto::server::eg_expedition_help_reply reply;
	uint32_t reply_code = offline_fight_manager_t::expedition_help(p_user, helper_list);
	reply.set_reply_code(reply_code);
	p_user->send_msg_to_game(op_cmd::eg_expedition_help_reply, reply);
	return true;
}
