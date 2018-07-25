#include "shop_msg_handle.hpp"

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
#include "shop_manager.hpp"
#include "tblh/errcode_enum.hpp"

bool shop_msg_handle_t::init_gate_msg_handler()
{
	bool bresult = true;

	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_shop_goods_request, handle_ce_shop_goods_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::ce_buy_goods_request, handle_ce_buy_goods_request);

	return bresult;
}

bool shop_msg_handle_t::init_game_msg_handler()
{
	bool bresult = true;

	bresult &= game_msg_handler_t::m_game_msg_handle.register_func(op_cmd::ge_check_user_buy_goods_reply, handle_ge_check_user_buy_goods_reply);

	return bresult;
}

bool shop_msg_handle_t::handle_ce_shop_goods_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_shop_goods_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}

	proto::client::ec_shop_goods_reply reply;
	reply.set_type(msg.type());
	bool result = true;
	result &= shop_manager_t::get_shop_list_by_type(msg.type(), reply);

//	log_warn("time_now ...............[%u]", common::time_util_t::now_time());
//	for (int i = 0; i < reply.shop_list().goods_list_size(); ++i)
//	{
//		log_warn("goods[%u] buy_value[%u] start_time[%u] end_time[%u] refresh_time[%u]", reply.shop_list().goods_list(i).goods_id(),
//			reply.shop_list().goods_list(i).buy_count(),
//			reply.shop_list().goods_list(i).start_time(),
//			reply.shop_list().goods_list(i).end_time(),
//			reply.shop_list().goods_list(i).last_refresh_time());
//	}
	result &= p_user->get_shop_user_data().get_shop_list_by_type(msg.type(), reply);

	if (result)
	{
		reply.set_reply_code(common::errcode_enum::error_ok);
	}
	else
	{
		reply.set_reply_code(common::errcode_enum::notice_shop_error1);
	}

	p_user->send_msg_to_client(op_cmd::ec_shop_goods_reply, reply);

	return true;
}

bool shop_msg_handle_t::handle_ce_buy_goods_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::ce_buy_goods_request);
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(uid);

	if (NULL == p_user)
	{
		log_error("user[%lu] null error", uid);
		return false;
	}
	proto::client::ec_buy_goods_reply reply;
	uint32_t back_code = shop_manager_t::user_want_buy_goods(p_user, msg.info(), reply);
	if (back_code != common::errcode_enum::error_ok)
	{
		reply.set_reply_code(back_code);
		p_user->send_msg_to_client(op_cmd::ec_buy_goods_reply, reply);
	}

	return true;
}

bool shop_msg_handle_t::handle_ge_check_user_buy_goods_reply(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::ge_check_user_buy_goods_reply);

	uint64_t user_id = msg.uid();
	global_user_ptr p_user = global_user_manager_t::get_global_user_by_uid(user_id);

	// 如果人物不存在
	if (NULL == p_user)
	{
		log_error("user[%lu] null error", user_id);
		return false;
	}
	proto::server::eg_give_user_goods_notify notify;
	uint32_t back_code = shop_manager_t::user_game_check_back(p_user, msg.info(), notify);
	notify.set_uid(p_user->get_uid());
	notify.set_reply_code(back_code);
	proto::common::user_buy_goods* goods_info = notify.mutable_info();
	if (goods_info != NULL)
	{
		goods_info->CopyFrom(msg.info());
	}

	proto::common::role_change_data* data = notify.mutable_change_data();
	if (data != NULL)
	{
		data->CopyFrom(msg.change_data());
	}

	p_user->send_msg_to_game(op_cmd::eg_give_user_goods_notify, notify);

	return true;
}
