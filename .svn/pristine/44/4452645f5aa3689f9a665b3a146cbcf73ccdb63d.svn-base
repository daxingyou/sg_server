#include "field_boss_msg_handle.hpp"

#include "gate/gate_msg_handler.hpp"
#include "game/game_msg_handler.hpp"

#include "client.pb.h"
#include "server.pb.h"
#include "user/global_user_manager.hpp"
#include "field_boss_manager.hpp"
#include "utility.hpp"
#include "main/center_server_fwd.hpp"
#include "main/center_server.hpp"
#include "tblh/errcode_enum.hpp"

bool field_boss_msg_handle_t::init_gate_msg_handler()
{
	bool bresult = true;

	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_field_boss_info_request, handle_ce_field_boss_info_request);

	return bresult;
}

bool field_boss_msg_handle_t::init_game_msg_handler()
{
	bool bresult = true;

	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_summon_field_boss_replay, handle_ge_summon_field_boss_replay);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_field_boss_killed_notify, handle_ge_field_boss_killed_notify);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_field_boss_rank_notify, handle_ge_field_boss_rank_notify);

	return bresult;
}

bool field_boss_msg_handle_t::handle_ce_field_boss_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_field_boss_info_request);

	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_field_boss_info_notify notify;
	field_boss_manager_t::get_all_boss_info(notify);
	p_user->send_msg_to_client(op_cmd::ec_field_boss_info_notify, notify);

	return true;
}

bool field_boss_msg_handle_t::handle_ge_summon_field_boss_replay(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_summon_field_boss_replay);

	if (!field_boss_manager_t::summon_boss_result(msg.id(), msg.object_id(), msg.reply_code() == common::errcode_enum::error_ok)) {
		if (msg.reply_code() == common::errcode_enum::error_ok) {
			// 通知game 异常删除怪物
			proto::server::eg_field_boss_die_notify notify;
			notify.set_object_id(msg.object_id());
			notify.set_mode(proto::server::FIELD_BOSS_NONE);
			env::server->send_msg_to_game(s, op_cmd::eg_field_boss_die_notify, 0, notify);
		}
	}

	return true;
}

bool field_boss_msg_handle_t::handle_ge_field_boss_killed_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_field_boss_killed_notify);

	field_boss_manager_t::field_boss_killed(msg.id(), msg.object_id(), msg.killer_name());

	return true;
}

bool field_boss_msg_handle_t::handle_ge_field_boss_rank_notify(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_field_boss_rank_notify);

	field_boss_manager_t::field_boss_rank_reward(msg);

	return true;
}

