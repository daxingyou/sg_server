#include "interior_msg_handle.hpp"
#include "common/log_enum.hpp"
#include "interior_manager.hpp"
#include "GarrisonCitylist.pb.h"
#include "config/config_manager.hpp"
#include "hero/hero_manager.hpp"

USING_NS_NETWORK;
USING_NS_COMMON;

bool interior_msg_handle_t::init_msg_handler()
{
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_get_build_copper_request, handle_cg_get_build_copper_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_upgrade_interior_build_request, handle_cg_upgrade_interior_build_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_dispatch_task_request, handle_cg_dispatch_task_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_refresh_task_request, handle_cg_refresh_task_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_upgrade_task_star_level_request, handle_cg_upgrade_task_star_level_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_get_interior_reward_request, handle_cg_get_interior_reward_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_complete_interior_task_request, handle_cg_complete_interior_task_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_get_offline_reward_request, handle_cg_get_offline_reward_request);
	gate_msg_handler_t::m_gate_msg_handle_from_client.register_func(op_cmd::cg_instant_battle_request, handle_cg_instant_battle_request);
    return true;
}

bool interior_msg_handle_t::handle_cg_get_build_copper_request(const network::tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_get_build_copper_request);

	interior_manager_t::get_build_copper(uid);

	return true;
}

bool interior_msg_handle_t::handle_cg_upgrade_interior_build_request(const network::tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_upgrade_interior_build_request);

	interior_manager_t::upgrade_build(uid);

	return true;
}

bool interior_msg_handle_t::handle_cg_dispatch_task_request(const network::tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_dispatch_task_request);

	interior_manager_t::dispatch_task(uid, msg);

	return true;
}

bool interior_msg_handle_t::handle_cg_refresh_task_request(const network::tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_refresh_task_request);

	interior_manager_t::refresh_task(uid);

	return true;
}

bool interior_msg_handle_t::handle_cg_upgrade_task_star_level_request(const network::tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_upgrade_task_star_level_request);

	interior_manager_t::upgrade_task_star_level(uid);

	return true;
}

bool interior_msg_handle_t::handle_cg_get_interior_reward_request(const network::tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_get_interior_reward_request);

	interior_manager_t::get_interior_reward(uid);

	return true;
}

bool interior_msg_handle_t::handle_cg_complete_interior_task_request(const network::tcp_socket_ptr& s, const msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_complete_interior_task_request);

	interior_manager_t::complete_interior_task(uid);

	return true;
}


bool interior_msg_handle_t::handle_cg_get_offline_reward_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_get_offline_reward_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}
	Hades::Data::GarrisonCitylist* p_citylist_config = GET_CONFIG(GarrisonCitylist, p_role->get_interior_info()->get_garrison_city());
	if (NULL == p_citylist_config)
	{
		log_error("Garrison is null [%d]", p_role->get_interior_info()->get_garrison_city());
		return false;

	}
	// 客户端没有这个消息
	//proto::client::gc_get_offline_reward_reply reply;
	//proto::common::role_change_data * p_data = reply.mutable_rcd();
	//uint32_t non_auto_play_time = p_role->get_non_auto_play_time();
	//uint32_t minute = non_auto_play_time / 60;
	//if (minute == 0)
	//{
	//	reply.set_reply_code(1);
	//	p_role->send_msg_to_client(op_cmd::gc_get_offline_reward_reply, reply);
	//	return false;
	//}
	//reply.set_reply_code(0);
	//reply.set_copper(p_citylist_config->coin() * minute);
	//p_role->add_money(proto::common::MONEY_TYPE_COPPER, p_citylist_config->coin() * minute);

	//reply.set_exp(p_citylist_config->exp() * minute);
	///*ntf.set_map_id(p_role->get_map_tid());*/
	//p_role->add_exp(p_citylist_config->exp() * minute, true, p_data);
	//for (auto item_tid : p_citylist_config->item())
	//{
	//	item_ptr p_item = item_manager_t::add_item(p_role, item_tid, minute, log_enum::source_type_offline_reward_item);
	//	if (p_item != NULL)
	//	{
	//		proto::common::item_single* p_item_single = reply.add_item_list();
	//		p_item->peek_data(p_item_single);

	//		proto::common::item_data* p_item_data = p_data->mutable_item();
	//		proto::common::item_single* p_item_single1 = p_item_data->add_item_list();
	//		p_item->peek_data(p_item_single1);
	//	}
	//}

	//p_role->set_non_auto_play_time(non_auto_play_time % 60);
	//proto::common::personal_info* p_info = p_data->mutable_per_info();
	//p_role->get_personal_info(p_info);
	//p_role->send_msg_to_client(op_cmd::gc_get_offline_reward_reply, reply);
	return true;
}

bool interior_msg_handle_t::handle_cg_instant_battle_request(const network::tcp_socket_ptr& s, const network::msg_buf_ptr& msg_buf)
{
	PRE_S2S_MSG(proto::client::cg_instant_battle_request);
	role_ptr p_role = role_manager_t::find_role(uid);
	if (p_role == NULL)
	{
		log_error("p_role is null [%lu]", uid);
		return false;
	}
	interior_ptr p_interior = p_role->get_interior_info();
	if (NULL == p_interior)
	{
		log_error("p_interior is null [%lu]", uid);
		return false;
	}

	Hades::Data::GarrisonCitylist* p_citylist_config = GET_CONFIG(GarrisonCitylist, p_interior->get_garrison_city());
	if (NULL == p_citylist_config)
	{
		log_error("Garrison is null [%d]", p_interior->get_garrison_city());
		return false;

	}

	proto::client::gc_instant_battle_reply reply;
	proto::common::role_change_data * p_data = reply.mutable_rcd();
	reply.set_reply_code(0);
	reply.set_exp(p_citylist_config->exp() * 120);
	p_role->add_exp(p_citylist_config->exp() * 120, log_enum::source_type_interior_battle, true, p_data);

	/*uint32_t non_auto_play_time = p_role->get_non_auto_play_time();
	p_role->set_non_auto_play_time(120 * 60);*/
	interior_manager_t::notify_offline_reward_info(uid, true);
	// p_role->set_non_auto_play_time(non_auto_play_time);


	proto::common::personal_info* p_info = p_data->mutable_per_info();
	p_role->get_personal_info(p_info);

	p_role->send_msg_to_client(op_cmd::gc_instant_battle_reply, reply);

	return true;
}
