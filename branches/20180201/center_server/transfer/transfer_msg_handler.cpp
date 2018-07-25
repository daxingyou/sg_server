#include "transfer_msg_handler.hpp"
#include "main/center_server.hpp"
#include "main/center_server_fwd.hpp"
#include "protos_fwd.hpp"
#include "transfer/transfer_manager.hpp"
#include "user/global_user_manager.hpp"
#include "world_cup/world_cup_manager.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;


server_msg_handler_t<op_cmd::FE_BEGIN, op_cmd::FE_END, transfer_msg_handler_t::msg_handler_func> transfer_msg_handler_t::m_transfer_msg_handler;
server_msg_handler_t<op_cmd::RG_BEGIN, op_cmd::RG_END, transfer_msg_handler_t::msg_handler_func> transfer_msg_handler_t::m_area_2_game_msg_handler;
server_msg_handler_t<op_cmd::RE_BEGIN, op_cmd::RE_END, transfer_msg_handler_t::msg_handler_func> transfer_msg_handler_t::m_area_2_center_msg_handler;

bool transfer_msg_handler_t::init_msg_handler()
{
	bool ret = true;
	ret &= m_transfer_msg_handler.register_func(op_cmd::fe_syn_transfer_info_notify, handle_fe_syn_transfer_info_notify);
	ret &= m_transfer_msg_handler.register_func(op_cmd::fe_test_reply, handle_fe_test_reply);

	ret &= m_area_2_game_msg_handler.register_func(op_cmd::rg_arena_match_reply, handle_rg_arena_match_reply);
	ret &= m_area_2_game_msg_handler.register_func(op_cmd::rg_arena_battle_result_notify, handle_rg_arena_battle_result_notify);
	ret &= m_area_2_game_msg_handler.register_func(op_cmd::rg_get_arena_rank_data_reply, handle_rg_get_arena_rank_data_reply);
	ret &= m_area_2_game_msg_handler.register_func(op_cmd::rg_world_cup_play_reply, handle_rg_world_cup_play_reply);
	ret &= m_area_2_game_msg_handler.register_func(op_cmd::rg_world_cup_bet_reply, handle_rg_world_cup_bet_reply);
	ret &= m_area_2_game_msg_handler.register_func(op_cmd::rg_world_cup_bet_reward_reply, handle_rg_world_cup_bet_reward_reply);
	ret &= m_area_2_game_msg_handler.register_func(op_cmd::rg_arena_cancel_match_reply, handle_rg_arena_cancel_match_reply);
	ret &= m_area_2_game_msg_handler.register_func(op_cmd::rg_arena_self_rank_reply, handle_rg_arena_self_rank_reply);

	ret &= m_area_2_center_msg_handler.register_func(op_cmd::re_world_cup_data_reply, handle_re_world_cup_data_reply);
	ret &= m_area_2_center_msg_handler.register_func(op_cmd::re_world_cup_mail_notify, handle_re_world_cup_mail_notify);
	ret &= m_area_2_center_msg_handler.register_func(op_cmd::re_world_cup_coming_quarter_notify, handle_re_world_cup_coming_quarter_notify);
	ret &= m_area_2_center_msg_handler.register_func(op_cmd::re_world_cup_coming_soon_notify, handle_re_world_cup_coming_soon_notify);
	ret &= m_area_2_center_msg_handler.register_func(op_cmd::re_world_cup_ready_notify, handle_re_world_cup_ready_notify);
	ret &= m_area_2_center_msg_handler.register_func(op_cmd::re_world_cup_update_notify, handle_re_world_cup_update_notify);
	ret &= m_area_2_center_msg_handler.register_func(op_cmd::re_world_cup_bet_data_reply, handle_re_world_cup_bet_data_reply);
	ret &= m_area_2_center_msg_handler.register_func(op_cmd::re_world_cup_odds_reply, handle_re_world_cup_odds_reply);
	return ret;
}

bool transfer_msg_handler_t::handle_fe_syn_transfer_info_notify(const network::tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::fe_syn_transfer_info_notify);

	transfer_manager_t::set_transfer(s, msg.server_id());
	return true;
}

bool transfer_msg_handler_t::handle_fe_test_reply(const network::tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
{
    PRE_S2S_MSG(proto::server::fe_test_reply);
	
	uint32_t server_id = transfer_manager_t::get_transfer_id_by_socket(s);

	log_info("transfer_msg transfer_id[%d] test[%d] role_game_server_id[%d]", server_id, msg.test1(), msg.role_game_server_id());
	return true;
}

bool transfer_msg_handler_t::handle_rg_arena_match_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::rg_arena_match_reply);

	global_user_ptr p_role = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_role)
	{
		log_error("arena rg_arena_match_reply p_role[%lu] NULL", uid);
		return false;
	}
	p_role->send_msg_to_game(op_cmd::rg_arena_match_reply, msg);
	return true;
}

bool transfer_msg_handler_t::handle_rg_get_arena_rank_data_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::rg_get_arena_rank_data_reply);

	global_user_ptr p_role = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_role)
	{
		log_error("arena rg_get_arena_rank_data_reply p_role[%lu] NULL", uid);
		return false;
	}
	p_role->send_msg_to_game(op_cmd::rg_get_arena_rank_data_reply, msg);
	return true;
}


bool transfer_msg_handler_t::handle_rg_world_cup_play_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::rg_world_cup_play_reply);

	global_user_ptr p_role = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] NULL", uid);
		return false;
	}
	p_role->send_msg_to_game(op_cmd::rg_world_cup_play_reply, msg);
	return true;
}


bool transfer_msg_handler_t::handle_rg_world_cup_bet_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::rg_world_cup_bet_reply);

	global_user_ptr p_role = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] NULL", uid);
		return false;
	}
	p_role->send_msg_to_game(op_cmd::rg_world_cup_bet_reply, msg);
	return true;
}


bool transfer_msg_handler_t::handle_rg_world_cup_bet_reward_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::rg_world_cup_bet_reward_reply);

	global_user_ptr p_role = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] NULL", uid);
		return false;
	}
	p_role->send_msg_to_game(op_cmd::rg_world_cup_bet_reward_reply, msg);
	return true;
}

bool transfer_msg_handler_t::handle_rg_arena_cancel_match_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::rg_arena_cancel_match_reply);

	global_user_ptr p_role = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] NULL", uid);
		return false;
	}
	p_role->send_msg_to_game(op_cmd::rg_arena_cancel_match_reply, msg);
	return true;
}


bool transfer_msg_handler_t::handle_rg_arena_self_rank_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::rg_arena_self_rank_reply);

	global_user_ptr p_role = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] NULL", uid);
		return false;
	}
	p_role->send_msg_to_game(op_cmd::rg_arena_self_rank_reply, msg);
	return true;
}

bool transfer_msg_handler_t::handle_re_world_cup_data_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::re_world_cup_data_reply);

	world_cup_manager_t::load_data(msg.world_cup_info());
	return true;
}


bool transfer_msg_handler_t::handle_re_world_cup_mail_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::re_world_cup_mail_notify);
	
	return true;
}


bool transfer_msg_handler_t::handle_re_world_cup_coming_quarter_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::re_world_cup_coming_quarter_notify);
	global_user_ptr p_role = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_role)
	{
		//log_error("p_role[%lu] NULL", uid);
		return false;
	}
	proto::client::ec_world_cup_coming_quarter_notify ntf;
	ntf.set_time_remain(msg.time_remain());
	p_role->send_msg_to_client(op_cmd::ec_world_cup_coming_quarter_notify, ntf);
	return true;
}


bool transfer_msg_handler_t::handle_re_world_cup_coming_soon_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::re_world_cup_coming_soon_notify);
	global_user_ptr p_role = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_role)
	{
		//log_error("p_role[%lu] NULL", uid);
		return false;
	}
	proto::client::ec_world_cup_coming_soon_notify ntf;
	ntf.set_time_remain(msg.time_remain());
	p_role->send_msg_to_client(op_cmd::ec_world_cup_coming_soon_notify, ntf);
	return true;
}


bool transfer_msg_handler_t::handle_re_world_cup_ready_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::re_world_cup_ready_notify);
	global_user_ptr p_role = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_role)
	{
		//log_error("p_role[%lu] NULL", uid);
		return false;
	}
	proto::client::ec_world_cup_ready_notify ntf;
	ntf.mutable_battle_info()->CopyFrom(msg.battle_info());
	ntf.set_time_remain(msg.time_remain());
	p_role->send_msg_to_client(op_cmd::ec_world_cup_ready_notify, ntf);
	return true;
}


bool transfer_msg_handler_t::handle_re_world_cup_update_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::re_world_cup_update_notify);
	world_cup_manager_t::update_battle_info(msg.battle_info());
	return true;
}


bool transfer_msg_handler_t::handle_re_world_cup_bet_data_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::re_world_cup_bet_data_reply);

	global_user_ptr p_role = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] NULL", uid);
		return false;
	}
	proto::client::ec_world_cup_data_reply reply;
	world_cup_manager_t::peek_data(reply.mutable_world_cup_info(), msg);
	p_role->send_msg_to_client(op_cmd::ec_world_cup_data_reply, reply);
	return true;
}


bool transfer_msg_handler_t::handle_re_world_cup_odds_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::re_world_cup_odds_reply);

	global_user_ptr p_role = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] NULL", uid);
		return false;
	}
	proto::client::ec_world_cup_odds_reply reply;
	reply.mutable_odds_data()->CopyFrom(msg.odds_data());
	p_role->send_msg_to_client(op_cmd::ec_world_cup_odds_reply, reply);
	return true;
}

bool transfer_msg_handler_t::handle_rg_arena_battle_result_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::rg_arena_battle_result_notify);

	global_user_ptr p_role = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_role)
	{
		log_error("arena rg_arena_battle_result_notify p_role[%lu] NULL", uid);
		return false;
	}
	p_role->send_msg_to_game(op_cmd::rg_arena_battle_result_notify, msg);
	return true;
}
