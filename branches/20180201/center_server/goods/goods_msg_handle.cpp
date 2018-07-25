#include "goods_msg_handle.hpp"

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
#include "goods_manager.hpp"
#include "tblh/errcode_enum.hpp"

USING_NS_COMMON;

bool goods_msg_handle_t::init_client_msg_handler()
{
	bool bresult = true;

	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_goods_selling_list_request, handle_ce_goods_selling_list_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_goods_selling_data_request, handle_ce_goods_selling_data_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_goods_care_request, handle_ce_goods_care_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_goods_self_request, handle_ce_goods_self_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_goods_sell_info_request, handle_ce_goods_sell_info_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_goods_docare_request, handle_ce_goods_docare_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_goods_clear_care_request, handle_ce_goods_clear_care_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_goods_cancel_request, handle_ce_goods_cancel_request);
	return bresult;
}

bool goods_msg_handle_t::init_game_msg_handler()
{
	bool bresult = true;

	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_goods_buy_request, handle_ge_goods_buy_request);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_goods_sell_request, handle_ge_goods_sell_request);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_goods_return_request, handle_ge_goods_return_request);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_goods_resell_request, handle_ge_goods_resell_request);
	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_goods_profit_request, handle_ge_goods_profit_request);
	return bresult;

}


bool goods_msg_handle_t::handle_ce_goods_selling_list_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_goods_selling_list_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_goods_selling_list_reply reply;
	goods_manager_t::get_selling_list(msg.tab(), msg.item_type(), reply);
	p_user->send_msg_to_client(op_cmd::ec_goods_selling_list_reply, reply);
	return true;
}

bool goods_msg_handle_t::handle_ce_goods_selling_data_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_goods_selling_data_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_goods_selling_data_reply reply;
	goods_manager_t::get_selling_data(uid, msg.tab(), msg.rarity(), msg.item_tid(), reply);
	p_user->send_msg_to_client(op_cmd::ec_goods_selling_data_reply, reply);
	return true;
}

bool goods_msg_handle_t::handle_ce_goods_care_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_goods_care_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_goods_care_reply reply;
	goods_manager_t::get_care_goods(uid, msg.tab(), reply);
	p_user->send_msg_to_client(op_cmd::ec_goods_care_reply, reply);
	return true;
}

bool goods_msg_handle_t::handle_ce_goods_self_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_goods_self_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_goods_self_reply reply;
	goods_manager_t::get_self_goods(uid, reply);
	p_user->send_msg_to_client(op_cmd::ec_goods_self_reply, reply);
	return true;
}

bool goods_msg_handle_t::handle_ce_goods_sell_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_goods_sell_info_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_goods_sell_info_reply reply;
	reply.set_item_tid(msg.item_tid());
	reply.set_price(goods_manager_t::get_default_price(msg.item_tid()));
	p_user->send_msg_to_client(op_cmd::ec_goods_sell_info_reply, reply);
	return true;
}

bool goods_msg_handle_t::handle_ce_goods_docare_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_goods_docare_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}
	uint64_t goods_uid = string_util_t::string_to_uint64(msg.goods_uid());
	proto::client::ec_goods_docare_reply reply;
	uint32_t heat = 0;
	uint32_t reply_code = goods_manager_t::care_goods(uid, goods_uid, msg.operation(), heat);
	reply.set_reply_code(reply_code);
	reply.set_goods_uid(msg.goods_uid());
	reply.set_heat(heat);
	reply.set_care(msg.operation());
	p_user->send_msg_to_client(op_cmd::ec_goods_docare_reply, reply);
	return true;
}

bool goods_msg_handle_t::handle_ce_goods_clear_care_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_goods_clear_care_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_goods_clear_care_reply reply;
	goods_manager_t::clear_care(uid, reply);
	log_warn("reply goods_num[%d]", reply.goods_self_size());
	p_user->send_msg_to_client(op_cmd::ec_goods_clear_care_reply, reply);
	return true;
}

bool goods_msg_handle_t::handle_ce_goods_cancel_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_goods_cancel_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}
	uint64_t goods_uid = string_util_t::string_to_uint64(msg.goods_uid());
	proto::client::ec_goods_cancel_reply reply;
	reply.set_reply_code(goods_manager_t::cancel_goods(uid, p_user->get_name(), goods_uid));
	p_user->send_msg_to_client(op_cmd::ec_goods_cancel_reply, reply);
	return true;
}

bool goods_msg_handle_t::handle_ge_goods_buy_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_goods_buy_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::server::eg_goods_buy_reply reply;
	reply.set_buy_count(msg.buy_count());
	reply.set_price(msg.price());
	uint32_t reply_code = goods_manager_t::buy_goods(uid, p_user->get_name(), msg.goods_uid(), msg.item_tid(), msg.buy_count(), msg.price(), reply.mutable_item_info());
	reply.set_reply_code(reply_code);
	p_user->send_msg_to_game(op_cmd::eg_goods_buy_reply, reply);
	return true;
}

bool goods_msg_handle_t::handle_ge_goods_sell_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_goods_sell_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::server::eg_goods_sell_reply reply;
	reply.set_item_uid(msg.item_uid());
	reply.set_price(msg.price());
	reply.mutable_item_info()->CopyFrom(msg.item_info());
	uint32_t reply_code = goods_manager_t::sell_goods(uid, p_user->get_name(), msg.price(), msg.item_info());
	reply.set_reply_code(reply_code);
	p_user->send_msg_to_game(op_cmd::eg_goods_sell_reply, reply);
	return true;
}

bool goods_msg_handle_t::handle_ge_goods_return_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_goods_return_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::server::eg_goods_return_reply reply;
	uint32_t reply_code = goods_manager_t::return_goods(uid, p_user->get_name(), msg.goods_uid(), reply.mutable_item_info());
	reply.set_reply_code(reply_code);
	p_user->send_msg_to_game(op_cmd::eg_goods_return_reply, reply);
	return true;
}

bool goods_msg_handle_t::handle_ge_goods_resell_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_goods_resell_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::server::eg_goods_resell_reply reply;
	uint32_t reply_code = goods_manager_t::resell_goods(uid, p_user->get_name(), msg.goods_uid(), msg.sell_count(), msg.price());
	reply.set_reply_code(reply_code);
	p_user->send_msg_to_game(op_cmd::eg_goods_resell_reply, reply);
	return true;
}

bool goods_msg_handle_t::handle_ge_goods_profit_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_goods_profit_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::server::eg_goods_profit_reply reply;
	uint32_t profit = 0;
	uint32_t reply_code = goods_manager_t::profit_goods(uid, p_user->get_name(), profit);
	reply.set_reply_code(reply_code);
	reply.set_profit(profit);
	p_user->send_msg_to_game(op_cmd::eg_goods_profit_reply, reply);
	return true;
}
