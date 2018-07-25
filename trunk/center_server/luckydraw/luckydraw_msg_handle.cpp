#include "luckydraw_msg_handle.hpp"
#include "luckydraw_mgr.hpp"
#include "user/global_user.hpp"
#include "user/global_user_manager.hpp"
#include "gate/gate_msg_handler.hpp"
#include "game/game_msg_handler.hpp"
#include "tblh/errcode_enum.hpp"

bool luckydraw_msg_handle_t::init_gate_msg_handler()
{
	bool bresult = true;

	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_buy_wish_summon_request, handle_ce_buy_wish_summon_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_open_summon_wish_request, handle_ce_open_summon_wish_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_get_wish_summon_info_request, handle_ce_get_wish_summon_info_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_get_king_money_request, handle_ce_get_king_money_request);
	
	return bresult;
}

bool luckydraw_msg_handle_t::init_game_msg_handler()
{
	bool bresult = true;

	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_buy_wish_summon_reply, handle_ge_buy_wish_summon_reply);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_king_open_wish_summon_reply, handle_ge_king_open_wish_summon_reply);

	return bresult;
}

bool luckydraw_msg_handle_t::handle_ce_buy_wish_summon_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_buy_wish_summon_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::server::eg_buy_wish_summon_request req;
	uint32_t rlt = luckydraw_mgr_t::buy_wish_summon_req(p_user, msg.summon_id());
	req.set_summon_id(msg.summon_id());
	req.set_reply_code(rlt);
	p_user->send_msg_to_game(op_cmd::eg_buy_wish_summon_request, req);
	return true;
}

bool luckydraw_msg_handle_t::handle_ce_open_summon_wish_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_open_summon_wish_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::server::eg_king_open_wish_summon_request req;
	uint32_t rlt = luckydraw_mgr_t::open_king_wish_summon_req(p_user, msg.summon_id());
	req.set_summon_id(msg.summon_id());
	req.set_reply_code(rlt);
	p_user->send_msg_to_game(op_cmd::eg_king_open_wish_summon_request, req);
	return true;
}
bool luckydraw_msg_handle_t::handle_ce_get_wish_summon_info_request(const network::tcp_socket_ptr&s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_get_wish_summon_info_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_get_wish_summon_info_reply reply;
	luckydraw_mgr_t::get_wish_summon_info_request(p_user, reply);
	p_user->send_msg_to_client(op_cmd::ec_get_wish_summon_info_reply, reply);
	return true;
}


///////////////
bool luckydraw_msg_handle_t::handle_ge_buy_wish_summon_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_buy_wish_summon_reply);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	if (msg.reply_code() == errcode_enum::error_ok )
		luckydraw_mgr_t::on_buy_wish_summon(p_user, msg.summon_id());

	return true;
}

bool luckydraw_msg_handle_t::handle_ge_king_open_wish_summon_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_king_open_wish_summon_reply);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_open_summon_wish_reply reply;
	reply.set_summon_id(msg.summon_id());
	if (msg.reply_code() == errcode_enum::error_ok )
	{
		uint32_t reply_code = luckydraw_mgr_t::on_open_wish_summon(p_user, msg.summon_id(), reply);
		reply.set_reply_code(reply_code);
		reply.mutable_rcd()->CopyFrom(msg.rcd());
	}
	else
	{
		reply.set_reply_code(msg.reply_code());
	}

	p_user->send_msg_to_client(op_cmd::ec_open_summon_wish_reply, reply);
	return true;
}

bool luckydraw_msg_handle_t::handle_ce_get_king_money_request(const network::tcp_socket_ptr&s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_get_king_money_request);

	const global_user_ptr& p_user = global_user_manager_t::get_global_user_by_uid(uid);
	if (NULL == p_user) {
		log_error("NULL == p_user[%lu]", uid);
		return false;
	}

	proto::client::ec_get_king_money_reply reply;
	uint32_t reply_code = luckydraw_mgr_t::get_king_money(p_user);
	reply.set_reply_code(reply_code);
	p_user->send_msg_to_client(op_cmd::ec_get_king_money_reply, reply);

	return true;
}