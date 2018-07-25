#include "item_msg_handle.hpp"
#include "item_manager.hpp"
#include "common/log_enum.hpp"
#include "role/role.hpp"
#include "config/config_manager.hpp"
#include "tblh/errcode_enum.hpp"
#include "item/drop_manager.hpp"
#include "item/item_manager.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

bool item_msg_handle_t::init_msg_handler()
{
	//gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_create_item_request, handle_cg_create_item_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_use_item_request, handle_cg_use_item_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_sell_bag_item_request, handle_cg_sell_bag_item_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_merage_item_request, handle_cg_merage_item_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_sell_hero_chip_request, handle_cg_sell_hero_chip_request);
    return true;
}

bool item_msg_handle_t::init_center_msg_handler()
{
	bool ret = true;
	ret &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_check_drop_items_reply, handle_eg_check_drop_items_reply);
	ret &= center_msg_handler_t::m_center_msg_handle.register_func(op_cmd::eg_syn_item_limit_notify, handle_eg_syn_item_limit_notify);

	return ret;
}

//
//bool item_msg_handle_t::handle_cg_create_item_request(const network::tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
//{
//    PRE_S2S_MSG(proto::client::cg_create_item_request);
//
//    item_manager_t::create_item(uid, msg.item_tid(), msg.add_num(), log_enum::source_type_gm_create_item, true);
//
//    return true;
//}

bool item_msg_handle_t::handle_cg_use_item_request(const network::tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_use_item_request);
	auto role = role_manager_t::find_role(uid);
	if (NULL == role)
	{
		log_error("[wys] role[%lu] NULL", uid);
		return false;
	}

	proto::client::gc_use_item_reply reply;
	proto::common::role_change_data *p_data = reply.mutable_rcd();
	uint64_t item_uid = string_util_t::string_to_uint64(msg.item_uid());

	uint32_t item_tid = 0;
	uint32_t reply_code = item_manager_t::use_item(role, item_uid, msg.cost_num(), item_tid, p_data);
	
	if (reply_code == errcode_enum::notice_item_type_not_match)
	{
		//客户端要求藏宝道具不要返回道具使用的消息
		return true;
	}
	reply.set_use_item_tid(item_tid);
	reply.set_reply_code(reply_code);
	role->send_msg_to_client(op_cmd::gc_use_item_reply, reply);
	if (reply_code == errcode_enum::error_ok)
	{
		role->script_event_use_item(item_tid);
	}
    return true;
}

bool item_msg_handle_t::handle_cg_sell_bag_item_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_sell_bag_item_request);
	auto role = role_manager_t::find_role(uid);
	if (NULL == role)
	{
		log_error("[wys] role[%lu] NULL", uid);
		return false;
	}
	proto::client::gc_sell_bag_item_reply reply;

	uint64_t item_uid = string_util_t::string_to_uint64(msg.item_uid());
	uint32_t reply_code = item_manager_t::sell_bag_item(role, item_uid, msg.item_num(), reply.mutable_role_data());

	reply.set_item_uid(msg.item_uid());
	reply.set_reply_code(reply_code);
	role->send_msg_to_client(op_cmd::gc_sell_bag_item_reply, reply);
	return true;
}

bool item_msg_handle_t::handle_eg_check_drop_items_reply(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_check_drop_items_reply);
	
	for (int32_t i = 0; i < msg.item_list_size(); ++i)
	{
		item_manager_t::update_item_limit(msg.item_list(i).item_tid(), msg.item_list(i).item_num(), msg.item_list(i).last_give_time());
	}

	return true;
}


bool item_msg_handle_t::handle_eg_syn_item_limit_notify(const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::server::eg_syn_item_limit_notify);

	item_manager_t::load_item_limit_from_center(msg);

	return true;
}

bool item_msg_handle_t::handle_cg_merage_item_request(const network::tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_merage_item_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("[wys] role[%lu] NULL", uid);
		return false;
	}

	proto::client::gc_merage_item_reply reply;
	proto::common::role_change_data *p_data = reply.mutable_rcd();
	uint32_t item_tid = msg.item_tid();
	uint32_t merage_type = msg.mearge_type();
	uint32_t merage_num = 1;
	if (merage_type == 2)
	{
		merage_num = 5;
	}

	uint32_t reply_code = item_manager_t::merage_item(p_role, item_tid, merage_num, p_data);
	reply.set_reply_code(reply_code);
	p_role->send_msg_to_client(op_cmd::gc_merage_item_reply, reply);
	return true;
}

bool item_msg_handle_t::handle_cg_sell_hero_chip_request(const network::tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_sell_hero_chip_request);
	auto p_role = role_manager_t::find_role(uid);
	if (NULL == p_role)
	{
		log_error("[wys] role[%lu] NULL", uid);
		return false;
	}

	proto::client::gc_sell_hero_chip_reply reply;
	for (int32_t i = 0; i < msg.hero_chip_list_size(); ++i)
	{
		uint64_t item_uid = string_util_t::string_to_uint64(msg.hero_chip_list(i).uid());
		item_manager_t::decompose_item(p_role, item_uid, msg.hero_chip_list(i).num(), reply.mutable_rcd());
	}
	reply.set_reply_code(errcode_enum::error_ok);
	p_role->send_msg_to_client(op_cmd::gc_sell_hero_chip_reply, reply);
	return true;
}
