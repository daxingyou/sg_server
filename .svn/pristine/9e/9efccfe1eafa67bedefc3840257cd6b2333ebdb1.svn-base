#include "shop_msg_handle.hpp"

#include "common.pb.h"
#include "server.pb.h"
#include "client.pb.h"

#include "gate/gate_msg_handler.hpp"
#include "center/center_msg_handler.hpp"
#include "main/game_server_fwd.hpp"
#include "main/game_server.hpp"
#include "role/role_manager.hpp"
#include "shop_manager.hpp"
#include "tblh/errcode_enum.hpp"

bool shop_msg_handle_t::init_center_msg_handler()
{
	bool bresult = true;

	bresult &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_check_user_buy_goods_request, handle_eg_check_user_buy_goods_request);
	bresult &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_give_user_goods_notify, handle_eg_give_user_goods_notify);

	bresult &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_goods_buy_reply, handle_eg_goods_buy_reply);
	bresult &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_goods_sell_reply, handle_eg_goods_sell_reply);
	bresult &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_goods_return_reply, handle_eg_goods_return_reply);
	bresult &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_goods_resell_reply, handle_eg_goods_resell_reply);
	bresult &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_goods_profit_reply, handle_eg_goods_profit_reply);
	return bresult;
}

bool shop_msg_handle_t::init_gate_msg_handler()
{
	bool bresult = true;

	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_buy_goods_request, handle_cg_buy_goods_request);

	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_goods_buy_request, handle_cg_goods_buy_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_goods_sell_request, handle_cg_goods_sell_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_goods_return_request, handle_cg_goods_return_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_goods_resell_request, handle_cg_goods_resell_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_goods_profit_request, handle_cg_goods_profit_request);

	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_random_shop_info_request, handle_cg_random_shop_info_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_random_shop_buy_request, handle_cg_random_shop_buy_request);
	bresult &= gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_random_shop_refresh_request, handle_cg_random_shop_refresh_request);

	return bresult;
}

bool shop_msg_handle_t::handle_eg_check_user_buy_goods_request(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_check_user_buy_goods_request);

	uint64_t role_id = msg.uid();

	role_ptr p_role = role_manager_t::find_role(role_id);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", role_id);
		return false;
	}

	proto::server::ge_check_user_buy_goods_reply center_reply;

	uint32_t back_code = shop_manager_t::check_user_buy_goods(p_role, msg.info(), em_goods_check_type::goods_check_type_center, center_reply.mutable_change_data());
	if (back_code != common::errcode_enum::error_ok)
	{
		proto::client::gc_buy_goods_reply reply;
		reply.set_reply_code(back_code);
		p_role->send_msg_to_client(op_cmd::gc_buy_goods_reply, reply);
		return true;
	}


	// 通知center可以购买
	center_reply.set_uid(role_id);
	proto::common::user_buy_goods* info = center_reply.mutable_info();
	if (info != NULL)
	{
		info->CopyFrom(msg.info());
	}

	if (env::server == NULL)
	{
		log_error("env::server null error");
		return false;
	}

	env::server->send_msg_to_center(op_cmd::ge_check_user_buy_goods_reply, 0, center_reply);

	return true;
}

bool shop_msg_handle_t::handle_eg_give_user_goods_notify(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_give_user_goods_notify);

	uint64_t role_id = msg.uid();

	role_ptr p_role = role_manager_t::find_role(role_id);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", role_id);
		return false;
	}

	proto::client::gc_buy_goods_reply reply;
	
	// center 验证不通过 把消耗的钱还给玩家
	if (msg.reply_code() != common::errcode_enum::error_ok)
	{
		log_error("p_role[%lu] check error, back_code[%u]", role_id, msg.reply_code());
		reply.set_reply_code(msg.reply_code());

		shop_manager_t::back_role_money(p_role, msg.info());
	}
	else
	{
		proto::common::role_change_data* data = reply.mutable_change_data();
		if (data != NULL)
		{
			data->CopyFrom(msg.change_data());
			// 把道具发给玩家
			uint32_t back_code = shop_manager_t::give_role_item(p_role, msg.info(), data);
			reply.set_reply_code(back_code);

			if (msg.has_shop_goods_data())
			{
				proto::common::shop_goods* shop_goods_data = reply.mutable_shop_goods_data();
				if (shop_goods_data != NULL)
				{
					shop_goods_data->CopyFrom(msg.shop_goods_data());
				}
			}

			if (msg.has_user_goods_data())
			{
				proto::common::user_goods* user_goods_data = reply.mutable_user_goods_data();
				if (user_goods_data != NULL)
				{
					user_goods_data->CopyFrom(msg.user_goods_data());
				}
			}
		}
	}

	p_role->send_msg_to_client(op_cmd::gc_buy_goods_reply, reply);

	return true;
}

bool shop_msg_handle_t::handle_cg_buy_goods_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_buy_goods_request);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	proto::client::gc_buy_goods_reply reply;
	uint32_t back_code = shop_manager_t::check_user_buy_goods(p_role, msg.info(), em_goods_check_type::goods_check_type_game, reply.mutable_change_data());
	reply.set_reply_code(back_code);
	proto::common::role_change_data* p_rcd = reply.mutable_change_data();
	if (p_rcd) {
		p_role->get_personal_info(p_rcd->mutable_per_info());
	}

	p_role->send_msg_to_client(op_cmd::gc_buy_goods_reply, reply);

	return true;
}

bool shop_msg_handle_t::handle_cg_goods_buy_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_goods_buy_request);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}
	uint64_t goods_uid = string_util_t::string_to_uint64(msg.goods_uid());
	uint32_t reply_code = shop_manager_t::goods_buy_request(p_role, goods_uid, msg.item_tid(), msg.buy_count(), msg.price());
	if (errcode_enum::error_ok != reply_code)
	{
		proto::client::gc_goods_buy_reply reply;
		reply.set_reply_code(reply_code);
		p_role->send_msg_to_client(op_cmd::gc_goods_buy_reply, reply);
	}
	return true;
}

bool shop_msg_handle_t::handle_eg_goods_buy_reply(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_goods_buy_reply);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	proto::client::gc_goods_buy_reply reply;

	shop_manager_t::goods_buy_reply(p_role, msg, reply);
	proto::common::role_change_data* p_rcd = reply.mutable_rcd();
	if (p_rcd) {
		p_role->get_personal_info(p_rcd->mutable_per_info());
	}
	p_role->send_msg_to_client(op_cmd::gc_goods_buy_reply, reply);
	return true;
}

bool shop_msg_handle_t::handle_cg_goods_sell_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_goods_sell_request);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}
	uint64_t item_uid = string_util_t::string_to_uint64(msg.item_uid());
	uint32_t reply_code = shop_manager_t::goods_sell_request(p_role, item_uid, msg.sell_count(), msg.price());
	if (errcode_enum::error_ok != reply_code)
	{
		proto::client::gc_goods_sell_reply reply;
		reply.set_reply_code(reply_code);
		p_role->send_msg_to_client(op_cmd::gc_goods_sell_reply, reply);
	}
	return true;
}

bool shop_msg_handle_t::handle_eg_goods_sell_reply(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_goods_sell_reply);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	proto::client::gc_goods_sell_reply reply;

	shop_manager_t::goods_sell_reply(p_role, msg, reply);
	p_role->send_msg_to_client(op_cmd::gc_goods_sell_reply, reply);
	return true;
}

bool shop_msg_handle_t::handle_cg_goods_return_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_goods_return_request);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}
	uint64_t goods_uid = string_util_t::string_to_uint64(msg.goods_uid());
	uint32_t reply_code = shop_manager_t::goods_return_request(p_role, goods_uid, msg.item_tid(), msg.item_num());
	if (errcode_enum::error_ok != reply_code)
	{
		proto::client::gc_goods_return_reply reply;
		reply.set_reply_code(reply_code);
		p_role->send_msg_to_client(op_cmd::gc_goods_return_reply, reply);
	}
	return true;
}

bool shop_msg_handle_t::handle_eg_goods_return_reply(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_goods_return_reply);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	proto::client::gc_goods_return_reply reply;
	reply.set_reply_code(msg.reply_code());
	if (msg.reply_code() == errcode_enum::error_ok)
	{
		shop_manager_t::goods_return_reply(p_role, msg.item_info(), reply.mutable_rcd());
	}
	p_role->send_msg_to_client(op_cmd::gc_goods_return_reply, reply);
	return true;
}

bool shop_msg_handle_t::handle_cg_goods_resell_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_goods_resell_request);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}
	uint64_t goods_uid = string_util_t::string_to_uint64(msg.goods_uid());
	uint32_t reply_code = shop_manager_t::goods_resell_request(p_role, goods_uid, msg.sell_count(), msg.price());
	if (errcode_enum::error_ok != reply_code)
	{
		proto::client::gc_goods_resell_reply reply;
		reply.set_reply_code(reply_code);
		p_role->send_msg_to_client(op_cmd::gc_goods_resell_reply, reply);
	}
	return true;
}

bool shop_msg_handle_t::handle_eg_goods_resell_reply(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_goods_resell_reply);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	proto::client::gc_goods_resell_reply reply;
	shop_manager_t::goods_resell_reply(p_role, msg, reply);
	p_role->send_msg_to_client(op_cmd::gc_goods_resell_reply, reply);
	return true;
}

bool shop_msg_handle_t::handle_cg_goods_profit_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_goods_profit_request);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}
	shop_manager_t::goods_profit_request(uid);
	return true;
}

bool shop_msg_handle_t::handle_eg_goods_profit_reply(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_goods_profit_reply);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	proto::client::gc_goods_profit_reply reply;
	reply.set_reply_code(msg.reply_code());
	if (msg.reply_code() == errcode_enum::error_ok)
	{
		shop_manager_t::goods_profit_reply(p_role, msg.profit(), reply.mutable_rcd());
	}
	p_role->send_msg_to_client(op_cmd::gc_goods_profit_reply, reply);
	return true;
}

bool shop_msg_handle_t::handle_cg_random_shop_info_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_random_shop_info_request);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	proto::client::gc_random_shop_info_reply reply;
	p_role->get_random_shop_data().get_random_shop_info(p_role, msg.shop_id(), reply);
	p_role->send_msg_to_client(op_cmd::gc_random_shop_info_reply, reply);

	return true;
}

bool shop_msg_handle_t::handle_cg_random_shop_buy_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_random_shop_buy_request);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	proto::client::gc_random_shop_buy_reply reply;
	p_role->get_random_shop_data().buy_random_shop(p_role, msg.shop_id(), msg.item_index(), reply);
	p_role->send_msg_to_client(op_cmd::gc_random_shop_buy_reply, reply);

	return true;
}

bool shop_msg_handle_t::handle_cg_random_shop_refresh_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_random_shop_refresh_request);

	role_ptr p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("p_role[%lu] is null ", uid);
		return false;
	}

	proto::client::gc_random_shop_refresh_reply reply;
	p_role->get_random_shop_data().role_refresh_random_shop(p_role, msg.shop_id(), reply);
	p_role->send_msg_to_client(op_cmd::gc_random_shop_refresh_reply, reply);

	return true;
}
